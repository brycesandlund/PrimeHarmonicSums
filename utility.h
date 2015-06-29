#ifndef _UTILITY
#define _UTILITY

#include <cstdio>
#include <NTL/quad_float.h>
//  #include <NTL/RR.h>  // If using RR -- remember to fix RangeArray.h too

using namespace std;
using namespace NTL;

typedef quad_float ftype;

quad_float to_quad_float(long long n)
{
    char s[30];
    sprintf(s,"%lld%c",n,0);
    //cerr << n << " " << s << endl;
    return to_quad_float(s);
}

#endif
