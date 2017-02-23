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

// File Name:		CorticalLayer5.cpp
// File Description:	CorticalLayer5 member-function definitions. This file contains implementations of the
//			member functions prototyped in CorticalLayer5.h.

#include <iostream>
#include <stdlib.h>							// calloc, exit, free
#include <float.h>
#include "CorticalLayer5.h"						// include definition of class CorticalLayer5

using namespace std;

// constructor initializes the dimensionality of the input and output vectors, this function validates the object too 
CorticalLayer5::CorticalLayer5( int inputDim, int outputDim, KohonenFeatureMapAbs &clusterOfNeurons )
	: CorticalLayerAbs( inputDim, outputDim, clusterOfNeurons )	// explicitly calls CorticalLayerAbs base-class constructor
{
	validateObject();						// function that validates the object
} // end CorticalLayer5 constructor


// function to validate the object created of this class
void	CorticalLayer5::validateObject()
{
	int inputDim, outputDim, inputClusterDim, outputClusterDim;

	inputDim = CorticalLayerAbs::getInputDimensionality();
	outputDim = CorticalLayerAbs::getOutputDimensionality();

	inputClusterDim = cluster.getInputDimensionality();
	outputClusterDim = cluster.getOutputDimensionality();

	if ( inputDim != inputClusterDim || outputDim != outputClusterDim )
	{
		cout << "CorticalLayer5 object inconsistence: inputDimensionality = " << inputDim << " and inputClusterDim = " << inputClusterDim << "\n" << endl;
		cout << "CorticalLayer5 object inconsistence: outputDimensionality = " << outputDim << " and outputClusterDim = " << outputClusterDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// this function propagates the signals through the cortical layer and obtains an output
void	CorticalLayer5::propagateSignals( neurotransmitters parameter )
{
	levelOfExcitation level;

	level = CorticalLayer5::excitationLevel();				// obtains the level of excitation to the cortical layer
	if ( level.active )							// if this input is active
	{
		CorticalLayer5::putOnInput();
		// propagates the input signals through the cluster of neurons
		if ( parameter.enableLearning == 1 )
		{
			cluster.kohonenFeatureMapping(true, parameter.learningRate, true, parameter.learningNeighbor, parameter.firingNeighbor);
		}
		else
		{
			cluster.kohonenFeatureMapping(false, parameter.learningRate, true, parameter.learningNeighbor, parameter.firingNeighbor);
		}
		CorticalLayer5::putOnOutput();
	}
	else									// if not
	{
		// the cluster of neurons doesn't react at all
		cluster.kohonenFeatureMapping(false, parameter.learningRate, false, parameter.learningNeighbor, parameter.firingNeighbor);
		CorticalLayer5::putOnOutput();
	}
} // end function propagateSignals


// this function determines the level of excitation that arrives to the input of the layer
levelOfExcitation	CorticalLayer5::excitationLevel()
{
	int i, numberOfInputs;
	double *inputVectorPointer;
	levelOfExcitation excitation;

	excitation.active = true;					// active is initialized with true
//	excitation.maximum = -DBL_MAX;					// maximum is initialized with the minimum double representation
//	excitation.average = 0;						// average is initialized to zero

	numberOfInputs = CorticalLayerAbs::getInputDimensionality();	// gets the number of inputs in this cortical layer
	inputVectorPointer = CorticalLayerAbs::getInputVector();	// gets the cortical layer's input
	for (i = 0; i < numberOfInputs; i++)
	{
		// accumulates the input's components in average
//		excitation.average = excitation.average + *(inputVectorPointer + i);

		if ( *(inputVectorPointer + i) == 0 )			// tests if the input component is equal to zero
			excitation.active = false;			// active is false if some of input's components if equal to zero

//		if ( excitation.maximum < *(inputVectorPointer + i) )	// compares maximum with the following input's component
//			excitation.maximum = *(inputVectorPointer + i);	// update maximum just in case this is less than the following input's component
	}
//	excitation.average = excitation.average/numberOfInputs;		// computes the average on the input's vector

	free(inputVectorPointer);					// frees the space used by the vector to which inputVectorPointer points
	return excitation;						// returns the structure excitation
} // end function excitationLevel


// this function puts the input of the layer on the input of the cluster
void	CorticalLayer5::putOnInput()
{
	double	*inputVectorPointer;

	inputVectorPointer = CorticalLayerAbs::getInputVector();	// gets the input in this layer
	cluster.setInputVector(inputVectorPointer);			// sets the input of the cluster with the content of inputVectorPointer
	free(inputVectorPointer);					// frees the space used by the vector to which inputVectorPointer points
} // end function putOnInput


// this function puts the output of the cluster on the output of the layer
void	CorticalLayer5::putOnOutput()
{
	double	*outputVectorPointer;

	outputVectorPointer = cluster.getOutputVector();		// gets the pointer to the output of the cluster in this layer
	CorticalLayerAbs::setOutputVector(outputVectorPointer);		// sets the output of the layer with the content of outputVectorPointer
	free(outputVectorPointer);					// frees the space used by the vector to which outputVectorPointer points
} // end function putOnOutput





