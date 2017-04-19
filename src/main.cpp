#include "CSR.h"
#include "Parser.h"
#include <limits>

int main(){
    clock_t t = clock();

    Parser p = Parser();
    CSR csr = p.parseInput();
    t = clock() - t;
    cout << "CSR construction takes " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;
    vector<int32_t> dist(csr.getSize());
    vector<int32_t> pred(csr.getSize());
    
    for (int i = 0; i < (int)dist.size(); ++i) {
        dist[i] = numeric_limits<int32_t>::max();
        pred[i] = 0;
    }
    int32_t src = 1;
    dist[src] = 0;

    for (int i = 0; i < (int)dist.size(); ++i) {
        vector<vector<int32_t>> graph = csr.iterate();
        for (auto it = graph.begin(); it != graph.end(); ++it) {
             if(dist[it->at(0)] + it->at(2) < dist[it->at(1)]) {
                 dist[it->at(1)] = dist[it->at(0)] + it->at(2);
                 pred[it->at(1)] = it->at(0);
             }
        }
    }
    cout << "dist[3055]: " << dist[3055] << endl;
    return 0;
}
