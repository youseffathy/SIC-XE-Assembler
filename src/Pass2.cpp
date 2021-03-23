#include "Pass2.h"
#include "operationTable.h"
#include "directives.h"
#include "symbolTable.h"
#include "FlagBits.h"
#include "TargetAddress.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <iterator>
#include <regex>

using namespace std;

string startSymbol;// used in writing object code in file
vector<pair<string, string>>objectCodes;
vector<vector<string> > program;
vector<int> addresses;
symbolTable symTable;
operationTable opTabele;
directives directiveObj;
bool base = false;
int baseAddress;
int startAdress  = -1; // used in writing object code in file
FlagBits flagbits;
TargetAddress targetAddress;
bool format4 = false;

int intFromString(string addr);
string checkIfdirective(string directive, string operand);
int IntegerFromString(string operand);
string addressInHexaFromDicemal2(int address);
string hexaToBinary(char hexaNum);
string binaryToHexa(string binary);
string getRegisterValue(string R);
string hex(string binary);
string addressInHexaFromDicemal3(int address);


Pass2::Pass2()
{

}
void Pass2::setInstructions(vector<vector<string> > myInstructions)
{
    program = myInstructions;
}

void Pass2::setSymbolTable(symbolTable mySymTable)
{
    symTable = mySymTable;
    targetAddress.setSymbolTable(symTable);
}


vector<pair<string, string> >Pass2::getObjectCodes() // first = object code , second = location counter
{

    return objectCodes;
}

void Pass2::setAddresses(vector<int> address)
{
    addresses = address;
}

void Pass2::setStartAddress(int SA)
{
    startAdress = SA;
}


 void Pass2::setStartSymbol(string SS)
 {
    startSymbol = SS;
 }

int Pass2::getStartAddress()
{
    return startAdress;
}
string Pass2:: getStartSymbol()
{
    return startSymbol;
}







void Pass2::findObjectCodes()
{

    // get obCode for every instruction
    for(int i = 0; i < program.size(); ++i)
    {


        if(program[i][3] != "" || program[i][1] == "RESB" || program[i][1] == "RESW")continue;
        string obCode = "";


        obCode = checkIfdirective(program[i][1], program[i][2]);

        if(obCode != "" && obCode != " ") // it is directive
        {
            objectCodes.push_back(make_pair(obCode,addressInHexaFromDicemal3(addresses.at(i))));
            continue;

        }
        if(obCode == " ")continue;
        // it is operation

        pair<int, string> op = opTabele.getOperation(program[i][1]);
        if(op.first == -1 ) continue;


        if(op.first == 2) // register to register instruction (4 hexadecimal number object code ...format 2)
        {
            obCode = op.second;
            regex oneReg ("\([ASXTF])");
            regex twoReg ("\([ASXTF])[,]([ASXTF])");
            smatch match;
            if(regex_match(program[i][2], match, oneReg)) // operand is one register
            {
                string reg1 = match.str(1);
                obCode +=  getRegisterValue(reg1) + "0";
            }
            else if(regex_match(program[i][2], match, twoReg)) // operand is two register
            {
                string reg1 = match.str(1);
                string reg2 = match.str(2);
                obCode +=  getRegisterValue(reg1) + getRegisterValue(reg2);

            }
             objectCodes.push_back(make_pair(obCode,addressInHexaFromDicemal3(addresses.at(i))));
            continue;
        }

        // check if format3 or 4
        if(program[i][1][0] == '+')
        {
            format4 = true;
        }
        else
        {
            format4 = false;
        }
        // get first 6 bits
        obCode +=  getFirst6Bits(op.second);
        // get second 6 bits
        int x = flagbits.flagBitsNo(format4, program[i][2]);
        obCode += flagbits.getFlagBits(x);


        // calculating targetAddress
        int TA = intFromString(targetAddress.getAddress(program[i][2]));

        if(format4)
        {

            obCode += bitset<20>(TA).to_string();
            obCode = binaryToHexa(obCode);
             objectCodes.push_back(make_pair(obCode,addressInHexaFromDicemal3(addresses.at(i))));
            continue;

        }

        int  displacement = TA - addresses[i + 1];

        cout << "disp = " << displacement << endl;

        if(displacement <= -2048 ||displacement >= 2047) // base relative
        {
            if(base)
            {

                displacement = TA - baseAddress;
                if(displacement < 0 || displacement >= 4095)
                {
                    objectCodes.clear();
                    return;
                }
                obCode[9] = '1';
                obCode[10] = '0';

            }
            else
            {
                objectCodes.clear();
                return;
            }

        }

        obCode += bitset<12>(displacement).to_string();

        obCode  = binaryToHexa(obCode);
        objectCodes.push_back(make_pair(obCode,addressInHexaFromDicemal3(addresses.at(i))));


    }



}

string Pass2::getFirst6Bits(string opObCode)
{

    string opCodeInBinary = hexaToBinary(opObCode[0]) + hexaToBinary(opObCode[1]);
    opCodeInBinary.erase(opCodeInBinary.end() - 1);
    opCodeInBinary.erase(opCodeInBinary.end() - 1);
    return opCodeInBinary;
}






string checkIfdirective(string directive, string operand)
{
    string TA = "";

    if(directive == "START" || directive == "END")
    {
        TA = addressInHexaFromDicemal2(startAdress);
        int TALen = TA.length();

        for(int i = 0; i < 6 - TALen; ++i) // to make it 3 bytes (hexa number)
        {

            TA = "0" + TA;
        }
    }
    else if(directive == "WORD")
    {
        TA = addressInHexaFromDicemal2(IntegerFromString(operand));

    }
    else if(directive == "BYTE")
    {
        char tpyeOfByte = operand[0]; // if X'' or W'' or C''
        operand.erase(operand.begin()); // removing(C OR W OR X)
        operand.erase(operand.begin()); // removing first '
        operand.erase(operand.end() - 1); // removing last '
        if(tpyeOfByte == 'X')
        {
            TA = operand;
        }
        else if(tpyeOfByte == 'W')
        {
            TA = addressInHexaFromDicemal2(IntegerFromString(operand));// convert string of integers to string of hexadecimal
        }
        else if(tpyeOfByte == 'C')
        {
            for(int i = 0; i < operand.length(); ++i)
            {
                TA += addressInHexaFromDicemal2(operand[i]); // convert every char to hexadecimal representation (char is send to function as ascii decimal representation)
            }

        }
    }
    else if (directive == "NOBASE")
    {
        base = false;
        TA = " ";
    }
    else if (directive == "BASE")
    {
        base = true;
        baseAddress = intFromString(targetAddress.getAddress(operand));
        TA = " ";

    }
    else if(directive == "RESB" || directive == "RESW")
    {
        TA = " ";
    }

    return TA;
}

int IntegerFromString(string operand)
{
    stringstream convert(operand);
    int number = 0;
    convert >>number;
    return number;
}

string addressInHexaFromDicemal2(int address)
{
    stringstream sstream;
    sstream << std::hex << address;
    string hexa = sstream.str();
    std::string str = hexa;
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;

}
string hexaToBinary(char hexaNum)
{
    switch(hexaNum)
    {
    case '0':
        return "0000";
    case '1':
        return "0001";
    case '2':
        return "0010";
    case '3':
        return "0011";
    case '4':
        return "0100";
    case '5':
        return "0101";
    case '6':
        return "0110";
    case '7':
        return "0111";
    case '8':
        return "1000";
    case '9':
        return "1001";
    case 'A':
        return "1010";
    case 'B':
        return "1011";
    case 'C':
        return "1100";
    case 'D':
        return "1101";
    case 'E':
        return "1110";
    case 'F':
        return "1111";
    }
}

string getRegisterValue(string R)
{

    if(R == "A") return "0";
    if(R == "X") return "1";
    if(R == "L") return "2";
    if(R == "B") return "3";
    if(R == "S") return "4";
    if(R == "T") return "5";
    if(R == "F") return "6";

}

string binaryToHexa(string binary)
{
    string hexa = "";
    for(int i = 0; i < binary.length(); i += 4)
    {
        string sub = binary.substr(i, 4);
        hexa += hex(sub);

    }
    return hexa;
}
int intFromString(string addr)
{
    istringstream iss(addr);
    int num;
    iss >> num;

    return num;

}

string hex(string binary)
{
    if(binary == "0000")return "0";
    if(binary == "0001")return "1";
    if(binary == "0010")return "2";
    if(binary == "0011")return "3";
    if(binary == "0100")return "4";
    if(binary == "0101")return "5";
    if(binary == "0110")return "6";
    if(binary == "0111")return "7";
    if(binary == "1000")return "8";
    if(binary == "1001")return "9";
    if(binary == "1010")return "A";
    if(binary == "1011")return "B";
    if(binary == "1100")return "C";
    if(binary == "1101")return "D";
    if(binary == "1110")return "E";
    if(binary == "1111")return "F";

}
string addressInHexaFromDicemal3(int address)
{
   stringstream sstream;
    sstream << std::hex << address;
    string hexa = sstream.str();
    for(int i = 0; i < 6 - hexa.length(); ++i)
    {
        hexa = "0" + hexa;

    }
    std::string str = hexa;
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);

    return str;

}
