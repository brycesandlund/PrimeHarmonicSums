#include "utility.h"
#include <iostream>
#include "endgame.cpp"
#include "blocksieve.cpp"

using namespace std;
using namespace NTL;

bool is_prime(int n) {
    for (int i = 2; i*i <= n; ++i) {
        if (n % i == 0)
            return false;
    }
    return true;
}

int main() {
    ftype total = to_quad_float("3.50000941666553273268387035867");
    ftype y = to_quad_float("3.5");
    long long xlo = 118157392763;
    long long block_crossover = schofeld_crossover(total, y, xlo, 118213531110);
    long long crossover = find_crossover(total, y, xlo, block_crossover);
    //long long crossover = find_crossover(total, y, xlo, xhi);
    cout << "sum 1/p for p <= " << crossover << " is the smallest x with sum 1/p <= x > y" << endl;
    
    /*for (int i = 270; i < 300; ++i) {
        if (is_prime(i)) {
            cout << i << endl;
        }
    }*/
    return 0;
}
