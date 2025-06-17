#include <stdlib.h>
#include <string.h>
#include <kissat/kissat.h>
#include "sat.h"

struct _SatEngine {
  kissat* solver;
  int     nextVarId;
};

// === ENGINE LIFECYCLE ===

SatEngine* satCreate() {
  SatEngine* engine = malloc(sizeof(SatEngine));
  engine->solver    = kissat_init();
  engine->nextVarId = 1;

  kissat_set_option(engine->solver, "quiet", 1);
  kissat_set_option(engine->solver, "verbose", 0);
  return engine;
}

void satDispose(SatEngine* engine) {
  kissat_release(engine->solver);
  free(engine);
}

// === VARIABLE HANDLING ===

void satAdd(SatEngine* engine, int variable) {
  kissat_add(engine->solver, variable);
}

void satGenVariables(SatEngine* engine, int* variables, int count) {
  for (int i = 0; i < count; ++i)
    variables[i] = engine->nextVarId++;
}

// === CONSTRAINTS ===

void satAtLeastOne(SatEngine* engine, int* variables, int count) {
  for (int i = 0; i < count; ++i)
    kissat_add(engine->solver, variables[i]);
  kissat_add(engine->solver, 0);
}

static void atMostOnePairwise(SatEngine* engine, int* vars, int n) {
  for (int i = 0; i < n; ++i)
    for (int j = i + 1; j < n; ++j) {
      kissat_add(engine->solver, -vars[i]);
      kissat_add(engine->solver, -vars[j]);
      kissat_add(engine->solver, 0);
    }
}

static void atMostOneTree(SatEngine* engine, int* vars, int n) {
  if (n <= 1) return;
  int  k        = n / 2;
  int  auxCount = k;
  int* aux      = malloc(sizeof(int) * auxCount);
  satGenVariables(engine, aux, auxCount);

  for (int i = 0; i < k; ++i) {
    int a = vars[2 * i];
    int b = vars[2 * i + 1];
    int m = aux[i];

    kissat_add(engine->solver, -m);
    kissat_add(engine->solver, -a);
    kissat_add(engine->solver, 0);

    kissat_add(engine->solver, -m);
    kissat_add(engine->solver, -b);
    kissat_add(engine->solver, 0);

    kissat_add(engine->solver, a);
    kissat_add(engine->solver, b);
    kissat_add(engine->solver, -m);
    kissat_add(engine->solver, 0);
  }

  // recurse if needed
  satAtMostOne(engine, aux, auxCount, SAT_ENCODING_AUTO);
  free(aux);
}

void satAtMostOne(SatEngine* engine, int* vars, int count, EncodingType encoding) {
  if (count <= 1) return;

  if (encoding == SAT_ENCODING_AUTO) {
    encoding = (count <= 4) ? SAT_ENCODING_PAIRWISE : SAT_ENCODING_TREE;
  }

  if (encoding == SAT_ENCODING_PAIRWISE || encoding == SAT_ENCODING_NONE) {
    atMostOnePairwise(engine, vars, count);
  } else if (encoding == SAT_ENCODING_TREE) {
    // pad to even count
    if (count % 2 != 0) {
      int* padded = malloc(sizeof(int) * (count + 1));
      memcpy(padded, vars, sizeof(int) * count);
      padded[count] = engine->nextVarId++; // dummy variable
      kissat_add(engine->solver, -padded[count]);
      kissat_add(engine->solver, 0); // dummy var always false
      atMostOneTree(engine, padded, count + 1);
      free(padded);
    } else {
      atMostOneTree(engine, vars, count);
    }
  }
}


void satExactlyOne(SatEngine* engine, int* variables, int count, EncodingType encoding) {
  satAtLeastOne(engine, variables, count);
  satAtMostOne(engine, variables, count, encoding);
}

// === SOLVE ===

int satSolve(SatEngine* engine) {
  return kissat_solve(engine->solver) == 10; // 10 = SATISFIABLE
}

int satGetValue(SatEngine* engine, int variable) {
  return kissat_value(engine->solver, variable) > 0;
}
