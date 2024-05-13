#include "Tokenizer.hpp"

std::vector<Token> Tokenizer::tokenize() {
  std::vector<Token> tokens;

  for (size_t i = 0; i < pattern.length(); i++) {
    if (pattern[i] == '\\' && i + 1 < pattern.length()) {

      char next = pattern[i + 1];

      switch (next) {
      case 'd':
        tokens.push_back({TokenType::DIGIT, "\\d"});
        i++;
        break;

      default:
        throw std::runtime_error("Unhandled pattern " + pattern);
      }

    } else {

      tokens.push_back({TokenType::LITERAL, std::string(1, pattern[i])});
    }
  }

  return tokens;
}
