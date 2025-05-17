all: checker solver

LINKFLAGS=-lilocplex -lconcert -lcplex -lm -lpthread


checker: checker.cc
	g++ -O3 checker.cc -o checker

solver: bin/solver.o bin/main.o bin/parser.o
	g++ bin/solver.o bin/main.o bin/parser.o  -o solver $(LINKFLAGS)

bin/solver.o : src/solver.cc src/solver.hh
	g++ -O3 -c src/solver.cc -o bin/solver.o 

bin/main.o : src/main.cc src/solver.hh src/parser.hh
	g++ -O3 -c src/main.cc -o bin/main.o

bin/parser.o : src/parser.cc src/parser.hh
	g++ -O3 -c src/parser.cc -o bin/parser.o
