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

// File Name:		StaticProcessor.h
// File Description:	StaticProcessor class definition. This file presents StaticProcessor's public
//			interface without revealing the implementations of StaticProcessor's member functions,
//			which are defined in StaticProcessor.cpp.


// Class Description: This is a Self Organizing Map class. It is a bunch of units arranged in an d-dimensional array.
// All these units have a learning capacity by means of the modifications of the synaptic weights.
#ifndef STATIC_PROCESSOR_H
#define STATIC_PROCESSOR_H

#include <fstream>
#include <array>

#include "Structs.h"

class StaticProcessor
{
public:
		StaticProcessor( const std::vector<std::size_t>&,
				 const std::size_t,
				 const double,
				 const double,
				 const std::array<double,2>& weightLimits = {0.0,1.0} );// constructor that initializes _weights with
											// random numbers between weight limits
											// this function guaranties certain level of sparsity
											// in the weights matrix

		StaticProcessor( const std::string&,
				 const std::string& );					// constructor that initializes _weights with previous
											// from file
	void	validateObject();							// function that validates the object
	
	responseInfo	learningRule( const double,
				      const double,
				      const double,
				      const std::vector<std::size_t>&,
				      const double,
		       		      const bool activationHomeostasis = false,
				      const bool randomness = false );			// function that modifies _weights members through learning rule
	
	double	learningNeighborhood( const double,
				      const std::size_t,
				      const std::size_t,
				      const std::string& );				// function that computes the neighborhood lateral influence
											// for the learning process.
											// Specific neighborhood functions can be chosen by means of
											// a string parameter. i.e. "gaussian" for a Gaussian bell,
											// "mex" for a Mexican hat function, etc

	responseInfo	getResponse( const std::vector<std::size_t>& );			// function that gets the response information from the input.
											// the response is compose by a vector with the inverse of the inner product
											// between the input and every unit in the class and a
											// vector with the indexes of such units in ascending value order
	void	homeostasis( const bool,
			     const bool,
			     const bool,
		       	     const double );						// applies homeostasis to the object
	void	synapticHomeostasis( const double );					// computes synaptic homeostasis over the weights
	
	void	synapticGrowthLimitation( const double );				// computes synaptic growth limitation over the weights

	void	activationHomeostasis( const double );					// computes activation homeostasis over the units

	void	saveStaticProcessorStatus( const std::string&,
					   std::ofstream& );				// function that saves the Self Organizing Map's status in a file

	void	loadStaticProcessorStatus( const std::string&,
					   std::ifstream& );				// function to load the Self Organizing Map's status from a file

protected:
        std::vector<std::size_t>		_unitsArrayDimensionality;		// vector with the dimensions of the array of units
	std::size_t				_unitsDimensionality;			// flat dimensionality of the units vector

private:
	std::size_t				_updateStep;				// accumulates the steps elapsed until now in order to check UPDATE_PERIOD

	std::size_t				_inputDimensionality;			// flat dimensionality of the input vector
	double					_potentialPercentage;			// this is the percentage of potential connections to the inputs
	std::size_t				_potentialDimensionality;		// dimensionality of the potential connections
	twodvector<std::size_t>			_potentialConnections;			// matrix with the potential connections to the inputs

	std::vector<bool>			_weightsSparsity;			// indicates if the weights array for each unit is sparse
	twodvector<double>			_weights;				// matrix of the weights produced by the algorithm

	std::vector<std::size_t>		_unitsActivity;				// registers the count of the unit's activity
	std::vector<double>			_activationBoosting;			// units activation busting
}; // end class SelfOrganizingMap

#endif











