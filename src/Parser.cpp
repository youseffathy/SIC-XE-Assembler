#include <iostream>
#include <vector>
#include <regex>
#include "Parser.h"
#include "operationTable.h"
#include "directives.h"
#include "symbolTable.h"
#include <string>
using namespace std;

bool f = false;
string registersNames[5] = {"A","X","T","S","F"};
vector<string> registers;
string errorLine = "";
vector<pair<int, string>> symbolChecker;
int counter = 1;
operationTable optable;
symbolTable symtable;
directives direc;
bool moreThan3 ;
pair<int, string> temp = make_pair(-1, "");
string opSymbol;
vector<string> express;
int add1;
int add2;
string type;
string operation;
bool isExpression;
string exp11;
string exp22;



bool checkIfExpression(bool prevIdentified);
int intFromString2(string addr);


Parser::Parser()
{

}

string Parser:: getExp11()
{
    return exp11;
}

string Parser:: getExp22()
{
    return exp22;
}
bool Parser::isExpr()
{
    return isExpression;
}
bool Parser::isComment(string line)
{

    regex comment("\\s*[\.].*");
    smatch match;
    if (regex_match(line, match, comment) == 0)
    {
        f = false;
        return false;
    }
    else
    {
        f = true;
        return true;
    }
}


vector<string>Parser::parse(string line)
{
    express.clear();
    exp11 = "";
    exp22 = "";
    isExpression = false;
    opSymbol = "";
    errorLine = "";
    moreThan3 = false;
    temp.first = counter;
    temp.second = "";
    add1 = 0;
    add2 = 0;
    type = "";
    operation = "";


    line = " " + line;
    regex rgx("^\\s*?(\[\\S]*)?\\s*(\[\\S]*)\\s*(\[\\S]*||[C]['][^']{0,15}[']||[X]['][A-F0-9]{1,14}['])\\s*$");
    regex comment("^\\s*?([\.].*)\\s*$");
    smatch match;
    string symbol;
    string mnemonic;
    string operand;
    string comment1 = "";
    symbol = "";
    mnemonic = "";
    operand = "";


    if (regex_search(line, match, comment) ||regex_search(line,match,rgx))
    {
        symbol = match.str(1);
        mnemonic = match.str(2);
        operand = match.str(3);
        if(isComment(line))
        {
            symbol = "";
            mnemonic = "";
            operand = "";
            comment1 = match.str(0);

        }
    }
    else if (!(regex_search(line, match, comment)))
    {
        moreThan3 = true;
    }
    else
    {
        symbol = "";
        mnemonic = "";
        operand = "";
    }




    vector<string> instruction;
    instruction.push_back(symbol);
    instruction.push_back(mnemonic);
    instruction.push_back(operand);
    instruction.push_back(comment1);
    if(!moreThan3)
    {
        checker(instruction);
    }
    else
    {
        errorLine = "syntax error";
    }
    counter++;
    registers.clear();

    instruction.push_back(errorLine);


    instructions.push_back(instruction);
    return instruction;

}

void Parser::updateSymbolTable(symbolTable updatedSymTable)
{
    symtable = updatedSymTable;
}

bool Parser::checker(vector<string> instruction)
{


    if(f)
    {
        return true;
    }
    //case 3_ s7 s7 Empty
    if(instruction[0] != "" && instruction[1] != "" && instruction[2] == "")
    {
        //error typed in errorline
        errorLine = "operand missing!";
        return false;
    }
    // added because we do not handle both directives
    if(instruction[1]  == "RSUB" || instruction[1] == "JSUB")
    {
        errorLine=  "mnemonic need an operand";
        return false;
    }
    //case 1_ Empty s7 Empty
    if(instruction[0] == "" && instruction[1] != "" && instruction[2] == "")
    {
        if(!(instruction[1] == "RSUB" || instruction[1] == "END" || instruction[1] == "NOBASE"))
        {
            if(optable.getOperation(instruction[1]).first != -1 || direc.checkIfDirective(instruction[1]))
            {
                errorLine=  "mnemonic need an operand";
                return false;
            }
            //error typed in errorline
            errorLine=  "wrong mnemonic";
            return false;

        }
    }







    //case 2 & 4
    else if( instruction[1] != "" && instruction[2] != "")
    {
        if(instruction[1] == "RSUB" || instruction[1] == "NOBASE")
        {
            //error typed in errorline
            errorLine= "wrong mnemonic";
            return false;
        }
        //check mnemonics which need registers operand
        else if(instruction[1] == "RMO" || instruction[1] == "ADDR" || instruction[1] == "SUBR" ||instruction[1] == "MULR" || instruction[1] == "DIVR"
                || instruction[1] == "COMPR" )
        {


            int num =  checkAddress(instruction[2], counter);

            if(num != 7)
            {
                errorLine= "missing or wrong register";
                return false;
            }

        }
        else if(instruction[1] == "TIXR")
        {
            if(checkAddress(instruction[2],  counter) != 6)
            {
                //error typed in errorline
                errorLine =  "missing or wrong register";
                return false;
            }
        }

        else if(optable.getOperation(instruction[1]).first != -1) // check wrong operation
        {

            int num = checkAddress(instruction[2], counter);


            //checking operand
            if(num == 9)
            {
                if(!checkIfExpression(false))
                {
                    return false;
                }
            }
            else if(num == -1 || num == 3 || num == 6 || num == 7 )
            {
                //error typed in errorline
                errorLine =  "wrong operand";
                return false;
            }


        }
        else if(direc.checkIfDirective(instruction[1]))
        {
            int num = checkAddress(instruction[2], counter);
            if(instruction[1] == "START" || instruction[1] == "END")
            {



                //checking operand
                if( !(num == 1 || num == 2 ) )
                {
                    //error typed in errorline
                    errorLine =   "wrong address";
                    return false;
                }
            }
            else if(instruction[1] == "BASE")
            {
                if(num == 9)
                {
                    if(!checkIfExpression(false))
                    {
                        return false;
                    }

                }
                else if( !(num == 1 || num == 2 || 5) )
                {
                    //error typed in errorline
                    errorLine =   "wrong address";
                    return false;
                }

            }
            else if (instruction[1] == "ORG" )
            {
                if(num == 9)
                {
                    bool f = checkIfExpression(true);
                    if(!f ||(f && type == "absolute"))
                    {
                        return false;
                    }
                }
                else if(num != 2 && num != 5)
                {
                    errorLine =  "wrong operand";
                    return false;
                }
                else
                {
                    string hexAdd = symtable.getAddressOfsymbol(opSymbol);
                    if(hexAdd == "" || hexAdd == "-1")
                    {
                        errorLine =  "wrong or not previously identified symbol";
                        return false;

                    }
                    if(symtable.getTypeOfsymbol(opSymbol) == "absolute")
                    {
                        errorLine =  "can not use absolute address with ORG";
                        return false;
                    }


                }


            }
            else if(instruction[1] == "EQU")
            {



                if(num == 2 || num == 5)
                {
                    string hexAdd = symtable.getAddressOfsymbol(opSymbol);
                    if(hexAdd == "" || hexAdd == "-1")
                    {

                        errorLine =  "wrong or not previously identified symbol";
                        return false;

                    }
                }
                else if (num == 9 ) //simple expression
                {
                    if(!checkIfExpression(true))
                    {
                        return false;
                    }
                }
                else if (num != 8)
                {

                    errorLine =  "wrong or not previously identified operand";
                    return false;
                }

            }
            else if(instruction[1] == "BYTE" )
            {

                if(num != 3)
                {
                    //error typed in errorline
                    errorLine =   "wrong operand";
                    return false;

                }

            }
            else
            {
                if(!(instruction[2].find_first_not_of("0123456789") == std::string::npos))
                {
                    //error typed in errorline
                    errorLine = "wrong operand";
                    return false;
                }


            }
        }
        else
        {
            //error typed in errorline
            errorLine =  "wrong mnemonic";
            return false;
        }

        if(instruction[0] != "")
        {
            string symb = symtable.getAddressOfsymbol(instruction[0]);
            if(instruction[1] == "NOBASE" || instruction[1] == "RSUB" || instruction[1] == "END" || instruction[1] == "ORG" ||instruction[1] == "BASE" )
            {
                //error typed in errorline
                errorLine =   "unwanted symbol";
                return false;
            }
            if(symb != "" && symb != "-1")
            {
                errorLine =   "symbol already used";
                return false;

            }
        }
        if(instruction[1] == "EQU" && instruction[0] == "")
        {
            errorLine =   "missing symbol";
            return false;
        }
    }

    return true;

}


bool Parser::checkRegister(string reg1)
{
    for(int i = 0; i < 5; ++i)
    {
        if(registersNames[i] == reg1)
        {
            return true;
        }
    }
    return false;
}

int  Parser::checkAddress(string operand, int counter)
{
    if(optable.getOperation(operand).first != -1 || direc.checkIfDirective(operand))
    {
        return -1;
    }
    // temp to ad symbol if exist
    regex address ("\[A-F0-9]{4}");
    regex symbol ("(\[@#]?)(\[A-Z]+[A-Z0-9]*)");
    regex num ("\[#][0-9]{0,4}");
    regex byteOper ("\([C]['][^']{0,15}[']||[X]['][A-F0-9]{1,14}['])");
    regex arrayOper ("\([A-Z]+[A-Z0-9]*)[,][X]");
    regex oneReg ("\([ASXTF])");
    regex twoReg ("\([ASXTF])[,]([ASXTF])");
    regex deciOrHexa ("\[=]([0-9]{1,4})");
    regex expression ("\([\\S]+)([/*-+])([\\S]+)");
    smatch match;
    if(regex_match(operand, match, address))
    {
        return 1;
    }
    if(regex_match(operand, match, symbol))
    {
        if(operand[0] == '#' || operand[0] == '@' ) operand.erase(operand.begin());
        temp = make_pair(counter, operand);
        symbolChecker.push_back(temp);
        opSymbol = operand;
        temp = make_pair(-1, "");
        return 2;
    }
    if(regex_match(operand, match, byteOper))
    {
        return 3;
    }
    if(regex_match(operand, match, num))
    {
        return 4;
    }
     if(regex_match(operand, match, oneReg))
    {
        string reg1 = match.str(1);
        registers.push_back(reg1);
        return 6;
    }
    if(regex_match(operand, match, twoReg))
    {

        string reg1 = match.str(1);
        string reg2 = match.str(2);
        registers.push_back(reg1);
        registers.push_back(reg2);
        return 7;
    }
    if(regex_match(operand, match, arrayOper))
    {
        string sy = match.str(1);
        temp = make_pair(counter, sy);
        symbolChecker.push_back(temp);
        opSymbol = sy;
        temp = make_pair(-1, "");
        return 5;
    }

    if(regex_match(operand, match, deciOrHexa))
    {
        return 8;
    }
    if(regex_match(operand, match, expression))
    {
        string op1 = match.str(1);
        string operation = match.str(2);
        string op2 = match.str(3);
        express.push_back(op1);
        express.push_back(operation);
        express.push_back(op2);
        return 9;
    }

    return -1;



}

string Parser::getSymbolInOperand()
{
    return opSymbol;
}

bool checkSymbol(string symb)
{

    if(symtable.getAddressOfsymbol(symb) == "") return true;
    return false;
}

int intFromString2(string addr)
{
    istringstream iss(addr);
    int num;
    iss >> num;

    return num;

}


int Parser::getAdd1()
{
    return add1;
}
int Parser::getAdd2()
{
    return add2;
}
string Parser::getoperation()
{
    return operation;
}

string Parser::getType()
{
    return type;
}


bool checkIfExpression(bool prevIdentified)
{
    bool f1 = true;
    bool f2 = true;
    string temp1 = symtable.getTypeOfsymbol(express[0]);
    string temp2 = symtable.getTypeOfsymbol(express[2]);
    string op1;
    string op2;


    //knowing type of the first operand
    if(express[0].find_first_not_of("0123456789") == std::string::npos )
    {
        f1 = false;
        op1 = "A";
        add1 = intFromString2(express[0]);


    }
    else if (temp1 == "absolute")
    {
        op1 = "A";
        add1 = intFromString2(symtable.getAddressOfsymbol(express[0]));
    }
    else if(temp1 == "relocatable")
    {
        op1 = "R";
        add1 = intFromString2(symtable.getAddressOfsymbol(express[0]));
    }
    else if(prevIdentified)
    {
        errorLine =  "invalid expression";
        return false;
    }
    if(f1) exp11 = express[0];


    //knowing type of the second operand
    if(express[2].find_first_not_of("0123456789") == std::string::npos )
    {
        f2 = false;
        op2 = "A";
        add2 = intFromString2(express[2]);


    }
    else if (temp2 == "absolute")
    {
        op2 = "A";
        add2 = intFromString2(symtable.getAddressOfsymbol(express[2]));
    }
    else if(temp2 == "relocatable")
    {
        op2 = "R";
        add2 = intFromString2(symtable.getAddressOfsymbol(express[2]));
    }
    else if(prevIdentified)
    {

        errorLine =  "invalid expression";
        return false;
    }
    if(f2) exp22 = express[2];

    // checking valid operation
    if(!(express[1] == "+" || express[1] == "-" || express[1] == "*"|| express[1] == "/"))
    {

        errorLine =  "invalid expression";
        return false;
    }
    operation = express[1];

    // case oF operandS
    if(op1 == "A" && op2 == "A")
    {
        type = "absolute";
    }
    else if(op1 == "A" && op2 == "R")
    {
        if(express[1] != "+")
        {
            errorLine =  "invalid expression";
            return false;
        }
        type = "relocatable";
    }
    else if(op1 == "R" && op2 == "A")
    {
        if(express[1] == "*" || express[1] == "/")
        {
            errorLine =  "invalid expression";
            return false;
        }
        type = "relocatable";

    }
    else if(op1 == "R" && op2 == "R")
    {
        if(express[1] != "-")
        {
            errorLine =  "invalid expression";
            return false;
        }
        type = "absolute";

    }
    isExpression = true;
    return true;

}
