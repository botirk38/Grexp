#include "DFA.hpp"
#include <iostream> // Ensure iostream is included for debugging output

bool DFA::match(const std::string &input_line) {
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
      for (char digit = '0'; digit <= '9'; ++digit) {
        transitions[state][digit] = nextState;
        std::cout << " Adding transition for digit '" << digit << "'\n";
      }
      transitions[nextState] = transitions[state];
    } else { // LITERAL
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

