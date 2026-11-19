CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
SRC = $(wildcard src/*.cpp)
sitrac: $(SRC) $(wildcard src/*.h)
	$(CXX) $(CXXFLAGS) $(SRC) -o sitrac
clean:
	rm -f sitrac
