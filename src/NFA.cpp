

#include "NFA.hpp"
NFA::NFA(const std::string &regexp) : graph(0) {
  // Preprocess the regular expression
  regex = expandBackreferences(regexp);
  regex = expandSpecifiedSet(regex);
  regex = expandExactRepetition(regex);
  regex = parenthesizeEscapes(regex);
  regexLength = regex.size();
  graph = Digraph(regexLength + 1);
  // Build epsilon transitions
  buildEpsilonTransitions();
}
void NFA::buildEpsilonTransitions() {
  std::stack<unsigned int> operatorsStack;
  for (unsigned int i = 0; i < regexLength; i++) {
    unsigned int leftParenthesisIndex = i;
    if (regex[i] == '(' || regex[i] == '|') {
      operatorsStack.push(i);
    } else if (regex[i] == ')') {
      std::vector<unsigned int> orIndices;
      while (regex[operatorsStack.top()] == '|') {
        orIndices.push_back(operatorsStack.top());
        operatorsStack.pop();
      }
      leftParenthesisIndex = operatorsStack.top();
      operatorsStack.pop();
      for (unsigned int orIndex : orIndices) {
        graph.addEdge(orIndex, i);
        graph.addEdge(leftParenthesisIndex, orIndex + 1);
      }
    }
    if (i < regexLength - 1) {
      if (regex[i + 1] == '*') {
        graph.addEdge(leftParenthesisIndex, i + 1);
        graph.addEdge(i + 1, leftParenthesisIndex);
      } else if (regex[i + 1] == '+') {
        graph.addEdge(i + 1, leftParenthesisIndex);
      } else if (regex[i + 1] == '?') {
        graph.addEdge(leftParenthesisIndex, i + 1);
      }
    }
    if (regex[i] == '(' || regex[i] == '*' || regex[i] == ')' ||
        regex[i] == '+' || regex[i] == '?') {
      graph.addEdge(i, i + 1);
    }
  }
}
std::string NFA::expandSpecifiedSet(const std::string &regex) {
  std::string result;
  std::stack<char> charStack;
  std::stack<bool> negationFlagStack;
  for (size_t i = 0; i < regex.size(); i++) {
    char currentChar = regex[i];
    if (currentChar == '[') {
      if (regex[i + 1] == '^') {
        negationFlagStack.push(true);
        result += "(";
      } else {
        negationFlagStack.push(false);
        result += '(';
      }
    } else if (currentChar == ']') {
      negationFlagStack.pop();
      result += ')';
    } else if (currentChar == '^') {
      continue;
    } else {
      if (!negationFlagStack.empty() && negationFlagStack.top()) {
        result += '^';
        result += currentChar;
      } else {
        result += currentChar;
      }
    }
    if (currentChar == '[' || currentChar == '(') {
      charStack.push(currentChar);
      continue;
    }
    if (currentChar == ']' || currentChar == ')') {
      charStack.pop();
    }
    if (!charStack.empty() && charStack.top() == '[' && regex[i + 1] != ']' &&
        !negationFlagStack.empty() && !negationFlagStack.top()) {
      result += '|';
    }
  }
  return result;
}
std::string NFA::expandExactRepetition(const std::string &regex) {
  std::string result;
  std::stack<size_t> charIndexStack;
  for (size_t i = 0; i < regex.size(); i++) {
    char currentChar = regex[i];
    if (currentChar == '(') {
      charIndexStack.push(i);
      result += currentChar;
      continue;
    } else {
      size_t repetitionCount = 0, endIndex = 0;
      if ((i + 1) < regex.size() && regex[i + 1] == '{') {
        std::tie(repetitionCount, endIndex) = findRepetitionCount(regex, i + 1);
      }
      if (currentChar == ')') {
        result += currentChar;
        if (repetitionCount > 0) {
          std::string repetitionGroup =
              regex.substr(charIndexStack.top(), i - charIndexStack.top() + 1);
          for (size_t j = 0; j < repetitionCount - 1; j++) {
            result += repetitionGroup;
          }
        }
        charIndexStack.pop();
      } else {
        if (repetitionCount > 0) {
          for (size_t j = 0; j < repetitionCount - 1; j++) {
            result += currentChar;
          }
          i = endIndex;
        }
        result += currentChar;
      }
    }
  }
  return result;
}
std::pair<size_t, size_t> NFA::findRepetitionCount(const std::string &regex,
                                                   size_t startIndex) const {
  size_t endIndex = regex.find('}', startIndex);
  if (endIndex != std::string::npos) {
    size_t repetitionCount =
        std::stoul(regex.substr(startIndex + 1, endIndex - startIndex - 1));
    return {repetitionCount, endIndex};
  } else {
    throw std::runtime_error("Closing '}' not found");
  }
}
std::string NFA::parenthesizeEscapes(const std::string &regex) {
  std::string result;
  for (size_t i = 0; i < regex.size(); i++) {
    char currentChar = regex[i];
    if (currentChar == '\\') {
      result += "(" + regex.substr(i, 2) + ")";
      i++;
    } else {
      result += currentChar;
    }
  }
  return result;
}
std::string NFA::expandBackreferences(const std::string &regex) {
  std::string result;
  std::stack<char> charStack;
  std::vector<std::string> backreferences;
  std::string referenceGroup;
  for (unsigned int i = 0; i < regex.size(); i++) {
    char currentChar = regex[i];
    if (currentChar == '(') {
      charStack.push(currentChar);
      result += currentChar;
    } else if (currentChar == ')') {
      result += currentChar;
      referenceGroup += currentChar;
      backreferences.push_back(referenceGroup);
      referenceGroup.clear();
      charStack.pop();
    } else if (currentChar == '\\' && (i + 1) < regex.size() &&
               regex[i + 1] == '1') {
      result += backreferences[0];
      i++;
    } else {
      result += currentChar;
    }
    if (!charStack.empty() && charStack.top() == '(') {
      referenceGroup += currentChar;
    }
  }
  return result;
}
bool NFA::matchPattern(const std::string &inputLine) const {
  std::vector<unsigned int> possibleStates;
  DirectedDFS initialDFS(graph, 0);
  for (unsigned int v = 0; v < graph.getV(); v++) {
    if (initialDFS.isMarked(v)) {
      possibleStates.push_back(v);
    }
  }
  for (unsigned int i = 0; i < inputLine.size(); i++) {
    std::vector<unsigned int> nextStates;
    for (unsigned int state : possibleStates) {
      if (state < regexLength) {
        if (regex[state] == '^') {
          assert(regex[state - 1] == '(');
          size_t closingParenthesisPos = regex.find(')', state);
          assert(closingParenthesisPos != std::string::npos);
          bool isMatch = true;
          for (size_t k = state; k < closingParenthesisPos; k += 2) {
            if (regex[k + 1] == inputLine[i]) {
              isMatch = false;
              break;
            }
          }
          if (isMatch) {
            nextStates.push_back(closingParenthesisPos + 1);
          }
        } else if (regex[state] == '\\' && regex[state + 1] == 'd' &&
                   std::isdigit(inputLine[i])) {
          nextStates.push_back(state + 2);
        } else if (regex[state] == '\\' && regex[state + 1] == 'w' &&
                   (std::isalpha(inputLine[i]) || std::isdigit(inputLine[i]))) {
          nextStates.push_back(state + 2);
        } else if (regex[state] == inputLine[i] || regex[state] == '.') {
          nextStates.push_back(state + 1);
        }
      }
    }
    possibleStates.clear();
    DirectedDFS nextDFS(graph, nextStates);
    for (unsigned int v = 0; v < graph.getV(); v++) {
      if (nextDFS.isMarked(v)) {
        possibleStates.push_back(v);
      }
    }
  }
  for (unsigned int state : possibleStates) {
    if (state == regexLength) {
      return true;
    }
  }
  return false;
}
