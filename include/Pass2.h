#ifndef PASS2_H
#define PASS2_H
#include <iostream>
#include <algorithm>
#include <string>
#include "operationTable.h"
#include "directives.h"
#include "symbolTable.h"

using namespace std;

class Pass2
{
    public:
        Pass2();
        vector<pair<string, string>>getObjectCodes();
        void findObjectCodes();
        void setInstructions(vector<vector<string> > myInstructions);
        void setSymbolTable(symbolTable mySymTable);
        void setAddresses(vector<int> address);
        void setStartAddress(int SA);
        int getStartAddress();
        string getStartSymbol();
        void setStartSymbol(string SS);

    protected:

    private:

        string getFirst6Bits(string operation);

};

#endif // PASS2_H
