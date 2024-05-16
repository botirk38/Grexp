#include "DFA.hpp"
#include "Tokenizer.hpp"
#include <iostream> // Ensure iostream is included for debugging output

bool DFA::match(const std::string &input_line) const {
  if (hasStartAnchor) { // If there's a start anchor, match only from the
    return matchFromStart(input_line); // beginning of the input
  } else {
    return matchFromAnyPosition(
        input_line); // Otherwise, match from any position
  }
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
      break; // No valid transition, stop the matching process
    }
    currentState = it->second;
    if (index == input_line.length() - 1 &&
        acceptStates.count(currentState) > 0) {
      return true; // Successfully matched at the end of input
    }
    ++index;
  }
  return false; // No matches found starting from the beginning
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

  std::cout << "Building DFA from tokens...\n";
  for (int i = 0; i < tokens.size(); ++i) {
    const Token &token = tokens[i];
    int nextState = state + 1;
    std::cout << "Previous state: " << state << " Next state: " << nextState << '\n';

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
      std::cout << "Adding transitions for character group: " << token.value << '\n';
      for (char ch : token.value) {
        transitions[state][ch] = nextState;
      }
      transitions[nextState] = transitions[state];
    } else if (token.type == TokenType::NEGATIVE_CHARACTER_GROUP) {
      std::cout << "Adding transitions for negative character group: " << token.value << '\n';
      for (unsigned char ch = 0; ch < 128; ++ch) {
        if (token.value.find(ch) == std::string::npos) {
          transitions[state][ch] = nextState;
        }
      }
      transitions[nextState] = transitions[state];
    } else if (token.type == TokenType::START_ANCHOR) {
      hasStartAnchor = true; // Set the start anchor flag
      std::cout << "Adding start anchor\n";
      continue;
    } else if (token.type == TokenType::END_ANCHOR) {
      hasEndAnchor = true; // Set the end anchor flag
      std::cout << "Adding end anchor\n";
      acceptStates.insert(state); // The state before end anchor should be accepting
      std::cout << "DFA constructed. Accepting state: " << state << std::endl;
      return;
    } else if (token.type == TokenType::ONE_OR_MORE) {
      std::cout << "Adding one or more quantifier\n";
      for (const auto &entry : transitions[state - 1]) {
        std::cout << "Adding transition for '" << entry.first << "'\n";
        transitions[state][entry.first] = state;
      }
      std::cout << "Adding transition for literal one or more '" << token.value[0] << "'\n";
      transitions[state][tokens[i - 1].value[0]] = nextState;
      continue;
    } else if (token.type == TokenType::ZERO_OR_ONE) {
      std::cout << "Adding zero or one quantifier\n";
      // Allow skipping the character before '?'
      for (const auto &entry : transitions[state - 1]) {
        transitions[state - 1][entry.first] = nextState;
      }
      // Add transition from current state to next state for the character before '?'
      transitions[state - 1][tokens[i - 1].value[0]] = nextState;
      continue;
    } else { // LITERAL
      transitions[state][token.value[0]] = nextState;
      std::cout << "Adding transition for literal '" << token.value[0] << "'\n";
      for (unsigned char ch = 0; ch < 128; ++ch) {
        transitions[nextState][ch] = nextState;
      }
    }
    state = nextState;
  }
  if (!hasEndAnchor) {
    acceptStates.insert(state);
  }
  std::cout << "DFA constructed. Accepting state: " << state << std::endl;
}

void DFA::addRangeTransition(int fromState, int toState, char startChar,
                             char endChar) {
  for (char ch = startChar; ch <= endChar; ++ch) {
    transitions[fromState][ch] = toState;
  }
}
