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
#include "../Libraries/DataTypes.h"
#include "StrongUnits.h"								// StrongUnits class declaration

class StrongPopulation : public StrongUnits
{
public:
				StrongPopulation( const std::vector<int>&,
						  const int );	// constructor that initializes StrongPopulation
											// with _strongUnits initialized to zero by default

				StrongPopulation( const std::vector<int>&, const int,
						  const std::vector<double>& );		// constructor that initializes StrongPopulation
											// with _strongUnits initialized with a previous vector

	std::vector<double>	Activate( const responseInfo&, const bool );		// returns the normalized coordinates of the best active unit
											// depending on response info and on _strongUnits.
	std::vector<double>	Activate( const std::vector<double>&, const bool );	// returns the normalized coordinates of the best active unit
											// depending on input and on _strongUnits.
	twodvector<double>	Activate( const responseInfo&, const bool,
					  int );					// returns the normalized coordinates of the "n" best active units
											// depending on response info and on _strongUnits,
											// where "n" is a function parameter.
											// The coordinates are sorted in a ranking of decreasing
											// activation priority.
											// In case of parity between the units, their position is
											// chosen randomly.
											// In case of parity between the last units in the ranking,
											// the classified units are chosen at random in order to
											// enter in the ranking of units activated.
	twodvector<double>	Activate( const std::vector<double>&, const bool,
					  int );					// returns the normalized coordinates of the "n" best active units
											// depending on input and on _strongUnits,
											// where "n" is a function parameter.
											// The coordinates are sorted in a ranking of decreasing
											// activation priority.
											// In case of parity between the units, their position is
											// chosen randomly.
											// In case of parity between the last units in the ranking,
											// the classified units are chosen at random in order to
											// enter in the ranking of units activated.
	twodvector<double>	Activate( const responseInfo&,
					  const double, const bool );			// decides which unit in the population to activate depending on response info.
											// If a set of best matching units is inside activationRadius, the function chose
											// the more active of them based on distances and on _strongUnits.
											// Then, it returns the corresponding unit's normalized coordinates.
											// In case of parity, all the units become active.
											// In the other case, (if no unit is inside activationRadius)
											// coordinates are filled with "-1".
	twodvector<double>	Activate( const std::vector<double>&,
					  const double, const bool );			// decides which unit in the population to activate depending on input.
											// If a set of best matching units is inside activationRadius, the function chose
											// the more active of them based on distances and on _strongUnits.
											// Then, it returns the corresponding unit's normalized coordinates.
											// In case of parity, all the units become active.
											// In the other case, (if no unit is inside activationRadius)
											// coordinates are filled with "-1"
	responseInfo	getStrongResponse( const responseInfo& );			// modifies response ussing _strongUnits

private:
	std::vector<double>	Normalize( const std::vector<int>& );			// normalizes neuron coorsinates in the population

}; // end class StrongPopulation

#endif

