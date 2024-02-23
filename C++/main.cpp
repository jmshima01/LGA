#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>

using namespace std;

struct rule {
    string nonT;
    vector<string> result;
};

int main(int argc, char *argv[]) {
    ifstream file_in;
    file_in.open(argv[1]);

    vector<rule> rules;
    set<string> notTs;
    set<string> symbols;
    string last;
    string nonT;
    string start;
    file_in >> last;
    
    while (!file_in.eof()) {
        string arrow;
        file_in >> arrow;
        if (arrow == "->") {
            notTs.insert(last);
            nonT = last;
            if (rules.size() > 0) {
                rules.back().result.pop_back();
            }
            
            rule current_rule;
            current_rule.nonT = nonT;
            rules.push_back(current_rule);
            symbols.insert(last);
        } else if (arrow == "|") {
            rule current_rule;
            current_rule.nonT = nonT;
            rules.push_back(current_rule);
        } else if (arrow == "$") {
            rules.back().result.push_back(arrow);
            start = nonT;
        } else {
            
            rules.back().result.push_back(arrow);
            symbols.insert(arrow);
        }
        last = arrow;
    }

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

    return 0;
}