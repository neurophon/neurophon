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

// File Name:		Structs.h
// File Description:	Structs definition. This file contains the structs definition to be used in the cortical layers of the node


#ifndef STRUCTS_H
#define STRUCTS_H

struct levelOfExcitation				// This structure will contain the level of excitation that arrives to the input of the layer
{
	bool	active;					// It is false if some component from the input is zero; otherwise it is true
	double	maximum;				// The maximum component in the input
	double	average;				// The average level in the input
};

struct neurotransmitters				// This structure will contain the neurotransmitters originated in sub-cortical regions
{
	int	enableLearning;				// this is the variable to enable the learning process in this layer
	double	sharpThreshold;				// this is the sharpThreshold to get out from the basal state
	double	averageThreshold;			// this is the averageThreshold to get out from the basal state
	double	learningRate;				// this is the intensity with which the synaptic weights are modified
	double	learningNeighbor;			// This is a measure of the lateral influence between nearby neurons for the learning process
	double	firingNeighbor;				// This is a measure of the lateral influence between nearby neurons for the firing

	bool	predictiveEnableLearning;		// This is the variable to enable the learning process on synapsis for prediction
	double	predictiveLearningRate;			// This is the intensity with which the predictive synaptic weights are modified
	double	predictiveLearningNeighbor;		// This is a measure of the lateral influence between nearby neurons for the predictive learning process
	double	predictionThreshold;			// This is the threshold to consider a cluster as a predictive one

	double	strongLearningRate;			// This is the intensity with which the strong units synaptic weights are modified
	double	strongLearningNeighbor;			// This is a measure of the lateral influence between nearby neurons for the strong units learning process
};

struct firingInformation
{
	int	winner;					// This is the winner neuron
	bool	active;					// This tell me if the cluster was active
	double	believe;				// This is the believe on the winner neuron
	double	neighbor;				// This is the maximum neighborhood taken for the cluster to fire
};

struct neuromodulators					// This structure will contain the neuromodulators originated in sub-cortical regions
{
	neurotransmitters	layer4;			// these are the neurotransmitters for cortical layer 4
	neurotransmitters	layer5;			// these are the neurotransmitters for cortical layer 5
	neurotransmitters	layer6;			// these are the neurotransmitters for cortical layer 6
	neurotransmitters	layer23FG;		// these are the neurotransmitters for cortical layer 23 in the forward group of clusters
	neurotransmitters	layer23BG;		// these are the neurotransmitters for cortical layer 23 in the backward group of clusters
};

#endif
