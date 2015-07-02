#include "utility.h"
#include <iostream>
#include "endgame.cpp"
#include "blocksieve.cpp"

using namespace std;
using namespace NTL;

void prime_counts(long long start, long long xint, int nbloks) {
    for (int i = 0; i < nbloks; ++i) {
        int nprimes = 0;
        for (int j = start + i*xint; j < start + (i+1)*xint; ++j) {
            if (is_prime(j)) {
                ++nprimes;
            }
        }
        cout << nprimes << " from " << start + i*xint << " to " << start + (i+1)*xint << endl;
    }
}

int main() {
    prime_counts(158918610, 11010, 105);
    /*ftype total = to_quad_float("3.50000941666553273268387035867");
    ftype y = to_quad_float("3.5");
    long long xlo = 118157392763;
    long long block_crossover = schofeld_crossover(total, y, xlo, 118213531110);
    long long crossover = find_crossover(total, y, xlo, block_crossover);
    //long long crossover = find_crossover(total, y, xlo, xhi);
    cout << "sum 1/p for p <= " << crossover << " is the smallest x with sum 1/p <= x > y" << endl;
    
    for (int i = 270; i < 300; ++i) {
        if (is_prime(i)) {
            cout << i << endl;
        }
    }*/
    return 0;
}
