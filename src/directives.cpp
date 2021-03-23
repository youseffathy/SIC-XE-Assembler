#include "directives.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
using namespace std;

directives::directives()
{
    //ctor
}
string myDirectives [10] = {"START","END","BYTE","WORD","RESW","RESB","EQU","ORG","BASE","NOBASE"};
int directives ::checkAndGetNumOfBytes(string directive, string value)
{

    if(directive == "BYTE")
    {
        if(value[0] == 'C') return value.length() - 3;
        else if(value[0] == 'X')return (value.length() - 3) * 2;

    }
    else if(directive == "WORD" || directive == "RESW")
    {
        if(value.find_first_not_of("0123456789") == std::string::npos) return atoi(value.c_str()) * 3;
    }
    else if(directive == "RESB")
    {
        if(value.find_first_not_of("0123456789") == std::string::npos) return atoi(value.c_str());
    }

    return -1;
}
 bool directives :: checkIfDirective(string direc)
 {
     for(int i = 0; i < 10; ++i)
     {
         if(myDirectives[i] == direc)
         {
             return true;
         }
     }
     return false;
 }

