#include "CSR.h"
#include "Parser.h"
#include <limits>

int main(){
    clock_t t = clock();

    Parser p = Parser();
    CSR csr = p.parseInput();

    vector<int32_t> dist(csr.getSize());
    vector<int32_t> pred(csr.getSize());

    for (int i = 0; i < (int)dist.size(); ++i) {
        dist[i] = numeric_limits<int32_t>::max();
        pred[i] = 0;
    }
    int32_t src = 1;
    dist[src] = 0;

    bool changed = true;
    int count = 0;

    for (int i = 0; i < (int)dist.size(); ++i) {
        if(!changed) {
            break;
        }
        count++;
        changed = false;
        vector<vector<int32_t>> graph = csr.iterate();
        for (auto it = graph.begin(); it != graph.end(); ++it) {
            if(dist[it->at(0)] + it->at(2) < dist[it->at(1)]) {
                dist[it->at(1)] = dist[it->at(0)] + it->at(2);
                pred[it->at(1)] = it->at(0);
                changed = true;
            }
        }
    }
    vector<vector<int32_t>> graph = csr.iterate();
    cout << "count at the end is " << count << endl;
    long long max_iterations = dist.size() * graph.size();
    cout << "max iterations: " << max_iterations << " %: " << count/max_iterations << endl;
    
    t = clock() - t;
    cout << "Bellman Ford took " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;

   
    /* for(int i = 0; i < dist.size(); ++i) {
        if(dist[i] == numeric_limits<int32_t>::max()) {
            cout << i << " INF" << endl;
        }
        else {
            cout << i << " " << dist[i] << endl;
        }
	}*/
    return 0;
}
