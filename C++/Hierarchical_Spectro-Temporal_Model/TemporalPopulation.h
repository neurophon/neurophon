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

// File Name:		TemporalPopulation.h
// File Description:	TemporalPopulation class definition. This file presents TemporalPopulation' public
//			interface without revealing the implementations of TemporalPopulation' member functions,
//			which are defined in TemporalPopulation.cpp.


// TemporalPopulation class derived from class TemporalUnits through inheritance.
// Class Description: This is a self organizing feature map kind class.
// It represents a bunch of neurons (units) arranged in an d-dimensional array.
// Its activaton function determines a unique active neuron per ejecution.

#ifndef TEMPORAL_POPULATION_H
#define TEMPORAL_POPULATION_H

#include "Structs.h"									// includes structs difinition
#include "TemporalUnits.h"								// TemporalUnits class declaration

class TemporalPopulation : public TemporalUnits
{
public:
				TemporalPopulation( const std::vector<int>&, int,
						    const std::vector<int>& );		// constructor that initializes TemporalPopulation
											// _temporalUnits are initialized to zero

				TemporalPopulation( const std::vector<int>&, int,
						    const threedvector<double>& );	// constructor that initializes TemporalPopulation
											// _temporalUnits are initialized from previous vectors

	std::vector<double>	Activate( const responseInfo&,
					  const std::vector<int>&, const bool );	// returns the normalized coordinates of the best active unit
											// depending on response info and on _temporalUnits.
	std::vector<double>	Activate( const std::vector<double>&,
					  const std::vector<int>&, const bool );	// returns the normalized coordinates of the best active unit
											// depending on input and on _temporalUnits.
	twodvector<double>	Activate( const responseInfo&, const std::vector<int>&,
					  const bool, int );				// returns the normalized coordinates of the "n" best active units
											// depending on response info and on _temporalUnits,
											// where "n" is a function parameter.
											// The coordinates are sorted in a ranking of decreasing
											// activation priority.
											// In case of parity between the units, their position is
											// chosen randomly.
											// In case of parity between the last units in the ranking,
											// the classified units are chosen at random in order to
											// enter in the ranking of units activated.
	twodvector<double>	Activate( const std::vector<double>&, const std::vector<int>&,
					  const bool, int );				// returns the normalized coordinates of the "n" best active units
											// depending on input and on _temporalUnits,
											// where "n" is a function parameter.
											// The coordinates are sorted in a ranking of decreasing
											// activation priority.
											// In case of parity between the units, their position is
											// chosen randomly.
											// In case of parity between the last units in the ranking,
											// the classified units are chosen by random in order to
											// enter in the ranking of units activated.
	twodvector<double>	Activate( const responseInfo&, const std::vector<int>&,
					  const double, const bool );			// decides which units in the population to activate depending on response info.
											// If a set of best matching units is inside activationRadius, the function chose
											// the more active of them based on distances and on _temporalUnits.
											// Then, it returns the corresponding unit's normalized coordinates.
											// In case of parity, all the units become active.
											// In the other case, (if no unit is inside activationRadius)
											// coordinates are filled with "-1".
	twodvector<double>	Activate( const std::vector<double>&, const std::vector<int>&,
					  const double, const bool );			// decides which unit in the population to activate depending on input.
											// If a set of best matching units is inside activationRadius, the function chose
											// the most active of them based on distances and on _temporalUnits.
											// Then, it returns the corresponding unit's normalized coordinates.
											// In case of parity, all the units become active.
											// In the other case, (if no unit is inside activationRadius)
											// coordinates are filled with "-1"
	responseInfo	getTemporalResponse( const responseInfo&,
					     const std::vector<int>& );			// modifies the response ussing _temporalUnits
											// the response is a responseInfo type structure which is compose of
											// a vector with distances and an vector with indexes sorthed in ascending order
											// corresponding to the vector of distances

private:
	std::vector<double>	Normalize( const std::vector<int>& );			// normalizes neuron coorsinates in the population

}; // end class TemporalPopulation

#endif

