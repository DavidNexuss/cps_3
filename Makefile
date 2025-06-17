all: checker solver

LINKFLAGS=-lkissat
CFLAGS=-O3

clean:
	rm -f bin/*.o solver checker

bin:
	mkdir -p bin

solver: bin/solver.o bin/main.o bin/parser.o bin/sat.o bin
	g++ bin/solver.o bin/main.o bin/parser.o bin/sat.o -o solver $(LINKFLAGS)

checker: checker.cc bin
	g++ $(CFLAGS) checker.cc -o checker

bin/solver.o : src/solver.cc src/solver.hh
	g++ $(CFLAGS) -c src/solver.cc -o bin/solver.o 

bin/main.o : src/main.cc src/solver.hh src/parser.hh
	g++ $(CFLAGS) -c src/main.cc -o bin/main.o
bin/sat.o : src/sat.c
	gcc $(CFLAGS) -c src/sat.c -o bin/sat.o

bin/parser.o : src/parser.cc src/parser.hh
	g++ $(CFLAGS) -c src/parser.cc -o bin/parser.o
