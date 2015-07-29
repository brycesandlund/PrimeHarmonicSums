// For determining where within a block that the prime harmonic sum crosses a threshold.
// A deterministic verison of the Miller-Rabin primality test is used to test primality.

#include "utility.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace NTL;

// Returns the first prime x in which sum 1/p p <= x crosses target
// Starts at hi and decrements until crossover occurs
long long find_crossover(ftype s, ftype target, long long lo, long long hi) {
    quad_float::SetOutputPrecision(40);
    for (long long p = hi; p >= lo; --p) {
        if(is_prime(p)) {
            if (DEBUG_BS)
                cerr << "Sum 1/p for p <= " << p << ": " << s << endl;
            s -= 1.0/p;
            if (s < target) {
                for (long long p2 = p-1; p2 >= lo; --p2) {
                    if (is_prime(p2)) {
                        if (DEBUG_BS)
                            cerr << "Sum 1/p for p <= " << p2 << ": " << s << endl;
                        return p;
                    }
                }
            }
        }
    }
    return -1;
}
