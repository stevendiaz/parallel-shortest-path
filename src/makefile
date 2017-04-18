CXX := g++
INCLUDE := /usr/local/include
CXXFLAGS := -pthread -pedantic -lstdc++ -I$(INCLUDE) -Wall -Werror -std=c++11
LIB := /usr/local/lib

benchmark: benchmark.cpp
	$(CXX) $(CXXFLAGS) benchmark.cpp -o benchmark.out

pi: pi.cpp
	$(CXX) $(CXXFLAGS) pi.cpp -o pi.out

csr: main.cpp CSR.cpp Parser.cpp
	$(CXX) $(CXXFLAGS) main.cpp CSR.cpp Parser.cpp -o csr.o

clean:
	rm *~
	rm *.o
