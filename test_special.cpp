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

double special(ll x, ll m, ll b) {
    ll pb;
    ll count = 0;
    for (pb = 2; count < b-1; ++pb) {
        if (isPrime(pb)) {
            ++count;
        }
    }

    double total = 0;
    for (ll i = x/m; i >= 1; --i) {
        bool success = true;
        for (ll j = 2; j <= pb; ++j) {
            if (i % j == 0) {
                success = false;
                break;
            }
        }

        if (success) {
            total += 1.0/i;
        }
    }

    return total;
}

int main(int argc, char *argv[]) {
    cout << setprecision(20);
    cout << fixed;
    ll x = atoll(argv[1]);
    ll m = atoll(argv[2]);
    ll b = atoll(argv[3]);

    cout << special(x, m, b) << endl;
    
    return 0;
}
