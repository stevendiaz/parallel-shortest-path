#include "Parser.h"

Parser::Parser(int32_t sourceNode) : src(sourceNode) {}

Parser::Parser():src(1) {}

CSR Parser::parseInput(){
    //Extract info from first line
    string s;
    int32_t size, edges, x, y, val;
    cin >> s >> s >> size >> edges;

    //Construct CSR as we read the file
    CSR csr = CSR(size, edges, src);
    for(int i = 0; i < edges; ++i){
        cin >> s >> x >> y >> val;
        if(s == "a") {
            csr.put(x, y, val);
        }
    }
    csr.phantom_put(x + 1);
    csr.debugInfo();

    return csr;
}
