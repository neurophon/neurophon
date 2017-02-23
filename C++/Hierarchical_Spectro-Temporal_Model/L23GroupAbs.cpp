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

// File Name:		L23GroupAbs.cpp
// File Description:	L23GroupAbs member-function definitions. This file contains implementations of the
//			member functions prototyped in L23GroupAbs.h.

#include <iostream>
#include <stdlib.h>
#include "L23GroupAbs.h"						// include definition of class L23GroupAbs
#include "KohonenFeatureMapAbs.h"					// KohonenFeatureMapAbs class declaration

using namespace std;

// constructor initializes the dimensionality of the input, the dimensionality of the output, the number of clusters in this group and the clusters in the group
L23GroupAbs::L23GroupAbs( int inputDimensions, int outputDimensions, int clustersNumber, KohonenFeatureMapAbsVector &clustersArray )
	: cluster( clustersArray ),					// initialize cluster KohonenFeatureMapAbs object
	  L23Group( inputDimensions, outputDimensions, clustersNumber )	// explicitly calls L23Group base-class constructor
{
	validateObject();						// calls function to validate the object
} // end L23GroupAbs constructor


// function to validate the object created of this class
void	L23GroupAbs::validateObject()
{
	int i, inputDim, outputDim, clusters;
	int clusterInputDim, clusterOutputDim, auxiliary;

	inputDim = L23Group::getInputDimensionality();
	outputDim = L23Group::getOutputDimensionality();
	clusters = L23Group::getNumberOfClusters();

	if ( inputDim != clusters )
	{
		cout << "L23GroupAbs object inconsistence: inputDim = " << inputDim << " is different to\n" << endl;
		cout << "the number of clusters in the group.\nThat is: clusters = " << clusters << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	auxiliary = 0;
	for ( i = 0; i < clusters; i++ )
	{
		clusterInputDim = cluster[i].getInputDimensionality();
		clusterOutputDim = cluster[i].getOutputDimensionality();
		if ( clusterInputDim != clusters*clusterOutputDim+1 )
		{
			cout << "L23GroupAbs object inconsistence: clusterInputDim = " << clusterInputDim << " from cluster number " << i+1 << " is different to\n" << endl;
			cout << "clusters*clusterOutputDim+1 = " << clusters*clusterOutputDim+1 << "\n" << endl;
			exit( EXIT_FAILURE );
		}
		auxiliary = auxiliary + clusterOutputDim;
	}

	if ( outputDim != auxiliary )
	{
		cout << "L23GroupAbs object inconsistence: outputDim = " << outputDim << " is different to\n" << endl;
		cout << "the sum of the outputs in every cluster.\nThat is = " << auxiliary << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// function to propagate the input signals from the inputVector through the group of clusters and put the results of this propagation in the outputVector
void	L23GroupAbs::propagateSignals( int enableLearning, double learningRate, bool firing, double learningNeighborParameter, double firingNeighborParameter )
{
	int i, clusters;

	clusters = L23Group::getNumberOfClusters();					// gets the number of clusters in this group

	L23GroupAbs::putOnInput();							// inicializes all clusters inputs	
	// propagates the signals through the array of clusters
	for (i=0; i < clusters; i++)
	{
		if ( enableLearning == 1 )
		{
			cluster[i].kohonenFeatureMapping(true, learningRate, firing, learningNeighborParameter, firingNeighborParameter);
		}
		else
		{
			cluster[i].kohonenFeatureMapping(false, learningRate, firing, learningNeighborParameter, firingNeighborParameter);
		}
	}

	L23GroupAbs::putOnOutput();							// sets the output of the group
} // end function propagateSignals


// function to initialize every cluster input vector in the group
void	L23GroupAbs::putOnInput()
{
	int i, j, k;
	int numberOfInputs, clusters, clusterInputDim, clusterOutputDim;
	double *inputPointer, *vector, *outputCluster;

	inputPointer = L23Group::getInputVector();					// gets the input vector of the group
	numberOfInputs = L23Group::getInputDimensionality();				// gets the number of components in the input vector of the group
	clusters = L23Group::getNumberOfClusters();					// gets the number of clusters in this group

	for ( i = 0; i < clusters; i++ )
	{
		clusterInputDim = cluster[i].getInputDimensionality();			// gets the dimensionality in the input of the cluster i+1
		vector = (double*)calloc(clusterInputDim, sizeof(double));		// reserves memory to hold the input of the cluster

		for ( j = 0; j < clusters; j++ )
		{
			clusterOutputDim = cluster[i].getOutputDimensionality();	// gets the dimensionality in the output of the cluster i+1
			outputCluster = cluster[j].getOutputVector();			// gets the output vector of the cluster number j+1
			// This fills vector to feed the input of the cluster number i+1
			for ( k = 0; k < clusterOutputDim; k++ )
			{
				*((vector + j*clusterOutputDim) + k) = *(outputCluster + k);
			}
		}

		*(vector + clusters*clusterOutputDim) = *(inputPointer + i);		// puts the component i+1 of the input to the group in the last component in vector
		cluster[i].setInputVector(vector);					// sets the input of the cluster i+1 in the group

		free(vector);								// frees the memory used by vector
		vector = NULL;
		free(outputCluster);							// frees the memory used by outputCluster
		vector = NULL;
	}
	free(inputPointer);								// frees the space used by the vector to which inputPointer points
} // end function putOnInput


// function to put every cluster output vector on the group output vector
void	L23GroupAbs::putOnOutput()
{
	int i, j;
	int clusters, clusterOutputDim, numberOfOutputs;
	double *vector, *outputCluster;

	numberOfOutputs = L23Group::getOutputDimensionality();				// gets the number of components in the output vector of the group
	clusters = L23Group::getNumberOfClusters();					// gets the number of clusters in this group

	vector = (double*)calloc(numberOfOutputs, sizeof(double));			// reserves memory to hold the output of the group

	for ( i = 0; i < clusters; i++ )
	{
		clusterOutputDim = cluster[i].getOutputDimensionality();		// gets the dimensionality in the output of the cluster i+1
		outputCluster = cluster[i].getOutputVector();				// gets the output vector of the cluster number j+1

		for ( j = 0; j < clusterOutputDim; j++)
			*((vector + clusterOutputDim*i) + j) = *(outputCluster + j);

		free(outputCluster);							// frees the memory used by outputCluster
		outputCluster = NULL;
	}

	L23Group::setOutputVector(vector);						// sets the outputs vector

	free(vector);									// frees the memory used by vector
} // end function putOnOutput


// function that resets the group. This functions resets the output vectors of every cluster in the group as well as the output vector in the group
void	L23GroupAbs::resetGroup()
{
	int i, numberOfOutputs, clusters;
	double *vector;

	clusters = L23Group::getNumberOfClusters();					// gets the number of clusters in this group

	for (i = 0; i < clusters; i++)
	{
		numberOfOutputs = cluster[i].getOutputDimensionality();			// gets output dimensionality of clusters inside the group
		vector = (double*)calloc(numberOfOutputs, sizeof(double));		// reserves space to hold a zero output vector for reset the output of the clusters in the group
		cluster[i].setOutputVector(vector);					// resets the output of the cluster number "i+1" in this group
		free(vector);								// frees the memory used by vector
		vector = NULL;
	}

	numberOfOutputs = L23Group::getOutputDimensionality();				// gets the dimensionality in the output vector of the group
	vector = (double*)calloc(numberOfOutputs, sizeof(double));			// reserves space for vector that has the same number of components as the group output vector
	L23Group::setOutputVector(vector);						// resets tho output vector in the group
	free(vector);									// frees the memory used by vector
	vector = NULL;
} // end function resetGroup









