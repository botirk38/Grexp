#ifndef NFA_HPP
#define NFA_HPP

#include "Digraph.hpp"
#include "DirectedDFS.hpp"
#include <cassert>
#include <cctype>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

class NFA {
    Digraph graph;
    std::string regex;
    unsigned int regexLength;

    void buildEpsilonTransitions();
    std::string expandSpecifiedSet(const std::string& regex);
    std::string expandExactRepetition(const std::string& regex);
    std::string parenthesizeEscapes(const std::string& regex);
    std::string expandBackreferences(const std::string& regex);
    std::pair<size_t, size_t> findRepetitionCount(const std::string& regex, size_t startIndex) const;

public:
    NFA(const std::string& regexp);
    bool matchPattern(const std::string& inputLine) const;
};

#endif // NFA_HPP

