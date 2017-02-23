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

// File Name:		CorticalLayerPred.cpp
// File Description:	CorticalLayerPred member-function definitions. This file contains implementations of the
//			member functions prototyped in CorticalLayerPred.h.

#include <iostream>
#include <stdlib.h>
#include "CorticalLayerPred.h"						// include definition of class CorticalLayerPred

using namespace std;

// constructor initializes the dimensionality of the input and output vectors, this function validates the object too 
CorticalLayerPred::CorticalLayerPred( int inputDim, int outputDim, KohonenFeatureMapPred &clusterOfNeurons )
	: cluster( clusterOfNeurons )					// initialize cluster KohonenFeatureMapPred object
{
	setInputDimensionality( inputDim );				// sets the dimensionality of the input vector
	setOutputDimensionality( outputDim );				// sets the dimensionality of the output vector
	validateObject();						// calls function to validate the object
	reservInputVector();						// reserves space for the inputs vector
	reservOutputVector();						// reserves space for the outputs vector
} // end CorticalLayerPred constructor


// function to validate the object created of this class
void	CorticalLayerPred::validateObject()
{
	int inputDim, outputDim;

	inputDim = CorticalLayerPred::getInputDimensionality();
	outputDim = CorticalLayerPred::getOutputDimensionality();

	if ( inputDim <= 0 || outputDim <= 0 )
	{
		cout << "CorticalLayerPred object inconsistence: inputDimensionality = " << inputDim << "\n" << endl;
		cout << "CorticalLayerPred object inconsistence: outputDimensionality = " << outputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// function to set the dimensionality of the input in this layer
void	CorticalLayerPred::setInputDimensionality( int dimension )
{
	inputDimensionality = dimension;				// store the dimensionality of the input of this cortical layer
} // end function setInputDimensionality


// function to get the dimensionality of the input in this layer
int	CorticalLayerPred::getInputDimensionality()
{
	return inputDimensionality;					// return the dimensionality of the input of this cortical layer
} // end function getInputDimensionality


// function to set the dimensionality of the output in this layer
void	CorticalLayerPred::setOutputDimensionality( int dimension )
{
	outputDimensionality = dimension;				// store the dimensionality of the output of this cortical layer
} // end function setOutputDimensionality


// function to get the dimensionality of the output in this layer
int	CorticalLayerPred::getOutputDimensionality()
{
	return outputDimensionality;					// return the dimensionality of the output of this cortical layer
} // end function getInputDimensionality


// function to reserve space for the inputs vector
void	CorticalLayerPred::reservInputVector()
{
	int numberOfInputs;

	numberOfInputs = CorticalLayerPred::getInputDimensionality();		// gets the number of inputs

	inputVector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for the inputVector vector
} // end function reserveInputVector


// function to set the inputs vector
void	CorticalLayerPred::setInputVector( double *inputs )
{
	int j, numberOfInputs;

	numberOfInputs = CorticalLayerPred::getInputDimensionality();		// gets the number of inputs

	for (j = 0; j < numberOfInputs; j++)
		*(inputVector + j) = *(inputs + j);				// store the inputs in the inputVector vector
} // end function setInputVector


// function to get the inputs vector
double	*CorticalLayerPred::getInputVector()
{
	int j, numberOfInputs;
	double *vector;

	numberOfInputs = CorticalLayerPred::getInputDimensionality();		// gets the number of inputs

	vector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for vector

	for (j = 0; j < numberOfInputs; j++)
		*(vector + j) = *(inputVector + j);				// stores the inputVector in vector

	return vector;								// returns vector pointer
} // end function getInputVector


// function to reserve space for the outputs vector
void	CorticalLayerPred::reservOutputVector()
{
	int numberOfOutputs;

	numberOfOutputs = CorticalLayerPred::getOutputDimensionality();		// gets the number of output units

	outputVector = (double*)calloc(numberOfOutputs, sizeof(double));	// reserves space for the outputVector vector
} // end function reserveOutputVector


// function to set the outputs vector
void	CorticalLayerPred::setOutputVector( double *outputs )
{
	int i, numberOfOutputs;

	numberOfOutputs = CorticalLayerPred::getOutputDimensionality();		// gets the number of output units

	for (i = 0; i < numberOfOutputs; i++)
		*(outputVector + i) = *(outputs + i);				// store the outputs in the outputVector vector
} // end function setOutputVector


// function to get the outputs vector
double	*CorticalLayerPred::getOutputVector()
{
	int i, numberOfOutputs;
	double *vector;

	numberOfOutputs = CorticalLayerPred::getOutputDimensionality();		// gets the number of output units

	vector = (double*)calloc(numberOfOutputs, sizeof(double));		// reserves space for vector

	for (i = 0; i < numberOfOutputs; i++)
		*(vector + i) = *(outputVector + i);				// stores the outputVector in vector

	return vector;								// returns vector pointer
} // end function getOutputVector







