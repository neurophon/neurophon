# Set the compilers you want to use

# Use g++-5 gcc version 4.8.5 (Ubuntu 4.8.5-2ubuntu1~14.04.1)
CC = g++-5

# Use mpic++ for MPICH version 3.2 (Ubuntu 4.8.5-2ubuntu1~14.04.1)
MPI = mpic++

# Use the following Compile-time flags
CFLAGS = -fopenmp -Wall -pedantic -std=c++14

# Use the following libraries
LIBRARIES = ../Libraries/Model/Templates.h\
	    ../Libraries/Model/Constants.h\
	    ../Libraries/Model/DataTypes.h\
	    ../Libraries/Model/OctaveInterface.h

# Linking compiled files
Test: main.o StaticSelfOrganizingMap.o StaticProcessor.o\
	     DynamicSelfOrganizingMap.o DynamicProcessor.o\
	     ComplexSelfOrganizingMap.o ComplexProcessor.o\
	     EncoderLayer.o RegularLayer.o\
	     Model.o\
	     Utilities.o Random.o Topology.o
	$(MPI) -cxx=$(CC) $(CFLAGS) main.o StaticSelfOrganizingMap.o StaticProcessor.o\
					   DynamicSelfOrganizingMap.o DynamicProcessor.o\
					   ComplexSelfOrganizingMap.o ComplexProcessor.o\
					   EncoderLayer.o RegularLayer.o\
					   Model.o\
					   Utilities.o Random.o Topology.o\
					   -o Test

# Compiling main
main.o: main.cpp
	$(MPI) -cxx=$(CC) -c $(CFLAGS) main.cpp

# Compiling Static self organizing map class
StaticSelfOrganizingMap.o: StaticSelfOrganizingMap.cpp StaticSelfOrganizingMap.h $(LIBRARIES)
	$(CC) -c $(CFLAGS) StaticSelfOrganizingMap.cpp

# Compiling Static processor class
StaticProcessor.o: StaticProcessor.cpp StaticProcessor.h $(LIBRARIES)
	$(CC) -c $(CFLAGS) StaticProcessor.cpp

# Compiling Dynamic self organizing map class
DynamicSelfOrganizingMap.o: DynamicSelfOrganizingMap.cpp DynamicSelfOrganizingMap.h $(LIBRARIES)
	$(CC) -c $(CFLAGS) DynamicSelfOrganizingMap.cpp

# Compiling Dynamic processor class
DynamicProcessor.o: DynamicProcessor.cpp DynamicProcessor.h $(LIBRARIES)
	$(CC) -c $(CFLAGS) DynamicProcessor.cpp

# Compiling Complex self organizing map class
ComplexSelfOrganizingMap.o: ComplexSelfOrganizingMap.cpp ComplexSelfOrganizingMap.h $(LIBRARIES)
	$(CC) -c $(CFLAGS) ComplexSelfOrganizingMap.cpp

# Compiling Complex processor class
ComplexProcessor.o: ComplexProcessor.cpp ComplexProcessor.h $(LIBRARIES)
	$(CC) -c $(CFLAGS) ComplexProcessor.cpp

# Compiling encoder layer class
EncoderLayer.o: EncoderLayer.cpp EncoderLayer.h $(LIBRARIES)
	$(MPI) -cxx=$(CC) -c $(CFLAGS) EncoderLayer.cpp

# Compiling regular layer class
RegularLayer.o: RegularLayer.cpp RegularLayer.h $(LIBRARIES)
	$(MPI) -cxx=$(CC) -c $(CFLAGS) RegularLayer.cpp
	
# Compiling model class
Model.o: Model.cpp Model.h $(LIBRARIES)
	$(MPI) -cxx=$(CC) -c $(CFLAGS) Model.cpp

# Compiling utilities library
Utilities.o: ../Libraries/Model/Utilities.cpp ../Libraries/Model/Utilities.h $(LIBRARIES)
	$(CC) -c $(CFLAGS) ../Libraries/Model/Utilities.cpp

# Compiling random library
Random.o: ../Libraries/Model/Random.cpp ../Libraries/Model/Random.h $(LIBRARIES)
	$(CC) -c $(CFLAGS) ../Libraries/Model/Random.cpp

# Compiling topology library
Topology.o: ../Libraries/Model/Topology.cpp ../Libraries/Model/Topology.h $(LIBRARIES)
	$(CC) -c $(CFLAGS) ../Libraries/Model/Topology.cpp

clean:
	rm *.o Test
