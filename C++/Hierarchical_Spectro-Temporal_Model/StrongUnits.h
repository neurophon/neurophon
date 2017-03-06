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

// File Name:		StrongUnits.h
// File Description:	StrongUnits class definition. This file presents StrongUnits' public
//			interface without revealing the implementations of StrongUnits' member functions,
//			which are defined in StrongUnits.cpp.


// StrongUnits class derived from class SelfOrganizingMap through inheritance.
// Class Description: This is a self organizing feature map kind class.
// It elaborates a statistic about the frequency with which the units activate.

#ifndef STRONG_UNITS_H
#define STRONG_UNITS_H

#include "Structs.h"								// includes struct difinitions
#include "SelfOrganizingMap.h"							// SelfOrganizingMap class declaration

class StrongUnits : public SelfOrganizingMap
{
public:
		StrongUnits( const std::vector<int>&, int );	// constructor that initializes StrongUnits by default
		StrongUnits( const std::vector<int>&, int,
			     const std::vector<double>& );	// constructor that initializes StrongUnits by previous vector

	void	Update( const responseInfo& );			// updates _strongUnits depending on response info.
	void	Update( const std::vector<double>& );		// updates _strongUnits depending on input.
	void	Update( int );					// updates _strongUnits depending on unit index.
	void	Update( const responseInfo&, double );		// updates _strongUnits neighborhood depending on response info.
	void	Update( const std::vector<double>&, double );	// updates _strongUnits neighborhood depending on input.
	void	Update( int, double );				// updates _strongUnits neighborhood depending on unit index.

	void	saveStrongUnitsStatus();			// function that saves the StrongUnits status in a file

protected:
	std::vector<double>	_strongUnits;			// vector with strong units stats

private:
	int			_updateStep;			// accumulates the steps elapsed until now in order to check UPDATE_PERIOD
}; // end class StrongUnits

#endif

