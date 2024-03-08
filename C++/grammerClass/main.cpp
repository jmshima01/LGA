#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include "grammer.hpp"
#include "print_func.h"

using namespace std;



int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Need a file name" << endl;
        return 1;
    }
    grammer gram;
    gram.read_file(argv[1]);
    gram.print_cfg();

    print_set(gram.firstSet(vector<string>{"T"}));
    print_set(gram.followSet("Var"));

    print2d_vec(gram.derive_LL1());
    print2d_vec(gram.parse_tree(argv[2]));

    return 0;
}