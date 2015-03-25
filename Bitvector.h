//  Classes Bitvector and BitvectorP
//
//  These classes supports simple operations on an array of bits.
//  BitvectorP packes 8 bits in a byte, with some performance cost.
//  Otherwise, the two classes are identical, and share the same interface:
//  
//  Constructors:
//    Bitvector B;            -- constructor, sets the length to 0
//    Bitvector B(long n);    -- constructor, sets the length to n
//                               and all bits set to 0
//                               (calls setsize(n) below)
//  Standard functions:
//    long B.length();        -- returns the length
//    long B.count();         -- returns the number of 1 bits
//    void B.setsize(long n); -- resets the length to n (calls clearall())
//    int !B;                 -- returns true if the length is 0
//                               (use this to make sure setsize() worked)
//  Accessing bits in the vector:
//    void B.clearall();      -- sets all bits to 0
//    void B.setall();        -- sets all bits to 1
//    void B.set(long pos);   -- does B[pos]=1 (inline)
//    void B.clear(long pos); -- does B[pos]=0 (inline)
//    int B[long pos];        -- returns the bit in position pos (inline)
//  Output operator:
//    cout << B;              -- prints the vector, 80 bits per line
//
//  The copy constructor and operator= are disabled.

#ifndef _BITVECTOR
#define _BITVECTOR

#include <iostream>
// #include "pause.h"

class Bitvector
{
  unsigned char *x;
  long len;
  long xlen;
public:
  Bitvector(): x(NULL), len(0), xlen(0) {}
  Bitvector(long bsize): x(NULL), len(0), xlen(0) { setsize(bsize); }
  ~Bitvector() { if(x!=NULL) delete[] x; }
  inline long length() const { return len; }
  inline int operator!() const { return x==NULL; }
  long count() const
  {
    long cnt=0;
    for(long i=0; i<len; i++)
      cnt+=(*this)[i];
    return cnt;
  }
  void setsize(long bsize)
  {
    if(x!=NULL) delete[] x;
    x = new unsigned char[bsize];
    if(x==NULL)
      { len=xlen=0;
        cerr<<"In Bitvector::setsize(): insufficient memory.\n"; return; }
    len=xlen=bsize;
    clearall();
  }
  void clearall() { for(long i=0; i<xlen; i++) x[i]=0; }
  inline int operator[](long i) const { return x[i]; }
  inline void set(long i) { x[i]=1; }
  inline void clear(long i) { x[i]=0; }
  void setall() { for(long i=0; i<xlen; i++) x[i]=1; }
private:
  Bitvector(const Bitvector &); // disabled
  const Bitvector & operator=(const Bitvector &); // disabled
};

inline ostream & operator<<(ostream & os, const Bitvector & B)
{
  for(long i=0; i<B.length(); i++)
    { os << B[i]; if(i%80==79 && i+1<B.length()) os << endl; }
  os << endl;
  return os;
}

class BitvectorP
{
  unsigned char *x;
  long len;
  long xlen;
public:
  BitvectorP(): x(NULL), len(0), xlen(0) {}
  BitvectorP(long bsize): x(NULL), len(0), xlen(0) { setsize(bsize); }
  ~BitvectorP() { if(x!=NULL) delete[] x; }
  inline long length() const { return len; }
  inline int operator!() const { return x==NULL; }
  long count() const
  {
    long cnt=0;
    for(long i=0; i<len; i++)
      cnt+=(*this)[i];
    return cnt;
  }
  void setsize(long bsize)
  {
    long newxlen;
    len=bsize;
    newxlen=(bsize+7)/8+1;
    if(newxlen>xlen)
    {
      if(x!=NULL) delete[] x;
      xlen=newxlen;
      x = new unsigned char[xlen];
      if(x==NULL)
        { len=xlen=0;
          cerr<<"In BitvectorP::setsize(): insufficient memory.\n"; return; }
    }
    clearall();
  }
  void clearall() { for(long i=0; i<xlen; i++) x[i]=0; }
//inline int operator[](long i) const { check(i); return ((x[i>>3]>>(i&7))&1); }
  inline int operator[](long i) const { return ((x[i>>3]>>(i&7))&1); }
//inline void set(long i) { check(i); x[i>>3]|=(1<<(i&7)); }
  inline void set(long i) { x[i>>3]|=(1<<(i&7)); }
//inline void clear(long i) { check(i); x[i>>3]&= ~(1<<(i&7)); }
  inline void clear(long i) { x[i>>3]&= ~(1<<(i&7)); }
  void setall() { for(long i=0; i<xlen; i++) x[i]=(1<<8)-1; }
private:
  void check(long i) const
    { if((i<0)||(i>=len))
      {cout <<"Error: "<<i<<" out of range" << endl; /* pause(); */ }}
  BitvectorP(const BitvectorP & B); // disabled
  const BitvectorP & operator=(const BitvectorP & B); // disabled
};

inline ostream & operator<<(ostream & os, const BitvectorP & B)
{
  for(long i=0; i<B.length(); i++)
    { os << B[i]; if(i%80==79 && i+1<B.length()) os << endl; }
  os << endl;
  return os;
}

#endif
