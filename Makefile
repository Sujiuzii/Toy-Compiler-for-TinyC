all: main

CC=clang

CCFLAGS=-Wall -O3

main: *.c
	$(CXX) $(CXXFLAGS) -o main *.c

clean: rm -f test

.PHONY: all clean
