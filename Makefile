all: checker solver

LINKFLAGS=-lilocplex -lconcert -lcplex -lm -lpthread
CFLAGS=-O3

clean:
	rm -f bin/*.o solver checker

solver: bin/solver.o bin/main.o bin/parser.o
	g++ bin/solver.o bin/main.o bin/parser.o  -o solver $(LINKFLAGS)

checker: checker.cc
	g++ $(CFLAGS) checker.cc -o checker

bin/solver.o : src/solver.cc src/solver.hh
	g++ $(CFLAGS) -c src/solver.cc -o bin/solver.o 

bin/main.o : src/main.cc src/solver.hh src/parser.hh
	g++ $(CFLAGS) -c src/main.cc -o bin/main.o

bin/parser.o : src/parser.cc src/parser.hh
	g++ $(CFLAGS) -c src/parser.cc -o bin/parser.o
