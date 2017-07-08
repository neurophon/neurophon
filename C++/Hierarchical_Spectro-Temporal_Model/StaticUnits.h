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

// File Name:		StaticUnits.h
// File Description:	StaticUnits class definition. This file presents StaticUnits's public
//			interface without revealing the implementations of StaticUnits's member functions,
//			which are defined in StaticUnits.cpp.


// Class Description: This is a Self Organizing Map class. It is a bunch of units arranged in an d-dimensional array.
// All these units have a learning capacity by means of the modifications of the synaptic weights.
#ifndef SOM_H
#define SOM_H

#include <fstream>
#include <array>

#include "Structs.h"

class StaticUnits
{
public:
		StaticUnits( const std::vector<int>&, const int,
	       		     const std::array<double,2>& weightLimits = {0.0,1.0}, const bool binaryProcessing = false );
											// constructor that initializes _weights with
											// random numbers between weight limits
		StaticUnits( const std::vector<int>&, const int, const double,
	       		     const std::array<double,2>& weightLimits = {0.0,1.0}, const bool binaryProcessing = false );
											// constructor that initializes _weights with
											// random numbers between weight limits
											// this function guaranties certain level of sparsity
											// in the weights matrix

		StaticUnits( const std::string&, const std::string& );			// constructor that initializes _weights with previous
											// from file
	void	validateObject();							// function that validates the object
	
	void	learningRule( const double, const double, const int,
			      const std::vector<double>& );				// function to modify the synapticWeights matrix throught
	       										// the use of the Kohonen's learning rule
	responseInfo	learningRule( const double, const double, const double,
				      const std::vector<std::size_t>&, const double,
		       		      const bool activationHomeostasis = false,
				      const bool randomness = false );			// function that modifies _weights members through learning rule
	
	double	learningNeighborhood( const double, const int, const int,
				      const std::string& );				// function that computes the neighborhood lateral influence
											// for the learning process.
											// Specific neighborhood functions can be chosen by means of
											// a string parameter. i.e. "gaussian" for a Gaussian bell,
											// "mex" for a Mexican hat function, etc
	responseInfo	getResponse( const std::vector<double>& );			// function to get the response information from the input

	responseInfo	getResponse( const std::vector<std::size_t>& );			// function that gets the response information from the input.
											// the response is compose by a vector with the inverse of the inner product
											// between the input and every unit in the class and a
											// vector with the indexes of such units in ascending value order
	void	homeostasis( const bool, const bool, const bool,
		       	     const double );						// applies homeostasis to the object
	void	synapticHomeostasis( const double );					// computes synaptic homeostasis over the weights
	
	void	synapticGrowthLimitation( const double );				// computes synaptic growth limitation over the weights

	void	activationHomeostasis( const double );					// computes activation homeostasis over the units

	void	saveStaticUnitsStatus( const std::string&, std::ofstream& );		// function that saves the Self Organizing Map's status in a file

	void	loadStaticUnitsStatus( const std::string&, std::ifstream& );		// function to load the Self Organizing Map's status from a file

protected:
        std::vector<int>			_unitsArrayDimensionality;		// vector with the dimensions of the array of units
        int					_unitsDimensionality;			// flat dimensionality of the units vector

private:
        int					_inputDimensionality;			// flat dimensionality of the input vector
	int					_updateStep;				// accumulates the steps elapsed until now in order to check UPDATE_PERIOD
	bool					_binaryProcessing;			// determines if the object is going to process binary information
	std::vector<bool>			_weightsSparsity;			// indicates if the weights array for each unit is sparse
	std::vector<std::vector<double>>	_weights;				// matrix of the weights produced by the Kohonen's algorithm
	std::vector<std::size_t>		_unitsActivity;				// registers the count of the unit's activity
	std::vector<double>			_activationBoosting;			// units activation busting
}; // end class SelfOrganizingMap

#endif











