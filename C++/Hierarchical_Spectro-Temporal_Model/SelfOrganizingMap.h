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

// File Name:		SelfOrganizingMap.h
// File Description:	SelfOrganizingMap class definition. This file presents SelfOrganizingMap's public
//			interface without revealing the implementations of SelfOrganizingMap's member functions,
//			which are defined in SelfOrganizingMap.cpp.


// Class Description: This is a Self Organizing Map class. It is a bunch of units arranged in an d-dimensional array.
// All these units have a learning capacity by means of the modifications of the synaptic weights.
#ifndef SOM_H
#define SOM_H

#include <fstream>
#include <array>

#include "Structs.h"

class SelfOrganizingMap
{
public:
		SelfOrganizingMap( const std::vector<int>&, int );			// constructor that initializes _weights with
		SelfOrganizingMap( const std::vector<int>&, int,
				   const std::array<double,2>& );			// constructor that initializes _weights between weight limits
											// random numbers x, where 0 <= x < 1, on SelfOrganizingMap
		SelfOrganizingMap( const std::string&, const std::string& );		// constructor that initializes _weights with previous
											// from file
	void	validateObject();							// function that validates the object

	void	learningRule( double, double, int,
			      const std::vector<double>& );				// function that modifies _weights members through the Kohonen's learning rule

	double	learningNeighborhood( double, int, int, const std::string& );		// function that computes the neighborhood lateral influence
											// for the learning process.
											// Specific neighborhood functions can be chosen by means of
											// a string parameter. i.e. "gaussian" for a Gaussian bell,
											// "mex" for a Mexican hat function, etc
	responseInfo	getResponse( const std::vector<double>& );			// function that gets the response information from the input.
											// the response is compose by a vector with the euclidean distances
											// between the input and every unit in the class and a
											// vector with the indexes of such units in ascending distance order
	void	saveSelfOrganizingMapStatus( const std::string&, std::ofstream& );	// function that saves the Self Organizing Map's status in a file

	void	loadSelfOrganizingMapStatus( const std::string&, std::ifstream& );	// function to load the Self Organizing Map's status from a file

protected:
        std::vector<int>			_unitsArrayDimensionality;		// vector with the dimensions of the array of units
        int					_unitsDimensionality;			// flat dimensionality of the units vector

private:
        int					_inputDimensionality;			// flat dimensionality of the input vector
	int					_updateStep;				// accumulates the steps elapsed until now in order to check UPDATE_PERIOD
	std::vector<std::vector<double>>	_weights;				// matrix of the weights produced by the Kohonen's algorithm
}; // end class SelfOrganizingMap

#endif











