#include "grammar.h"
#include "derivesToLambda.cpp"
#include "firstSet.cpp"

using namespace std;
int main(int argc, char** argv) {
    rules = vector<rule>{
        rule{"S",symbolList{"A","cool","$"}},
        rule{"A",symbolList{"a", "B", "C"}},
        rule{"A",symbolList{"B", "C"}},
        rule{"A",symbolList{"B"}},
        rule{"B",symbolList{"b"}},
        rule{"B",symbolList{"lambda"}},
        rule{"C",symbolList{"c"}}
        };
    non_terminals = set<string>{"A","B","C","S"};
    symbols = set<string>{"A","B","C","S","cool","$","a","b","c"};

    cout << "S" << derivesToLambda("S") << endl;
    cout << "A" << derivesToLambda("A") << endl;
    cout << "B" << derivesToLambda("B") << endl;
    cout << "C" << derivesToLambda("C") << endl;
}