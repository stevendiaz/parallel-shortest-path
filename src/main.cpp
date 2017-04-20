#include "CSR.h"
#include "Parser.h"
#include <limits>
#include <pthread.h>
#define MAX_THREADS 1

vector<int32_t> dist;
vector<int32_t> pred;
bool changed = true;

struct args_s {
    int thread_number;
    vector<vector<int32_t>> *graph;
};

void* relax(void *args) {
    cout << "relax here" << endl;
    args_s values = *(args_s *)args;
    for(auto it = values.graph->begin() + values.thread_number; it < values.graph->end(); it += MAX_THREADS) {
       if(dist[it->at(0)] + it->at(2) < dist[it->at(1)]) {
            dist[it->at(1)] = dist[it->at(0)] + it->at(2);
            pred[it->at(1)] = it->at(0);
            changed = true;
        }
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t p_threads[MAX_THREADS];
    pthread_attr_t attr;
    pthread_attr_init (&attr);
    clock_t t = clock();

    Parser p = Parser();
    CSR csr = p.parseInput();

    dist = vector<int32_t>(csr.getSize());
    pred = vector<int32_t>(csr.getSize());

    for (int i = 0; i < (int)dist.size(); ++i) {
        dist[i] = numeric_limits<int32_t>::max();
        pred[i] = 0;
    }
    int32_t src = 1;
    dist[src] = 0;

    int count = 0;
    /* Bellman Ford algorithm */
    for (int i = 0; i < (int)dist.size(); ++i) {
        if(!changed) {
            break;
        }
        count++;
        changed = false;
        vector<vector<int32_t>> graph = csr.iterate();
        for (int j = 0; j < MAX_THREADS; ++j) {
            //args_s args = { .thread_number = j; .graph = graph };
            cout << "thread creation here" << endl;
            args_s args;
            args.thread_number = j;
            args.graph = &graph;
            pthread_create(&p_threads[j], &attr, relax, (void *)&args);
        }
        for(int j = 0; j < MAX_THREADS; ++j) {
            pthread_join(p_threads[j], NULL);
            cout << "thread join " << endl;
        }
    }
    vector<vector<int32_t>> graph = csr.iterate();
    cout << "count at the end is " << count << endl;
    long long max_iterations = dist.size() * graph.size();
    cout << "max iterations: " << max_iterations << " %: " << count/max_iterations << endl;

    return 0;
}
