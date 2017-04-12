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
CorticalRegionAbs::CorticalRegionAbs( int inputDimensions, int outputDimensions,
				      CorticalNodeAbsBase &h_1_1, CorticalNodeAbsBase &h_1_2, CorticalNodeAbsBase &h_1_3, CorticalNodeAbsBase &h_1_4,
				      CorticalNodeAbsBase &h_1_5, CorticalNodeAbsBase &h_1_6, CorticalNodeAbsBase &h_1_7, CorticalNodeAbsBase &h_1_8,
				      CorticalNodeAbsBase &h_1_9, CorticalNodeAbsBase &h_1_10, CorticalNodeAbsBase &h_1_11, CorticalNodeAbsBase &h_1_12,
				      CorticalNodeAbsBase &h_1_13, CorticalNodeAbsBase &h_1_14, CorticalNodeAbsBase &h_1_15, CorticalNodeAbsBase &h_1_16,
				      CorticalNodeAbs &h_2_1, CorticalNodeAbs &h_2_2, CorticalNodeAbs &h_2_3, CorticalNodeAbs &h_2_4,
				      CorticalNodeAbs &h_3_1 )
	// initializes first hierarchical layer of CorticalNodeAbs objects
	: node_H1_11( h_1_1 ),
	  node_H1_12( h_1_2 ),
	  node_H1_13( h_1_3 ),
	  node_H1_14( h_1_4 ),
	  node_H1_15( h_1_5 ),
	  node_H1_16( h_1_6 ),
	  node_H1_17( h_1_7 ),
	  node_H1_18( h_1_8 ),

	  node_H1_21( h_1_9 ),
	  node_H1_22( h_1_10 ),
	  node_H1_23( h_1_11 ),
	  node_H1_24( h_1_12 ),
	  node_H1_25( h_1_13 ),
	  node_H1_26( h_1_14 ),
	  node_H1_27( h_1_15 ),
	  node_H1_28( h_1_16 ),

	// initializes second hierarchical layer of CorticalNodeAbs objects
	  node_H2_1( h_2_1 ),
	  node_H2_2( h_2_2 ),
	  node_H2_3( h_2_3 ),
	  node_H2_4( h_2_4 ),

	// initializes third hierarchical layer of CorticalNodeAbs objects
	  node_H3_1( h_3_1 ),

	// explicitly calls CorticalRegion base-class constructor
	  CorticalRegion( inputDimensions, outputDimensions )
{
	for ( int i = 1; i <= 16; i++ ) {
		setSynchronizationVariable( 1, i, false );		// calls function that sets the synchronization variable of node number 11 in hierarchy number 1 of the region to false
	}

	for ( int i = 1; i <= 4; i++ ) {
		setSynchronizationVariable( 2, i, false );		// calls function that sets the synchronization variable of node number 1 in hierarchy number 2 of the region to false
	}

	setSynchronizationVariable( 3, 1, false );			// calls function that sets the synchronization variable of node number 1 in hierarchy number 3 of the region to false

	validateObject();						// calls function to validate the object
} // end CorticalRegionAbs constructor


// function to validate the object created of this class
void	CorticalRegionAbs::validateObject()
{
	int inputDim, outputDim;

	inputDim = CorticalRegion::getInputDimensionality();
	outputDim = CorticalRegion::getOutputDimensionality();

	if ( inputDim != 5*128 )
	{
		cout << "CorticalRegion object inconsistence: inputDimensionality = " << inputDim << "\n" << endl;
		cout << "inputDimensionality should be equal to 640 (5*128) \n" << endl;
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
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 1 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_2.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 2 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_3.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 3 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_4.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 4 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}
	inputDim = node_H1_5.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 5 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_6.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 6 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_7.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 7 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_8.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 8 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}
	inputDim = node_H1_9.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 9 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_10.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 10 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_11.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 11 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_12.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 12 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}
	inputDim = node_H1_13.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 13 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_14.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 14 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_15.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 15 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
		exit( EXIT_FAILURE );
	}

	inputDim = node_H1_16.getForwardInputDimensionality();
	if ( inputDim != 60 )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First hierarchical layer inconsistency.\n" << endl;
		cout << "Node number " << 16 << " in the first layer of the hierarchy has an input vector of " << inputDim << " dimensions,\n" << endl;
		cout << "while it is espected to have a dimensionality of 60.\n" << endl;
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
	outputDim = outputDim + node_H1_3.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_4.getForwardOutputDimensionality();
	inputDim = node_H2_1.getForwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First to second hierarchical layer inconsistency in the forward path.\n" << endl;
		cout << "Nodes number 1, 2, 3 and 4 in the first layer of the hierarchy form a forward output vector of " << outputDim << " dimensions,\n" << endl;
		cout << "while node number 1 in the second layer of the hierarchy has a forward input vector with a dimensionality of " << inputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}

	outputDim = node_H1_5.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_6.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_7.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_8.getForwardOutputDimensionality();
	inputDim = node_H2_2.getForwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First to second hierarchical layer inconsistency in the forward path.\n" << endl;
		cout << "Nodes number 5, 6, 7 and 8 in the first layer of the hierarchy form a forward output vector of " << outputDim << " dimensions,\n" << endl;
		cout << "while node number 2 in the second layer of the hierarchy has a forward input vector with a dimensionality of " << inputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}

	outputDim = node_H1_9.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_10.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_11.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_12.getForwardOutputDimensionality();
	inputDim = node_H2_3.getForwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First to second hierarchical layer inconsistency in the forward path.\n" << endl;
		cout << "Nodes number 9, 10, 11 and 12 in the first layer of the hierarchy form a forward output vector of " << outputDim << " dimensions,\n" << endl;
		cout << "while node number 3 in the second layer of the hierarchy has a forward input vector with a dimensionality of " << inputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}

	outputDim = node_H1_13.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_14.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_15.getForwardOutputDimensionality();
	outputDim = outputDim + node_H1_16.getForwardOutputDimensionality();
	inputDim = node_H2_4.getForwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "First to second hierarchical layer inconsistency in the forward path.\n" << endl;
		cout << "Nodes number 13, 14, 15 and 16 in the first layer of the hierarchy form a forward output vector of " << outputDim << " dimensions,\n" << endl;
		cout << "while node number 4 in the second layer of the hierarchy has a forward input vector with a dimensionality of " << inputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateFirstToSecondLayer


// function to validate the consistency between the second and the third hierarchical layer
void	CorticalRegionAbs::validateSecondToThirdLayer()
{
	int	i, inputDim, outputDim;

	// this is for the forward path
	outputDim = node_H2_1.getForwardOutputDimensionality();
	outputDim = outputDim + node_H2_2.getForwardOutputDimensionality();
	outputDim = outputDim + node_H2_3.getForwardOutputDimensionality();
	outputDim = outputDim + node_H2_4.getForwardOutputDimensionality();
	inputDim = node_H3_1.getForwardInputDimensionality();

	if ( outputDim != inputDim )
	{
		cout << "CorticalRegion object inconsistence.\n" << endl;
		cout << "Second to third hierarchical layer inconsistency in the forward path.\n" << endl;
		cout << "Nodes number 1, 2, 3 and 4 in the second layer of the hierarchy form a forward output vector of " << outputDim << " dimensions,\n" << endl;
		cout << "while node number 1 in the third layer of the hierarchy has a forward input vector with a dimensionality of " << inputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}
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
		cout << "Node number 1 in the third layer of the hierarchy forms a forward output vector of " << outputDim1 << " dimensions,\n" << endl;
		cout << "while the output of the region has a dimensionality of " << outputDim << ".\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateThirdLayerToOutput


// function to propagate the input signals from the inputVector through the nodes in the cortical region and put the results of this propagation in the cortical region's outputVector
void	CorticalRegionAbs::propagateSignals( neuromodulators nodeNeuromodulator[21] )
{
	int i;
	std::thread t[3];
	neuromodulators	firstLayerNeuromodulators[16], secondLayerNeuromodulators[4], thirdLayerNeuromodulators;

	CorticalRegionAbs::putOnInput();					// puts the inputs on the first hierarchical layer of nodes
	CorticalRegionAbs::firstToSecondConnection();				// makes the connections between the first and the second hierarchical layer
	CorticalRegionAbs::secondToThirdConnection();				// makes the connections between the second and the third hierarchical layer
	CorticalRegionAbs::putOnOutput();					// puts the outputs from the hierarchical layer three on the output of the region

	for (i = 0; i < 16; i++ )
	{
		firstLayerNeuromodulators[i] = nodeNeuromodulator[i];
	}
	// This creates a thread that makes the following call "CorticalRegionAbs::propagateFirstLayer(firstLayerNeuromodulators);" which propagates the signals through the firdt hierarchical layer of nodes
	t[0] = CorticalRegionAbs::propagateFirstLayerLauncher(firstLayerNeuromodulators);

	//CorticalRegionAbs::propagateFirstLayer(firstLayerNeuromodulators);	// propagates the signals through the first hierarchical layer of nodes

	for (i = 0; i < 4; i++ )
	{
		secondLayerNeuromodulators[i] = nodeNeuromodulator[i+16];
	}
	// This creates a thread that makes the following call "CorticalRegionAbs::propagateSecondLayer(secondLayerNeuromodulators);" which propagates the signals through the second hierarchical layer of nodes
	//t[1] = CorticalRegionAbs::propagateSecondLayerLauncher(secondLayerNeuromodulators);

	CorticalRegionAbs::propagateSecondLayer(secondLayerNeuromodulators);	// propagates the signals through the second hierarchical layer of nodes

	thirdLayerNeuromodulators = nodeNeuromodulator[20];
	// This creates a thread that makes the following call "CorticalRegionAbs::propagateThirdLayer(thirdLayerNeuromodulators);" which propagates the signals through the third hierarchical layer of nodes
	//t[2] = CorticalRegionAbs::propagateThirdLayerLauncher(thirdLayerNeuromodulators);

	CorticalRegionAbs::propagateThirdLayer(thirdLayerNeuromodulators);	// propagates the signals through the third hierarchical layer of nodes

	t[0].join();
	//t[1].join();
	//t[2].join();
} // end function propagateSignals


// function to propagate the signals through the first hierarchical layer of nodes
void	CorticalRegionAbs::propagateFirstLayer( neuromodulators first[16] )
{
	//std::thread t[16];

	//t[0] = node_H1_1.propagateSignalsLauncher(first[0].layer4, first[0].layer5, first[0].layer6, first[0].layer23FG, first[0].layer23BG);
	node_H1_1.propagateSignals(first[0].layer4, first[0].layer5, first[0].layer6, first[0].layer23FG, first[0].layer23BG);

	//t[1] = node_H1_2.propagateSignalsLauncher(first[1].layer4, first[1].layer5, first[1].layer6, first[1].layer23FG, first[1].layer23BG);
	node_H1_2.propagateSignals(first[1].layer4, first[1].layer5, first[1].layer6, first[1].layer23FG, first[1].layer23BG);

	//t[2] = node_H1_3.propagateSignalsLauncher(first[2].layer4, first[2].layer5, first[2].layer6, first[2].layer23FG, first[2].layer23BG);
	node_H1_3.propagateSignals(first[2].layer4, first[2].layer5, first[2].layer6, first[2].layer23FG, first[2].layer23BG);

	//t[3] = node_H1_4.propagateSignalsLauncher(first[3].layer4, first[3].layer5, first[3].layer6, first[3].layer23FG, first[3].layer23BG);
	node_H1_4.propagateSignals(first[3].layer4, first[3].layer5, first[3].layer6, first[3].layer23FG, first[3].layer23BG);

	//t[4] = node_H1_1.propagateSignalsLauncher(first[4].layer4, first[4].layer5, first[4].layer6, first[4].layer23FG, first[4].layer23BG);
	node_H1_5.propagateSignals(first[4].layer4, first[4].layer5, first[4].layer6, first[4].layer23FG, first[4].layer23BG);

	//t[5] = node_H1_2.propagateSignalsLauncher(first[5].layer4, first[5].layer5, first[5].layer6, first[5].layer23FG, first[5].layer23BG);
	node_H1_6.propagateSignals(first[5].layer4, first[5].layer5, first[5].layer6, first[5].layer23FG, first[5].layer23BG);

	//t[6] = node_H1_3.propagateSignalsLauncher(first[6].layer4, first[6].layer5, first[6].layer6, first[6].layer23FG, first[6].layer23BG);
	node_H1_7.propagateSignals(first[6].layer4, first[6].layer5, first[6].layer6, first[6].layer23FG, first[6].layer23BG);

	//t[7] = node_H1_4.propagateSignalsLauncher(first[7].layer4, first[7].layer5, first[7].layer6, first[7].layer23FG, first[7].layer23BG);
	node_H1_8.propagateSignals(first[7].layer4, first[7].layer5, first[7].layer6, first[7].layer23FG, first[7].layer23BG);

	//t[8] = node_H1_1.propagateSignalsLauncher(first[8].layer4, first[8].layer5, first[8].layer6, first[8].layer23FG, first[8].layer23BG);
	node_H1_9.propagateSignals(first[8].layer4, first[8].layer5, first[8].layer6, first[8].layer23FG, first[8].layer23BG);

	//t[9] = node_H1_2.propagateSignalsLauncher(first[9].layer4, first[9].layer5, first[9].layer6, first[9].layer23FG, first[9].layer23BG);
	node_H1_10.propagateSignals(first[9].layer4, first[9].layer5, first[9].layer6, first[9].layer23FG, first[9].layer23BG);

	//t[10] = node_H1_3.propagateSignalsLauncher(first[10].layer4, first[10].layer5, first[10].layer6, first[10].layer23FG, first[10].layer23BG);
	node_H1_11.propagateSignals(first[10].layer4, first[10].layer5, first[10].layer6, first[10].layer23FG, first[10].layer23BG);

	//t[11] = node_H1_4.propagateSignalsLauncher(first[11].layer4, first[11].layer5, first[11].layer6, first[11].layer23FG, first[11].layer23BG);
	node_H1_12.propagateSignals(first[11].layer4, first[11].layer5, first[11].layer6, first[11].layer23FG, first[11].layer23BG);

	//t[12] = node_H1_1.propagateSignalsLauncher(first[12].layer4, first[12].layer5, first[12].layer6, first[12].layer23FG, first[12].layer23BG);
	node_H1_13.propagateSignals(first[12].layer4, first[12].layer5, first[12].layer6, first[12].layer23FG, first[12].layer23BG);

	//t[13] = node_H1_2.propagateSignalsLauncher(first[13].layer4, first[13].layer5, first[13].layer6, first[13].layer23FG, first[13].layer23BG);
	node_H1_14.propagateSignals(first[13].layer4, first[13].layer5, first[13].layer6, first[13].layer23FG, first[13].layer23BG);

	//t[14] = node_H1_3.propagateSignalsLauncher(first[14].layer4, first[14].layer5, first[14].layer6, first[14].layer23FG, first[14].layer23BG);
	node_H1_15.propagateSignals(first[14].layer4, first[14].layer5, first[14].layer6, first[14].layer23FG, first[14].layer23BG);

	//t[15] = node_H1_4.propagateSignalsLauncher(first[15].layer4, first[15].layer5, first[15].layer6, first[15].layer23FG, first[15].layer23BG);
	node_H1_16.propagateSignals(first[15].layer4, first[15].layer5, first[15].layer6, first[15].layer23FG, first[15].layer23BG);

	//t[0].join();
	//t[1].join();
	//t[2].join();
	//t[3].join();
	//t[4].join();
	//t[5].join();
	//t[6].join();
	//t[7].join();
	//t[8].join();
	//t[9].join();
	//t[10].join();
	//t[11].join();
	//t[12].join();
	//t[13].join();
	//t[14].join();
	//t[15].join();
} // end function propagateFirstLayer


// function to propagate the signals through the second hierarchical layer of nodes
void	CorticalRegionAbs::propagateSecondLayer( neuromodulators second[4] )
{
	std::thread t[4];

	//t[0] = node_H2_1.propagateSignalsLauncher(second[0].layer4, second[0].layer5, second[0].layer6, second[0].layer23FG, second[0].layer23BG);
	node_H2_1.propagateSignals(second[0].layer4, second[0].layer5, second[0].layer6, second[0].layer23FG, second[0].layer23BG);

	//t[1] = node_H2_2.propagateSignalsLauncher(second[1].layer4, second[1].layer5, second[1].layer6, second[1].layer23FG, second[1].layer23BG);
	node_H2_2.propagateSignals(second[1].layer4, second[1].layer5, second[1].layer6, second[1].layer23FG, second[1].layer23BG);

	//t[2] = node_H2_3.propagateSignalsLauncher(second[2].layer4, second[2].layer5, second[2].layer6, second[2].layer23FG, second[2].layer23BG);
	node_H2_3.propagateSignals(second[2].layer4, second[2].layer5, second[2].layer6, second[2].layer23FG, second[2].layer23BG);

	//t[3] = node_H2_4.propagateSignalsLauncher(second[3].layer4, second[3].layer5, second[3].layer6, second[3].layer23FG, second[3].layer23BG);
	node_H2_4.propagateSignals(second[3].layer4, second[3].layer5, second[3].layer6, second[3].layer23FG, second[3].layer23BG);

	//t[0].join();
	//t[1].join();
	//t[2].join();
	//t[3].join();
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
	double	*vector, *inputVector[5];

	sinc = CorticalRegion::getInputSynchronization();			// gets the input synchronization of the region

	for ( i = 0; i < 5; i++ )
		inputVector[i] = CorticalRegion::getInputVector(i);				// gets the input vector to the region

	vector = (double*)calloc(60, sizeof(double));				// reserves space for vector


	///////////////////////////////////////////////
	// First geoup of four nodes (1, 2, 3 and 4).//
	///////////////////////////////////////////////
	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *(inputVector[i] + j);
		}
	}
	//CorticalRegionAbs::normalizeVector(60, vector);				// normalizes vector
	node_H1_1.setForwardInputVector(vector);				// sets forward input vector for the 1-st node in the first hierarchical layer
	node_H1_1.setInputSynchronization(sinc);				// sets the input synchronizaion for the 1-st node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i] + j) + 14);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_2.setForwardInputVector(vector);				// sets forward input vector for the 2-nd node in the first hierarchical layer
	node_H1_2.setInputSynchronization(sinc);				// sets the input synchronizaion for the 2-nd node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *(inputVector[i+2] + j);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_3.setForwardInputVector(vector);				// sets forward input vector for the 3-rd node in the first hierarchical layer
	node_H1_3.setInputSynchronization(sinc);				// sets the input synchronizaion for the 3-rd node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i+2] + j) + 14);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_4.setForwardInputVector(vector);				// sets forward input vector for the 4-th node in the first hierarchical layer
	node_H1_4.setInputSynchronization(sinc);				// sets the input synchronizaion for the 4-th node in the first hierarchical layer


	////////////////////////////////////////////////
	// Second geoup of four nodes (5, 6, 7 and 8).//
	////////////////////////////////////////////////
	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i] + j) + 30);
		}
	}
	//CorticalRegionAbs::normalizeVector(60, vector);				// normalizes vector
	node_H1_5.setForwardInputVector(vector);				// sets forward input vector for the 5-th node in the first hierarchical layer
	node_H1_5.setInputSynchronization(sinc);				// sets the input synchronizaion for the 5-th node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i] + j) + 46);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_6.setForwardInputVector(vector);				// sets forward input vector for the 6-th node in the first hierarchical layer
	node_H1_6.setInputSynchronization(sinc);				// sets the input synchronizaion for the 6-th node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i+2] + j) + 30);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_7.setForwardInputVector(vector);				// sets forward input vector for the 7-th node in the first hierarchical layer
	node_H1_7.setInputSynchronization(sinc);				// sets the input synchronizaion for the 7-th node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i+2] + j) + 46);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_8.setForwardInputVector(vector);				// sets forward input vector for the 8-th node in the first hierarchical layer
	node_H1_8.setInputSynchronization(sinc);				// sets the input synchronizaion for the 8-th node in the first hierarchical layer


	///////////////////////////////////////////////////
	// Third geoup of four nodes (9, 10, 11 and 12).//
	///////////////////////////////////////////////////
	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i] + j) + 62);
		}
	}
	//CorticalRegionAbs::normalizeVector(60, vector);				// normalizes vector
	node_H1_9.setForwardInputVector(vector);				// sets forward input vector for the 9-th node in the first hierarchical layer
	node_H1_9.setInputSynchronization(sinc);				// sets the input synchronizaion for the 9-th node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i] + j) + 78);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_10.setForwardInputVector(vector);				// sets forward input vector for the 10-th node in the first hierarchical layer
	node_H1_10.setInputSynchronization(sinc);				// sets the input synchronizaion for the 10-th node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i+2] + j) + 62);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_11.setForwardInputVector(vector);				// sets forward input vector for the 11-th node in the first hierarchical layer
	node_H1_11.setInputSynchronization(sinc);				// sets the input synchronizaion for the 11-th node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i+2] + j) + 78);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_12.setForwardInputVector(vector);				// sets forward input vector for the 12-th node in the first hierarchical layer
	node_H1_12.setInputSynchronization(sinc);				// sets the input synchronizaion for the 12-th node in the first hierarchical layer


	////////////////////////////////////////////////////
	// Fourth geoup of four nodes (13, 14, 15 and 16).//
	////////////////////////////////////////////////////
	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i] + j) + 94);
		}
	}
	//CorticalRegionAbs::normalizeVector(60, vector);				// normalizes vector
	node_H1_13.setForwardInputVector(vector);				// sets forward input vector for the 13-th node in the first hierarchical layer
	node_H1_13.setInputSynchronization(sinc);				// sets the input synchronizaion for the 13-th node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i] + j) + 108);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_14.setForwardInputVector(vector);				// sets forward input vector for the 14-th node in the first hierarchical layer
	node_H1_14.setInputSynchronization(sinc);				// sets the input synchronizaion for the 14-th node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i+2] + j) + 94);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_15.setForwardInputVector(vector);				// sets forward input vector for the 15-th node in the first hierarchical layer
	node_H1_15.setInputSynchronization(sinc);				// sets the input synchronizaion for the 15-th node in the first hierarchical layer

	for ( i = 0; i < 3; i++ ) {
		for (j = 0; j < 20; j++ ) {
			*((vector + j) + 20*i) = *((inputVector[i+2] + j) + 108);
		}
	}
	//CorticalRegionAbs::normalizeVector(11, vector);				// normalizes vector
	node_H1_16.setForwardInputVector(vector);				// sets forward input vector for the 16-th node in the first hierarchical layer
	node_H1_16.setInputSynchronization(sinc);				// sets the input synchronizaion for the 16-th node in the first hierarchical layer

	free(vector);								// frees the memory used by vector
	free(inputVector);							// frees the memory used by inputVector
} // end function putOnInput


// function to make all the connections between nodes of the first to the second hierarchical layer
void	CorticalRegionAbs::firstToSecondConnection()
{
	int	i, j, number, number1, number2, number3, number4;
	bool	sinc, sinc1, sinc2, sinc3, sinc4, active;
	double	*inputVector, *outputVector;

	// this is for the forward information path

	active = true;
	number1 = node_H1_1.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 1 in the first hierarchical layer
	number2 = node_H1_2.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 2 in the first hierarchical layer
	number3 = node_H1_3.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 3 in the first hierarchical layer
	number4 = node_H1_4.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 4 in the first hierarchical layer
	number = number1 + number2 + number3, number4;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H1_1.getForwardOutputVector();			// gets the forward output vector from node 1 in the first hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_2.getForwardOutputVector();			// gets the forward output vector from node 2 in the first hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_3.getForwardOutputVector();			// gets the forward output vector from node 3 in the first hierarchical layer
	for (j = 0; j < number3; j++ )
	{
		*((inputVector + number1 + number2) + j) = *(outputVector + j);			// fills the third part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_4.getForwardOutputVector();			// gets the forward output vector from node 4 in the first hierarchical layer
	for (j = 0; j < number4; j++ )
	{
		*((inputVector + number1 + number2 + number3) + j) = *(outputVector + j);		// fills the fourth part of inputVector
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
	sinc3 = node_H1_3.getOutputSynchronization();				// gets the output synchronization from the 3-rd node in the first hierarchical layer
	sinc4 = node_H1_4.getOutputSynchronization();				// gets the output synchronization from the 4-th node in the first hierarchical layer

	if ( sinc1 )
		CorticalRegionAbs::setSynchronizationVariable(1, 1, true);

	if ( sinc2 )
		CorticalRegionAbs::setSynchronizationVariable(1, 2, true);

	if ( sinc3 )
		CorticalRegionAbs::setSynchronizationVariable(1, 3, true);

	if ( sinc4 )
		CorticalRegionAbs::setSynchronizationVariable(1, 4, true);

	sinc1 = CorticalRegionAbs::getSynchronizationVariable(1, 1);
	sinc2 = CorticalRegionAbs::getSynchronizationVariable(1, 2);
	sinc3 = CorticalRegionAbs::getSynchronizationVariable(1, 3);
	sinc4 = CorticalRegionAbs::getSynchronizationVariable(1, 4);

	sinc = sinc1 & sinc2 & sinc3 & sinc4;

	if ( sinc )
	{
		node_H2_1.setInputSynchronization(true);			// sets the input synchronization of the 1-st node in the second hierarchical layer
		if ( active )
		{
			CorticalRegionAbs::setSynchronizationVariable(1, 1, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 2, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 3, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 4, false);
		}
	}
	else
	{
		node_H2_1.setInputSynchronization(false);			// sets the input synchronization of the 1-st node in the second hierarchical layer
	}

	///////////////

	active = true;
	number1 = node_H1_5.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 5 in the first hierarchical layer
	number2 = node_H1_6.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 6 in the first hierarchical layer
	number3 = node_H1_7.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 7 in the first hierarchical layer
	number4 = node_H1_8.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 8 in the first hierarchical layer
	number = number1 + number2 + number3, number4;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H1_5.getForwardOutputVector();			// gets the forward output vector from node 5 in the first hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_6.getForwardOutputVector();			// gets the forward output vector from node 6 in the first hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_7.getForwardOutputVector();			// gets the forward output vector from node 7 in the first hierarchical layer
	for (j = 0; j < number3; j++ )
	{
		*((inputVector + number1 + number2) + j) = *(outputVector + j);			// fills the third part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_8.getForwardOutputVector();			// gets the forward output vector from node 8 in the first hierarchical layer
	for (j = 0; j < number4; j++ )
	{
		*((inputVector + number1 + number2 + number3) + j) = *(outputVector + j);		// fills the fourth part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	node_H2_2.setForwardInputVector(inputVector);				// sets the forward input vector of node 2 in the second hierarchical layer
	free(inputVector);
	inputVector = NULL;

	sinc1 = node_H1_5.getOutputSynchronization();				// gets the output synchronization from the 5-th node in the first hierarchical layer
	sinc2 = node_H1_6.getOutputSynchronization();				// gets the output synchronization from the 6-th node in the first hierarchical layer
	sinc3 = node_H1_7.getOutputSynchronization();				// gets the output synchronization from the 7-th node in the first hierarchical layer
	sinc4 = node_H1_8.getOutputSynchronization();				// gets the output synchronization from the 8-th node in the first hierarchical layer

	if ( sinc1 )
		CorticalRegionAbs::setSynchronizationVariable(1, 5, true);

	if ( sinc2 )
		CorticalRegionAbs::setSynchronizationVariable(1, 6, true);

	if ( sinc3 )
		CorticalRegionAbs::setSynchronizationVariable(1, 7, true);

	if ( sinc4 )
		CorticalRegionAbs::setSynchronizationVariable(1, 8, true);

	sinc1 = CorticalRegionAbs::getSynchronizationVariable(1, 5);
	sinc2 = CorticalRegionAbs::getSynchronizationVariable(1, 6);
	sinc3 = CorticalRegionAbs::getSynchronizationVariable(1, 7);
	sinc4 = CorticalRegionAbs::getSynchronizationVariable(1, 8);

	sinc = sinc1 & sinc2 & sinc3 & sinc4;

	if ( sinc )
	{
		node_H2_2.setInputSynchronization(true);			// sets the input synchronization of the 2-nd node in the second hierarchical layer
		if ( active )
		{
			CorticalRegionAbs::setSynchronizationVariable(1, 5, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 6, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 7, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 8, false);
		}
	}
	else
	{
		node_H2_2.setInputSynchronization(false);			// sets the input synchronization of the 2-nd node in the second hierarchical layer
	}

	///////////////

	active = true;
	number1 = node_H1_9.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 9 in the first hierarchical layer
	number2 = node_H1_10.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 10 in the first hierarchical layer
	number3 = node_H1_11.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 11 in the first hierarchical layer
	number4 = node_H1_12.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 12 in the first hierarchical layer
	number = number1 + number2 + number3, number4;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H1_9.getForwardOutputVector();			// gets the forward output vector from node 9 in the first hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_10.getForwardOutputVector();			// gets the forward output vector from node 10 in the first hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_11.getForwardOutputVector();			// gets the forward output vector from node 11 in the first hierarchical layer
	for (j = 0; j < number3; j++ )
	{
		*((inputVector + number1 + number2) + j) = *(outputVector + j);			// fills the third part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_12.getForwardOutputVector();			// gets the forward output vector from node 12 in the first hierarchical layer
	for (j = 0; j < number4; j++ )
	{
		*((inputVector + number1 + number2 + number3) + j) = *(outputVector + j);		// fills the fourth part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	node_H2_3.setForwardInputVector(inputVector);				// sets the forward input vector of node 3 in the second hierarchical layer
	free(inputVector);
	inputVector = NULL;

	sinc1 = node_H1_9.getOutputSynchronization();				// gets the output synchronization from the 9-th node in the first hierarchical layer
	sinc2 = node_H1_10.getOutputSynchronization();				// gets the output synchronization from the 10-th node in the first hierarchical layer
	sinc3 = node_H1_11.getOutputSynchronization();				// gets the output synchronization from the 11-th node in the first hierarchical layer
	sinc4 = node_H1_12.getOutputSynchronization();				// gets the output synchronization from the 12-th node in the first hierarchical layer

	if ( sinc1 )
		CorticalRegionAbs::setSynchronizationVariable(1, 9, true);

	if ( sinc2 )
		CorticalRegionAbs::setSynchronizationVariable(1, 10, true);

	if ( sinc3 )
		CorticalRegionAbs::setSynchronizationVariable(1, 11, true);

	if ( sinc4 )
		CorticalRegionAbs::setSynchronizationVariable(1, 12, true);

	sinc1 = CorticalRegionAbs::getSynchronizationVariable(1, 9);
	sinc2 = CorticalRegionAbs::getSynchronizationVariable(1, 10);
	sinc3 = CorticalRegionAbs::getSynchronizationVariable(1, 11);
	sinc4 = CorticalRegionAbs::getSynchronizationVariable(1, 12);

	sinc = sinc1 & sinc2 & sinc3 & sinc4;

	if ( sinc )
	{
		node_H2_3.setInputSynchronization(true);			// sets the input synchronization of the 3-rd node in the second hierarchical layer
		if ( active )
		{
			CorticalRegionAbs::setSynchronizationVariable(1, 9, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 10, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 11, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 12, false);
		}
	}
	else
	{
		node_H2_3.setInputSynchronization(false);			// sets the input synchronization of the 3-rd node in the second hierarchical layer
	}

	///////////////

	active = true;
	number1 = node_H1_13.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 13 in the first hierarchical layer
	number2 = node_H1_14.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 14 in the first hierarchical layer
	number3 = node_H1_15.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 15 in the first hierarchical layer
	number4 = node_H1_16.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 16 in the first hierarchical layer
	number = number1 + number2 + number3, number4;
	inputVector = (double*)calloc(number, sizeof(double));			// reserves space for inputVector

	outputVector = node_H1_13.getForwardOutputVector();			// gets the forward output vector from node 13 in the first hierarchical layer
	for (j = 0; j < number1; j++ )
	{
		*(inputVector + j) = *(outputVector + j);			// fills the first part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_14.getForwardOutputVector();			// gets the forward output vector from node 14 in the first hierarchical layer
	for (j = 0; j < number2; j++ )
	{
		*((inputVector + number1) + j) = *(outputVector + j);		// fills the second part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_15.getForwardOutputVector();			// gets the forward output vector from node 15 in the first hierarchical layer
	for (j = 0; j < number3; j++ )
	{
		*((inputVector + number1 + number2) + j) = *(outputVector + j);			// fills the third part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H1_16.getForwardOutputVector();			// gets the forward output vector from node 16 in the first hierarchical layer
	for (j = 0; j < number4; j++ )
	{
		*((inputVector + number1 + number2 + number3) + j) = *(outputVector + j);		// fills the fourth part of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	node_H2_4.setForwardInputVector(inputVector);				// sets the forward input vector of node 4 in the second hierarchical layer
	free(inputVector);
	inputVector = NULL;

	sinc1 = node_H1_13.getOutputSynchronization();				// gets the output synchronization from the 9-th node in the first hierarchical layer
	sinc2 = node_H1_14.getOutputSynchronization();				// gets the output synchronization from the 10-th node in the first hierarchical layer
	sinc3 = node_H1_15.getOutputSynchronization();				// gets the output synchronization from the 11-th node in the first hierarchical layer
	sinc4 = node_H1_16.getOutputSynchronization();				// gets the output synchronization from the 12-th node in the first hierarchical layer

	if ( sinc1 )
		CorticalRegionAbs::setSynchronizationVariable(1, 13, true);

	if ( sinc2 )
		CorticalRegionAbs::setSynchronizationVariable(1, 14, true);

	if ( sinc3 )
		CorticalRegionAbs::setSynchronizationVariable(1, 15, true);

	if ( sinc4 )
		CorticalRegionAbs::setSynchronizationVariable(1, 16, true);

	sinc1 = CorticalRegionAbs::getSynchronizationVariable(1, 13);
	sinc2 = CorticalRegionAbs::getSynchronizationVariable(1, 14);
	sinc3 = CorticalRegionAbs::getSynchronizationVariable(1, 15);
	sinc4 = CorticalRegionAbs::getSynchronizationVariable(1, 16);

	sinc = sinc1 & sinc2 & sinc3 & sinc4;

	if ( sinc )
	{
		node_H2_4.setInputSynchronization(true);			// sets the input synchronization of the 4-th node in the second hierarchical layer
		if ( active )
		{
			CorticalRegionAbs::setSynchronizationVariable(1, 13, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 14, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 15, false);
			CorticalRegionAbs::setSynchronizationVariable(1, 16, false);
		}
	}
	else
	{
		node_H2_4.setInputSynchronization(false);			// sets the input synchronization of the 4-th node in the second hierarchical layer
	}
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
	number3 = node_H2_3.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 3 in the second hierarchical layer
	number4 = node_H2_4.getForwardOutputDimensionality();			// gets the forward output dimensionality from node 4 in the second hierarchical layer

	number = number1 + number2, number3 + number4;
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

	outputVector = node_H2_3.getForwardOutputVector();			// gets the forward output vector from node 3 in the second hierarchical layer
	for (j = 0; j < number3; j++ )
	{
		*((inputVector + number1 + number2) + j) = *(outputVector + j);			// fills the first half of inputVector
		if ( *(outputVector + j) == 0 )
			active = false;
	}
	free(outputVector);
	outputVector = NULL;

	outputVector = node_H2_4.getForwardOutputVector();			// gets the forward output vector from node 4 in the second hierarchical layer
	for (j = 0; j < number4; j++ )
	{
		*((inputVector + number1 + number2 + number3) + j) = *(outputVector + j);		// fills the second half of inputVector
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
	sinc3 = node_H2_3.getOutputSynchronization();				// gets the output synchronization from the 3-rd node in the second hierarchical layer
	sinc4 = node_H2_4.getOutputSynchronization();				// gets the output synchronization from the 4-th node in the second hierarchical layer

	if ( sinc1 )
		CorticalRegionAbs::setSynchronizationVariable(2, 1, true);

	if ( sinc2 )
		CorticalRegionAbs::setSynchronizationVariable(2, 2, true);

	if ( sinc3 )
		CorticalRegionAbs::setSynchronizationVariable(2, 3, true);

	if ( sinc4 )
		CorticalRegionAbs::setSynchronizationVariable(2, 4, true);

	sinc1 = CorticalRegionAbs::getSynchronizationVariable(2, 1);
	sinc2 = CorticalRegionAbs::getSynchronizationVariable(2, 2);
	sinc3 = CorticalRegionAbs::getSynchronizationVariable(2, 3);
	sinc4 = CorticalRegionAbs::getSynchronizationVariable(2, 4);

	sinc = sinc1 & sinc2 & sinc3 & sinc4;

	if ( sinc )
	{
		node_H3_1.setInputSynchronization(true);			// sets the input synchronization of the 1-st node in the third hierarchical layer
		if ( active )
		{
			CorticalRegionAbs::setSynchronizationVariable(2, 1, false);
			CorticalRegionAbs::setSynchronizationVariable(2, 2, false);
			CorticalRegionAbs::setSynchronizationVariable(2, 3, false);
			CorticalRegionAbs::setSynchronizationVariable(2, 4, false);
		}
	}
	else
	{
		node_H3_1.setInputSynchronization(false);			// sets the input synchronization of the 1-st node in the third hierarchical layer
	}
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
			synchronization_1[node-1] = state;
			synchronizationAge_1[node-1] = 0;
		break;

		case 2:
			synchronization_2[node-1] = state;
			synchronizationAge_2[node-1] = 0;
		break;

		case 3:
			synchronization_3 = state;
			synchronizationAge_3 = 0;
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
			return synchronization_1[node-1];

			if ( synchronization_1[node-1] )
				synchronizationAge_1[node-1]++;

			if ( synchronizationAge_1[node-1] >= H1_TEMPORALPOOLING-1 )
				synchronization_1[node-1] = false;
		break;

		case 2:
			return synchronization_2[node-1];

			if ( synchronization_2[node-1] )
				synchronizationAge_2[node-1]++;

			if ( synchronizationAge_2[node-1] >= H2_TEMPORALPOOLING-1 )
				synchronization_2[node-1] = false;
		break;

		case 3:
			return synchronization_3;

			if ( synchronization_3 )
				synchronizationAge_3++;

			if ( synchronizationAge_3 >= H3_TEMPORALPOOLING-1 )
				synchronization_3 = false;
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
































