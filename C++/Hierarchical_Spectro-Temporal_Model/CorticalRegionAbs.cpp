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

// File Name:		CorticalRegionAbs.cpp
// File Description:	CorticalRegionAbs member-function definitions. This file contains implementations of the
//			member functions prototyped in CorticalRegionAbs.h.

#include <iostream>
#include <stdlib.h>
#include "CorticalRegionAbs.h"						// include definition of class CorticalRegionAbs
#include "CorticalNodeAbs.h"						// CorticalNodeAbs class declaration

using namespace std;

// constructor initializes the dimensionality of the input, the dimensionality of the output and the layers of nodes in the cortical region
CorticalRegionAbs::CorticalRegionAbs( int inputDimensions, int outputDimensions, CorticalNodeAbsBase &h_1_1, CorticalNodeAbsBase &h_1_2, CorticalNodeAbsBase &h_1_3, CorticalNodeAbsBase &h_1_4, CorticalNodeAbs &h_2_1, CorticalNodeAbs &h_2_2, CorticalNodeAbs &h_3_1 )
	// initializes first hierarchical layer of CorticalNodeAbs objects
	: node_H1_1( h_1_1 ),
	  node_H1_2( h_1_2 ),
	  node_H1_3( h_1_3 ),
	  node_H1_4( h_1_4 ),

	// initializes second hierarchical layer of CorticalNodeAbs objects
	  node_H2_1( h_2_1 ),
	  node_H2_2( h_2_2 ),

	// initializes third hierarchical layer of CorticalNodeAbs objects
	  node_H3_1( h_3_1 ),

	// explicitly calls CorticalRegion base-class constructor
	  CorticalRegion( inputDimensions, outputDimensions )
{
	setSynchronizationVariable( 1, 1, false );			// calls function that sets the synchronization variable of node number 1 in hierarchy number 1 of the region to false
	setSynchronizationVariable( 1, 2, false );			// calls function that sets the synchronization variable of node number 2 in hierarchy number 1 of the region to false
	setSynchronizationVariable( 1, 3, false );			// calls function that sets the synchronization variable of node number 3 in hierarchy number 1 of the region to false
	setSynchronizationVariable( 1, 4, false );			// calls function that sets the synchronization variable of node number 4 in hierarchy number 1 of the region to false

	setSynchronizationVariable( 2, 1, false );			// calls function that sets the synchronization variable of node number 1 in hierarchy number 2 of the region to false
	setSynchronizationVariable( 2, 2, false );			// calls function that sets the synchronization variable of node number 2 in hierarchy number 2 of the region to false

	setSynchronizationVariable( 3, 1, false );			// calls function that sets the synchronization variable of node number 1 in hierarchy number 3 of the region to false

	validateObject();						// calls function to validate the object
} // end CorticalRegionAbs constructor


// function to validate the object created of this class
void	CorticalRegionAbs::validateObject()
{
	int inputDim, outputDim;

	inputDim = CorticalRegion::getInputDimensionality();
	outputDim = CorticalRegion::getOutputDimensionality();

	if ( inputDim != 28 )
	{
		cout << "CorticalRegion object inconsistence: inputDimensionality = " << inputDim << "\n" << endl;
		cout << "inputDimensionality should be equal to 28 \n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( outputDim != 8 )
	{
		cout << "CorticalRegion object inconsistence: outputDimensionality = " << outputDim << "\n" << endl;
		cout << "outputDimensionality should be equal to 8 \n" << endl;
		exit( EXIT_FAILURE );
	}

	CorticalRegionAbs::validateInputToFirstLayer();			// validates the consistency between the input and the first hierarchical layer
	CorticalRegionAbs::validateFirstToSecondLayer();		// validates the consistency between the first and the second hierarchical layers
	CorticalRegionAbs::validateSecondToThirdLayer();		// validates the consistency between the second and the third hierarchical layers
	CorticalRegionAbs::validateThirdLayerToOutput();		// validates the consistency between the third layer and the output of the cortical region
} // end function validateObject


// function to validate the consistency between the input and the first hierarchical layer
void	CorticalRegionAbs::validateInputToFirstLayer()
{
	int	i, inputDim;

	inputDim = node_H1_1.getForwardInputDimensionality();
	if ( inputDim != 11 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 1 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 11.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_2.getForwardInputDimensionality();
	if ( inputDim != 11 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 2 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 11.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_3.getForwardInputDimensionality();
	if ( inputDim != 11 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 3 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 11.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_4.getForwardInputDimensionality();
	if ( inputDim != 11 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 4 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 11.\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateInputToFirstLayer


// function to validate the consistency between the first and the second hierarchical layer
void	CorticalRegionAbs::validateFirstToSecondLayer()
{
	int	i, inputDim, outputDim;

	// this is for the forward path
	outputDim = node_H1_1.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_2.getForwardOutputDimensionality();
	inputDim = node_H2_1.getForwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First to second hierarchical layer inconsistency in the forward path.\n" << endl;
		cout << "Nodes number " << 1 << " and " << 2 << " in the first layer of the hierarchy form a forward output vector of " << outputDim << " dimensions,\n" << endl;
		cout << "while node number " << 1 << " in the second layer of the hierarchy has a forward input vector with a dimensionality of " << inputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}

	outputDim = node_H1_3.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_4.getForwardOutputDimensionality();
	inputDim = node_H2_2.getForwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First to second hierarchical layer inconsistency in the forward path.\n" << endl;
		cout << "Nodes number " << 3 << " and " << 4 << " in the first layer of the hierarchy form a forward output vector of " << outputDim << " dimensions,\n" << endl;
		cout << "while node number " << 2 << " in the second layer of the hierarchy has a forward input vector with a dimensionality of " << inputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}

/*	// this is for the backward path
	outputDim = node_H2_1.getBackwardOutputDimensionality();
	outputDim = outputDim + node_H1_1.getForwardOutputDimensionality();
	inputDim = node_H1_1.getBackwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First to second hierarchical layer inconsistency in the backward path.\n" << endl;
		cout << "Node number " << 1 << " in the first layer of the hierarchy has a backward input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while backward output of node number " << 1 << " in the second layer of the hierarchy\ntogether with forward output vector of node " << 1 << " in the first layer of the hierarchy has a dimensionality of " << outputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}

	outputDim = node_H2_1.getBackwardOutputDimensionality();
	outputDim = outputDim + node_H1_2.getForwardOutputDimensionality();
	inputDim = node_H1_2.getBackwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First to second hierarchical layer inconsistency in the backward path.\n" << endl;
		cout << "Node number " << 2 << " in the first layer of the hierarchy has a backward input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while backward output of node number " << 1 << " in the second layer of the hierarchy\ntogether with forward output vector of node " << 2 << " in the first layer of the hierarchy has a dimensionality of " << outputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}

	outputDim = node_H2_2.getBackwardOutputDimensionality();
	outputDim = outputDim + node_H1_3.getForwardOutputDimensionality();
	inputDim = node_H1_3.getBackwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First to second hierarchical layer inconsistency in the backward path.\n" << endl;
		cout << "Node number " << 3 << " in the first layer of the hierarchy has a backward input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while backward output of node number " << 2 << " in the second layer of the hierarchy\ntogether with forward output vector of node " << 3 << " in the first layer of the hierarchy has a dimensionality of " << outputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}

	outputDim = node_H2_2.getBackwardOutputDimensionality();
	outputDim = outputDim + node_H1_4.getForwardOutputDimensionality();
	inputDim = node_H1_4.getBackwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First to second hierarchical layer inconsistency in the backward path.\n" << endl;
		cout << "Node number " << 4 << " in the first layer of the hierarchy has a backward input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while backward output of node number " << 2 << " in the second layer of the hierarchy\ntogether with forward output vector of node " << 4 << " in the first layer of hte hierarchy has a dimensionality of " << outputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}*/
} // end function validateFirstToSecondLayer


// function to validate the consistency between the second and the third hierarchical layer
void	CorticalRegionAbs::validateSecondToThirdLayer()
{
	int	i, inputDim, outputDim;

	// this is for the forward path
	outputDim = node_H2_1.getForwardOutputDimensionality();
	outputDim = outputDim + node_H2_2.getForwardOutputDimensionality();
	inputDim = node_H3_1.getForwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "Second to third hierarchical layer inconsistency in the forward path.\n" << endl;
		cout << "Nodes number " << 1 << " and " << 2 << " in the second layer of the hierarchy form a forward output vector of " << outputDim << " dimensions,\n" << endl;
		cout << "while node number " << 1 << " in the third layer of the hierarchy has a forward input vector with a dimensionality of " << inputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}

/*	// this is for the backward path
	outputDim = node_H3_1.getBackwardOutputDimensionality();
	outputDim = outputDim + node_H2_1.getForwardOutputDimensionality();
	inputDim = node_H2_1.getBackwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "Second to third hierarchical layer inconsistency in the backward path.\n" << endl;
		cout << "Node number " << 1 << " in the second layer of the hierarchy has a backward input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while backward output of node number " << 1 << " in the third layer of the hierarchy\ntogether with forward output vector of node " << 1 << " in the second layer of the hierarchy has a dimensionality of " << outputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}

	outputDim = node_H3_1.getBackwardOutputDimensionality();
	outputDim = outputDim + node_H2_2.getForwardOutputDimensionality();
	inputDim = node_H2_2.getBackwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "Second to third hierarchical layer inconsistency in the backward path.\n" << endl;
		cout << "Node number " << 2 << " in the second layer of the hierarchy has a backward input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while backward output of node number " << 1 << " in the third layer of the hierarchy\ntogether with forward output vector of node " << 2 << " in the second layer of the hierarchy has a dimensionality of " << outputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}*/
} // end function validateSecondToThirdLayer


// function to validate the consistency between the third layer and the output of the cortical region
void	CorticalRegionAbs::validateThirdLayerToOutput()
{
	int	outputDim, outputDim1, inputDim;

	outputDim = CorticalRegion::getOutputDimensionality();
	outputDim1 = node_H3_1.getForwardOutputDimensionality();

	if ( outputDim != outputDim1 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "Third hierarchical layer to output inconsistency in the forward path.\n" << endl;
		cout << "Node number " << 1 << " in the third layer of the hierarchy forms a forward output vector of " << outputDim1 << " dimensions,\n" << endl;
		cout << "while the output of the region has a dimensionality of " << outputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}

/*	inputDim = node_H3_1.getBackwardInputDimensionality();

	if ( inputDim != outputDim1 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "Third hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 1 << " in the third layer of the hierarchy forms a forward output vector of " << outputDim1 << " dimensions,\n" << endl;
		cout << "while the backward input vector of such node has a dimensionality of " << inputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}*/
} // end function validateThirdLayerToOutput


// function to propagate the input signals from the inputVector through the nodes in the cortical region and put the results of this propagation in the cortical region's outputVector
void	CorticalRegionAbs::propagateSignals( neuromodulators nodeNeuromodulator[7] )
{
	int i;
	std::thread t[3];
	neuromodulators	firstLayerNeuromodulators[4], secondLayerNeuromodulators[2], thirdLayerNeuromodulators;

	CorticalRegionAbs::putOnInput();					// puts the inputs on the first hierarchical layer of nodes
	CorticalRegionAbs::firstToSecondConnection();				// makes the connections between the first and the second hierarchical layer
	CorticalRegionAbs::secondToThirdConnection();				// makes the connections between the second and the third hierarchical layer
	CorticalRegionAbs::putOnOutput();					// puts the outputs from the hierarchical layer three on the output of the region

	for (i = 0; i < 4; i++ )
	{
		firstLayerNeuromodulators[i] = nodeNeuromodulator[i];
	}
	// This creates a thread that makes the following call "CorticalRegionAbs::propagateFirstLayer(firstLayerNeuromodulators);" which propagates the signals through the firdt hierarchical layer of nodes
	t[0] = CorticalRegionAbs::propagateFirstLayerLauncher(firstLayerNeuromodulators);

	//CorticalRegionAbs::propagateFirstLayer(firstLayerNeuromodulators);	// propagates the signals through the first hierarchical layer of nodes

	for (i = 0; i < 2; i++ )
	{
		secondLayerNeuromodulators[i] = nodeNeuromodulator[i+4];
	}
	// This creates a thread that makes the following call "CorticalRegionAbs::propagateSecondLayer(secondLayerNeuromodulators);" which propagates the signals through the second hierarchical layer of nodes
	//t[1] = CorticalRegionAbs::propagateSecondLayerLauncher(secondLayerNeuromodulators);

	CorticalRegionAbs::propagateSecondLayer(secondLayerNeuromodulators);	// propagates the signals through the second hierarchical layer of nodes

	thirdLayerNeuromodulators = nodeNeuromodulator[6];
	// This creates a thread that makes the following call "CorticalRegionAbs::propagateThirdLayer(thirdLayerNeuromodulators);" which propagates the signals through the third hierarchical layer of nodes
	//t[2] = CorticalRegionAbs::propagateThirdLayerLauncher(thirdLayerNeuromodulators);

	CorticalRegionAbs::propagateThirdLayer(thirdLayerNeuromodulators);	// propagates the signals through the third hierarchical layer of nodes

	t[0].join();
	//t[1].join();
	//t[2].join();
} // end function propagateSignals


// function to propagate the signals through the first hierarchical layer of nodes
void	CorticalRegionAbs::propagateFirstLayer( neuromodulators first[4] )
{
	std::thread t[4];

	//t[0] = node_H1_1.propagateSignalsLauncher(first[0].layer4, first[0].layer5, first[0].layer6, first[0].layer23FG, first[0].layer23BG);
	node_H1_1.propagateSignals(first[0].layer4, first[0].layer5, first[0].layer6, first[0].layer23FG, first[0].layer23BG);

	//t[1] = node_H1_2.propagateSignalsLauncher(first[1].layer4, first[1].layer5, first[1].layer6, first[1].layer23FG, first[1].layer23BG);
	node_H1_2.propagateSignals(first[1].layer4, first[1].layer5, first[1].layer6, first[1].layer23FG, first[1].layer23BG);

	//t[2] = node_H1_3.propagateSignalsLauncher(first[2].layer4, first[2].layer5, first[2].layer6, first[2].layer23FG, first[2].layer23BG);
	node_H1_3.propagateSignals(first[2].layer4, first[2].layer5, first[2].layer6, first[2].layer23FG, first[2].layer23BG);

	//t[3] = node_H1_4.propagateSignalsLauncher(first[3].layer4, first[3].layer5, first[3].layer6, first[3].layer23FG, first[3].layer23BG);
	node_H1_4.propagateSignals(first[3].layer4, first[3].layer5, first[3].layer6, first[3].layer23FG, first[3].layer23BG);

	//t[0].join();
	//t[1].join();
	//t[2].join();
	//t[3].join();
} // end function propagateFirstLayer


// function to propagate the signals through the second hierarchical layer of nodes
void	CorticalRegionAbs::propagateSecondLayer( neuromodulators second[2] )
{
	std::thread t[2];

	//t[0] = node_H2_1.propagateSignalsLauncher(second[0].layer4, second[0].layer5, second[0].layer6, second[0].layer23FG, second[0].layer23BG);
	node_H2_1.propagateSignals(second[0].layer4, second[0].layer5, second[0].layer6, second[0].layer23FG, second[0].layer23BG);

	//t[1] = node_H2_2.propagateSignalsLauncher(second[1].layer4, second[1].layer5, second[1].layer6, second[1].layer23FG, second[1].layer23BG);
	node_H2_2.propagateSignals(second[1].layer4, second[1].layer5, second[1].layer6, second[1].layer23FG, second[1].layer23BG);

	//t[0].join();
	//t[1].join();
} // end function propagateSecondLayer


// function to propagate the signals through the third hierarchical layer of nodes
void	CorticalRegionAbs::propagateThirdLayer( neuromodulators third )
{
	std::thread t;

	//t = node_H3_1.propagateSignalsLauncher(third.layer4, third.layer5, third.layer6, third.layer23FG, third.layer23BG);
	node_H3_1.propagateSignals(third.layer4, third.layer5, third.layer6, third.layer23FG, third.layer23BG);
} // end function propagateThirdLayer


// function to put all the necessary data on every node input vector
void	CorticalRegionAbs::putOnInput()
{
	int	i, j;
	bool	sinc;
	double	*vector, *inputVector;

	sinc = CorticalRegion::getInputSynchronization();			// gets the input synchronization of the region

	inputVector = CorticalRegion::getInputVector();				// gets the input vector to the region

	vector = (double*)calloc(11, sizeof(double));				// reserves space for vector

	for (j = 0; j < 11; j++ )
	{
		*(vector + j) = *(inputVector + j);
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_1.setForwardInputVector(vector);				// sets forward input vector for the 1-st node in the first hierarchical layer
	node_H1_1.setInputSynchronization(sinc);				// sets the input synchronizaion for the 1-st node in the first hierarchical layer

	for (j = 0; j < 11; j++ )
	{
		*(vector + j) = *((inputVector + j) + 5);
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_2.setForwardInputVector(vector);				// sets forward input vector for the 2-nd node in the first hierarchical layer
	node_H1_2.setInputSynchronization(sinc);				// sets the input synchronizaion for the 2-nd node in the first hierarchical layer

	for (j = 0; j < 11; j++ )
	{
		*(vector + j) = *((inputVector + j) + 12);
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_3.setForwardInputVector(vector);				// sets forward input vector for the 3-rd node in the first hierarchical layer
	node_H1_3.setInputSynchronization(sinc);				// sets the input synchronizaion for the 3-rd node in the first hierarchical layer

	for (j = 0; j < 11; j++ )
	{
		*(vector + j) = *((inputVector + j) + 17);
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_4.setForwardInputVector(vector);				// sets forward input vector for the 4-th node in the first hierarchical layer
	node_H1_4.setInputSynchronization(sinc);				// sets the input synchronizaion for the 4-th node in the first hierarchical layer

	free(vector);								// frees the memory used by vector
	free(inputVector);							// frees the memory used by inputVector
} // end function putOnInput


// function to make all the connections between nodes of the first to the second hierarchical layer
void	CorticalRegionAbs::firstToSecondConnection()
{
	int	i, j, number, number1, number2;
	bool	sinc, sinc1, sinc2, active;
	double	*inputVector, *outputVector;

	// this is for the forward information path

	active = true;
	number1 = node_H1_1.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 1 in the first hierarchical layer
	number2 = node_H1_2.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 2 in the first hierarchical layer
	number = number1 + number2;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H1_1.getForwardOutputVector();			// gets the forward output vector from node 1 in the first hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first half of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_2.getForwardOutputVector();			// gets the forward output vector from node 2 in the first hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second half of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	node_H2_1.setForwardInputVector(inputVector);				// sets the forward input vector of node 1 in the second hierarchical layer
	free(inputVector);
	inputVector = NULL;

	sinc1 = node_H1_1.getOutputSynchronization();				// gets the output synchronization from the 1-st node in the first hierarchical layer
	sinc2 = node_H1_2.getOutputSynchronization();				// gets the output synchronization from the 2-nd node in the first hierarchical layer

	if ( sinc1 )
		CorticalRegionAbs::setSynchronizationVariable(1, 1, true);

	if ( sinc2 )
		CorticalRegionAbs::setSynchronizationVariable(1, 2, true);

	sinc1 = CorticalRegionAbs::getSynchronizationVariable(1, 1);
	sinc2 = CorticalRegionAbs::getSynchronizationVariable(1, 2);

	sinc = sinc1 & sinc2;

	if ( sinc )
	{
		node_H2_1.setInputSynchronization(true);			// sets the input synchronization of the 1-st node in the second hierarchical layer
		if ( active )
		{
			CorticalRegionAbs::setSynchronizationVariable(1, 1, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 2, false);
		}
	}
	else
	{
		node_H2_1.setInputSynchronization(false);			// sets the input synchronization of the 1-st node in the second hierarchical layer
	}

	///////////////

	active = true;
	number1 = node_H1_3.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 3 in the first hierarchical layer
	number2 = node_H1_4.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 4 in the first hierarchical layer
	number = number1 + number2;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H1_3.getForwardOutputVector();			// gets the forward output vector from node 3 in the first hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first half of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_4.getForwardOutputVector();			// gets the forward output vector from node 4 in the first hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second half of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	node_H2_2.setForwardInputVector(inputVector);				// sets the forward input vector of node 2 in the second hierarchical layer
	free(inputVector);
	inputVector = NULL;

	sinc1 = node_H1_3.getOutputSynchronization();				// gets the output synchronization from the 3-rd node in the first hierarchical layer
	sinc2 = node_H1_4.getOutputSynchronization();				// gets the output synchronization from the 4-th node in the first hierarchical layer

	if ( sinc1 )
		CorticalRegionAbs::setSynchronizationVariable(1, 3, true);

	if ( sinc2 )
		CorticalRegionAbs::setSynchronizationVariable(1, 4, true);

	sinc1 = CorticalRegionAbs::getSynchronizationVariable(1, 3);
	sinc2 = CorticalRegionAbs::getSynchronizationVariable(1, 4);

	sinc = sinc1 & sinc2;

	if ( sinc )
	{
		node_H2_2.setInputSynchronization(true);			// sets the input synchronization of the 2-nd node in the second hierarchical layer
		if ( active )
		{
			CorticalRegionAbs::setSynchronizationVariable(1, 3, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 4, false);
		}
	}
	else
	{
		node_H2_2.setInputSynchronization(false);			// sets the input synchronization of the 2-nd node in the second hierarchical layer
	}

	// this is for the backward information path
/*	number1 = node_H2_1.getBackwardOutputDimensionality();			// gets the backward output dimensionality from node 1 in the second hierarchical layer
	number2 = node_H1_1.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 1 in the first hierarchical layer
	number = number1 + number2;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H2_1.getBackwardOutputVector();			// gets the backward output vector from node 1 in the second hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_1.getForwardOutputVector();			// gets the forward output vector from node 1 in the first hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	node_H1_1.setBackwardInputVector(inputVector);				// sets the backward input vector of node 1 in the first hierarchical layer
	free(inputVector);
	inputVector = NULL;

	///////////////

	number1 = node_H2_1.getBackwardOutputDimensionality();			// gets the backward output dimensionality from node 1 in the second hierarchical layer
	number2 = node_H1_2.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 2 in the first hierarchical layer
	number = number1 + number2;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H2_1.getBackwardOutputVector();			// gets the backward output vector from node 1 in the second hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_2.getForwardOutputVector();			// gets the forward output vector from node 2 in the first hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	node_H1_2.setBackwardInputVector(inputVector);				// sets the backward input vector of node 2 in the first hierarchical layer
	free(inputVector);
	inputVector = NULL;

	///////////////

	number1 = node_H2_2.getBackwardOutputDimensionality();			// gets the backward output dimensionality from node 2 in the second hierarchical layer
	number2 = node_H1_3.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 3 in the first hierarchical layer
	number = number1 + number2;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H2_2.getBackwardOutputVector();			// gets the backward output vector from node 2 in the second hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_3.getForwardOutputVector();			// gets the forward output vector from node 3 in the first hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	node_H1_3.setBackwardInputVector(inputVector);				// sets the backward input vector of node 3 in the first hierarchical layer
	free(inputVector);
	inputVector = NULL;

	///////////////

	number1 = node_H2_2.getBackwardOutputDimensionality();			// gets the backward output dimensionality from node 2 in the second hierarchical layer
	number2 = node_H1_4.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 4 in the first hierarchical layer
	number = number1 + number2;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H2_2.getBackwardOutputVector();			// gets the backward output vector from node 2 in the second hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_4.getForwardOutputVector();			// gets the forward output vector from node 4 in the first hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	node_H1_4.setBackwardInputVector(inputVector);				// sets the backward input vector of node 4 in the first hierarchical layer
	free(inputVector);
	inputVector = NULL;*/
} // end function firstToSecondConnection


// function to make all the connections between nodes of the second to the third hierarchical layer
void	CorticalRegionAbs::secondToThirdConnection()
{
	int	i, j, number, number1, number2;
	bool	sinc1, sinc2, sinc, active;
	double	*inputVector, *outputVector;

	// this is for the forward information path

	active = true;
	number1 = node_H2_1.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 1 in the second hierarchical layer
	number2 = node_H2_2.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 2 in the second hierarchical layer
	number = number1 + number2;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H2_1.getForwardOutputVector();			// gets the forward output vector from node 1 in the second hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first half of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H2_2.getForwardOutputVector();			// gets the forward output vector from node 2 in the second hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second half of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	node_H3_1.setForwardInputVector(inputVector);				// sets the forward input vector of node 1 in the third hierarchical layer
	free(inputVector);
	inputVector = NULL;

	sinc1 = node_H2_1.getOutputSynchronization();				// gets the output synchronization from the 1-st node in the second hierarchical layer
	sinc2 = node_H2_2.getOutputSynchronization();				// gets the output synchronization from the 2-nd node in the second hierarchical layer

	if ( sinc1 )
		CorticalRegionAbs::setSynchronizationVariable(2, 1, true);

	if ( sinc2 )
		CorticalRegionAbs::setSynchronizationVariable(2, 2, true);

	sinc1 = CorticalRegionAbs::getSynchronizationVariable(2, 1);
	sinc2 = CorticalRegionAbs::getSynchronizationVariable(2, 2);

	sinc = sinc1 & sinc2;

	if ( sinc )
	{
		node_H3_1.setInputSynchronization(true);			// sets the input synchronization of the 1-st node in the third hierarchical layer
		if ( active )
		{
			CorticalRegionAbs::setSynchronizationVariable(2, 1, false);
			CorticalRegionAbs::setSynchronizationVariable(2, 2, false);
		}
	}
	else
	{
		node_H3_1.setInputSynchronization(false);			// sets the input synchronization of the 1-st node in the third hierarchical layer
	}

	// this is for the backward information path
/*	number1 = node_H3_1.getBackwardOutputDimensionality();			// gets the backward output dimensionality from node 1 in the third hierarchical layer
	number2 = node_H2_1.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 1 in the second hierarchical layer
	number = number1 + number2;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H3_1.getBackwardOutputVector();			// gets the backward output vector from node 1 in the third hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H2_1.getForwardOutputVector();			// gets the forward output vector from node 1 in the second hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	node_H2_1.setBackwardInputVector(inputVector);				// sets the backward input vector of node 1 in the second hierarchical layer
	free(inputVector);
	inputVector = NULL;

	///////////////

	number1 = node_H3_1.getBackwardOutputDimensionality();			// gets the backward output dimensionality from node 1 in the third hierarchical layer
	number2 = node_H2_2.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 2 in the second hierarchical layer
	number = number1 + number2;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H3_1.getBackwardOutputVector();			// gets the backward output vector from node 1 in the third hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H2_2.getForwardOutputVector();			// gets the forward output vector from node 2 in the second hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second half of inputVector
	}
	free(outputVector);
	outputVector = NULL;

	node_H2_2.setBackwardInputVector(inputVector);				// sets the backward input vector of node 2 in the second hierarchical layer
	free(inputVector);
	inputVector = NULL;*/
} // end function secondToThirdConnection


// function to put all the necessary data of every node output vector -in the third hierarchical layer- on the output of the cortical region
void	CorticalRegionAbs::putOnOutput()
{
	bool	sinc;
	double	*outputVector;

	outputVector = node_H3_1.getForwardOutputVector();				// gets the forward output vector from node 1 in the third hierarchical layer
	CorticalRegion::setOutputVector(outputVector);					// puts this vector on the output of the region
	free(outputVector);
	outputVector = NULL;
	sinc = node_H3_1.getOutputSynchronization();					// gets the output synchronization from the node 1 in the third hierarchical layer
	CorticalRegion::setOutputSynchronization(sinc);					// sets the synchronization output of the region
} // end function putOnOutput


// function to normalize the vector received as parameter
void	CorticalRegionAbs::normalizeVector( int dim, double *vector )
{
	double maximum;

	maximum = 0;
	for(int i = 0; i < dim; i++)
	{
		if ( *(vector+i) > maximum )
			maximum = *(vector+i);
	}

	if ( maximum != 0 )
	{
		for(int i = 0; i < dim; i++)
		{
			*(vector+i) = *(vector+i)/maximum;
		}
	}
} // end function normalizeVector


// function that sets the synchronization variables of the region
void	CorticalRegionAbs::setSynchronizationVariable( int hierarchy, int node, bool state )
{
	switch	( hierarchy )
	{
		case 1:
			switch	( node )
			{
				case 1:
					synchronization_1_1 = state;
				break;

				case 2:
					synchronization_1_2 = state;
				break;

				case 3:
					synchronization_1_3 = state;
				break;

				case 4:
					synchronization_1_4 = state;
				break;

				default:
					printf( "Incorrect option in setSynchronizationVariable. Incorrect node number %d in hierarchy number %d.\n", node, hierarchy );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		case 2:
			switch	( node )
			{
				case 1:
					synchronization_2_1 = state;
				break;

				case 2:
					synchronization_2_2 = state;
				break;

				default:
					printf( "Incorrect option in setSynchronizationVariable. Incorrect node number %d in hierarchy number %d.\n", node, hierarchy );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		case 3:
			switch	( node )
			{
				case 1:
					synchronization_3_1 = state;
				break;

				default:
					printf( "Incorrect option in setSynchronizationVariable. Incorrect node number %d in hierarchy number %d.\n", node, hierarchy );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		default:
			printf( "Incorrect option in setSynchronizationVariable. Incorrect hierarchy number %d.\n", hierarchy );
			exit( EXIT_FAILURE );
		break;
	}
} // end function setSynchronizationVariable


// function that gets the synchronization variables of the region
bool	CorticalRegionAbs::getSynchronizationVariable( int hierarchy, int node )
{
	switch	( hierarchy )
	{
		case 1:
			switch	( node )
			{
				case 1:
					return synchronization_1_1;
				break;

				case 2:
					return synchronization_1_2;
				break;

				case 3:
					return synchronization_1_3;
				break;

				case 4:
					return synchronization_1_4;
				break;

				default:
					printf( "Incorrect option in setSynchronizationVariable. Incorrect node number %d in hierarchy number %d.\n", node, hierarchy );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		case 2:
			switch	( node )
			{
				case 1:
					return synchronization_2_1;
				break;

				case 2:
					return synchronization_2_2;
				break;

				default:
					printf( "Incorrect option in setSynchronizationVariable. Incorrect node number %d in hierarchy number %d.\n", node, hierarchy );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		case 3:
			switch	( node )
			{
				case 1:
					return synchronization_3_1;
				break;

				default:
					printf( "Incorrect option in setSynchronizationVariable. Incorrect node number %d in hierarchy number %d.\n", node, hierarchy );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		default:
			printf( "Incorrect option in setSynchronizationVariable. Incorrect hierarchy number %d.\n", hierarchy );
			exit( EXIT_FAILURE );
		break;
	}
} // end function getSynchronizationVariable


// function to launch a thread that calls the method CorticalRegionAbs::propagateFirstLayer()
std::thread CorticalRegionAbs::propagateFirstLayerLauncher( neuromodulators *parameter )
{
	return std::thread(&CorticalRegionAbs::propagateFirstLayer, this, parameter);
} // end function propagateFirstLayerLauncher


// function to launch a thread that calls the method CorticalRegionAbs::propagateSecondLayer()
std::thread CorticalRegionAbs::propagateSecondLayerLauncher( neuromodulators *parameter )
{
	return std::thread(&CorticalRegionAbs::propagateSecondLayer, this, parameter);
} // end function propagateSecondLayerLauncher


// function to launch a thread that calls the method CorticalRegionAbs::propagateThirdLayer()
std::thread CorticalRegionAbs::propagateThirdLayerLauncher( neuromodulators parameter )
{
	return std::thread(&CorticalRegionAbs::propagateThirdLayer, this, parameter);
} // end function propagateThirdLayerLauncher
































