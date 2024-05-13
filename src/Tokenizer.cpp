#include "Tokenizer.hpp"
#include <iostream>

std::vector<Token> Tokenizer::tokenize() {
  std::vector<Token> tokens;

  for (size_t i = 0; i < pattern.length(); i++) {
    if (pattern[i] == '\\' && i + 1 < pattern.length()) {

      char next = pattern[i + 1];

      switch (next) {
      case 'd': {
        tokens.push_back({TokenType::DIGIT, "\\d"});
        i++;
        break;
      }
      case 'w': {
        tokens.push_back({TokenType::ALPHANUM, "\\w"});
        i++;
        break;
      }

      default:
        throw std::runtime_error("Unhandled pattern " + pattern);
      }

    } else if (pattern[i] == '[') {
      size_t j = i + 1;
      size_t end = pattern.find(']', j);

      if (end == std::string::npos) {
        throw std::runtime_error("Unmatched '[' in pattern " + pattern);
      }

      std::cout << "Character group: " << pattern.substr(j, end - j) << '\n';

      tokens.push_back(
          {TokenType::CHARACTER_GROUP, pattern.substr(j, end - j)});
      i = end;

    }

    else {

      tokens.push_back({TokenType::LITERAL, std::string(1, pattern[i])});
    }
  }

  return tokens;
}
