#include <iostream>
#include <cmath>
#include <iomanip>

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

int main(int argc, char *argv[]) {
    cout << setprecision(20);
    cout << fixed;
    ll x = atoll(argv[1]);

    double sum = 0;
    for (ll i = x; i >= 2; --i) {
        if (isPrime(i)) {
            sum += 1.0 / i;
        }
    }

    cout << sum << endl;
    return 0;
}
