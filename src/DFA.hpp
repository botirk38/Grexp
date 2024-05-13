#pragma once

#include "Tokenizer.hpp"
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class DFA {
public:
  DFA(std::vector<Token> tokens) {

    if (tokens.empty()) {
      throw std::runtime_error("Unhandled pattern");
    }

    buildDFA(tokens);
  }

  ~DFA() = default;

  bool match(const std::string &input_line);

private:
  int startState = 0;
  std::unordered_set<int> acceptStates;
  std::vector<std::unordered_map<char, int>> transitions;

  void buildDFA(const std::vector<Token> &tokens);
};
