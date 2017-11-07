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

// File Name:		StaticSelfOrganizingMap.h
// File Description:	StaticSelfOrganizingMap class definition. This file presents StaticSelfOrganizingMap's public
//			interface without revealing the implementations of StaticSelfOrganizingMap's member functions,
//			which are defined in StaticSelfOrganizingMap.cpp.


// Class Description: This is a Self Organizing Map class. It is a bunch of units arranged in an d-dimensional array.
// All these units have a learning capacity by means of the modifications of the synaptic weights.
#ifndef STATIC_SOM_H
#define STATIC_SOM_H

#include <fstream>
#include <array>

#include "Structs.h"

class StaticSelfOrganizingMap
{
public:
		StaticSelfOrganizingMap( const std::vector<std::size_t>&,
			       		 const std::size_t );				// constructor that initializes object of class StaticSelfOrganizingMap
											// _weights is initialized with
											// random numbers between 0.0 and 1.0
		StaticSelfOrganizingMap( const std::vector<std::size_t>&,
			       		 const std::size_t,
				   	 const std::array<double,2>& );			// constructor that initializes object of class StaticSelfOrganizingMap
											// _weights is initialized with random numbers between limits
											// passed to the function as arguments 
		StaticSelfOrganizingMap( std::stringstream&,
			       		 const std::string& );				// constructor that initializes object of class StaticSelfOrganizingMap
											// from file
	void	validateObject();							// function that validates the object

	void	learningRule( const double, const double,
		       	      const std::size_t,
			      const std::vector<double>& );				// function that modifies _weights members
       											// through the Kohonen's learning rule

	double	learningNeighborhood( const double,
		       		      const std::size_t,
				      const std::size_t,
				      const std::string& );				// function that computes the neighborhood lateral influence
											// for the learning process.
											// Specific neighborhood functions can be chosen by means of
											// a string parameter. i.e. "gaussian" for a Gaussian bell,
											// "mex" for a Mexican hat function, etc
	somResponseInfo	getResponse( const std::vector<double>& );			// function that gets the response information from the input.
											// the response is compose by a vector with the euclidean distances
											// between the input and every unit in the class and a
											// vector with the indexes of such units in ascending distance order
	void	saveStaticSelfOrganizingMapStatus( const std::string&,
		       				   std::stringstream& );			// function that saves the Self Organizing Map's status in a file

	void	loadStaticSelfOrganizingMapStatus( const std::string&,
		       				   std::stringstream& );			// function to load the Self Organizing Map's status from a file

protected:
        std::vector<std::size_t>		_unitsArrayDimensionality;		// vector with the dimensions of the array of units
	std::size_t				_unitsDimensionality;			// flat dimensionality of the units vector

private:
	std::size_t				_inputDimensionality;			// flat dimensionality of the input vector
	std::size_t				_updateStep;				// accumulates the steps elapsed until now in order to check UPDATE_PERIOD
	twodvector<double>			_weights;				// matrix of the weights produced by the Kohonen's algorithm
}; // end class StaticSelfOrganizingMap

#endif



