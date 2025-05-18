#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include "solver.hh"
#include "parser.hh"
#include "stringtree.hh"

void debugStringTree(ParseContent content) {
  std::vector<std::string>& accept = content.acceptingWords;
  std::vector<std::string>& reject = content.rejectingWords;

  StringTree<bool> tree;

  std::cerr << "Writing debug" << std::endl;
  for (auto& a : accept) tree.addString(a, 1);
  for (auto& a : reject) tree.addString(a, 0);

  auto paths = tree.getPaths();
  std::cerr << "Path " << paths.size() << std::endl;
  for (auto path : paths) {
    std::cerr << path.startingindex << ": " << path.word << " => " << path.value << std::endl;
  }
}

minimalDFASolution solve(ParseContent content) {
  std::vector<std::string>& accept = content.acceptingWords;
  std::vector<std::string>& reject = content.rejectingWords;

  minimalDFACreateInfo ci;
  ci.rejectingWords = reject;
  ci.acceptingWords = accept;
  return solveLP(ci);
}


int main() {
  ParseContent input = parseStdin();
  debugStringTree(input);

  // Get the starting time
  auto               start    = std::chrono::high_resolution_clock::now();
  minimalDFASolution solution = solve(input);
  auto               end      = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cerr << "S = " << solution.states.size() << " time = " << duration.count() << " ms" << std::endl;

  serializeToStdout(input, solution);
}
