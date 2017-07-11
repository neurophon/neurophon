// Example program
#include <iostream>
#include <fstream>
#include <vector>

#include <array>

#include "SelfOrganizingMap.h"				// include definition of class SelfOrganizingMap
//#include "../Libraries/Utilities.h"
#include "../Libraries/OctaveInterface.h"
//#include "../Libraries/Templates.h"
//#include "../Libraries/DataTypes.h"
//#include "../Libraries/Constants.h"

using namespace std;

int main()
{
int numberOfInputs = 1000, inputDim = 3, dim = 5, outputDim = 3, unitsWinnerPosition;
double learningRate = 0.1, neighborParameter = 0.05;
std::string     str;
std::string     STR;
std::vector<int> dimensions = {dim,dim,dim};
std::vector<std::vector<double>> input;

input.resize(numberOfInputs);
for ( int i = 0; i < numberOfInputs; i++ )
	input[i].resize(inputDim);

// open a file in read mode.
ifstream infile; 
infile.open("../../Octave/Input.mat", ios::in | std::ifstream::binary);

while ( std::getline(infile, str) ) {
        STR = "# name: inputVector";
        if ( str.compare(STR) == 0 )
                load_matrix_to_vector_of_vectors(input, infile);

}

std::cout << "Input loaded.\n";

SelfOrganizingMap SOM(dimensions, inputDim, dim*dim*dim, outputDim);

for ( int row = 0; row < numberOfInputs; row++ ) {
	SOM.setInputVector(input[row]);
	unitsWinnerPosition = SOM.getResponse().ranking[0];
	SOM.learningRule(learningRate, neighborParameter, unitsWinnerPosition);
	std::cout << "Iteration: " << row+1 << ". Winner unit: " << unitsWinnerPosition << "\n";
}

SOM.saveWeights();

return
}
