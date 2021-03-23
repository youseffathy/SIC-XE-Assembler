#include "FlagBits.h"
#include <regex>
FlagBits::FlagBits()
{
    generateTable();
}


int FlagBits::flagBitsNo (bool formatFour, string operand)
{
    bool indexed = false;
    regex arrayOper ("\([A-Z]+[A-Z0-9]*)[,][X]");
    smatch match;
    // to check if operand is indexed
    if(regex_match(operand, match, arrayOper))
    {
        indexed = true;
    }
    // Indirect addressing
    if(operand.find("@") == 0)
    {
        if(formatFour)
            return 8;
        return 9;
    }
    // Immediate addressing
	else if(operand.find("#") == 0)
    {
        if(formatFour)
            return 11;
        return 12;
    }
    // Simple addressing
    else
    {
        if(formatFour)
        {
            if(indexed)
                return 5;
            return 2;
        }
        if(indexed)
            return 6;
        return 3;
    }

    return -1;

}

void FlagBits::generateTable()
{
    table.insert(make_pair(1,"110000"));
    table.insert(make_pair(2,"110001"));
    table.insert(make_pair(3,"110010"));
    table.insert(make_pair(4,"111000"));
    table.insert(make_pair(5,"111001"));
    table.insert(make_pair(6,"111010"));
    table.insert(make_pair(7,"100000"));
    table.insert(make_pair(8,"100001"));
    table.insert(make_pair(9,"100010"));
    table.insert(make_pair(10,"010000"));
    table.insert(make_pair(11,"010001"));
    table.insert(make_pair(12,"010010"));
}

string FlagBits::getFlagBits(int i)
{
    return table[i];
}
