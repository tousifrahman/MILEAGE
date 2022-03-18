Demo: Setup.h MultiClassTsetlinMachine.cpp MultiClassTsetlinMachine.h TsetlinMachine.cpp TsetlinMachine.h Clause_filter.cpp
	g++ -Wall -O3 -ffast-math -o clause_dropper Clause_filter.cpp MultiClassTsetlinMachine.cpp TsetlinMachine.cpp 

clean:
	rm *.o clause_dropper
