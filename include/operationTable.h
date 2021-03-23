#ifndef OPERATIONTABLE_H
#define OPERATIONTABLE_H
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>

using namespace std;
class operationTable
{
    public:

        operationTable();
        pair<int, string> getOperation(string operation);
        bool checkIfOperation(string operation); // need to be implemented


    protected:

    private:
        unordered_map<string, pair<int, string>> table;
        void generateOperationTable();
};

#endif // OPERATIONTABLE_H
