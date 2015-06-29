// For determining where within a block that the prime harmonic sum crosses a threshold.
// A deterministic verison of the Miller-Rabin primality test is used to test primality.

#include "utility.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace NTL;

typedef quad_float ftype;
typedef long long T;

T mult_mod(T a, T b, T m) {
    T q;
    T r;
    asm(
        "mulq %3;"
        "divq %4;"
        : "=a"(q), "=d"(r)
        : "a"(a), "rm"(b), "rm"(m));
    return r;
}

/* Computes a^b mod m. Assumes 1 <= m <= 2^62-1 and 0^0=1.
 * The return value will always be in [0, m) regardless of the sign of a.
 */
T pow_mod(T a, T b, T m) {
    if (b == 0) return 1 % m;
    if (b == 1) return a < 0 ? a % m + m : a % m;
    T t = pow_mod(a, b / 2, m);
    t = mult_mod(t, t, m);
    if (b % 2) t = mult_mod(t, a, m);
    return t >= 0 ? t : t + m;
}

/* A deterministic implementation of Miller-Rabin primality test.
 * This implementation is guaranteed to give the correct result for n < 2^64
 * by using a 7-number magic base.
 * Alternatively, the base can be replaced with the first 12 prime numbers
 * (prime numbers <= 37) and still work correctly.
 */
bool is_prime(T n) {
    T small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (int i = 0; i < 12; ++i)
        if (n > small_primes[i] && n % small_primes[i] == 0)
            return false;
    T base[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    T d = n - 1;
    int s = 0;
    for (; d % 2 == 0; d /= 2, ++s);
    for (int i = 0; i < 7; ++i) {
        T a = base[i] % n;
        if (a == 0) continue;
        T t = pow_mod(a, d, n);
        if (t == 1 || t == n - 1) continue;
        bool found = false;
        for (int r = 1; r < s; ++r) {
            t = pow_mod(t, 2, n);
            if (t == n - 1) {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    return true;
}

// Returns the first prime x in which sum 1/p p <= x crosses target
long long find_crossover(ftype s, ftype target, long long lo, long long hi) {
    quad_float::SetOutputPrecision(40);
    for (long long p = hi; p >= lo; --p) {
        if(is_prime(p)) {
            cerr << "Sum 1/p for p <= " << p << ": " << s << endl;
            s -= 1.0/p;
            if (s < target) {
                for (long long p2 = p-1; p2 >= lo; --p2) {
                    if (is_prime(p2)) {
                        cerr << "Sum 1/p for p <= " << p2 << ": " << s << endl;
                        return p;
                    }
                }
            }
        }
    }
    return -1;
}
