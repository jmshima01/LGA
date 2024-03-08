#ifndef GRAMMER_HPP
#define GRAMMER_HPP

#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <stack>

using namespace std;

struct rule {
    string nonT;
    vector<string> result;
};

struct parse_node {
    bool terminal = false;
    string node;
    vector<parse_node*> children;

};

class grammer {
    public:
        void read_file(string file_name);
        void print_cfg();
        set<string> followSet(string A);
        bool derivesToLambda(string nonTerminal); 
        set<string> firstSet(vector<string> nonTerminal);
        set<string> predictSet(rule prodRule);
        vector<vector<string>> derive_LL1();
        vector<vector<string>> parse_tree(string file_name);
    private:
        typedef std::vector<std::string> symbolList;
        vector<rule> rules; // The rules in order
        set<string> notTs; // Contains all the non terminals
        set<string> symbols; // Contains all the terminals and non terminals
        string start; // The starting terminal
        map<string, bool> lambdaLookup;
        map<string, int> ter_dict;
        map<string, int> non_ter_dict;
        vector<vector<string>> LL;
        void derivesToLambda(string nonTerminal, set<int>* T);
        const std::string lambda = "lambda";
        vector<string> after_vec(size_t idx, vector<string> vec);
        set<string> union_set(set<string> first, set<string> second);
        bool check_lambda(vector<string> vec);
        set<string> firstSetH(vector<string> nonTerminal, set<string>* T);
        set<string> followSetH(string A, set<string>* T);
};

void grammer::read_file(string file_name) {
    ifstream file_in;
    file_in.open(file_name);

    string last; // The last value read in
    string nonT; // The last non terminal read in
    
    file_in >> last;
    // Reads in the grammer
    while (!file_in.eof()) {
        string arrow;
        file_in >> arrow;
        if (arrow == "->") { // if the value is an arrow then the last value is a non terminal so it adds it to the list and changes the vales
            notTs.insert(last);
            nonT = last;
            if (rules.size() > 0) {
                rules.back().result.pop_back();
            }
            
            rule current_rule;
            current_rule.nonT = nonT;
            rules.push_back(current_rule);
            symbols.insert(last);
        } else if (arrow == "|") { // If it is the | then its the end of the current rule but doesn't change the non terminal
            rule current_rule;
            current_rule.nonT = nonT;
            rules.push_back(current_rule);
        } else if (arrow == "$") { // This means that this rule is the starting rule so set it as such
            rules.back().result.push_back(arrow);
            start = nonT;
        } else { // Its just some random value
            rules.back().result.push_back(arrow);
            symbols.insert(arrow);
        }
        last = arrow;
    }
    int count = 0;
    for (auto a:symbols) {
        if (notTs.count(a) == 0) {
            ter_dict.insert({a, count});
            count++;
        }
    }
    file_in.close();
}

void grammer::print_cfg() {
    cout << "Grammar Non-Terminals" << endl;
    bool first = true;
    for (auto a:notTs) {
        if (!first) {
            cout << ", ";
            
        } else {
            first = false;
        }
        cout << a;
    }
    cout << endl;
    cout << "Grammar Symbols" << endl;
    first = true;
    for (auto a:symbols) {
        if (!first) {
            cout << ", ";
            
        } else {
            first = false;
        }
        cout << a;
    }
    cout << endl << endl;

    cout << "Grammar Rules" << endl;
    int counter = 1;
    for (auto a:rules) {
        cout << "(" << counter << ")    " << a.nonT << " -> ";
        for (auto b:a.result) {
            cout << b << " ";
        }
        cout << endl;
        counter++;
    }
    cout << "Grammar Start Symbol or Goal: " << start << endl;
}

set<string> grammer::firstSet(vector<string> nonTerminal) {
    return firstSetH(nonTerminal, new set<string>());
}

set<string> grammer::firstSetH(vector<string> nonTerminal, set<string>* T) {
    set<string> F;
    if (notTs.count(nonTerminal[0]) == 0) {
        F.insert(nonTerminal[0]);
        return F;
    }
    if (T->count(nonTerminal[0]) == 0) {
        T->insert(nonTerminal[0]);
        for (auto p:rules) {
            if (nonTerminal[0] == p.nonT) {
                auto G = firstSetH(p.result, T);
                F = union_set(F, G);
            }
        }
    }
    if (derivesToLambda(nonTerminal[0]) && nonTerminal.size() > 1) {
        auto G = firstSetH(after_vec(1, nonTerminal), T);
        F = union_set(F, G);
    }
    return F;
}

set<string> grammer::followSet(string A) {
    return followSetH(A, new set<string>());
}

set<string> grammer::followSetH(string A, set<string>* T) {
    if (T->count(A) != 0) {
        return set<string>{};
    }
    T->insert(A);
    set<string> F;
    for (auto p:rules) {
        for (size_t i = 0; i < p.result.size(); i++) {
            if (A == p.result[i]) {
                if (i != p.result.size() - 1) {
                    auto G = firstSetH(after_vec(i + 1, p.result), new set<string>());
                    F = union_set(F, G);
                }
                if (i == p.result.size() - 1 || check_lambda(after_vec(i, p.result))) {
                    auto G = followSetH(p.result[i], T);
                    F = union_set(F, G);
                }
            }
        }
    }
    return F;
}

bool grammer::derivesToLambda(string nonTerminal) {
    if(!lambdaLookup.count(nonTerminal)) {
        derivesToLambda(nonTerminal, new set<int>());
    }
    return lambdaLookup.at(nonTerminal);
}

//helper, does recursion
void grammer::derivesToLambda(string nonTerminal, set<int>* T) {
    for(size_t p = 0; p < rules.size(); p++) {// p represents the index of a rule
        rule r = rules[p];
        if(r.nonT != nonTerminal) { //if nonterminals link to rule indexes, change bounds of p, remove condition
            continue;
        }
        if(T->count(p)) {
            continue;
        }
        if(lambda == r.result[0]) {
            lambdaLookup.emplace(nonTerminal,true);
            return;
        }

        //probably a better way to do this
        symbolList recurse = symbolList();
        for(string sym : r.result) {
            if(!notTs.count(sym)){
                break;
            }
            recurse.push_back(sym);
        }
        if(recurse.size() != r.result.size()) {
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

vector<string> grammer::after_vec(size_t idx, vector<string> vec) {
    vector<string> new_vec;
    for (size_t i = idx; i < vec.size(); i++) {
        new_vec.push_back(vec[i]);
    }
    return new_vec;
}

set<string> grammer::union_set(set<string> first, set<string> second) {
    for (auto a:second) {
        first.insert(a);
    }
    return first;
}

bool grammer::check_lambda(vector<string> vec) {
    bool valid = true;
    for (size_t i = 0; i < vec.size(); i++) {
        if (notTs.count(vec[i]) == 0) {
            valid = false;
            break;
        } else {
            if (!derivesToLambda(vec[i])) {
                valid = false;
                break;
            }
        }
    }
    return valid;
}

set<string> grammer::predictSet(rule prodRule) {
    set<string> F;
    F = firstSet(prodRule.result);
    if (derivesToLambda(prodRule.nonT)) {
        F = union_set(F, followSet(prodRule.nonT));
    }
    return F;
}

vector<vector<string>> grammer::derive_LL1() {
    int count = 0;
    for (auto a:notTs) {
        LL.push_back({a});
        non_ter_dict.insert({a,count});
        for (auto b:symbols) {
            if (notTs.count(b) == 0) {
                LL[count].push_back("0");
            }
        }
        for (size_t i = 0; i < rules.size(); i++) {
            if (rules[i].nonT == a) {
                set<string> S = predictSet(rules[i]);
                for (auto b:S) {
                    LL[count][ter_dict[b]] = to_string(i+1);
                }
            }
        }
        count++;
    }
    return LL;
}

vector<vector<string>> grammer::parse_tree(string file_name) {
    ifstream file_in(file_name);
    queue<string> Q;
    string next;
    while (file_in >> next) {
        Q.push(next);
    }
    stack<string> S;
    S.push(start);
    vector<vector<string>> tree;
    // cout << "1" << endl;
    while (!S.empty() && !Q.empty()) {
        string node = S.top();
        S.pop();
        
        if (node == "lambda") {
            
            tree.back().push_back(node);
            continue;
        } else if (notTs.count(node) == 0) {
            if (node == Q.front()) {
                Q.pop();
                // tree.back().push_back(node);
                continue;
            } else {
                cout << "Invalid grammer" << endl;
                return tree;
            }
        }
        tree.push_back({node});
        next = Q.front();
        
        int next_idx = stoi(LL[non_ter_dict[node]][ter_dict[next]]) - 1;
        cout << next_idx << " " << node << endl;
        rule next_rule = rules[next_idx];
        for (size_t i = 0; i < next_rule.result.size(); i++) {
            // cout << next_rule.result[i] << " " << i << endl;
            tree.back().push_back(next_rule.result[i]);
        }
        for (int i = (int)next_rule.result.size()-1; i >= 0; i--) {
            // cout << next_rule.result[i] << " " << i << endl;
            S.push(next_rule.result[i]);
            
        }
    }
    if (!Q.empty() || !S.empty()) {
        cout << "Invalid grammer" << endl;
    }
    return tree;
}

#endif