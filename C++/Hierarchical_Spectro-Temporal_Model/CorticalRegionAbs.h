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

// File Name:		CorticalRegionAbs.h
// File Description:	CorticalRegionAbs class definition. This file presents CorticalRegionAbs' public
//			interface without revealing the implementations of CorticalRegionAbs' member functions,
//			which are defined in CorticalRegionAbs.cpp.


// Class Description: This is a cortical region in which a group of nodes are intercconected in a hierarchical arrange.
// This is a CorticalRegion box filled with several cortical nodes inside. The box has one input and one output. The box determines a group of nodes.
// With specialization through inheritance, this box is a special kind inherited from CorticalRegion class kind, and contains a group of nodes.
// These nodes are of CorticalNodeAbs class kind
#ifndef REGIONABS_H
#define REGIONABS_H

#include <thread>
#include <vector>
#include "CorticalRegion.h"										// CorticalRegion class declaration
#include "CorticalNodeAbs.h"										// CorticalNodeAbs class declaration
#include "CorticalNodeAbsBase.h"									// CorticalNodeAbsBase class declaration

class CorticalRegionAbs : public CorticalRegion
{
public:
		CorticalRegionAbs( int, int, CorticalNodeAbsBase &, CorticalNodeAbsBase &, CorticalNodeAbsBase &, CorticalNodeAbsBase &, CorticalNodeAbs &, CorticalNodeAbs &, CorticalNodeAbs & );	// constructor that initializes CorticalRegionAbs
	void	validateObject();									// function that validates the object
	void	validateInputToFirstLayer();								// function that validates the consistency between the input and the first hierarchical layer
	void	validateFirstToSecondLayer();								// function that validates the consistency between the first and the second hierarchical layer
	void	validateSecondToThirdLayer();								// function that validates the consistency between the second and the third hierarchical layer
	void	validateThirdLayerToOutput();								// function that validates the consistency between the third layer and the output of the cortical region

	void	propagateSignals( neuromodulators * );							// function that propagates the signals through the cortical region
	void	propagateFirstLayer( neuromodulators * );						// function that propagates the signals through the first hierarchical layer of nodes
	void	propagateSecondLayer( neuromodulators * );						// function that propagates the signals through the second hierarchical layer of nodes
	void	propagateThirdLayer( neuromodulators );							// function that propagates the signals through the third hierarchical layer of nodes
	void	putOnInput();										// function that puts all the necessary data on every node input vector
	void	firstToSecondConnection();								// function that makes all the connections between nodes of the first to the second hierarchical layer
	void	secondToThirdConnection();								// function that makes all the connections between nodes of the second to the third hierarchical layer
	void	putOnOutput();										// function that puts all the necessary data of every node output vector on the output of the cortical region
	void	normalizeVector( int, double * );							// function that normalizes the vector received as parameter

	void	setSynchronizationVariable( int, int, bool );						// function that sets the synchronization variables of the region
	bool	getSynchronizationVariable( int, int );							// function that gets the synchronization variables of the region

	std::thread propagateFirstLayerLauncher( neuromodulators * );					// function that launches a thread to run the method propagateFirstLayer
	std::thread propagateSecondLayerLauncher( neuromodulators * );					// function that launches a thread to run the method propagateSecondLayer
	std::thread propagateThirdLayerLauncher( neuromodulators );					// function that launches a thread to run the method propagateThirdLayer

private:
	bool	synchronization_1_1;									// this is the synchronization state from node 1 in hierarchical layer 1
	bool	synchronization_1_2;									// this is the synchronization state from node 2 in hierarchical layer 1
	bool	synchronization_1_3;									// this is the synchronization state from node 3 in hierarchical layer 1
	bool	synchronization_1_4;									// this is the synchronization state from node 4 in hierarchical layer 1

	bool	synchronization_2_1;									// this is the synchronization state from node 1 in hierarchical layer 2
	bool	synchronization_2_2;									// this is the synchronization state from node 2 in hierarchical layer 2

	bool	synchronization_3_1;									// this is the synchronization state from node 1 in hierarchical layer 3

	// these nodes are in the first layer of the region
	CorticalNodeAbsBase	node_H1_1;
	CorticalNodeAbsBase	node_H1_2;
	CorticalNodeAbsBase	node_H1_3;
	CorticalNodeAbsBase	node_H1_4;

	// these nodes are in the second layer of the region
	CorticalNodeAbs	node_H2_1;
	CorticalNodeAbs	node_H2_2;

	// these nodes are in the third layer of the region
	CorticalNodeAbs	node_H3_1;

}; // end class CorticalRegionAbs

#endif







