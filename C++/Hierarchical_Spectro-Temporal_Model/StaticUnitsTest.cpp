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
int numberOfInputs, inputDim = 3, dim = 50;
double learningRate = 0.1, neighborParameter = 0.05;
std::string     str;
std::string     STR;
std::vector<int> dimensions = {dim,5,5};
std::vector<std::vector<double>> input;

ifstream infile; 
infile.open("../../Octave/Input.mat", ios::in | std::ifstream::binary);

while ( std::getline(infile, str) ) {
        STR = "# name: inputVector";
        if ( str.compare(STR) == 0 )
                load_matrix_to_vector_of_vectors(input, infile);

}

std::cout << "Input loaded.\n";

numberOfInputs = input.size();

std::cout << "Creatin SOM object.\n";
StaticUnits SOM(dimensions, inputDim);
std::cout << "SOM object created.\n";

std::cout << "Processing data.\n";
for ( int row = 0; row < numberOfInputs; row++ ) {
	learningRate = 0.9 * std::pow((0.01/0.9),(row)/(numberOfInputs));
	neighborParameter = 5 *	std::pow(0.01,(row)/(numberOfInputs));
	auto	unitsWinnerPosition = SOM.getResponse(input[row]).ranking[0];
	SOM.learningRule(learningRate, neighborParameter, unitsWinnerPosition, input[row]);
}

for ( int row = 0; row < numberOfInputs; row++ ) {
	learningRate = 0.1 * std::pow((0.01/0.9),(row)/(numberOfInputs));
	neighborParameter = 0.5 *	std::pow(0.01,(row)/(numberOfInputs));
	auto	unitsWinnerPosition = SOM.getResponse(input[row]).ranking[0];
	SOM.learningRule(learningRate, neighborParameter, unitsWinnerPosition, input[row]);
}

learningRate = 0.1*(0.01/0.9);
neighborParameter = 0.5*0.01;
for ( int row = 0; row < numberOfInputs; row++ ) {
	auto	unitsWinnerPosition = SOM.getResponse(input[row]).ranking[0];
	SOM.learningRule(learningRate, neighborParameter, unitsWinnerPosition, input[row]);
}

std::cout << "Saving Object.\n";
std::string	selfOrganizingMapIdentifier = "SOM", fileName = "SOM_Status";
// open a file in write mode.
ofstream outfile;
outfile.open("../../Octave/" + fileName + ".mat", ios::out | ios::trunc);

// file preamble.
outfile << "# This is a file created by saveLayerStatus member function in Layer class from," << endl;
outfile << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
outfile << "# Author: Dematties Dario Jesus." << endl;

outfile << "\n\n" << endl;
	
SOM.saveStaticUnitsStatus(selfOrganizingMapIdentifier,outfile);

// close the opened file.
outfile.close();
std::cout << "Object saved.\n";

return	0;
}
