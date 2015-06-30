// S2
// Computes S2 and other sums (currently in the same function)

#include "utility.h"
#include "Primelist.h"

using namespace std;
using namespace NTL;


void sieve(long long newleft, Bitvector &B, ftype &qf_left, long long &lft, long long &rt, Primelist &P, long &pos)
{
    //cerr << "\nSieving...\n";
    pos=0;
    lft=newleft;
    qf_left=to_ftype(lft);
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

ftype nextprime(Bitvector &B, ftype &qf_left, long long &lft, long long &rt, Primelist &P, long &pos, ftype &sqrtx)
{
    if(rt==0) { sieve(to_long(floor(sqrtx))+1, B, qf_left, lft, rt, P, pos); }
    else if(pos>=B.length()) sieve(rt+1, B, qf_left, lft, rt, P, pos);

    while(pos<B.length() && !B[pos]) pos++;

    if(pos>=B.length()) return nextprime(B, qf_left, lft, rt, P, pos, sqrtx);
    pos++;
    return (qf_left+(pos-1));
}

// Returns the contribution of sum 1/p p <= largest prime less than x^(1/3) minus S2 minus 1
ftype sum1p_and_s2_m1(long long input) {
    Bitvector B;
    long long lft, rt=0;
    ftype qf_left;
    long pos=0;
    ftype x; ftype cuberootx; ftype sqrtx;
    Primelist P;

    cuberootx = to_ftype(exp(log(input)/3));
    ftype::SetOutputPrecision(30);

    x = cuberootx * cuberootx * cuberootx;
    cerr << "x = " << x << endl;
    cerr << "cube root = " << cuberootx << endl;
    sqrtx = sqrt(x);
    cerr << "sqrt = " << sqrtx << endl;

    long maxp = to_long(floor(sqrtx));
    P.find(maxp);
    if(!P) { cerr << "Error: unable to allocate space.\n"; return to_ftype(0); }

    B.setsize(to_long(floor(cuberootx)));

    // code to test nextprime()
    //long long p;
    //while((p=nextprime()) < x) cerr << p << " ";
    //cerr << endl;
    //return 0;

    ftype sum, sum1, sum2, sum1p;
    sum=0;
    long i;

    sum1p=0;
    for(i=0; i < P.length() && P[i] <= floor(cuberootx); i++) sum1p += 1/to_ftype(P[i]);
    long a=i-1;
    cerr << "a=" << a << " P[a]=" << P[a] << endl;
    cerr << "sum 1/p up to p_a = " << sum1p << endl;
    cerr << "log log pa + B = " << to_double(log(log(to_ftype(P[a])))
            +to_ftype(0.26149)) << endl << endl;

    sum1=0;
    sum2=0;
    long qpos=P.length()-1;
    ftype q;
    q=nextprime(B, qf_left, lft, rt, P, pos, sqrtx); // smallest prime >= sqrtx
    cerr << "First prime >= sqrtx : " << q << endl;

    for( i=P.length()-1; i>a; i--)
    {
        ftype p;
        p=to_ftype(P[i]);
        //cerr << "p=" << p << endl;
        // going down from sqrtx:
        sum1 += 1/p;

        // going up from sqrtx:
        while(q<=x/p) { sum2+=1/q; q=nextprime(B, qf_left, lft, rt, P, pos, sqrtx); }

        sum += (sum1+sum2)/p;
    }
    cerr << "sum1=" << sum1 << endl;
    cerr << "sum2=" << sum2 << endl;
    cerr << "sum1+sum2=" << sum1+sum2 << endl;
    cerr << "log log x/p_a - log log p_a ="
        << log(log(x/to_ftype(P[a]))) - log(log(to_ftype(P[a]))) 
        << endl;
    cerr << "S2=" << sum << endl;
    cerr << "-1-S2+(sum 1/p up to p_a) = " << -1-sum+sum1p << endl;
    return -1-sum+sum1p;
}
