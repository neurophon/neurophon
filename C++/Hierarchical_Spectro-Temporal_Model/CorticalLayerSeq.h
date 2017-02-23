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

// File Name:		CorticalLayerSeq.h
// File Description:	CorticalLayerSeq class definition. This file presents CorticalLayerSeq' public
//			interface without revealing the implementations of CorticalLayerSeq' member functions,
//			which are defined in CorticalLayerSeq.cpp.


// Class Description: This is a general cortical layer with a vector of axons as input and another vector of axons as output and a L23GroupAbs group.
// This cortical layer contains a group of clusters of kind L23GroupAbs.
// This class specializes in CorticalLayer23 by means of inheritance.
#ifndef CORTICALLAYERSEQ_H
#define CORTICALLAYERSEQ_H

#include "L23GroupAbs.h"				// L23GroupAbs class declaration
#include "Structs.h"					// Structs definitions for the cortical layers

class CorticalLayerSeq
{
public:
		CorticalLayerSeq( int, int, L23GroupAbs & );	// constructor that initializes CorticalLayerSeq
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
	L23GroupAbs		group;	// this is the group of clusters inside the cortical layer
}; // end class CorticalLayerSeq

#endif




