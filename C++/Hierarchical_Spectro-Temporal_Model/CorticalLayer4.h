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

// File Name:		CorticalLayer4.h
// File Description:	CorticalLayer4 class definition. This file presents CorticalLayer4's public
//			interface without revealing the implementations of CorticalLayer4's member functions,
//			which are defined in CorticalLayer4.cpp.


// Class Description: This is a CorticalLayer4 class. This is a specialization from CorticalLayerPred class through inheritance.
#ifndef CORTICALLAYER4_H
#define CORTICALLAYER4_H

#include "CorticalLayerPred.h"							// CorticalLayerPred class declaration

class CorticalLayer4 : public CorticalLayerPred
{
public:
		CorticalLayer4( int, int, KohonenFeatureMapPred & );		// constructor that initializes CorticalLayer4
	void	validateObject();						// function that validates the object

	void	propagateSignals( neurotransmitters );				// propagates the signals through the layer
	levelOfExcitation	excitationLevel();				// determines the level of excitation that arrives to the input of the layer

	void	putOnInput();							// puts the input of the layer on the input of the cluster
	void	putOnOutput();							// puts the output of the cluster on the output of the layer
}; // end class CorticalLayer4

#endif

