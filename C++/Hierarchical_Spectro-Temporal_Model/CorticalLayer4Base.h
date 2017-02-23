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

// File Name:		CorticalLayer4Base.h
// File Description:	CorticalLayer4Base class definition. This file presents CorticalLayer4Base's public
//			interface without revealing the implementations of CorticalLayer4Base's member functions,
//			which are defined in CorticalLayer4Base.cpp.


// Class Description: This is a CorticalLayer4Base class. This is a specialization from CorticalLayerPred class through inheritance.
#ifndef CORTICALLAYER4BASE_H
#define CORTICALLAYER4BASE_H

#include "CorticalLayerPred.h"							// CorticalLayerPred class declaration

class CorticalLayer4Base : public CorticalLayerPred
{
public:
		CorticalLayer4Base( int, int, KohonenFeatureMapPred & );	// constructor that initializes CorticalLayer4Base
	void	validateObject();						// function that validates the object

	void	propagateSignals( neurotransmitters );				// propagates the signals through the layer
	levelOfExcitation	excitationLevel();				// determines the level of excitation that arrives to the input of the layer

	void	putOnInput();							// puts the input of the layer on the input of the cluster
	void	putOnOutput();							// puts the output of the cluster on the output of the layer
}; // end class CorticalLayer4Base

#endif

