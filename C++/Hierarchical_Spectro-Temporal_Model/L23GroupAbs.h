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

// File Name:		L23GroupAbs.h
// File Description:	L23GroupAbs class definition. This file presents L23GroupAbs's public
//			interface without revealing the implementations of L23GroupAbs's member functions,
//			which are defined in L23GroupAbs.cpp.


// Class Description: This is a box filled with several neuron clusters inside. The box has one input and one output. The box determines a Group of clusters.
// With specialization through inheritance, this box is a special kind inherited from L23Group class kind, and contains a group of clusters.
// These clusters are of KohonenFeatureMapAbs class kind
#ifndef GROUP_ABS_H
#define GROUP_ABS_H

#include <vector>
#include "L23Group.h"											// L23Group class declaration
#include "KohonenFeatureMapAbs.h"									// KohonenFeatureMapAbs class declaration
typedef std::vector<KohonenFeatureMapAbs> KohonenFeatureMapAbsVector;					// this type is defined because private member class "otherCluster" is an array of clusters of type KohonenFeatureMapAbs

class L23GroupAbs : public L23Group
{
public:
		L23GroupAbs( int, int, int, KohonenFeatureMapAbsVector & );				// constructor that initializes L23GroupAbs
	void	validateObject();									// function that validates the object

	void	propagateSignals( int, double, bool, double, double );					// propagates the signals through the group of clusters
	void	putOnInput();										// function that puts all the necessary data on every cluster input vector
	void	putOnOutput();										// function that puts all the necessary data on every cluster output vector
	void	resetGroup();										// function that resets the group

private:
	KohonenFeatureMapAbsVector	cluster;							// this is the cluster array in the group
}; // end class L23GroupAbs

#endif




