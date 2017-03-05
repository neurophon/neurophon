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

// File Name:		StrongUnits.cpp
// File Description:	StrongUnits member-function definitions. This file contains implementations of the
//			member functions prototyped in StrongUnits.h.

#include <fstream>
#include <algorithm>

#include "StrongUnits.h"				// includes definition of class StrongUnits
#include "../Libraries/Utilities.h"
#include "../Libraries/Constants.h"
#include "../Libraries/OctaveInterface.h"

using namespace std;

// constructor initializes populationDimensions and numberOfInputs with variables supplied as arguments
StrongUnits::StrongUnits( const std::vector<int>& populationDimensions, int numberOfInputs )
	// explicitly call base-class constructor
	: SelfOrganizingMap(populationDimensions, numberOfInputs)
{
	int	unitsDimensionality = std::accumulate(populationDimensions.begin(), populationDimensions.end(), 1, std::multiplies<int>());
	_strongUnits.resize(unitsDimensionality);
	_updateStep = 0;
} // end StrongUnits constructor


// constructor initializes populationDimensions and numberOfInputs with variables supplied as arguments.
// This loads _strongUnits with previous vector supplied as argument too
StrongUnits::StrongUnits( const std::vector<int>& populationDimensions, int numberOfInputs,
			  const std::vector<double> strongUnits )
	// explicitly call base-class constructor
	: SelfOrganizingMap(populationDimensions, numberOfInputs)
{
	int	unitsDimensionality = std::accumulate(populationDimensions.begin(), populationDimensions.end(), 1, std::multiplies<int>());
	_strongUnits.resize(unitsDimensionality);
	_updateStep = 0;

	for ( int i = 0; i < unitsDimensionality; i++ )
		_strongUnits[i] = strongUnits[i];
} // end StrongUnits constructor


// updates _strongUnits depending on response info.
void	StrongUnits::Update( const responseInfo& response )
{
	double	sum;
	_strongUnits[response.ranking[0]] = _strongUnits[response.ranking[0]] + SYNAPTIC_INCREMENT;

	if ( _updateStep > UPDATE_PERIOD ) {
		sum = std::accumulate(_strongUnits.begin(), _strongUnits.end(), 0);
		_updateStep = 0;
	}
	_updateStep++;

	if ( sum > 1 )
		std::transform(_strongUnits.begin(), _strongUnits.end(), _strongUnits.begin(),
		std::bind2nd(std::divides<double>(),sum));
} // end function Update


// updates _strongUnits depending on input.
void	StrongUnits::Update( const std::vector<double>& input )
{
	double	sum;
	responseInfo	response;

	response = SelfOrganizingMap::getResponse(input);

	_strongUnits[response.ranking[0]] = _strongUnits[response.ranking[0]] + SYNAPTIC_INCREMENT;

	if ( _updateStep > UPDATE_PERIOD ) {
		sum = std::accumulate(_strongUnits.begin(), _strongUnits.end(), 0);
		_updateStep = 0;
	}
	_updateStep++;

	if ( sum > 1 )
		std::transform(_strongUnits.begin(), _strongUnits.end(), _strongUnits.begin(),
		std::bind2nd(std::divides<double>(),sum));
} // end function Update


// updates _strongUnits depending on unit index.
void	StrongUnits::Update( int index )
{
	double	sum;

	if ( index > _unitsDimensionality || index < 0 ) {
		cout << "StrongUnits bad method parameter:" << endl;
		cout << "Wrong index (" << index << ") in Update function." << endl;
		exit( EXIT_FAILURE );
	}

	_strongUnits[index] = _strongUnits[index] + SYNAPTIC_INCREMENT;

	if ( _updateStep > UPDATE_PERIOD ) {
		sum = std::accumulate(_strongUnits.begin(), _strongUnits.end(), 0);
		_updateStep = 0;
	}
	_updateStep++;

	if ( sum > 1 )
		std::transform(_strongUnits.begin(), _strongUnits.end(), _strongUnits.begin(),
		std::bind2nd(std::divides<double>(),sum));
} // end function Update


// updates _strongUnits neighborhood depending on response info.
void	StrongUnits::Update( const responseInfo& response, double neighborParameter )
{
	double	sum, neighborhoodValue;

	for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
		neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, response.ranking[0], unit);
		_strongUnits[unit] = _strongUnits[unit] + neighborhoodValue*SYNAPTIC_INCREMENT;
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		sum = std::accumulate(_strongUnits.begin(), _strongUnits.end(), 0);
		_updateStep = 0;
	}
	_updateStep++;

	if ( sum > 1 )
		std::transform(_strongUnits.begin(), _strongUnits.end(), _strongUnits.begin(),
		std::bind2nd(std::divides<double>(),sum));
} // end function Update


// updates _strongUnits neighborhood depending on input.
void	StrongUnits::Update( const std::vector<double>& input, double neighborParameter )
{
	double	sum, neighborhoodValue;
	responseInfo	response;

	response = SelfOrganizingMap::getResponse(input);

	for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
		neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, response.ranking[0], unit);
		_strongUnits[unit] = _strongUnits[unit] + neighborhoodValue*SYNAPTIC_INCREMENT;
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		sum = std::accumulate(_strongUnits.begin(), _strongUnits.end(), 0);
		_updateStep = 0;
	}
	_updateStep++;

	if ( sum > 1 )
		std::transform(_strongUnits.begin(), _strongUnits.end(), _strongUnits.begin(),
		std::bind2nd(std::divides<double>(),sum));
} // end function Update


// updates _strongUnits neighborhood depending on unit index.
void	StrongUnits::Update( int index, double neighborParameter )
{
	double	sum, neighborhoodValue;

	if ( index > _unitsDimensionality || index < 0 ) {
		cout << "StrongUnits bad method parameter:" << endl;
		cout << "Wrong index (" << index << ") in Update function." << endl;
		exit( EXIT_FAILURE );
	}

	for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
		neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, index, unit);
		_strongUnits[unit] = _strongUnits[unit] + neighborhoodValue*SYNAPTIC_INCREMENT;
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		sum = std::accumulate(_strongUnits.begin(), _strongUnits.end(), 0);
		_updateStep = 0;
	}
	_updateStep++;

	if ( sum > 1 )
		std::transform(_strongUnits.begin(), _strongUnits.end(), _strongUnits.begin(),
		std::bind2nd(std::divides<double>(),sum));
} // end function Update


// function to save the Self Organizing Map's status in a file
void	StrongUnits::saveStrongUnitsStatus()
{
        // open a file in write mode.
        ofstream outfile;
        outfile.open("../../Octave/SU_Status.mat", ios::out | ios::trunc);

        // file preamble.
        outfile << "# This is a file created by saveStatus member function in StrongUnits class." << endl;
        outfile << "# Author: Dematties Dario Jesus." << endl;

        outfile << "\n\n" << endl;

        // saves _strongUnits
	save_vector_as_matrix("strongUnits", _strongUnits, outfile);

	// close the opened file.
	outfile.close();
} // end functiom saveWeights












