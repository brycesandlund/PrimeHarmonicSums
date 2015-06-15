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

    ll x = atoll(argv[1]);
    cout << calc(x) << endl;
    return 0;
}
