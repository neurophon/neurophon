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
#include <fstream>
#include <cassert>
#include <algorithm>
#include <time.h>

#include "SelfOrganizingMap.h"				// include definition of class SelfOrganizingMap
#include "../Libraries/Utilities.h"
#include "../Libraries/OctaveInterface.h"
#include "../Libraries/Templates.h"
#include "../Libraries/DataTypes.h"

using namespace std;

// constructor that initializes vectors to zero
SelfOrganizingMap::SelfOrganizingMap( const std::vector<int>& unitsArrayDimensionality, int inputDimensionality )
{
	_inputDimensionality = inputDimensionality;
	_unitsDimensionality = std::accumulate(unitsArrayDimensionality.begin(), unitsArrayDimensionality.end(), 1, std::multiplies<int>());
	_unitsArrayDimensionality.resize(unitsArrayDimensionality.size());
	for ( unsigned int dim = 0; dim < unitsArrayDimensionality.size(); dim++ )
		_unitsArrayDimensionality[dim] = unitsArrayDimensionality[dim];

	SelfOrganizingMap::validateObject();

	_weights.resize(_unitsDimensionality);
	for ( int row = 0; row < _unitsDimensionality; row++ )
		_weights[row].resize(inputDimensionality);

	/* initialize random seed: */
	srand (time(NULL));

	for ( int row = 0; row < _unitsDimensionality; row++ )
		for ( int column = 0; column < inputDimensionality; column++ )
			_weights[row][column] = ((double) rand() / (RAND_MAX)) + 1;	// generate secret number between 0 and 1
} // end SelfOrganizingMap default constructor

// constructor that initializes vectors to zero except _weights which is initialized from a file
SelfOrganizingMap::SelfOrganizingMap( const std::vector<int>& unitsArrayDimensionality, int inputDimensionality,
				      const std::vector<std::vector<double>>& weights )
{
	_inputDimensionality = inputDimensionality;
	_unitsDimensionality = std::accumulate(unitsArrayDimensionality.begin(), unitsArrayDimensionality.end(), 1, std::multiplies<int>());
	_unitsArrayDimensionality.resize(unitsArrayDimensionality.size());
	for ( unsigned int dim = 0; dim < unitsArrayDimensionality.size(); dim++ )
		_unitsArrayDimensionality[dim] = unitsArrayDimensionality[dim];

	SelfOrganizingMap::validateObject();

	_weights.resize(_unitsDimensionality);
	for ( int row = 0; row < _unitsDimensionality; row++ )
		_weights[row].resize(inputDimensionality);

	if ( (int)weights.size() != _unitsDimensionality ) {
		cout << "SelfOrganizingMap object inconsistence:" << endl;
		cout << "Wrong number of rows in parameter weigts: " << weights.size() << endl;
		cout << "The expected number of rows is: " << _weights.size() << endl;
		exit( EXIT_FAILURE );
	}
	else {
		for ( int row = 0; row < _unitsDimensionality; row++ ) {
			if ( (int)weights[row].size() != inputDimensionality ) {
				cout << "SelfOrganizingMap object inconsistence:" << endl;
				cout << "Wrong number of columns (" << weights[row].size() << ") in parameter weigts row: " << row << endl;
				cout << "The expected number of columns for every row is: " << inputDimensionality << endl;
				exit( EXIT_FAILURE );
			}
		}
	}

	for ( int row = 0; row < _unitsDimensionality; row++ )
		for ( int column = 0; column < inputDimensionality; column++ )
			_weights[row][column] = weights[row][column];
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
		cout << "SelfOrganizingMap object inconsistence: inputDimensionality = " << _inputDimensionality << " must be greater than or -at least- equal to unitsArrayDimensionality.size() = " << _unitsArrayDimensionality.size() << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	int product = std::accumulate(_unitsArrayDimensionality.begin(), _unitsArrayDimensionality.end(), 1, std::multiplies<int>());

	if ( _unitsDimensionality != product )
	{
		cout << "SelfOrganizingMap object inconsistence: unitsDimensionality = " << _unitsDimensionality << " must be equal to the product of the elements in unitsArrayDimensionality which is = " << product << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// function to modify the synapticWeights matrix throught the use of the Kohonen's learning rule
void	SelfOrganizingMap::learningRule( double learningRate, double neighborParameter, int unitsWinnerPosition,
					 const std::vector<double>& input )
{
	double	neighborhoodValue;
	std::vector<std::vector<double>>	deltaWeights;

	deltaWeights.resize(_unitsDimensionality);

	for ( int row = 0; row < _unitsDimensionality; row++ ) {							// the index row corresponds to the unit
		neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, unitsWinnerPosition, row);
		// This is the Kohonen learning rule to update the synaptic weights
		deltaWeights[row] = input - _weights[row];
		std::transform(deltaWeights[row].begin(), deltaWeights[row].end(), deltaWeights[row].begin(),
		std::bind1st(std::multiplies<double>(),learningRate*neighborhoodValue));
		_weights[row]+=deltaWeights[row];
	}
} // end function learningRuleFunction


// function to compute the neighborhood value in the lateral interaction between units in the array for learning process
double	SelfOrganizingMap::learningNeighborhood( double widthParameter, int winnerPosition, int otherPosition )
{
	std::vector<int>	winnerPositionArray, otherPositionArray, auxiliary;

	winnerPositionArray = unravelIndex(winnerPosition, _unitsArrayDimensionality);		// gets a vector with the array coordinates corresponding to the winnerPosition
	otherPositionArray = unravelIndex(otherPosition, _unitsArrayDimensionality);		// gets a vector with the array coordinates corresponding to the otherPosition

	auxiliary = winnerPositionArray - otherPositionArray;
	std::transform(auxiliary.begin(), auxiliary.end(), auxiliary.begin(),
		       static_cast<double(*)(double)>(&std::abs));
	int distance = std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0);

	return exp(-(pow((double)distance, (double)2)) / (2*widthParameter));		// returns the neighborhood value
} // end function learningNeighborhoodFunction


// function to get the response information from the input
responseInfo	SelfOrganizingMap::getResponse( const std::vector<double>& input )
{
	responseInfo response;

	response.distances.resize(_unitsDimensionality);
	for ( int row = 0; row < _unitsDimensionality; row++ )						// the index row corresponds to the units
		response.distances[row] = vectors_distance(_weights[row], input);

	response.ranking = sort_indexes(response.distances);

	return response;										// returns the units response
} // end function findWinnerUnit


// function to save the Self Organizing Map's status in a file
void	SelfOrganizingMap::saveStatus()
{
        // open a file in write mode.
        ofstream outfile;
        outfile.open("../../Octave/SOM_Status.mat", ios::out | ios::trunc);

        // file preamble.
        outfile << "# This is a file created by saveWeights member function in SelfOrganizingMap class." << endl;
        outfile << "# Author: Dematties Dario Jesus." << endl;

        outfile << "\n\n" << endl;

        // saves _inputDimensionality
	save_as_scalar("inputDimensionality", _inputDimensionality, outfile);

        // saves _unitsDimensionality
	save_as_scalar("unitsDimensionality", _unitsDimensionality, outfile);

        // saves _unitsArrayDimensionality
	save_vector_as_matrix("unitsArrayDimensionality", _unitsArrayDimensionality, outfile);

        // saves _weights
	save_vector_of_vectors_as_matrix("weights", _weights, outfile);

	// close the opened file.
	outfile.close();
} // end functiom saveWeights


