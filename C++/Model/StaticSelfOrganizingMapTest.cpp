// Example program
// This main program test how StaticSelfOrganizingMap class works in its Self Organized Map function
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <omp.h>

#include "StaticSelfOrganizingMap.h"				// include definition of class StaticSelfOrganizingMap
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/OctaveInterface.h"
#include "../Libraries/Model/MatlabInterface.h"
#include "../Libraries/Model/Templates.h"
#include "../Libraries/Model/DataTypes.h"
#include "../Libraries/Model/Constants.h"
#include "../Libraries/Model/Random.h"

using namespace std;

bool	big_endianness;

int main()
{
std::size_t	numberOfInputs, inputDim = 3, dim = 10;
double learningRate = 0.1, neighborParameter = 0.05;
std::string     str;
std::string     STR;
std::vector<std::size_t> dimensions = {dim,dim,dim};
twodvector<double>	input;

ifstream infile; 
infile.open("../../Octave/inputs.mat", ios::in | std::ifstream::binary);

if (ENABLE_MATLAB_COMPATIBILITY) {
	big_endianness = load_the_header(infile);
	auto	array_structure = check_next_data_structure(infile, big_endianness);
	while ( array_structure.more_data ) {
		STR = "inputVector";
		if ( array_structure.name.compare(STR) == 0 )
			load_numeric_array_to_vector_of_vectors(array_structure, input, infile, big_endianness);

		array_structure = check_next_data_structure(infile,big_endianness);
	}
}
else {
	while ( std::getline(infile, str) ) {
		STR = "# name: inputVector";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector_of_vectors(input, infile);

	}
}

std::cout << "Input loaded.\n";

numberOfInputs = input.size();

std::cout << "Creatin SOM object.\n";
StaticSelfOrganizingMap SOM(dimensions, inputDim);
std::cout << "SOM object created.\n";

std::cout << "Processing data.\n";
for ( std::size_t row = 0; row < numberOfInputs; row++ ) {
	learningRate = 0.9 * std::pow((0.01/0.9),(double)row/(double)numberOfInputs);
	neighborParameter = 5 *	std::pow(0.01,(double)row/(double)numberOfInputs);
	auto	unitsWinnerPosition = SOM.getResponse(input[row]).ranking[0];
	SOM.learningRule(learningRate, neighborParameter, unitsWinnerPosition, input[row]);
}

for ( std::size_t row = 0; row < numberOfInputs; row++ ) {
	learningRate = 0.1 * std::pow((0.01/0.9),(double)row/(double)numberOfInputs);
	neighborParameter = 0.5 *	std::pow(0.01,(double)row/(double)numberOfInputs);
	auto	unitsWinnerPosition = SOM.getResponse(input[row]).ranking[0];
	SOM.learningRule(learningRate, neighborParameter, unitsWinnerPosition, input[row]);
}

learningRate = 0.1*(0.01/0.9);
neighborParameter = 0.5*0.01;
for ( std::size_t row = 0; row < numberOfInputs; row++ ) {
	auto	unitsWinnerPosition = SOM.getResponse(input[row]).ranking[0];
	SOM.learningRule(learningRate, neighborParameter, unitsWinnerPosition, input[row]);
}

std::cout << "Saving Object.\n";
std::string	selfOrganizingMapIdentifier = "SOM", fileName = "SOM_Status";

// stream to put the formated data to be saved in the file
std::stringstream	outstream;

// file preamble.
if (ENABLE_MATLAB_COMPATIBILITY) {
	save_the_header(outstream);
}
else {
	outstream << "# This is a file created by saveLayerStatus member function in Layer class from," << endl;
	outstream << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
	outstream << "# Author: Dematties Dario Jesus." << endl;

	outstream << "\n\n" << endl;
}
	
SOM.saveStaticSelfOrganizingMapStatus(selfOrganizingMapIdentifier,outstream);



// open a file in write mode.
ofstream outfile;
outfile.open("../../Octave/" + fileName + ".mat", ios::out | ios::trunc);

// write the file with the stream
outfile << outstream.rdbuf();

// close the opened file.
outfile.close();
std::cout << "Object saved.\n";

return	0;
}
