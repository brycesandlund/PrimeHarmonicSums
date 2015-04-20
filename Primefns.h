
// Mobius and Largest prime factor functions

#ifndef _PRIMEFNS
#define _PRIMEFNS

#include "Primelist.h"

class Mulist  // table of Mobius function
{

    private:
        char *M;
        int Msize;

    public:

        Mulist(long size) { 
            unsigned int i, p, S; 
            M = new char[size+1];
            Primelist P(size);
            for (i=1;i<=size;i++) M[i] = 1;
            S = (int) sqrt((double)size);
            cerr << "Mulist: size = " << size << " S = " << S << endl;

            P.reset();
            for (;;) {
                p = P.next();
                // Debug cerr << "p = " << p << endl;
                for(i = p;i<=size;i+=p) M[i] = -M[i];
                if (p <= S) for(i = p*p;i<=size;i+= p*p) M[i] = 0; 
                if (p == P.max()) break;
            }
            Msize = size;
        }

        ~Mulist() { delete[]M; }

        inline int mu(long i) { return(M[i]); }

        void print() {
            long i;
            for (i=1;i<=Msize;i++) {
                printf("%ld %d\n",i,M[i]);
            }
        }

};

class Lpflist // table of largest prime factor function
{             // by convention Lpf(1) = 1

    private:
        long *L;
        int Lsize;

    public:
        Lpflist(long size) {
            int i, p;
            L = new long[size+1];
            Primelist P(size);
            for (i=1;i<=size;i++) L[i] = i;
            P.reset();
            for (;;) {
                p = P.next();
                // Debug cerr << "p = " << p << endl;
                for(i = p;i<=size;i+=p) L[i] = p;
                if (p == P.max()) break;
            }
            Lsize = size;
        }

        inline unsigned long lpf(long i) { return(L[i]); }

        void print() {
            long i;
            for (i=1;i<=Lsize;i++) {
                printf("%ld %ld\n",i,L[i]);
            }
        }

};

class Spflist // table of smallest prime factor function
{             // by convention Spf(1) = 1

    private:
        long *S;
        int Ssize;

    public:
        Spflist(long size) {
            unsigned int i, p;
            S = new long[size+1];
            Primelist P(size);
            S[1] = 1; for (i=0;i<=size;i++) S[i] = 0;
            P.reset();
            for (;;) {
                p = P.next();
                // Debug cerr << "p = " << p << endl;
                for(i = p;i<=size;i+=p) if (!S[i]) S[i] = p;
                if (p == P.max()) break;
            }
            Ssize = size;
        }

        ~Spflist() { delete[]S; }

        inline unsigned long spf(long i) { return(S[i]); }

        void print() {
            long i;
            for (i=1;i<=Ssize;i++) {
                printf("%ld %ld\n",i,S[i]);
            }
        }

};

#endif
