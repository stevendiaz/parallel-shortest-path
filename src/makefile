CXX := g++
INCLUDE := /usr/local/include
CXXFLAGS := -pthread -pedantic -lstdc++ -I$(INCLUDE) -Wall -std=c++11
LIB := /usr/local/lib

pi_integration: pi_integration.cpp
	$(CXX) $(CXXFLAGS) pi_integration.cpp -o pi_integration.out

bellman: main.cpp CSR.cpp Parser.cpp
	$(CXX) $(CXXFLAGS) main.cpp CSR.cpp Parser.cpp -o bellman.o

clean:
	rm *~
	rm *.o
