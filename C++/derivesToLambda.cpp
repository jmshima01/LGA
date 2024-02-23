#include "grammar.h"

using namespace std;

//cheat to get faster lookups
map<string, bool> lambdaLookup;
void derivesToLambda(string nonTerminal, set<int>* T);


bool derivesToLambda(string nonTerminal) {
    if(!lambdaLookup.count(nonTerminal)) {
        derivesToLambda(nonTerminal, new set<int>());
    }
    return lambdaLookup.at(nonTerminal);
}

//helper, does recursion
void derivesToLambda(string nonTerminal, set<int>* T) {
    for(int p = 0; p < rules.size(); p++) {// p represents the index of a rule
        rule r = rules[p];
        if(r.lhs != nonTerminal) { //if nonterminals link to rule indexes, change bounds of p, remove condition
            continue;
        }
        if(T->count(p)) {
            continue;
        }
        if(lambda == r.rhs[0]) {
            lambdaLookup.emplace(nonTerminal,true);
            return;
        }

        //probably a better way to do this
        symbolList recurse = symbolList();
        for(string sym : r.rhs) {
            if(!non_terminals.count(sym)){
                break;
            }
            recurse.push_back(sym);
        }
        if(recurse.size() != r.rhs.size()) {
            continue;
        }

        bool allDerive = true;
        for(string sym : recurse) {
            if(!lambdaLookup.count(sym)) {
                T->insert(p);
                derivesToLambda(sym, T);
                T->erase(p);
            }
            if (!(allDerive &= lambdaLookup.at(sym))) {
                break;
            }
        }
        if(allDerive) {
            lambdaLookup.emplace(nonTerminal,true);
            return;
        }
        
    }
    lambdaLookup.emplace(nonTerminal,false);
    return;
}