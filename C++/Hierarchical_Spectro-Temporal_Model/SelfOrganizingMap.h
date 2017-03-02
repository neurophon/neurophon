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

#include "Structs.h"

class SelfOrganizingMap
{
public:
	SelfOrganizingMap( const std::vector<int>&, int );				// default constructor that initializes arrays to zero on SelfOrganizingMap
	SelfOrganizingMap( const std::vector<int>&, int,
			   const std::vector<std::vector<double>>& );			// explicit constructor that initializes weight array from file and other arrays to zero
	void	validateObject();							// function that validates the object

	void	learningRule( double, double, int,
			      const std::vector<double>& );				// function that modifies the synapticWeights matrix through the Kohonen's learning rule
	double	learningNeighborhood( double, int, int );				// function that computes the neighborhood lateral influence parameter for learning process
	responseInfo	getResponse( const std::vector<double>& );			// function that gets the response information from the input

	void	saveStatus();								// function that saves the Self Organizing Map's status in a file

protected:
        std::vector<int>			_unitsArrayDimensionality;		// vector with the dimensions of the array of units

private:
        int					_inputDimensionality;			// flat dimensionality of the input vector
        int					_unitsDimensionality;			// flat dimensionality of the units vector

	std::vector<std::vector<double>>	_weights;				// matrix of the weights
}; // end class KohonenFeatureMap

#endif











