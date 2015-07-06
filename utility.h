#ifndef _UTILITY
#define _UTILITY

#define DEBUG 0
#define DEBUG_SP 0
#define DEBUG_OD 0
#define DEBUG_S2 0
#define DEBUG_EG 0
#define EP 1e-20

#include <cstdio>
#include <NTL/quad_float.h>
//#include <NTL/RR.h>  // If using RR. Changing functions here applies to all files

using namespace std;
using namespace NTL;

typedef quad_float ftype;

ftype to_ftype(long long n)
{
    char s[30];
    sprintf(s,"%lld%c",n,0);
    return to_quad_float(s);
}

ftype to_ftype(double n) {
    return to_quad_float(n);
}

ftype to_ftype(int n) {
    return to_quad_float(n);
}

ftype to_ftype(const char* n) {
    return to_quad_float(n);
}

ftype to_ftype(long n) {
    return to_ftype((long long)n);
}

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

bool * get_primes(long long x) {
    bool *prime = new bool[x+1];
    memset(prime, true, (x+1)*sizeof(bool));
    prime[0] = false;
    prime[1] = false;
    for (long long i = 2; i*i <= x; ++i) {
        if (prime[i]) {
            for (long long j = i*i; j <= x; j+=i) {
                prime[j] = false;
            }
        }
    }
    return prime;
}


#endif
