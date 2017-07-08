// Example program
#include <iostream>
#include <fstream>
#include <vector>
#include <array>

#include "SelfOrganizingMap.h"				// include definition of class SelfOrganizingMap
#include "SimplePopulation.h"				// includes definition of class SimplePopulation
#include "StrongUnits.h"				// includes definition of class StrongUnits
#include "TemporalUnits.h"				// includes definition of class TemporalUnits
#include "StrongPopulation.h"				// includes definition of class StrongPopulation
#include "TemporalPopulation.h"				// includes definition of class TemporalPopulation
#include "../Libraries/OctaveInterface.h"
#include "../Libraries/Utilities.h"

using namespace std;

int main()
{
srand(time(NULL));
int iterations = 1;
int numberOfInputs, inputDim = 3, dim0 = 2, dim1 = 1, dim2 = 5, unitsWinnerPosition;
double learningRate = 0.9, neighborParameter = 0.05, radius = 0.6;
std::string     str;
std::string     STR;
std::vector<int> dimensions = {dim0, dim2};
std::vector<std::vector<double>> input;

// open a file in read mode.
ifstream infile; 
infile.open("../../Octave/Input.mat", ios::in | std::ifstream::binary);
while ( std::getline(infile, str) ) {
        STR = "# name: numberOfInputs";
        if ( str.compare(STR) == 0 ) {
                load_scalar(numberOfInputs, infile);

		while ( std::getline(infile, str) ) {
			STR = "# name: inputVector";
			if ( str.compare(STR) == 0 ) {
				input.resize(numberOfInputs);
				for ( int i = 0; i < numberOfInputs; i++ )
					input[i].resize(inputDim);

				load_matrix_to_vector_of_vectors(input, infile);
			}
		}
	}
}

std::cout << "Input loaded.\n";

std::vector<int>	temporalUnits = {10,9,8,10};	

//TemporalUnits TU(dimensions, inputDim, temporalUnits);
TemporalPopulation TP(dimensions, inputDim, temporalUnits);
//StrongPopulation TP(dimensions, inputDim);

std::cout << "StrongPopulation created.\n";

std::vector<int>	linkingUnits;
linkingUnits.resize(temporalUnits.size());
int n = 4;
for ( int row = 0; row < numberOfInputs; row++ ) {
	auto response = TP.getResponse(input[row]);

	for ( int i = 0; i < (int)temporalUnits.size(); i++ )
		linkingUnits[i] = rand() % temporalUnits[i];

	auto	newResponse = TP.getTemporalResponse(response, linkingUnits);

//*
	std::cout << "\nDistances: \n";
	for(const auto& s: response.distances)
		std::cout << s << ' ';

	std::cout << "\nRanking: \n";
	for(const auto& s: response.ranking)
		std::cout << s << ' '; 

	std::cout << "\nNew Distances: \n";
	for(const auto& s: newResponse.distances)
		std::cout << s << ' ';

	std::cout << "\nNew Ranking: \n";
	for(const auto& s: newResponse.ranking)
		std::cout << s << ' '; 

	std::cout << "\nOutput: \n";
	for(const auto& s: TP.Activate(response, linkingUnits, false))
		std::cout << s << ' '; 

	std::cout << "\nNormalized Output: \n";
	for(const auto& s: TP.Activate(response, linkingUnits, true))
		std::cout << s << ' '; 

	std::cout << "\nOutputs inside radius: \n";
	auto aux = TP.Activate(response, linkingUnits, radius, false);
	for( int i = 0; i < (int)aux.size(); i++ ) {
		for(const auto& s: aux[i])
			std::cout << s << ' ';

		std::cout << "\n";
	}

	std::cout << "\nNormalized Outputs inside radius: \n";
	aux = TP.Activate(response, linkingUnits, radius, true);
	for( int i = 0; i < (int)aux.size(); i++ ) {
		for(const auto& s: aux[i])
			std::cout << s << ' ';

		std::cout << "\n";
	}

	std::cout << "\nn Outputs: \n";
	aux = TP.Activate(response, linkingUnits, false, n);
	for( int i = 0; i < n; i++ ) {
		for(const auto& s: aux[i])
			std::cout << s << ' ';

		std::cout << "\n";
	}

	std::cout << "\nn Normalized Outputs: \n";
	aux = TP.Activate(response, linkingUnits, true, n);
	for( int i = 0; i < n; i++ ) {
		for(const auto& s: aux[i])
			std::cout << s << ' ';

		std::cout << "\n";
	}
	std::cout << "\n\n\n\n\n\n";

//*/

	//auto aux = TP.Activate(response, linkingUnits, radius, true);
	TP.Update(response, linkingUnits, true, 0.001);
	TP.learningRule(learningRate, neighborParameter, response.ranking[0], input[row]);
	learningRate = 0.9 * pow((0.01/0.9),row/numberOfInputs);
	neighborParameter = 5 * pow(0.01,row/numberOfInputs);
}


learningRate = 0.1;
neighborParameter = 0.5;
for ( int iter = 0; iter < iterations; iter++ ) {
	std::cout << "\nIteration number: " << iter+1 << "\n";
	for ( int row = 0; row < numberOfInputs; row++ ) {
		auto response = TP.getResponse(input[row]);

		for ( int i = 0; i < 4; i++ )
			linkingUnits[i] = rand() % temporalUnits[i];

		auto	newResponse = TP.getTemporalResponse(response, linkingUnits);
//*
		std::cout << "\nDistances: \n";
		for(const auto& s: response.distances)
			std::cout << s << ' ';

		std::cout << "\nRanking: \n";
		for(const auto& s: response.ranking)
			std::cout << s << ' '; 

		std::cout << "\nNew Distances: \n";
		for(const auto& s: newResponse.distances)
			std::cout << s << ' ';

		std::cout << "\nNew Ranking: \n";
		for(const auto& s: newResponse.ranking)
			std::cout << s << ' '; 

		std::cout << "\nOutput: \n";
		for(const auto& s: TP.Activate(response, linkingUnits, false))
			std::cout << s << ' '; 

		std::cout << "\nNormalized Output: \n";
		for(const auto& s: TP.Activate(response, linkingUnits, true))
			std::cout << s << ' '; 

		std::cout << "\nOutputs inside radius: \n";
		auto aux = TP.Activate(response, linkingUnits, radius, false);
		for( int i = 0; i < (int)aux.size(); i++ ) {
			for(const auto& s: aux[i])
				std::cout << s << ' ';

			std::cout << "\n";
		}

		std::cout << "\nNormalized Outputs inside radius: \n";
		aux = TP.Activate(response, linkingUnits, radius, true);
		for( int i = 0; i < (int)aux.size(); i++ ) {
			for(const auto& s: aux[i])
				std::cout << s << ' ';

			std::cout << "\n";
		}

		std::cout << "\nn Outputs: \n";
		aux = TP.Activate(response, linkingUnits, false, n);
		for( int i = 0; i < n; i++ ) {
			for(const auto& s: aux[i])
				std::cout << s << ' ';

			std::cout << "\n";
		}

		std::cout << "\nn Normalized Outputs: \n";
		aux = TP.Activate(response, linkingUnits, true, n);
		for( int i = 0; i < n; i++ ) {
			for(const auto& s: aux[i])
				std::cout << s << ' ';

			std::cout << "\n";
		}
		std::cout << "\n\n\n\n\n\n";

//*/

		//aux = TP.Activate(response, linkingUnits, radius, true);
		TP.Update(response, linkingUnits, true, 0.001);
		TP.learningRule(learningRate, neighborParameter, response.ranking[0], input[row]);
		learningRate = 0.1 * pow((0.01/0.9),(row+numberOfInputs*iter)/(iterations*numberOfInputs));
		neighborParameter = 0.5 * pow(0.01,(row+numberOfInputs*iter)/(iterations*numberOfInputs));
	}
}


TP.saveTemporalUnitsStatus();
TP.saveSelfOrganizingMapStatus();
std::cout << "TP_Status saved\n";
}
