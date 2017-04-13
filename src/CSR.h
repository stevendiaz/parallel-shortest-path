#include <cstdint>
#include <climits>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>

#ifndef CSR_h
#define CSR_h

typedef std::pair<int32_t, long> csrTuple;

using namespace std;

class CSR {
private:
    int32_t size;
    int32_t numEdges;
    int32_t src;
    vector<int32_t> value;
    vector<int32_t> IA;
    vector<int32_t> JA;
    vector<int32_t> seenNodes;
    int32_t currSrc;
    int32_t NNZ;
    vector<long> nodeLabels;
    vector<int32_t> tempJA;
    map<int32_t, set<int32_t>> relaxMap;

public:
    CSR (int32_t size, int32_t edge, int32_t source);
    void put(int32_t x, int32_t y, int32_t val);
    vector<vector<int32_t>> iterate();
    void printNodeLabels();
    int32_t getLargestOutDegree();
    long getTent(int32_t u);
    void setTent(int32_t u, long val);
    void debugInfo();
    bool nodeFullyRelaxed(int32_t node);
    void relaxNode(int32_t src, int32_t dest);
    void update(int32_t x, int32_t end);
    void to_dimacs();
};

#endif