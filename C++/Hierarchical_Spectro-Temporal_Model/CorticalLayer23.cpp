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

// File Name:		CorticalLayer23.cpp
// File Description:	CorticalLayer23 member-function definitions. This file contains implementations of the
//			member functions prototyped in CorticalLayer23.h.

#include <iostream>
#include <stdlib.h>							// calloc, exit, free
#include <float.h>
#include "CorticalLayer23.h"						// include definition of class CorticalLayer23

using namespace std;

// constructor initializes the dimensionality of the input and output vectors, this function validates the object too 
CorticalLayer23::CorticalLayer23( int temporalPool, int inputDim, int outputDim, int backwardInputDim, int backwardOutputDim, int synchronization, L23GroupAbs &forwarGroupOfClusters, L23GroupAbs &backwardGroupOfClusters )
	: backwardGroup( backwardGroupOfClusters ),			// initialize backwardGroup L23GroupAbs object
	  CorticalLayerSeq( inputDim, outputDim, forwarGroupOfClusters )// explicitly calls CorticalLayerSeq base-class constructor
{
	setTemporalPooling( temporalPool );				// calls function to set the temporal pooling of the layer
	setBackwardInputDimensionality( backwardInputDim );		// calls function to set the dimensionality of the backward input vector
	setBackwardOutputDimensionality( backwardOutputDim );		// calls function to set the dimensionality of the backward output vector
	setSynchronizationCount( synchronization );			// calls function to set the synchronization count
	setActiveState( false );					// calls function to reset activeState in the layer
	setOutputSynchronization(false);				// sets the output synchronization of the layer to false
	validateObject();						// function that validates the object
	reservInternalVector();						// reserves space for the internal vector
	reservBackwardInputVector();					// reserves space for the backward input vector
	reservBackwardOutputVector();					// reserves space for the backward output vector
} // end CorticalLayer23 constructor


// function to validate the object created of this class
void	CorticalLayer23::validateObject()
{
	int forwardInputDim, forwardOutputDim, backwardInputDim, backwardOutputDim;
	int forwardGroupInputDim, forwardGroupOutputDim, backwardGroupInputDim, backwardGroupOutputDim, temporal;

	forwardInputDim = CorticalLayerSeq::getInputDimensionality();
	forwardOutputDim = CorticalLayerSeq::getOutputDimensionality();
	backwardInputDim = CorticalLayer23::getBackwardInputDimensionality();
	backwardOutputDim = CorticalLayer23::getBackwardOutputDimensionality();

	forwardGroupInputDim = group.getInputDimensionality();
	forwardGroupOutputDim = group.getOutputDimensionality();

	backwardGroupInputDim = backwardGroup.getInputDimensionality();
	backwardGroupOutputDim = backwardGroup.getOutputDimensionality();

	temporal = CorticalLayer23::getTemporalPooling();

	if ( backwardInputDim <= 0 || backwardOutputDim <= 0 )
	{
		cout << "CorticalLayer23 object inconsistence: backwardInputDim = " << backwardInputDim << "\n" << endl;
		cout << "CorticalLayer23 object inconsistence: backwardOutputDim = " << backwardOutputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( forwardInputDim != forwardGroupInputDim )
	{
		cout << "CorticalLayer23 object inconsistence: forwardInputDim = " << forwardInputDim << " is different to" << endl;
		cout << "forwardGroupInputDim = " << forwardGroupInputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( temporal*forwardGroupOutputDim != forwardOutputDim )
	{
		cout << "CorticalLayer23 object inconsistence: temporal*forwardGroupOutputDim = " << temporal*forwardGroupOutputDim << " is different to" << endl;
		cout << "forwardOutputDim = " << forwardOutputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( forwardInputDim+backwardInputDim != backwardGroupInputDim )
	{
		cout << "CorticalLayer23 object inconsistence: forwardInputDim+backwardInputDim = " << forwardInputDim+backwardInputDim << " is different to" << endl;
		cout << "backwardGroupInputDim = " << backwardGroupInputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( backwardGroupOutputDim != backwardOutputDim )
	{
		cout << "CorticalLayer23 object inconsistence: backwardGroupOutputDim = " << backwardGroupOutputDim << " is different to" << endl;
		cout << "backwardOutputDim = " << backwardOutputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// this function propagates the signals through the cortical layer and obtains an output
void	CorticalLayer23::propagateSignals( neurotransmitters forwardGroupParameter, neurotransmitters backwardGroupParameter )
{
	int	i;
	int	number, temporal, count;
	bool	active;
	double	average, maximum;
	double	*vector;
	levelOfExcitation	forwardLevel, backwardLevel, internalLevel;

	CorticalLayer23::setOutputSynchronization(false);						// resets the output synchronization of the layer to false
	number = CorticalLayerSeq::getInputDimensionality();						// gets the number of components in the forward input to the layer
	vector = CorticalLayerSeq::getInputVector();							// gets the forward input of the layer
	forwardLevel = CorticalLayer23::excitationLevel(number, vector);				// obtains the level of excitation on the forward input of the cortical layer
	free(vector);
	vector = NULL;
	number = CorticalLayer23::getBackwardInputDimensionality();					// gets the number of components in the backward input to the layer
	vector = CorticalLayer23::getBackwardInputVector();						// gets the backward input of the layer
	backwardLevel = CorticalLayer23::excitationLevel(number, vector);				// obtains the level of excitation on the backward input of the cortical layer
	free(vector);
	vector = NULL;

	temporal = CorticalLayer23::getTemporalPooling();						// gets the temporal pooling of the layer

	//active = getActiveState();									// gets the active state of the layer

	CorticalLayer23::putOnForwardInput();								// puts the forward input of the layer on the input of the forward group
	if ( forwardLevel.active )									// if forward input is active
	{
		// propagates the input signals through the forward group in the layer
		group.propagateSignals(forwardGroupParameter.enableLearning, forwardGroupParameter.learningRate, true, forwardGroupParameter.learningNeighbor, forwardGroupParameter.firingNeighbor);
	}
	else												// if not
	{
		// the forward group of clusters doesn't react al all
		group.propagateSignals(forwardGroupParameter.enableLearning, forwardGroupParameter.learningRate, false, forwardGroupParameter.learningNeighbor, forwardGroupParameter.firingNeighbor);
	}

/*	CorticalLayer23::putOnBackwardInput();								// puts the forward and the backward inputs of the layer on the input of the backward group
	if ( (forwardLevel.active) && (backwardLevel.active) )						// if forward and backward inputs are active
	{
		// propagates the input signals through the backward group in the layer
		backwardGroup.propagateSignals(forwardGroupParameter.enableLearning, forwardGroupParameter.learningRate, true, forwardGroupParameter.learningNeighbor, forwardGroupParameter.firingNeighbor);
	}
	else												// if not
	{
		// the backward group of clusters doesn't react al all
		backwardGroup.propagateSignals(forwardGroupParameter.enableLearning, forwardGroupParameter.learningRate, false, forwardGroupParameter.learningNeighbor, forwardGroupParameter.firingNeighbor);
	}*/

	number = group.getOutputDimensionality();					// gets the number of components in the output from the forward group
	vector = group.getOutputVector();						// gets the output of the forward group
	internalLevel = CorticalLayer23::excitationLevel(number, vector);		// obtains the level on the output forward group of the cortical layer
	free(vector);
	vector = NULL;

	if ( internalLevel.active )								// if internalLevel is active
	{
		count = CorticalLayer23::getSynchronizationCount();				// gets the synchronization count of the layer

		if ( count == temporal-1 )
		{
			CorticalLayer23::addInternalVector();					// adds the forward group output to the internal vector of the layer
			CorticalLayer23::setSynchronizationCount(0);				// resets the synchronization count of the layer
			CorticalLayer23::putOnForwardOutput();					// puts the internal vector of the layer on the forward output of the layer
			CorticalLayer23::setOutputSynchronization(true);			// sets the output synchronization of the layer to true
			CorticalLayer23::resetInternalVector();					// resets the internal vector of the layer to zero
			group.resetGroup();							// resets the forward group in the layer
		}
		else
		{
			CorticalLayer23::addInternalVector();					// adds the forward group output to the internal vector of the layer
			CorticalLayer23::setSynchronizationCount(count+1);			// increments the synchronization count of the layer
		}
	}
	else
	{
		CorticalLayer23::setSynchronizationCount(0);					// resets the synchronization count of the layer
		CorticalLayer23::resetInternalVector();						// resets the internal vector of the layer to zero
		CorticalLayer23::putOnForwardOutput();						// puts the internal vector of the layer on the forward output of the layer
		CorticalLayer23::setOutputSynchronization(true);				// sets the output synchronization of the layer to true
		group.resetGroup();								// resets the forward group in the layer
	}

	//CorticalLayer23::putOnBackrwardOutput();						// puts the output of the backward group on the backward output of the layer
} // end function propagateSignals


// this function determines the level of excitation that arrives to the vector specified by the parameters
levelOfExcitation	CorticalLayer23::excitationLevel( int numberOfComponents, double *vectorPointer )
{
	int i;
	double aux;
	levelOfExcitation excitation;

	excitation.active = true;					// active is initialized with true
	//excitation.maximum = -DBL_MAX;					// maximum is initialized with the minimum double representation
	//excitation.average = 0;						// average is initialized to zero

	for (i = 0; i < numberOfComponents; i++)
	{
		if ( *(vectorPointer + i) == 0 )			// tests if the input component is equal to zero
			excitation.active = false;			// active is false if some of input's components if equal to zero

	/*	// accumulates the components in average
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
	*/
	}

	//excitation.average = excitation.average/(numberOfComponents);	// computes the average on the vector
	return excitation;						// returns the structure excitation
} // end function excitationLevel


// this function puts the forward inputs of the layer on the input of the forward group
void	CorticalLayer23::putOnForwardInput()
{
	double	*inputPointer;

	inputPointer = CorticalLayerSeq::getInputVector();		// gets the forward input vector of the layer
	group.setInputVector(inputPointer);				// puts inputPointer contents on the input of the forward group
	free(inputPointer);						// frees space used by the vector to which inputPointer points
} // end function putOnForwardInput


// this function puts the internal vector of the layer on the forward output of the layer
void	CorticalLayer23::putOnForwardOutput()
{
	double	*outputVector;

	outputVector = CorticalLayer23::getInternalVector();		// gets the pointer to the internal vector of the layer

	CorticalLayerSeq::setOutputVector(outputVector);		// puts outputVector contents on the forward output of the layer
	free(outputVector);						// frees space used by the vector to which outputVector points	
} // end function putOnForwardOutput


// this function puts the backward input of the layer on the second part input of the backward group and the forward input of the layer on the first part input of the backward group
void	CorticalLayer23::putOnBackwardInput()
{
	int	i, numberOfInputs, numberOfForwardInputs, numberOfBackwardInputs;
	double	*vector;
	double	*forwardInputPointer, *backwardInputPointer;

	numberOfInputs = backwardGroup.getInputDimensionality();			// gets the number of inputs to the backward group
	vector = (double*)calloc(numberOfInputs, sizeof(double));			// reserves space for vector that has the same number of components as the backward group input vector

	numberOfForwardInputs = CorticalLayerSeq::getInputDimensionality();		// gets the number of forward inputs to the layer
	numberOfBackwardInputs = CorticalLayer23::getBackwardInputDimensionality();	// gets the number of backward inputs to the layer

	forwardInputPointer = CorticalLayerSeq::getInputVector();			// gets the pointer to the forward input vector of the layer
	backwardInputPointer = CorticalLayer23::getBackwardInputVector();		// gets the pointer to the backward input vector of the layer

	for (i = 0; i < numberOfForwardInputs; i++)
		*(vector + i) = *(forwardInputPointer + i);				// stores the forward input vector on the first part of vector

	for (i = numberOfForwardInputs; i < numberOfBackwardInputs+numberOfForwardInputs; i++)
		*(vector + i) = *(backwardInputPointer + i-numberOfForwardInputs);	// stores the forward input vector on the first part of vector

	backwardGroup.setInputVector(vector);						// puts the content of vector in the input of the backward group
	free(vector);									// frees the memory space used by vector
	free(forwardInputPointer);							// frees space used by the vector to which forwardInputPointer points
	free(backwardInputPointer);							// frees space used by the vector to which backwardInputPointer points
} // end function putOnBackwardInput


// this function puts the output of the backward group on the backward output of the layer
void	CorticalLayer23::putOnBackrwardOutput()
{
	double	*outputPointer;

	outputPointer = backwardGroup.getOutputVector();				// gets the pointer to the output vector of the backward group
	CorticalLayer23::setBackwardOutputVector(outputPointer);			// puts the content at which outputPointer points on the backward output of the layer
	free(outputPointer);								// frees space used by the vector to which outputPointer points	
} // end function putOnBackrwardOutput


// function to set the dimensionality of the backward input vector
void	CorticalLayer23::setBackwardInputDimensionality( int dimension )
{
	backwardInputDimensionality = dimension;			// store the dimensionality of the backward input vector
} // end function setBackwardInputDimensionality


// function to get the dimensionality of the backward input vector
int	CorticalLayer23::getBackwardInputDimensionality()
{
	return backwardInputDimensionality;				// return the dimensionality of the backward input vector
} // end function getBackwardInputDimensionality


// function to set the dimensionality of the backward output vector
void	CorticalLayer23::setBackwardOutputDimensionality( int dimension )
{
	backwardOutputDimensionality = dimension;			// store the dimensionality of the backward output vector
} // end function setBackwardOutputDimensionality


// function to get the dimensionality of the backward output vector
int	CorticalLayer23::getBackwardOutputDimensionality()
{
	return backwardOutputDimensionality;				// return the dimensionality of the backward output vector
} // end function getBackwardOutputDimensionality


// function to set the synchronization count of the layer
void	CorticalLayer23::setSynchronizationCount( int synchronization )
{
	synchronizationCount = synchronization;				// stores the synchronization count of the layer
} // end function setSynchronizationCount


// function to get the synchronization count of the layer
int	CorticalLayer23::getSynchronizationCount()
{
	return synchronizationCount;					// returns the synchronization count of the layer
} // end function getSynchronizationCount


// function to set the state of the layer
void	CorticalLayer23::setActiveState( bool state )
{
	activeState = state;
} // end function setActiveState


// function to get the state of the layer
bool	CorticalLayer23::getActiveState()
{
	return activeState;
} // end function getActiveState


// function to reserve space for the internal vector
void	CorticalLayer23::reservInternalVector()
{
	int numberOfOutputs, temporal;

	temporal = CorticalLayer23::getTemporalPooling();			// gets the temporal pooling of the layer
	numberOfOutputs = group.getOutputDimensionality();		// gets the number of forward group outputs

	internalVector = (double*)calloc(temporal*numberOfOutputs, sizeof(double));	// reserves space for the internalVector vector
} // end function reservInternalVector


// function to reset the internal vector of the layer to zero
void	CorticalLayer23::resetInternalVector()
{
	int numberOfOutputs, temporal;
	double	*vector;

	temporal = CorticalLayer23::getTemporalPooling();			// gets the temporal pooling of the layer
	numberOfOutputs = group.getOutputDimensionality();			// gets the number of forward group outputs
	vector = (double*)calloc(temporal*numberOfOutputs, sizeof(double));	// reserves space for vector

	CorticalLayer23::setInternalVector(vector);				// sets the internal vector to zero
	free(vector);								// frees the memory used by vector
} // end function resetInternalVector


// function to set the internal vector
void	CorticalLayer23::setInternalVector( double *vector )
{
	int j;
	int numberOfOutputs, temporal;

	temporal = CorticalLayer23::getTemporalPooling();			// gets the temporal pooling of the layer
	numberOfOutputs = group.getOutputDimensionality();			// gets the number of forward group outputs

	for (j = 0; j < temporal*numberOfOutputs; j++)
		*(internalVector + j) = *(vector + j);				// stores vector in the internalVector of the layer
} // end function setInternalVector


// function to add the forward group's output of the layer to the internal vector of the layer
void	CorticalLayer23::addInternalVector()
{
	int	j;
	int	sync, numberOfOutputs;
	double	*output, *vector;

	sync = CorticalLayer23::getSynchronizationCount();			// gets the synchronization count in the layer
	numberOfOutputs = group.getOutputDimensionality();			// gets the number of forward group outputs
	output = group.getOutputVector();					// gets the output of the forward group in the layer
	vector = CorticalLayer23::getInternalVector();				// gets the internal vector of the layer

	for ( j = 0; j < numberOfOutputs; j++ )
	{
		{
			*((vector + sync*numberOfOutputs) + j) = *(output + j);	// adds forward group's output to the internal vector of the layer
		}
	}

	CorticalLayer23::setInternalVector(vector);
	free(output);								// frees the memory used by output
	free(vector);								// frees the memory used by vector
} // end function addInternalVector


// function to get the internal vector of the layer
double	*CorticalLayer23::getInternalVector()
{
	int j;
	int numberOfOutputs, temporal;
	double *vector;

	temporal = CorticalLayer23::getTemporalPooling();			// gets the temporal pooling of the layer
	numberOfOutputs = group.getOutputDimensionality();		// gets the number of forward group outputs

	vector = (double*)calloc(temporal*numberOfOutputs, sizeof(double));	// reserves space for vector

	for (j = 0; j < temporal*numberOfOutputs; j++)
		*(vector + j) = *(internalVector + j);				// stores internalVector in vector

	return vector;								// returns vector pointer
} // end function getInternalVector


// function to reserve space for the backward inputs vector
void	CorticalLayer23::reservBackwardInputVector()
{
	int numberOfInputs;

	numberOfInputs = CorticalLayer23::getBackwardInputDimensionality();	// gets the number of backward inputs

	backwardInputVector = (double*)calloc(numberOfInputs, sizeof(double));	// reserves space for the backwardInputVector vector
} // end function reservBackwardInputVector


// function to set the backward input vector
void	CorticalLayer23::setBackwardInputVector( double *inputs )
{
	int j, numberOfInputs;

	numberOfInputs = CorticalLayer23::getBackwardInputDimensionality();	// gets the number of backward inputs

	for (j = 0; j < numberOfInputs; j++)
		*(backwardInputVector + j) = *(inputs + j);			// stores the inputs in the backwardInputVector vector
} // end function setBackwardInputVector


// function to get the backward input vector
double	*CorticalLayer23::getBackwardInputVector()
{
	int j, numberOfInputs;
	double *vector;

	numberOfInputs = CorticalLayer23::getBackwardInputDimensionality();	// gets the number of backward inputs

	vector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for vector

	for (j = 0; j < numberOfInputs; j++)
		*(vector + j) = *(backwardInputVector + j);			// stores the backwardInputVector in vector

	return vector;								// returns vector pointer
} // end function getBackwardInputVector


// function to reserve space for the backward output vector
void	CorticalLayer23::reservBackwardOutputVector()
{
	int numberOfOutputs;

	numberOfOutputs = CorticalLayer23::getBackwardOutputDimensionality();		// gets the number of backward outputs

	backwardOutputVector = (double*)calloc(numberOfOutputs, sizeof(double));	// reserves space for the backwardOutputVector vector
} // end function reservBackwardOutputVector


// function to set the backward output vector
void	CorticalLayer23::setBackwardOutputVector( double *outputs )
{
	int j, numberOfOutputs;

	numberOfOutputs = CorticalLayer23::getBackwardOutputDimensionality();		// gets the number of backward outputs

	for (j = 0; j < numberOfOutputs; j++)
		*(backwardOutputVector + j) = *(outputs + j);				// stores the outputs in the backwardOutputVector vector
} // end function setBackwardOutputVector


// function to get the backward output vector
double	*CorticalLayer23::getBackwardOutputVector()
{
	int j, numberOfOutputs;
	double *vector;

	numberOfOutputs = CorticalLayer23::getBackwardOutputDimensionality();		// gets the number of backward outputs

	vector = (double*)calloc(numberOfOutputs, sizeof(double));			// reserves space for vector

	for (j = 0; j < numberOfOutputs; j++)
		*(vector + j) = *(backwardOutputVector + j);				// stores the backwardOutputVector in vector

	return vector;									// return vector pointer
} // end function getBackwardOutputVector


// function to set the output synchronization of the layer
void	CorticalLayer23::setOutputSynchronization( bool sinc )
{
	outputSynchronization = sinc;
} // end function setOutputSynchronization


// function to get the output synchronization of the layer
bool	CorticalLayer23::getOutputSynchronization()
{
	return outputSynchronization;
} // end function getOutputSynchronization


// function to set the temporal pooling of the layer
void	CorticalLayer23::setTemporalPooling( int temporal )
{
	temporalPooling = temporal;					// stores the temporal pooling of the layer
} // end function setTemporalPooling


// function to get the temporal pooling of the layer
int	CorticalLayer23::getTemporalPooling()
{
	return temporalPooling;						// returns the temporal pooling of the layer
} // end function getTemporalPooling




