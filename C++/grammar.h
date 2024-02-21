
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <map>
typedef std::vector<std::string> symbolList;
struct rule {
    std::string lhs;
    std::vector<std::string> rhs;
};
std::vector<rule> rules;

bool derivesToLambda(std::string nonTerminal);
symbolList firstSet(std::string nonTerminal);

#endif
