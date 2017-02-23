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

// File Name:		L23Group.h
// File Description:	L23Group class definition. This file presents L23Group's public
//			interface without revealing the implementations of L23Group's member functions,
//			which are defined in L23Group.cpp.


// Class Description: This is a empty box with one input and one output. With specialization through inheritance, this empty box will contain a group of clusters.
// Those clusters could be of KohonenFeatureMapAbs kind class or of KohonenFeatureMapRel kind class
#ifndef GROUP_H
#define GROUP_H

class L23Group
{
public:
		L23Group( int, int, int );		// constructor that initializes L23Group
	void	validateObject();			// function that validates the object

	void	setInputDimensionality( int );		// function that sets the dimensionality of the input vector
	int	getInputDimensionality();		// function that gets the dimensionality of the input vector

	void	setOutputDimensionality( int );		// function that sets the dimensionality of the output vector
	int	getOutputDimensionality();		// function that gets the dimensionality of the output vector

	void	setNumberOfClusters( int );		// function that sets the number of clusters in this group
	int	getNumberOfClusters();			// function that gets the number of clusters in this group

	void	reservInputVector();			// function that reserves space for the inputs vector
	void	setInputVector( double * );		// function that sets the inputs vector
	double	*getInputVector();			// function that gets the inputs vector

	void	reservOutputVector();			// function that reserves space for the outputs vector
	void	setOutputVector( double * );		// function that sets the outputs vector
	double	*getOutputVector();			// function that gets the outputs vector

private:
	int	inputDimensionality;			// dimensionality of the input
	int	outputDimensionality;			// dimensionality of the output
	int	numberOfClusters;			// number of clusters in the group

	double	*inputVector;				// vector of inputs
	double	*outputVector;				// vector of outputs
}; // end class L23Group

#endif




