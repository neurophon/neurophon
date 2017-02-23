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

// File Name:		KohonenFeatureMapPred.h
// File Description:	KohonenFeatureMapPred class definition. This file presents KohonenFeatureMapPred' public
//			interface without revealing the implementations of KohonenFeatureMapPred' member functions,
//			which are defined in KohonenFeatureMapPred.cpp.


// KohonenFeatureMapPred class derived from class KohonenFeatureMap through inheritance.
// Class Description: This is a Kohonen feature map class kind class. It is a bunch of units (neurons) arranged in an d-dimensional array whose firing function fires in an absolute way.
#ifndef PRED_KOHONEN_H
#define PRED_KOHONEN_H

#include "Structs.h"									// includes structs difinition
#include "KohonenFeatureMap.h"								// KohonenFeatureMap class declaration

class KohonenFeatureMapPred : public KohonenFeatureMap
{
public:
		KohonenFeatureMapPred( int, int, int, int );				// constructor that initializes KohonenFeatureMapPred
	void	reserveMemory( double * );						// function that reserves memory for the object arrays

	void	kohonenFeatureMapping( bool, bool, double, bool, double, double, double );		// function that computes the outputs of the network and (if the learning process is enabled) updates the synaptic weights matrix
	firingInformation	firingFunction( bool, bool, bool, double, double, double, double );	// function that returns the output winner index, the believe in that winner and update the output vector with firing intensities in every component
	void	firingOutput();								// function that sets the firing output vector based in the firing of the units array

	void	setLastWinner( int );							// function that sets lastWinner in the cluster
	int	getLastWinner();							// function that gets lastWinner in the cluster

	void	reservPredictionSynapsis();						// function that reserves space for predictionSynapsis in the cluster
	void	setPredictionSynapsis( double * );					// function that sets predictionSynapsis in the cluster
	double	*getPredictionSynapsis();						// function that gets predictionSynapsis in the cluster
	void	updatePredictionSynapsis( bool, int, double, double, double * );	// function that updates predictionSynapsis in the cluster
	double	getPrediction( int );							// function that gets a specific prediction value
	double	predictiveNeighborhoodFunction( bool, double, int, int );		// function that computes the predictive neighborhood value in the lateral interaction between units in the array for learning process

	void	reservStrongUnits();							// function that reserves space for strongUnits in the cluster
	void	setStrongUnits( double * );						// function that sets strongUnits in the cluster
	double	*getStrongUnits();							// function that gets strongUnits in the cluster
	void	updateStrongUnits( int, double, double, double * );			// function that updates strongUnits in the cluster
	double	strongNeighborhoodFunction( double, int, int );				// function that computes the strong neighborhood value in the lateral interaction between units in the array for learning process

private:
	int	lastWinner;								// this is the last winner unit of the cluster
	double	*predictionSynapsis;							// those are the prediction synapsis of the cluster
	double	*strongUnits;								// these are the measures for the strong units in the cluster
}; // end class KohonenFeatureMapPred

#endif



