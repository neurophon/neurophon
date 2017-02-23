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

// File Name:		CorticalNodeAbsBase.h
// File Description:	CorticalNodeAbsBase class definition. This file presents CorticalNodeAbsBase' public
//			interface without revealing the implementations of CorticalNodeAbsBase' member functions,
//			which are defined in CorticalNodeAbsBase.cpp.


// Class Description: This is a cortical node. This cortical node is composed by layers 6, 5, 4 and 2/3.
// In this cortical node the groups of clusters in layer 2/3 fire in absolute way.
#ifndef CORTICALNODEABSBASE_H
#define CORTICALNODEABSBASE_H

#include <thread>
#include "CorticalLayer4Base.h"					// includes the definition of class CorticalLayer4Base
#include "CorticalLayer5.h"					// includes the definition of class CorticalLayer5
#include "CorticalLayer6Base.h"					// includes the definition of class CorticalLayer6Base
#include "CorticalLayer23.h"					// includes the definition of class CorticalLayer23

class CorticalNodeAbsBase
{
public:
		CorticalNodeAbsBase( int, int, int, int, int, CorticalLayer4Base &, CorticalLayer5 &, CorticalLayer6Base &, CorticalLayer23 & );	// constructor that initializes CorticalNodeAbs
	void	validateObject();				// function that validates the object

	void	propagateSignals( neurotransmitters, neurotransmitters, neurotransmitters, neurotransmitters, neurotransmitters );	// propagates the signals through the layer

	void	putOnForwardInput();				// puts the forward input of the node on the input of the layer 4 and on the forward input of layer 6
	void	putOnForwardOutput();				// puts the forward output of layer 2/3 on the forward output of the node
	void	putOnBackwardInput();				// puts the backward input of the node on the backward input of layer 2/3
	void	putOnBackrwardOutput();				// puts the output of layer 6 on the backward output of the node
	void	putOnExtraBackrwardOutput();			// puts the output of layer 5 on the extra backward output of the node

	void	layer4ToLayer23();				// puts the output of layer 4 on the forward input of layer 2/3
	void	layer23ToLayer5();				// puts the backward output of layer 2/3 on the input of layer 5
	void	layer5ToLayer6();				// puts the output of layer 5 on the backward input of layer 6

	void	setForwardInputDimensionality( int );		// function that sets the dimensionality of the forward input vector
	int	getForwardInputDimensionality();		// function that gets the dimensionality of the forward input vector

	void	setForwardOutputDimensionality( int );		// function that sets the dimensionality of the forward output vector
	int	getForwardOutputDimensionality();		// function that gets the dimensionality of the forward output vector

	void	reservForwardInputVector();			// function that reserves space for the forward input vector
	void	setForwardInputVector( double * );		// function that sets the forward input vector
	double	*getForwardInputVector();			// function that gets the forward input vector

	void	reservForwardOutputVector();			// function that reserves space for the forward output vector
	void	setForwardOutputVector( double * );		// function that sets the forward output vector
	double	*getForwardOutputVector();			// function that gets the forward output vector

	void	setBackwardInputDimensionality( int );		// function that sets the dimensionality of the backward input vector
	int	getBackwardInputDimensionality();		// function that gets the dimensionality of the backward input vector

	void	setBackwardOutputDimensionality( int );		// function that sets the dimensionality of the backward output vector
	int	getBackwardOutputDimensionality();		// function that gets the dimensionality of the backward output vector

	void	reservBackwardInputVector();			// function that reserves space for the backward input vector
	void	setBackwardInputVector( double * );		// function that sets the backward input vector
	double	*getBackwardInputVector();			// function that gets the backward input vector

	void	reservBackwardOutputVector();			// function that reserves space for the backward output vector
	void	setBackwardOutputVector( double * );		// function that sets the backward output vector
	double	*getBackwardOutputVector();			// function that gets the backward output vector

	void	setExtraBackwardOutputDimensionality( int );	// function that sets the dimensionality of the extra backward output vector
	int	getExtraBackwardOutputDimensionality();		// function that gets the dimensionality of the extra backward output vector

	void	reservExtraBackwardOutputVector();		// function that reserves space for the extra backward output vector
	void	setExtraBackwardOutputVector( double * );	// function that sets the extra backward output vector
	double	*getExtraBackwardOutputVector();		// function that gets the extra backward output vector

	void	setInputSynchronization( bool );		// function that sets the input synchronization of the node
	bool	getInputSynchronization();			// function that gets the input synchronization of the node
	void	setOutputSynchronization( bool );		// function that sets the output synchronization of the node
	bool	getOutputSynchronization();			// function that gets the output synchronization of the node

	std::thread propagateSignalsLauncher( neurotransmitters, neurotransmitters, neurotransmitters, neurotransmitters, neurotransmitters );	// function that launches a thread to run the method propagateSignals

private:
	int	forwardInputDimensionality;				// dimensionality of the input of the forward information
	int	forwardOutputDimensionality;				// dimensionality of the output of the forward information
	int	backwardInputDimensionality;				// dimensionality of the input of the backward information
	int	backwardOutputDimensionality;				// dimensionality of the output of the backward information
	int	extraBackwardOutputDimensionality;			// dimensionality of the output of the extra backward information

	bool	inputSynchronization;					// this is the synchronization input of the node
	bool	outputSynchronization;					// this is the synchronization output of the node

	double	*forwardInputVector;					// forward input vector
	double	*forwardOutputVector;					// forward output vector
	double	*backwardInputVector;					// backward input vector
	double	*backwardOutputVector;					// backward output vector
	double	*extraBackwardOutputVector;				// extra backward output vector

	CorticalLayer4Base	layer4;					// this is the layer 4 in the cortical node
	CorticalLayer5		layer5;					// this is the layer 5 in the cortical node
	CorticalLayer6Base	layer6;					// this is the layer 6 in the cortical node 
	CorticalLayer23		layer23;				// this is the layer 23 in the cortical node
}; // end class CorticalNodeAbsBase

#endif




