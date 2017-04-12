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

// File Name:		TemporalUnits.h
// File Description:	TemporalUnits class definition. This file presents TemporalUnits' public
//			interface without revealing the implementations of TemporalUnits' member functions,
//			which are defined in TemporalUnits.cpp.


// TemporalUnits class derived from class SelfOrganizingMap through inheritance.
// Class Description: This is a self organizing feature map kind class.
// It elaborates a statistic relationship between different units activity.
// This is about the frequency with which certain units activity happens.
// This is specially useful to represent the statistical relationship between the units activities
// produced in different time steps. 

#ifndef TEMPORAL_UNITS_H
#define TEMPORAL_UNITS_H

#include <fstream>

#include "Structs.h"							// includes struct difinitions
#include "../Libraries/DataTypes.h"
#include "SelfOrganizingMap.h"						// SelfOrganizingMap class declaration

class TemporalUnits : public SelfOrganizingMap
{
public:
		TemporalUnits( const std::vector<int>&, int,
			       const std::vector<int>& );			// constructor that initializes _temporalUnits by default to zero
										// the first parameter is the dimensionality of the array of units,
										// for example (20,10,5) (a prisma with 1000 units);
										// the second one is the number of input components and the
										// third one is a vector in which every component is the number of units
										// that can be connected from the same or another group of units with this one.
		TemporalUnits( const std::string&, const std::string& );	// constructor that initializes _temporalUnits by means of previous
										// values from file

	void	Update( const responseInfo&, const std::vector<int>&,
			const bool, const double, const double );		// updates _temporalUnits depending on response info,
										// using the minimum distance,
										// if there are more than one unit with the minimum distance,
										// this choses one at random.
	void	Update( const std::vector<double>&, const std::vector<int>&,
			const bool, const double, const double );		// updates _temporalUnits depending on input.
	void	Update( const int, const std::vector<int>&,
			const bool, const double, const double );		// updates _temporalUnits depending on unit index.
	void	Update( const int, const int, const int,
			const bool, const double, const double );		// updates _temporalUnits depending on individual unit indexes.
	void	Update( const responseInfo&, double, const std::vector<int>&,
			const bool, const double,
			const std::string&, const double );			// updates _temporalUnits taking into account the neighborhood
										// structure of the array of units and depending on response info.
	void	Update( const std::vector<double>&, double, const std::vector<int>&,
			const bool, const double,
			const std::string&, const double );			// updates _temporalUnits taking into account the neighborhood
										// structure of the array of units and depending on input.
	void	Update( const int, double, const std::vector<int>&,
			const bool, const double,
			const std::string&, const double );			// updates _temporalUnits taking into account the neighborhood
										// structure of the array of units and depending on unit index.
	void	Update( const int, double, const int, const int,
			const bool, const double,
			const std::string&, const double );			// updates _temporalUnits taking into account the neighborhood
										// structure of the array of units and depending on individual unit indexes.

	void	Update( const std::vector<int>&, const twodvector<int>&,
			       const bool, const double, const double );	// updates a group of _temporalUnits depending on a set of unit indexes.
										// on the other hand, every link -from which information is coming- is a vector
										// hence, it could contain a set of linking units as well as a unique linking unit.

	void	saveTemporalUnitsStatus( const std::string, std::ofstream& );	// function that saves the TemporalUnits' status in a file

	void	loadTemporalUnitsStatus( const std::string, std::ifstream& );	// function to save the TemporalUnits' status from a file

protected:
	threedvector<double>	_temporalUnits;			// matrixes with temporal units stats

private:
	int			_updateStep;			// accumulates the steps elapsed until now in order to check UPDATE_PERIOD
}; // end class TemporalUnits

#endif

