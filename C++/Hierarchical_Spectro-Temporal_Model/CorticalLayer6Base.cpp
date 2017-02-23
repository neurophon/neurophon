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

// File Name:		CorticalLayer6Base.cpp
// File Description:	CorticalLayer6Base member-function definitions. This file contains implementations of the
//			member functions prototyped in CorticalLayer6Base.h.

#include <iostream>
#include <stdlib.h>							// calloc, exit, free
#include <float.h>
#include "CorticalLayer6Base.h"						// include definition of class CorticalLayer6Base

using namespace std;

// constructor initializes the dimensionality of the input and output vectors, this function validates the object too 
CorticalLayer6Base::CorticalLayer6Base( int l5InputDim, int inputDim, int outputDim, KohonenFeatureMapAbs &clusterOfNeurons )
	: CorticalLayerAbs( inputDim, outputDim, clusterOfNeurons )	// explicitly calls CorticalLayerAbs base-class constructor
{
	setL5InputDimensionality( l5InputDim );				// sets the dimensionality of the extra input vector from layer 5
	validateObject();						// function that validates the object
	reservL5InputVector();						// reserves space for the extra inputs vector from layer 5
} // end CorticalLayer6Base constructor


// function to validate the object created of this class
void	CorticalLayer6Base::validateObject()
{
	int inputDim, extraInputDim, outputDim, inputClusterDim, outputClusterDim;

	inputDim = CorticalLayerAbs::getInputDimensionality();
	extraInputDim = CorticalLayer6Base::getL5InputDimensionality();
	outputDim = CorticalLayerAbs::getOutputDimensionality();

	inputClusterDim = cluster.getInputDimensionality();
	outputClusterDim = cluster.getOutputDimensionality();

	if ( inputDim + extraInputDim != inputClusterDim || outputDim != outputClusterDim )
	{
		cout << "CorticalLayer6Base object inconsistence: inputDimensionality + extraInputDim = " << inputDim + extraInputDim << " and inputClusterDim = " << inputClusterDim << "\n" << endl;
		cout << "CorticalLayer6Base object inconsistence: outputDimensionality = " << outputDim << " and outputClusterDim = " << outputClusterDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// this function propagates the signals through the cortical layer and obtains an output
void	CorticalLayer6Base::propagateSignals( neurotransmitters parameter )
{
	int	number;
	double	*vector;
	levelOfExcitation level, levelL5;

	number = CorticalLayerAbs::getInputDimensionality();
	vector = CorticalLayerAbs::getInputVector();
	level = CorticalLayer6Base::excitationLevel(number, vector);		// obtains the level of excitation to the cortical layer
	free(vector);
	vector = NULL;

	number = CorticalLayer6Base::getL5InputDimensionality();
	vector = CorticalLayer6Base::getL5InputVector();
	levelL5 = CorticalLayer6Base::excitationLevel(number, vector);		// obtains the level of excitation to the cortical layer
	free(vector);
	vector = NULL;

	if ( (level.maximum >= parameter.sharpThreshold ) && (levelL5.active) )	// if this level is greater than the threshold and the input from layer five is active
	{
		CorticalLayer6Base::putOnInput();
		// propagates the input signals through the cluster of neurons
		if ( parameter.enableLearning == 1 )
		{
			cluster.kohonenFeatureMapping(true, parameter.learningRate, true, parameter.learningNeighbor, parameter.firingNeighbor);
		}
		else
		{
			cluster.kohonenFeatureMapping(false, parameter.learningRate, true, parameter.learningNeighbor, parameter.firingNeighbor);
		}
		CorticalLayer6Base::putOnOutput();
	}
	else									// if not
	{
		// the cluster of neurons doesn't react at all
		cluster.kohonenFeatureMapping(false, parameter.learningRate, false, parameter.learningNeighbor, parameter.firingNeighbor);
		CorticalLayer6Base::putOnOutput();
	}
} // end function propagateSignals


// this function determines the level of excitation that arrives to the vector specified by the parameters
levelOfExcitation	CorticalLayer6Base::excitationLevel( int numberOfComponents, double *vectorPointer )
{
	int i;
	double aux;
	levelOfExcitation excitation;

	excitation.active = true;						// active is initialized with true
	excitation.maximum = -DBL_MAX;						// maximum is initialized with the minimum double representation
	excitation.average = 0;							// average is initialized to zero

	for (i = 0; i < numberOfComponents; i++)
	{
		if ( *(vectorPointer + i) == 0 )			// tests if the input component is equal to zero
			excitation.active = false;			// active is false if some of input's components if equal to zero

		// accumulates the components in average
		if ( *(vectorPointer + i) < 0 )
		{
			aux = -*(vectorPointer + i);
		}
		else
		{
			aux = *(vectorPointer + i);
		}
		excitation.average = excitation.average + aux;
		if ( excitation.maximum < aux )				// compares maximum with the following component
			excitation.maximum = aux;			// updates maximum just in case this is less than the following component
	}

	excitation.average = excitation.average/(numberOfComponents);	// computes the average on the vector
	return excitation;						// returns the structure excitation
} // end function excitationLevel


// this function puts the inputs (the first input and the input from layer 5) of the layer on the input of the cluster
void	CorticalLayer6Base::putOnInput()
{
	int	i, numberOfInputs, numberOfInputsFromL5;
	double	*vector, *inputVectorPointer;

	numberOfInputs = CorticalLayerAbs::getInputDimensionality();			// gets the number of first inputs in this cortical layer
	numberOfInputsFromL5 = CorticalLayer6Base::getL5InputDimensionality();		// gets the number of inputs from layer 5 in this cortical layer
	vector = (double*)calloc(numberOfInputs+numberOfInputsFromL5, sizeof(double));	// reserves space for vector

	// this part is for the first input to this cortical layer
	inputVectorPointer = CorticalLayerAbs::getInputVector();			// gets the first input in this layer
	for (i = 0; i < numberOfInputs; i++)
	{
		*(vector + i) = *(inputVectorPointer + i);				// stores the first input of the layer in the first part of vector
	}

	// this part is for the input from layer 5 to this cortical layer
	free(inputVectorPointer);							// frees the space used by te vector to which inputVectorPointer points
	inputVectorPointer = CorticalLayer6Base::getL5InputVector();			// gets the cortical layer's input from layer 5
	for (i = numberOfInputs; i < numberOfInputsFromL5+numberOfInputs; i++)
	{
		*(vector + i) = *(inputVectorPointer + i-numberOfInputs);		// stores the input from layer 5 in the second part of vector
	}

	cluster.setInputVector(vector);							// sets the input of the cluster with the content of vector
	free(inputVectorPointer);							// frees the space used by te vector to which inputVectorPointer points
	free(vector);									// frees the memory space used by vector
} // end function putOnInput


// this function puts the output of the cluster on the output of the layer
void	CorticalLayer6Base::putOnOutput()
{
	double	*outputVectorPointer;

	outputVectorPointer = cluster.getOutputVector();		// gets the output of the cluster in this layer
	CorticalLayerAbs::setOutputVector(outputVectorPointer);		// sets the output of the layer with the content of outputVectorPointer
	free(outputVectorPointer);					// frees the space used by te vector to which outputVectorPointer points
} // end function putOnOutput


// function to set the dimensionality of the extra input from layer 5
void	CorticalLayer6Base::setL5InputDimensionality( int dimension )
{
	l5InputDimensionality = dimension;				// store the dimensionality of the extra input from layer 5
} // end function setL5InputDimensionality


// function to get the dimensionality of the input in this layer
int	CorticalLayer6Base::getL5InputDimensionality()
{
	return l5InputDimensionality;					// return the dimensionality of the extra input from layer 5
} // end function getL5InputDimensionality


// function to reserve space for the extra inputs vector from layer 5
void	CorticalLayer6Base::reservL5InputVector()
{
	int numberOfInputs;

	numberOfInputs = CorticalLayer6Base::getL5InputDimensionality();		// gets the number of extra inputs from layer 5

	l5InputVector = (double*)calloc(numberOfInputs, sizeof(double));	// reserves space for the l5InputVector vector that comes from layer 5
} // end function reservL5InputVector


// function to set the inputs vector from layer 5
void	CorticalLayer6Base::setL5InputVector( double *inputs )
{
	int j, numberOfInputs;

	numberOfInputs = CorticalLayer6Base::getL5InputDimensionality();	// gets the number of inputs from layer 5

	for (j = 0; j < numberOfInputs; j++)
		*(l5InputVector + j) = *(inputs + j);				// store the inputs in the l5InputVector vector that comes from layer 5
} // end function setL5InputVector


// function to get the inputs vector
double	*CorticalLayer6Base::getL5InputVector()
{
	int j, numberOfInputs;
	double *vector;

	numberOfInputs = CorticalLayer6Base::getL5InputDimensionality();	// gets the number of inputs from layer 5

	vector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for vector

	for (j = 0; j < numberOfInputs; j++)
		*(vector + j) = *(l5InputVector + j);				// stores the l5InputVector in vector

	return vector;								// returns vector pointer
} // end function getL5InputVector







