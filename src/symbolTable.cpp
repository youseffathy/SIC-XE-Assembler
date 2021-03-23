#include "symbolTable.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
using namespace std;
symbolTable::symbolTable()
{

}

bool symbolTable::addSymbol(string symbol, string hexAddress, string type)
{
    auto it = table.find(symbol);
    if(it != table.end() && (it->second).first != "")  // already exist
    {
        return false;
    }

    if(it != table.end() && (it->second).first == "")  //symbol was added with no address when found as operand
    {
        (it->second).first = hexAddress;
        (it->second).second = type;
    }
    if(symbol != "")// new symbol added
    {
        table.insert(make_pair(symbol, make_pair(hexAddress,type)));

    }
    return true;


}


string symbolTable:: getAddressOfsymbol(string symbol)
{
    auto it = table.find(symbol);

    if(it == table.end())
    {
        return "";
    }
    else if ((it->second).first == "")
    {
        return "-1";

    }
    return (it->second).first;
}

string symbolTable:: getTypeOfsymbol(string symbol)
{
    auto it = table.find(symbol);

    if(it == table.end())
    {
        return "";
    }
    else if ((it->second).first == "")
    {
        return "-1";

    }
    return (it->second).second;
}

