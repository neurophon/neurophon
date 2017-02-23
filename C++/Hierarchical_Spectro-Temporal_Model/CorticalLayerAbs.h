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

// File Name:		CorticalLayerAbs.h
// File Description:	CorticalLayerAbs class definition. This file presents CorticalLayerAbs' public
//			interface without revealing the implementations of CorticalLayerAbs' member functions,
//			which are defined in CorticalLayerAbs.cpp.


// Class Description: This is a general cortical layer with a vector of axons as input and another vector of axons as output and a KohonenFeatureMapAbs cluster.
// This cortical layer contains a cluster of neurons of kind KohonenFeatureMapAbs.
// This class specializes in CorticalLayer6, CorticalLayer5, CorticalLayer4 and CorticalLayer23 by means of inheritance.
#ifndef CORTICALLAYERABS_H
#define CORTICALLAYERABS_H

#include "KohonenFeatureMapAbs.h"			// KohonenFeatureMapAbs class declaration
#include "Structs.h"					// Structs definitions for the cortical layers

class CorticalLayerAbs
{
public:
		CorticalLayerAbs( int, int, KohonenFeatureMapAbs & );	// constructor that initializes CorticalLayerAbs
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
	KohonenFeatureMapAbs		cluster;	// this is the cluster inside the cortical layer
}; // end class CorticalLayerAbs

#endif




