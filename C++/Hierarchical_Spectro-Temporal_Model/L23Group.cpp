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

// File Name:		L23Group.cpp
// File Description:	L23Group member-function definitions. This file contains implementations of the
//			member functions prototyped in L23Group.h.

#include <iostream>
#include <stdlib.h>
#include "L23Group.h"						// include definition of class L23GroupAbs

using namespace std;

// constructor initializes the dimensionality of the input, the dimensionality of the output and the number of clusters in this group
L23Group::L23Group( int inputDimensions, int outputDimensions, int clustersNumber )
{
	setInputDimensionality( inputDimensions );			// calls set function to initialize inputDimensionality
	setOutputDimensionality( outputDimensions );			// calls set function to initialize outputDimensionality
	setNumberOfClusters( clustersNumber );				// calls set function to initialize numberOfClusters
	validateObject();						// calls function to validate the object
	reservInputVector();						// calls function to reserve space for the inputs vector
	reservOutputVector();						// calls function to reserve space for the outputs vector
} // end L23GroupAbs constructor


// function to validate the object created of this class
void	L23Group::validateObject()
{
	int inputDim, outputDim, clusters;

	inputDim = L23Group::getInputDimensionality();
	outputDim = L23Group::getOutputDimensionality();
	clusters = L23Group::getNumberOfClusters();

	if ( inputDim <= 0 || outputDim <= 0 || clusters <= 1)
	{
		cout << "L23Group object inconsistence: inputDimensionality = " << inputDim << "\n" << endl;
		cout << "L23Group object inconsistence: outputDimensionality = " << outputDim << "\n" << endl;
		cout << "L23Group object inconsistence: numberOfClusters must be >= 2.\nnumberOfClusters = " << clusters << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// function to set the dimensionality of the input in this group
void	L23Group::setInputDimensionality( int dimension )
{
	inputDimensionality = dimension;				// store the dimensionality of the input of this group
} // end function setInputDimensionality


// function to get the dimensionality of the input in this group
int	L23Group::getInputDimensionality()
{
	return inputDimensionality;					// return the dimensionality of the input of this group
} // end function getInputDimensionality


// function to set the dimensionality of the output in this group
void	L23Group::setOutputDimensionality( int dimension )
{
	outputDimensionality = dimension;				// store the dimensionality of the output of this group
} // end function setOutputDimensionality


// function to get the dimensionality of the output in this group
int	L23Group::getOutputDimensionality()
{
	return outputDimensionality;					// return the dimensionality of the output of this group
} // end function getInputDimensionality


// function to set the number of clusters in this group
void	L23Group::setNumberOfClusters( int number )
{
	numberOfClusters = number;					// store the number of clusters of this group
} // end function setNumberOfClusters


// function to get the number of clusters in this group
int	L23Group::getNumberOfClusters()
{
	return numberOfClusters;					// return the number of clusters of this group
} // end function getNumberOfClusters


// function to reserve space for the inputs vector
void	L23Group::reservInputVector()
{
	int numberOfInputs;

	numberOfInputs = L23Group::getInputDimensionality();			// gets the number of inputs

	inputVector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for the inputVector vector
} // end function reserveInputVector


// function to set the inputs vector
void	L23Group::setInputVector( double *inputs )
{
	int j, numberOfInputs;

	numberOfInputs = L23Group::getInputDimensionality();			// gets the number of inputs

	for (j = 0; j < numberOfInputs; j++)
		*(inputVector + j) = *(inputs + j);				// store the inputs in the inputVector vector
} // end function setInputVector


// function to get the inputs vector
double	*L23Group::getInputVector()
{
	int j, numberOfInputs;
	double *vector;

	numberOfInputs = L23Group::getInputDimensionality();			// gets the number of inputs

	vector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for vector

	for (j = 0; j < numberOfInputs; j++)
		*(vector + j) = *(inputVector + j);				// stores the inputVector in vector

	return vector;								// returns vector pointer
} // end function getInputVector


// function to reserve space for the outputs vector
void	L23Group::reservOutputVector()
{
	int numberOfOutputs;

	numberOfOutputs = L23Group::getOutputDimensionality();		// gets the number of output units

	outputVector = (double*)calloc(numberOfOutputs, sizeof(double));	// reserves space for the outputVector vector
} // end function reserveOutputVector


// function to set the outputs vector
void	L23Group::setOutputVector( double *outputs )
{
	int i, numberOfOutputs;

	numberOfOutputs = L23Group::getOutputDimensionality();		// gets the number of output units

	for (i = 0; i < numberOfOutputs; i++)
		*(outputVector + i) = *(outputs + i);				// store the outputs in the outputVector vector
} // end function setOutputVector


// function to get the outputs vector
double	*L23Group::getOutputVector()
{
	int i, numberOfOutputs;
	double *vector;

	numberOfOutputs = L23Group::getOutputDimensionality();			// gets the number of output units

	vector = (double*)calloc(numberOfOutputs, sizeof(double));		// reserves space for vector

	for (i = 0; i < numberOfOutputs; i++)
		*(vector + i) = *(outputVector + i);				// stores the outputVector in vector

	return vector;								// returns vector pointer
} // end function getOutputVector





