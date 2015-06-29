#include <iostream>
#include "endgame.cpp"

int main() {
    ftype sum = to_quad_float(2);
    long long crossover_block = schofeld_crossover(sum, to_quad_float(2.0), 210, 300);
    cerr << crossover_block << " " << sum << endl;
    return 0;
}

