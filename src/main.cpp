#include "CSR.h"
#include "Parser.h"


int main(){
    clock_t t;
    Parser p = Parser();
    CSR csr = p.parseInput();
    t = clock() - t;
    cout << "CSR construction takes " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;

    return 0;
}