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

// File Name:		CorticalLayer6Base.h
// File Description:	CorticalLayer6Base class definition. This file presents CorticalLayer6Base's public
//			interface without revealing the implementations of CorticalLayer6Base's member functions,
//			which are defined in CorticalLayer6Base.cpp.


// Class Description: This is a CorticalLayer6Base class. This is a specialization from CorticalLayer class through inheritance.
// By means of this class we can define only Layer 6 objects.
#ifndef CORTICALLAYER6BASE_H
#define CORTICALLAYER6BASE_H

#include "CorticalLayerAbs.h"							// CorticalLayerAbs class declaration

class CorticalLayer6Base : public CorticalLayerAbs
{
public:
		CorticalLayer6Base( int, int, int, KohonenFeatureMapAbs & );	// constructor that initializes CorticalLayer6Base
	void	validateObject();						// function that validates the object

	void	propagateSignals( neurotransmitters );				// propagates the signals through the layer
	levelOfExcitation	excitationLevel( int, double * );		// determines the level of excitation that arrives to the input of the layer

	void	putOnInput();							// puts the input of the layer on the input of the cluster
	void	putOnOutput();							// puts the output of the cluster on the output of the layer

	void	setL5InputDimensionality( int );				// function that sets the dimensionality of the extra input vector from layer 5
	int	getL5InputDimensionality();					// function that gets the dimensionality of the extra input vector from layer 5

	void	reservL5InputVector();						// function that reserves space for the extra inputs vector from layer 5
	void	setL5InputVector( double * );					// function that sets the extra inputs vector from layer 5
	double	*getL5InputVector();						// function that gets the extra inputs vector from layer 5

private:
	int	l5InputDimensionality;						// dimensionality of the input from layer 5
	double	*l5InputVector;							// vector of inputs from layer 5
}; // end class CorticalLayer6Base

#endif

