
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
typedef std::vector<std::string> symbolList;
struct rule {
    std::string lhs;
    std::vector<std::string> rhs;
};
std::vector<rule> rules;
std::set<std::string> non_terminals;
std::set<std::string> symbols;
const std::string lambda = "lambda";

bool derivesToLambda(std::string nonTerminal);
symbolList firstSet(std::string nonTerminal);

#endif