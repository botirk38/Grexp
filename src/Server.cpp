#include "DFA.hpp"
#include "Tokenizer.hpp"
#include <iostream>
#include <string>
#include "NFA.hpp"

bool match_pattern(const std::string& input_line, const std::string& pattern) {
    if (pattern.length() > 0) {
        //return input_line.find(pattern) != std::string::npos;
        std::string pat = pattern;
        NFA nfa(pat);
        return nfa.matchPattern(input_line);
    }
    else {
        std::cout << "Unhandled pattern" << std::endl;
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}
bool grep(const std::string & input_line, const std::string & pattern) {
    if (pattern[0] == '^' && pattern[pattern.size()-1] == '$') {
        std::string new_pattern = pattern.substr(1,pattern.size()-2);
        return match_pattern(input_line,new_pattern);
    } else if (pattern[0] == '^') 
        return match_pattern(input_line,pattern.substr(1,pattern.size()-1)+".*");
    else if (pattern[pattern.size()-1] == '$' )
        return match_pattern(input_line,".*"+pattern.substr(0,pattern.size()-1));
    else
        return match_pattern(input_line,".*"+pattern+".*");
}


int main(int argc, char *argv[]) {
  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.
  std::cout << "Logs from your program will appear here" << std::endl;

  if (argc != 3) {
    std::cerr << "Expected two arguments" << std::endl;
    return 1;
  }

  std::string flag = argv[1];
  std::string pattern = argv[2];

  if (flag != "-E") {
    std::cerr << "Expected first argument to be '-E'" << std::endl;
    return 1;
  }

  std::string input_line;
  std::getline(std::cin, input_line);

  try {
    if (grep(input_line, pattern)) {
      return 0;
    } else {
      return 1;
    }
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
