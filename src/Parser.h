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

public:
    Parser(int32_t sourceNode);
    CSR parseInput();
};

#endif