#include <stdlib.h>
#include "solver.hh"
#include <ilcplex/ilocplex.h>
#include <string>

ILOSTLBEGIN

bool solveLPN(int N, minimalDFACreateInfo ci, minimalDFASolution& solution) {
  IloEnv   env;
  IloModel model(env);

  IloArray<IloBoolVarArray> D;
  IloArray<IloBoolVarArray> U;
  IloBoolVarArray           A;

  D = IloArray<IloBoolVarArray>(env, N);
  U = IloArray<IloBoolVarArray>(env, N);
  for (int i = 0; i < N; ++i) {
    D[i] = IloBoolVarArray(env, N);
    U[i] = IloBoolVarArray(env, N);
  }
  A = IloBoolVarArray(env, N);

  for (int i = 0; i < N; ++i) {
    IloExpr sumD(env), sumU(env);
    for (int j = 0; j < N; ++j) {
      sumD += D[i][j];
      sumU += U[i][j];
    }
    model.add(sumD == 1);
    model.add(sumU == 1);
    sumD.end();
    sumU.end();
  }

  auto add_word_constraints = [&](const string& w, bool should_accept) {
    int                       L = w.length();
    IloArray<IloBoolVarArray> X(env, L + 1);
    for (int t = 0; t <= L; ++t)
      X[t] = IloBoolVarArray(env, N);

    model.add(X[0][0] == 1);
    for (int i = 1; i < N; ++i)
      model.add(X[0][i] == 0);

    for (int t = 0; t < L; ++t) {
      for (int j = 0; j < N; ++j) {
        IloExpr reach(env);
        for (int i = 0; i < N; ++i) {
          IloBoolVar z(env);
          if (w[t] == '0') {
            model.add(z <= X[t][i]);
            model.add(z <= D[i][j]);
            model.add(z >= X[t][i] + D[i][j] - 1);
          } else {
            model.add(z <= X[t][i]);
            model.add(z <= U[i][j]);
            model.add(z >= X[t][i] + U[i][j] - 1);
          }
          reach += z;
        }
        model.add(X[t + 1][j] == reach);
        reach.end();
      }
    }

    IloExpr final(env);
    for (int i = 0; i < N; ++i) {
      IloBoolVar z(env);
      model.add(z <= A[i]);
      model.add(z <= X[L][i]);
      model.add(z >= A[i] + X[L][i] - 1);
      final += z;
    }
    if (should_accept)
      model.add(final >= 1);
    else
      model.add(final == 0);
    final.end();
  };

  for (auto& word : ci.acceptingWords)
    add_word_constraints(word, true);
  for (auto& word : ci.rejectingWords)
    add_word_constraints(word, false);

  IloExpr obj(env);
  for (int i = 0; i < N; ++i)
    obj += A[i];
  model.add(IloMinimize(env, obj));
  obj.end();

  IloCplex cplex(model);
  cplex.setOut(env.getNullStream());
  if (!cplex.solve()) {
    return 0;
  }

  //Write solution
  solution.states.resize(N);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (cplex.getValue(D[i][j]) > 0.5)
        solution.states[i].onZero = j;
      if (cplex.getValue(U[i][j]) > 0.5)
        solution.states[i].onOne = j;
    }
    solution.states[i].isAccepting = cplex.getValue(A[i]) > 0.5;
  }
  return 1;
}

minimalDFASolution solveLP(minimalDFACreateInfo ci) {

  minimalDFASolution solution;

  for (int i = 1; i < 100; i++) {
    std::cerr << i << "... ";
    if (solveLPN(i, ci, solution)) {
      return solution;
    }
  }

  return {};
}
