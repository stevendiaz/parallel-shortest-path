#include "CSR.h"

/* CSRImpl Class Methods */

/* @param int32_t size: Number of nodes, represents a size*size matrix
 * @param int32_t edges: Number of edges
 * @param int32_t src: Source node of graph
 * @return CSR : an empty compressed sparse row object
 */

CSR::CSR(int32_t size, int32_t numEdges) : size(size + 1), numEdges(numEdges), currSrc(1), NNZ(0) {
    size += 1;
    value = vector<int32_t>();
    IA = vector<int32_t> (size, 0);
    JA = vector<int32_t>();
    currSrc = 1;
    seenNodes = vector<int32_t > (size + 1, -1);
    indexInValue = vector<int32_t > (size + 1, -1);
    nodeLabels = vector<long>(size, INT_MAX);
}

int32_t CSR::getSize(){ return size; }

void CSR::phantom_put(int32_t x) {
    int32_t new_val = IA[currSrc] + NNZ;
    for (int i = currSrc; i < (int)IA.size(); ++i) {
        IA[i] = new_val;
    }
}
/* @param int32_t x: x value in the adjaceny matrix, the from node label
 * @param int32_t y: y value in the adjaceny matrix, the to node label
 * @param int32_t val: the weight in the adjaceny matrix
 * public method:
 *      sets the weight of edge x to y to val
 */
void CSR::put(int32_t x, int32_t y, int32_t val) {
    if (currSrc < x) {
        seenNodes = vector<int32_t> (size + 1, -1);
        indexInValue = vector<int32_t> (size + 1, -1);
        int32_t new_val = IA[currSrc] + NNZ;
        for (int i = currSrc; i <= x; ++i) {
            IA[i] = new_val;
        }
        currSrc = x;
        value.push_back(val);
        JA.push_back(y);
        indexInValue[y] = value.size() - 1;
        seenNodes[y] = val;
        NNZ = 1;
    }
    else if(seenNodes[y] < val) {
        if(seenNodes[y] == -1){
            ++NNZ;
            value.push_back(val);
            JA.push_back(y);
            seenNodes[y] = val;
            indexInValue[y] = value.size() - 1;
        }
        else {
            seenNodes[y] = val;
            value[indexInValue[y]] = val;
        }
    }
}

/*
 * public method:
 *      returns a set of all the edges in the graph
 *      each vector will be of the from <to, from, weight>
 */
vector <vector<int32_t>> CSR::iterate() {
    vector <vector<int32_t>> result;

    for (size_t i = 1; i < IA.size(); ++i) {
        int32_t currentRowIndex = 0;

        for(int j = IA[i-1]; j < IA[i]; ++j){
            int32_t rowVal = i;
            int32_t colVal = JA[IA[i - 1] + currentRowIndex];
            int32_t val = value[IA[i - 1] + currentRowIndex];

            vector <int32_t> pairing{rowVal, colVal, val};
            result.push_back(pairing);
            ++currentRowIndex;
        }
    }
    return result;
}

/*
 * public method:
 *      Prints the tentative cost of all the nodes
 */
void CSR::printNodeLabels() {
    cout << "0 INF" << endl;
    for (size_t i = 1; i < nodeLabels.size(); ++i){
        cout << i << " ";
        if (nodeLabels[i] == INT_MAX) cout << "INF" << endl;
        else cout << nodeLabels[i] << endl;
    }

}


/* @param int32_t u: node id
* public method:
*      gets the tentative cost of node u
* @return long: tentative cost of node u
*/

/*
 * public method:
 *      prints all inner datastructures of CSR
 */
void CSR::debugInfo() {
    cout << "value: " << value.size() << endl;
    for(auto it = value.begin(); it != value.end(); ++it)
        cout << *it << " ";
    cout << endl;

    cout << "IA: " << IA.size() << endl;
    for(auto it = IA.begin(); it != IA.end(); ++it)
        cout << *it << " ";
    cout << endl;

    cout << "JA: " << JA.size() << endl;
    for(auto it = JA.begin(); it != JA.end(); ++it)
        cout << *it << " ";
    cout << endl;
}

 
void CSR::to_dimacs() {
    cout << "p sp " << size - 1 << " " <<  numEdges << endl;

    vector<vector<int32_t> > edges = iterate();
    for(auto it = edges.begin(); it != edges.end(); ++it) {
        int32_t u = it->at(0);
        int32_t v = it->at(1);
        int32_t w = it->at(2);
        cout << "a " << u << " " << v << " " << w << endl;
    }
}
