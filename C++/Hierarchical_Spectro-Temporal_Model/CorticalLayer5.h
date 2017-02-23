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

// File Name:		CorticalLayer5.h
// File Description:	CorticalLayer5 class definition. This file presents CorticalLayer5's public
//			interface without revealing the implementations of CorticalLayer5's member functions,
//			which are defined in CorticalLayer5.cpp.


// Class Description: This is a CorticalLayer5 class. This is a specialization from CorticalLayer class through inheritance.
// By means of this class we can define not only Layer 4 objects but also Layer 5 objects because both of them have the same configuration.
#ifndef CORTICALLAYER5_H
#define CORTICALLAYER5_H

#include "CorticalLayerAbs.h"							// CorticalLayerAbs class declaration

class CorticalLayer5 : public CorticalLayerAbs
{
public:
		CorticalLayer5( int, int, KohonenFeatureMapAbs & );		// constructor that initializes CorticalLayer5
	void	validateObject();						// function that validates the object

	void	propagateSignals( neurotransmitters );				// propagates the signals through the layer
	levelOfExcitation	excitationLevel();				// determines the level of excitation that arrives to the input of the layer

	void	putOnInput();							// puts the input of the layer on the input of the cluster
	void	putOnOutput();							// puts the output of the cluster on the output of the layer
}; // end class CorticalLayer5

#endif

