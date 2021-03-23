#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "operationTable.h"
#include "directives.h"
#include "symbolTable.h"
using namespace std;
class Parser
{
    public:

        Parser();
        vector<string> parse(string line);
        bool isComment(string line);
        void updateSymbolTable(symbolTable updatedSymTable);
        string getSymbolInOperand();
        int getAdd1();
        int getAdd2();
        string getoperation();
        string getType();
        string getExp11();
        string getExp22();
        bool isExpr();


    protected:

    private:

        bool checker(vector<string> instruction);

        vector<vector<string>> instructions;
        bool checkRegister(string reg1);
        int checkAddress(string operand, int counter);

};

#endif // PARSER_H
