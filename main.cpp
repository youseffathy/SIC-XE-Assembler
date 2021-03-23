#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "TargetAddress.h"
#include <string>
#include <iterator>
#include "operationTable.h"
#include "directives.h"
#include "symbolTable.h"
#include "ObjectProgramHandler.h"
#include "Pass2.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include "Parser.h"
#include <regex>

using namespace std;

int dicemalFromHex(string hex);
string addressInHexaFromDicemal(int address);
string formateAddress(string hexa);
string stringFromInteger(int address);
int calculateAddress(int add1, int add2, string operation);
int intFromString3(string addr);
vector<int>address;
vector<vector<string>> instructions;


int main()
{
    int SA = 0;
    bool isStart = false;
    string headerName = "";
    bool ended = false;
    bool started = false;
    int counter = 0;
    string input;
    int locctr = 0 ;
    vector<string> instruction;
    Parser parser;
    operationTable opTable;
    symbolTable symTable;
    directives direc;
    string fileName = "prog.txt";

    cout << "Enter pass1 then file name not separated by space :" << endl;
    cin >> input;
    regex user("(\[p][a][s][s][1])(.+)");
    smatch match;

    if(regex_match(input, match, user))
    {
        input = match.str(1);
    }
    else
    {
        cout << "no pass1 entered !!" << endl;
        cout << "click any button to finish" << endl;
        string temp;
        cin >> temp;
        return 0;
    }



    fileName = match.str(2);

    ifstream file;
    file.open(fileName);
    string line;
    if(!file.is_open())
    {
        cout << "no file exists with that name !!" << endl;
        cout << "click any button to finish" << endl;
        string temp;
        cin >> temp;
        return 0;

    }
    while (getline(file, line))
    {


        parser.updateSymbolTable(symTable);
        instruction = parser.parse(line);



        if (instruction[0] == "" && instruction[1] == "" && instruction[2] == "" && instruction[3] == "" && instruction[4] == "" )// empty line
        {
            continue;
        }
        instructions.push_back(instruction);

        if(instruction[3] != "") // case 1 : comment
        {

            if(started == false || ended)
            {
                address.push_back(-2); // before first instruction
            }
            else
            {

                address.push_back(locctr);// after first instruction
            }

        }
        else if(ended)
        {
            instruction[0] = "";
            instruction[1] = "";
            instruction[2] = "";
            instruction[3] = "";
            instruction[4] = "cannot writing instruction after ending program";
            address.push_back(locctr);
            instructions.push_back(instruction);
            address.push_back(-1);


        }
        else if(instruction[4] != "")
        {


            address.push_back(locctr);
            instructions.push_back(instruction);
            address.push_back(-1);


        }
        else if (started && instruction[1] == "START")
        {
            address.push_back(locctr);
            address.push_back(-1);
            instruction[4] = "you can use START at the first line only ";
            instructions.push_back(instruction);

        }
        else
        {

            if(!started && instruction[1] == "START")
            {
                isStart = true;
                locctr = dicemalFromHex(instruction[2]);
                SA = locctr;
                headerName = instruction[0];
                address.push_back(locctr);
                symTable.addSymbol(instruction[0], stringFromInteger(locctr), "relocatable");
                started = true;
                continue;
            }

            started = true;

            pair<int, string> temp = opTable.getOperation(instruction[1]);
            int numOfByte = direc.checkAndGetNumOfBytes(instruction[1], instruction[2]);

            if(temp.first != -1)
            {
                symTable.addSymbol(instruction[0], stringFromInteger(locctr), "relocatable");
                symTable.addSymbol(parser.getExp11(), "", "");
                symTable.addSymbol(parser.getExp22(), "", "");
                symTable.addSymbol(parser.getSymbolInOperand(), "", "");
                address.push_back(locctr);
                locctr = locctr +  temp.first;
            }
            else if(numOfByte != -1)
            {
                symTable.addSymbol(instruction[0], stringFromInteger(locctr),  "relocatable");
                symTable.addSymbol(parser.getSymbolInOperand(), "", "");
                address.push_back(locctr);
                locctr += numOfByte;

            }
            else if (instruction[1] == "ORG")
            {

                address.push_back(locctr);
                if(parser.isExpr())
                {
                    locctr = calculateAddress( parser.getAdd1(),  parser.getAdd2(),parser.getoperation() );
                }
                else
                {
                    locctr = intFromString3(symTable.getAddressOfsymbol(parser.getSymbolInOperand()));

                }



            }
            else if(instruction[1] == "EQU")
            {
                address.push_back(locctr);
                // check if absolute(hexa address) or relocatable(symbol)
                string operandSymbol = parser.getSymbolInOperand();
                if(operandSymbol != "") //case of symbol
                {
                    symTable.addSymbol(instruction[0], symTable.getAddressOfsymbol(operandSymbol),  "relocatable");
                }
                else if(instruction[2][0] == '=') // case =(integer)
                {

                    if(instruction[2][0] == '=')operandSymbol = instruction[2];

                    operandSymbol.erase(operandSymbol.begin());
                    int n = atoi( operandSymbol.c_str() );
                    symTable.addSymbol(instruction[0], stringFromInteger(n),  "absolute");
                }
                else
                {

                    string type = parser.getType();
                    int resultAdd = calculateAddress( parser.getAdd1(),  parser.getAdd2(),parser.getoperation() );
                    symTable.addSymbol(instruction[0], stringFromInteger(resultAdd),type);
                }


            }
            else if(instruction[1] == "BASE" )
            {
                address.push_back(locctr);
                symTable.addSymbol(parser.getExp11(), "", "");
                symTable.addSymbol(parser.getExp22(), "", "");
                symTable.addSymbol(parser.getSymbolInOperand(), "", "");
            }
            else if( instruction[1] == "NOBASE")
            {
                address.push_back(locctr);
            }
            else if(instruction[1] == "END")
            {
                ended = true;
                address.push_back(locctr);
            }
        }
    }



    file.close();
    if(instruction[1] != "END" && instruction[1] != "")
    {
        address.push_back(-1);
        instruction[0] = "";
        instruction[1] = "";
        instruction[2] = "";
        instruction[3] = "";
        instruction[4] = "missing END instruction at end of your program";
        instructions.push_back(instruction);

    }




    if(instructions.size() != address.size()) return 0;

    ofstream fout;
    fout.open("result.txt");
    int deleted = 0;
    if(fout.is_open())
    {

        fout.width(20);
        fout << left << "Line no.";
        fout.width(20);
        fout << left << "Address";
        fout.width(20);
        fout << left << "Label";
        fout.width(20);
        fout << left << "Op-Code";
        fout.width(20);
        fout << left << "Operands"<< endl;

        for(int i = 0; i < instructions.size() ; ++i)
        {

            std::stringstream ssssss;

            if(address.at(i) == -1)
            {
                fout.width(20);
                fout << left << "";
                fout.width(20);
                fout << left << "";
                fout.width(35);
                fout<< left << instructions[i][4] << endl;
                deleted++;

            }
            else if (instructions[i][3] != "")
            {

                ssssss << (i - deleted + 1);
                fout.width(20);
                fout<< left << ssssss.str();
                if (address.at(i) != -2)
                {
                    fout.width(20);
                    fout << left << addressInHexaFromDicemal(address.at(i));
                }
                fout.width(20);
                fout << left << "";
                fout.width(35);
                fout<< left << instructions[i][3] << endl;

            }
            else
            {

                ssssss << (i - deleted +1);
                fout.width(20);
                fout << left << ssssss.str();
                fout.width(20);
                fout << left << addressInHexaFromDicemal(address.at(i));
                fout.width(20);
                fout << left << instructions[i][0];
                fout.width(20);
                fout << left << instructions[i][1];
                fout.width(20);
                fout << left << instructions[i][2] << endl;
            }


        }

        fout << "-----------------------------------------------------------------------" << endl;
        fout.width(20);
        fout << left << "Symbol";
        fout.width(20);
        fout << left << "Address";
        fout.width(20);
        fout << left << "Type" << endl;


        auto it = symTable.table.begin();

        while (it != symTable.table.end())
        {
            fout.width(20);
            fout << left << it->first;
            fout.width(20);
            fout << left << (it->second).first;
            fout.width(20);
            fout << left << (it->second).second << endl;
            it++;
        }
        fout.close();
    }



    cout << "     " << endl;
    cout << "Look for file named \"result.txt\"!" << endl;




    // we need to take input to make pass 2

    //pass 2

    //checking for error
    for(int i = 0; i < instructions.size(); ++i)
    {
        if(instructions[i][4] != "")
        {
            cout << "can not perform pass 2 due to error(s)" << endl;
            cout << "click any button to finish" << endl;
            string temp;
            cin >> temp;

            return 0;
        }
    }

    // checking for  symbol with no address
    for(auto it = (symTable.table).begin(); it != (symTable.table).end(); ++it)
    {
        if((it->second).first == "")
        {
            cout << "can not perform pass 2 due to symbols(s) with no addresses "<< endl;
            cout << "click any button to finish" << endl;
            string temp;
            cin >> temp;
            return 0;
        }
    }

    cout << "pass2" << endl;
    Pass2 p2;
    p2.setAddresses(address);
    p2.setInstructions(instructions);
    p2.setStartAddress(SA);
    p2.setSymbolTable(symTable);
    p2.findObjectCodes();
    vector<pair<string, string> > obCodes = p2.getObjectCodes();
    for(int i = 0; i < obCodes.size(); ++i)
    {
        cout << obCodes.at(i).first << " " << obCodes.at(i).second << endl;
    }

    if(obCodes.size() == 0)
    {
        cout << "ERROR IN PASS 2" << endl;
        cout << "click any button to finish" << endl;
        string temp;
        cin >> temp;
        return 0;

    }



    ObjectProgramHandler writer = ObjectProgramHandler(obCodes,headerName,isStart);
    writer.generateObjectCode();
    writer.writeInFile();


    return 0;
}


int dicemalFromHex(string hexa)
{
    int intValue;
    stringstream stream;
    stream << hex << hexa;
    stream >> intValue;
    return intValue;
}
string addressInHexaFromDicemal(int address)
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
string stringFromInteger(int address)
{

    stringstream ss;
    ss << address;
    return ss.str();
}

int calculateAddress(int add1, int add2, string operation)
{
    int resultAdd;
    if(operation == "+")
    {

        resultAdd = add1 + add2;
    }
    else if(operation == "-")
    {

        resultAdd = add1 - add2;
    }
    else if (operation == "*")
    {

        resultAdd = add1 * add2;
    }
    else if (operation == "/")
    {

        resultAdd = add1 / add2;
    }

    return resultAdd;

}
int intFromString3(string addr)
{
    istringstream iss(addr);
    int num;
    iss >> num;

    return num;

}
