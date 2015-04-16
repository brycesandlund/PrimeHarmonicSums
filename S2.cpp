// S2
//
// The user inputs x, and this program computes S2.

#include <NTL/quad_float.h>
#include "Primelist.h"

#define EP 1e-10

using namespace std;
using namespace NTL;

quad_float to_quad_float(long long n)
{
  char s[30];
  sprintf(s,"%lld%c",n,0);
  //cerr << n << " " << s << endl;
  return to_quad_float(s);
}

Bitvector B;
long long lft, rt=0;
quad_float qf_left;
long pos=0;
quad_float x; quad_float cuberootx; quad_float sqrtx;
Primelist P;

void sieve(long long newleft)
{
  //cerr << "\nSieving...\n";
  pos=0;
  lft=newleft;
  qf_left=to_quad_float(lft);
  rt=lft+B.length()-1;
  B.setall();
  double sqrtrt = sqrt((double)rt);
  for(int i=0; i<P.length() && P[i]<=sqrtrt; i++)
  {
    long p=P[i];
    long long first=(lft%p==0)?lft:lft+p-lft%p;
    for(long long q=first-lft; q<=rt-lft; q+=p)
      B.clear(q);
  }
}

quad_float nextprime()
{
  if(rt==0) { sieve(to_long(floor(sqrtx))+1); }
  else if(pos>=B.length()) sieve(rt+1);

  while(pos<B.length() && !B[pos]) pos++;

  if(pos>=B.length()) return nextprime();
  pos++;
  return (qf_left+(pos-1));
}

quad_float sum1p_and_s2_m1(long long input) {
    cuberootx = to_quad_float(exp(log(input)/3));
      quad_float::SetOutputPrecision(30);
      //cerr << "cube root of x: ";
      //cin >> cuberootx;

    //  cuberootx = exp(log(x)/3);
    //  cuberootx = 1216720;
      x = cuberootx * cuberootx * cuberootx;
      cerr << "x = " << x << endl;
      cerr << "cube root = " << cuberootx << endl;
      sqrtx = sqrt(x);
      cerr << "sqrt = " << sqrtx << endl;

      long maxp = to_long(floor(sqrtx));
      P.find(maxp);
      if(!P) { cerr << "Error: unable to allocate space.\n"; return to_quad_float("0"); }
      
      B.setsize(to_long(floor(cuberootx)));

      // code to test nextprime()
      //long long p;
      //while((p=nextprime()) < x) cerr << p << " ";
      //cerr << endl;
      //return 0;

      quad_float sum, sum1, sum2, sum1p;
      sum=0;
      long i;

      sum1p=0;
      for(i=0; i < P.length() && P[i] <= floor(cuberootx); i++) sum1p += 1/to_quad_float(P[i]);
      long a=i-1;
      cerr << "a=" << a << " P[a]=" << P[a] << endl;
      cerr << "sum 1/p up to p_a = " << sum1p << endl;
      cerr << "log log pa + B = " << to_double(log(log(to_quad_float(P[a])))
          +to_quad_float(0.26149)) << endl << endl;

      sum1=0;
      sum2=0;
      long qpos=P.length()-1;
      quad_float q;
      q=nextprime(); // smallest prime >= sqrtx
      cerr << "First prime >= sqrtx : " << q << endl;

      for( i=P.length()-1; i>a; i--)
      {
        quad_float p;
        p=to_quad_float(P[i]);
        //cerr << "p=" << p << endl;
        // going down from sqrtx:
        sum1 += 1/p;

        // going up from sqrtx:
        while(q<=x/p) { sum2+=1/q; q=nextprime(); }

        sum += (sum1+sum2)/p;
      }
      cerr << "sum1=" << sum1 << endl;
      cerr << "sum2=" << sum2 << endl;
      cerr << "sum1+sum2=" << sum1+sum2 << endl;
      cerr << "log log x/p_a - log log p_a ="
           << log(log(x/to_quad_float(P[a]))) - log(log(to_quad_float(P[a]))) 
           << endl;
      cerr << "S2=" << sum << endl;
      cerr << "-1-S2+(sum 1/p up to p_a) = " << -1-sum+sum1p << endl;
      return -1-sum+sum1p;
}