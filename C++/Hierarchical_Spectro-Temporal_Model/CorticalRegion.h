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

// File Name:		CorticalRegion.h
// File Description:	CorticalRegion class definition. This file presents CorticalRegion's public
//			interface without revealing the implementations of CorticalRegion's member functions,
//			which are defined in CorticalRegion.cpp.


// Class Description: This is a empty box with one input and one output. With specialization through inheritance, this empty box will contain a region of cortex with several nodes.
// Those nodes could be of CorticalNodeAbs kind class or of CorticalNodeRel kind class
#ifndef REGION_H
#define REGION_H

class CorticalRegion
{
public:
		CorticalRegion( int, int );		// constructor that initializes CorticalRegion
	void	validateObject();			// function that validates the object

	void	setInputDimensionality( int );		// function that sets the dimensionality of the input vector
	int	getInputDimensionality();		// function that gets the dimensionality of the input vector

	void	setOutputDimensionality( int );		// function that sets the dimensionality of the output vector
	int	getOutputDimensionality();		// function that gets the dimensionality of the output vector

	void	reservInputVector();			// function that reserves space for the inputs vector
	void	setInputVector( int, double * );	// function that sets the inputs vector
	double	*getInputVector( int );			// function that gets the inputs vector

	void	reservOutputVector();			// function that reserves space for the outputs vector
	void	setOutputVector( double * );		// function that sets the outputs vector
	double	*getOutputVector();			// function that gets the outputs vector

	void	setInputSynchronization( bool );	// function that sets the input synchronization of the region
	bool	getInputSynchronization();		// function that gets the input synchronization of the region
	void	setOutputSynchronization( bool );	// function that sets the output synchronization of the region
	bool	getOutputSynchronization();		// function that gets the output synchronization of the region

private:
	int	inputDimensionality;			// dimensionality of the input
	int	outputDimensionality;			// dimensionality of the output

	bool	inputSynchronization;			// this is the synchronization input of the region
	bool	outputSynchronization;			// this is the synchronization output of the region

	double	*inputVector[5];			// vector of inputs
	double	*outputVector;				// vector of outputs
}; // end class CorticalRegion

#endif




