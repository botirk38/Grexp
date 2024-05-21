#include "Tokenizer.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

std::vector<Token> Tokenizer::tokenize() {

  std::vector<Token> tokens;
  size_t i = 0;

  if (pattern[0] == '^') {
    tokens.push_back({TokenType::START_ANCHOR, "^"});
    i++;
  }

  for (; i < pattern.length(); i++) {
    if (pattern[i] == '\\' && i + 1 < pattern.length()) {
      char next = pattern[i + 1];
      switch (next) {
        case 'd':
          tokens.push_back({TokenType::DIGIT, "\\d"});
          i++;
          break;
        case 'w':
          tokens.push_back({TokenType::ALPHANUM, "\\w"});
          i++;
          break;
        default:
          throw std::runtime_error("Unhandled escape sequence in pattern: " + pattern);
      }
    } else if (pattern[i] == '[') {
      bool isNegative = (i + 1 < pattern.length() && pattern[i + 1] == '^');
      size_t j = i + (isNegative ? 2 : 1);
      size_t end = pattern.find(']', j);
      if (end == std::string::npos) {
        throw std::runtime_error("Unmatched '[' in pattern: " + pattern);
      }
      tokens.push_back({isNegative ? TokenType::NEGATIVE_CHARACTER_GROUP : TokenType::CHARACTER_GROUP, pattern.substr(j, end - j)});
      i = end;
    } else if (pattern[i] == '$') {
      tokens.push_back({TokenType::END_ANCHOR, "$"});
    } else if (pattern[i] == '+') {
      tokens.push_back({TokenType::ONE_OR_MORE, "+"});
    } else if (pattern[i] == '?') {
      tokens.push_back({TokenType::ZERO_OR_ONE, "?"});
    } else if (pattern[i] == '.') {
      tokens.push_back({TokenType::WILDCARD, "."});
    } else if (pattern[i] == '|') {
      tokens.push_back({TokenType::PIPE, "|"});
    } else if (pattern[i] == '(') {
      tokens.push_back({TokenType::OPEN_PAREN, "("});
    } else if (pattern[i] == ')') {
      tokens.push_back({TokenType::CLOSE_PAREN, ")"});
    } else {
      tokens.push_back({TokenType::LITERAL, std::string(1, pattern[i])});
    }
  }

  return tokens;
}

