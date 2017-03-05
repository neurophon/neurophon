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

// File Name:		StrongPopulation.h
// File Description:	StrongPopulation class definition. This file presents StrongPopulation' public
//			interface without revealing the implementations of StrongPopulation' member functions,
//			which are defined in StrongPopulation.cpp.


// StrongPopulation class derived from class StrongUnits through inheritance.
// Class Description: This is a self organizing feature map kind class.
// It represents a bunch of neurons (units) arranged in an d-dimensional array.
// Its activaton function determines a unique active neuron per ejecution.

#ifndef STRONG_POPULATION_H
#define STRONG_POPULATION_H

#include "Structs.h"									// includes structs difinition
#include "StrongUnits.h"								// StrongUnits class declaration

class StrongPopulation : public StrongUnits
{
public:
				StrongPopulation( const std::vector<int>&, int );	// constructor that initializes StrongPopulation

	std::vector<double>	Activate( const responseInfo&, bool );			// returns the normalized coordinates of the best active unit
											// depending on response info and on _strongUnits.
	std::vector<double>	Activate( const std::vector<double>&, bool );		// returns the normalized coordinates of the best active unit
											// depending on input and on _strongUnits.





	std::vector<double>	Activate( const responseInfo&,				// decides which unit in the population to activate depending on response info.
					  double activationRadius, bool );		// If a set of best matching units is inside activationRadius, the function chose
											// the most active of them based on distances and on _strongUnits.
											// Then, it returns the corresponding unit's normalized coordinates.
											// In the other case, (if no unit is inside activationRadius)
											// coordinates are filled with "-1".

	std::vector<double>	Activate( const std::vector<double>&,			// decides which unit in the population to activate depending on input.
					  double activationRadius, bool );		// If a set of best matching units is inside activationRadius, the function chose
											// the most active of them based on distances and on _strongUnits.
											// Then, it returns the corresponding unit's normalized coordinates.
											// In the other case, (if no unit is inside activationRadius)
											// coordinates are filled with "-1"
	responseInfo	getStrongResponse( const responseInfo& );			// modifies response ussing _strongUnits

private:
	std::vector<double>	Normalize( const std::vector<int>& );			// normalizes neuron coorsinates in the population

}; // end class StrongPopulation

#endif

