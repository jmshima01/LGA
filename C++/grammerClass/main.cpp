#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include "grammer.hpp"

using namespace std;


int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Need a file name" << endl;
        return 1;
    }
    grammer gram;
    gram.read_file(argv[1]);
    gram.print_cfg();

    return 0;
}