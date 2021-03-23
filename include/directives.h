#ifndef DIRECTIVES_H
#define DIRECTIVES_H
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
using namespace std;

class directives
{
    public:
        directives();
        int checkAndGetNumOfBytes(string directive, string value);
        bool checkIfDirective(string direc);

    protected:

    private:


};

#endif // DIRECTIVES_H
