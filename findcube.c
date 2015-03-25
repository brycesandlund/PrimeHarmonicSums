#include <iostream.h>
#include <math.h>

int main()
{
  long double left=1.80124093e18l;
  long double right=1.80124152e18l;

  long long x=1000000;
  while ( ((long double)x)*x*x < left) x++;
  cout << x << endl;
  cout << ((long double) x)*x*x << endl;
  return 0;
}
