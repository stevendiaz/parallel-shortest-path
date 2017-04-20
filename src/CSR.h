#include <cstdint>
#include <climits>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cassert>

#ifndef CSR_h
#define CSR_h

typedef std::pair<int32_t, long> csrTuple;

using namespace std;

class CSR {
private:
    int32_t size;
    int32_t numEdges;
    vector<int32_t> value;
    vector<int32_t> IA;
    vector<int32_t> JA;
    int32_t currSrc;
    int32_t NNZ;
    vector<int32_t> seenNodes;
    vector<int32_t> indexInValue;
    vector<long> nodeLabels;
    map<int32_t, set<int32_t>> relaxMap;

public:
    CSR (int32_t size, int32_t edge);
    int32_t getSize();
    int32_t getSrc();
    void put(int32_t x, int32_t y, int32_t val);
    void phantom_put(int32_t x);
    vector<vector<int32_t>> iterate();
    void printNodeLabels();
    int32_t getLargestOutDegree();
    long getTent(int32_t u);
    void setTent(int32_t u, long val);
    void debugInfo();
    bool nodeFullyRelaxed(int32_t node);
    void relaxNode(int32_t src, int32_t dest);
    void to_dimacs();
};

#endif
