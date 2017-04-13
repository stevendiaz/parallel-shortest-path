#include "CSR.h"

/* CSRImpl Class Methods */

/* @param int32_t size: Number of nodes, represents a size*size matrix
 * @param int32_t edges: Number of edges
 * @param int32_t src: Source node of graph
 * @return CSR : an empty compressed sparse row object
 */
CSR::CSR(int32_t size, int32_t numEdges, int32_t src) : size(size + 1), numEdges(numEdges), src(src - 1), currSrc(1), NNZ(0) {
    size += 1;
    value = vector<int32_t>();
    IA = vector<int32_t>({0, 0});
    JA = vector<int32_t>();
    currSrc = 1;
    seenNodes = vector<int32_t > (size, -1);
    nodeLabels = vector<long>(size, INT_MAX);
    relaxMap = map<int32_t, set<int32_t>>();
    tempJA = vector<int32_t>();
}

/* @param int32_t x: x value in the adjaceny matrix, the from node label
 * @param int end: next node with at least one out degree
 * private method:
 *      If the value has been set before, make proper adjustments to
 *      internal datastructures.
 */
void CSR::update(int32_t x, int end){
    //Update JA
    sort(tempJA.begin(), tempJA.end());
    JA.insert(JA.end(), tempJA.begin(), tempJA.end());

    //Update Value
    for (auto it = tempJA.begin(); it != tempJA.end(); ++it)
        value.push_back(seenNodes[*it]);

    //Update IA
    while(x <= end) {
        //Update CSR arrays
        IA.push_back(NNZ);

        //update current source node
        ++currSrc;
        ++x;
    }
}

/* @param int32_t x: x value in the adjaceny matrix, the from node label
 * @param int32_t y: y value in the adjaceny matrix, the to node label
 * @param int32_t val: the weight in the adjaceny matrix
 * public method:
 *      sets the weight of edge x to y to val
 */
void CSR::put(int32_t x, int32_t y, int32_t val) {
    x -= src;
    y -= src;
    if(relaxMap.find(x) == relaxMap.end()) relaxMap[x] = set<int32_t>({y});
    else relaxMap[x].insert(y);

    //Skip all 0-outDegree nodes from current source and update current source node
    if(currSrc < x) {
        update(currSrc + 1, x);
        seenNodes = vector<int32_t>(size, -1);
        tempJA = vector<int32_t>();
    }

    if(seenNodes[y] == -1){
        ++NNZ;
        tempJA.push_back(y);
        seenNodes[y] = val;
    } else {
        if(seenNodes[y] < val) seenNodes[y] = val;
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

        while (currentRowIndex < IA[i] - IA[i - 1]) {
            int32_t rowVal = i - 1 + src;
            int32_t colVal = JA[IA[i - 1] + currentRowIndex] + src;
            int32_t realVal = value[IA[i - 1] + currentRowIndex];

            vector <int32_t> pairing{rowVal, colVal, realVal};
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
long CSR::getTent(int32_t u) {
    return nodeLabels[u];
}

/* @param int32_t u: node id
 * @param int32_t v: tentative cost to be set
 * public method:
 *      sets the weight of edge x to y to val
 */
void CSR::setTent(int32_t u, long val) {
    nodeLabels[u] = val;
}

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


/*
 * public method:
 *      gets the node with the largest outdegree
 * @return int32_t: node with the largest out degree
 */
int32_t CSR::getLargestOutDegree() {
    int32_t oldDegree = -1;
    int32_t row = -1;

    for (int i = 0; i < size; ++i) {
        int32_t currDegree = IA[i + 1] - IA[i];
        if (currDegree > oldDegree) {
            row = i;
            oldDegree = currDegree;
        }
    }
    return row;
}

/* @param int32_t node: node id
 * public method:
 *      checks if node has had all it's edges relaxed
 * @return bool: true if all edges have been relax, false otherwise
 */
bool CSR::nodeFullyRelaxed(int32_t node){
    return relaxMap[node].size() == 0;
}

/* @param int32_t src: to edge
 * @param int32_t dest: from edge
 * public method:
 *      sets edge src->dest as relaxed
 */
void CSR::relaxNode(int32_t src, int32_t dest){
    if(relaxMap[src].find(dest) != relaxMap[src].end())
        relaxMap[src].erase(dest);
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