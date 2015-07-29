#include <iostream>
#include "blocksieve.cpp"

using namespace std;
using namespace NTL;

int main() {
    cout << find_crossover(to_quad_float("4.0000000000000000299157237077"), to_quad_float(4.0), 0, 1801241230056598273LL) << endl;
}
