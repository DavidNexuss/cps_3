# Minimum Consistent Finite Automaton Solver

This project provides a SAT-based solver for the Minimum Consistent Finite Automaton problem, implemented in C++ and using [Kissat](https://github.com/arminbiere/kissat) as the SAT solver backend.

---

## Features

- Encodes the DFA inference problem as a SAT instance.
- Supports dynamic and heuristic-driven constraint generation.
- Incrementally searches for the minimal number of states \( n \).
- Includes a checker tool for solution verification.
- Efficient C++ implementation with optimized compilation flags.

---

## Requirements

- C++ compiler (e.g., `g++`) supporting C++11 or later.
- GCC compiler for C source files.
- Kissat SAT solver library (linked via `-lkissat`).

---

## Building

Use the provided `Makefile` to build both the solver and the checker:

```bash
make
```

## Running

Run the solver with input via stdin (input format defined in the documentation):

```
./solver < input.txt

```

Test if the given DFA is a solution for its problem statement:

```
./checker < solution.out

```
