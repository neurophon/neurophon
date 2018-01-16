# Set the compilers you want to use

# Use g++-5 gcc version 4.8.5 (Ubuntu 4.8.5-2ubuntu1~14.04.1)
#CC = g++-5
CC = g++
#CC = icpc

# Use mpic++ for MPICH version 3.2 (Ubuntu 4.8.5-2ubuntu1~14.04.1)
MPI = mpic++ -cxx=
#MPI = mpicc -cc=

# Use the following Compile-time flags
#CFLAGS = -fopenmp -Wall -pedantic -std=c++14
#CFLAGS = -O3 -mcmodel=large -fopenmp -Wall -pedantic -std=c++14
CFLAGS = -O3 -mcmodel=large -fopenmp -Wall -pedantic -std=c++14

# Use the following libraries
LIBRARIES = ../Libraries/Model/Templates.h\
	    ../Libraries/Model/Constants.h\
	    ../Libraries/Model/DataTypes.h\
	    ../Libraries/Model/OctaveInterface.h\
	    ../Libraries/Model/MatlabInterface.h\
	    ../Libraries/Model/GlobalVariables.h\
	    ../Libraries/Model/Timer.h\
	    ../Libraries/Model/TimerC.h

all: Test S_SOM_Test S_Proc_Test

# Linking compiled files for Test
Test: main.o StaticSelfOrganizingMap.o StaticProcessor.o\
	     DynamicSelfOrganizingMap.o DynamicProcessor.o\
	     ComplexSelfOrganizingMap.o ComplexProcessor.o\
	     EncoderLayer.o RegularLayer.o\
	     Model.o\
	     Utilities.o Random.o Topology.o Timer.o
	$(MPI) $(CC) $(CFLAGS) main.o StaticSelfOrganizingMap.o StaticProcessor.o\
				      DynamicSelfOrganizingMap.o DynamicProcessor.o\
				      ComplexSelfOrganizingMap.o ComplexProcessor.o\
				      EncoderLayer.o RegularLayer.o\
				      Model.o\
				      Utilities.o Random.o Topology.o Timer.o\
				      -o Test

# Linking compiled files S_Proc_Test
S_Proc_Test: StaticProcessorTest.o	StaticProcessor.o\
					Utilities.o Random.o Topology.o Timer.o
	$(MPI) $(CC) $(CFLAGS) StaticProcessorTest.o	StaticProcessor.o\
							Utilities.o Random.o Topology.o Timer.o\
							-o S_Proc_Test
# Linking compiled files S_SOM_Test
S_SOM_Test: StaticSelfOrganizingMapTest.o	StaticSelfOrganizingMap.o\
						Utilities.o Random.o Topology.o Timer.o
	$(MPI) $(CC) $(CFLAGS) StaticSelfOrganizingMapTest.o	StaticSelfOrganizingMap.o\
								Utilities.o Random.o Topology.o Timer.o\
								-o S_SOM_Test

# Compiling main
main.o: main.cpp
	$(MPI) $(CC) -c $(CFLAGS) main.cpp

# Compiling StaticProcessorTest
StaticProcessorTest.o: StaticProcessorTest.cpp $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) StaticProcessorTest.cpp

# Compiling StaticSelfOrganizingMapTest
StaticSelfOrganizingMapTest.o: StaticSelfOrganizingMapTest.cpp $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) StaticSelfOrganizingMapTest.cpp

# Compiling Static self organizing map class
StaticSelfOrganizingMap.o: StaticSelfOrganizingMap.cpp StaticSelfOrganizingMap.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) StaticSelfOrganizingMap.cpp

# Compiling Static processor class
StaticProcessor.o: StaticProcessor.cpp StaticProcessor.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) StaticProcessor.cpp

# Compiling Dynamic self organizing map class
DynamicSelfOrganizingMap.o: DynamicSelfOrganizingMap.cpp DynamicSelfOrganizingMap.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) DynamicSelfOrganizingMap.cpp

# Compiling Dynamic processor class
DynamicProcessor.o: DynamicProcessor.cpp DynamicProcessor.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) DynamicProcessor.cpp

# Compiling Complex self organizing map class
ComplexSelfOrganizingMap.o: ComplexSelfOrganizingMap.cpp ComplexSelfOrganizingMap.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) ComplexSelfOrganizingMap.cpp

# Compiling Complex processor class
ComplexProcessor.o: ComplexProcessor.cpp ComplexProcessor.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) ComplexProcessor.cpp

# Compiling encoder layer class
EncoderLayer.o: EncoderLayer.cpp EncoderLayer.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) EncoderLayer.cpp

# Compiling regular layer class
RegularLayer.o: RegularLayer.cpp RegularLayer.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) RegularLayer.cpp
	
# Compiling model class
Model.o: Model.cpp Model.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) Model.cpp

# Compiling utilities library
Utilities.o: ../Libraries/Model/Utilities.cpp ../Libraries/Model/Utilities.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) ../Libraries/Model/Utilities.cpp

# Compiling random library
Random.o: ../Libraries/Model/Random.cpp ../Libraries/Model/Random.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) ../Libraries/Model/Random.cpp

# Compiling topology library
Topology.o: ../Libraries/Model/Topology.cpp ../Libraries/Model/Topology.h $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) ../Libraries/Model/Topology.cpp

# Compiling Timer library
Timer.o: ../Libraries/Model/Timer.cxx $(LIBRARIES)
	$(MPI) $(CC) -c $(CFLAGS) ../Libraries/Model/Timer.cxx

clean:
	rm *.o Test S_SOM_Test
