
// Checker
//
// The user inputs a goal, and this program computes x such that sum 1/p >=goal

using namespace std;
#include <NTL/quad_float.h>
using namespace NTL;
#include "Primelist.h"


int main()
{
  quad_float x; 
  Primelist P;
  quad_float::SetOutputPrecision(30);

  double goal;
  //cout << "Enter goal: ";
  //cin >> goal;
  goal=3.25;
  x = exp(exp(goal-0.26149));
  //x = 173.0*173.0*173.0;
  x = x*1.1;
  cout << "Estimate for x: " << x << endl;

  P.find(to_long(x));
  if(!P) { cout << "Error: unable to allocate space.\n"; return 0; }

  quad_float sum1p, sumprev;
  long i;
  sum1p=0;
  for(i=0; i<P.length() && sum1p<goal; i++)
    { sumprev=sum1p; sum1p += 1/to_quad_float(P[i]); }

  cout << "sum 1/p up to " << P[i-1] <<" is " << sum1p << endl;
  cout << "Previous sum = " << sumprev << endl;
  cout << "Previous prime = " << P[i-2] << endl;

  return 0;
}
