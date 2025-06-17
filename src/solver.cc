#include <stdlib.h>
#include "solver.hh"
#include "stringtree.hh"
#include "sat.h"
#include <iostream>

EncodingType encoding = SAT_ENCODING_NONE;

minimalDFASolution solveSAT(int N, minimalDFACreateInfo ci, const StringTree<bool>& trie) {
  minimalDFASolution solution = {};

  std::vector<std::vector<int>> D(N, std::vector<int>(N));
  std::vector<std::vector<int>> U(N, std::vector<int>(N));
  std::vector<int>              A(N);

  SatEngine* engine = satCreate();

  //Generate variables
  for (int i = 0; i < N; i++) {
    satGenVariables(engine, D[i].data(), N);
    satGenVariables(engine, U[i].data(), N);
  }
  satGenVariables(engine, A.data(), N);

  //Constraint consistent
  for (int i = 0; i < N; i++) {
    satExactlyOne(engine, D[i].data(), N, encoding);
    satExactlyOne(engine, U[i].data(), N, encoding);
  }

  struct WordLabel {
    std::string word;
    bool        accepted;
  };

  std::vector<WordLabel> words;
  for (const auto& w : ci.acceptingWords) words.push_back({w, true});
  for (const auto& w : ci.rejectingWords) words.push_back({w, false});

  // Execution traces
  std::vector<std::vector<std::vector<int>>> X(words.size());

  for (int w = 0; w < words.size(); ++w) {
    int len = words[w].word.length();
    X[w].resize(len + 1, std::vector<int>(N));
    for (int p = 0; p <= len; ++p)
      satGenVariables(engine, X[w][p].data(), N);
  }

  // Starting state (state 0)
  for (int w = 0; w < words.size(); ++w) {
    std::vector<int>& x0 = X[w][0];
    for (int s = 0; s < N; ++s) {
      if (s == 0)
        satAdd(engine, x0[s]);
      else
        satAdd(engine, -x0[s]);
      satAdd(engine, 0);
    }
  }

  // Each position must be in exactly one state
  for (int w = 0; w < words.size(); ++w)
    for (int p = 0; p <= words[w].word.length(); ++p)
      satExactlyOne(engine, X[w][p].data(), N, encoding);

  // Transition simulation
  for (int w = 0; w < words.size(); ++w) {
    const std::string& word = words[w].word;
    for (int p = 0; p < word.length(); ++p) {
      char  sym  = word[p];
      auto& curr = X[w][p];
      auto& next = X[w][p + 1];

      for (int from = 0; from < N; ++from) {
        for (int to = 0; to < N; ++to) {
          int trans = (sym == '0') ? D[from][to] : U[from][to];
          // ¬curr[from] ∨ ¬trans ∨ next[to]
          satAdd(engine, -curr[from]);
          satAdd(engine, -trans);
          satAdd(engine, next[to]);
          satAdd(engine, 0);
        }
      }
    }
  }

  // Final state must match accept/reject requirement
  for (int w = 0; w < words.size(); ++w) {
    bool  shouldAccept = words[w].accepted;
    auto& finalStates  = X[w].back();

    for (int s = 0; s < N; ++s) {
      int lit = shouldAccept ? A[s] : -A[s];
      satAdd(engine, -finalStates[s]);
      satAdd(engine, lit);
      satAdd(engine, 0);
    }
  }

  int success = satSolve(engine);

  if (success) {
    for (int i = 0; i < N; ++i) {
      minimalDFASolution::State s;
      for (int j = 0; j < N; ++j) {
        if (satGetValue(engine, D[i][j]))
          s.onZero = j;
        if (satGetValue(engine, U[i][j]))
          s.onOne = j;
      }
      s.isAccepting = satGetValue(engine, A[i]);
      solution.states.push_back(s);
    }
  }

  satDispose(engine);
  return solution;
}

minimalDFASolution solveLP(minimalDFACreateInfo ci) {
  int i = 1;
  while (true) {
    std::cerr << i << "...";
    minimalDFASolution sol = solveSAT(i, ci, {});
    if (sol.states.size() != 0) {
      std::cerr << std::endl;
      return sol;
    }
    i++;
  }
}
