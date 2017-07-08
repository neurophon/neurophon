//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				Author:		Dematties Dario Jesus						//
//				Contact:	dariodematties@hotmail.com.ar					//
//						dariodematties@yahoo.com.ar					//
//						dario.dematties@frm.utn.edu.ar					//
//				Project:	Engineering PhD Project						//
//				Institution:	Universidad de Buenos Aires					//
//						Facultad de Ingeniería (FIUBA)					//
//				Workplace:	Instituto de Ingeniería						//
//						Biomédica FIUBA	&						//
//						CCT CONICET Mendoza INCIHUSA					//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// File Name:		SelfOrganizingMap.cpp
// File Description:	SelfOrganizingMap member-function definitions. This file contains implementations of the
//			member functions prototyped in SelfOrganizingMap.h.

#include <iostream>
#include <algorithm>

#include "SelfOrganizingMap.h"				// include definition of class SelfOrganizingMap
#include "../Libraries/Utilities.h"
#include "../Libraries/OctaveInterface.h"
#include "../Libraries/Templates.h"
#include "../Libraries/DataTypes.h"
#include "../Libraries/Random.h"

using namespace std;

// constructor that initializes _weights at random
SelfOrganizingMap::SelfOrganizingMap( const std::vector<int>& unitsArrayDimensionality, int inputDimensionality )
{
	_updateStep = 0;
	_inputDimensionality = inputDimensionality;
	_unitsDimensionality = std::accumulate(unitsArrayDimensionality.begin(), unitsArrayDimensionality.end(), 1, std::multiplies<int>());
	_unitsArrayDimensionality.resize(unitsArrayDimensionality.size());
	for ( unsigned int dim = 0; dim < unitsArrayDimensionality.size(); dim++ )
		_unitsArrayDimensionality[dim] = unitsArrayDimensionality[dim];

	SelfOrganizingMap::validateObject();

	_weights.resize(_unitsDimensionality);
	for ( int row = 0; row < _unitsDimensionality; row++ )
		_weights[row].resize(inputDimensionality);

	for ( int row = 0; row < _unitsDimensionality; row++ )
		for ( int column = 0; column < inputDimensionality; column++ )
			_weights[row][column] = ((double) rand() / (RAND_MAX));		// generate secret number between 0 and 1
} // end SelfOrganizingMap default constructor


// constructor that initializes _weights at random
// between weight limits
SelfOrganizingMap::SelfOrganizingMap( const std::vector<int>& unitsArrayDimensionality, int inputDimensionality,
	       			      const std::array<double,2>& weightLimits )
{
	_updateStep = 0;
	_inputDimensionality = inputDimensionality;
	_unitsDimensionality = std::accumulate(unitsArrayDimensionality.begin(), unitsArrayDimensionality.end(), 1, std::multiplies<int>());
	_unitsArrayDimensionality.resize(unitsArrayDimensionality.size());
	for ( unsigned int dim = 0; dim < unitsArrayDimensionality.size(); dim++ )
		_unitsArrayDimensionality[dim] = unitsArrayDimensionality[dim];

	SelfOrganizingMap::validateObject();

	_weights.resize(_unitsDimensionality);
	for ( int row = 0; row < _unitsDimensionality; row++ )
		_weights[row].resize(inputDimensionality);

	for ( int row = 0; row < _unitsDimensionality; row++ )
		for ( int column = 0; column < inputDimensionality; column++ )
			_weights[row][column] = randomFromDoubleInterval(weightLimits[0],weightLimits[1]);
} // end SelfOrganizingMap default constructor


// constructor that initializes _weights with previous values from file
SelfOrganizingMap::SelfOrganizingMap( const std::string& fileName, const std::string& selfOrganizingMapIdentifier )
{
	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + fileName + ".mat", ios::in | std::ifstream::binary);

	SelfOrganizingMap::loadSelfOrganizingMapStatus(selfOrganizingMapIdentifier, infile);

	// close the opened file.
	infile.close();
} // end SelfOrganizingMap explicit constructor

// function to validate the object created of this class
void	SelfOrganizingMap::validateObject()
{
	if ( _inputDimensionality <= 0 || _unitsDimensionality <= 0 || _unitsArrayDimensionality.size() <= 0 )
	{
		cout << "SelfOrganizingMap object inconsistence: inputDimensionality = " << _inputDimensionality << "\n" << endl;
		cout << "SelfOrganizingMap object inconsistence: unitsArrayDimensionality.size() = " << _unitsArrayDimensionality.size() << "\n" << endl;
		cout << "SelfOrganizingMap object inconsistence: unitsDimensionality = " << _unitsDimensionality << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	for ( unsigned int dim = 0; dim < _unitsArrayDimensionality.size(); dim++ ) {
		if ( _unitsArrayDimensionality[dim] <= 0 ) {
			cout << "SelfOrganizingMap object inconsistence: unitsArrayDimensionality in its dim " << dim << "\n";
			cout << "has a wrong value: " << _unitsArrayDimensionality[dim] << "\n" << endl;
			exit( EXIT_FAILURE );
		}
	}

	if ( _inputDimensionality < (int)_unitsArrayDimensionality.size() )
	{
		cout << "SelfOrganizingMap object inconsistence: inputDimensionality = " << _inputDimensionality
		     << " must be greater than or -at least- equal to unitsArrayDimensionality.size() = "
		     << _unitsArrayDimensionality.size() << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	int product = std::accumulate(_unitsArrayDimensionality.begin(), _unitsArrayDimensionality.end(), 1, std::multiplies<int>());

	if ( _unitsDimensionality != product )
	{
		cout << "SelfOrganizingMap object inconsistence: unitsDimensionality = " << _unitsDimensionality
		     << " must be equal to the product of the elements in unitsArrayDimensionality which is = "
		     << product << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// function to modify the synapticWeights matrix throught the use of the Kohonen's learning rule
void	SelfOrganizingMap::learningRule( double learningRate, double neighborParameter, int unitsWinnerPosition,
					 const std::vector<double>& input )
{
	assert((int)input.size() == _inputDimensionality);
	assert(unitsWinnerPosition < _unitsDimensionality);
	double	neighborhoodValue;
	std::vector<std::vector<double>>	deltaWeights;

	deltaWeights.resize(_unitsDimensionality);

	std::string	gaussian = "gaussian";
	for ( int row = 0; row < _unitsDimensionality; row++ ) {							// the index row corresponds to the unit
		neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, unitsWinnerPosition, row, gaussian);
		// This is the Kohonen learning rule to update the synaptic weights
		deltaWeights[row] = input - _weights[row];
		std::transform(deltaWeights[row].begin(), deltaWeights[row].end(), deltaWeights[row].begin(),
		std::bind1st(std::multiplies<double>(),learningRate*neighborhoodValue));
		_weights[row]+=deltaWeights[row];
	}
} // end function learningRule


// function to compute the neighborhood value in the lateral interaction between units in the array for learning process
double	SelfOrganizingMap::learningNeighborhood( double widthParameter, int winnerPosition, int otherPosition, const std::string& str )
{
	assert(winnerPosition < _unitsDimensionality && otherPosition < _unitsDimensionality);
	std::vector<int>	winnerPositionArray, otherPositionArray, auxiliary;

	winnerPositionArray = unravelIndex(winnerPosition, _unitsArrayDimensionality);		// gets a vector with the array coordinates
       												// corresponding to the winnerPosition
	otherPositionArray = unravelIndex(otherPosition, _unitsArrayDimensionality);		// gets a vector with the array coordinates
       												// corresponding to the otherPosition
	auxiliary = winnerPositionArray - otherPositionArray;
	std::transform(auxiliary.begin(), auxiliary.end(), auxiliary.begin(),
		       static_cast<double(*)(double)>(&std::abs));
	int distance = std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0);

	std::string	STR;
	STR = "gaussian";
	if ( str.compare(STR) == 0 ) {
		return exp(-(pow((double)distance, (double)2)) / (2*widthParameter));		// returns the neighborhood value (gaussian)
	}
	STR = "mex";
	if ( str.compare(STR) == 0 ) {
		return 2/(sqrt(3*widthParameter)*pow(M_PI,(double)0.25)) *
		       (1 - (pow(distance,(double)2))/(pow(widthParameter,(double)2))) *
		       exp(-(pow(distance,(double)2))/(2*pow(widthParameter,(double)2)));	// returns the neighborhood value (mexican hat)
	}

	cout << "SelfOrganizingMap object inconsistence: bad function option: " << str << ".\n" << endl;
	exit( EXIT_FAILURE );
} // end function learningNeighborhoodFunction


// function to get the response information from the input
responseInfo	SelfOrganizingMap::getResponse( const std::vector<double>& input )
{
	assert((int)input.size() == _inputDimensionality);
	responseInfo response;

	response.distances.resize(_unitsDimensionality);
	for ( int row = 0; row < _unitsDimensionality; row++ )						// the index row corresponds to the units
		response.distances[row] = vectors_distance(_weights[row], input);

	response.ranking = sort_indexes(response.distances);

	return response;										// returns the units response
} // end function getResponse


// function to save the Self Organizing Map's status in a file
void	SelfOrganizingMap::saveSelfOrganizingMapStatus( const std::string& selfOrganizingMapIdentifier, ofstream& outfile )
{
	std::string	str = "SelfOrganizingMap_";
	std::string	STR;
	str += selfOrganizingMapIdentifier;
	str += "_";

        // saves _inputDimensionality
	save_as_scalar(str + "inputDimensionality", _inputDimensionality, outfile);

        // saves _unitsDimensionality
	save_as_scalar(str + "unitsDimensionality", _unitsDimensionality, outfile);

        // saves _unitsArrayDimensionality
	save_vector_as_matrix(str + "unitsArrayDimensionality", _unitsArrayDimensionality, outfile);

        // saves _weights
	save_vector_of_vectors_conditionally_as_sparse_matrix(str + "weights",_weights,SPARSITY_THRESHOLD,outfile);

        // saves _updateStep
	save_as_scalar(str + "updateStep", _updateStep, outfile);
} // end functiom saveSelfOrganizingMapStatus


// function to load the Self Organizing Map's status from a file
void	SelfOrganizingMap::loadSelfOrganizingMapStatus( const std::string& selfOrganizingMapIdentifier, ifstream& infile )
{
	std::string	str;
	std::string	STR = "SelfOrganizingMap_";
	STR += selfOrganizingMapIdentifier;
	STR += "_";

	while ( std::getline(infile, str) ) {

		auto	auxiliary = "# name: " + STR + "inputDimensionality";
		if ( str.compare(auxiliary) == 0 )
			load_scalar(_inputDimensionality, infile);

		auxiliary = "# name: " + STR + "unitsDimensionality";
		if ( str.compare(auxiliary) == 0 )
			load_scalar(_unitsDimensionality, infile);

		auxiliary = "# name: " + STR + "unitsArrayDimensionality";
		if ( str.compare(auxiliary) == 0 )
			load_matrix_to_vector(_unitsArrayDimensionality, infile);

		auxiliary = "# name: " + STR + "weights";
		if ( str.compare(auxiliary) == 0 )
			load_conditional_sparse_matrix_to_vector_of_vectors(_weights,infile);

		auxiliary = "# name: " + STR + "updateStep";
		if ( str.compare(auxiliary) == 0 )
			load_scalar(_updateStep, infile);
	}
} // end functiom loadSelfOrganizingMapStatus


