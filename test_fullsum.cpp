#include "utility.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include "special.cpp"
#include "ordinary.cpp"
#include "S2.cpp"

#define NUM 100
#define STEP 10000000

using namespace std;


// finds prime harmonic sum via sieve and naive computation
ftype calc(long long start, long long end, bool *prime) {
    ftype sum = to_ftype(0);
    for (long long i = end; i >= start; --i) {
        if (prime[i]) {
            sum += to_ftype(1)/i;
        }
    }
    return sum;
}

void test_fullsum(long long start) {
    bool *prime = get_primes(NUM*STEP + start);
    ftype naive = calc(0, start-STEP, prime);
    ftype last = to_ftype(-1);
    for (long long i = start; i < NUM*STEP + start; i += STEP) {
        naive += calc(i-STEP+1, i, prime);
        ftype total = phi_o(i) + phi_s(i) + sum1p_and_s2_m1(i);
        if (fabs(total - naive) > to_ftype(EP)) {
            cout << "naive and calculated off" << endl;
        }
        else {
            cout << "naive and calculated correct" << endl;
        }
        cout << "naive: " << naive << endl;
        cout << "calculated: " << total << endl;
        if (last != -1 && last - total > EP) {
            cout << "total went down" << endl;
        }
        last = total;
    }
}

int main(int argc, char *argv[]) {
    cout << setprecision(20);
    cout << fixed;

    long long x = atoll(argv[1]);
    quad_float::SetOutputPrecision(30);
    //test_fullsum(x);
    cout << calc(0, x, get_primes(x)) << endl;
    return 0;
}
