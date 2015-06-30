// Computes first prime x where sum 1/p p <= x crosses y, inputted as a command line argument.
// If preferred, these pieces can be run individually via their respective _main programs.

#include "utility.h"
#include "special.cpp"
#include "ordinary.cpp"
#include "S2.cpp"
#include "sinterval.cpp"
#include "endgame.cpp"
#include "blocksieve.cpp"
#include <iostream>
#include <cstdio>

using namespace std;

bool isNumber(char* input) {
    for (int i = 0; input[i] != '\0'; ++i) {
        if ((input[i] < '0' || input[i] > '9') && input[i] != '.') {
            return false;
        }
    }

    return true;
}

void usage(char* name) {
    printf("Usage: %s y\n", name);
}

int main(int argc, char *argv[]) {
    if (argc != 2 || !isNumber(argv[1])) {
        usage(argv[0]);
    }
    else {
        ftype y = to_quad_float(argv[1]);

        long long crossover;
        if (y <= 0) {
            
        }
        else {
            long long xlo = (long long)conv<double>(floor(find_z(y, true)));
            long long xhi = (long long)conv<double>(ceil(find_z(y, false)));
            xhi += (30 - xhi%30);
            cout << "x-: " << xlo << " x+: " << xhi << endl;
            
            ftype special = phi_s(xhi);
            cout << "phi_s: " << special << endl;

            ftype ordinary = phi_o(xhi);
            cout << "phi_o: " << ordinary << endl;

            ftype rest = sum1p_and_s2_m1(xhi);
            cout << "rest: " << rest << endl;

            ftype total = special + ordinary + rest;
            cout << "total: " << total << endl;

            long long block_crossover = schofeld_crossover(total, y, xlo, xhi);
            crossover = find_crossover(total, y, xlo, block_crossover);
        }

        cout << "Sum 1/p for p <= " << crossover << " is the smallest prime x with sum 1/p <= x > " << y << endl;
    }

    return 0;
}
