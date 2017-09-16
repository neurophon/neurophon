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

// File Name:		StaticSelfOrganizingMap.cpp
// File Description:	StaticSelfOrganizingMap member-function definitions. This file contains implementations of the
//			member functions prototyped in StaticSelfOrganizingMap.h.

#include <iostream>
#include <algorithm>
#include <omp.h>

#include "StaticSelfOrganizingMap.h"				// include definition of class StaticSelfOrganizingMap
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/OctaveInterface.h"
#include "../Libraries/Model/Templates.h"
#include "../Libraries/Model/DataTypes.h"
#include "../Libraries/Model/Random.h"

using namespace std;

// constructor that initializes _weights at random
StaticSelfOrganizingMap::StaticSelfOrganizingMap( const std::vector<std::size_t>& unitsArrayDimensionality,
						  const std::size_t inputDimensionality )
{
	_updateStep = 0;
	_inputDimensionality = inputDimensionality;
	_unitsDimensionality = std::accumulate(unitsArrayDimensionality.begin(), unitsArrayDimensionality.end(), 1, std::multiplies<std::size_t>());
	_unitsArrayDimensionality.resize(unitsArrayDimensionality.size());
	for ( std::size_t dim = 0; dim < unitsArrayDimensionality.size(); dim++ )
		_unitsArrayDimensionality[dim] = unitsArrayDimensionality[dim];

	StaticSelfOrganizingMap::validateObject();

	_weights.resize(_unitsDimensionality);
	for ( std::size_t row = 0; row < _unitsDimensionality; row++ )
		_weights[row].resize(_inputDimensionality);

	for ( std::size_t row = 0; row < _unitsDimensionality; row++ )
		for ( std::size_t column = 0; column < inputDimensionality; column++ )
			_weights[row][column] = ((double) rand() / (RAND_MAX));		// generate secret number between 0 and 1

	assert(is_rectangular(_weights));
} // end StaticSelfOrganizingMap constructor


// constructor that initializes _weights at random
// between weight limits
StaticSelfOrganizingMap::StaticSelfOrganizingMap( const std::vector<std::size_t>& unitsArrayDimensionality,
	       					  const std::size_t inputDimensionality,
	       			      		  const std::array<double,2>& weightLimits )
{
	_updateStep = 0;
	_inputDimensionality = inputDimensionality;
	_unitsDimensionality = std::accumulate(unitsArrayDimensionality.begin(), unitsArrayDimensionality.end(), 1, std::multiplies<std::size_t>());
	_unitsArrayDimensionality.resize(unitsArrayDimensionality.size());
	for ( std::size_t dim = 0; dim < unitsArrayDimensionality.size(); dim++ )
		_unitsArrayDimensionality[dim] = unitsArrayDimensionality[dim];

	StaticSelfOrganizingMap::validateObject();

	_weights.resize(_unitsDimensionality);
	for ( std::size_t row = 0; row < _unitsDimensionality; row++ )
		_weights[row].resize(inputDimensionality);

	for ( std::size_t row = 0; row < _unitsDimensionality; row++ )
		for ( std::size_t column = 0; column < inputDimensionality; column++ )
			_weights[row][column] = randomFromDoubleInterval(weightLimits[0],weightLimits[1]);

	assert(is_rectangular(_weights));
} // end StaticSelfOrganizingMap constructor


// constructor that initializes _weights with previous values from file
StaticSelfOrganizingMap::StaticSelfOrganizingMap( const std::string& fileName,
						  const std::string& selfOrganizingMapIdentifier )
{
	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + fileName + ".mat", ios::in | std::ifstream::binary);

	StaticSelfOrganizingMap::loadStaticSelfOrganizingMapStatus(selfOrganizingMapIdentifier, infile);

	// close the opened file.
	infile.close();
} // end StaticSelfOrganizingMap explicit constructor


// function to validate the object created of this class
void	StaticSelfOrganizingMap::validateObject()
{
	if ( _inputDimensionality == 0 || _unitsDimensionality == 0 || _unitsArrayDimensionality.size() == 0 )
	{
		cout << "StaticSelfOrganizingMap object inconsistence: inputDimensionality = " << _inputDimensionality << "\n" << endl;
		cout << "StaticSelfOrganizingMap object inconsistence: unitsArrayDimensionality.size() = " << _unitsArrayDimensionality.size() << "\n" << endl;
		cout << "StaticSelfOrganizingMap object inconsistence: unitsDimensionality = " << _unitsDimensionality << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	for ( std::size_t dim = 0; dim < _unitsArrayDimensionality.size(); dim++ ) {
		if ( _unitsArrayDimensionality[dim] == 0 ) {
			cout << "StaticSelfOrganizingMap object inconsistence: unitsArrayDimensionality in its dim " << dim << "\n";
			cout << "has a wrong value: " << _unitsArrayDimensionality[dim] << "\n" << endl;
			exit( EXIT_FAILURE );
		}
	}

	if ( _inputDimensionality < _unitsArrayDimensionality.size() )
	{
		cout << "StaticSelfOrganizingMap object inconsistence: inputDimensionality = " << _inputDimensionality
		     << " must be greater than or -at least- equal to unitsArrayDimensionality.size() = "
		     << _unitsArrayDimensionality.size() << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	std::size_t product = std::accumulate(_unitsArrayDimensionality.begin(), _unitsArrayDimensionality.end(), 1, std::multiplies<std::size_t>());

	if ( _unitsDimensionality != product )
	{
		cout << "StaticSelfOrganizingMap object inconsistence: unitsDimensionality = " << _unitsDimensionality
		     << " must be equal to the product of the elements in unitsArrayDimensionality which is = "
		     << product << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// function to modify the synapticWeights matrix throught the use of the Kohonen's learning rule
void	StaticSelfOrganizingMap::learningRule( const double learningRate,
	       				       const double neighborParameter,
					       const std::size_t unitsWinnerPosition,
					       const std::vector<double>& input )
{
	assert(input.size() == _inputDimensionality);
	assert(unitsWinnerPosition < _unitsDimensionality);
	twodvector<double>	deltaWeights;

	deltaWeights.resize(_unitsDimensionality);
	std::string	gaussian = "gaussian";
	#pragma omp parallel for default(none) shared(deltaWeights, input, gaussian)
	for ( std::size_t row = 0; row < _unitsDimensionality; row++ ) { // the index row corresponds to the unit
		auto	neighborhoodValue = StaticSelfOrganizingMap::learningNeighborhood(neighborParameter, unitsWinnerPosition, row, gaussian);
		// This is the Kohonen learning rule to update the synaptic weights
		deltaWeights[row] = input - _weights[row];
		std::transform(deltaWeights[row].begin(), deltaWeights[row].end(), deltaWeights[row].begin(),
		std::bind1st(std::multiplies<double>(),learningRate*neighborhoodValue));
		_weights[row]+=deltaWeights[row];
	}
} // end function learningRule


// function to compute the neighborhood value in the lateral interaction between units in the array for learning process
double	StaticSelfOrganizingMap::learningNeighborhood( const double widthParameter,
	       					       const std::size_t winnerPosition,
						       const std::size_t otherPosition,
						       const std::string& str )
{
	assert(winnerPosition < _unitsDimensionality && otherPosition < _unitsDimensionality);
	std::vector<std::size_t>	winnerPositionArray, otherPositionArray, auxiliary;

	winnerPositionArray = unravelIndex(winnerPosition, _unitsArrayDimensionality);		// gets a vector with the array coordinates
       												// corresponding to the winnerPosition
	otherPositionArray = unravelIndex(otherPosition, _unitsArrayDimensionality);		// gets a vector with the array coordinates
       												// corresponding to the otherPosition
	auxiliary.resize(winnerPositionArray.size());
	for ( std::size_t index = 0; index < winnerPositionArray.size(); index++ ) {
		if ( winnerPositionArray[index] > otherPositionArray[index] )
			auxiliary[index] = winnerPositionArray[index]-otherPositionArray[index];
		else
			auxiliary[index] = otherPositionArray[index]-winnerPositionArray[index];
	}
	
	std::size_t distance = std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0);

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

	cout << "StaticSelfOrganizingMap object inconsistence: bad function option: " << str << ".\n" << endl;
	exit( EXIT_FAILURE );
} // end function learningNeighborhoodFunction


// function to get the response information from the input
somResponseInfo	StaticSelfOrganizingMap::getResponse( const std::vector<double>& input )
{
	assert(input.size() == _inputDimensionality);
	somResponseInfo response;

	response.distances.resize(_unitsDimensionality);

	#pragma omp parallel for default(none) shared(response, input)
	for ( std::size_t row = 0; row < _unitsDimensionality; row++ )						// the index row corresponds to the units
		response.distances[row] = vectors_distance(_weights[row], input);

	response.ranking = sort_indexes(response.distances);

	return response;										// returns the units response
} // end function getResponse


// function to save the Self Organizing Map's status in a file
void	StaticSelfOrganizingMap::saveStaticSelfOrganizingMapStatus( const std::string& selfOrganizingMapIdentifier,
	       							    ofstream& outfile )
{
	std::string	str = "StaticSelfOrganizingMap_";
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
} // end functiom saveStaticSelfOrganizingMapStatus


// function to load the Self Organizing Map's status from a file
void	StaticSelfOrganizingMap::loadStaticSelfOrganizingMapStatus( const std::string& selfOrganizingMapIdentifier,
	       							    ifstream& infile )
{
	std::string	str;
	std::string	STR = "StaticSelfOrganizingMap_";
	STR += selfOrganizingMapIdentifier;
	STR += "_";

	bool	check_inputDimensionality = false;
	bool	check_unitsDimensionality = false;
	bool	check_unitsArrayDimensionality = false;
	bool	check_weights = false;
	bool	check_updateStep = false;

	while ( std::getline(infile, str) ) {

		auto	auxiliary = "# name: " + STR + "inputDimensionality";
		if ( str.compare(auxiliary) == 0 ) {
			load_scalar(_inputDimensionality, infile);
			check_inputDimensionality = true;
		}

		auxiliary = "# name: " + STR + "unitsDimensionality";
		if ( str.compare(auxiliary) == 0 ) {
			load_scalar(_unitsDimensionality, infile);
			check_unitsDimensionality = true;
		}

		auxiliary = "# name: " + STR + "unitsArrayDimensionality";
		if ( str.compare(auxiliary) == 0 ) {
			load_matrix_to_vector(_unitsArrayDimensionality, infile);
			check_unitsArrayDimensionality = true;
		}

		auxiliary = "# name: " + STR + "weights";
		if ( str.compare(auxiliary) == 0 ) {
			load_conditional_sparse_matrix_to_vector_of_vectors(_weights,infile);
			check_weights = true;
		}

		auxiliary = "# name: " + STR + "updateStep";
		if ( str.compare(auxiliary) == 0 ) {
			load_scalar(_updateStep, infile);
			check_updateStep = true;
		}
	}

	assert(check_inputDimensionality == true);
	assert(check_unitsDimensionality == true);
	assert(check_unitsArrayDimensionality == true);
	assert(check_weights == true);
	assert(check_updateStep == true);

} // end functiom loadStaticSelfOrganizingMapStatus


