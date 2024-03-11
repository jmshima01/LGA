#include "print_func.h"

void print1d_vec(vector<string> vec) {
    for (unsigned int j = 0; j < vec.size(); j++) {
        cout << vec.at(j) << " ";
    }
    cout << endl;
}

void print2d_vec(vector<vector<string>> vec) {
    for (unsigned int i = 0; i < vec.size(); i++) {
        for (unsigned int j = 0; j < vec.at(i).size(); j++) {
            cout << vec.at(i).at(j) << " ";
        }
        cout << endl;
    }
}

void print_set(set<string> list) {
    for (auto a:list) {
        cout << a << " ";
    }
    cout << endl;
}