#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
using namespace std;

class symbolTable
{
public:
    symbolTable();
    bool addSymbol(string symbol, string hexAddress, string type);
    string getAddressOfsymbol(string symbol);
    unordered_map<string, pair<string,string>> table;
    string getTypeOfsymbol(string symbol);

protected:

private:

};

#endif // SYMBOLTABLE_H
