#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <cstring>
#include <ctime>
#include "CSR.h"

const int MAX_CHARS_PER_LINE = 512;

using namespace std;

class Parser{
private:
    int32_t src;
    vector<string> split(char* s);
    int32_t stoi32(string s);
public:
    Parser(int32_t sourceNode);
    Parser();
    CSR parseInput();
};

#endif