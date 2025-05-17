#include <vector>
#include <string>
#include "solver.hh"

struct ParseContent {
  std::vector<std::string> acceptingWords;
  std::vector<std::string> rejectingWords;
};

ParseContent parseStdin();
void         serializeToStdout(ParseContent, minimalDFASolution);
