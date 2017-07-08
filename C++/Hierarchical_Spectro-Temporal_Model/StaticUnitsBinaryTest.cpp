// Example program
// This main program test how StaticUnits class works in its Self Organized Map function
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <omp.h>

#include "StaticUnits.h"				// include definition of class StaticUnits
#include "../Libraries/Utilities.h"
#include "../Libraries/OctaveInterface.h"
#include "../Libraries/Templates.h"
#include "../Libraries/DataTypes.h"
#include "../Libraries/Constants.h"
#include "../Libraries/Random.h"

using namespace std;

int main()
{
int iterations = 50, numberOfInputs, inputDim = 32*32, dim = 16;
double learningRate = 0.1, neighborParameter = 0.05, plasticity = 0.01, sparsity = 0.01;
std::string     str;
std::string     STR;
std::vector<int> dimensions = {dim,dim};
std::vector<std::vector<std::size_t>> input;

std::size_t	numberOfActiveUnits = sparsity*std::accumulate(dimensions.begin(), dimensions.end(), 1, std::multiplies<int>());
ifstream infile; 
infile.open("../../Octave/Input.mat", ios::in | std::ifstream::binary);

while ( std::getline(infile, str) ) {
        STR = "# name: inputVector";
        if ( str.compare(STR) == 0 )
                load_cell_to_multidimensional_vector(input, infile);

}

std::cout << "Input loaded.\n";

numberOfInputs = input.size();

std::cout << "Creating CorticalColumn object.\n";
StaticUnits CorticalColumn(dimensions, inputDim,0.6,{SYNAPTIC_DECREMENT,SYNAPTIC_INCREMENT},true);
std::cout << "CorticalColumn object created.\n";

std::cout << "Processing data.\n";
std::vector<std::vector<std::size_t>>	output;
for ( int iteration = 0; iteration < iterations; iteration++ ) {
	for ( int row = 0; row < numberOfInputs; row++ ) {
		learningRate = 0.9 * std::pow((0.01/0.9),(row+numberOfInputs*iteration)/(numberOfInputs*iterations));
		neighborParameter = 5 *	std::pow(0.01,(row+numberOfInputs*iteration)/(numberOfInputs*iterations));
		auto	response = CorticalColumn.learningRule(learningRate,
							       neighborParameter,
							       plasticity,
							       input[row],
							       PROXIMAL_SYNAPTIC_THRESHOLD,
							       true, false);
		CorticalColumn.homeostasis(true, true, true, PROXIMAL_SYNAPTIC_THRESHOLD);
		std::vector<std::size_t>	auxiliary(response.ranking.end()-std::min((int)numberOfActiveUnits,(int)response.ranking.size()),
							  response.ranking.end());
		output.push_back(auxiliary);
	}
}

for ( int iteration = 0; iteration < iterations; iteration++ ) {
	for ( int row = 0; row < numberOfInputs; row++ ) {
		learningRate = 0.1 * std::pow((0.01/0.9),(row+numberOfInputs*iteration)/(numberOfInputs*iterations));
		neighborParameter = 0.5*std::pow(0.01,(row+numberOfInputs*iteration)/(numberOfInputs*iterations));
		auto	response = CorticalColumn.learningRule(learningRate,
							       neighborParameter,
							       plasticity,
							       input[row],
							       PROXIMAL_SYNAPTIC_THRESHOLD,
							       true, false);
		CorticalColumn.homeostasis(true, true, true, PROXIMAL_SYNAPTIC_THRESHOLD);
		std::vector<std::size_t>	auxiliary(response.ranking.end()-std::min((int)numberOfActiveUnits,(int)response.ranking.size()),
							  response.ranking.end());
		output.push_back(auxiliary);
	}
}

learningRate = 0.1*(0.01/0.9);
neighborParameter = 0.5*0.01;
for ( int iteration = 0; iteration < iterations; iteration++ ) {
	for ( int row = 0; row < numberOfInputs; row++ ) {
		auto	response = CorticalColumn.learningRule(learningRate,
							       neighborParameter,
							       plasticity,
							       input[row],
							       PROXIMAL_SYNAPTIC_THRESHOLD,
							       true, false);
		CorticalColumn.homeostasis(true, true, true, PROXIMAL_SYNAPTIC_THRESHOLD);
		std::vector<std::size_t>	auxiliary(response.ranking.end()-std::min((int)numberOfActiveUnits,(int)response.ranking.size()),
							 response.ranking.end());
		output.push_back(auxiliary);
	}
}

std::cout << "Saving Object.\n";
std::string	selfOrganizingMapIdentifier = "CorticalColumn", fileName = "CorticalColumn_Status";
// open a file in write mode.
ofstream outfile;
outfile.open("../../Octave/" + fileName + ".mat", ios::out | ios::trunc);

// file preamble.
outfile << "# This is a file created by saveLayerStatus member function in Layer class from," << endl;
outfile << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
outfile << "# Author: Dematties Dario Jesus." << endl;

outfile << "\n\n" << endl;
	
CorticalColumn.saveStaticUnitsStatus(selfOrganizingMapIdentifier,outfile);

// close the opened file.
outfile.close();
std::cout << "Object saved.\n";

std::cout << "Saving Output.\n";
fileName = "CorticalColumnOutput";
// open a file in write mode.
outfile.open("../../Octave/" + fileName + ".mat", ios::out | ios::trunc);

// file preamble.
outfile << "# This is a file created by saveLayerStatus member function in Layer class from," << endl;
outfile << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
outfile << "# Author: Dematties Dario Jesus." << endl;

outfile << "\n\n" << endl;

save_multidimensional_vector_as_cell("output",output,outfile);
save_vector_as_matrix("dimensions",dimensions,outfile);

// close the opened file.
outfile.close();
std::cout << "Output saved.\n";

return	0;
}
