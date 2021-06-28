CXX ?= g++
CXXFLAGS += -O3 -march=native -std=c++17 -DNDEBUG

all: checker

checker: checker.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm checker
