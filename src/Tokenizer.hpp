#pragma once

#include <stdexcept>
#include <string>
#include <vector>


enum class TokenType {
  LITERAL,
  DIGIT,
  ALPHANUM,
  CHARACTER_GROUP,
  NEGATIVE_CHARACTER_GROUP,
  START_ANCHOR,
  END_ANCHOR,
  ONE_OR_MORE,
  ZERO_OR_ONE,
  WILDCARD


};

struct Token {
  TokenType type;
  std::string value;
};

class Tokenizer {

public:
  Tokenizer(const std::string &pattern) : pattern(pattern){
    if (pattern.empty()) {
      throw std::runtime_error("Unhandled pattern " + pattern);
    }

  }

  ~Tokenizer() = default;

  std::vector<Token> tokenize();

private:
  std::string pattern;

};
