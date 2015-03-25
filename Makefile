# compile with -pg to get profile
IDIR = $(HOME)/sw/include
LDIR = $(HOME)/sw/lib
clstest : clstest.cpp
	g++ -I$(IDIR) -L$(LDIR) clstest.cpp -O3 -lntl -lm -o clstest
clstest.s : clstest.cpp
	g++ -I$(IDIR) -L$(LDIR) clstest.cpp -lntl -lm -S
qftest : qftest.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 qftest.cpp -lntl -lm -o qftest
RRtest : RRtest.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 RRtest.cpp -lntl -lm -o RRtest
qftest.s : qftest.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 qftest.cpp -lntl -lm -S
psum : psum.cpp
	g++ -I$(IDIR) -L$(LDIR) psum.cpp -lntl -lm -o psum
ordhi : ordhi.cpp
	g++ -I$(IDIR) -L$(LDIR) ordhi.cpp -lntl -lm -o ordhi
endgamehi : endgamehi.cpp
	g++ -I$(IDIR) -L$(LDIR) endgamehi.cpp -lntl -lm -o endgamehi
endgame : endgame.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 endgame.cpp -lntl -lm -o endgame
special : special.cpp RangeArray.h Primefns.h
	g++ -I$(IDIR) -L$(LDIR) special.cpp -O3 -lntl -lm -o special
opt : opt.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 opt.cpp -lntl -lm -o opt
test : test.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 test.cpp -lntl -lm -o test
shn : shn.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 shn.cpp -lntl -lm -o shn
