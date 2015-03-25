//  Class Primelist
//
//  This class supports simple operations on a list of primes.
//  It uses the Sieve of Eratosthenes to construct the list of primes.
//  The Primelist class supports the following public functions:
//
//  Constructors:
//    Primelist P;         -- constructor, initializes the list to empty
//    Primelist P(long n); -- constructor, finds primes up to n (calls find())
//  Standard functions:
//    void P.find(long n); -- finds primes up to n (deletes any previous list)
//                            allocates several large arrays, and may run out
//                            of memory; to detect failure, use !P below
//    long P[long i];      -- returns the ith prime
//    long P.max();        -- returns the largest prime in the list
//    long P.length();     -- returns the number of primes in the list
//    int !P;              -- returns true if the list is empty
//                            (use this to see if find(n) worked)
//  Iterator support:
//    void P.reset();      -- sets an iterator to the beginning
//    long P.next();       -- returns the next prime
//  Output operator:
//    cout << P;           -- prints the list, 10 primes per line.
//
//  The copy constructor and operator= are disabled.
//
//=========================================================================
//
//  Class ReversePrimelist
//
//  This class is an array r[] of indexes into a Primelist P such that
//  P[r[x]] is the smallest prime >= x, with the exception that
//  P[r[x]]=P.max() for x>P.max().
//
//  Constructor:
//    ReversePrimelist R(const Primelist & P, long max=0)
//                         -- initializes R as described above,
//                            for x<=max (if max==0, P.max() is used)
//  Standard functions:
//    long R[long x];      -- returns smallest i such that P[i]>=x (see above)
//    int !R;              -- returns true if the constructor failed
//    long R.length();     -- returns the largest valid x to use in R[x] (max)
//
//  The copy constructor and operator= are disabled.

#ifndef _PRIMELIST
#define _PRIMELIST

using namespace std;
#include <iostream>
#include <iomanip>

// As sent this was
// #include <iostream.h>
// #include <iomanip.h>

#include "Bitvector.h"

class Primelist
{
  private:
    long *P;  // points to array of primes
    long len;  // length of the array
    long pos;  // position for an iterator

  public:
    Primelist(): P(NULL), len(0), pos(0) {}
    Primelist(long N): P(NULL), len(0), pos(0) { find(N); }
    ~Primelist() { if(P!=NULL) delete[] P; }

    inline long operator[](long i) const { return P[i]; }
    inline long max() const              { return P[len-1]; }
    inline long length() const           { return len; }
    inline void reset(long start=0)      { pos=start; }
    inline long next()                   { return P[pos++]; }
    inline int operator!() const         { return len==0; }

    void find(long N)  // finds the primes up to N
    {
      pos=0;
      if(P!=NULL) { delete[] P; len=0; }
      BitvectorP x(N+1);
      if(!x) return;  // failed!

      long i,p,q;
      x.set(2);
      for(i=3; i<=N; i+=2) x.set(i);
      for(p=3; p*p<=N; p+=2)
        for(q=p*p; q<=N; q+=2*p) x.clear(q);
      len=x.count();
      P=new long[len];
      if(P==NULL) { len=0; return; }
      for(i=0, len=0; i<=N; i++) if(x[i]) P[len++]=i;
    }

  private:
    Primelist(const Primelist &); // disabled
    const Primelist & operator=(const Primelist &); // disabled
};

inline ostream & operator<<(ostream & os, const Primelist & P)
{
  long i,p;
  int width=1;
  for(p=P.max(); p>0; p/=10) width++;
  for(i=0; i<P.length(); i++)
    { os << setw(width) << P[i] ; if(i%10==9) os << endl; }
  os << endl;
  return os;
}

//=========================================================================

class ReversePrimelist // gives # of primes < i (EB)
{
    long *r;
    long rlen;
  public:
    ~ReversePrimelist() { if(r!=NULL) delete[] r; }
    long operator[](long i) const { return r[i]; }
    int operator!() const { return rlen==0; }
    long length() const { return rlen; }
    ReversePrimelist(const Primelist & P, long max=0)
    {
      if(max==0) rlen=P.max()+1;
      else rlen=max+1;
      r = new long[rlen];
      if(r==NULL) { rlen=0; return; }
      r[0]=r[1]=r[2]=0;
      for(long i=1; i<P.length(); i++)
        for(long j=P[i-1]+1; j<=P[i]; j++) r[j]=i;
      for(long j=P.max(); j<rlen; j++)
        r[j]=P.length()-1;
    }
  private:
    ReversePrimelist(const ReversePrimelist &); // disabled
    const ReversePrimelist & operator=(const ReversePrimelist &); // disabled
};

#endif
