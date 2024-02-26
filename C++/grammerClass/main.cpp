#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include "grammer.hpp"

using namespace std;

void print_set(set<string> list) {
    for (auto a:list) {
        cout << a << " ";
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Need a file name" << endl;
        return 1;
    }
    grammer gram;
    gram.read_file(argv[1]);
    gram.print_cfg();

    print_set(gram.firstSet(vector<string>{"T"}, new set<string>()));
    print_set(gram.followSet("Var", new set<string>()));

    return 0;
}