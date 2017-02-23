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

// File Name:		CorticalLayer23.h
// File Description:	CorticalLayer23 class definition. This file presents CorticalLayer23' public
//			interface without revealing the implementations of CorticalLayer23' member functions,
//			which are defined in CorticalLayer23.cpp.


// Class Description: This is a CorticalLayer23 class. This is a specialization from CorticalLayer class through inheritance.
// By means of this class we can define Layer23 objects.
#ifndef CorticalLayer23_H
#define CorticalLayer23_H

#include "CorticalLayerSeq.h"								// CorticalLayer class declaration
#include "L23GroupAbs.h"

class CorticalLayer23 : public CorticalLayerSeq
{
public:
		CorticalLayer23( int, int, int, int, int, int, L23GroupAbs &, L23GroupAbs & );	// constructor that initializes CorticalLayer23
	void	validateObject();							// function that validates the object

	void	propagateSignals( neurotransmitters, neurotransmitters );		// propagates the signals through the layer
	levelOfExcitation	excitationLevel( int, double * );			// determines the level of excitation that arrives to the vector specified by the function parameters

	void	putOnForwardInput();							// puts the forward input of the layer on the input of the forward group
	void	putOnForwardOutput();							// puts the internal vector of the layer on the forward output of the layer
	void	putOnBackwardInput();							// puts the backward input of the layer on the second part input of the backward group and the forward input of the layer on the first part input of the backward group
	void	putOnBackrwardOutput();							// puts the output of the backward group on the backward output of the layer

	void	setBackwardInputDimensionality( int );					// function that sets the dimensionality of the backward input vector
	int	getBackwardInputDimensionality();					// function that gets the dimensionality of the backward input vector

	void	setBackwardOutputDimensionality( int );					// function that sets the dimensionality of the backward output vector
	int	getBackwardOutputDimensionality();					// function that gets the dimensionality of the backward output vector

	void	setSynchronizationCount( int );						// function that sets the synchronization count of the cluster
	int	getSynchronizationCount();						// function that gets the synchronization count of the cluster

	void	setActiveState( bool );							// function that sets the state of the layer
	bool	getActiveState();							// function that gets the state of the layer

	void	reservInternalVector();							// function that reserves space for the internal vector
	void	resetInternalVector();							// function that resets the internal vector to zero
	void	setInternalVector( double * );						// function that sets the internal vector
	void	addInternalVector();							// function that adds the forward group output of the layer to the internal vector of the layer
	double	*getInternalVector();							// function that gets the internal vector

	void	reservBackwardInputVector();						// function that reserves space for the backward input vector
	void	setBackwardInputVector( double * );					// function that sets the backward input vector
	double	*getBackwardInputVector();						// function that gets the backward input vector

	void	reservBackwardOutputVector();						// function that reserves space for the backward output vector
	void	setBackwardOutputVector( double * );					// function that sets the backward output vector
	double	*getBackwardOutputVector();						// function that gets the backward output vector

	void	setOutputSynchronization( bool );					// function that sets the output synchronization of the layer
	bool	getOutputSynchronization();						// function that gets the output synchronization of the layer

	void	setTemporalPooling( int );						// function that sets the temporal pooling of the layer
	int	getTemporalPooling();							// function that gets the temporal pooling of the layer

private:
	int	temporalPooling;							// temporal pooling of the layer
	int	synchronizationCount;							// synchronization of the cluster in the layer
	int	backwardInputDimensionality;						// dimensionality of the backward input
	int	backwardOutputDimensionality;						// dimensionality of the backward output

	bool	activeState;								// establishes the state of the layer
	bool	outputSynchronization;							// this is the synchronization output of the layer

	double	*internalVector;							// internal vector of the layer
	double	*backwardInputVector;							// backward input vector
	double	*backwardOutputVector;							// backwar output vector

	L23GroupAbs	backwardGroup;							// group of clusters to process the backward information
}; // end class CorticalLayer23

#endif

