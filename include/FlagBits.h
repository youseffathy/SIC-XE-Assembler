#ifndef FLAGBITS_H
#define FLAGBITS_H
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include "operationTable.h"
#include "directives.h"
#include "symbolTable.h"
using namespace std;

class FlagBits
{
    public:
        FlagBits();
        int flagBitsNo (bool formatFour, string operand);
        string getFlagBits(int i);

    protected:

    private:
        unordered_map<int, string> table;
        void generateTable();
};

#endif // FLAGBITS_H
