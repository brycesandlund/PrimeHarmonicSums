#include <iostream>
#include <cmath>
#include <NTL/quad_float.h>

#define C 0.261497212847642783755426838609

using namespace std;
using namespace NTL;

typedef quad_float ftype;

const double EPS_ABS = 1e-10;
const double EPS_REL = 1e-10;

bool feq(ftype a, ftype b) {
    ftype d = fabs(b-a);
    if (d <= EPS_ABS) return true;
    if (d <= max(fabs(a),fabs(b))*EPS_REL) return true;
    return false;
}

ftype find_z(ftype y, bool lower) {
    ftype lo = to_quad_float(0.0);
    ftype hi = to_quad_float(1e100);

    while (!feq(lo, hi)) {
        ftype z = (hi+lo) / 2;
        ftype err = (3*log(z)+4) / (8*M_PI*sqrt(z));
        ftype value = log(log(z)) + C + (lower ? err : -err);
        if (value > y) {
            hi = z;
        }
        else {
            lo = z;
        }
    }

    if (lower)
        return lo;
    else
        return hi;
}
