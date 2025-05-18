#pragma once
#include <vector>
#include <string>

struct minimalDFACreateInfo {
  int                      stateCount;
  std::vector<std::string> acceptingWords;
  std::vector<std::string> rejectingWords;
};

struct minimalDFASolution {
  struct State {
    int  onZero;
    int  onOne;
    bool isAccepting;
  };
  std::vector<State> states;
};

minimalDFASolution solveLP(minimalDFACreateInfo ci);
minimalDFASolution solveLPV2(minimalDFACreateInfo ci);
