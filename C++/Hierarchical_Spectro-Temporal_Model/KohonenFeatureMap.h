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

// File Name:		KohonenFeatureMap.h
// File Description:	KohonenFeatureMap class definition. This file presents KohonenFeatureMap's public
//			interface without revealing the implementations of KohonenFeatureMap's member functions,
//			which are defined in KohonenFeatureMap.cpp.


// Class Description: This is a Kohonen feature map class. It is a bunch of units (neurons) arranged in an d-dimensional array.
// All these neurons have a learning capacity by means of the modifications of the synaptic weights.
#ifndef KOHONEN_H
#define KOHONEN_H

class KohonenFeatureMap
{
public:
		KohonenFeatureMap( int, int, int, int );				// constructor that initializes KohonenFeatureMap
	void	validateObject();							// function that validates the object
	void	reserveMemory( double * );						// function that reserves memory for the object arrays

	void	learningRuleFunction( double, double, int );				// function that modifies the synapticWeights matrix through the Kohonen's learning rule
	double	learningNeighborhoodFunction( double, int, int );			// function that computes the neighborhood lateral influence parameter for learning process
	void	unitsArrayMapping( int, int * );					// function that returns the array coordinates corresponding to the unit position given as a parameter

	void	setUnitsArrayDimensionality( int );					// function that sets the dimensionality of the array of units
	int	getUnitsArrayDimensionality();						// function that gets the dimensionality of the array of units

	void	setInputDimensionality( int );						// function that sets the dimensionality of the input vector
	int	getInputDimensionality();						// function that gets the dimensionality of the input vector

	void	setUnitsDimensionality( int );						// function that sets the dimensionality of the units vector
	int	getUnitsDimensionality();						// function that gets the dimensionality of the units vector

	void	setOutputDimensionality( int );						// function that sets the dimensionality of the output vector
	int	getOutputDimensionality();						// function that gets the dimensionality of the output vector

	void	reservSynapticWeights();						// function that reserves space for the synaptic weights matrix
	void	setSynapticWeights( double * );						// function that sets the synaptic weights matrix
	double	*getSynapticWeights();							// function that gets the synaptic weights matrix

	void	reservInputVector();							// function that reserves space for the inputs vector
	void	setInputVector( double * );						// function that sets the inputs vector
	double	*getInputVector();							// function that gets the inputs vector

	void	reservUnitsVector();							// function that reserves space for the units vector
	void	setUnitsVector( bool * );						// function that sets the units vector
	bool	*getUnitsVector();							// function that gets the units vector

	void	reservOutputVector();							// function that reserves space for the output vector
	void	setOutputVector( double * );						// function that sets the output vector
	double	*getOutputVector();							// function that gets the output vector

private:
	int	unitsArrayDimensionality;						// dimensionality of the array of units
	int	inputDimensionality;							// dimensionality of the input vector
	int	unitsDimensionality;							// dimensionality of the units vector
	int	outputDimensionality;							// dimensionality of the output vector

	double	*synapticWeights;							// matrix of the synaptic weights
	double	*inputVector;								// vector of inputs
	bool	*unitsVector;								// vector of units
	double	*outputVector;								// vector of units
}; // end class KohonenFeatureMap

#endif











