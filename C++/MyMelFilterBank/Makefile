# Set the compilers you want to use

# Use g++-5 gcc version 4.8.5 (Ubuntu 4.8.5-2ubuntu1~14.04.1)
CC = g++-5
#CC = g++
#CC = icpc

# Use mpic++ for MPICH version 3.2 (Ubuntu 4.8.5-2ubuntu1~14.04.1)
MPI = mpic++ -cxx=
#MPI = mpicc -cc=

# Use the following Compile-time flags
# IMPORTANT!!! DO NOT APPLY OPTIMIZATION
#CFLAGS = -Wall -pedantic -std=c++14
CFLAGS = -fopenmp -Wall -pedantic -std=c++14

# Use the following libraries
LIBRARIES = ../Libraries/Model/Templates.h\
	    ../Libraries/Model/Constants.h\
	    ../Libraries/Model/DataTypes.h\
	    ../Libraries/Model/OctaveInterface.h\
	    ../Libraries/Model/MatlabInterface.h\
	    ../Libraries/Model/GlobalVariables.h


# Linking compiled files
Test: main.o MFCC.o AudioService.o Utilities.o
	$(MPI) $(CC) $(CFLAGS) main.o MFCC.o AudioService.o Utilities.o -lfftw3 -lm -o Test


# Compiling main.cpp
main.o: main.cpp $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) main.cpp

# Compiling MFCC.cpp
MFCC.o: MFCC.cpp MFCC.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) MFCC.cpp

# Compiling AudioService.cpp
AudioService.o: AudioService.cpp AudioService.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) AudioService.cpp

# Compiling Utilities.cpp
Utilities.o: ../Libraries/Model/Utilities.cpp ../Libraries/Model/Utilities.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) ../Libraries/Model/Utilities.cpp


clean:
	rm *.o Test
