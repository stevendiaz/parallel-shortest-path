# Parallel Bellman-Ford

### Input Format

This program takes in DIMACS graph input. The specificaiton for DIMACS can be found [here](http://lcs.ios.ac.cn/~caisw/Resource/about_DIMACS_graph_format.txt)

### Functionality

Apply a parallel Bellman-Ford algorithm to compute SSSP from a given start node. This uses
POSIX Threads (pthreads.h library) and observes different approaches to parallelizing the
Bellman-Ford algorithm. 

### How to use

```
$ make bellman
$ ./bellman.o < $INPUT_GRAPH
```
