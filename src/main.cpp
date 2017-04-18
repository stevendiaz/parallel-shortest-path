#include "CSR.h"
#include "Parser.h"


int main(){
  Parser p = Parser();

    clock_t t = clock();
  CSR csr = p.parseInput();
  t = clock() - t;
  cout << "CSR construction takes " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;
  
  return 0;
}
