#include "TargetAddress.h"
#include "symbolTable.h"
#include <regex>
#include <string>
#include <unordered_map>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>


using namespace std;

symbolTable symbTable;
string stringFromInteger3(int address);
int dicemalFromHex2(string hex);
int getTA();
int intFromStr(string addr);
vector<string> express2;
int add11;
int add22;

TargetAddress::TargetAddress()
{


}
void TargetAddress::setSymbolTable(symbolTable mySymTable)
{
    symbTable = mySymTable;
}
string TargetAddress::getAddress(string operand)
{
    express2.clear();

    regex symbol ("(\[@#]?)(\[A-Z]+[A-Z0-9]*)");
    regex num ("\[#][0-9]{0,4}");
    regex arrayOper ("\([A-Z]+[A-Z0-9]*)(\[,][X])");
    regex address ("\[A-F0-9]{4}");
    regex expression ("\([\\S]+)([/*-+])([\\S]+)");
    smatch match ;
    string TA;

    if (regex_match(operand, match, symbol))
    {
        auto it = symbTable.table.find(match.str(2));
        TA = (it->second).first;
    }
    else if (regex_match(operand, match, num))
    {
        operand.erase(operand.begin());
        TA = operand;
    }
    else if (regex_match(operand, match, arrayOper))
    {
        auto it = symbTable.table.find(match.str(1));
        TA = (it->second).first;
    }
    else if (regex_match(operand, match, address))
    {
        TA = stringFromInteger3(dicemalFromHex2(operand));
    }
    else if (regex_match(operand, match, expression))
    {
        string op1 = match.str(1);
        string operation = match.str(2);
        string op2 = match.str(3);
        express2.push_back(op1);
        express2.push_back(operation);
        express2.push_back(op2);
        TA = stringFromInteger3(getTA());
    }

    else
    {
        TA = "";
    }
    return TA;
}

int dicemalFromHex2(string hexa)
{
    int intValue;
    stringstream stream;
    stream << hex << hexa;
    stream >> intValue;
    return intValue;
}
string stringFromInteger3(int address)
{

    stringstream ss;
    ss << address;
    return ss.str();
}


int getTA()
{

    string temp1 = symbTable.getTypeOfsymbol(express2[0]);
    string temp2 = symbTable.getTypeOfsymbol(express2[2]);

    //knowing type of the first operand
    if(express2[0].find_first_not_of("0123456789") == std::string::npos )
    {
        add11 = intFromStr(express2[0]);
    }
    else
    {
        add11 = intFromStr(symbTable.getAddressOfsymbol(express2[0]));
    }


    //knowing type of the second operand
    if(express2[2].find_first_not_of("0123456789") == std::string::npos )
    {

        add22 = intFromStr(express2[2]);
    }
    else
    {

        add22 = intFromStr(symbTable.getAddressOfsymbol(express2[2]));
    }


    int resultAdd;
    string operation = express2[1];
    if(operation == "+")
    {
        cout << 1 ;
        resultAdd = add11 + add22;
    }
    else if(operation == "-")
    {
        cout << 2 ;
        resultAdd = add11 - add22;
    }
    else if (operation == "*")
    {
        cout << 3 ;
        resultAdd = add11 * add22;
    }
    else if (operation == "/")
    {
        cout << 4 ;
        resultAdd = add11 / add22;
    }

    return resultAdd;


}
int intFromStr(string addr)
{
    istringstream iss(addr);
    int num;
    iss >> num;

    return num;

}


