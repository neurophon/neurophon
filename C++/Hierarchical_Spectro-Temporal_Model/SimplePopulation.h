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

// File Name:		SimplePopulation.h
// File Description:	SimplePopulation class definition. This file presents SimplePopulation' public
//			interface without revealing the implementations of SimplePopulation' member functions,
//			which are defined in SimplePopulation.cpp.


// SimplePopulation class derived from class SelfOrganizingMap through inheritance.
// Class Description: This is a self organizing feature map kind class.
// It represents a bunch of neurons (units) arranged in an d-dimensional array.
// Its activaton function determines a unique active neuron per ejecution.

#ifndef SIMPLE_POPULATION_H
#define SIMPLE_POPULATION_H

#include "Structs.h"									// includes structs difinition
#include "SelfOrganizingMap.h"								// SelfOrganizingMap class declaration

class SimplePopulation : public SelfOrganizingMap
{
public:
				SimplePopulation( const std::vector<int>&, int );	// constructor that initializes SimplePopulation

	std::vector<double>	Activate( const responseInfo&, bool );			// returns the normalized coordinates of the best matching unit depending on response info.
	std::vector<double>	Activate( const std::vector<double>&, bool );		// returns the normalized coordinates of the best matching unit depending on input.
	std::vector<double>	Activate( const responseInfo&,				// decides which unit in the population to activate depending on response info.
					  double activationRadius, bool );		// If the best matching unit is inside activationRadius, the function returns
											// its normalized coordinates.
											// In the other case, coordinates are filled with "-1".
	std::vector<double>	Activate( const std::vector<double>&,			// decides which unit in the population to activate depending on input.
					  double activationRadius, bool );		// If the best matching unit is inside activationRadius, the function returns
											// its normalized coordinates.
											// In the other case, coordinates are filled with "-1".

private:
	std::vector<double>	Normalize( const std::vector<int>& );		// normalizes neuron coorsinates in the population
}; // end class KohonenFeatureMapAbs

#endif

