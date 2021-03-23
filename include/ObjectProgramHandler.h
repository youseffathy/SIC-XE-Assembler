#ifndef OBJECTPROGRAMHANDLER_H
#define OBJECTPROGRAMHANDLER_H
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;
class ObjectProgramHandler {
private:
    vector<pair<string , string> > table; // (object code: address)
    string name;
    string result;
    string zeros (int n);
    int hexToDecimal (string hex);
    string decimalToHex (int dec);
    bool startExist;
public:
    ObjectProgramHandler(vector<pair<string , string> > t, string n, bool withStart);
    void generateObjectCode ();
    void writeInFile ();
};

#endif // OBJECTPROGRAMHANDLER_H
