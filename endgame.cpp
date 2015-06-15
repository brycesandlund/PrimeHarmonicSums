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

#include <iostream>
#include <cmath>

#include "Primelist.h"

#include <NTL/quad_float.h>  // You need the NTL prefix
using namespace std;
using namespace NTL;

// #define START   1800000000000000000LL  -- for debugging runs
#define START      1000020 // needs to be 0 mod 30

#define BLOKSIZE 1001              // needs to be >= 4th root of START
#define NBLOKS   1001              // can be reduced for debugging

#define PCOUNT  80000       // upper bound on pi(START ^ 1/2)

unsigned char G[PCOUNT];   // G[0] ... G[g-1] are half-gaps between odd primes
long g;                    // G[0] = (5-3)/2, G[1] = (7-5)/2, etc.

char Sblok[BLOKSIZE];  // small segments we sieve to make the prime gaps
unsigned int i;

#define XINT 1200000L // size of sieving interval, should be mult of 30
#define XSIZE XINT/30   // actual size of the data block; must be integral
#define NUMX 100        // number of blocks this run
unsigned char Xblok[XSIZE]; // bit vector, re-used

int H[256], B[256];  // Hamming wt and isum tables for all possible bytes
// If the zero bits are at positions c1,..,ck then
// H = k and B = c1+...+ck.

void HBinit()       // constructor for these tables
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
    // for (c=0;c<256;c++) cout << H[c] << " "; cout << endl << endl;
    // for (c=0;c<256;c++) cout << B[c] << " "; cout << endl << endl;
}

#define WSIZE 30
int Wrp[WSIZE];           // stripped-down size 30 wheel
char Wshft[WSIZE];        // mod 30 to bit position encoding

void Winit()  // constructor for the wheel
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

int main()
{
    Primelist P(BLOKSIZE);
    long prevp, newp;
    long j, k;
    long p;
    long long offset;
    
    // statistics for each block
    float fsum;
    double dsum;
    long double ldsum;
    quad_float qfsum, offinv;
    int primecount;
    long long firstprime, lastprime, isum;
    
    cout.precision(20);
    quad_float::SetOutputPrecision(30);
    
    // Find all gaps between "small" primes
    
    g = 0;
    P.reset(); prevp = P.next(); // we throw away 2
    prevp = P.next();
    for (;;) {
        newp = P.next();
        G[g++] = (newp - prevp)>>1;
        prevp = newp;
        if (newp == P.max()) break;
    }
    
    // We have now found all gaps in block 0 so we can start the little sieve
    // with k=1.  This is only done once so it need not be all that efficient
    
    offset = BLOKSIZE+1;
    for (k=1;k<=NBLOKS;k++) {
        
        // cout << "little sieve " << k << endl;
        
        for (i=0;i<BLOKSIZE;i++) Sblok[i] = 1;
        
        P.reset();
        for (;;) {
            p = P.next();
            i = p - offset%p;
            if (i==p) i=0;  // inelegant but works
            while (i<BLOKSIZE) {
                Sblok[i] = 0;
                i += p;
            }
            if (p == P.max()) break;
        }
        
        for (i=0;i<BLOKSIZE;i++) {
            if (Sblok[i]) {
                newp = i+offset;
                G[g++] = (newp - prevp)>>1;
                prevp = newp;
            }
        }
        offset += BLOKSIZE;
    }
    
    cout << "Prime table size: " << g << endl;
    cout << "Max prime from table: " << newp << endl;
    
    // now we sieve blocks of large numbers
    
    HBinit();
    Winit();
    
    offset = START; // this and XSIZE should be multiples of 30
    for (k=0;k<NUMX;k++) {
        
        if (offset%30) {
            cout << "bad offset " << offset << endl;
            exit(1);
        }
        
        for (i=0;i<XSIZE;i++) Xblok[i] = 0;
        
        j = 2; // we skip 2,3,5
        p = 7;
        while (j<=g)  {
            unsigned long p2;
            p2 = p<<1;
            i = p - offset%p;
            if (i==p) i=0;
            if (i%2 == 0) i += p;
            while (i<XINT) {
                if (Wrp[i%30]) Xblok[i/30] |= Wshft[i%30];
                i += p2;
            }
            p = p + 2*G[j++];
        }
        
        cout << endl;
        cout << "finished sieving block " << k << endl;
        
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
        
        cout << "offset " << offset << endl;
        cout << "first prime at " << firstprime << endl;
        
        primecount = 0; // get coeffs for sum of 1/p
        isum = 0;
        for (i=0;i<XSIZE;i++) {
            if (Xblok[i] == 255) continue;
            primecount += H[Xblok[i]];
            isum += (30*(long long)i)*H[Xblok[i]] + B[Xblok[i]];
        }
        
        cout << "prime count " << primecount << endl;
        cout << "sum of i's " << isum << endl;
        
        offset += XINT;
    }
    return 0;
}
