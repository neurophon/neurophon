Test: main.o StaticSelfOrganizingMap.o StaticProcessor.o DynamicSelfOrganizingMap.o DynamicProcessor.o ComplexSelfOrganizingMap.o ComplexProcessor.o EncoderLayer.o RegularLayer.o Model.o Utilities.o Random.o Topology.o
	mpic++ -cxx=g++-5 -fopenmp -Wall -pedantic -std=c++14 main.o StaticSelfOrganizingMap.o StaticProcessor.o DynamicSelfOrganizingMap.o DynamicProcessor.o ComplexSelfOrganizingMap.o ComplexProcessor.o EncoderLayer.o RegularLayer.o Model.o Utilities.o Random.o Topology.o -o Test

main.o: main.cpp
	mpic++ -cxx=g++-5 -c -fopenmp -Wall -pedantic -std=c++14 main.cpp

StaticSelfOrganizingMap.o: StaticSelfOrganizingMap.cpp StaticSelfOrganizingMap.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	g++-5 -c -fopenmp -Wall -pedantic -std=c++14 StaticSelfOrganizingMap.cpp

StaticProcessor.o: StaticProcessor.cpp StaticProcessor.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	g++-5 -c -fopenmp -Wall -pedantic -std=c++14 StaticProcessor.cpp

DynamicSelfOrganizingMap.o: DynamicSelfOrganizingMap.cpp DynamicSelfOrganizingMap.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	g++-5 -c -fopenmp -Wall -pedantic -std=c++14 DynamicSelfOrganizingMap.cpp

DynamicProcessor.o: DynamicProcessor.cpp DynamicProcessor.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	g++-5 -c -fopenmp -Wall -pedantic -std=c++14 DynamicProcessor.cpp

ComplexSelfOrganizingMap.o: ComplexSelfOrganizingMap.cpp ComplexSelfOrganizingMap.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	g++-5 -c -fopenmp -Wall -pedantic -std=c++14 ComplexSelfOrganizingMap.cpp

ComplexProcessor.o: ComplexProcessor.cpp ComplexProcessor.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	g++-5 -c -fopenmp -Wall -pedantic -std=c++14 ComplexProcessor.cpp

EncoderLayer.o: EncoderLayer.cpp EncoderLayer.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	mpic++ -cxx=g++-5 -c -fopenmp -Wall -pedantic -std=c++14 EncoderLayer.cpp

RegularLayer.o: RegularLayer.cpp RegularLayer.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	mpic++ -cxx=g++-5 -c -fopenmp -Wall -pedantic -std=c++14 RegularLayer.cpp
	
Model.o: Model.cpp Model.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	mpic++ -cxx=g++-5 -c -fopenmp -Wall -pedantic -std=c++14 Model.cpp

Utilities.o: ../Libraries/Model/Utilities.cpp ../Libraries/Model/Utilities.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	g++-5 -c -fopenmp -Wall -pedantic -std=c++14 ../Libraries/Model/Utilities.cpp

Random.o: ../Libraries/Model/Random.cpp ../Libraries/Model/Random.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	g++-5 -c -fopenmp -Wall -pedantic -std=c++14 ../Libraries/Model/Random.cpp

Topology.o: ../Libraries/Model/Topology.cpp ../Libraries/Model/Topology.h ../Libraries/Model/Templates.h ../Libraries/Model/Constants.h ../Libraries/Model/DataTypes.h ../Libraries/Model/OctaveInterface.h
	g++-5 -c -fopenmp -Wall -pedantic -std=c++14 ../Libraries/Model/Topology.cpp

clean:
	rm *.o Test
