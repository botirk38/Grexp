#include "DFA.hpp"
#include "Tokenizer.hpp"
#include <iostream>
#include <map>
#include <set>
#include <vector>

bool DFA::match(const std::string &input_line) const {

  return matchFromStart(input_line) || matchFromAnyPosition(input_line); 
}

bool DFA::matchFromStart(const std::string &input_line) const {
  int currentState = startState;
  size_t index = 0;
  while (index < input_line.length()) {
    char ch = input_line[index];
    std::cout << "Processing character '" << ch << "' at index " << index
              << " from state " << currentState << '\n';
    auto it = transitions[currentState].find(ch);

    if (it == transitions[currentState].end()) {
      std::cout << "No transition found for character '" << ch << "'\n";

      break;
    }
    currentState = it->second;
    if (index == input_line.length() - 1 &&
        acceptStates.count(currentState) > 0) {
      return true;
    }
    ++index;
  }
  return false;
}

bool DFA::matchFromAnyPosition(const std::string &input_line) const {
  for (size_t startIndex = 0; startIndex < input_line.length(); ++startIndex) {
    int currentState = startState;
    for (size_t index = startIndex; index < input_line.length(); ++index) {
      char ch = input_line[index];
      std::cout << "Processing character '" << ch << "' at index " << index
                << " from state " << currentState << '\n';
      auto it = transitions[currentState].find(ch);
      if (it == transitions[currentState].end()) {
        break;
      }
      currentState = it->second;
    }
    if (acceptStates.count(currentState) > 0 &&
        (!hasEndAnchor || startIndex + currentState == input_line.length())) {
      return true;
    }
  }
  return false;
}

void DFA::buildDFA(const std::vector<Token> &tokens) {
  int state = 0;
  transitions.resize(tokens.size() + 1);
  std::vector<int> startStates = {state};
  std::vector<int> endStates;

  std::cout << "Building DFA from tokens...\n";
  for (int i = 0; i < tokens.size(); ++i) {
    const Token &token = tokens[i];
    int nextState = state + 1;
    std::cout << "Previous state: " << state << " Next state: " << nextState
              << '\n';

    if (token.type == TokenType::DIGIT) {
      std::cout << "Adding transitions for digits\n";
      addRangeTransition(state, nextState, '0', '9');
      transitions[nextState] = transitions[state];
    } else if (token.type == TokenType::ALPHANUM) {
      std::cout << "Adding transitions for alphanumerics\n";
      addRangeTransition(state, nextState, 'a', 'z');
      addRangeTransition(state, nextState, 'A', 'Z');
      addRangeTransition(state, nextState, '0', '9');
      transitions[nextState] = transitions[state];
    } else if (token.type == TokenType::CHARACTER_GROUP) {
      std::cout << "Adding transitions for character group: " << token.value
                << '\n';
      for (char ch : token.value) {
        transitions[state][ch] = nextState;
      }
      transitions[nextState] = transitions[state];
    } else if (token.type == TokenType::NEGATIVE_CHARACTER_GROUP) {
      std::cout << "Adding transitions for negative character group: "
                << token.value << '\n';
      for (unsigned char ch = 0; ch < 128; ++ch) {
        if (token.value.find(ch) == std::string::npos) {
          transitions[state][ch] = nextState;
        }
      }
      transitions[nextState] = transitions[state];
    } else if (token.type == TokenType::START_ANCHOR) {
      hasStartAnchor = true;
      std::cout << "Adding start anchor\n";
      continue;
    } else if (token.type == TokenType::END_ANCHOR) {
      hasEndAnchor = true;
      std::cout << "Adding end anchor\n";
      acceptStates.insert(state);
      std::cout << "DFA constructed. Accepting state: " << state << std::endl;
      return;
    } else if (token.type == TokenType::ONE_OR_MORE) {
      std::cout << "Adding one or more quantifier\n";
      for (const auto &entry : transitions[state - 1]) {
        std::cout << "Adding transition for '" << entry.first << "'\n";
        transitions[state][entry.first] = state;
      }
      std::cout << "Adding transition for literal one or more '"
                << token.value[0] << "'\n";
      transitions[state][tokens[i - 1].value[0]] = nextState;
      continue;
    } else if (token.type == TokenType::ZERO_OR_ONE) {
      std::cout << "Adding zero or one quantifier\n";
      // Allow skipping the character before '?'
      for (const auto &entry : transitions[state]) {
        transitions[state][entry.first] = nextState;
      }
      // Add transition from current state to next state for the character
      // before '?'
      transitions[state][tokens[i - 1].value[0]] = nextState;
      continue;
    } else if (token.type == TokenType::OPEN_PAREN) {
      startStates.push_back(state);
      continue;
    } else if (token.type == TokenType::CLOSE_PAREN) {
      endStates.push_back(state);
      continue;
    } else if (token.type == TokenType::WILDCARD) {
      std::cout << "Adding wildcard\n";
      addRangeTransition(state, nextState, 0, 127);
    } else if (token.type == TokenType::PIPE) {
      std::cout << "Adding pipe\n";
      endStates.push_back(state);
      state =
          startStates
              .back(); // Reset the state for the next pattern after the pipe
      continue;
    } else { // LITERAL
      std::cout << "Adding transitions for literal: " << token.value << '\n';
      transitions[state][token.value[0]] = nextState;

      std::cout << "Adding transition for literal '" << token.value[0] << "'\n";
    }

    state = nextState;
  }

  if (!endStates.empty()) {
    endStates.push_back(state);
    for (int s : endStates) {
      acceptStates.insert(s);
    }
  } else if (!hasEndAnchor) {
    acceptStates.insert(state);
  }

  std::cout << "DFA constructed. Accepting state: " << state << std::endl;
}

void DFA::addRangeTransition(int fromState, int toState,
                             unsigned char startChar, unsigned char endChar) {
  for (unsigned char ch = startChar; ch <= endChar; ++ch) {
    transitions[fromState][ch] = toState;
  }
}
