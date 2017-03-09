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

#include "Structs.h"							// includes struct difinitions
#include "../Libraries/DataTypes.h"
#include "SelfOrganizingMap.h"						// SelfOrganizingMap class declaration

class TemporalUnits : public SelfOrganizingMap
{
public:
		TemporalUnits( const std::vector<int>&, int,
			       const std::vector<int>& );			// constructor that initializes TemporalUnits by default
		TemporalUnits( const std::vector<int>&, int,
			       const ThreeD_Vector_Double& );			// constructor that initializes TemporalUnits by previous vector

	void	Update( const responseInfo&, const std::vector<int>&,
			const bool, const double );				// updates _temporalUnits depending on response info.
	void	Update( const std::vector<double>&, const std::vector<int>&,
			const bool, const double );				// updates _temporalUnits depending on input.
	void	Update( const int, const std::vector<int>&,
			const bool, const double );				// updates _temporalUnits depending on unit index.
	void	Update( const int, const int, const int,
			       const bool, const double );			// updates _temporalUnits depending on individual unit indexes.
	void	Update( const responseInfo&, double, const std::vector<int>&,
			const bool, const double, const std::string& );		// updates _temporalUnits neighborhood depending on response info.
	void	Update( const std::vector<double>&, double, const std::vector<int>&,
			const bool, const double, const std::string& );		// updates _temporalUnits neighborhood depending on input.
	void	Update( const int, double, const std::vector<int>&,
			const bool, const double, const std::string& );		// updates _temporalUnits neighborhood depending on unit index.
	void	Update( const int, double, const int, const int,
			const bool, const double, const std::string& );		// updates _temporalUnits neighborhood depending on individual unit indexes.

	void	saveTemporalUnitsStatus();			// function that saves the TemporalUnits status in a file

protected:
	ThreeD_Vector_Double	_temporalUnits;			// matrixes with temporal units stats

private:
	int			_updateStep;			// accumulates the steps elapsed until now in order to check UPDATE_PERIOD
}; // end class TemporalUnits

#endif

