#pragma once

#include "Tokenizer.hpp"
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class DFA {
public:
  DFA(const std::vector<Token>& tokens) {
    if (tokens.empty()) {
      throw std::runtime_error("Unhandled pattern");
    }
    buildDFA(tokens);
  }

  ~DFA() = default;

  bool match(const std::string &input_line) const;

private:
  int startState = 0;
  bool hasStartAnchor = false;
  bool hasEndAnchor = false;
  bool hasZeroOrOne = false;
  std::unordered_set<int> acceptStates;
  std::vector<std::unordered_map<char, int>> transitions;

  void buildDFA(const std::vector<Token> &tokens);
  void processToken(const Token &token, int &state);
  void addRangeTransition(int state, int nextState, char start, char end);
  bool matchFromStart(const std::string &input_line) const;
  bool matchFromAnyPosition(const std::string &input_line) const;
};

