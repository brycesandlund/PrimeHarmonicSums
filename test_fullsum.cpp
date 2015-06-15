#include <iostream>
#include <cmath>
#include <iomanip>
#include "special.cpp"
#include "ordinary.cpp"
#include "S2.cpp"

using namespace std;

#define EP 1e-10

typedef long long ll;

bool isPrime(ll n) {
    for (ll i = 2; i <= sqrt(n)+EP; ++i) {
        if (n % i == 0)
            return false;
    }

    return true;
}

double calc(ll x) {
    double sum = 0;
    for (ll i = x; i >= 2; --i) {
        if (isPrime(i)) {
            sum += 1.0 / i;
        }
    }
    
    return sum;
}

int main(int argc, char *argv[]) {
    cout << setprecision(20);
    cout << fixed;

    /*for (ll i = 8; i < 10000; ++i) {
        quad_float result1 = sum1p_and_s2_m1(i) + phi_o(i) + phi_s(i);
        quad_float result2 = to_quad_float(calc(i));

        if (result1 - result2 > EP || result1 - result2 < -EP) {
            cout << "ERROR: x = " << i << endl;
            cout << "Expected: " << result2 << endl;
            cout << "Actual:   " << result1 << endl;
        }
        else {
            cout << i << " Correct." << endl;
        }
    }*/

    ll x = atoll(argv[1]);
    cout << calc(x) << endl;
    
    //ll x = 5195976;
    //cout << calc(x) << endl;
    //cout << sum1p_and_s2_m1(x) + phi_o(x) + phi_s(x) << endl;

    return 0;
}
