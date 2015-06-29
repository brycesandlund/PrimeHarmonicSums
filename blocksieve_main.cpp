#include <iostream>
#include "blocksieve.cpp"

using namespace std;
using namespace NTL;

int main() {
    cout << find_crossover(to_quad_float("4.0000000000003090948486749102209790"), to_quad_float(4.0), 1801241229120000000LL, 1801241230080000000LL) << endl;
}
