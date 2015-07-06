// Estimates lower and upper bounds on the Schoenfeld interval.
// This is accomplished by binary searching for z in the equation log(log(z)) + C +- err
// Where err is the error term that upper and lower bounds sum 1/p
// This only works when x is <= 10^100 (anything higher is impractical), and is done
// With a special feq method that uses absolute and relative error.

#include "utility.h"
#include <iostream>
#include <cmath>

#define C 0.261497212847642783755426838609
#define E 4

using namespace std;
using namespace NTL;

const double EPS_ABS = 1e-10;
const double EPS_REL = 1e-10;

// Returns true if ftype a and b are within an absolute or relative error of 1e-10
bool feq(ftype a, ftype b) {
    ftype d = fabs(b-a);
    if (d <= EPS_ABS) return true;
    if (d <= max(fabs(a),fabs(b))*EPS_REL) return true;
    return false;
}

// Returns an upper and lower bound for the sum 1/p p <= z
ftype get_value(ftype z, bool lower) {
    ftype err = (3*log(z)+4) / (8*M_PI*sqrt(z));
    return log(log(z)) + C + (lower ? err : -err);
}

// Binary searches for lower and upper bounds on z in sum 1/p p <= z = y
ftype find_z(ftype y, bool lower) {
    ftype lo = to_ftype(0.0);
    ftype hi = to_ftype(1e100);

    while (!feq(lo, hi)) {
        ftype z = (hi+lo) / 2;
        ftype value = get_value(z, lower);
        if (value > y) {
            hi = z;
        }
        else {
            lo = z;
        }
    }

    if (lower)  // E term necessary for small crossovers. Won't have performance impact for large
        return lo-E;
    else
        return hi+E;
}
