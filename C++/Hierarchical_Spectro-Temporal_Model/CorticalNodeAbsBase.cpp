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

// File Name:		CorticalNodeAbsBase.cpp
// File Description:	CorticalNodeAbsBase member-function definitions. This file contains implementations of the
//			member functions prototyped in CorticalNodeAbsBase.h.

#include <iostream>
#include <stdlib.h>							// calloc, exit, free
#include <float.h>
#include "CorticalNodeAbsBase.h"						// include definition of class CorticalNodeAbsBase

using namespace std;

// constructor initializes the dimensionality of the input and output vectors, this function validates the object too 
CorticalNodeAbsBase::CorticalNodeAbsBase( int forwardInputDim, int forwardOutputDim, int backwardInputDim, int backwardOutputDim, int extraBackwardOutputDim, CorticalLayer4Base &layerFour, CorticalLayer5 &layerFive, CorticalLayer6Base &layerSix, CorticalLayer23 &layers23 )
	: layer4( layerFour ),						// initializes the layer 4 in the node
	  layer5( layerFive ),						// initializes the layer 5 in the node
	  layer6( layerSix ),						// initializes the layer 6 in the node
	  layer23( layers23 )						// initializes the layer 23 in the node
{
	setForwardInputDimensionality( forwardInputDim );		// calls function to set the dimensionality of the forward input vector of the node
	setForwardOutputDimensionality( forwardOutputDim );		// calls function to set the dimensionality of the forward output vector of the node
	setBackwardInputDimensionality( backwardInputDim );		// calls function to set the dimensionality of the backward input vector of the node
	setBackwardOutputDimensionality( backwardOutputDim );		// calls function to set the dimensionality of the backward output vector of the node
	setExtraBackwardOutputDimensionality( backwardOutputDim );	// calls function to set the dimensionality of the extra backward output vector of the node
	setInputSynchronization(false);					// sets the input synchronization of the node
	setOutputSynchronization(false);				// sets the output synchronization of the node
	validateObject();						// function that validates the object
	reservForwardInputVector();					// reserves space for the forward input vector
	reservForwardOutputVector();					// reserves space for the forward output vector
	reservBackwardInputVector();					// reserves space for the backward input vector
	reservBackwardOutputVector();					// reserves space for the backward output vector
	reservExtraBackwardOutputVector();				// reserves space for the extra backward output vector
} // end CorticalNodeAbsBase constructor


// function to validate the object created of this class
void	CorticalNodeAbsBase::validateObject()
{
	int forwardInputDim, forwardOutputDim, backwardInputDim, backwardOutputDim, extraBackwardOutputDim;
	int l4InputDim, l4OutputDim;
	int l5InputDim, l5OutputDim;
	int l6InputDim, l6ExtraInputDim, l6OutputDim;
	int l23ForwardInputDim, l23ForwardOutputDim, l23BackwardInputDim, l23BackwardOutputDim;

	forwardInputDim = CorticalNodeAbsBase::getForwardInputDimensionality();
	forwardOutputDim = CorticalNodeAbsBase::getForwardOutputDimensionality();
	backwardInputDim = CorticalNodeAbsBase::getBackwardInputDimensionality();
	backwardOutputDim = CorticalNodeAbsBase::getBackwardOutputDimensionality();
	extraBackwardOutputDim = CorticalNodeAbsBase::getExtraBackwardOutputDimensionality();

	if ( forwardInputDim <= 0 || forwardOutputDim <= 0 || backwardInputDim <= 0 || backwardOutputDim <= 0 || extraBackwardOutputDim <= 0 )
	{
		cout << "CorticalNodeAbsBase object inconsistence: forwardInputDim = " << forwardInputDim << "\n" << endl;
		cout << "CorticalNodeAbsBase object inconsistence: forwardOutputDim = " << forwardOutputDim << "\n" << endl;
		cout << "CorticalNodeAbsBase object inconsistence: backwardInputDim = " << backwardInputDim << "\n" << endl;
		cout << "CorticalNodeAbsBase object inconsistence: backwardOutputDim = " << backwardOutputDim << "\n" << endl;
		cout << "CorticalNodeAbsBase object inconsistence: extraBackwardOutputDim = " << extraBackwardOutputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	l4InputDim = layer4.getInputDimensionality();
	l4OutputDim = layer4.getOutputDimensionality();

	l5InputDim = layer5.getInputDimensionality();
	l5OutputDim = layer5.getOutputDimensionality();

	l6InputDim = layer6.getInputDimensionality();
	l6ExtraInputDim = layer6.getL5InputDimensionality();
	l6OutputDim = layer6.getOutputDimensionality();

	l23ForwardInputDim = layer23.getInputDimensionality();
	l23ForwardOutputDim = layer23.getOutputDimensionality();
	l23BackwardInputDim = layer23.getBackwardInputDimensionality();
	l23BackwardOutputDim = layer23.getBackwardOutputDimensionality();

	if ( forwardInputDim != l4InputDim )
	{
		cout << "CorticalNodeAbsBase object inconsistence: forwardInputDim = " << forwardInputDim << " is different to" << endl;
		cout << "l4InputDim = " << l4InputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( forwardOutputDim != l23ForwardOutputDim )
	{
		cout << "CorticalNodeAbsBase object inconsistence: forwardOutputDim = " << forwardOutputDim << " is different to" << endl;
		cout << "l23ForwardOutputDim = " << l23ForwardOutputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( backwardInputDim != l23BackwardInputDim )
	{
		cout << "CorticalNodeAbsBase object inconsistence: backwardInputDim = " << backwardInputDim << " is different to" << endl;
		cout << "l23BackwardInputDim = " << l23BackwardInputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( backwardOutputDim != l6OutputDim )
	{
		cout << "CorticalNodeAbsBase object inconsistence: backwardOutputDim = " << backwardOutputDim << " is different to" << endl;
		cout << "l6OutputDim = " << l6OutputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( extraBackwardOutputDim != l5OutputDim )
	{
		cout << "CorticalNodeAbsBase object inconsistence: extraBackwardOutputDim = " << extraBackwardOutputDim << " is different to" << endl;
		cout << "l5OutputDim = " << l5OutputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( forwardInputDim != l6InputDim )
	{
		cout << "CorticalNodeAbsBase object inconsistence: forwardInputDim = " << forwardInputDim << " is different to" << endl;
		cout << "l6InputDim = " << l6InputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( l4OutputDim != l23ForwardInputDim )
	{
		cout << "CorticalNodeAbsBase object inconsistence: l4OutputDim = " << l4OutputDim << " is different to" << endl;
		cout << "l23ForwardInputDim = " << l23ForwardInputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( l23BackwardOutputDim != l5InputDim )
	{
		cout << "CorticalNodeAbsBase object inconsistence: l23BackwardOutputDim = " << l23BackwardOutputDim << " is different to" << endl;
		cout << "l5InputDim = " << l5InputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( l5OutputDim != l6ExtraInputDim )
	{
		cout << "CorticalNodeAbsBase object inconsistence: l5OutputDim = " << l5OutputDim << " is different to" << endl;
		cout << "l6ExtraInputDim = " << l6ExtraInputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// this function propagates the signals through the cortical node and obtains an output
void	CorticalNodeAbsBase::propagateSignals( neurotransmitters l4, neurotransmitters l5, neurotransmitters l6, neurotransmitters l23ForwardGroup, neurotransmitters l23BackwardGroup )
{
	bool	sinc;
	sinc = CorticalNodeAbsBase::getInputSynchronization();		// gets the input synchronization of the node
	if ( sinc )
	{
		CorticalNodeAbsBase::putOnForwardInput();			// puts the forward input of the node on the input of the layer 4 and on the forward input of layer 6
		layer4.propagateSignals(l4);				// propagates the signals through layer 4

		CorticalNodeAbsBase::layer4ToLayer23();			// puts the output of layer 4 on the forward input of layer 2/3
		//CorticalNodeAbsBase::putOnBackwardInput();			// puts the backward input of the node on the backward input of layer 2/3
		layer23.propagateSignals(l23ForwardGroup, l23BackwardGroup);	// propagates the signals through layer 2/3

		//CorticalNodeAbsBase::layer23ToLayer5();			// puts the backward output of layer 2/3 on the input of layer 5
		//layer5.propagateSignals(l5);				// propagates the signals through layer 5

		//CorticalNodeAbsBase::layer5ToLayer6();			// puts the output of layer 5 on the backward input of layer 6
		//layer6.propagateSignals(l6);				// propagates the signals through layer 6

		CorticalNodeAbsBase::putOnForwardOutput();			// puts the forward output of layer 2/3 on the forward output of the node
		//CorticalNodeAbsBase::putOnBackrwardOutput();		// puts the output of layer 6 on the backward output of the node
		//CorticalNodeAbsBase::putOnExtraBackrwardOutput();		// puts the output of layer 5 on the extra backward output of the node
	}
	else
	{
		CorticalNodeAbsBase::setOutputSynchronization(false);	// sets the output synchronization of the node
	}
} // end function propagateSignals


// this function puts the forward input of the node on the input of the layer 4 and on the forward input of layer 6
void	CorticalNodeAbsBase::putOnForwardInput()
{
	double	*inputPointer;

	inputPointer = CorticalNodeAbsBase::getForwardInputVector();	// gets the pointer to the forward input vector of the node
	layer4.setInputVector(inputPointer);				// puts the content to which points inputPointer on the input vector of layer 4
	layer6.setInputVector(inputPointer);				// puts the content to which points inputPointer on the first input vector of layer 6
	free(inputPointer);						// frees the space used by the vector to which inputPointer points
} // end function putOnForwardInput


// this function puts the forward output of the layer 2/3 on the forward output of the node
void	CorticalNodeAbsBase::putOnForwardOutput()
{
	bool	sinc;
	double	*outputVector;

	sinc = layer23.getOutputSynchronization();			// gets the output synchronization of the layer 23
	CorticalNodeAbsBase::setOutputSynchronization(sinc);		// sets the output synchronization of the node

	outputVector = layer23.getOutputVector();			// gets the pointer to the forward output vector of layer 2/3
	CorticalNodeAbsBase::setForwardOutputVector(outputVector);		// puts the content to which points outputVector on the forward output of the node
	free(outputVector);						// frees the space used by the vector to which outputVector points
} // end function putOnForwardOutput


// this function puts the backward input vector of the node on the backward input of the layer 2/3
void	CorticalNodeAbsBase::putOnBackwardInput()
{
	double *inputVector;

	inputVector = CorticalNodeAbsBase::getBackwardInputVector();	// gets the pointer to the backward input vector of the node
	layer23.setBackwardInputVector(inputVector);			// puts the content to which points inputVector on the backward input vector of layer 2/3
	free(inputVector);						// frees the space used by the vector to which inputVector points
} // end function putOnBackwardInput


// this function puts the output of layer 6 on the backward output of the node
void	CorticalNodeAbsBase::putOnBackrwardOutput()
{
	double	*outputVector;

	outputVector = layer6.getOutputVector();			// gets the pointer to the output vector of layer 6
	CorticalNodeAbsBase::setBackwardOutputVector(outputVector);		// puts the content to which points outputVector on the backward output vector of the node
	free(outputVector);						// frees the space used by the vector to which outputVector points	
} // end function putOnBackrwardOutput


// this function puts the output of layer 5 on the extra backward output of the node
void	CorticalNodeAbsBase::putOnExtraBackrwardOutput()
{
	double	*outputVector;

	outputVector = layer5.getOutputVector();			// gets the pointer to the output vector of layer 5
	CorticalNodeAbsBase::setExtraBackwardOutputVector(outputVector);	// puts the content to which points outputVector on the extra backward output vector of the node
	free(outputVector);						// frees the space used by the vector to which outputVector points
} // end function putOnExtraBackrwardOutput


// this function puts the output of layer 4 on the forward input of layer 2/3
void	CorticalNodeAbsBase::layer4ToLayer23()
{
	double	*vector;

	vector = layer4.getOutputVector();				// gets the pointer to the output vector of layer 4
	layer23.setInputVector(vector);					// puts the content to which points vector on the forward input vector of layer 2/3
	free(vector);							// frees the space used by the vector to which vector points
} // end function layer4ToLayer23


// this function puts the backward output of layer 2/3 on the input of layer 5
void	CorticalNodeAbsBase::layer23ToLayer5()
{
	double	*vector;

	vector = layer23.getBackwardOutputVector();			// gets the pointer to the backward output vector of layer 2/3
	layer5.setInputVector(vector);					// puts the content to which points vector on the input vector of layer 5
	free(vector);							// frees the space used by the vector to which vector points
} // end function layer23ToLayer5


// this function puts the output of layer 5 on the backward input of layer 6
void	CorticalNodeAbsBase::layer5ToLayer6()
{
	double	*vector;

	vector = layer5.getOutputVector();				// gets the pointer to the output vector of layer 5
	layer6.setL5InputVector(vector);				// puts the content to which points vector on the extra input vector of layer 6
	free(vector);							// frees the space used by the vector to which vector points
} // end function layer5ToLayer6


// function to set the dimensionality of the forward input vector of the node
void	CorticalNodeAbsBase::setForwardInputDimensionality( int dimension )
{
	forwardInputDimensionality = dimension;			// stores the dimensionality of the forward input vector
} // end function setForwardInputDimensionality


// function to get the dimensionality of the forkward input vector of the node
int	CorticalNodeAbsBase::getForwardInputDimensionality()
{
	return forwardInputDimensionality;			// returns the dimensionality of the forward input vector
} // end function getForwardInputDimensionality


// function to set the dimensionality of the forward output vector of the node
void	CorticalNodeAbsBase::setForwardOutputDimensionality( int dimension )
{
	forwardOutputDimensionality = dimension;			// stores the dimensionality of the forward output vector
} // end function setForwardOutputDimensionality


// function to get the dimensionality of the forkward output vector of the node
int	CorticalNodeAbsBase::getForwardOutputDimensionality()
{
	return forwardOutputDimensionality;			// returns the dimensionality of the forward output vector
} // end function getForwardOutputDimensionality


// function to reserve space for the forward inputs vector of the node
void	CorticalNodeAbsBase::reservForwardInputVector()
{
	int numberOfInputs;

	numberOfInputs = CorticalNodeAbsBase::getForwardInputDimensionality();	// gets the number of forward inputs to the node

	forwardInputVector = (double*)calloc(numberOfInputs, sizeof(double));	// reserves space for the forwardInputVector vector
} // end function reservForwardInputVector


// function to set the forward input vector of the node
void	CorticalNodeAbsBase::setForwardInputVector( double *inputs )
{
	int j, numberOfInputs;

	numberOfInputs = CorticalNodeAbsBase::getForwardInputDimensionality();	// gets the number of forward inputs in the node

	for (j = 0; j < numberOfInputs; j++)
		*(forwardInputVector + j) = *(inputs + j);			// stores the inputs in the forwardInputVector vector
} // end function setForwardInputVector


// function to get the forward input vector in the node
double	*CorticalNodeAbsBase::getForwardInputVector()
{
	int j, numberOfInputs;
	double *vector;

	numberOfInputs = CorticalNodeAbsBase::getForwardInputDimensionality();	// gets the number of forward inputs in the node

	vector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for vector

	for (j = 0; j < numberOfInputs; j++)
		*(vector + j) = *(forwardInputVector + j);			// stores the forwardInputVector in vector

	return vector;								// returns vector pointer
} // end function getForwardInputVector


// function to reserve space for the forward outputs vector of the node
void	CorticalNodeAbsBase::reservForwardOutputVector()
{
	int numberOfOutputs;

	numberOfOutputs = CorticalNodeAbsBase::getForwardOutputDimensionality();	// gets the number of forward outputs to the node

	forwardOutputVector = (double*)calloc(numberOfOutputs, sizeof(double));	// reserves space for the forwardOutputVector vector
} // end function reservForwardOutputVector


// function to set the forward output vector of the node
void	CorticalNodeAbsBase::setForwardOutputVector( double *inputs )
{
	int j, numberOfOutputs;

	numberOfOutputs = CorticalNodeAbsBase::getForwardOutputDimensionality();	// gets the number of forward outputs in the node

	for (j = 0; j < numberOfOutputs; j++)
		*(forwardOutputVector + j) = *(inputs + j);			// stores the inputs in the forwardOutputVector vector
} // end function setForwardOutputVector


// function to get the forward output vector in the node
double	*CorticalNodeAbsBase::getForwardOutputVector()
{
	int j, numberOfOutputs;
	double *vector;

	numberOfOutputs = CorticalNodeAbsBase::getForwardOutputDimensionality();	// gets the number of forward outputs in the node

	vector = (double*)calloc(numberOfOutputs, sizeof(double));		// reserves space for vector

	for (j = 0; j < numberOfOutputs; j++)
		*(vector + j) = *(forwardOutputVector + j);			// stores the forwardOutputVector in vector

	return vector;								// returns vector pointer
} // end function getForwardOutputVector


// function to set the dimensionality of the backward input vector of the node
void	CorticalNodeAbsBase::setBackwardInputDimensionality( int dimension )
{
	backwardInputDimensionality = dimension;			// stores the dimensionality of the backward input vector
} // end function setBackwardInputDimensionality


// function to get the dimensionality of the backkward input vector of the node
int	CorticalNodeAbsBase::getBackwardInputDimensionality()
{
	return backwardInputDimensionality;				// returns the dimensionality of the backward input vector
} // end function getBackwardInputDimensionality


// function to set the dimensionality of the backward output vector of the node
void	CorticalNodeAbsBase::setBackwardOutputDimensionality( int dimension )
{
	backwardOutputDimensionality = dimension;			// stores the dimensionality of the backward output vector
} // end function setBackwardOutputDimensionality


// function to get the dimensionality of the backward output vector of the node
int	CorticalNodeAbsBase::getBackwardOutputDimensionality()
{
	return backwardOutputDimensionality;			// returns the dimensionality of the backward output vector
} // end function getBackwardOutputDimensionality


// function to reserve space for the backward inputs vector of the node
void	CorticalNodeAbsBase::reservBackwardInputVector()
{
	int numberOfInputs;

	numberOfInputs = CorticalNodeAbsBase::getBackwardInputDimensionality();	// gets the number of backward inputs to the node

	backwardInputVector = (double*)calloc(numberOfInputs, sizeof(double));	// reserves space for the backwardInputVector vector
} // end function reservBackwardInputVector


// function to set the backward input vector of the node
void	CorticalNodeAbsBase::setBackwardInputVector( double *inputs )
{
	int j, numberOfInputs;

	numberOfInputs = CorticalNodeAbsBase::getBackwardInputDimensionality();	// gets the number of backward inputs in the node

	for (j = 0; j < numberOfInputs; j++)
		*(backwardInputVector + j) = *(inputs + j);			// stores the inputs in the backwardInputVector vector
} // end function setBackwardInputVector


// function to get the backward input vector in the node
double	*CorticalNodeAbsBase::getBackwardInputVector()
{
	int j, numberOfInputs;
	double *vector;

	numberOfInputs = CorticalNodeAbsBase::getBackwardInputDimensionality();	// gets the number of backward inputs in the node

	vector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for vector

	for (j = 0; j < numberOfInputs; j++)
		*(vector + j) = *(backwardInputVector + j);			// stores the backwardInputVector in vector

	return vector;								// returns vector pointer
} // end function getBackwardInputVector


// function to reserve space for the backward outputs vector of the node
void	CorticalNodeAbsBase::reservBackwardOutputVector()
{
	int numberOfOutputs;

	numberOfOutputs = CorticalNodeAbsBase::getBackwardOutputDimensionality();	// gets the number of backward outputs to the node

	backwardOutputVector = (double*)calloc(numberOfOutputs, sizeof(double));// reserves space for the backwardOutputVector vector
} // end function reservBackwardOutputVector


// function to set the backward output vector of the node
void	CorticalNodeAbsBase::setBackwardOutputVector( double *inputs )
{
	int j, numberOfOutputs;

	numberOfOutputs = CorticalNodeAbsBase::getBackwardOutputDimensionality();	// gets the number of backward outputs in the node

	for (j = 0; j < numberOfOutputs; j++)
		*(backwardOutputVector + j) = *(inputs + j);			// stores the inputs in the backwardOutputVector vector
} // end function setBackwardOutputVector


// function to get the backward output vector in the node
double	*CorticalNodeAbsBase::getBackwardOutputVector()
{
	int j, numberOfOutputs;
	double *vector;

	numberOfOutputs = CorticalNodeAbsBase::getBackwardOutputDimensionality();	// gets the number of backward outputs in the node

	vector = (double*)calloc(numberOfOutputs, sizeof(double));		// reserves space for vector

	for (j = 0; j < numberOfOutputs; j++)
		*(vector + j) = *(backwardOutputVector + j);			// stores the backwardOutputVector in vector

	return vector;								// returns vector pointer
} // end function getBackwardOutputVector


// function to set the dimensionality of the extra backward output vector of the node
void	CorticalNodeAbsBase::setExtraBackwardOutputDimensionality( int dimension )
{
	extraBackwardOutputDimensionality = dimension;		// stores the dimensionality of the extra backward output vector
} // end function setExtraBackwardOutputDimensionality


// function to get the dimensionality of the extra backward output vector of the node
int	CorticalNodeAbsBase::getExtraBackwardOutputDimensionality()
{
	return extraBackwardOutputDimensionality;		// returns the dimensionality of the extra backward output vector
} // end function getExtraBackwardOutputDimensionality


// function to reserve space for the extra backward outputs vector of the node
void	CorticalNodeAbsBase::reservExtraBackwardOutputVector()
{
	int numberOfOutputs;

	numberOfOutputs = CorticalNodeAbsBase::getExtraBackwardOutputDimensionality();	// gets the number of extra backward outputs to the node

	extraBackwardOutputVector = (double*)calloc(numberOfOutputs, sizeof(double));	// reserves space for the extraBackwardOutputVector vector
} // end function reservBackwardOutputVector


// function to set the extra backward output vector of the node
void	CorticalNodeAbsBase::setExtraBackwardOutputVector( double *inputs )
{
	int j, numberOfOutputs;

	numberOfOutputs = CorticalNodeAbsBase::getExtraBackwardOutputDimensionality();	// gets the number of backward outputs in the node

	for (j = 0; j < numberOfOutputs; j++)
		*(extraBackwardOutputVector + j) = *(inputs + j);			// stores the inputs in the extraBackwardOutputVector vector
} // end function setExtraBackwardOutputVector


// function to get the extra backward output vector in the node
double	*CorticalNodeAbsBase::getExtraBackwardOutputVector()
{
	int j, numberOfOutputs;
	double *vector;

	numberOfOutputs = CorticalNodeAbsBase::getExtraBackwardOutputDimensionality();	// gets the number of backward outputs in the node

	vector = (double*)calloc(numberOfOutputs, sizeof(double));			// reserves space for vector

	for (j = 0; j < numberOfOutputs; j++)
		*(vector + j) = *(extraBackwardOutputVector + j);			// stores the extraBackwardOutputVector in vector

	return vector;									// returns vector pointer
} // end function getExtraBackwardOutputVector


// function to set the input synchronization of the node
void	CorticalNodeAbsBase::setInputSynchronization( bool sinc )
{
	inputSynchronization = sinc;
} // end function setInputSynchronization


// function to get the input synchronization of the node
bool	CorticalNodeAbsBase::getInputSynchronization()
{
	return inputSynchronization;
} // end function getInputSynchronization


// function to set the output synchronization of the node
void	CorticalNodeAbsBase::setOutputSynchronization( bool sinc )
{
	outputSynchronization = sinc;
} // end function setOutputSynchronization


// function to get the output synchronization of the node
bool	CorticalNodeAbsBase::getOutputSynchronization()
{
	return outputSynchronization;
} // end function getOutputSynchronization


// function to launch a thread that calls the method CorticalNodeAbsBase::propagateSignals()
std::thread CorticalNodeAbsBase::propagateSignalsLauncher( neurotransmitters parameter1, neurotransmitters parameter2, neurotransmitters parameter3, neurotransmitters parameter4, neurotransmitters parameter5 )
{
	return std::thread(&CorticalNodeAbsBase::propagateSignals, this, parameter1, parameter2, parameter3, parameter4, parameter5);
} // end function propagateSignalsLauncher




















