#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>

using namespace std;
// Structure that just stores the non-terminal as a string and the options it can be reduced to as a vector.
struct rule {
    string nonT;
    vector<string> result;
};

int main(int argc, char *argv[]) {
    ifstream file_in;
    file_in.open(argv[1]);

    vector<rule> rules; // The rules in order
    set<string> notTs; // Contains all the non terminals
    set<string> symbols; // Contains all the terminals and non terminals
    string last; // The last value read in
    string nonT; // The last non terminal read in
    string start; // The starting terminal
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
    // Print out the values
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
