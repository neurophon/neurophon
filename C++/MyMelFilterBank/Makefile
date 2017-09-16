all:
	g++-5 -c -fopenmp -Wall -pedantic -std=c++14 AudioService.cpp

	g++-5 -c -fopenmp -Wall -pedantic -std=c++14 MFCC.cpp

	g++-5 -fopenmp -Wall -pedantic -std=c++14 MFCC.o AudioService.o -lfftw3 -lm

