#include <iostream>
#include "endgame.cpp"

int main() {
    ftype sum = to_quad_float(3.95000001029768342623284838078);
    long long crossover_block = schofeld_crossover(sum, to_quad_float(3.95), 231865938704741408LL, 231866128644471000LL);
    cerr << crossover_block << " " << sum << endl;
    return 0;
}

