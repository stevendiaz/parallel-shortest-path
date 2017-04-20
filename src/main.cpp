#include "CSR.h"
#include "Parser.h"
#include <limits>
#include <pthread.h>
#define MAX_THREADS 2

struct atom;

vector<atom> a_dist;

bool changed = true;

struct args_s {
  int thread_number;
  vector<vector<int32_t>> *graph;
};


struct atom {
  atomic<int32_t>* val;
};

void* relax(void *args) {
  args_s values = *(args_s *)args;
  
  for(auto it = values.graph->begin() + values.thread_number; it < values.graph->end(); it += MAX_THREADS) {

  
    int32_t oldWeight = a_dist[it->at(1)].val->load();
    int32_t newWeight = a_dist[it->at(0)].val->load() + it->at(2);
    
    if(newWeight < oldWeight) {   
       while(!a_dist[it->at(1)].val->compare_exchange_weak(oldWeight, newWeight));

       changed = true;
   }
}
pthread_exit(NULL);
}

void bellman_ford_parallel(CSR csr, int32_t src) {
    pthread_t p_threads[MAX_THREADS];
    pthread_attr_t attr;
    pthread_attr_init (&attr);

    clock_t t = clock();
    a_dist = vector<atom>();

    for (int i = 0; i < (int)csr.getSize(); ++i) {

       atom temp;
       temp.val = new atomic<int32_t>{INT_MAX};
       a_dist.push_back(temp);
   }
   a_dist[src].val->store(0);

   int count = 0;
   cout << "Bellman Ford Parallel with " << MAX_THREADS << " threads." << endl;
   for (int i = 0; i < (int)csr.getSize(); ++i) {
    if(!changed) {
        break;
    }
    count++;
    changed = false;
    vector<vector<int32_t>> graph = csr.iterate();
    for (int j = 0; j < MAX_THREADS; ++j) {
        args_s* args = new args_s();
        args->thread_number = j;
        args->graph = &graph;
        pthread_create(&p_threads[j], &attr, relax, (void *)args);
    }
    for(int j = 0; j < MAX_THREADS; ++j) {
        pthread_join(p_threads[j], NULL);
    }
}

t = clock() - t;
cout << "Convergence count: " << count << endl;
cout << "Bellman Ford with " << MAX_THREADS << "threads took " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;

bool print = true;
if (print) {
  for(int i = 0; i < (int)csr.getSize(); ++i) {
    if(a_dist[i].val->load() == INT_MAX) {
        cout << i << " INF" << endl;
    }
    else {
        cout << i << " " << a_dist[i].val->load() << endl;
    }
}
}
}


void bellman_ford_sequential(CSR csr, int32_t src) {
    clock_t t = clock();
    /* Begin Bellman Ford algorithm */
    vector<int32_t> dist (csr.getSize());
    for(int i = 0; i < (int)dist.size(); ++i) {
        dist[i] = numeric_limits<int32_t>::max();
    }
    dist[src] = 0;

    int count = 0;
    cout << "Bellman-Ford Sequential Algorithm" << endl;
    for (int i = 0; i < (int)dist.size(); ++i) {
        if (!changed) {
            break;
        }
        count++;
        changed = false;
        vector<vector<int32_t>> graph = csr.iterate();
        for (auto it = graph.begin(); it != graph.end(); ++it) {
            if(dist[it->at(0)] + it->at(2) < dist[it->at(1)]) {
                dist[it->at(1)] = dist[it->at(0)] + it->at(2);
                changed = true;
            }
        }
    }
    /* End Bellman Ford algorithm */

    /* Output and debug info */
    t = clock() - t;
    cout << "Convergence count: " << count << endl;
    cout << "Sequential Bellman-Ford takes " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl; 
    bool print = false;
    if (print) {
      for(int i = 0; i < (int)dist.size(); ++i) {
        if(dist[i] == numeric_limits<int32_t>::max()) {
            cout << i << " INF" << endl;
        }
        else {
            cout << i << " " << dist[i] << endl;
        }
    }
}
}

int main(){

    Parser p = Parser();
    CSR csr = p.parseInput();

    bellman_ford_parallel(csr, 1);
    return 0;
}
