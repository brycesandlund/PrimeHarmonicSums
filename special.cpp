
// Sieve algorithm for sum over special nodes
// Eric Bach August 2005
// Modified to work on non-perfect cubes x - Bryce Sandlund Spring 2015
//
// Reference values:

// 0.353153846859959286841150501533 for x = 10^3 (quad float)
// 0.3531538468599592868411505015333118978661 (sieve alg, Maple 40D arithmetic)
// 0.3531538468599592868411505015333118978661 (recursive alg, ditto)
// 0.353153846859959264 (double precision -- 53 bit fraction, about 16D)
// 0.3531538546 (float -- 24 bit fraction, about 7D)

// 
// 0.980192482969692996921123359775 for x = 10^6 (quad float)
// 0.9801924829696929969211233597748507920510 (Maple)
// 0.980192482969692191 (double)
// 0.9801927209 (float)
//
// 1.20552307041031223822946251983  for x = 10^9
// 1.205523070410312238229462519833430734613 (Maple)
// 1.20552307041030993 (double)
// 1.205523968 (float)

// 1.25854648643047059759641688202  for x = 10^12
// 1.25854648643047059759641688201818410321297122 (RR w/ default precision)
// 1.25854648643046807 (double)
// 1.258552313 (float)
//
// 1.24862902556037133904243183442  for x = 10^15 (takes 75 minutes)
// 1.24862902556036137 (double)    46135231 special nodes
// 1.248066068 (float)

// All of the following had b=256.
//
// No performance enhancements done, user time only
//
// Timings: x = 10^6   0.10 sec       269 special nodes
//              10^9   0.49 sec     13982        "
//              10^12 48.17 sec    759300        "
//              10^15  ???      wall clock about an hour; since this is an
//                              x^(2/3) algorithm I'd guess 4800 sec = 80 min
// If this holds true we would expect to use 480 ksec = 5.5 days for 10^18

// Enhancement 1: compute crude upper bound for q (= x ^ (2/3) / 5k)
//                and use this to eliminate some sieving

//          x = 10^6   0.02 sec       269 special nodes
//              10^9   0.45 sec     13982        "
//              10^12 46.11 sec    759300

// Enhancement 2: compute upper bound using optimal m' (~ x^(1/6)?)
//                the bound is x^(2/3)/(mhat * k), where mhat =
//                smallest odd sqfree s.t. for some prime p < mhat,
//                mhat*p > x^(1/3).  (Do we need p to not divide mhat?)

//          x = 10^6   0.00 sec      269 
//              10^9   0.43 sec    13982
//              10^12 42.60 sec   759300
//                    40.9 w/ per-b node counts not printed
//                    37.3 sieve and decrease mprime only (b=256)
//                    25.2            "                   (b = 16384)

// Distribution of special nodes over segments is very non-uniform
// Here are some tests for varying the tree degree b
//
// x = 10^12 12.5% of the special nodes in segment 1
//             time for segment 1  b = 1024 1.56 sec
//                                      128 0.66
//                                       16 0.29
//                                        4 0.22
//
//   x = 10^15 13% of special nodes in segment 1
//             time for segment 1  b = 1024 107    sec
//                                        8  17.86 sec
//                                        4  17.45
//                                        2  20.7
//
//   x = 10^18 13.4 % of special nodes est (via LMO) to be in segment 1
//             time for segment 1  b = 8 1388 sec
//                                     4 1328 sec
//                                     2 1571 sec

// All of this suggests that we should make b variable.  Early on 
// we want it to be small since there are a lot of prefix sums.
// At the end it needs to be large since we are mainly running sift()

// Experiments with variable tree degree (7/26/05).  Resetting b
// for each segment seems to be very costly (memory management overhead?)
// It is OK to reset at a small number of break points.  The following
// settings were determined by guesswork:
// k=0 b = 32768
//   1         4
//   10      128
//  100     1024
// 1000    32768
//
// x = 10^6 took     0.10 sec
//     10^9 took     0.35  "
//     10^12 took   34.39  "
//     10^15 took 4375.27  "
//

// We also need to write code that can be started in the middle.
// The obvious way to do it runs into segmentation faults.
// It seems that we are not decrementing nextmprime properly when
// we jump into the middle of a loop.

// Number of nodes with b=1 deemed not enough to merit special
// evaluation (for x=10^9, 202 out of 13982 total)

// 7/30 developed some "theory" for how to set tree degrees.
// For details see opt.cpp in this directory

#include <stdio.h>
#include <iostream>
#include <cmath>

#include <NTL/quad_float.h>  // You need the NTL prefix
//  #include <NTL/RR.h>  // If using RR -- remember to fix RangeArray.h too

#include "Primelist.h"

#include "Primefns.h"

#include "RangeArray.h"  // includes defn of ftype 

#define EP 1e-10

using namespace std;
using namespace NTL;

double mhat = 1;

// table of mhat values for the x we are testing
// these seem to be very close to x^(1/6)
//
//   10^3      5
//   10^6     13
//   10^9     33
//   10^12   103
//   10^15   319
//   10^18  1005
//
//   5177717 15
//   1801241484456448000 1105
//      for this "target" value final b = 2^21 = 2097152
void set_mhat(long long x) {
    if (x < 1000)
        mhat = 1;
    else if (x < 1000000)
        mhat = 5;
    else if (x < 1000000000)
        mhat = 13;
    else if (x < 1000000000000LL)
        mhat = 33;
    else if (x < 1000000000000000LL)
        mhat = 103;
    else if (x < 1000000000000000000LL)
        mhat = 319;
    else
        mhat = 1005;
}

// a cube near 10^17 is 99999429057832312
//         "   10^16 is  9999934692543307
//         "   1.5e15 is 1499983322109111
// Note: perfect cubes are no longer necessary
ftype phi_s(long long x)  // transliteration of maple code in psum.m
{                        // however we will compute a rather than bring it in
    set_mhat(x);
    long a; 
    long i;

    double x13; // exact cube root of x
    x13 = pow((double)x, 1.0/3);

    double x23; // used for some bounds so we may as well compute it now
    x23 = (double)x13*x13;

    Primelist P((long long)(x13+EP));
    a = P.length();
#define nthprime(x) P[(x)-1]  // since P[0] = 2, P[1] = 3, etc.
    long pa;   // a-th prime
    pa = nthprime(a);

    cerr << setprecision(18);
    quad_float::SetOutputPrecision(30);

    cerr << "x = " << x << endl;
    cerr << "x13 = " << x13 << endl;
    cerr << "x23 = " << x23 << endl;
    cerr << "a = " << a << endl;
    cerr << "pa = " << pa << endl;

    ftype *C;  // cumulative sum array
    long b;    // index for primes
    C = new ftype[a-1];
    for (b=1;b<=a-2;b++) C[b] = 0;
    cerr << "C done." << endl; 

    Mulist M((int)(x13+1-EP)); // table of Mobius function (computed correctly)
    cerr << "M done." << endl; 
    Spflist S((int)(x13+1-EP)); // table of smallest prime factor
    cerr << "S done." << endl; 

    long *Mprimetable; // lists smallest prime factor for odd squarefree numbers
    Mprimetable = new long[(int)(x13+1+EP)];
    Mprimetable[1] = 1;
    for (i=2;i<=x13+EP;i++) {
        if (M.mu(i) && (i%2)) {
            Mprimetable[i] = S.spf(i);
        }
        else Mprimetable[i] = 0;
    }

    cerr << "Mprimetable done." << endl; 

    long long Mchek;
    Mchek = 0;
    for (i=1;i<=x13+EP;i++) Mchek += Mprimetable[i];
    cerr << "Mprimetable check sum = " << Mchek << endl; 

    long *Nextmprime;
    Nextmprime = new long[a-1];
    for (b=1;b<=a-2;b++) Nextmprime[b] = x13;
    cerr << "Nextmprime done." << endl; 

    // include if you want the node count map
    // long *Nodecount; Nodecount = new long[a-1];

    long deg; // tree degree

    long k, mprime, q;  // as in the paper
    long long m;

    long long lo; // beginning of segment k
    long long specialcount; 
    long countthisk, countthisb;
    double bound;
    ftype totalpos, totalneg, total, thisnode, term;

    totalpos = 0; totalneg = 0; total = 0;

    specialcount = 0;

    lo = 0;
    deg = 2097152; // segment 0 has no special nodes so we just sieve

    for (k=0; k<=x13+EP;k++) {

        lo = (long long)(k*x13+EP);

        // Efficiency not worth it here since we are in the outer loop
        if (k==1) {
            cerr << "Initial sift done." << endl;
            deg = 4;
        }

        if (k>1 && deg == 4 && countthisk < 2000000) deg = 8;
        if (k>1 && deg == 8 && countthisk < 100000) deg = 16;
        if (k>1 && deg == 16 && countthisk < 50000) deg =  64;
        if (k>1 && deg == 64 && countthisk < 10000) deg = 128;
        if (k>1 && deg == 128 && countthisk < 1000) deg = 2048;
        if (k>1 && deg == 2048 && countthisk <= 2) deg = 2097152;

        long long hi = (long long)((k+1)*x13+EP);
        RangeArray R(lo, hi-lo, deg);
        cerr << "lo: " << lo << " hi: " << hi << endl;
        cerr << "RangeArray R(" << lo << ", " << hi-lo << ", " << deg << ");" << endl;

        countthisk = 0;

        // clear node count map
        // for (b=1;b<=a-2;b++) Nodecount[b] = 0;

        for (b=1;b<=a-2;b++) {

            countthisb = 0;

            q = nthprime(b+1);
            if (q > (x23+EP)/(mhat*k)) break; // all done with this k


            //bound = (x23+EP)/((k+1.0)*q);  // check that this is OK!
            bound = (1.0/hi)*x/q;
            cerr << "bound: " << bound << endl;

            mprime = Nextmprime[b];
            while (mprime > bound) {  

                if (Mprimetable[mprime] > q) {

                    m = (long long) mprime*q;

                    countthisk++;
                    countthisb++;

                    // bump node count map
                    // Nodecount[b]++ ;
                    
                    long long spot = x/m-lo;
                    ftype prefix = R.prefix(spot);

                    // also include to see other terms included in the paper
                    //cerr << "R.prefix(" << spot << ") = " << prefix << endl;

                    thisnode = C[b] + prefix;

                    //cerr << "C[b]: " << C[b] << endl;
                    
                    // include if you want a list of special nodes
                    //cerr << "special node " ;
                    //cerr << "(" << x << "/" << m << ", " << b << ") = " << thisnode << endl;
                    
                    term = thisnode/m;

                    if (M.mu(mprime) > 0) {
                        totalneg += term;
                        total -= term;
                    }
                    else {
                        totalpos += term;
                        total += term;
                    }

                }

                mprime--;  // iterating thru odds makes very little difference

            }

            Nextmprime[b] = mprime;

            C[b] += R.total();
            cerr << "R.sift(" << q << ");" << endl;
            R.sift(q);
        }

        // include if you want a count of special nodes per segment
        //if (countthisk) { 
        //    cerr << "sgmt " << k << ": " << countthisk << " nodes." << endl;
            // cerr << "total = " << total << endl;
        //}

        // include if you want the node count map
        // if (countthisk) {
        // cerr << "node count map: " ;
        // for (b=1;b<=a-2;b++) cerr << Nodecount[b] << " " ;
        // cerr << "   " << k << endl;
        // }

        specialcount += countthisk;

    }

    cerr << "x = " << x << endl;

    cerr << specialcount << " special nodes." << endl;

    cerr << "LMO estimate = " << (double)a*a/2 << endl;
    // Note: this estimate is basically pairs p>q with p*q > x^(1/3)

    cerr << "actual/estimate = " << specialcount/ ( (double)a*a/2 ) << endl;

    cerr << "total positive terms = " << totalpos << endl;
    cerr << "total negative terms = " << totalneg << endl;
    cerr << "               total = " << total << endl;
    cerr << "         discrepancy = " << total - (totalpos - totalneg) << endl;

    return total;
}
