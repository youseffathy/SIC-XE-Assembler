#include "operationTable.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
using namespace std;

operationTable::operationTable()
{
    generateOperationTable();
}
 pair<int, string> operationTable::getOperation(string operation)
{
    //std::transform(operation.begin(), operation.end(),operation.begin(), ::toupper);

    bool formate4 = false;

    if(operation[0] == '+') // to handle formate 4
    {
        operation.erase(operation.begin());
        formate4 = true;
    }

    auto op = table.find(operation) ;

    if( op == table.end() || ((op->second).first != 3 && formate4))
    {
       return make_pair(-1,"");
    }
    if((op->second).first == 3 && formate4){
        return make_pair((op->second).first + 1, (op->second).second);
    }



   return op->second;
}


void operationTable::generateOperationTable()
{
    table.insert(make_pair("ADD", make_pair(3, "18")));
    table.insert(make_pair("ADDF", make_pair(3 , "58")));
    table.insert(make_pair("ADDR", make_pair(2, "90")));
    table.insert(make_pair("AND", make_pair(3, "40")));
    table.insert(make_pair("CLEAR", make_pair(2, "B4")));
    table.insert(make_pair("COMP", make_pair(3, "28")));
    table.insert(make_pair("COMPF", make_pair(3, "88")));
    table.insert(make_pair("COMPR", make_pair(2, "A0")));
    table.insert(make_pair("DIV", make_pair(3, "24")));
    table.insert(make_pair("DIVF", make_pair(3, "64")));
    table.insert(make_pair("DIVR", make_pair(2, "9C")));
    table.insert(make_pair("FIX", make_pair(1, "C4")));
    table.insert(make_pair("FLOAT", make_pair(1, "0")));
    table.insert(make_pair("HIO", make_pair(1, "F4")));
    table.insert(make_pair("J", make_pair(3, "3C")));
    table.insert(make_pair("JEQ", make_pair(3, "30")));
    table.insert(make_pair("JGT", make_pair(3, "34")));
    table.insert(make_pair("JLT", make_pair(3, "38")));
    table.insert(make_pair("JSUB", make_pair(3, "48")));
    table.insert(make_pair("LDA", make_pair(3, "00")));
    table.insert(make_pair("LDB", make_pair(3, "68")));
    table.insert(make_pair("LDCH", make_pair(3, "50")));
    table.insert(make_pair("LDF", make_pair(3, "70")));
    table.insert(make_pair("LDL", make_pair(3, "08")));
    table.insert(make_pair("LDS", make_pair(3, "6C")));
    table.insert(make_pair("LDT", make_pair(3, "74")));
    table.insert(make_pair("LDX", make_pair(3, "04")));
    table.insert(make_pair("LPS", make_pair(3, "D0")));
    table.insert(make_pair("MUL", make_pair(3, "20")));
    table.insert(make_pair("MULF", make_pair(3, "60")));
    table.insert(make_pair("MULR", make_pair(2, "98")));
    table.insert(make_pair("NORM", make_pair(1, "C8")));
    table.insert(make_pair("OR", make_pair(3, "44")));
    table.insert(make_pair("RD", make_pair(3, "D8")));
    table.insert(make_pair("RMO", make_pair(2, "AC")));
    table.insert(make_pair("RSUB", make_pair(3, "4C")));
    table.insert(make_pair("SHIFTL", make_pair(2, "A4")));
    table.insert(make_pair("SHIFTR", make_pair(2, "A8")));
    table.insert(make_pair("SIO", make_pair(1, "F0")));
    table.insert(make_pair("SSK", make_pair(3, "EC")));
    table.insert(make_pair("STA", make_pair(3, "0C")));
    table.insert(make_pair("STP", make_pair(3, "78")));
    table.insert(make_pair("STCH", make_pair(3, "54")));
    table.insert(make_pair("STF", make_pair(3, "80")));
    table.insert(make_pair("STI", make_pair(3, "D4")));
    table.insert(make_pair("STL", make_pair(3, "14")));
    table.insert(make_pair("STS", make_pair(3, "7C")));
    table.insert(make_pair("STSW", make_pair(3, "E8")));
    table.insert(make_pair("STT", make_pair(3, "84")));
    table.insert(make_pair("STX", make_pair(3, "10")));
    table.insert(make_pair("SUB", make_pair(3, "1C")));
    table.insert(make_pair("SUBF", make_pair(3, "5C")));
    table.insert(make_pair("SUBR", make_pair(2, "94")));
    table.insert(make_pair("SVC", make_pair(2, "B0")));
    table.insert(make_pair("TD", make_pair(3, "E0")));
    table.insert(make_pair("TIO", make_pair(1, "F8")));
    table.insert(make_pair("TIX", make_pair(3, "2C")));
    table.insert(make_pair("TIXR", make_pair(2, "B8")));
    table.insert(make_pair("WD", make_pair(3, "DC")));



}
