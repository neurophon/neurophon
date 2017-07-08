
Test: main.o SelfOrganizingMap.o StaticUnits.o StrongUnits.o TemporalUnits.o SimplePopulation.o StrongPopulation.o TemporalPopulation.o Layer.o Utilities.o Random.o Topology.o
	g++-5 -fopenmp -Wall -std=c++14 main.o SelfOrganizingMap.o StaticUnits.o StrongUnits.o TemporalUnits.o SimplePopulation.o StrongPopulation.o TemporalPopulation.o Layer.o Utilities.o Random.o Topology.o -o Test

main.o: main.cpp
	g++-5 -c -fopenmp -Wall -std=c++14 main.cpp

SelfOrganizingMap.o: SelfOrganizingMap.cpp SelfOrganizingMap.h
	g++-5 -c -fopenmp -Wall -std=c++14 SelfOrganizingMap.cpp

StaticUnits.o: StaticUnits.cpp StaticUnits.h
	g++-5 -c -fopenmp -Wall -std=c++14 StaticUnits.cpp

StrongUnits.o: StrongUnits.cpp StrongUnits.h
	g++-5 -c -fopenmp -Wall -std=c++14 StrongUnits.cpp

TemporalUnits.o: TemporalUnits.cpp TemporalUnits.h
	g++-5 -c -fopenmp -Wall -std=c++14 TemporalUnits.cpp

SimplePopulation.o: SimplePopulation.cpp SimplePopulation.h
	g++-5 -c -fopenmp -Wall -std=c++14 SimplePopulation.cpp

StrongPopulation.o: StrongPopulation.cpp StrongPopulation.h
	g++-5 -c -fopenmp -Wall -std=c++14 StrongPopulation.cpp

TemporalPopulation.o: TemporalPopulation.cpp TemporalPopulation.h
	g++-5 -c -fopenmp -Wall -std=c++14 TemporalPopulation.cpp

Layer.o: Layer.cpp Layer.h
	g++-5 -c -fopenmp -Wall -std=c++14 Layer.cpp

Utilities.o: ../Libraries/Utilities.cpp ../Libraries/Utilities.h
	g++-5 -c -fopenmp -Wall -std=c++14 ../Libraries/Utilities.cpp

Random.o: ../Libraries/Random.cpp ../Libraries/Random.h
	g++-5 -c -fopenmp -Wall -std=c++14 ../Libraries/Random.cpp

Topology.o: ../Libraries/Topology.cpp ../Libraries/Topology.h
	g++-5 -c -fopenmp -Wall -std=c++14 ../Libraries/Topology.cpp

clean:
	rm *.o Test
