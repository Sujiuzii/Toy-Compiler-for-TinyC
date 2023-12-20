all: main

CC=clang

CCFLAGS=-Wall

main: *.c
	$(CC) $(CCFLAGS) -o main *.c

clean: rm -f main

.PHONY: all clean
