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

// File Name:		KohonenFeatureMapAbs.h
// File Description:	KohonenFeatureMapAbs class definition. This file presents KohonenFeatureMapAbs' public
//			interface without revealing the implementations of KohonenFeatureMapAbs' member functions,
//			which are defined in KohonenFeatureMapAbs.cpp.


// KohonenFeatureMapAbs class derived from class KohonenFeatureMap through inheritance.
// Class Description: This is a Kohonen feature map class kind class. It is a bunch of units (neurons) arranged in an d-dimensional array whose firing function fires in an absolute way.
#ifndef ABS_KOHONEN_H
#define ABS_KOHONEN_H

#include "Structs.h"									// includes structs difinition
#include "KohonenFeatureMap.h"								// KohonenFeatureMap class declaration

class KohonenFeatureMapAbs : public KohonenFeatureMap
{
public:
		KohonenFeatureMapAbs( int, int, int, int );				// constructor that initializes KohonenFeatureMapAbs
	void	reserveMemory( double * );						// function that reserves memory for the object arrays

	void	kohonenFeatureMapping( bool, double, bool, double, double );		// function that computes the outputs of the network and (if the learning process is enabled) updates the synaptic weights matrix
	firingInformation	firingFunction( bool, bool, double, double, double );	// function that returns the output winner index, the believe in that winner and update the output vector with firing intensities in every component
	void	firingOutput();								// function that sets the firing output vector based in the firing of the units array

	void	reservStrongUnits();							// function that reserves space for strongUnits in the cluster
	void	setStrongUnits( double * );						// function that sets strongUnits in the cluster
	double	*getStrongUnits();							// function that gets strongUnits in the cluster
	void	updateStrongUnits( int, double, double, double * );			// function that updates strongUnits in the cluster
	double	strongNeighborhoodFunction( double, int, int );				// function that computes the strong neighborhood value in the lateral interaction between units in the array for learning process

private:
	double	*strongUnits;								// these are the measures for the strong units in the cluster
}; // end class KohonenFeatureMapAbs

#endif



