CXX := g++
INCLUDE := /usr/local/include
CXXFLAGS := -pedantic -lstdc++ -I$(INCLUDE) -Wall -Werror -std=c++11
#CXXFLAGS := -std=c++11
LIB := /usr/local/lib

benchmark: benchmark.cpp
	$(CXX) $(CXXFLAGS) benchmark.cpp -o benchmark.out

