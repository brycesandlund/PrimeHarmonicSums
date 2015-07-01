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
endgame_main : endgame_main.cpp endgame.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 endgame_main.cpp -lntl -lm -o endgame_main
special_main : special.cpp special_main.cpp RangeArray.h Primefns.h
	g++ -I$(IDIR) -L$(LDIR) special_main.cpp -O3 -lntl -lm -o special_main
ordinary_main : ordinary_main.cpp ordinary.cpp
	g++ -I$(IDIR) -L$(LDIR) ordinary_main.cpp -O3 -lntl -lm -o ordinary_main
checker : checker.cpp
	g++ -I$(IDIR) -L$(LDIR) checker.cpp -O3 -lntl -lm -o checker
S2_main : S2_main.cpp S2.cpp 
	g++ -I$(IDIR) -L$(LDIR) -O3 S2_main.cpp -lntl -lm -o S2_main
opt : opt.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 opt.cpp -lntl -lm -o opt
test_fullsum : test_fullsum.cpp special.cpp ordinary.cpp S2.cpp RangeArray.h Primefns.h
	g++ -I$(IDIR) -L$(LDIR) -O3 test_fullsum.cpp -lntl -lm -o test_fullsum
test_special : test_special.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 test_special.cpp -lntl -lm -o test_special
test_endgame : test_endgame.cpp endgame.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 test_endgame.cpp -lntl -lm -o test_endgame
shn : shn.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 shn.cpp -lntl -lm -o shn
fullsum : fullsum.cpp RangeArray.h special.cpp ordinary.cpp S2.cpp Primefns.h utility.h
	g++ -I$(IDIR) -L$(LDIR) -O3 fullsum.cpp -lntl -lm -o fullsum
crossover : crossover.cpp RangeArray.h special.cpp ordinary.cpp S2.cpp Primefns.h endgame.cpp sinterval.cpp utility.h
	g++ -I$(IDIR) -L$(LDIR) -O3 crossover.cpp -lntl -lm -o crossover
blocksieve_main : blocksieve.cpp blocksieve_main
	g++ -I$(IDIR) -L$(LDIR) -O3 blocksieve_main.cpp -lntl -lm -o blocksieve_main
sinterval_main : sinterval_main.cpp
	g++ -I$(IDIR) -L$(LDIR) -O3 sinterval_main.cpp -lntl -lm -o sinterval_main
