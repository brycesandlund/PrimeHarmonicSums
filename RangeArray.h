
// Range Array Module
// Eric Bach 7/05

// Implements a compact sieveable array using a b-ary tree.
// Space needed for s bits is approximately s bytes + s/b numbers.
// Since we want a small structure, we only store a bit indicating
// whether a leaf is zero, and recompute its value (1/t) 
// as needed.  The goal of this is to reduce cache misses, which it
// is hoped compensates us for the time to recompute.

// Example: s = 10, b=3
//
//                            0      <- root (has the total)  }
//              1             2            3 <- grandparents  }= tree nodes
//         4    5    6        7                <- parents     }
//        ***  ***  ***       *  <- bit array (not actually in tree)
//
//  Note that we only add superfluous grandparents, great-grandparents, etc.
//  so this is pretty compact.

// Tests for this:   1. Compute the first n harmonic numbers
//                   2. Compute sum of 1/p for p <= n
//                   3. Sieve out by all i, 2 <= i <= n, and see if total = 1

// Note: this is the "old" design in which b is fixed.  Some code
// to use this is in special.cpp.fix

#ifndef _RANGEARRAY
#define _RANGEARRAY

using namespace std;
#include <stdio.h>
#include <iostream>
#include <cmath>

// #include <NTL/quad_float.h>  // You need the NTL prefix
                     #include <NTL/RR.h>  // You need the NTL prefix
using namespace NTL;

#include "Primelist.h"

#define ftype quad_float // use a template here ?

#define recip(x) 1.0/(x)

class RangeArray  // sieveable array w/ fast prefix sum capability
{                 // the leaf for t = offset+i has the value 1/t
                  // if you want to change this look for places 
                  // marked with ###

private:
   char *B; // bit array; 1 for in, 0 for out;
   long long offset; // can't be 0 unless we pre-sieve by 2
   unsigned long size; // we assume size > 1
   ftype *T; // tree of sums
   long b; // branching factor
   long tsize; // number of nodes
   long lc;   // left corner == B0's parent

public:
   // constructor
   inline RangeArray(const long long o, const long s, const long d) {
   unsigned long long pow;

   offset = o;
   size = s;
   B = new char[size];

   b = d;
   lc = 0; pow = 1;    // find left corner (can screw up if b is large)
   while (pow < s) {   // and compute # of actual tree nodes
         lc += pow;
         pow *= b;
         }

   tsize = (lc+s-2)/b + 1;
   T = new ftype[tsize];

   reset(offset);
   }

   void reset(long long o) { // we assume size doesn't change
   ftype t;
   unsigned long i, p;

   for (i=0;i<tsize;i++)  T[i] = 0;

   offset = o;

   // pre-sieve by 2
   long even, odd;
   even = offset%2; odd = 1-even;
   for (i=even;i<size;i+=2) B[i] = 0;
   // if you don't want to do this replace the next line by
   // for (i=0;i<size;i++)  

   for (i=odd;i<size;i+=2)  { // turn odd bits on and compute subtree totals
       t = offset+i; t = recip(t); // ###
       B[i] = 1;
       p = (lc+i-1)/b;
       for (;;) {
           T[p] += t;
           if (!p) break;
           p = (p-1)/b;
           }
       }
   }

   ~RangeArray() { // destructor, called at block/proc exit
   if (B!=NULL) delete[]B; 
   if (T!=NULL) delete[]T; 
   }

   void print() {                 // prints bit array and sum tree
   long i, rc, pow;
   printf("offset = %lld  size = %d.\n",offset,size);
   printf("b = %d  tsize = %d  lc = %d\n",b,tsize,lc);
   return; // delete to get more info
   for (i=0;i<size;i++) {
       printf("%d",B[i]);
       }
   printf("\n");
   return; // ditto
   rc = 0; pow = b;
   for (i=0;i<tsize;i++) {
        cout << T[i] << " " ;
        if (i==rc) { cout << endl;
                     rc = rc+pow;
                     pow *= b;
                     }
        }
   printf("\n\n");
   }

   void sift(const long d) { // clears every d-th Bi
   ftype t;
   unsigned long i, p;
   i = d - (offset%d); if (i==d) i=0; // min i s.t. offset+i == 0 mod d
   for (;i<size;i+=d) if (B[i]) {
           t = offset+i; t = recip(t); // ###
           B[i] = 0;
           p = (lc+i-1)/b;
           for (;;) {
               T[p] -= t;
               if (!p) break;
               p = (p-1)/b;
               }
           }
   }

   ftype prefix(const long i) {  // sum values @ offset+0..i; need 0 <= i < s.
   unsigned long r, p, j;
   ftype t, s;
   r = i%b; // rank of i compared to its siblings, counted from 0
   s = 0;
   for (j=i-r;j<=i;j++) if (B[j]) {t = offset+j; s += recip(t);}
   p = (lc+i-1)/b;
   while (p) {
          r = (p-1)%b;
          for (j=p-r;j<p;j++) s += T[j]; // ###
          p = (p-1)/b;
          }
   return(s);
   }

   ftype total() { // fast way to get to prefix(s-1)
   return(T[0]);
   }

};

// Two testing routines for this module


void Htest(long n) { // computes and prints first n odd-harmonic numbers
RangeArray R(1,n,2); // (sum of 1/n for odd n)
long i;
ftype sum;
sum = 0;
for (i=0;i<n;i++) {
    cout << i+1 << " " << R.prefix(i) << endl; 
    }
};
// Reference output:
// n
// 1,2   1
// 3,4   1.33333333333333333333333333333
// 5,6   1.53333333333333333333333333333
// 7,8   1.67619047619047619047619047619
// 9,10  1.7873015873015873015873015873
// 11,12 1.87821067821067821067821067821
// 13,14 1.95513375513375513375513375513
// 15,16 2.0218004218004218004218004218
// 17,18 2.08062395121218650630415336298
// 19,20 2.13325553015955492735678494192

void Stest(long n) { // computes and prints sum of 1/p for p <= n
long s, p;
ftype t, total;
s = (int) sqrt(n)+1;
                        cout << n << endl;
                        cout << s << endl;
RangeArray R(1,n,2097152);
                        cout << "RangeArray done." << endl;
Primelist P(s);      // was P(s); changed to n to get a realistic test
                        cout << "Primelist done." << endl;
P.reset();
total = 0;
for (;;) {
        p = P.next();
        t = p; total += recip(t); // ###
        R.sift(p);
        if (p == P.max()) break;
        }

total += R.total();
total -= 1;
cout << n << " " << total << endl;
};
// Reference value: for n = 10^6 Maple gets
// 2.887328099567672712348011299009470085543
// whereas this code computes
// 2.88732809956767271234801129902


#endif
