#include "DFA.hpp"
#include "Tokenizer.hpp"
#include <iostream> // Ensure iostream is included for debugging output

bool DFA::match(const std::string &input_line) const {
  int currentState = startState;

  std::cout << "Starting DFA match. Initial state: " << currentState
            << std::endl;

  for (char ch : input_line) {
    std::cout << "Processing character: " << ch << std::endl;

    auto it = transitions[currentState].find(ch);
    if (it == transitions[currentState].end()) {
      std::cout << "No transition found from state " << currentState
                << " with character '" << ch << "'. Match failed.\n";
      return false;
    }

    currentState = it->second;
    std::cout << "Transitioned to state " << currentState << std::endl;
  }

  bool isAccepting = acceptStates.count(currentState) > 0;
  std::cout << "End of input reached. Final state: " << currentState << " is "
            << (isAccepting ? "an accepting" : "not an accepting")
            << " state.\n";
  return isAccepting;
}

void DFA::buildDFA(const std::vector<Token> &tokens) {
  int state = 0;

  transitions.resize(tokens.size() + 2);

  std::cout << "Building DFA from tokens...\n";
  for (const Token &token : tokens) {
    int nextState = state + 1;
    std::cout << "Processing token: "
              << (token.type == TokenType::DIGIT ? "\\d" : token.value)
              << " from state " << state << " to state " << nextState
              << std::endl;

    if (token.type == TokenType::DIGIT) {
      std::cout << "Adding transitions for digits\n";
      std::cout.flush();
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
    }

    else { // LITERAL
      transitions[state][token.value[0]] = nextState;
      std::cout << " Adding transition for literal '" << token.value[0]
                << "'\n";
      for (unsigned char ch = 0; ch < 128; ++ch) {
        transitions[nextState][ch] = nextState;
      }
    }
    state = nextState;
  }

  acceptStates.insert(state);
  std::cout << "DFA constructed. Accepting state: " << state << std::endl;
}

void DFA::addRangeTransition(int fromState, int toState, char startChar,
                             char endChar) {
  for (char ch = startChar; ch <= endChar; ++ch) {
    transitions[fromState][ch] = toState;
  }
}
