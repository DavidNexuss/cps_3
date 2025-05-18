#include <string>
#include <vector>
#include <iostream>
#include <string>
#include "parser.hh"

using namespace std;

std::vector<std::string> parseVectorlist() {
  int N;
  cin >> N;

  std::vector<std::string> result(N);
  for (int i = 0; i < N; i++) {

    int M;
    cin >> M;

    std::string word;
    word.resize(M);
    for (int j = 0; j < M; j++) cin >> word[j];

    result[i] = word;
  }

  return result;
}

void writeVectorlist(const std::vector<std::string>& v) {
  cout << v.size() << endl;
  for (const auto& word : v) {
    cout << word.size();
    for (char c : word)
      cout << "\t" << c;
    cout << endl;
  }
}

ParseContent parseStdin() {
  return ParseContent{
    .acceptingWords = parseVectorlist(),
    .rejectingWords = parseVectorlist()};
}


void writeStates(const std::vector<int>& transitions) {
  for (int i = 0; i < transitions.size(); i++) {
    cout << transitions[i] << " ";
  }
  cout << endl;
}

void serializeToStdout(ParseContent content, minimalDFASolution solution) {
  writeVectorlist(content.acceptingWords);
  cout << endl;
  writeVectorlist(content.rejectingWords);
  cout << endl;
  cout << solution.states.size() << endl;
  for (int i = 0; i < solution.states.size(); i++) {
    cout << solution.states[i].onZero << " ";
    cout << solution.states[i].onOne << " ";
    cout << solution.states[i].isAccepting << endl;
  }
}
