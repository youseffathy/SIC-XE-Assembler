#include "ObjectProgramHandler.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "ObjectProgramHandler.h"

using namespace std;

string ObjectProgramHandler::zeros (int n)
{
    string res = "";
    for (int i = 0; i < n; i++)
    {
        res += "0";
    }
    return res;
}
int ObjectProgramHandler::hexToDecimal (string hex)
{
    string hexStr = hex;
    unsigned int x;
    stringstream ss;
    ss << std::hex << hexStr;
    ss >> x;
    return x;
}
string ObjectProgramHandler::decimalToHex (int dec)
{
    int i = dec;
    stringstream ss;
    ss << uppercase << hex << i ;
    string res = ss.str();

    return res;
}
ObjectProgramHandler::ObjectProgramHandler(vector<pair<string, string> > t, string n, bool withStart)
{
    table = t;
    result = "";
    name = n;
    startExist = withStart;
    table.pop_back();
}
void ObjectProgramHandler::generateObjectCode ()
{
    int numOfZeros;
    int progLen = 0;
    /// text record
    int counter = 0; // must be <= 60 character
    vector<string> objCodes;
    string startingAdress = table[0].second;
    for (int i = 0; i < table.size(); i++) {
        counter += table[i].first.size();
        if (i == 0) {
            objCodes.push_back(table[i].first);
        } else if (counter <= 60 && (hexToDecimal(table[i].second) - hexToDecimal(table[i-1].second) <= 4)) {
            //// checking counter < 60 && address diff <= 4
            objCodes.push_back(table[i].first);
        } else {
            // discard change in counter
            counter -= table[i].first.size();
            /// write a complete text record line in result
            // adding starting address
            result += "T^";
            numOfZeros = 6 - (int)startingAdress.size();
            result += zeros(numOfZeros) + startingAdress + "^";
            // adding Length of object program in bytes (hex)
            string len = decimalToHex(counter/2 + counter%2);
            progLen += counter/2 + counter%2;
            result += len;
            // adding object codes
            for (int i = 0; i < objCodes.size(); i++) {
                result += "^" + objCodes[i];
            }
            // clearing
            objCodes.clear();
            objCodes.push_back(table[i].first);
            counter = 0;
            counter += table[i].first.size();
            startingAdress = table[i].second;
            // adding newline
            result += '\n';
        }
    }
    /// write a complete text record line in result
    // adding starting address
    result += "T^";
    numOfZeros = 6 - (int)startingAdress.size();
    result += zeros(numOfZeros) + startingAdress + "^";
    // adding Length of object program in bytes (hex)
    string len = decimalToHex(counter/2 + counter%2);
    progLen += counter/2 + counter%2;
    result += len;
    // adding object codes
    for (int i = 0; i < objCodes.size(); i++) {
        result += "^" + objCodes[i];
    }
    result += '\n';
    result += "E^";
    numOfZeros = 6 - (int)table[0].second.size();
    result += zeros(numOfZeros) + table[0].second + '\n';
    // H
    if (startExist)
    {
        /// header record
        string HResult = "";
        HResult += "H^";
        HResult += name + "^";
        // adding starting adress;
        numOfZeros = 6 - (int)table[0].second.size();
        HResult += zeros(numOfZeros) + table[0].second + "^";
        // adding Length of object program in bytes (hex)
        int lengthInDecimal = progLen;
        string lengthInHex = decimalToHex(lengthInDecimal);
        numOfZeros = 6 - (int)lengthInHex.size();
        HResult += zeros(numOfZeros) + lengthInHex;
        HResult += '\n';
        result.insert(0, HResult);
    }
}

void ObjectProgramHandler::writeInFile ()
{
    ofstream file;
    file.open ("object_code.txt");
    file << result;
    file.close();
}
