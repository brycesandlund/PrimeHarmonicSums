// Computes full sum 1/p for all p <= x, inputted as a command line argument.
// Debug output is directed to cerr. To avoid seeing it, run "./fullsum x 2> debug.out"
// For performance-critical calculations, comment cerr lines in files below (primarily special.cpp)

#include "utility.h"
#include "special.cpp"
#include "ordinary.cpp"
#include "S2.cpp"
#include <iostream>
#include <cstdio>

using namespace std;

bool isNumber(char* input) {
    for (int i = 0; input[i] != '\0'; ++i) {
        if (input[i] < '0' || input[i] > '9') {
            return false;
        }
    }

    return true;
}

void usage(char* name) {
    printf("Usage: %s x\n", name);
}

int main(int argc, char *argv[]) {
    if (argc != 2 || !isNumber(argv[1])) {
        usage(argv[0]);
    }
    else {
        long long x = atoll(argv[1]);
        ftype special = phi_s(x);
        cout << "phi_s: " << special << endl;

        ftype ordinary = phi_o(x);
        cout << "phi_o: " << ordinary << endl;

        ftype rest = sum1p_and_s2_m1(x);
        cout << "rest: " << rest << endl;

        ftype total = special + ordinary + rest;
        cout << "total: " << total << endl;
    }

    return 0;
}
