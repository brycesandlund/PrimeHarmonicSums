
// High-precision ordinary node sum
// Uses Briggs "quad float" data type as implemented in NTL
// About 30D accuracy

// Runs in "calculator" mode.

// Reference output (for checking)

// x = 1e18
// N = 1000000
// 405286 ordinary nodes.
// sum of positive terms = 59.0571472746281668524102961026
// sum of negative terms = 58.0546841312934597798383129809
// sum = 1.00246314333470707257198312163
// inconsistency : -0.374708929979980607570571068509e-29
// sum of next term omitted = 0.127574947768196531645385068952e-52

using namespace std;
#include <stdio.h>
#include <iostream>
#include <cmath>

#include <NTL/quad_float.h>  // You need the NTL prefix

using namespace NTL;

#include "Primelist.h"

#define SIZE 2000000

int mu[SIZE];  // I know, this should use a class ...

void muinit(int N)
{
Primelist P(N);
int i;
int p;
int S; 

S = (int) sqrt((double)N);

mu[1] = 1; 
for (i=2;i<N;i++) mu[i] = 1;
P.reset();
for (;;) {
        p = P.next();
        //  -- DEBUG -- cout << "p = " << p << endl;
        for(i = p;i<N;i+=p) mu[i] = -mu[i];
        if (p <= S) for(i = p*p;i<N;i+= p*p) mu[i] = 0;
        if (p == P.max()) break;
        } 
}

int main()
{
quad_float gamma, log2;
gamma = to_quad_float("0.57721566490153286060651209008240243");
log2 =  to_quad_float("0.69314718055994530941723212145817657");
quad_float::SetOutputPrecision(30);

Primelist P(SIZE);
muinit(SIZE);

for (;;) {

        quad_float x,y,y2,y4,
                   phi,t,
                   sum,sumpos,sumneg,sumomit;

        int N,m,count;

        printf("x: ");
        cin >> x;

        N = to_long( floor(exp(log(x)/3)) );   // [ cube root of x ]

        cout << x << endl;
        cout << N << endl;

        sum = 0;
        sumpos = 0;
        sumneg = 0;
        sumomit = 0;
        count = 0;
        for (m=N;m>=1;m--) if (mu[m] && m&01) {
                y = 2*floor((x/m-1)/2) + 1; /* largest odd <= x/m */
                y2 = y*y;
                y4 = y2*y2;
                phi = log(y)/2  + (gamma + log2)/2 + 1/(2*y) - 1/(6*y2); 
                t = mu[m]*phi/m;
                sum += t;
                if (mu[m] > 0) sumpos += phi/m;
                          else sumneg += phi/m;
                sumomit += mu[m]/(15*m*y4);
                count++;
                }

        cout << count << " ordinary nodes." << endl;
        cout << " sum of positive terms = " << sumpos << endl;
        cout << " sum of negative terms = " << sumneg << endl;
        cout << " sum = " << sum << endl;
        cout << " inconsistency : " << sum - (sumpos - sumneg) << endl;
        cout << " sum of next term omitted = " << sumomit << endl;

        }
}

