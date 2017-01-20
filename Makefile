EXE=tree
CC=g++
CCFLAGS=-g -O0 
LIBS=

all: dtree.o main.o
	$(CC) $(CCFLAGS) dtree.o main.o -o $(EXE) $(LIBS)

dtree.o: dtree.cpp dtree.hpp
	$(CC) $(CCFLAGS) dtree.cpp -c

main.o: main.cpp
	$(CC) $(CCFLAGS) main.cpp -c

clean:
	rm *.o $(EXE)
