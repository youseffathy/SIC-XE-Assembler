#ifndef TARGETADDRESS_H
#define TARGETADDRESS_H
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "symbolTable.h"
using namespace std;
class TargetAddress
{
    public:
        TargetAddress();
        string getAddress(string operand);
        void setSymbolTable(symbolTable mySymTable);


    protected:

    private:

};

#endif // TARGETADDRESS_H
