// For approximating sum_{1/p} in blocks
//
// Uses segmented sieve of Eratosthenes. We only care about numbers
// prime to 30, flags for which will fit in a byte.  (Note phi(30) = 8.)
// We skip over even multiples of 30 and test the others mod 30.
// At the end each array reference is a cache miss anyway so we can
// afford to do this test with a table look-up.
//
// Instead of accumulating 1/p, we do a "byte scan" at the end to
// recover # of primes in the interval, and the total of
// (p - start of interval).  From this we can get a good value for
// sum of 1/p from a first order Taylor approximation.
//
// Timings:   sieving interval size              time per block
//            1.2 x 10^8                         16 sec
//            2.4 x 10^8                         27 sec
//            4.8 x 10^8                         48 sec
//            9.6 x 10^8                         91 sec
//
// This code is pretty much straight-through.

#include "utility.h"
#include <iostream>
#include <cmath>
#include <vector>

#include "Primelist.h"

using namespace std;
using namespace NTL;

void HBinit(int *H, int *B)       // constructor for these tables
{ int c, s;
    unsigned char t;
    for (c=0;c<256;c++) {
        H[c] = 0; B[c] = 0;
        if (!(c&01)) {H[c]++;  B[c] += 1;}
        if (!(c&02)) {H[c]++; B[c] += 7;}
        if (!(c&04)) {H[c]++; B[c] += 11;}
        if (!(c&010)) {H[c]++; B[c] += 13;}
        if (!(c&020)) {H[c]++; B[c] += 17;}
        if (!(c&040)) {H[c]++; B[c] += 19;}
        if (!(c&0100)) {H[c]++; B[c] += 23;}
        if (!(c&0200)) {H[c]++; B[c] += 29;}
    }
    if (DEBUG_EG) {
       for (c=0;c<256;c++) cerr << H[c] << " "; cerr << endl << endl;
       for (c=0;c<256;c++) cerr << B[c] << " "; cerr << endl << endl;
    }
}

#define WSIZE 30

void Winit(int *Wrp, char *Wshft)  // constructor for the wheel
{
    int i;
    for (i=0;i<WSIZE;i++) Wrp[i] = 1;
    for (i=0;i<WSIZE;i+=2) Wrp[i] = 0;
    for (i=0;i<WSIZE;i+=3) Wrp[i] = 0;
    for (i=0;i<WSIZE;i+=5) Wrp[i] = 0;
    Wshft[1] = 1;
    Wshft[7] = 1 << 1;
    Wshft[11] = 1 << 2;
    Wshft[13] = 1 << 3;
    Wshft[17] = 1 << 4;
    Wshft[19] = 1 << 5;
    Wshft[23] = 1 << 6;
    Wshft[29] = 1 << 7;
}

long long pi_x_upper(long long x) {
    return (long long)ceil((1.25506 * x)/log(x));
}

// Returns the offset of the block in which the sum crosses the goal value
// Updates sum according to where the offset left off
long long schofeld_crossover(ftype &sum, ftype goal, long long lo, long long hi)
{
    int Wrp[WSIZE];           // stripped-down size 30 wheel
    char Wshft[WSIZE];        // mod 30 to bit position encoding
    Winit(Wrp, Wshft);

    int H[256], B[256];  // Hamming wt and isum tables for all possible bytes
    // If the zero bits are at positions c1,..,ck then
    // H = k and B = c1+...+ck.
    HBinit(H, B);

    
    long long xint = (long long)pow(hi - lo, 3.0/4);     // size of sieving interval in large sieve, should be multiple of 30
    xint += (30 - xint%30);                             // best performance is achieved when xint is as large as possible
                                                        // without running out of memory.
    long long xsize = xint/30;                          // actual size of the data block; must be integral
    long long numx = (hi - lo + xint-1) / xint;         // number of intervals to sieve in large sieve
    long long start = hi - numx*xint;                   // where to start the sieve
    

    long long bloksize = max((long long)ceil(pow(hi, 1.0/4)), 5LL);     // size of block for little sieve
    long long nbloks = max((long long)ceil(pow(hi, 1.0/4)), 5LL);     // number of blocks for little sieve
                                                            // nbloks*bloksize must be >= hi^(1.0/2)
                                                            // value can't be "too small", so take max of it with 5 
    long long pcount = pi_x_upper(nbloks*bloksize);     // should be an upper bound on pi(nbloks*bloksize)
                                                        // note that this should be pi(hi^(1/2))
   
    if (DEBUG_EG) {
        cerr << "nbloks: " << nbloks << endl;
        cerr << "bloksize: " << bloksize << endl; 
    }
    unsigned int i;
    unsigned char *Xblok = new unsigned char[xsize]; // bit vector, re-used
   
    unsigned char *G = new unsigned char[pcount];   // G[0] ... G[g-1] are half-gaps between odd primes
    long g;                    // G[0] = (5-3)/2, G[1] = (7-5)/2, etc.

    char *Sblok = new char[bloksize];  // small segments we sieve to make the prime gaps

    Primelist P(bloksize);
    if (DEBUG_EG)
        cerr << "P.max(): " << P.max() << endl;
    long prevp, newp;
    long j, k;
    long p;
    long long offset;
    
    // statistics for each block
    float fsum;
    double dsum;
    long double ldsum;
    ftype qfsum, offinv;
    int primecount;
    long long firstprime, lastprime, isum;
    
    cerr.precision(20);
    ftype::SetOutputPrecision(30);
    
    // Find all gaps between "small" primes
    
    g = 0;
    P.reset(); prevp = P.next(); // we throw away 2
    prevp = P.next();
    for (;;) {
        newp = P.next();
        if (g >= pcount) {
            cerr << "ERROR, pcount too small" << endl;
            cerr << "pcount: " << pcount << ", g: " << g << endl;
            cerr << "P.max(): " << P.max() << ", bloksize: " << bloksize << endl;
        }
        G[g++] = (newp - prevp)>>1;
        prevp = newp;
        if (newp == P.max()) break;
    }
    
    // We have now found all gaps in block 0 so we can start the little sieve
    // with k=1.  This is only done once so it need not be all that efficient
  
    if (DEBUG_EG) {
        cerr << "bloksize*nbloks: " << bloksize*nbloks << endl;
        cerr << "upper bound on pi(bloksize*nbloks): " << pi_x_upper(bloksize*nbloks) << endl;
    }

    offset = bloksize+1;
    for (k=1;k<nbloks;k++) {    //changed 7/6/15 to '<' rather than '<='
        
        if (DEBUG_EG)
            cerr << "little sieve " << k << endl;
        
        for (i=0;i<bloksize;i++) Sblok[i] = 1;
        
        P.reset();
        for (;;) {
            p = P.next();
            i = p - offset%p;
            if (i==p) i=0;  // inelegant but works
            while (i<bloksize) {
                Sblok[i] = 0;
                i += p;
            }
            if (p == P.max()) break;
        }
        
        for (i=0;i<bloksize;i++) {
            if (Sblok[i]) {
                newp = i+offset;
                if (g >= pcount) {
                    cerr << "ERROR, pcount too small" << endl;
                    cerr << "pcount: " << pcount << ", g: " << g << endl;
                }
                G[g++] = (newp - prevp)>>1;
                prevp = newp;
            }
        }
        offset += bloksize;
    }
   
    if (DEBUG_EG) { 
        cerr << "Prime table size: " << g << endl;
        cerr << "Max prime from table: " << newp << endl;
    }
    // now we sieve blocks of large numbers
    
    vector<long long> offsetA(numx);
    vector<long long> firstprimeA(numx);
    vector<long long> countA(numx);
    vector<long long> isumA(numx);

    offset = start; // this and xsize should be multiples of 30
    for (k=0;k<numx;k++) {
        
        if (offset%30) {
            cerr << "bad offset " << offset << endl;
            cerr << "should be 0 mod 30, but is " << offset%30 << " mod 30" << endl;
            exit(1);
        }
        
        for (i=0;i<xsize;i++) Xblok[i] = 0;
        
        j = 2; // we skip 2,3,5
        p = 7;
        while (j<=g)  {
            unsigned long p2;
            p2 = p<<1;
            i = p - offset%p;
            if (i==p) i=0;
            if (i%2 == 0) i += p;
            while (i<xint) {
                if (Wrp[i%30]) Xblok[i/30] |= Wshft[i%30];
                i += p2;
            }
            p = p + 2*G[j++];
            
            if (j >= pcount) {
                cerr << "ERROR, pcount too small" << endl;
                cerr << "pcount: " << pcount << ", j: " << j << endl;
            }
        }
        
        if (DEBUG_EG) {
            cerr << endl;
            cerr << "finished sieving block " << k << endl;
        }

        i=0; // find the first prime in the sieved interval
        while (Xblok[i] == 255) i++;
        { int s, t;
            s = 0;
            t = Xblok[i];
            while (t&01) {
                s++;
                t = t>>1;
            }
            if (s==0) t=1;
            if (s==1) t=7;
            if (s==2) t=11;
            if (s==3) t=13;
            if (s==4) t=17;
            if (s==5) t=19;
            if (s==6) t=23;
            if (s==7) t=29;
            firstprime = offset + 30*i + t;
        }
        
        if (DEBUG_EG)
            cerr << "offset " << offset << endl;
        offsetA[k] = offset;
        
        if (DEBUG_EG)
            cerr << "first prime at " << firstprime << endl;
        firstprimeA[k] = firstprime;

        primecount = 0; // get coeffs for sum of 1/p
        isum = 0;
        for (i=0;i<xsize;i++) {
            if (Xblok[i] == 255) continue;
            primecount += H[Xblok[i]];
            isum += (30*(long long)i)*H[Xblok[i]] + B[Xblok[i]];
        }
        
        if (DEBUG_EG)
            cerr << "prime count " << primecount << endl;
        countA[k] = primecount;
        
        if (DEBUG_EG)
            cerr << "sum of i's " << isum << endl;
        isumA[k] = isum;
        

        offset += xint;
    }
    
    ftype cumulative = sum;
    
    for (long long k = numx-1; k >= 0; --k) {
        ftype offset_float = to_ftype(offsetA[k]);

        if (offset_float == 0) {
            sum = cumulative;
            return offsetA[k] + xint;
        }

        ftype sum1p = countA[k]/offset_float - isumA[k]/(offset_float*offset_float);
        cumulative -= sum1p;
        if (DEBUG_EG)
            cerr << "cumulative: " << cumulative << " offset: " << offsetA[k] << endl;
        if (cumulative < goal) {
            sum = cumulative+sum1p;
            return offsetA[k] + xint;
        }
    }
    
    return 0;
}

