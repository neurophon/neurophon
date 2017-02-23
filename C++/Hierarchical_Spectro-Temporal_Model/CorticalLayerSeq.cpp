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

// File Name:		CorticalLayerSeq.cpp
// File Description:	CorticalLayerSeq member-function definitions. This file contains implementations of the
//			member functions prototyped in CorticalLayerSeq.h.

#include <iostream>
#include <stdlib.h>
#include "CorticalLayerSeq.h"						// include definition of class CorticalLayerSeq

using namespace std;

// constructor initializes the dimensionality of the input and output vectors, this function validates the object too 
CorticalLayerSeq::CorticalLayerSeq( int inputDim, int outputDim, L23GroupAbs &groupOfClusters )
	: group( groupOfClusters )					// initialize group L23GroupAbs object
{
	setInputDimensionality( inputDim );				// sets the dimensionality of the input vector
	setOutputDimensionality( outputDim );				// sets the dimensionality of the output vector
	validateObject();						// calls function to validate the object
	reservInputVector();						// reserves space for the inputs vector
	reservOutputVector();						// reserves space for the outputs vector
} // end CorticalLayerSeq constructor


// function to validate the object created of this class
void	CorticalLayerSeq::validateObject()
{
	int inputDim, outputDim;

	inputDim = CorticalLayerSeq::getInputDimensionality();
	outputDim = CorticalLayerSeq::getOutputDimensionality();

	if ( inputDim <= 0 || outputDim <= 0 )
	{
		cout << "CorticalLayerSeq object inconsistence: inputDimensionality = " << inputDim << "\n" << endl;
		cout << "CorticalLayerSeq object inconsistence: outputDimensionality = " << outputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// function to set the dimensionality of the input in this layer
void	CorticalLayerSeq::setInputDimensionality( int dimension )
{
	inputDimensionality = dimension;				// store the dimensionality of the input of this cortical layer
} // end function setInputDimensionality


// function to get the dimensionality of the input in this layer
int	CorticalLayerSeq::getInputDimensionality()
{
	return inputDimensionality;					// return the dimensionality of the input of this cortical layer
} // end function getInputDimensionality


// function to set the dimensionality of the output in this layer
void	CorticalLayerSeq::setOutputDimensionality( int dimension )
{
	outputDimensionality = dimension;				// store the dimensionality of the output of this cortical layer
} // end function setOutputDimensionality


// function to get the dimensionality of the output in this layer
int	CorticalLayerSeq::getOutputDimensionality()
{
	return outputDimensionality;					// return the dimensionality of the output of this cortical layer
} // end function getInputDimensionality


// function to reserve space for the inputs vector
void	CorticalLayerSeq::reservInputVector()
{
	int numberOfInputs;

	numberOfInputs = CorticalLayerSeq::getInputDimensionality();		// gets the number of inputs

	inputVector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for the inputVector vector
} // end function reserveInputVector


// function to set the inputs vector
void	CorticalLayerSeq::setInputVector( double *inputs )
{
	int j, numberOfInputs;

	numberOfInputs = CorticalLayerSeq::getInputDimensionality();		// gets the number of inputs

	for (j = 0; j < numberOfInputs; j++)
		*(inputVector + j) = *(inputs + j);				// store the inputs in the inputVector vector
} // end function setInputVector


// function to get the inputs vector
double	*CorticalLayerSeq::getInputVector()
{
	int j, numberOfInputs;
	double *vector;

	numberOfInputs = CorticalLayerSeq::getInputDimensionality();		// gets the number of inputs

	vector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for vector

	for (j = 0; j < numberOfInputs; j++)
		*(vector + j) = *(inputVector + j);				// stores the inputVector in vector

	return vector;								// returns vector pointer
} // end function getInputVector


// function to reserve space for the outputs vector
void	CorticalLayerSeq::reservOutputVector()
{
	int numberOfOutputs;

	numberOfOutputs = CorticalLayerSeq::getOutputDimensionality();		// gets the number of output units

	outputVector = (double*)calloc(numberOfOutputs, sizeof(double));	// reserves space for the outputVector vector
} // end function reserveOutputVector


// function to set the outputs vector
void	CorticalLayerSeq::setOutputVector( double *outputs )
{
	int i, numberOfOutputs;

	numberOfOutputs = CorticalLayerSeq::getOutputDimensionality();		// gets the number of output units

	for (i = 0; i < numberOfOutputs; i++)
		*(outputVector + i) = *(outputs + i);				// store the outputs in the outputVector vector
} // end function setOutputVector


// function to get the outputs vector
double	*CorticalLayerSeq::getOutputVector()
{
	int i, numberOfOutputs;
	double *vector;

	numberOfOutputs = CorticalLayerSeq::getOutputDimensionality();		// gets the number of output units

	vector = (double*)calloc(numberOfOutputs, sizeof(double));		// reserves space for vector

	for (i = 0; i < numberOfOutputs; i++)
		*(vector + i) = *(outputVector + i);				// stores the outputVector in vector

	return vector;								// returns vector pointer
} // end function getOutputVector







