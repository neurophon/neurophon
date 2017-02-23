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

// File Name:		CorticalRegion.cpp
// File Description:	CorticalRegion member-function definitions. This file contains implementations of the
//			member functions prototyped in CorticalRegion.h.

#include <iostream>
#include <stdlib.h>
#include "CorticalRegion.h"						// include definition of class CorticalNodeAbs

using namespace std;

// constructor initializes the dimensionality of the input and the dimensionality of the output of the cortical region
CorticalRegion::CorticalRegion( int inputDimensions, int outputDimensions )
{
	setInputDimensionality( inputDimensions );			// calls set function to initialize inputDimensionality
	setOutputDimensionality( outputDimensions );			// calls set function to initialize outputDimensionality
	setInputSynchronization(false);					// calls function to set the input synchronization of the region
	setOutputSynchronization(false);				// calls function to set the output synchronization of the region
	validateObject();						// calls function to validate the object
	reservInputVector();						// calls function to reserve space for the inputs vector
	reservOutputVector();						// calls function to reserve space for the outputs vector
} // end CorticalRegion constructor


// function to validate the object created of this class
void	CorticalRegion::validateObject()
{
	int inputDim, outputDim;

	inputDim = CorticalRegion::getInputDimensionality();
	outputDim = CorticalRegion::getOutputDimensionality();

	if ( inputDim <= 0 || outputDim <= 0 )
	{
		cout << "CorticalRegion object inconsistence: inputDimensionality = " << inputDim << "\n" << endl;
		cout << "CorticalRegion object inconsistence: outputDimensionality = " << outputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// function to set the dimensionality of the input in this region
void	CorticalRegion::setInputDimensionality( int dimension )
{
	inputDimensionality = dimension;				// stores the dimensionality of the input of this region
} // end function setInputDimensionality


// function to get the dimensionality of the input in this region
int	CorticalRegion::getInputDimensionality()
{
	return inputDimensionality;					// returns the dimensionality of the input of this region
} // end function getInputDimensionality


// function to set the dimensionality of the output in this region
void	CorticalRegion::setOutputDimensionality( int dimension )
{
	outputDimensionality = dimension;				// stores the dimensionality of the output of this region
} // end function setOutputDimensionality


// function to get the dimensionality of the output in this region
int	CorticalRegion::getOutputDimensionality()
{
	return outputDimensionality;					// returns the dimensionality of the output of this region
} // end function getInputDimensionality


// function to reserve space for the inputs vector
void	CorticalRegion::reservInputVector()
{
	int numberOfInputs;

	numberOfInputs = CorticalRegion::getInputDimensionality();		// gets the number of inputs

	inputVector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for the inputVector vector
} // end function reserveInputVector


// function to set the inputs vector
void	CorticalRegion::setInputVector( double *inputs )
{
	int j, numberOfInputs;

	numberOfInputs = CorticalRegion::getInputDimensionality();		// gets the number of inputs

	for (j = 0; j < numberOfInputs; j++)
		*(inputVector + j) = *(inputs + j);				// store the inputs in the inputVector vector
} // end function setInputVector


// function to get the inputs vector
double	*CorticalRegion::getInputVector()
{
	int j, numberOfInputs;
	double *vector;

	numberOfInputs = CorticalRegion::getInputDimensionality();		// gets the number of inputs

	vector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for vector

	for (j = 0; j < numberOfInputs; j++)
		*(vector + j) = *(inputVector + j);				// stores the inputVector in vector

	return vector;								// returns vector pointer
} // end function getInputVector


// function to reserve space for the outputs vector
void	CorticalRegion::reservOutputVector()
{
	int numberOfOutputs;

	numberOfOutputs = CorticalRegion::getOutputDimensionality();		// gets the number of output units

	outputVector = (double*)calloc(numberOfOutputs, sizeof(double));	// reserves space for the outputVector vector
} // end function reserveOutputVector


// function to set the outputs vector
void	CorticalRegion::setOutputVector( double *outputs )
{
	int i, numberOfOutputs;

	numberOfOutputs = CorticalRegion::getOutputDimensionality();		// gets the number of output units

	for (i = 0; i < numberOfOutputs; i++)
		*(outputVector + i) = *(outputs + i);				// store the outputs in the outputVector vector
} // end function setOutputVector


// function to get the outputs vector
double	*CorticalRegion::getOutputVector()
{
	int i, numberOfOutputs;
	double *vector;

	numberOfOutputs = CorticalRegion::getOutputDimensionality();		// gets the number of output units

	vector = (double*)calloc(numberOfOutputs, sizeof(double));		// reserves space for vector

	for (i = 0; i < numberOfOutputs; i++)
		*(vector + i) = *(outputVector + i);				// stores the outputVector in vector

	return vector;								// returns vector pointer
} // end function getOutputVector


// function to set the input synchronization of the region
void	CorticalRegion::setInputSynchronization( bool sinc )
{
	inputSynchronization = sinc;
} // end function setInputSynchronization


// function to get the input synchronization of the region
bool	CorticalRegion::getInputSynchronization()
{
	return inputSynchronization;
} // end function getInputSynchronization


// function to set the output synchronization of the region
void	CorticalRegion::setOutputSynchronization( bool sinc )
{
	outputSynchronization = sinc;
} // end function setOutputSynchronization


// function to get the output synchronization of the region
bool	CorticalRegion::getOutputSynchronization()
{
	return outputSynchronization;
} // end function getOutputSynchronization






