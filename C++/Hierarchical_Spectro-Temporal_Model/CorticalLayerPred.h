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

// File Name:		CorticalLayerPred.h
// File Description:	CorticalLayerPred class definition. This file presents CorticalLayerPred' public
//			interface without revealing the implementations of CorticalLayerPred' member functions,
//			which are defined in CorticalLayerPred.cpp.


// Class Description: This is a general cortical layer with a vector of axons as input and another vector of axons as output and a KohonenFeatureMapPred cluster.
// This cortical layer contains a cluster of neurons of kind KohonenFeatureMapPred.
// This class specializes in CorticalLayer6, CorticalLayer5, CorticalLayer4 and CorticalLayer23 by means of inheritance.
#ifndef CORTICALLAYERPRED_H
#define CORTICALLAYERPRED_H

#include "KohonenFeatureMapPred.h"			// KohonenFeatureMapPred class declaration
#include "Structs.h"					// Structs definitions for the cortical layers

class CorticalLayerPred
{
public:
		CorticalLayerPred( int, int, KohonenFeatureMapPred & );	// constructor that initializes CorticalLayerPred
	void	validateObject();			// function that validates the object

	void	setInputDimensionality( int );		// function that sets the dimensionality of the input vector
	int	getInputDimensionality();		// function that gets the dimensionality of the input vector

	void	setOutputDimensionality( int );		// function that sets the dimensionality of the output vector
	int	getOutputDimensionality();		// function that gets the dimensionality of the output vector

	void	reservInputVector();			// function that reserves space for the inputs vector
	void	setInputVector( double * );		// function that sets the inputs vector
	double	*getInputVector();			// function that gets the inputs vector

	void	reservOutputVector();			// function that reserves space for the outputs vector
	void	setOutputVector( double * );		// function that sets the outputs vector
	double	*getOutputVector();			// function that gets the outputs vector

private:
	int	inputDimensionality;			// dimensionality of the input
	int	outputDimensionality;			// dimensionality of the output

	double	*inputVector;				// vector of inputs
	double	*outputVector;				// vector of outputs

protected:
	KohonenFeatureMapPred		cluster;	// this is the cluster inside the cortical layer
}; // end class CorticalLayerPred

#endif




