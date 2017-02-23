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

// File Name:		GenerateCorticalRegion.cpp
// File Description:	This file includes a program whose main objective is to generate a CorticalRegionAbs class object.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CorticalRegionAbs.h"
#include "RegionGenerationServiceNew.h"

// starts the program GenerateCorticalNode
int main()
{




////////////////////////////////////////// objects generation variables

int	dim, inputDim, extraInputDim, unitsDim, outputDim, extraOutputDim, clusters;
char	option;
double	*weights;

////////////////////////////////////////// objects generation variables






////////////////////////////////////////// function to print the title screen

titleScreen();

////////////////////////////////////////// title screen printed






////////////////////////////////////////// This is for the clusters' generation in the region //////////////////////////////////////////
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// generates cortical layer 4 cluster

///////////// First hierarchical layer
dim = H1_CLUSTERDIMENSIONALITY;							// dimensionality of the cluster of neurons
inputDim = H1_FORWARDINPUTNODE;							// number of components in the input
unitsDim = H1_NEURONSPERCLUSTER;						// number of units in the array
outputDim = H1_OUTPUTSPERCLUSTER;						// number of components in the output

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

KohonenFeatureMapPred H1_layer4Cluster_1(dim, inputDim, unitsDim, outputDim);	// generates the layer 4 cluster of neurons
KohonenFeatureMapPred H1_layer4Cluster_2(dim, inputDim, unitsDim, outputDim);	// generates the layer 4 cluster of neurons
KohonenFeatureMapPred H1_layer4Cluster_3(dim, inputDim, unitsDim, outputDim);	// generates the layer 4 cluster of neurons
KohonenFeatureMapPred H1_layer4Cluster_4(dim, inputDim, unitsDim, outputDim);	// generates the layer 4 cluster of neurons

H1_layer4Cluster_1.reserveMemory(weights);					// reserves memory for the cluster's arrays
H1_layer4Cluster_2.reserveMemory(weights);					// reserves memory for the cluster's arrays
H1_layer4Cluster_3.reserveMemory(weights);					// reserves memory for the cluster's arrays
H1_layer4Cluster_4.reserveMemory(weights);					// reserves memory for the cluster's arrays

free(weights);
weights = NULL;

///////////// Second hierarchical layer
dim = H2_CLUSTERDIMENSIONALITY;							// dimensionality of the cluster of neurons
inputDim = H2_FORWARDINPUTNODE;							// number of components in the input
unitsDim = H2_NEURONSPERCLUSTER;						// number of units in the array
outputDim = H2_OUTPUTSPERCLUSTER;						// number of components in the output

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

KohonenFeatureMapPred H2_layer4Cluster_1(dim, inputDim, unitsDim, outputDim);	// generates the layer 4 cluster of neurons
KohonenFeatureMapPred H2_layer4Cluster_2(dim, inputDim, unitsDim, outputDim);	// generates the layer 4 cluster of neurons

H2_layer4Cluster_1.reserveMemory(weights);					// reserves memory for the cluster's arrays
H2_layer4Cluster_2.reserveMemory(weights);					// reserves memory for the cluster's arrays

free(weights);
weights = NULL;

///////////// Third hierarchical layer
dim = H3_CLUSTERDIMENSIONALITY;							// dimensionality of the cluster of neurons
inputDim = H3_FORWARDINPUTNODE;							// number of components in the input
unitsDim = H3_NEURONSPERCLUSTER;						// number of units in the array
outputDim = H3_OUTPUTSPERCLUSTER;						// number of components in the output

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

KohonenFeatureMapPred H3_layer4Cluster_1(dim, inputDim, unitsDim, outputDim);	// generates the layer 4 cluster of neurons

H3_layer4Cluster_1.reserveMemory(weights);					// reserves memory for the cluster's arrays

free(weights);
weights = NULL;

////////////////////////////////////////// cortical layer 4 cluster generated






////////////////////////////////////////// generates cortical layer 5 cluster

///////////// First hierarchical layer
dim = H1_CLUSTERDIMENSIONALITY;							// dimensionality of the cluster of neurons
inputDim = H1_CLUSTERSPERBACKWARDGROUP*H1_OUTPUTSPERGROUPCLUSTER;		// number of components in the input
unitsDim = H1_NEURONSPERCLUSTER;						// number of units in the array
outputDim = H1_OUTPUTSPERCLUSTER;						// number of components in the output

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

KohonenFeatureMapAbs H1_layer5Cluster_1(dim, inputDim, unitsDim, outputDim);	// generates the layer 5 cluster of neurons
KohonenFeatureMapAbs H1_layer5Cluster_2(dim, inputDim, unitsDim, outputDim);	// generates the layer 5 cluster of neurons
KohonenFeatureMapAbs H1_layer5Cluster_3(dim, inputDim, unitsDim, outputDim);	// generates the layer 5 cluster of neurons
KohonenFeatureMapAbs H1_layer5Cluster_4(dim, inputDim, unitsDim, outputDim);	// generates the layer 5 cluster of neurons

H1_layer5Cluster_1.reserveMemory(weights);					// reserves memory for the cluster's arrays
H1_layer5Cluster_2.reserveMemory(weights);					// reserves memory for the cluster's arrays
H1_layer5Cluster_3.reserveMemory(weights);					// reserves memory for the cluster's arrays
H1_layer5Cluster_4.reserveMemory(weights);					// reserves memory for the cluster's arrays

free(weights);
weights = NULL;

///////////// Second hierarchical layer
dim = H2_CLUSTERDIMENSIONALITY;							// dimensionality of the cluster of neurons
inputDim = H2_CLUSTERSPERBACKWARDGROUP*H2_OUTPUTSPERGROUPCLUSTER;		// number of components in the input
unitsDim = H2_NEURONSPERCLUSTER;						// number of units in the array
outputDim = H2_OUTPUTSPERCLUSTER;						// number of components in the output

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

KohonenFeatureMapAbs H2_layer5Cluster_1(dim, inputDim, unitsDim, outputDim);	// generates the layer 5 cluster of neurons
KohonenFeatureMapAbs H2_layer5Cluster_2(dim, inputDim, unitsDim, outputDim);	// generates the layer 5 cluster of neurons

H2_layer5Cluster_1.reserveMemory(weights);					// reserves memory for the cluster's arrays
H2_layer5Cluster_2.reserveMemory(weights);					// reserves memory for the cluster's arrays

free(weights);
weights = NULL;

///////////// Third hierarchical layer
dim = H3_CLUSTERDIMENSIONALITY;							// dimensionality of the cluster of neurons
inputDim = H3_CLUSTERSPERBACKWARDGROUP*H3_OUTPUTSPERGROUPCLUSTER;		// number of components in the input
unitsDim = H3_NEURONSPERCLUSTER;						// number of units in the array
outputDim = H3_OUTPUTSPERCLUSTER;						// number of components in the output

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

KohonenFeatureMapAbs H3_layer5Cluster_1(dim, inputDim, unitsDim, outputDim);	// generates the layer 5 cluster of neurons

H3_layer5Cluster_1.reserveMemory(weights);					// reserves memory for the cluster's arrays

free(weights);
weights = NULL;

////////////////////////////////////////// cortical layer 5 cluster generated






////////////////////////////////////////// generates cortical layer 6 cluster

///////////// First hierarchical layer
dim = H1_CLUSTERDIMENSIONALITY;							// dimensionality of the cluster of neurons
inputDim = H1_FORWARDINPUTNODE+H1_OUTPUTSPERCLUSTER;				// number of components in the input
unitsDim = H1_NEURONSPERCLUSTER;						// number of units in the array
outputDim = H1_OUTPUTSPERCLUSTER;						// number of components in the output

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

KohonenFeatureMapAbs H1_layer6Cluster_1(dim, inputDim, unitsDim, outputDim);	// generates the layer 6 cluster of neurons
KohonenFeatureMapAbs H1_layer6Cluster_2(dim, inputDim, unitsDim, outputDim);	// generates the layer 6 cluster of neurons
KohonenFeatureMapAbs H1_layer6Cluster_3(dim, inputDim, unitsDim, outputDim);	// generates the layer 6 cluster of neurons
KohonenFeatureMapAbs H1_layer6Cluster_4(dim, inputDim, unitsDim, outputDim);	// generates the layer 6 cluster of neurons

H1_layer6Cluster_1.reserveMemory(weights);					// reserves memory for the cluster's arrays
H1_layer6Cluster_2.reserveMemory(weights);					// reserves memory for the cluster's arrays
H1_layer6Cluster_3.reserveMemory(weights);					// reserves memory for the cluster's arrays
H1_layer6Cluster_4.reserveMemory(weights);					// reserves memory for the cluster's arrays

free(weights);
weights = NULL;

///////////// Second hierarchical layer
dim = H2_CLUSTERDIMENSIONALITY;							// dimensionality of the cluster of neurons
inputDim = H2_FORWARDINPUTNODE+H2_OUTPUTSPERCLUSTER;				// number of components in the input
unitsDim = H2_NEURONSPERCLUSTER;						// number of units in the array
outputDim = H2_OUTPUTSPERCLUSTER;						// number of components in the output

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

KohonenFeatureMapAbs H2_layer6Cluster_1(dim, inputDim, unitsDim, outputDim);	// generates the layer 6 cluster of neurons
KohonenFeatureMapAbs H2_layer6Cluster_2(dim, inputDim, unitsDim, outputDim);	// generates the layer 6 cluster of neurons

H2_layer6Cluster_1.reserveMemory(weights);					// reserves memory for the cluster's arrays
H2_layer6Cluster_2.reserveMemory(weights);					// reserves memory for the cluster's arrays

free(weights);
weights = NULL;

///////////// Third hierarchical layer
dim = H3_CLUSTERDIMENSIONALITY;							// dimensionality of the cluster of neurons
inputDim = H3_FORWARDINPUTNODE+H3_OUTPUTSPERCLUSTER;				// number of components in the input
unitsDim = H3_NEURONSPERCLUSTER;						// number of units in the array
outputDim = H3_OUTPUTSPERCLUSTER;						// number of components in the output

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

KohonenFeatureMapAbs H3_layer6Cluster_1(dim, inputDim, unitsDim, outputDim);	// generates the layer 6 cluster of neurons

H3_layer6Cluster_1.reserveMemory(weights);					// reserves memory for the cluster's arrays

free(weights);
weights = NULL;

////////////////////////////////////////// cortical layer 6 cluster generated






////////////////////////////////////////// generates forward group cluster array

int i;
///////////// First hierarchical layer
dim = H1_CLUSTERGROUPDIMENSIONALITY;						// dimensionality of the cluster of neurons
inputDim = H1_CLUSTERSPERFORWARDGROUP*H1_OUTPUTSPERGROUPCLUSTER+1;		// number of components in the input
unitsDim = H1_NEURONSPERGROUPCLUSTER;						// number of units in the array
outputDim = H1_OUTPUTSPERGROUPCLUSTER;						// number of components in the output
clusters = H1_CLUSTERSPERFORWARDGROUP;						// number of clusters per group

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

std::vector<KohonenFeatureMapAbs> H1_forwardGroupArrayCluster_1(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the forward group cluster array
std::vector<KohonenFeatureMapAbs> H1_forwardGroupArrayCluster_2(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the forward group cluster array
std::vector<KohonenFeatureMapAbs> H1_forwardGroupArrayCluster_3(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the forward group cluster array
std::vector<KohonenFeatureMapAbs> H1_forwardGroupArrayCluster_4(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the forward group cluster array

for (i = 0; i < clusters; i++)
{
	H1_forwardGroupArrayCluster_1[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
	H1_forwardGroupArrayCluster_2[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
	H1_forwardGroupArrayCluster_3[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
	H1_forwardGroupArrayCluster_4[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
}
free(weights);
weights = NULL;

///////////// Second hierarchical layer
dim = H2_CLUSTERGROUPDIMENSIONALITY;						// dimensionality of the cluster of neurons
inputDim = H2_CLUSTERSPERFORWARDGROUP*H2_OUTPUTSPERGROUPCLUSTER+1;		// number of components in the input
unitsDim = H2_NEURONSPERGROUPCLUSTER;						// number of units in the array
outputDim = H2_OUTPUTSPERGROUPCLUSTER;						// number of components in the output
clusters = H2_CLUSTERSPERFORWARDGROUP;						// number of clusters per group

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

std::vector<KohonenFeatureMapAbs> H2_forwardGroupArrayCluster_1(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the forward group cluster array
std::vector<KohonenFeatureMapAbs> H2_forwardGroupArrayCluster_2(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the forward group cluster array
for (i = 0; i < clusters; i++)
{
	H2_forwardGroupArrayCluster_1[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
	H2_forwardGroupArrayCluster_2[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
}
free(weights);
weights = NULL;

///////////// Third hierarchical layer
dim = H3_CLUSTERGROUPDIMENSIONALITY;						// dimensionality of the cluster of neurons
inputDim = H3_CLUSTERSPERFORWARDGROUP*H3_OUTPUTSPERGROUPCLUSTER+1;		// number of components in the input
unitsDim = H3_NEURONSPERGROUPCLUSTER;						// number of units in the array
outputDim = H3_OUTPUTSPERGROUPCLUSTER;						// number of components in the output
clusters = H3_CLUSTERSPERFORWARDGROUP;						// number of clusters per group

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

std::vector<KohonenFeatureMapAbs> H3_forwardGroupArrayCluster_1(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the forward group cluster array
for (i = 0; i < clusters; i++)
{
	H3_forwardGroupArrayCluster_1[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
}
free(weights);
weights = NULL;

////////////////////////////////////////// forward group cluster array generated






////////////////////////////////////////// generates backward group cluster array

///////////// First hierarchical layer
dim = H1_CLUSTERGROUPDIMENSIONALITY;						// dimensionality of the cluster of neurons
inputDim = H1_CLUSTERSPERBACKWARDGROUP*H1_OUTPUTSPERGROUPCLUSTER+1;		// number of components in the input
unitsDim = H1_NEURONSPERGROUPCLUSTER;						// number of units in the array
outputDim = H1_OUTPUTSPERGROUPCLUSTER;						// number of components in the output
clusters = H1_CLUSTERSPERBACKWARDGROUP;						// number of clusters per group

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

std::vector<KohonenFeatureMapAbs> H1_backwardGroupArrayCluster_1(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the backward group cluster array
std::vector<KohonenFeatureMapAbs> H1_backwardGroupArrayCluster_2(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the backward group cluster array
std::vector<KohonenFeatureMapAbs> H1_backwardGroupArrayCluster_3(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the backward group cluster array
std::vector<KohonenFeatureMapAbs> H1_backwardGroupArrayCluster_4(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the backward group cluster array
for (i = 0; i < clusters; i++)
{
	H1_backwardGroupArrayCluster_1[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
	H1_backwardGroupArrayCluster_2[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
	H1_backwardGroupArrayCluster_3[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
	H1_backwardGroupArrayCluster_4[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
}

free(weights);
weights = NULL;

///////////// Second hierarchical layer
dim = H2_CLUSTERGROUPDIMENSIONALITY;						// dimensionality of the cluster of neurons
inputDim = H2_CLUSTERSPERBACKWARDGROUP*H2_OUTPUTSPERGROUPCLUSTER+1;		// number of components in the input
unitsDim = H2_NEURONSPERGROUPCLUSTER;						// number of units in the array
outputDim = H2_OUTPUTSPERGROUPCLUSTER;						// number of components in the output
clusters = H2_CLUSTERSPERBACKWARDGROUP;						// number of clusters per group

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

std::vector<KohonenFeatureMapAbs> H2_backwardGroupArrayCluster_1(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the backward group cluster array
std::vector<KohonenFeatureMapAbs> H2_backwardGroupArrayCluster_2(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the backward group cluster array
for (i = 0; i < clusters; i++)
{
	H2_backwardGroupArrayCluster_1[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
	H2_backwardGroupArrayCluster_2[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
}

free(weights);
weights = NULL;

///////////// Third hierarchical layer
dim = H3_CLUSTERGROUPDIMENSIONALITY;						// dimensionality of the cluster of neurons
inputDim = H3_CLUSTERSPERBACKWARDGROUP*H3_OUTPUTSPERGROUPCLUSTER+1;		// number of components in the input
unitsDim = H3_NEURONSPERGROUPCLUSTER;						// number of units in the array
outputDim = H3_OUTPUTSPERGROUPCLUSTER;						// number of components in the output
clusters = H3_CLUSTERSPERBACKWARDGROUP;						// number of clusters per group

weights = (double*)calloc(unitsDim*inputDim, sizeof(double));			// reserves space for the weights of the cluster's neurons
generateWeights(0, 1, inputDim, unitsDim, weights);				// generates a random matrix with the weights of the cluster's neurons

std::vector<KohonenFeatureMapAbs> H3_backwardGroupArrayCluster_1(clusters,KohonenFeatureMapAbs(dim, inputDim, unitsDim, outputDim));	// generates the backward group cluster array
for (i = 0; i < clusters; i++)
{
	H3_backwardGroupArrayCluster_1[i].reserveMemory(weights);		// reserves memory for the cluster's arrays
}

free(weights);
weights = NULL;

////////////////////////////////////////// backward group cluster array generated






printf("\nAll clusters of the region have been generated correctly\n");






////////////////////////////////////////// This is for the groups' generation in the region //////////////////////////////////////////
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// generates cortical layer 2/3 forward group

///////////// First hierarchical layer
inputDim = H1_OUTPUTSPERCLUSTER;						// number of components in the input
outputDim = H1_CLUSTERSPERFORWARDGROUP*H1_OUTPUTSPERGROUPCLUSTER;		// number of components in the output
clusters = H1_CLUSTERSPERFORWARDGROUP;						// number of clusters per group

L23GroupAbs H1_forwardGroup_1(inputDim, outputDim, clusters, H1_forwardGroupArrayCluster_1);	// generates the layer 2/3 forward group
L23GroupAbs H1_forwardGroup_2(inputDim, outputDim, clusters, H1_forwardGroupArrayCluster_2);	// generates the layer 2/3 forward group
L23GroupAbs H1_forwardGroup_3(inputDim, outputDim, clusters, H1_forwardGroupArrayCluster_3);	// generates the layer 2/3 forward group
L23GroupAbs H1_forwardGroup_4(inputDim, outputDim, clusters, H1_forwardGroupArrayCluster_4);	// generates the layer 2/3 forward group

///////////// Second hierarchical layer
inputDim = H2_OUTPUTSPERCLUSTER;						// number of components in the input
outputDim = H2_CLUSTERSPERFORWARDGROUP*H2_OUTPUTSPERGROUPCLUSTER;		// number of components in the output
clusters = H2_CLUSTERSPERFORWARDGROUP;						// number of clusters per group

L23GroupAbs H2_forwardGroup_1(inputDim, outputDim, clusters, H2_forwardGroupArrayCluster_1);	// generates the layer 2/3 forward group
L23GroupAbs H2_forwardGroup_2(inputDim, outputDim, clusters, H2_forwardGroupArrayCluster_2);	// generates the layer 2/3 forward group

///////////// Third hierarchical layer
inputDim = H3_OUTPUTSPERCLUSTER;						// number of components in the input
outputDim = H3_CLUSTERSPERFORWARDGROUP*H3_OUTPUTSPERGROUPCLUSTER;		// number of components in the output
clusters = H3_CLUSTERSPERFORWARDGROUP;						// number of clusters per group

L23GroupAbs H3_forwardGroup_1(inputDim, outputDim, clusters, H3_forwardGroupArrayCluster_1);	// generates the layer 2/3 forward group

////////////////////////////////////////// cortical layer 2/3 forward group generated






////////////////////////////////////////// generates cortical layer 2/3 backward group

///////////// First hierarchical layer
inputDim = H1_OUTPUTSPERCLUSTER+H1_BACKWARDINPUTNODE;				// number of components in the input
outputDim = H1_CLUSTERSPERBACKWARDGROUP*H1_OUTPUTSPERGROUPCLUSTER;		// number of components in the output
clusters = H1_CLUSTERSPERBACKWARDGROUP;						// number of clusters per group

L23GroupAbs H1_backwardGroup_1(inputDim, outputDim, clusters, H1_backwardGroupArrayCluster_1);	// generates the layer 2/3 backward group
L23GroupAbs H1_backwardGroup_2(inputDim, outputDim, clusters, H1_backwardGroupArrayCluster_2);	// generates the layer 2/3 backward group
L23GroupAbs H1_backwardGroup_3(inputDim, outputDim, clusters, H1_backwardGroupArrayCluster_3);	// generates the layer 2/3 backward group
L23GroupAbs H1_backwardGroup_4(inputDim, outputDim, clusters, H1_backwardGroupArrayCluster_4);	// generates the layer 2/3 backward group

///////////// Second hierarchical layer
inputDim = H2_OUTPUTSPERCLUSTER+H2_BACKWARDINPUTNODE;				// number of components in the input
outputDim = H2_CLUSTERSPERBACKWARDGROUP*H2_OUTPUTSPERGROUPCLUSTER;		// number of components in the output
clusters = H2_CLUSTERSPERBACKWARDGROUP;						// number of clusters per group

L23GroupAbs H2_backwardGroup_1(inputDim, outputDim, clusters, H2_backwardGroupArrayCluster_1);	// generates the layer 2/3 backward group
L23GroupAbs H2_backwardGroup_2(inputDim, outputDim, clusters, H2_backwardGroupArrayCluster_2);	// generates the layer 2/3 backward group

///////////// Third hierarchical layer
inputDim = H3_OUTPUTSPERCLUSTER+H3_BACKWARDINPUTNODE;				// number of components in the input
outputDim = H3_CLUSTERSPERBACKWARDGROUP*H3_OUTPUTSPERGROUPCLUSTER;		// number of components in the output
clusters = H3_CLUSTERSPERBACKWARDGROUP;						// number of clusters per group

L23GroupAbs H3_backwardGroup_1(inputDim, outputDim, clusters, H3_backwardGroupArrayCluster_1);	// generates the layer 2/3 backward group

////////////////////////////////////////// cortical layer 2/3 backward group generated






printf("\nAll groups of the region have been generated correctly\n");






////////////////////////////////////////// This is for the cortical layers' generation in the region //////////////////////////////////////////
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// generates cortical layer 4

///////////// First hierarchical layer
inputDim = H1_FORWARDINPUTNODE;							// number of components in the input
outputDim = H1_OUTPUTSPERCLUSTER;						// number of components in the output

CorticalLayer4Base H1_layer4_1(inputDim, outputDim, H1_layer4Cluster_1);		// generates the layer 4
CorticalLayer4Base H1_layer4_2(inputDim, outputDim, H1_layer4Cluster_2);		// generates the layer 4
CorticalLayer4Base H1_layer4_3(inputDim, outputDim, H1_layer4Cluster_3);		// generates the layer 4
CorticalLayer4Base H1_layer4_4(inputDim, outputDim, H1_layer4Cluster_4);		// generates the layer 4

///////////// Second hierarchical layer
inputDim = H2_FORWARDINPUTNODE;							// number of components in the input
outputDim = H2_OUTPUTSPERCLUSTER;						// number of components in the output

CorticalLayer4 H2_layer4_1(inputDim, outputDim, H2_layer4Cluster_1);		// generates the layer 4
CorticalLayer4 H2_layer4_2(inputDim, outputDim, H2_layer4Cluster_2);		// generates the layer 4

///////////// Third hierarchical layer
inputDim = H3_FORWARDINPUTNODE;							// number of components in the input
outputDim = H3_OUTPUTSPERCLUSTER;						// number of components in the output

CorticalLayer4 H3_layer4_1(inputDim, outputDim, H3_layer4Cluster_1);		// generates the layer 4

////////////////////////////////////////// cortical layer 4 generated






////////////////////////////////////////// generates cortical layer 5

///////////// First hierarchical layer
inputDim = H1_CLUSTERSPERBACKWARDGROUP*H1_OUTPUTSPERGROUPCLUSTER;		// number of components in the input
outputDim = H1_OUTPUTSPERCLUSTER;						// number of components in the output

CorticalLayer5 H1_layer5_1(inputDim, outputDim, H1_layer5Cluster_1);		// generates the layer 5
CorticalLayer5 H1_layer5_2(inputDim, outputDim, H1_layer5Cluster_2);		// generates the layer 5
CorticalLayer5 H1_layer5_3(inputDim, outputDim, H1_layer5Cluster_3);		// generates the layer 5
CorticalLayer5 H1_layer5_4(inputDim, outputDim, H1_layer5Cluster_4);		// generates the layer 5

///////////// Second hierarchical layer
inputDim = H2_CLUSTERSPERBACKWARDGROUP*H2_OUTPUTSPERGROUPCLUSTER;		// number of components in the input
outputDim = H2_OUTPUTSPERCLUSTER;						// number of components in the output

CorticalLayer5 H2_layer5_1(inputDim, outputDim, H2_layer5Cluster_1);		// generates the layer 5
CorticalLayer5 H2_layer5_2(inputDim, outputDim, H2_layer5Cluster_2);		// generates the layer 5

///////////// Third hierarchical layer
inputDim = H3_CLUSTERSPERBACKWARDGROUP*H3_OUTPUTSPERGROUPCLUSTER;		// number of components in the input
outputDim = H3_OUTPUTSPERCLUSTER;						// number of components in the output

CorticalLayer5 H3_layer5_1(inputDim, outputDim, H3_layer5Cluster_1);		// generates the layer 5

////////////////////////////////////////// cortical layer 5 generated






////////////////////////////////////////// generates cortical layer 6

///////////// First hierarchical layer
extraInputDim = H1_OUTPUTSPERCLUSTER;						// number of components in the extra input
inputDim = H1_FORWARDINPUTNODE;							// number of components in the input
outputDim = H1_OUTPUTSPERCLUSTER;						// number of components in the output

CorticalLayer6Base H1_layer6_1(extraInputDim, inputDim, outputDim, H1_layer6Cluster_1);	// generates the layer 6
CorticalLayer6Base H1_layer6_2(extraInputDim, inputDim, outputDim, H1_layer6Cluster_2);	// generates the layer 6
CorticalLayer6Base H1_layer6_3(extraInputDim, inputDim, outputDim, H1_layer6Cluster_3);	// generates the layer 6
CorticalLayer6Base H1_layer6_4(extraInputDim, inputDim, outputDim, H1_layer6Cluster_4);	// generates the layer 6

///////////// Second hierarchical layer
extraInputDim = H2_OUTPUTSPERCLUSTER;						// number of components in the extra input
inputDim = H2_FORWARDINPUTNODE;							// number of components in the input
outputDim = H2_OUTPUTSPERCLUSTER;						// number of components in the output

CorticalLayer6 H2_layer6_1(extraInputDim, inputDim, outputDim, H2_layer6Cluster_1);	// generates the layer 6
CorticalLayer6 H2_layer6_2(extraInputDim, inputDim, outputDim, H2_layer6Cluster_2);	// generates the layer 6

///////////// Third hierarchical layer
extraInputDim = H3_OUTPUTSPERCLUSTER;						// number of components in the extra input
inputDim = H3_FORWARDINPUTNODE;							// number of components in the input
outputDim = H3_OUTPUTSPERCLUSTER;						// number of components in the output

CorticalLayer6 H3_layer6_1(extraInputDim, inputDim, outputDim, H3_layer6Cluster_1);	// generates the layer 6

////////////////////////////////////////// cortical layer 6 generated






////////////////////////////////////////// generates cortical layer 2/3

int temporal;

///////////// First hierarchical layer
inputDim = H1_OUTPUTSPERCLUSTER;						// number of components in the forward input
outputDim = H1_TEMPORALPOOLING*H1_CLUSTERSPERFORWARDGROUP*H1_OUTPUTSPERGROUPCLUSTER;		// number of components in the forward output
extraInputDim = H1_OUTPUTSPERCLUSTER+H2_OUTPUTSPERCLUSTER;			// number of components in the backward input
extraOutputDim = H1_CLUSTERSPERBACKWARDGROUP*H1_OUTPUTSPERGROUPCLUSTER;		// number of components in the backward output
temporal = H1_TEMPORALPOOLING;							// number of temporal intances in the layer

CorticalLayer23 H1_layer23_1(temporal, inputDim, outputDim, extraInputDim, extraOutputDim, 0, H1_forwardGroup_1, H1_backwardGroup_1);	// generates the layer 2/3
CorticalLayer23 H1_layer23_2(temporal, inputDim, outputDim, extraInputDim, extraOutputDim, 0, H1_forwardGroup_2, H1_backwardGroup_2);	// generates the layer 2/3
CorticalLayer23 H1_layer23_3(temporal, inputDim, outputDim, extraInputDim, extraOutputDim, 0, H1_forwardGroup_3, H1_backwardGroup_3);	// generates the layer 2/3
CorticalLayer23 H1_layer23_4(temporal, inputDim, outputDim, extraInputDim, extraOutputDim, 0, H1_forwardGroup_4, H1_backwardGroup_4);	// generates the layer 2/3

///////////// Second hierarchical layer
inputDim = H2_OUTPUTSPERCLUSTER;						// number of components in the forward input
outputDim = H2_TEMPORALPOOLING*H2_CLUSTERSPERFORWARDGROUP*H2_OUTPUTSPERGROUPCLUSTER;		// number of components in the forward output
extraInputDim = H2_OUTPUTSPERCLUSTER+H3_OUTPUTSPERCLUSTER;			// number of components in the backward input
extraOutputDim = H2_CLUSTERSPERBACKWARDGROUP*H2_OUTPUTSPERGROUPCLUSTER;		// number of components in the backward output
temporal = H2_TEMPORALPOOLING;							// number of temporal intances in the layer

CorticalLayer23 H2_layer23_1(temporal, inputDim, outputDim, extraInputDim, extraOutputDim, 0, H2_forwardGroup_1, H2_backwardGroup_1);	// generates the layer 2/3
CorticalLayer23 H2_layer23_2(temporal, inputDim, outputDim, extraInputDim, extraOutputDim, 0, H2_forwardGroup_2, H2_backwardGroup_2);	// generates the layer 2/3

///////////// Third hierarchical layer
inputDim = H3_OUTPUTSPERCLUSTER;						// number of components in the forward input
outputDim = H3_TEMPORALPOOLING*H3_CLUSTERSPERFORWARDGROUP*H3_OUTPUTSPERGROUPCLUSTER;		// number of components in the forward output
extraInputDim = H3_OUTPUTSPERCLUSTER;						// number of components in the backward input
extraOutputDim = H3_CLUSTERSPERBACKWARDGROUP*H3_OUTPUTSPERGROUPCLUSTER;		// number of components in the backward output
temporal = H3_TEMPORALPOOLING;							// number of temporal intances in the layer

CorticalLayer23 H3_layer23_1(temporal, inputDim, outputDim, extraInputDim, extraOutputDim, 0, H3_forwardGroup_1, H3_backwardGroup_1);	// generates the layer 2/3

////////////////////////////////////////// cortical layer 2/3 generated



printf("\nAll layers of the region have been generated correctly\n");



////////////////////////////////////////// This is for the nodes' generation in the region //////////////////////////////////////////
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// generates cortical nodes

///////////// First hierarchical layer
CorticalNodeAbsBase H1_node_1(H1_FORWARDINPUTNODE, H1_FORWARDOUTPUTNODE, H1_BACKWARDINPUTNODE, H1_BACKWARDOUTPUTNODE, H1_BACKWARDEXTRAOUTPUTNODE, H1_layer4_1, H1_layer5_1, H1_layer6_1, H1_layer23_1);	// generates node
CorticalNodeAbsBase H1_node_2(H1_FORWARDINPUTNODE, H1_FORWARDOUTPUTNODE, H1_BACKWARDINPUTNODE, H1_BACKWARDOUTPUTNODE, H1_BACKWARDEXTRAOUTPUTNODE, H1_layer4_2, H1_layer5_2, H1_layer6_2, H1_layer23_2);	// generates node
CorticalNodeAbsBase H1_node_3(H1_FORWARDINPUTNODE, H1_FORWARDOUTPUTNODE, H1_BACKWARDINPUTNODE, H1_BACKWARDOUTPUTNODE, H1_BACKWARDEXTRAOUTPUTNODE, H1_layer4_3, H1_layer5_3, H1_layer6_3, H1_layer23_3);	// generates node
CorticalNodeAbsBase H1_node_4(H1_FORWARDINPUTNODE, H1_FORWARDOUTPUTNODE, H1_BACKWARDINPUTNODE, H1_BACKWARDOUTPUTNODE, H1_BACKWARDEXTRAOUTPUTNODE, H1_layer4_4, H1_layer5_4, H1_layer6_4, H1_layer23_4);	// generates node

///////////// Second hierarchical layer
CorticalNodeAbs H2_node_1(H2_FORWARDINPUTNODE, H2_FORWARDOUTPUTNODE, H2_BACKWARDINPUTNODE, H2_BACKWARDOUTPUTNODE, H2_BACKWARDEXTRAOUTPUTNODE, H2_layer4_1, H2_layer5_1, H2_layer6_1, H2_layer23_1);	// generates node
CorticalNodeAbs H2_node_2(H2_FORWARDINPUTNODE, H2_FORWARDOUTPUTNODE, H2_BACKWARDINPUTNODE, H2_BACKWARDOUTPUTNODE, H2_BACKWARDEXTRAOUTPUTNODE, H2_layer4_2, H2_layer5_2, H2_layer6_2, H2_layer23_2);	// generates node

///////////// Third hierarchical layer
CorticalNodeAbs H3_node_1(H3_FORWARDINPUTNODE, H3_FORWARDOUTPUTNODE, H3_BACKWARDINPUTNODE, H3_BACKWARDOUTPUTNODE, H3_BACKWARDEXTRAOUTPUTNODE, H3_layer4_1, H3_layer5_1, H3_layer6_1, H3_layer23_1);	// generates node

printf("\n\nCorticalNodeAbs objects generated\n\n");

////////////////////////////////////////// cortical nodes generated






////////////////////////////////////////// This is for the region's generation //////////////////////////////////////////
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// 
////////////////////////////////////////// generates cortical region

CorticalRegionAbs region(INPUTREGION, OUTPUTREGION, H1_node_1, H1_node_2, H1_node_3, H1_node_4, H2_node_1, H2_node_2, H3_node_1);	// generates region

printf("\n\nCorticalRegion object generated\n\n");

////////////////////////////////////////// cortical region generated






////////////////////////////////////////// loads the cortical region connectome

connectome	connectomeNode;

int forwardClusters, backwardClusters;

printf("\nDo you want to load the previous node connectome on the new node? (Y) for yes and any key for no\n\n");
scanf(" %c", &option);

if ( option == 'Y' )
{

	// This is for the first hierarchical layer

	forwardClusters = H1_CLUSTERSPERFORWARDGROUP;
	backwardClusters = H1_CLUSTERSPERBACKWARDGROUP;

	// Cortical Node number 1

	connectomeNode = loadConnectome(1, 1);							// loads the connectome

	H1_layer4Cluster_1.setSynapticWeights(connectomeNode.l4W);				// sets the layer 4 cluster synaptic weights
	free(connectomeNode.l4W);								// frees the memory used by connectomeNode.l4W
	connectomeNode.l4W = NULL;

	H1_layer4Cluster_1.setPredictionSynapsis(connectomeNode.l4P);				// sets the layer 4 cluster prediction synaptic weights
	free(connectomeNode.l4P);								// frees the memory used by connectomeNode.l4P
	connectomeNode.l4P = NULL;

	H1_layer4Cluster_1.setStrongUnits(connectomeNode.l4S);					// sets the layer 4 cluster strong units synaptic weights
	free(connectomeNode.l4S);								// frees the memory used by connectomeNode.l4S
	connectomeNode.l4S = NULL;

	H1_layer5Cluster_1.setSynapticWeights(connectomeNode.l5W);				// sets the layer 5 cluster synaptic weights
	free(connectomeNode.l5W);								// frees the memory used by connectomeNode.l5W
	connectomeNode.l5W = NULL;

	H1_layer5Cluster_1.setStrongUnits(connectomeNode.l5S);					// sets the layer 5 cluster strong units synaptic weights
	free(connectomeNode.l5S);								// frees the memory used by connectomeNode.l5S
	connectomeNode.l5S = NULL;

	H1_layer6Cluster_1.setSynapticWeights(connectomeNode.l6W);				// sets the layer 6 cluster synaptic weights
	free(connectomeNode.l6W);								// frees the memory used by connectomeNode.l6W
	connectomeNode.l6W = NULL;

	H1_layer6Cluster_1.setStrongUnits(connectomeNode.l6S);					// sets the layer 6 cluster strong units synaptic weights
	free(connectomeNode.l6S);								// frees the memory used by connectomeNode.l6S
	connectomeNode.l6S = NULL;

	for ( i = 0; i < forwardClusters; i++ )
	{
		H1_forwardGroupArrayCluster_1[i].setSynapticWeights(connectomeNode.l23FGAW[i]);	// sets the forward group array cluster [i] synaptic weights
		free(connectomeNode.l23FGAW[i]);						// frees the memory used by connectomeNode.l23FGAW[i]
		connectomeNode.l23FGAW[i] = NULL;

		H1_forwardGroupArrayCluster_1[i].setStrongUnits(connectomeNode.l23FGAS[i]);	// sets the forward group array cluster [i] strong units weights
		free(connectomeNode.l23FGAS[i]);						// frees the memory used by connectomeNode.l23FGAS[i]
		connectomeNode.l23FGAS[i] = NULL;
	}

	for ( i = 0; i < backwardClusters; i++ )
	{
		H1_backwardGroupArrayCluster_1[i].setSynapticWeights(connectomeNode.l23BGAW[i]);// sets the backward group array cluster [i] synaptic weights
		free(connectomeNode.l23BGAW[i]);						// frees the memory used by connectomeNode.l23BGAW[i]
		connectomeNode.l23BGAW[i] = NULL;

		H1_backwardGroupArrayCluster_1[i].setStrongUnits(connectomeNode.l23BGAS[i]);	// sets the backward group array cluster [i] strong units weights
		free(connectomeNode.l23BGAS[i]);						// frees the memory used by connectomeNode.l23BGAS[i]
		connectomeNode.l23BGAS[i] = NULL;
	}

	// Cortical Node number 2

	connectomeNode = loadConnectome(1, 2);							// loads the connectome

	H1_layer4Cluster_2.setSynapticWeights(connectomeNode.l4W);				// sets the layer 4 cluster synaptic weights
	free(connectomeNode.l4W);								// frees the memory used by connectomeNode.l4W
	connectomeNode.l4W = NULL;

	H1_layer4Cluster_2.setPredictionSynapsis(connectomeNode.l4P);				// sets the layer 4 cluster prediction synaptic weights
	free(connectomeNode.l4P);								// frees the memory used by connectomeNode.l4P
	connectomeNode.l4P = NULL;

	H1_layer4Cluster_2.setStrongUnits(connectomeNode.l4S);					// sets the layer 4 cluster strong units synaptic weights
	free(connectomeNode.l4S);								// frees the memory used by connectomeNode.l4S
	connectomeNode.l4S = NULL;

	H1_layer5Cluster_2.setSynapticWeights(connectomeNode.l5W);				// sets the layer 5 cluster synaptic weights
	free(connectomeNode.l5W);								// frees the memory used by connectomeNode.l5W
	connectomeNode.l5W = NULL;

	H1_layer5Cluster_2.setStrongUnits(connectomeNode.l5S);					// sets the layer 5 cluster strong units synaptic weights
	free(connectomeNode.l5S);								// frees the memory used by connectomeNode.l5S
	connectomeNode.l5S = NULL;

	H1_layer6Cluster_2.setSynapticWeights(connectomeNode.l6W);				// sets the layer 6 cluster synaptic weights
	free(connectomeNode.l6W);								// frees the memory used by connectomeNode.l6W
	connectomeNode.l6W = NULL;

	H1_layer6Cluster_2.setStrongUnits(connectomeNode.l6S);					// sets the layer 6 cluster strong units synaptic weights
	free(connectomeNode.l6S);								// frees the memory used by connectomeNode.l6S
	connectomeNode.l6S = NULL;

	for ( i = 0; i < forwardClusters; i++ )
	{
		H1_forwardGroupArrayCluster_2[i].setSynapticWeights(connectomeNode.l23FGAW[i]);	// sets the forward group array cluster [i] synaptic weights
		free(connectomeNode.l23FGAW[i]);						// frees the memory used by connectomeNode.l23FGAW[i]
		connectomeNode.l23FGAW[i] = NULL;

		H1_forwardGroupArrayCluster_2[i].setStrongUnits(connectomeNode.l23FGAS[i]);	// sets the forward group array cluster [i] strong units weights
		free(connectomeNode.l23FGAS[i]);						// frees the memory used by connectomeNode.l23FGAS[i]
		connectomeNode.l23FGAS[i] = NULL;
	}

	for ( i = 0; i < backwardClusters; i++ )
	{
		H1_backwardGroupArrayCluster_2[i].setSynapticWeights(connectomeNode.l23BGAW[i]);// sets the backward group array cluster [i] synaptic weights
		free(connectomeNode.l23BGAW[i]);						// frees the memory used by connectomeNode.l23BGAW[i]
		connectomeNode.l23BGAW[i] = NULL;

		H1_backwardGroupArrayCluster_2[i].setStrongUnits(connectomeNode.l23BGAS[i]);	// sets the backward group array cluster [i] strong units weights
		free(connectomeNode.l23BGAS[i]);						// frees the memory used by connectomeNode.l23BGAS[i]
		connectomeNode.l23BGAS[i] = NULL;
	}

	// Cortical Node number 3

	connectomeNode = loadConnectome(1, 3);							// loads the connectome

	H1_layer4Cluster_3.setSynapticWeights(connectomeNode.l4W);				// sets the layer 4 cluster synaptic weights
	free(connectomeNode.l4W);								// frees the memory used by connectomeNode.l4W
	connectomeNode.l4W = NULL;

	H1_layer4Cluster_3.setPredictionSynapsis(connectomeNode.l4P);				// sets the layer 4 cluster prediction synaptic weights
	free(connectomeNode.l4P);								// frees the memory used by connectomeNode.l4P
	connectomeNode.l4P = NULL;

	H1_layer4Cluster_3.setStrongUnits(connectomeNode.l4S);					// sets the layer 4 cluster strong units synaptic weights
	free(connectomeNode.l4S);								// frees the memory used by connectomeNode.l4S
	connectomeNode.l4S = NULL;

	H1_layer5Cluster_3.setSynapticWeights(connectomeNode.l5W);				// sets the layer 5 cluster synaptic weights
	free(connectomeNode.l5W);								// frees the memory used by connectomeNode.l5W
	connectomeNode.l5W = NULL;

	H1_layer5Cluster_3.setStrongUnits(connectomeNode.l5S);					// sets the layer 5 cluster strong units synaptic weights
	free(connectomeNode.l5S);								// frees the memory used by connectomeNode.l5S
	connectomeNode.l5S = NULL;

	H1_layer6Cluster_3.setSynapticWeights(connectomeNode.l6W);				// sets the layer 6 cluster synaptic weights
	free(connectomeNode.l6W);								// frees the memory used by connectomeNode.l6W
	connectomeNode.l6W = NULL;

	H1_layer6Cluster_3.setStrongUnits(connectomeNode.l6S);					// sets the layer 6 cluster strong units synaptic weights
	free(connectomeNode.l6S);								// frees the memory used by connectomeNode.l6S
	connectomeNode.l6S = NULL;

	for ( i = 0; i < forwardClusters; i++ )
	{
		H1_forwardGroupArrayCluster_3[i].setSynapticWeights(connectomeNode.l23FGAW[i]);	// sets the forward group array cluster [i] synaptic weights
		free(connectomeNode.l23FGAW[i]);						// frees the memory used by connectomeNode.l23FGAW[i]
		connectomeNode.l23FGAW[i] = NULL;

		H1_forwardGroupArrayCluster_3[i].setStrongUnits(connectomeNode.l23FGAS[i]);	// sets the forward group array cluster [i] strong units weights
		free(connectomeNode.l23FGAS[i]);						// frees the memory used by connectomeNode.l23FGAS[i]
		connectomeNode.l23FGAS[i] = NULL;
	}

	for ( i = 0; i < backwardClusters; i++ )
	{
		H1_backwardGroupArrayCluster_3[i].setSynapticWeights(connectomeNode.l23BGAW[i]);// sets the backward group array cluster [i] synaptic weights
		free(connectomeNode.l23BGAW[i]);						// frees the memory used by connectomeNode.l23BGAW[i]
		connectomeNode.l23BGAW[i] = NULL;

		H1_backwardGroupArrayCluster_3[i].setStrongUnits(connectomeNode.l23BGAS[i]);	// sets the backward group array cluster [i] strong units weights
		free(connectomeNode.l23BGAS[i]);						// frees the memory used by connectomeNode.l23BGAS[i]
		connectomeNode.l23BGAS[i] = NULL;
	}

	// Cortical Node number 4

	connectomeNode = loadConnectome(1, 4);							// loads the connectome

	H1_layer4Cluster_4.setSynapticWeights(connectomeNode.l4W);				// sets the layer 4 cluster synaptic weights
	free(connectomeNode.l4W);								// frees the memory used by connectomeNode.l4W
	connectomeNode.l4W = NULL;

	H1_layer4Cluster_4.setPredictionSynapsis(connectomeNode.l4P);				// sets the layer 4 cluster prediction synaptic weights
	free(connectomeNode.l4P);								// frees the memory used by connectomeNode.l4P
	connectomeNode.l4P = NULL;

	H1_layer4Cluster_4.setStrongUnits(connectomeNode.l4S);					// sets the layer 4 cluster strong units synaptic weights
	free(connectomeNode.l4S);								// frees the memory used by connectomeNode.l4S
	connectomeNode.l4S = NULL;

	H1_layer5Cluster_4.setSynapticWeights(connectomeNode.l5W);				// sets the layer 5 cluster synaptic weights
	free(connectomeNode.l5W);								// frees the memory used by connectomeNode.l5W
	connectomeNode.l5W = NULL;

	H1_layer5Cluster_4.setStrongUnits(connectomeNode.l5S);					// sets the layer 5 cluster strong units synaptic weights
	free(connectomeNode.l5S);								// frees the memory used by connectomeNode.l5S
	connectomeNode.l5S = NULL;

	H1_layer6Cluster_4.setSynapticWeights(connectomeNode.l6W);				// sets the layer 6 cluster synaptic weights
	free(connectomeNode.l6W);								// frees the memory used by connectomeNode.l6W
	connectomeNode.l6W = NULL;

	H1_layer6Cluster_4.setStrongUnits(connectomeNode.l6S);					// sets the layer 6 cluster strong units synaptic weights
	free(connectomeNode.l6S);								// frees the memory used by connectomeNode.l6S
	connectomeNode.l6S = NULL;

	for ( i = 0; i < forwardClusters; i++ )
	{
		H1_forwardGroupArrayCluster_4[i].setSynapticWeights(connectomeNode.l23FGAW[i]);	// sets the forward group array cluster [i] synaptic weights
		free(connectomeNode.l23FGAW[i]);						// frees the memory used by connectomeNode.l23FGAW[i]
		connectomeNode.l23FGAW[i] = NULL;

		H1_forwardGroupArrayCluster_4[i].setStrongUnits(connectomeNode.l23FGAS[i]);	// sets the forward group array cluster [i] strong units weights
		free(connectomeNode.l23FGAS[i]);						// frees the memory used by connectomeNode.l23FGAS[i]
		connectomeNode.l23FGAS[i] = NULL;
	}

	for ( i = 0; i < backwardClusters; i++ )
	{
		H1_backwardGroupArrayCluster_4[i].setSynapticWeights(connectomeNode.l23BGAW[i]);// sets the backward group array cluster [i] synaptic weights
		free(connectomeNode.l23BGAW[i]);						// frees the memory used by connectomeNode.l23BGAW[i]
		connectomeNode.l23BGAW[i] = NULL;

		H1_backwardGroupArrayCluster_4[i].setStrongUnits(connectomeNode.l23BGAS[i]);	// sets the backward group array cluster [i] strong units weights
		free(connectomeNode.l23BGAS[i]);						// frees the memory used by connectomeNode.l23BGAS[i]
		connectomeNode.l23BGAS[i] = NULL;
	}

	// This is for the second hierarchical layer

	forwardClusters = H2_CLUSTERSPERFORWARDGROUP;
	backwardClusters = H2_CLUSTERSPERBACKWARDGROUP;

	// Cortical Node number 1

	connectomeNode = loadConnectome(2, 1);							// loads the connectome

	H2_layer4Cluster_1.setSynapticWeights(connectomeNode.l4W);				// sets the layer 4 cluster synaptic weights
	free(connectomeNode.l4W);								// frees the memory used by connectomeNode.l4W
	connectomeNode.l4W = NULL;

	H2_layer4Cluster_1.setPredictionSynapsis(connectomeNode.l4P);				// sets the layer 4 cluster prediction synaptic weights
	free(connectomeNode.l4P);								// frees the memory used by connectomeNode.l4P
	connectomeNode.l4P = NULL;

	H2_layer4Cluster_1.setStrongUnits(connectomeNode.l4S);					// sets the layer 4 cluster strong units synaptic weights
	free(connectomeNode.l4S);								// frees the memory used by connectomeNode.l4S
	connectomeNode.l4S = NULL;

	H2_layer5Cluster_1.setSynapticWeights(connectomeNode.l5W);				// sets the layer 5 cluster synaptic weights
	free(connectomeNode.l5W);								// frees the memory used by connectomeNode.l5W
	connectomeNode.l5W = NULL;

	H2_layer5Cluster_1.setStrongUnits(connectomeNode.l5S);					// sets the layer 5 cluster strong units synaptic weights
	free(connectomeNode.l5S);								// frees the memory used by connectomeNode.l5S
	connectomeNode.l5S = NULL;

	H2_layer6Cluster_1.setSynapticWeights(connectomeNode.l6W);				// sets the layer 6 cluster synaptic weights
	free(connectomeNode.l6W);								// frees the memory used by connectomeNode.l6W
	connectomeNode.l6W = NULL;

	H2_layer6Cluster_1.setStrongUnits(connectomeNode.l6S);					// sets the layer 6 cluster strong units synaptic weights
	free(connectomeNode.l6S);								// frees the memory used by connectomeNode.l6S
	connectomeNode.l6S = NULL;

	for ( i = 0; i < forwardClusters; i++ )
	{
		H2_forwardGroupArrayCluster_1[i].setSynapticWeights(connectomeNode.l23FGAW[i]);	// sets the forward group array cluster [i] synaptic weights
		free(connectomeNode.l23FGAW[i]);						// frees the memory used by connectomeNode.l23FGAW[i]
		connectomeNode.l23FGAW[i] = NULL;

		H2_forwardGroupArrayCluster_1[i].setStrongUnits(connectomeNode.l23FGAS[i]);	// sets the forward group array cluster [i] strong units weights
		free(connectomeNode.l23FGAS[i]);						// frees the memory used by connectomeNode.l23FGAS[i]
		connectomeNode.l23FGAS[i] = NULL;
	}

	for ( i = 0; i < backwardClusters; i++ )
	{
		H2_backwardGroupArrayCluster_1[i].setSynapticWeights(connectomeNode.l23BGAW[i]);// sets the backward group array cluster [i] synaptic weights
		free(connectomeNode.l23BGAW[i]);						// frees the memory used by connectomeNode.l23BGAW[i]
		connectomeNode.l23BGAW[i] = NULL;

		H2_backwardGroupArrayCluster_1[i].setStrongUnits(connectomeNode.l23BGAS[i]);	// sets the backward group array cluster [i] strong units weights
		free(connectomeNode.l23BGAS[i]);						// frees the memory used by connectomeNode.l23BGAS[i]
		connectomeNode.l23BGAS[i] = NULL;
	}

	// Cortical Node number 2

	connectomeNode = loadConnectome(2, 2);							// loads the connectome

	H2_layer4Cluster_2.setSynapticWeights(connectomeNode.l4W);				// sets the layer 4 cluster synaptic weights
	free(connectomeNode.l4W);								// frees the memory used by connectomeNode.l4W
	connectomeNode.l4W = NULL;

	H2_layer4Cluster_2.setPredictionSynapsis(connectomeNode.l4P);				// sets the layer 4 cluster prediction synaptic weights
	free(connectomeNode.l4P);								// frees the memory used by connectomeNode.l4P
	connectomeNode.l4P = NULL;

	H2_layer4Cluster_2.setStrongUnits(connectomeNode.l4S);					// sets the layer 4 cluster strong units synaptic weights
	free(connectomeNode.l4S);								// frees the memory used by connectomeNode.l4S
	connectomeNode.l4S = NULL;

	H2_layer5Cluster_2.setSynapticWeights(connectomeNode.l5W);				// sets the layer 5 cluster synaptic weights
	free(connectomeNode.l5W);								// frees the memory used by connectomeNode.l5W
	connectomeNode.l5W = NULL;

	H2_layer5Cluster_2.setStrongUnits(connectomeNode.l5S);					// sets the layer 5 cluster strong units synaptic weights
	free(connectomeNode.l5S);								// frees the memory used by connectomeNode.l5S
	connectomeNode.l5S = NULL;

	H2_layer6Cluster_2.setSynapticWeights(connectomeNode.l6W);				// sets the layer 6 cluster synaptic weights
	free(connectomeNode.l6W);								// frees the memory used by connectomeNode.l6W
	connectomeNode.l6W = NULL;

	H2_layer6Cluster_2.setStrongUnits(connectomeNode.l6S);					// sets the layer 6 cluster strong units synaptic weights
	free(connectomeNode.l6S);								// frees the memory used by connectomeNode.l6S
	connectomeNode.l6S = NULL;

	for ( i = 0; i < forwardClusters; i++ )
	{
		H2_forwardGroupArrayCluster_2[i].setSynapticWeights(connectomeNode.l23FGAW[i]);	// sets the forward group array cluster [i] synaptic weights
		free(connectomeNode.l23FGAW[i]);						// frees the memory used by connectomeNode.l23FGAW[i]
		connectomeNode.l23FGAW[i] = NULL;

		H2_forwardGroupArrayCluster_2[i].setStrongUnits(connectomeNode.l23FGAS[i]);	// sets the forward group array cluster [i] strong units weights
		free(connectomeNode.l23FGAS[i]);						// frees the memory used by connectomeNode.l23FGAS[i]
		connectomeNode.l23FGAS[i] = NULL;
	}

	for ( i = 0; i < backwardClusters; i++ )
	{
		H2_backwardGroupArrayCluster_2[i].setSynapticWeights(connectomeNode.l23BGAW[i]);// sets the backward group array cluster [i] synaptic weights
		free(connectomeNode.l23BGAW[i]);						// frees the memory used by connectomeNode.l23BGAW[i]
		connectomeNode.l23BGAW[i] = NULL;

		H2_backwardGroupArrayCluster_2[i].setStrongUnits(connectomeNode.l23BGAS[i]);	// sets the backward group array cluster [i] strong units weights
		free(connectomeNode.l23BGAS[i]);						// frees the memory used by connectomeNode.l23BGAS[i]
		connectomeNode.l23BGAS[i] = NULL;
	}

	// This is for the third hierarchical layer

	forwardClusters = H3_CLUSTERSPERFORWARDGROUP;
	backwardClusters = H3_CLUSTERSPERBACKWARDGROUP;

	// Cortical Node number 1

	connectomeNode = loadConnectome(3, 1);							// loads the connectome

	H3_layer4Cluster_1.setSynapticWeights(connectomeNode.l4W);				// sets the layer 4 cluster synaptic weights
	free(connectomeNode.l4W);								// frees the memory used by connectomeNode.l4W
	connectomeNode.l4W = NULL;

	H3_layer4Cluster_1.setPredictionSynapsis(connectomeNode.l4P);				// sets the layer 4 cluster prediction synaptic weights
	free(connectomeNode.l4P);								// frees the memory used by connectomeNode.l4P
	connectomeNode.l4P = NULL;

	H3_layer4Cluster_1.setStrongUnits(connectomeNode.l4S);					// sets the layer 4 cluster strong units synaptic weights
	free(connectomeNode.l4S);								// frees the memory used by connectomeNode.l4S
	connectomeNode.l4S = NULL;

	H3_layer5Cluster_1.setSynapticWeights(connectomeNode.l5W);				// sets the layer 5 cluster synaptic weights
	free(connectomeNode.l5W);								// frees the memory used by connectomeNode.l5W
	connectomeNode.l5W = NULL;

	H3_layer5Cluster_1.setStrongUnits(connectomeNode.l5S);					// sets the layer 5 cluster strong units synaptic weights
	free(connectomeNode.l5S);								// frees the memory used by connectomeNode.l5S
	connectomeNode.l5S = NULL;

	H3_layer6Cluster_1.setSynapticWeights(connectomeNode.l6W);				// sets the layer 6 cluster synaptic weights
	free(connectomeNode.l6W);								// frees the memory used by connectomeNode.l6W
	connectomeNode.l6W = NULL;

	H3_layer6Cluster_1.setStrongUnits(connectomeNode.l6S);					// sets the layer 6 cluster strong units synaptic weights
	free(connectomeNode.l6S);								// frees the memory used by connectomeNode.l6S
	connectomeNode.l6S = NULL;

	for ( i = 0; i < forwardClusters; i++ )
	{
		H3_forwardGroupArrayCluster_1[i].setSynapticWeights(connectomeNode.l23FGAW[i]);	// sets the forward group array cluster [i] synaptic weights
		free(connectomeNode.l23FGAW[i]);						// frees the memory used by connectomeNode.l23FGAW[i]
		connectomeNode.l23FGAW[i] = NULL;

		H3_forwardGroupArrayCluster_1[i].setStrongUnits(connectomeNode.l23FGAS[i]);	// sets the forward group array cluster [i] strong units weights
		free(connectomeNode.l23FGAS[i]);						// frees the memory used by connectomeNode.l23FGAS[i]
		connectomeNode.l23FGAS[i] = NULL;
	}

	for ( i = 0; i < backwardClusters; i++ )
	{
		H3_backwardGroupArrayCluster_1[i].setSynapticWeights(connectomeNode.l23BGAW[i]);// sets the backward group array cluster [i] synaptic weights
		free(connectomeNode.l23BGAW[i]);						// frees the memory used by connectomeNode.l23BGAW[i]
		connectomeNode.l23BGAW[i] = NULL;

		H3_backwardGroupArrayCluster_1[i].setStrongUnits(connectomeNode.l23BGAS[i]);	// sets the backward group array cluster [i] strong units weights
		free(connectomeNode.l23BGAS[i]);						// frees the memory used by connectomeNode.l23BGAS[i]
		connectomeNode.l23BGAS[i] = NULL;
	}

}

////////////////////////////////////////// cortical region connectome loaded






////////////////////////////////////////// loads the cortical region inputs data

inputData	trainingData;

trainingData = loadInputData();									// loads the input data to train the model

printf("\nThe training data to feed the model have been loaded correctly.\n\n");

////////////////////////////////////////// cortical region inputs data loaded






////////////////////////////////////////// propagates the training data through the node

int j, it, k, iterations, index, opt, opt1, enableLearning;
int hierarchicalLayer, node;
int units, inputs;
bool cinch;
char firingOn;
double threshold;

threshold = 0.01;
iterations = 0;
enableLearning = 0;

iterations = iterations + 1;

if ( enableLearning != 0 )
{
	printf("\nYou have to know the following information before proceed:\n\n%d vectors of %d components will be used to train the Cortical Region\n", iterations*trainingData.chunks, trainingData.filters);
	printf("\nDo you want to proceed with the training process? (Y) for yes and any key for no\n\n");
}
else
{
	printf("\nYou have to know the following information before proceed:\n\n%d vectors of %d components will be used to propagate through the Cortical Region\n", iterations*trainingData.chunks, trainingData.filters);
	printf("\nDo you want to proceed with the propagation process? (Y) for yes and any key for no\n\n");
}

scanf(" %c", &option);

int	clustersPerForwardGroup1, clustersPerForwardGroup2, clustersPerForwardGroup3;
int	clustersPerBackwardGroup1, clustersPerBackwardGroup2, clustersPerBackwardGroup3, n;
bool	*vector1;
double	*input, *vector, *outputFiring, *weightsCluster, *unitsFiring;
double	*energy;
double	firingNeighbor = 0.5+0;

neuromodulators hierarchy1[4], hierarchy2[2], hierarchy3;

if ( option == 'Y' )
{

	// This is for the first hierarchical layer neuromodulators.
	for (i = 0; i < 4; i++)
	{
		if ( enableLearning != 0 )
		{
			hierarchy1[i].layer4.enableLearning = 1;
		}
		else
		{
			hierarchy1[i].layer4.enableLearning = 0;
		}

		hierarchy1[i].layer4.predictiveEnableLearning = false;
		hierarchy1[i].layer4.sharpThreshold = 0;
		hierarchy1[i].layer4.averageThreshold = 10000;
		hierarchy1[i].layer4.learningRate = 0.01;
		hierarchy1[i].layer4.learningNeighbor = 0.05;
		hierarchy1[i].layer4.firingNeighbor = firingNeighbor+0;
		hierarchy1[i].layer4.predictionThreshold = 0;

		hierarchy1[i].layer5 = hierarchy1[i].layer4;
		hierarchy1[i].layer6 = hierarchy1[i].layer4;
		hierarchy1[i].layer23FG = hierarchy1[i].layer4;
		hierarchy1[i].layer23BG = hierarchy1[i].layer4;

		//*
		if ( enableLearning != 0 )
		{
			hierarchy1[i].layer4.predictiveEnableLearning = false;
			hierarchy1[i].layer4.enableLearning = 0;
			hierarchy1[i].layer23FG.enableLearning = 0;
			hierarchy1[i].layer23BG.enableLearning = 0;
			hierarchy1[i].layer5.enableLearning = 0;
			hierarchy1[i].layer6.enableLearning = 0;
		}
		//*/
	}

	// This is for the second hierarchical layer neuromodulators.
	for (i = 0; i < 2; i++)
	{
		if ( enableLearning != 0 )
		{
			hierarchy2[i].layer4.enableLearning = 1;
		}
		else
		{
			hierarchy2[i].layer4.enableLearning = 0;
		}

		hierarchy2[i].layer4.predictiveEnableLearning = false;
		hierarchy2[i].layer4.sharpThreshold = 0.01;
		hierarchy2[i].layer4.averageThreshold = 10000;
		hierarchy2[i].layer4.learningRate = 0.01;
		hierarchy2[i].layer4.learningNeighbor = 0.05;
		hierarchy2[i].layer4.firingNeighbor = firingNeighbor+0;
		hierarchy2[i].layer4.predictionThreshold = 0;

		hierarchy2[i].layer5 = hierarchy2[i].layer4;
		hierarchy2[i].layer6 = hierarchy2[i].layer4;
		hierarchy2[i].layer23FG = hierarchy2[i].layer4;
		hierarchy2[i].layer23BG = hierarchy2[i].layer4;

		//*
		if ( enableLearning != 0 )
		{
			hierarchy2[i].layer4.predictiveEnableLearning = false;
			hierarchy2[i].layer4.enableLearning = 0;
			hierarchy2[i].layer23FG.enableLearning = 0;
			hierarchy2[i].layer23BG.enableLearning = 0;
			hierarchy2[i].layer5.enableLearning = 0;
			hierarchy2[i].layer6.enableLearning = 0;
		}
		//*/
	}

	// This is for the third hierarchical layer neuromodulators.
	if ( enableLearning != 0 )
	{
		hierarchy3.layer4.enableLearning = 1;
	}
	else
	{
		hierarchy3.layer4.enableLearning = 0;
	}

	hierarchy3.layer4.predictiveEnableLearning = false;
	hierarchy3.layer4.sharpThreshold = 0.01;
	hierarchy3.layer4.averageThreshold = 10000;
	hierarchy3.layer4.learningRate = 0.01;
	hierarchy3.layer4.learningNeighbor = 0.05;
	hierarchy3.layer4.firingNeighbor = firingNeighbor+0;
	hierarchy3.layer4.predictionThreshold = 0;

	hierarchy3.layer5 = hierarchy3.layer4;
	hierarchy3.layer6 = hierarchy3.layer4;
	hierarchy3.layer23FG = hierarchy3.layer4;
	hierarchy3.layer23BG = hierarchy3.layer4;

	//*
	if ( enableLearning != 0 )
	{
		hierarchy3.layer4.predictiveEnableLearning = false;
		hierarchy3.layer4.enableLearning = 0;
		hierarchy3.layer23FG.enableLearning = 0;
		hierarchy3.layer23BG.enableLearning = 0;
		hierarchy3.layer5.enableLearning = 0;
		hierarchy3.layer6.enableLearning = 0;
	}
	//*/

	input = (double*)calloc(trainingData.filters, sizeof(double));				// reserves space for the input vector
	energy = (double*)calloc(iterations*trainingData.chunks, sizeof(double));		// reserves space for the input vector


	clustersPerForwardGroup1 = H1_CLUSTERSPERFORWARDGROUP;
	clustersPerBackwardGroup1 = H1_CLUSTERSPERBACKWARDGROUP;

	clustersPerForwardGroup2 = H2_CLUSTERSPERFORWARDGROUP;
	clustersPerBackwardGroup2 = H2_CLUSTERSPERBACKWARDGROUP;

	clustersPerForwardGroup3 = H3_CLUSTERSPERFORWARDGROUP;
	clustersPerBackwardGroup3 = H3_CLUSTERSPERBACKWARDGROUP;

	if ( enableLearning == 0 )
	{
		printf("\nDo you want to save the firing activity inside the HSTM?\n(Y) for yes and any key for no\n\n");
		scanf(" %c", &firingOn);
	}
	else
	{
		firingOn = 'n';
	}

	int H1_CC1_CL4_unitsDim, H1_CC1_CL4_outputDim, H1_CC1_CL23_FG_unitsDim[clustersPerForwardGroup1], H1_CC1_CL23_FG_outputDim[clustersPerForwardGroup1];
	int H1_CC2_CL4_unitsDim, H1_CC2_CL4_outputDim, H1_CC2_CL23_FG_unitsDim[clustersPerForwardGroup1], H1_CC2_CL23_FG_outputDim[clustersPerForwardGroup1];
	int H1_CC3_CL4_unitsDim, H1_CC3_CL4_outputDim, H1_CC3_CL23_FG_unitsDim[clustersPerForwardGroup1], H1_CC3_CL23_FG_outputDim[clustersPerForwardGroup1];
	int H1_CC4_CL4_unitsDim, H1_CC4_CL4_outputDim, H1_CC4_CL23_FG_unitsDim[clustersPerForwardGroup1], H1_CC4_CL23_FG_outputDim[clustersPerForwardGroup1];

	int H1_CC1_CL4_Prediction_Fault = 0;
	int H1_CC2_CL4_Prediction_Fault = 0;
	int H1_CC3_CL4_Prediction_Fault = 0;
	int H1_CC4_CL4_Prediction_Fault = 0;

	int H2_CC1_CL4_unitsDim, H2_CC1_CL4_outputDim, H2_CC1_CL23_FG_unitsDim[clustersPerForwardGroup2], H2_CC1_CL23_FG_outputDim[clustersPerForwardGroup2];
	int H2_CC2_CL4_unitsDim, H2_CC2_CL4_outputDim, H2_CC2_CL23_FG_unitsDim[clustersPerForwardGroup2], H2_CC2_CL23_FG_outputDim[clustersPerForwardGroup2];

	int H2_CC1_CL4_Prediction_Fault = 0;
	int H2_CC2_CL4_Prediction_Fault = 0;

	int H3_CC1_CL4_unitsDim, H3_CC1_CL4_outputDim, H3_CC1_CL23_FG_unitsDim[clustersPerForwardGroup3], H3_CC1_CL23_FG_outputDim[clustersPerForwardGroup3];

	int H3_CC1_CL4_Prediction_Fault = 0;

	double *H1_CC1_CL4_unitsFiring, *H1_CC1_CL4_outputFiring, *H1_CC1_CL23_FG_unitsFiring[clustersPerForwardGroup1], *H1_CC1_CL23_FG_outputFiring[clustersPerForwardGroup1];
	double *H1_CC2_CL4_unitsFiring, *H1_CC2_CL4_outputFiring, *H1_CC2_CL23_FG_unitsFiring[clustersPerForwardGroup1], *H1_CC2_CL23_FG_outputFiring[clustersPerForwardGroup1];
	double *H1_CC3_CL4_unitsFiring, *H1_CC3_CL4_outputFiring, *H1_CC3_CL23_FG_unitsFiring[clustersPerForwardGroup1], *H1_CC3_CL23_FG_outputFiring[clustersPerForwardGroup1];
	double *H1_CC4_CL4_unitsFiring, *H1_CC4_CL4_outputFiring, *H1_CC4_CL23_FG_unitsFiring[clustersPerForwardGroup1], *H1_CC4_CL23_FG_outputFiring[clustersPerForwardGroup1];

	double *H2_CC1_CL4_unitsFiring, *H2_CC1_CL4_outputFiring, *H2_CC1_CL23_FG_unitsFiring[clustersPerForwardGroup2], *H2_CC1_CL23_FG_outputFiring[clustersPerForwardGroup2];
	double *H2_CC2_CL4_unitsFiring, *H2_CC2_CL4_outputFiring, *H2_CC2_CL23_FG_unitsFiring[clustersPerForwardGroup2], *H2_CC2_CL23_FG_outputFiring[clustersPerForwardGroup2];

	double *H3_CC1_CL4_unitsFiring, *H3_CC1_CL4_outputFiring, *H3_CC1_CL23_FG_unitsFiring[clustersPerForwardGroup3], *H3_CC1_CL23_FG_outputFiring[clustersPerForwardGroup3];

	index = 0;
	for (it = 0; it < iterations; it++)
	{
		printf("\nYou are here: %d\n", index);
		for (i = 0; i < trainingData.chunks; i++)
		{
//*
			if ( enableLearning == 1 )
			{

			/*	// This is for the first hierarchical layer
				for (k = 0; k < 4; k++)
				{
					//hierarchy1[k].layer4.learningNeighbor = 5 * pow(0.01,index/(iterations*trainingData.chunks-1));
					//hierarchy1[k].layer4.learningRate = 0.9 * pow((0.01/0.9),index/(iterations*trainingData.chunks-1));
					//hierarchy1[k].layer4.firingNeighbor = 0.5*pow((0.25/0.5),index/(iterations*trainingData.chunks-1));

					//hierarchy1[k].layer5.learningNeighbor = hierarchy1[k].layer4.learningNeighbor;
					//hierarchy1[k].layer5.learningRate = hierarchy1[k].layer4.learningRate;
					//hierarchy1[k].layer5.firingNeighbor = hierarchy1[k].layer4.firingNeighbor;

					//hierarchy1[k].layer6.learningNeighbor = hierarchy1[k].layer4.learningNeighbor;
					//hierarchy1[k].layer6.learningRate = hierarchy1[k].layer4.learningRate;
					//hierarchy1[k].layer6.firingNeighbor = hierarchy1[k].layer4.firingNeighbor;

					//hierarchy1[k].layer23FG.learningNeighbor = 5 * pow(0.01,index/(iterations*trainingData.chunks-1));
					//hierarchy1[k].layer23FG.learningRate = 0.9 * pow((0.01/0.9),index/(iterations*trainingData.chunks-1));
					//hierarchy1[k].layer23FG.firingNeighbor = hierarchy1[k].layer4.firingNeighbor;

					//hierarchy1[k].layer23BG.learningNeighbor = hierarchy1[k].layer4.learningNeighbor;
					//hierarchy1[k].layer23BG.learningRate = hierarchy1[k].layer4.learningRate;
					//hierarchy1[k].layer23BG.firingNeighbor = hierarchy1[k].layer4.firingNeighbor;
				} //*/

			/*	// This is for the second hierarchical layer
				for (k = 0; k < 2; k++)
				{
					//hierarchy2[k].layer4.learningNeighbor = 5 * pow(0.01,index/(iterations*trainingData.chunks-1));
					//hierarchy2[k].layer4.learningRate = 0.9 * pow((0.01/0.9),index/(iterations*trainingData.chunks-1));
					//hierarchy2[k].layer4.firingNeighbor = 0.5*pow((0.25/0.5),index/(iterations*trainingData.chunks-1));

					//hierarchy2[k].layer5.learningNeighbor = hierarchy2[k].layer4.learningNeighbor;
					//hierarchy2[k].layer5.learningRate = hierarchy2[k].layer4.learningRate;
					//hierarchy2[k].layer5.firingNeighbor = hierarchy2[k].layer4.firingNeighbor;

					//hierarchy2[k].layer6.learningNeighbor = hierarchy2[k].layer4.learningNeighbor;
					//hierarchy2[k].layer6.learningRate = hierarchy2[k].layer4.learningRate;
					//hierarchy2[k].layer6.firingNeighbor = hierarchy2[k].layer4.firingNeighbor;

					//hierarchy2[k].layer23FG.learningNeighbor = 5 * pow(0.01,index/(iterations*trainingData.chunks-1));;
					//hierarchy2[k].layer23FG.learningRate = 0.9 * pow((0.01/0.9),index/(iterations*trainingData.chunks-1));
					//hierarchy2[k].layer23FG.firingNeighbor = hierarchy2[k].layer4.firingNeighbor;

					//hierarchy2[k].layer23BG.learningNeighbor = hierarchy2[k].layer4.learningNeighbor;
					//hierarchy2[k].layer23BG.learningRate = hierarchy2[k].layer4.learningRate;
					//hierarchy2[k].layer23BG.firingNeighbor = hierarchy2[k].layer4.firingNeighbor;
				} //*/

			/*	// This is for the third hierarchical layer
				//hierarchy3.layer4.learningNeighbor = 5 * pow(0.01,index/(iterations*trainingData.chunks-1));
				//hierarchy3.layer4.learningRate = 0.9 * pow((0.01/0.9),index/(iterations*trainingData.chunks-1));
				//hierarchy3.layer4.firingNeighbor = 0.5*pow((0.25/0.5),index/(iterations*trainingData.chunks-1));

				//hierarchy3.layer5.learningNeighbor = hierarchy3.layer4.learningNeighbor;
				//hierarchy3.layer5.learningRate = hierarchy3.layer4.learningRate;
				//hierarchy3.layer5.firingNeighbor = hierarchy3.layer4.firingNeighbor;

				//hierarchy3.layer6.learningNeighbor = hierarchy3.layer4.learningNeighbor;
				//hierarchy3.layer6.learningRate = hierarchy3.layer4.learningRate;
				//hierarchy3.layer6.firingNeighbor = hierarchy3.layer4.firingNeighbor;

				//hierarchy3.layer23FG.learningNeighbor = 5 * pow(0.01,index/(iterations*trainingData.chunks-1));
				//hierarchy3.layer23FG.learningRate = 0.9 * pow((0.01/0.9),index/(iterations*trainingData.chunks-1));
				//hierarchy3.layer23FG.firingNeighbor = hierarchy3.layer4.firingNeighbor;

				//hierarchy3.layer23BG.learningNeighbor = hierarchy3.layer4.learningNeighbor;
				//hierarchy3.layer23BG.learningRate = hierarchy3.layer4.learningRate;
				//hierarchy3.layer23BG.firingNeighbor = hierarchy3.layer4.firingNeighbor; //*/

			}
//*/
			cinch = false;
			for (j = 0; j < trainingData.filters; j++)
			{
				*(input + j) = *((trainingData.inputs + i*trainingData.filters) + j);

				if ( (*(input + j) >= threshold) )
					cinch = true;
			}

			if ( cinch )
			{
				for (n = 0; n < 4; n++)
				{
					hierarchy1[n].layer4.sharpThreshold = 0;
					hierarchy1[n].layer6.sharpThreshold = 0;
				}
			}
			else
			{
				for (n = 0; n < 4; n++)
				{
					hierarchy1[n].layer4.sharpThreshold = 1000;
					hierarchy1[n].layer6.sharpThreshold = 1000;
				}
			}
			region.setInputSynchronization(true);					// sets the synchronization of the region
			region.setInputVector(input);						// sets the input vector of the region
			vector = region.getOutputVector();					// gets the output vector of the region

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// This is the array of neuromodulators
			neuromodulators	modulators[7];

			// First hierarchical layer of neuromodulators
			for (k = 0; k < 4; k++)
			{
				modulators[k] = hierarchy1[k];
			}

			// Second hierarchical layer of neuromodulators
			for (k = 0; k < 2; k++)
			{
				modulators[k+4] = hierarchy2[k];
			}

			// Third hierarchical layer of neuromodulators
			modulators[6] = hierarchy3;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			region.propagateSignals( modulators );				// propagates the signals through the node
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			int accum;
			if ( firingOn == 'Y' && enableLearning == 0 )
			{
				//////////////////////////////////////////////////////////////////////////////////
				////////////////// This is for the hierarchical layer 1 (H1) /////////////////////
				//////////////////////////////////////////////////////////////////////////////////

					//////////////////////////////////////////////////////////////////////////////////
					////////////////// This is for the cortical column 1 (CC1) ///////////////////////
					//////////////////////////////////////////////////////////////////////////////////

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 4 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						if ( index == 0 )					// reserves space for the units firing vector
						{
							H1_CC1_CL4_unitsDim = H1_layer4Cluster_1.getUnitsDimensionality();// gets units dimensionality
							H1_CC1_CL4_outputDim = H1_layer4Cluster_1.getOutputDimensionality();// gets output dimensionality
							H1_CC1_CL4_unitsFiring = (double*)calloc(H1_CC1_CL4_unitsDim, sizeof(double));
							H1_CC1_CL4_outputFiring = (double*)calloc(iterations*trainingData.chunks*H1_CC1_CL4_outputDim, sizeof(double));
						}

						vector1 = H1_layer4Cluster_1.getUnitsVector();
						accum = 0;
						for (j = 0; j < H1_CC1_CL4_unitsDim; j++ )
						{
							if ( *(vector1 + j) )
							{
								accum++;
								if ( accum > 1 ) {
									H1_CC1_CL4_Prediction_Fault++;
									break;
								}
							}
						}
						if ( accum == 1 )
						{
							for (j = 0; j < H1_CC1_CL4_unitsDim; j++ )
							{
								if ( *(vector1 + j) )
								{
									*(H1_CC1_CL4_unitsFiring + j) += 1;
									break;
								}
							}
						}
						free(vector1);
						vector1 = NULL;
						vector = H1_layer4Cluster_1.getOutputVector();
						for (j = 0; j < H1_CC1_CL4_outputDim; j++ )
							*((H1_CC1_CL4_outputFiring + H1_CC1_CL4_outputDim*index) + j) = *(vector + j);

						free(vector);
						vector = NULL;

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 23 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						for (k=0; k < clustersPerForwardGroup1; k++)
						{
							if ( index == 0 )					// reserves space for the units firing vector
							{
								H1_CC1_CL23_FG_unitsDim[k] = H1_forwardGroupArrayCluster_1[k].getUnitsDimensionality();// gets units dimensionality
								H1_CC1_CL23_FG_outputDim[k] = H1_forwardGroupArrayCluster_1[k].getOutputDimensionality();// gets output dimensionality
								H1_CC1_CL23_FG_unitsFiring[k] = (double*)calloc(H1_CC1_CL23_FG_unitsDim[k], sizeof(double));
								H1_CC1_CL23_FG_outputFiring[k] = (double*)calloc(iterations*trainingData.chunks*H1_CC1_CL23_FG_outputDim[k], sizeof(double));
							}

							vector1 = H1_forwardGroupArrayCluster_1[k].getUnitsVector();
							for (j = 0; j < H1_CC1_CL23_FG_unitsDim[k]; j++ )
								if ( *(vector1 + j) )
									*(H1_CC1_CL23_FG_unitsFiring[k] + j) += 1;

							free(vector1);
							vector1 = NULL;
							vector = H1_forwardGroupArrayCluster_1[k].getOutputVector();
							for (j = 0; j < H1_CC1_CL23_FG_outputDim[k]; j++ )
								*((H1_CC1_CL23_FG_outputFiring[k] + H1_CC1_CL23_FG_outputDim[k]*index) + j) = *(vector + j);

							free(vector);
							vector = NULL;
						}

					//////////////////////////////////////////////////////////////////////////////////
					////////////////// This is for the cortical column 2 (CC2) ///////////////////////
					//////////////////////////////////////////////////////////////////////////////////

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 4 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						if ( index == 0 )					// reserves space for the units firing vector
						{
							H1_CC2_CL4_unitsDim = H1_layer4Cluster_2.getUnitsDimensionality();// gets units dimensionality
							H1_CC2_CL4_outputDim = H1_layer4Cluster_2.getOutputDimensionality();// gets output dimensionality
							H1_CC2_CL4_unitsFiring = (double*)calloc(H1_CC2_CL4_unitsDim, sizeof(double));
							H1_CC2_CL4_outputFiring = (double*)calloc(iterations*trainingData.chunks*H1_CC2_CL4_outputDim, sizeof(double));
						}

						vector1 = H1_layer4Cluster_2.getUnitsVector();
						accum = 0;
						for (j = 0; j < H1_CC2_CL4_unitsDim; j++ )
						{
							if ( *(vector1 + j) )
							{
								accum++;
								if ( accum > 1 ) {
									H1_CC2_CL4_Prediction_Fault++;
									break;
								}
							}
						}
						if ( accum == 1 )
						{
							for (j = 0; j < H1_CC2_CL4_unitsDim; j++ )
							{
								if ( *(vector1 + j) )
								{
									*(H1_CC2_CL4_unitsFiring + j) += 1;
									break;
								}
							}
						}
						free(vector1);
						vector1 = NULL;
						vector = H1_layer4Cluster_2.getOutputVector();
						for (j = 0; j < H1_CC2_CL4_outputDim; j++ )
							*((H1_CC2_CL4_outputFiring + H1_CC2_CL4_outputDim*index) + j) = *(vector + j);

						free(vector);
						vector = NULL;

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 23 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						for (k=0; k < clustersPerForwardGroup1; k++)
						{
							if ( index == 0 )					// reserves space for the units firing vector
							{
								H1_CC2_CL23_FG_unitsDim[k] = H1_forwardGroupArrayCluster_2[k].getUnitsDimensionality();// gets units dimensionality
								H1_CC2_CL23_FG_outputDim[k] = H1_forwardGroupArrayCluster_2[k].getOutputDimensionality();// gets output dimensionality
								H1_CC2_CL23_FG_unitsFiring[k] = (double*)calloc(H1_CC2_CL23_FG_unitsDim[k], sizeof(double));
								H1_CC2_CL23_FG_outputFiring[k] = (double*)calloc(iterations*trainingData.chunks*H1_CC2_CL23_FG_outputDim[k], sizeof(double));
							}

							vector1 = H1_forwardGroupArrayCluster_2[k].getUnitsVector();
							for (j = 0; j < H1_CC2_CL23_FG_unitsDim[k]; j++ )
								if ( *(vector1 + j) )
									*(H1_CC2_CL23_FG_unitsFiring[k] + j) += 1;

							free(vector1);
							vector1 = NULL;
							vector = H1_forwardGroupArrayCluster_2[k].getOutputVector();
							for (j = 0; j < H1_CC2_CL23_FG_outputDim[k]; j++ )
								*((H1_CC2_CL23_FG_outputFiring[k] + H1_CC2_CL23_FG_outputDim[k]*index) + j) = *(vector + j);

							free(vector);
							vector = NULL;
						}

					//////////////////////////////////////////////////////////////////////////////////
					////////////////// This is for the cortical column 3 (CC3) ///////////////////////
					//////////////////////////////////////////////////////////////////////////////////

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 4 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						if ( index == 0 )					// reserves space for the units firing vector
						{
							H1_CC3_CL4_unitsDim = H1_layer4Cluster_3.getUnitsDimensionality();// gets units dimensionality
							H1_CC3_CL4_outputDim = H1_layer4Cluster_3.getOutputDimensionality();// gets output dimensionality
							H1_CC3_CL4_unitsFiring = (double*)calloc(H1_CC3_CL4_unitsDim, sizeof(double));
							H1_CC3_CL4_outputFiring = (double*)calloc(iterations*trainingData.chunks*H1_CC3_CL4_outputDim, sizeof(double));
						}

						vector1 = H1_layer4Cluster_3.getUnitsVector();
						accum = 0;
						for (j = 0; j < H1_CC3_CL4_unitsDim; j++ )
						{
							if ( *(vector1 + j) )
							{
								accum++;
								if ( accum > 1 ) {
									H1_CC3_CL4_Prediction_Fault++;
									break;
								}
							}
						}
						if ( accum == 1 )
						{
							for (j = 0; j < H1_CC3_CL4_unitsDim; j++ )
							{
								if ( *(vector1 + j) )
								{
									*(H1_CC3_CL4_unitsFiring + j) += 1;
									break;
								}
							}
						}
						free(vector1);
						vector1 = NULL;
						vector = H1_layer4Cluster_3.getOutputVector();
						for (j = 0; j < H1_CC3_CL4_outputDim; j++ )
							*((H1_CC3_CL4_outputFiring + H1_CC3_CL4_outputDim*index) + j) = *(vector + j);

						free(vector);
						vector = NULL;

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 23 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						for (k=0; k < clustersPerForwardGroup1; k++)
						{
							if ( index == 0 )					// reserves space for the units firing vector
							{
								H1_CC3_CL23_FG_unitsDim[k] = H1_forwardGroupArrayCluster_3[k].getUnitsDimensionality();// gets units dimensionality
								H1_CC3_CL23_FG_outputDim[k] = H1_forwardGroupArrayCluster_3[k].getOutputDimensionality();// gets output dimensionality
								H1_CC3_CL23_FG_unitsFiring[k] = (double*)calloc(H1_CC3_CL23_FG_unitsDim[k], sizeof(double));
								H1_CC3_CL23_FG_outputFiring[k] = (double*)calloc(iterations*trainingData.chunks*H1_CC3_CL23_FG_outputDim[k], sizeof(double));
							}

							vector1 = H1_forwardGroupArrayCluster_3[k].getUnitsVector();
							for (j = 0; j < H1_CC3_CL23_FG_unitsDim[k]; j++ )
								if ( *(vector1 + j) )
									*(H1_CC3_CL23_FG_unitsFiring[k] + j) += 1;

							free(vector1);
							vector1 = NULL;
							vector = H1_forwardGroupArrayCluster_3[k].getOutputVector();
							for (j = 0; j < H1_CC3_CL23_FG_outputDim[k]; j++ )
								*((H1_CC3_CL23_FG_outputFiring[k] + H1_CC3_CL23_FG_outputDim[k]*index) + j) = *(vector + j);

							free(vector);
							vector = NULL;
						}

					//////////////////////////////////////////////////////////////////////////////////
					////////////////// This is for the cortical column 4 (CC4) ///////////////////////
					//////////////////////////////////////////////////////////////////////////////////

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 4 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						if ( index == 0 )					// reserves space for the units firing vector
						{
							H1_CC4_CL4_unitsDim = H1_layer4Cluster_4.getUnitsDimensionality();// gets units dimensionality
							H1_CC4_CL4_outputDim = H1_layer4Cluster_4.getOutputDimensionality();// gets output dimensionality
							H1_CC4_CL4_unitsFiring = (double*)calloc(H1_CC4_CL4_unitsDim, sizeof(double));
							H1_CC4_CL4_outputFiring = (double*)calloc(iterations*trainingData.chunks*H1_CC4_CL4_outputDim, sizeof(double));
						}

						vector1 = H1_layer4Cluster_4.getUnitsVector();
						accum = 0;
						for (j = 0; j < H1_CC4_CL4_unitsDim; j++ )
						{
							if ( *(vector1 + j) )
							{
								accum++;
								if ( accum > 1 ) {
									H1_CC4_CL4_Prediction_Fault++;
									break;
								}
							}
						}
						if ( accum == 1 )
						{
							for (j = 0; j < H1_CC4_CL4_unitsDim; j++ )
							{
								if ( *(vector1 + j) )
								{
									*(H1_CC4_CL4_unitsFiring + j) += 1;
									break;
								}
							}
						}
						free(vector1);
						vector1 = NULL;
						vector = H1_layer4Cluster_4.getOutputVector();
						for (j = 0; j < H1_CC4_CL4_outputDim; j++ )
							*((H1_CC4_CL4_outputFiring + H1_CC4_CL4_outputDim*index) + j) = *(vector + j);

						free(vector);
						vector = NULL;

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 23 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						for (k=0; k < clustersPerForwardGroup1; k++)
						{
							if ( index == 0 )					// reserves space for the units firing vector
							{
								H1_CC4_CL23_FG_unitsDim[k] = H1_forwardGroupArrayCluster_4[k].getUnitsDimensionality();// gets units dimensionality
								H1_CC4_CL23_FG_outputDim[k] = H1_forwardGroupArrayCluster_4[k].getOutputDimensionality();// gets output dimensionality
								H1_CC4_CL23_FG_unitsFiring[k] = (double*)calloc(H1_CC4_CL23_FG_unitsDim[k], sizeof(double));
								H1_CC4_CL23_FG_outputFiring[k] = (double*)calloc(iterations*trainingData.chunks*H1_CC4_CL23_FG_outputDim[k], sizeof(double));
							}

							vector1 = H1_forwardGroupArrayCluster_4[k].getUnitsVector();
							for (j = 0; j < H1_CC4_CL23_FG_unitsDim[k]; j++ )
								if ( *(vector1 + j) )
									*(H1_CC4_CL23_FG_unitsFiring[k] + j) += 1;

							free(vector1);
							vector1 = NULL;
							vector = H1_forwardGroupArrayCluster_4[k].getOutputVector();
							for (j = 0; j < H1_CC4_CL23_FG_outputDim[k]; j++ )
								*((H1_CC4_CL23_FG_outputFiring[k] + H1_CC4_CL23_FG_outputDim[k]*index) + j) = *(vector + j);

							free(vector);
							vector = NULL;
						}

				//////////////////////////////////////////////////////////////////////////////////
				////////////////// This is for the hierarchical layer 2 (H2) /////////////////////
				//////////////////////////////////////////////////////////////////////////////////

					//////////////////////////////////////////////////////////////////////////////////
					////////////////// This is for the cortical column 1 (CC1) ///////////////////////
					//////////////////////////////////////////////////////////////////////////////////

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 4 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						if ( index == 0 )					// reserves space for the units firing vector
						{
							H2_CC1_CL4_unitsDim = H2_layer4Cluster_1.getUnitsDimensionality();// gets units dimensionality
							H2_CC1_CL4_outputDim = H2_layer4Cluster_1.getOutputDimensionality();// gets output dimensionality
							H2_CC1_CL4_unitsFiring = (double*)calloc(H2_CC1_CL4_unitsDim, sizeof(double));
							H2_CC1_CL4_outputFiring = (double*)calloc(iterations*trainingData.chunks*H2_CC1_CL4_outputDim, sizeof(double));
						}

						vector1 = H2_layer4Cluster_1.getUnitsVector();
						accum = 0;
						for (j = 0; j < H2_CC1_CL4_unitsDim; j++ )
						{
							if ( *(vector1 + j) )
							{
								accum++;
								if ( accum > 1 ) {
									H2_CC1_CL4_Prediction_Fault++;
									break;
								}
							}
						}
						if ( accum == 1 )
						{
							for (j = 0; j < H2_CC1_CL4_unitsDim; j++ )
							{
								if ( *(vector1 + j) )
								{
									*(H2_CC1_CL4_unitsFiring + j) += 1;
									break;
								}
							}
						}
						free(vector1);
						vector1 = NULL;
						vector = H2_layer4Cluster_1.getOutputVector();
						for (j = 0; j < H2_CC1_CL4_outputDim; j++ )
							*((H2_CC1_CL4_outputFiring + H2_CC1_CL4_outputDim*index) + j) = *(vector + j);

						free(vector);
						vector = NULL;

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 23 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						for (k=0; k < clustersPerForwardGroup2; k++)
						{
							if ( index == 0 )					// reserves space for the units firing vector
							{
								H2_CC1_CL23_FG_unitsDim[k] = H2_forwardGroupArrayCluster_1[k].getUnitsDimensionality();// gets units dimensionality
								H2_CC1_CL23_FG_outputDim[k] = H2_forwardGroupArrayCluster_1[k].getOutputDimensionality();// gets output dimensionality
								H2_CC1_CL23_FG_unitsFiring[k] = (double*)calloc(H2_CC1_CL23_FG_unitsDim[k], sizeof(double));
								H2_CC1_CL23_FG_outputFiring[k] = (double*)calloc(iterations*trainingData.chunks*H2_CC1_CL23_FG_outputDim[k], sizeof(double));
							}

							vector1 = H2_forwardGroupArrayCluster_1[k].getUnitsVector();
							for (j = 0; j < H2_CC1_CL23_FG_unitsDim[k]; j++ )
								if ( *(vector1 + j) )
									*(H2_CC1_CL23_FG_unitsFiring[k] + j) += 1;

							free(vector1);
							vector1 = NULL;
							vector = H2_forwardGroupArrayCluster_1[k].getOutputVector();
							for (j = 0; j < H2_CC1_CL23_FG_outputDim[k]; j++ )
								*((H2_CC1_CL23_FG_outputFiring[k] + H2_CC1_CL23_FG_outputDim[k]*index) + j) = *(vector + j);

							free(vector);
							vector = NULL;
						}

					//////////////////////////////////////////////////////////////////////////////////
					////////////////// This is for the cortical column 2 (CC2) ///////////////////////
					//////////////////////////////////////////////////////////////////////////////////

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 4 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						if ( index == 0 )					// reserves space for the units firing vector
						{
							H2_CC2_CL4_unitsDim = H2_layer4Cluster_2.getUnitsDimensionality();// gets units dimensionality
							H2_CC2_CL4_outputDim = H2_layer4Cluster_2.getOutputDimensionality();// gets output dimensionality
							H2_CC2_CL4_unitsFiring = (double*)calloc(H2_CC2_CL4_unitsDim, sizeof(double));
							H2_CC2_CL4_outputFiring = (double*)calloc(iterations*trainingData.chunks*H2_CC2_CL4_outputDim, sizeof(double));
						}

						vector1 = H2_layer4Cluster_2.getUnitsVector();
						accum = 0;
						for (j = 0; j < H2_CC2_CL4_unitsDim; j++ )
						{
							if ( *(vector1 + j) )
							{
								accum++;
								if ( accum > 1 ) {
									H2_CC2_CL4_Prediction_Fault++;
									break;
								}
							}
						}
						if ( accum == 1 )
						{
							for (j = 0; j < H2_CC2_CL4_unitsDim; j++ )
							{
								if ( *(vector1 + j) )
								{
									*(H2_CC2_CL4_unitsFiring + j) += 1;
									break;
								}
							}
						}
						free(vector1);
						vector1 = NULL;
						vector = H2_layer4Cluster_2.getOutputVector();
						for (j = 0; j < H2_CC2_CL4_outputDim; j++ )
							*((H2_CC2_CL4_outputFiring + H2_CC2_CL4_outputDim*index) + j) = *(vector + j);

						free(vector);
						vector = NULL;

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 23 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						for (k=0; k < clustersPerForwardGroup2; k++)
						{
							if ( index == 0 )					// reserves space for the units firing vector
							{
								H2_CC2_CL23_FG_unitsDim[k] = H2_forwardGroupArrayCluster_2[k].getUnitsDimensionality();// gets units dimensionality
								H2_CC2_CL23_FG_outputDim[k] = H2_forwardGroupArrayCluster_2[k].getOutputDimensionality();// gets output dimensionality
								H2_CC2_CL23_FG_unitsFiring[k] = (double*)calloc(H2_CC2_CL23_FG_unitsDim[k], sizeof(double));
								H2_CC2_CL23_FG_outputFiring[k] = (double*)calloc(iterations*trainingData.chunks*H2_CC2_CL23_FG_outputDim[k], sizeof(double));
							}

							vector1 = H2_forwardGroupArrayCluster_2[k].getUnitsVector();
							for (j = 0; j < H2_CC2_CL23_FG_unitsDim[k]; j++ )
								if ( *(vector1 + j) )
									*(H2_CC2_CL23_FG_unitsFiring[k] + j) += 1;

							free(vector1);
							vector1 = NULL;
							vector = H2_forwardGroupArrayCluster_2[k].getOutputVector();
							for (j = 0; j < H2_CC2_CL23_FG_outputDim[k]; j++ )
								*((H2_CC2_CL23_FG_outputFiring[k] + H2_CC2_CL23_FG_outputDim[k]*index) + j) = *(vector + j);

							free(vector);
							vector = NULL;
						}

				//////////////////////////////////////////////////////////////////////////////////
				////////////////// This is for the hierarchical layer 3 (H3) /////////////////////
				//////////////////////////////////////////////////////////////////////////////////

					//////////////////////////////////////////////////////////////////////////////////
					////////////////// This is for the cortical column 1 (CC1) ///////////////////////
					//////////////////////////////////////////////////////////////////////////////////

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 4 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						if ( index == 0 )					// reserves space for the units firing vector
						{
							H3_CC1_CL4_unitsDim = H3_layer4Cluster_1.getUnitsDimensionality();// gets units dimensionality
							H3_CC1_CL4_outputDim = H3_layer4Cluster_1.getOutputDimensionality();// gets output dimensionality
							H3_CC1_CL4_unitsFiring = (double*)calloc(H3_CC1_CL4_unitsDim, sizeof(double));
							H3_CC1_CL4_outputFiring = (double*)calloc(iterations*trainingData.chunks*H3_CC1_CL4_outputDim, sizeof(double));
						}

						vector1 = H3_layer4Cluster_1.getUnitsVector();
						accum = 0;
						for (j = 0; j < H3_CC1_CL4_unitsDim; j++ )
						{
							if ( *(vector1 + j) )
							{
								accum++;
								if ( accum > 1 ) {
									H3_CC1_CL4_Prediction_Fault++;
									break;
								}
							}
						}
						if ( accum == 1 )
						{
							for (j = 0; j < H3_CC1_CL4_unitsDim; j++ )
							{
								if ( *(vector1 + j) )
								{
									*(H3_CC1_CL4_unitsFiring + j) += 1;
									break;
								}
							}
						}
						free(vector1);
						vector1 = NULL;
						vector = H3_layer4Cluster_1.getOutputVector();
						for (j = 0; j < H3_CC1_CL4_outputDim; j++ )
							*((H3_CC1_CL4_outputFiring + H3_CC1_CL4_outputDim*index) + j) = *(vector + j);

						free(vector);
						vector = NULL;

						//////////////////////////////////////////////////////////////////////////////////
						////////////////// This is for the cortical layer 23 //////////////////////////////
						//////////////////////////////////////////////////////////////////////////////////

						for (k=0; k < clustersPerForwardGroup3; k++)
						{
							if ( index == 0 )					// reserves space for the units firing vector
							{
								H3_CC1_CL23_FG_unitsDim[k] = H3_forwardGroupArrayCluster_1[k].getUnitsDimensionality();// gets units dimensionality
								H3_CC1_CL23_FG_outputDim[k] = H3_forwardGroupArrayCluster_1[k].getOutputDimensionality();// gets output dimensionality
								H3_CC1_CL23_FG_unitsFiring[k] = (double*)calloc(H3_CC1_CL23_FG_unitsDim[k], sizeof(double));
								H3_CC1_CL23_FG_outputFiring[k] = (double*)calloc(iterations*trainingData.chunks*H3_CC1_CL23_FG_outputDim[k], sizeof(double));
							}

							vector1 = H3_forwardGroupArrayCluster_1[k].getUnitsVector();
							for (j = 0; j < H3_CC1_CL23_FG_unitsDim[k]; j++ )
								if ( *(vector1 + j) )
									*(H3_CC1_CL23_FG_unitsFiring[k] + j) += 1;

							free(vector1);
							vector1 = NULL;
							vector = H3_forwardGroupArrayCluster_1[k].getOutputVector();
							for (j = 0; j < H3_CC1_CL23_FG_outputDim[k]; j++ )
								*((H3_CC1_CL23_FG_outputFiring[k] + H3_CC1_CL23_FG_outputDim[k]*index) + j) = *(vector + j);

							free(vector);
							vector = NULL;
						}
			}
			index++;
		}
	}


	if ( firingOn == 'Y' && enableLearning == 0 )
	{
		std::string name;

	//////////////////////////////////////////////////////////////////////////////////
	////////////////// This is for the hierarchical layer 1 (H1) /////////////////////
	//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		////////////////// This is for the cortical column 1 (CC1) ///////////////////////
		//////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 4 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			name = "H1_CC1_CL4_Firing.mat";
			saveFiringForOctave(H1_CC1_CL4_unitsFiring, 1, H1_CC1_CL4_unitsDim, H1_CC1_CL4_outputFiring, iterations*trainingData.chunks, H1_CC1_CL4_outputDim, name);

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 23 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			for (k=0; k < clustersPerForwardGroup1; k++)
			{
				name = "H1_CC1_CL23_FG_Firing" + std::to_string(k+1) + ".mat";
				saveFiringForOctave(H1_CC1_CL23_FG_unitsFiring[k], 1, H1_CC1_CL23_FG_unitsDim[k], H1_CC1_CL23_FG_outputFiring[k], iterations*trainingData.chunks, H1_CC1_CL23_FG_outputDim[k], name);
			}

		//////////////////////////////////////////////////////////////////////////////////
		////////////////// This is for the cortical column 2 (CC2) ///////////////////////
		//////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 4 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			name = "H1_CC2_CL4_Firing.mat";
			saveFiringForOctave(H1_CC2_CL4_unitsFiring, 1, H1_CC2_CL4_unitsDim, H1_CC2_CL4_outputFiring, iterations*trainingData.chunks, H1_CC2_CL4_outputDim, name);

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 23 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			for (k=0; k < clustersPerForwardGroup1; k++)
			{
				name = "H1_CC2_CL23_FG_Firing" + std::to_string(k+1) + ".mat";
				saveFiringForOctave(H1_CC2_CL23_FG_unitsFiring[k], 1, H1_CC2_CL23_FG_unitsDim[k], H1_CC2_CL23_FG_outputFiring[k], iterations*trainingData.chunks, H1_CC2_CL23_FG_outputDim[k], name);
			}

		//////////////////////////////////////////////////////////////////////////////////
		////////////////// This is for the cortical column 3 (CC3) ///////////////////////
		//////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 4 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			name = "H1_CC3_CL4_Firing.mat";
			saveFiringForOctave(H1_CC3_CL4_unitsFiring, 1, H1_CC3_CL4_unitsDim, H1_CC3_CL4_outputFiring, iterations*trainingData.chunks, H1_CC3_CL4_outputDim, name);

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 23 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			for (k=0; k < clustersPerForwardGroup1; k++)
			{
				name = "H1_CC3_CL23_FG_Firing" + std::to_string(k+1) + ".mat";
				saveFiringForOctave(H1_CC3_CL23_FG_unitsFiring[k], 1, H1_CC3_CL23_FG_unitsDim[k], H1_CC3_CL23_FG_outputFiring[k], iterations*trainingData.chunks, H1_CC3_CL23_FG_outputDim[k], name);
			}

		//////////////////////////////////////////////////////////////////////////////////
		////////////////// This is for the cortical column 4 (CC4) ///////////////////////
		//////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 4 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			name = "H1_CC4_CL4_Firing.mat";
			saveFiringForOctave(H1_CC4_CL4_unitsFiring, 1, H1_CC4_CL4_unitsDim, H1_CC4_CL4_outputFiring, iterations*trainingData.chunks, H1_CC4_CL4_outputDim, name);

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 23 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			for (k=0; k < clustersPerForwardGroup1; k++)
			{
				name = "H1_CC4_CL23_FG_Firing" + std::to_string(k+1) + ".mat";
				saveFiringForOctave(H1_CC4_CL23_FG_unitsFiring[k], 1, H1_CC4_CL23_FG_unitsDim[k], H1_CC4_CL23_FG_outputFiring[k], iterations*trainingData.chunks, H1_CC4_CL23_FG_outputDim[k], name);
			}

	//////////////////////////////////////////////////////////////////////////////////
	////////////////// This is for the hierarchical layer 2 (H2) /////////////////////
	//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		////////////////// This is for the cortical column 1 (CC1) ///////////////////////
		//////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 4 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			name = "H2_CC1_CL4_Firing.mat";
			saveFiringForOctave(H2_CC1_CL4_unitsFiring, 1, H2_CC1_CL4_unitsDim, H2_CC1_CL4_outputFiring, iterations*trainingData.chunks, H2_CC1_CL4_outputDim, name);

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 23 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			for (k=0; k < clustersPerForwardGroup2; k++)
			{
				name = "H2_CC1_CL23_FG_Firing" + std::to_string(k+1) + ".mat";
				saveFiringForOctave(H2_CC1_CL23_FG_unitsFiring[k], 1, H2_CC1_CL23_FG_unitsDim[k], H2_CC1_CL23_FG_outputFiring[k], iterations*trainingData.chunks, H2_CC1_CL23_FG_outputDim[k], name);
			}

		//////////////////////////////////////////////////////////////////////////////////
		////////////////// This is for the cortical column 2 (CC2) ///////////////////////
		//////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 4 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			name = "H2_CC2_CL4_Firing.mat";
			saveFiringForOctave(H2_CC2_CL4_unitsFiring, 1, H2_CC2_CL4_unitsDim, H2_CC2_CL4_outputFiring, iterations*trainingData.chunks, H2_CC2_CL4_outputDim, name);

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 23 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			for (k=0; k < clustersPerForwardGroup2; k++)
			{
				name = "H2_CC2_CL23_FG_Firing" + std::to_string(k+1) + ".mat";
				saveFiringForOctave(H2_CC2_CL23_FG_unitsFiring[k], 1, H2_CC2_CL23_FG_unitsDim[k], H2_CC2_CL23_FG_outputFiring[k], iterations*trainingData.chunks, H2_CC2_CL23_FG_outputDim[k], name);
			}

	//////////////////////////////////////////////////////////////////////////////////
	////////////////// This is for the hierarchical layer 3 (H3) /////////////////////
	//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		////////////////// This is for the cortical column 1 (CC1) ///////////////////////
		//////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 4 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			name = "H3_CC1_CL4_Firing.mat";
			saveFiringForOctave(H3_CC1_CL4_unitsFiring, 1, H3_CC1_CL4_unitsDim, H3_CC1_CL4_outputFiring, iterations*trainingData.chunks, H3_CC1_CL4_outputDim, name);

			//////////////////////////////////////////////////////////////////////////////////
			////////////////// This is for the cortical layer 23 //////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////

			for (k=0; k < clustersPerForwardGroup3; k++)
			{
				name = "H3_CC1_CL23_FG_Firing" + std::to_string(k+1) + ".mat";
				saveFiringForOctave(H3_CC1_CL23_FG_unitsFiring[k], 1, H3_CC1_CL23_FG_unitsDim[k], H3_CC1_CL23_FG_outputFiring[k], iterations*trainingData.chunks, H3_CC1_CL23_FG_outputDim[k], name);
			}

	}
}

////////////////////////////////////////// training data propagated through the region






////////////////////////////////////////// saves the cortical node connectome

if ( enableLearning == 1 )
{
	printf("\nDo you want to save the current node connectome? (Y) for yes and any key for no\n\n");
	//scanf(" %c", &option);

	option = 'Y';

	if ( option == 'Y' )
	{
		printf("\nIt said yes\n\n");
		// This is for the first hierarchical layer

		forwardClusters = H1_CLUSTERSPERFORWARDGROUP;
		backwardClusters = H1_CLUSTERSPERBACKWARDGROUP;

		// Cortical Node number 1

		connectomeNode.l4I = H1_layer4Cluster_1.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H1_layer4Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H1_layer4Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H1_layer4Cluster_1.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H1_layer4Cluster_1.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster
		connectomeNode.l4S = H1_layer4Cluster_1.getStrongUnits();					// gets strongUnits in the layer 4 cluster

		connectomeNode.l5I = H1_layer5Cluster_1.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H1_layer5Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H1_layer5Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H1_layer5Cluster_1.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights
		connectomeNode.l5S = H1_layer5Cluster_1.getStrongUnits();					// gets strongUnits in the layer 5 cluster

		connectomeNode.l6I = H1_layer6Cluster_1.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H1_layer6Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H1_layer6Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H1_layer6Cluster_1.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights
		connectomeNode.l6S = H1_layer6Cluster_1.getStrongUnits();					// gets strongUnits in the layer 6 cluster

		connectomeNode.l23FGAI = H1_forwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H1_forwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H1_forwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H1_forwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
		{
			connectomeNode.l23FGAW[i] = H1_forwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights
			connectomeNode.l23FGAS[i] = H1_forwardGroupArrayCluster_1[i].getStrongUnits();		// gets a pointer to the forward group array cluster [i] strong units weights
		}

		connectomeNode.l23BGAI = H1_backwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H1_backwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H1_backwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H1_backwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
		{
			connectomeNode.l23BGAW[i] = H1_backwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights
			connectomeNode.l23BGAS[i] = H1_backwardGroupArrayCluster_1[i].getStrongUnits();		// gets a pointer to the backwardgroup array cluster [i] strong units weights
		}

		saveConnectome(connectomeNode, 1, 1);								// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l4S);
		free(connectomeNode.l5W);
		free(connectomeNode.l5S);
		free(connectomeNode.l6W);
		free(connectomeNode.l6S);

		for ( i = 0; i < forwardClusters; i++ )
		{
			free(connectomeNode.l23FGAW[i]);
			free(connectomeNode.l23FGAS[i]);
		}

		for ( i = 0; i < backwardClusters; i++ )
		{
			free(connectomeNode.l23BGAW[i]);
			free(connectomeNode.l23BGAS[i]);
		}

		// Cortical Node number 2

		connectomeNode.l4I = H1_layer4Cluster_2.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H1_layer4Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H1_layer4Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H1_layer4Cluster_2.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H1_layer4Cluster_2.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster
		connectomeNode.l4S = H1_layer4Cluster_2.getStrongUnits();					// gets strongUnits in the layer 4 cluster

		connectomeNode.l5I = H1_layer5Cluster_2.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H1_layer5Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H1_layer5Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H1_layer5Cluster_2.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights
		connectomeNode.l5S = H1_layer5Cluster_2.getStrongUnits();					// gets strongUnits in the layer 5 cluster

		connectomeNode.l6I = H1_layer6Cluster_2.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H1_layer6Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H1_layer6Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H1_layer6Cluster_2.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights
		connectomeNode.l6S = H1_layer6Cluster_2.getStrongUnits();					// gets strongUnits in the layer 6 cluster

		connectomeNode.l23FGAI = H1_forwardGroupArrayCluster_2[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H1_forwardGroupArrayCluster_2[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H1_forwardGroupArrayCluster_2[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H1_forwardGroup_2.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
		{
			connectomeNode.l23FGAW[i] = H1_forwardGroupArrayCluster_2[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights
			connectomeNode.l23FGAS[i] = H1_forwardGroupArrayCluster_2[i].getStrongUnits();		// gets a pointer to the forward group array cluster [i] strong units weights
		}

		connectomeNode.l23BGAI = H1_backwardGroupArrayCluster_2[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H1_backwardGroupArrayCluster_2[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H1_backwardGroupArrayCluster_2[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H1_backwardGroup_2.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
		{
			connectomeNode.l23BGAW[i] = H1_backwardGroupArrayCluster_2[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights
			connectomeNode.l23BGAS[i] = H1_backwardGroupArrayCluster_2[i].getStrongUnits();		// gets a pointer to the backwardgroup array cluster [i] strong units weights
		}

		saveConnectome(connectomeNode, 1, 2);								// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l4S);
		free(connectomeNode.l5W);
		free(connectomeNode.l5S);
		free(connectomeNode.l6W);
		free(connectomeNode.l6S);

		for ( i = 0; i < forwardClusters; i++ )
		{
			free(connectomeNode.l23FGAW[i]);
			free(connectomeNode.l23FGAS[i]);
		}

		for ( i = 0; i < backwardClusters; i++ )
		{
			free(connectomeNode.l23BGAW[i]);
			free(connectomeNode.l23BGAS[i]);
		}

		// Cortical Node number 3

		connectomeNode.l4I = H1_layer4Cluster_3.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H1_layer4Cluster_3.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H1_layer4Cluster_3.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H1_layer4Cluster_3.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H1_layer4Cluster_3.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster
		connectomeNode.l4S = H1_layer4Cluster_3.getStrongUnits();					// gets strongUnits in the layer 4 cluster

		connectomeNode.l5I = H1_layer5Cluster_3.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H1_layer5Cluster_3.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H1_layer5Cluster_3.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H1_layer5Cluster_3.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights
		connectomeNode.l5S = H1_layer5Cluster_3.getStrongUnits();					// gets strongUnits in the layer 5 cluster

		connectomeNode.l6I = H1_layer6Cluster_3.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H1_layer6Cluster_3.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H1_layer6Cluster_3.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H1_layer6Cluster_3.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights
		connectomeNode.l6S = H1_layer6Cluster_3.getStrongUnits();					// gets strongUnits in the layer 6 cluster

		connectomeNode.l23FGAI = H1_forwardGroupArrayCluster_3[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H1_forwardGroupArrayCluster_3[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H1_forwardGroupArrayCluster_3[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H1_forwardGroup_3.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
		{
			connectomeNode.l23FGAW[i] = H1_forwardGroupArrayCluster_3[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights
			connectomeNode.l23FGAS[i] = H1_forwardGroupArrayCluster_3[i].getStrongUnits();		// gets a pointer to the forward group array cluster [i] strong units weights
		}

		connectomeNode.l23BGAI = H1_backwardGroupArrayCluster_3[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H1_backwardGroupArrayCluster_3[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H1_backwardGroupArrayCluster_3[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H1_backwardGroup_3.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
		{
			connectomeNode.l23BGAW[i] = H1_backwardGroupArrayCluster_3[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights
			connectomeNode.l23BGAS[i] = H1_backwardGroupArrayCluster_3[i].getStrongUnits();		// gets a pointer to the backwardgroup array cluster [i] strong units weights
		}

		saveConnectome(connectomeNode, 1, 3);								// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l4S);
		free(connectomeNode.l5W);
		free(connectomeNode.l5S);
		free(connectomeNode.l6W);
		free(connectomeNode.l6S);

		for ( i = 0; i < forwardClusters; i++ )
		{
			free(connectomeNode.l23FGAW[i]);
			free(connectomeNode.l23FGAS[i]);
		}

		for ( i = 0; i < backwardClusters; i++ )
		{
			free(connectomeNode.l23BGAW[i]);
			free(connectomeNode.l23BGAS[i]);
		}

		// Cortical Node number 4

		connectomeNode.l4I = H1_layer4Cluster_4.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H1_layer4Cluster_4.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H1_layer4Cluster_4.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H1_layer4Cluster_4.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H1_layer4Cluster_4.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster
		connectomeNode.l4S = H1_layer4Cluster_4.getStrongUnits();					// gets strongUnits in the layer 4 cluster

		connectomeNode.l5I = H1_layer5Cluster_4.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H1_layer5Cluster_4.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H1_layer5Cluster_4.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H1_layer5Cluster_4.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights
		connectomeNode.l5S = H1_layer5Cluster_4.getStrongUnits();					// gets strongUnits in the layer 5 cluster

		connectomeNode.l6I = H1_layer6Cluster_4.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H1_layer6Cluster_4.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H1_layer6Cluster_4.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H1_layer6Cluster_4.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights
		connectomeNode.l6S = H1_layer6Cluster_4.getStrongUnits();					// gets strongUnits in the layer 6 cluster

		connectomeNode.l23FGAI = H1_forwardGroupArrayCluster_4[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H1_forwardGroupArrayCluster_4[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H1_forwardGroupArrayCluster_4[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H1_forwardGroup_4.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
		{
			connectomeNode.l23FGAW[i] = H1_forwardGroupArrayCluster_4[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights
			connectomeNode.l23FGAS[i] = H1_forwardGroupArrayCluster_4[i].getStrongUnits();		// gets a pointer to the forward group array cluster [i] strong units weights
		}

		connectomeNode.l23BGAI = H1_backwardGroupArrayCluster_4[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H1_backwardGroupArrayCluster_4[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H1_backwardGroupArrayCluster_4[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H1_backwardGroup_4.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
		{
			connectomeNode.l23BGAW[i] = H1_backwardGroupArrayCluster_4[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights
			connectomeNode.l23BGAS[i] = H1_backwardGroupArrayCluster_4[i].getStrongUnits();		// gets a pointer to the backwardgroup array cluster [i] strong units weights
		}

		saveConnectome(connectomeNode, 1, 4);								// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l4S);
		free(connectomeNode.l5W);
		free(connectomeNode.l5S);
		free(connectomeNode.l6W);
		free(connectomeNode.l6S);

		for ( i = 0; i < forwardClusters; i++ )
		{
			free(connectomeNode.l23FGAW[i]);
			free(connectomeNode.l23FGAS[i]);
		}

		for ( i = 0; i < backwardClusters; i++ )
		{
			free(connectomeNode.l23BGAW[i]);
			free(connectomeNode.l23BGAS[i]);
		}

		// This is for the second hierarchical layer

		forwardClusters = H2_CLUSTERSPERFORWARDGROUP;
		backwardClusters = H2_CLUSTERSPERBACKWARDGROUP;

		// Cortical Node number 1

		connectomeNode.l4I = H2_layer4Cluster_1.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H2_layer4Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H2_layer4Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H2_layer4Cluster_1.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H2_layer4Cluster_1.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster
		connectomeNode.l4S = H2_layer4Cluster_1.getStrongUnits();					// gets strongUnits in the layer 4 cluster

		connectomeNode.l5I = H2_layer5Cluster_1.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H2_layer5Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H2_layer5Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H2_layer5Cluster_1.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights
		connectomeNode.l5S = H2_layer5Cluster_1.getStrongUnits();					// gets strongUnits in the layer 5 cluster

		connectomeNode.l6I = H2_layer6Cluster_1.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H2_layer6Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H2_layer6Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H2_layer6Cluster_1.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights
		connectomeNode.l6S = H2_layer6Cluster_1.getStrongUnits();					// gets strongUnits in the layer 6 cluster

		connectomeNode.l23FGAI = H2_forwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H2_forwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H2_forwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H2_forwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
		{
			connectomeNode.l23FGAW[i] = H2_forwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights
			connectomeNode.l23FGAS[i] = H2_forwardGroupArrayCluster_1[i].getStrongUnits();		// gets a pointer to the forward group array cluster [i] strong units weights
		}

		connectomeNode.l23BGAI = H2_backwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H2_backwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H2_backwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H2_backwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
		{
			connectomeNode.l23BGAW[i] = H2_backwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights
			connectomeNode.l23BGAS[i] = H2_backwardGroupArrayCluster_1[i].getStrongUnits();		// gets a pointer to the backwardgroup array cluster [i] strong units weights	
		}

		saveConnectome(connectomeNode, 2, 1);								// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l4S);
		free(connectomeNode.l5W);
		free(connectomeNode.l5S);
		free(connectomeNode.l6W);
		free(connectomeNode.l6S);

		for ( i = 0; i < forwardClusters; i++ )
		{
			free(connectomeNode.l23FGAW[i]);
			free(connectomeNode.l23FGAS[i]);
		}

		for ( i = 0; i < backwardClusters; i++ )
		{
			free(connectomeNode.l23BGAW[i]);
			free(connectomeNode.l23BGAS[i]);
		}

		// Cortical Node number 2

		connectomeNode.l4I = H2_layer4Cluster_2.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H2_layer4Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H2_layer4Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H2_layer4Cluster_2.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H2_layer4Cluster_2.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster
		connectomeNode.l4S = H2_layer4Cluster_2.getStrongUnits();					// gets strongUnits in the layer 4 cluster

		connectomeNode.l5I = H2_layer5Cluster_2.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H2_layer5Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H2_layer5Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H2_layer5Cluster_2.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights
		connectomeNode.l5S = H2_layer5Cluster_2.getStrongUnits();					// gets strongUnits in the layer 5 cluster

		connectomeNode.l6I = H2_layer6Cluster_2.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H2_layer6Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H2_layer6Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H2_layer6Cluster_2.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights
		connectomeNode.l6S = H2_layer6Cluster_2.getStrongUnits();					// gets strongUnits in the layer 6 cluster

		connectomeNode.l23FGAI = H2_forwardGroupArrayCluster_2[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H2_forwardGroupArrayCluster_2[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H2_forwardGroupArrayCluster_2[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H2_forwardGroup_2.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
		{
			connectomeNode.l23FGAW[i] = H2_forwardGroupArrayCluster_2[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights
			connectomeNode.l23FGAS[i] = H2_forwardGroupArrayCluster_2[i].getStrongUnits();		// gets a pointer to the forward group array cluster [i] strong units weights
		}

		connectomeNode.l23BGAI = H2_backwardGroupArrayCluster_2[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H2_backwardGroupArrayCluster_2[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H2_backwardGroupArrayCluster_2[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H2_backwardGroup_2.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
		{
			connectomeNode.l23BGAW[i] = H2_backwardGroupArrayCluster_2[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights
			connectomeNode.l23BGAS[i] = H2_backwardGroupArrayCluster_2[i].getStrongUnits();		// gets a pointer to the backwardgroup array cluster [i] strong units weights	}
		}

		saveConnectome(connectomeNode, 2, 2);								// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l4S);
		free(connectomeNode.l5W);
		free(connectomeNode.l5S);
		free(connectomeNode.l6W);
		free(connectomeNode.l6S);

		for ( i = 0; i < forwardClusters; i++ )
		{
			free(connectomeNode.l23FGAW[i]);
			free(connectomeNode.l23FGAS[i]);
		}

		for ( i = 0; i < backwardClusters; i++ )
		{
			free(connectomeNode.l23BGAW[i]);
			free(connectomeNode.l23BGAS[i]);
		}

		// This is for the third hierarchical layer

		forwardClusters = H3_CLUSTERSPERFORWARDGROUP;
		backwardClusters = H3_CLUSTERSPERBACKWARDGROUP;

		// Cortical Node number 1

		connectomeNode.l4I = H3_layer4Cluster_1.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H3_layer4Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H3_layer4Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H3_layer4Cluster_1.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H3_layer4Cluster_1.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster
		connectomeNode.l4S = H3_layer4Cluster_1.getStrongUnits();					// gets strongUnits in the layer 4 cluster

		connectomeNode.l5I = H3_layer5Cluster_1.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H3_layer5Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H3_layer5Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H3_layer5Cluster_1.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights
		connectomeNode.l5S = H3_layer5Cluster_1.getStrongUnits();					// gets strongUnits in the layer 5 cluster

		connectomeNode.l6I = H3_layer6Cluster_1.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H3_layer6Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H3_layer6Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H3_layer6Cluster_1.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights
		connectomeNode.l6S = H3_layer6Cluster_1.getStrongUnits();					// gets strongUnits in the layer 6 cluster

		connectomeNode.l23FGAI = H3_forwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H3_forwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H3_forwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H3_forwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
		{
			connectomeNode.l23FGAW[i] = H3_forwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights
			connectomeNode.l23FGAS[i] = H3_forwardGroupArrayCluster_1[i].getStrongUnits();		// gets a pointer to the forward group array cluster [i] strong units weights
		}

		connectomeNode.l23BGAI = H3_backwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H3_backwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H3_backwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H3_backwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
		{
			connectomeNode.l23BGAW[i] = H3_backwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights
			connectomeNode.l23BGAS[i] = H3_backwardGroupArrayCluster_1[i].getStrongUnits();		// gets a pointer to the backwardgroup array cluster [i] strong units weights	}
		}

		saveConnectome(connectomeNode, 3, 1);								// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l4S);
		free(connectomeNode.l5W);
		free(connectomeNode.l5S);
		free(connectomeNode.l6W);
		free(connectomeNode.l6S);

		for ( i = 0; i < forwardClusters; i++ )
		{
			free(connectomeNode.l23FGAW[i]);
			free(connectomeNode.l23FGAS[i]);
		}

		for ( i = 0; i < backwardClusters; i++ )
		{
			free(connectomeNode.l23BGAW[i]);
			free(connectomeNode.l23BGAS[i]);
		}

	}
}

////////////////////////////////////////// cortical node connectome saved






////////////////////////////////////////// saves the cortical node connectome in a Octave format file (.mat) for visualization issues

if ( enableLearning == 1 )
{
	printf("\nDo you want to save the current node connectome in a Octave file (.mat) for future visualizotion issues? (Y) for yes and any key for no\n\n");
	//scanf(" %c", &option);

	option = 'Y';

	if ( option == 'Y' )
	{
		printf("\nIt said yes\n\n");
		// This is for the first hierarchical layer

		forwardClusters = H1_CLUSTERSPERFORWARDGROUP;
		backwardClusters = H1_CLUSTERSPERBACKWARDGROUP;

		// Cortical Node number 1

		connectomeNode.l4I = H1_layer4Cluster_1.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H1_layer4Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H1_layer4Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H1_layer4Cluster_1.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H1_layer4Cluster_1.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster

		connectomeNode.l5I = H1_layer5Cluster_1.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H1_layer5Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H1_layer5Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H1_layer5Cluster_1.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights

		connectomeNode.l6I = H1_layer6Cluster_1.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H1_layer6Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H1_layer6Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H1_layer6Cluster_1.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights

		connectomeNode.l23FGAI = H1_forwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H1_forwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H1_forwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H1_forwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
			connectomeNode.l23FGAW[i] = H1_forwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights

		connectomeNode.l23BGAI = H1_backwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H1_backwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H1_backwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H1_backwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
			connectomeNode.l23BGAW[i] = H1_backwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights

		saveConnectomeForOctave(connectomeNode, 1, 1);							// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l5W);
		free(connectomeNode.l6W);

		for ( i = 0; i < forwardClusters; i++ )
			free(connectomeNode.l23FGAW[i]);

		for ( i = 0; i < backwardClusters; i++ )
			free(connectomeNode.l23BGAW[i]);

		// Cortical Node number 2

		connectomeNode.l4I = H1_layer4Cluster_2.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H1_layer4Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H1_layer4Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H1_layer4Cluster_2.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H1_layer4Cluster_2.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster

		connectomeNode.l5I = H1_layer5Cluster_2.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H1_layer5Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H1_layer5Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H1_layer5Cluster_2.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights

		connectomeNode.l6I = H1_layer6Cluster_2.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H1_layer6Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H1_layer6Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H1_layer6Cluster_2.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights

		connectomeNode.l23FGAI = H1_forwardGroupArrayCluster_2[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H1_forwardGroupArrayCluster_2[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H1_forwardGroupArrayCluster_2[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H1_forwardGroup_2.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
			connectomeNode.l23FGAW[i] = H1_forwardGroupArrayCluster_2[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights

		connectomeNode.l23BGAI = H1_backwardGroupArrayCluster_2[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H1_backwardGroupArrayCluster_2[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H1_backwardGroupArrayCluster_2[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H1_backwardGroup_2.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
			connectomeNode.l23BGAW[i] = H1_backwardGroupArrayCluster_2[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights

		saveConnectomeForOctave(connectomeNode, 1, 2);							// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l5W);
		free(connectomeNode.l6W);

		for ( i = 0; i < forwardClusters; i++ )
			free(connectomeNode.l23FGAW[i]);

		for ( i = 0; i < backwardClusters; i++ )
			free(connectomeNode.l23BGAW[i]);

		// Cortical Node number 3

		connectomeNode.l4I = H1_layer4Cluster_3.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H1_layer4Cluster_3.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H1_layer4Cluster_3.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H1_layer4Cluster_3.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H1_layer4Cluster_3.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster

		connectomeNode.l5I = H1_layer5Cluster_3.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H1_layer5Cluster_3.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H1_layer5Cluster_3.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H1_layer5Cluster_3.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights

		connectomeNode.l6I = H1_layer6Cluster_3.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H1_layer6Cluster_3.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H1_layer6Cluster_3.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H1_layer6Cluster_3.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights

		connectomeNode.l23FGAI = H1_forwardGroupArrayCluster_3[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H1_forwardGroupArrayCluster_3[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H1_forwardGroupArrayCluster_3[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H1_forwardGroup_3.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
			connectomeNode.l23FGAW[i] = H1_forwardGroupArrayCluster_3[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights

		connectomeNode.l23BGAI = H1_backwardGroupArrayCluster_3[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H1_backwardGroupArrayCluster_3[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H1_backwardGroupArrayCluster_3[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H1_backwardGroup_3.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
			connectomeNode.l23BGAW[i] = H1_backwardGroupArrayCluster_3[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights

		saveConnectomeForOctave(connectomeNode, 1, 3);							// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l5W);
		free(connectomeNode.l6W);

		for ( i = 0; i < forwardClusters; i++ )
			free(connectomeNode.l23FGAW[i]);

		for ( i = 0; i < backwardClusters; i++ )
			free(connectomeNode.l23BGAW[i]);

		// Cortical Node number 4

		connectomeNode.l4I = H1_layer4Cluster_4.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H1_layer4Cluster_4.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H1_layer4Cluster_4.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H1_layer4Cluster_4.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H1_layer4Cluster_4.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster

		connectomeNode.l5I = H1_layer5Cluster_4.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H1_layer5Cluster_4.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H1_layer5Cluster_4.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H1_layer5Cluster_4.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights

		connectomeNode.l6I = H1_layer6Cluster_4.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H1_layer6Cluster_4.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H1_layer6Cluster_4.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H1_layer6Cluster_4.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights

		connectomeNode.l23FGAI = H1_forwardGroupArrayCluster_4[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H1_forwardGroupArrayCluster_4[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H1_forwardGroupArrayCluster_4[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H1_forwardGroup_4.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
			connectomeNode.l23FGAW[i] = H1_forwardGroupArrayCluster_4[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights

		connectomeNode.l23BGAI = H1_backwardGroupArrayCluster_4[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H1_backwardGroupArrayCluster_4[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H1_backwardGroupArrayCluster_4[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H1_backwardGroup_4.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
			connectomeNode.l23BGAW[i] = H1_backwardGroupArrayCluster_4[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights

		saveConnectomeForOctave(connectomeNode, 1, 4);							// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l5W);
		free(connectomeNode.l6W);

		for ( i = 0; i < forwardClusters; i++ )
			free(connectomeNode.l23FGAW[i]);

		for ( i = 0; i < backwardClusters; i++ )
			free(connectomeNode.l23BGAW[i]);

		// This is for the second hierarchical layer

		forwardClusters = H2_CLUSTERSPERFORWARDGROUP;
		backwardClusters = H2_CLUSTERSPERBACKWARDGROUP;

		// Cortical Node number 1

		connectomeNode.l4I = H2_layer4Cluster_1.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H2_layer4Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H2_layer4Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H2_layer4Cluster_1.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H2_layer4Cluster_1.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster

		connectomeNode.l5I = H2_layer5Cluster_1.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H2_layer5Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H2_layer5Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H2_layer5Cluster_1.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights

		connectomeNode.l6I = H2_layer6Cluster_1.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H2_layer6Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H2_layer6Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H2_layer6Cluster_1.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights

		connectomeNode.l23FGAI = H2_forwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H2_forwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H2_forwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H2_forwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
			connectomeNode.l23FGAW[i] = H2_forwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights

		connectomeNode.l23BGAI = H2_backwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H2_backwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H2_backwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H2_backwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
			connectomeNode.l23BGAW[i] = H2_backwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights

		saveConnectomeForOctave(connectomeNode, 2, 1);							// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l5W);
		free(connectomeNode.l6W);

		for ( i = 0; i < forwardClusters; i++ )
			free(connectomeNode.l23FGAW[i]);

		for ( i = 0; i < backwardClusters; i++ )
			free(connectomeNode.l23BGAW[i]);

		// Cortical Node number 2

		connectomeNode.l4I = H2_layer4Cluster_2.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H2_layer4Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H2_layer4Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H2_layer4Cluster_2.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H2_layer4Cluster_2.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster

		connectomeNode.l5I = H2_layer5Cluster_2.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H2_layer5Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H2_layer5Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H2_layer5Cluster_2.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights

		connectomeNode.l6I = H2_layer6Cluster_2.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H2_layer6Cluster_2.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H2_layer6Cluster_2.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H2_layer6Cluster_2.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights

		connectomeNode.l23FGAI = H2_forwardGroupArrayCluster_2[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H2_forwardGroupArrayCluster_2[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H2_forwardGroupArrayCluster_2[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H2_forwardGroup_2.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
			connectomeNode.l23FGAW[i] = H2_forwardGroupArrayCluster_2[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights

		connectomeNode.l23BGAI = H2_backwardGroupArrayCluster_2[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H2_backwardGroupArrayCluster_2[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H2_backwardGroupArrayCluster_2[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H2_backwardGroup_2.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
			connectomeNode.l23BGAW[i] = H2_backwardGroupArrayCluster_2[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights

		saveConnectomeForOctave(connectomeNode, 2, 2);							// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l5W);
		free(connectomeNode.l6W);

		for ( i = 0; i < forwardClusters; i++ )
			free(connectomeNode.l23FGAW[i]);

		for ( i = 0; i < backwardClusters; i++ )
			free(connectomeNode.l23BGAW[i]);

		// This is for the third hierarchical layer

		forwardClusters = H3_CLUSTERSPERFORWARDGROUP;
		backwardClusters = H3_CLUSTERSPERBACKWARDGROUP;

		// Cortical Node number 1

		connectomeNode.l4I = H3_layer4Cluster_1.getInputDimensionality();				// gets the number of components of the layer 4 cluster input
		connectomeNode.l4U = H3_layer4Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 4 cluster units
		connectomeNode.l4O = H3_layer4Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 4 cluster output
		connectomeNode.l4W = H3_layer4Cluster_1.getSynapticWeights();					// gets a pointer to the layer 4 cluster synaptic weights
		connectomeNode.l4P = H3_layer4Cluster_1.getPredictionSynapsis();				// gets predictionSynapsis in the layer 4 cluster

		connectomeNode.l5I = H3_layer5Cluster_1.getInputDimensionality();				// gets the number of components of the layer 5 cluster input
		connectomeNode.l5U = H3_layer5Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 5 cluster units
		connectomeNode.l5O = H3_layer5Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 5 cluster output
		connectomeNode.l5W = H3_layer5Cluster_1.getSynapticWeights();					// gets a pointer to the layer 5 cluster synaptic weights

		connectomeNode.l6I = H3_layer6Cluster_1.getInputDimensionality();				// gets the number of components of the layer 6 cluster input
		connectomeNode.l6U = H3_layer6Cluster_1.getUnitsDimensionality();				// gets the number of components of the layer 6 cluster units
		connectomeNode.l6O = H3_layer6Cluster_1.getOutputDimensionality();				// gets the number of components of the layer 6 cluster output
		connectomeNode.l6W = H3_layer6Cluster_1.getSynapticWeights();					// gets a pointer to the layer 6 cluster synaptic weights

		connectomeNode.l23FGAI = H3_forwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the forward group array cluster input
		connectomeNode.l23FGAU = H3_forwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the forward group array cluster units
		connectomeNode.l23FGAO = H3_forwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the forward group array cluster output
		connectomeNode.l23FGC = H3_forwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 forward group
		for ( i = 0; i < forwardClusters; i++ )
			connectomeNode.l23FGAW[i] = H3_forwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the forward group array cluster [i] synaptic weights

		connectomeNode.l23BGAI = H3_backwardGroupArrayCluster_1[0].getInputDimensionality();		// gets the number of components of the backward group array cluster input
		connectomeNode.l23BGAU = H3_backwardGroupArrayCluster_1[0].getUnitsDimensionality();		// gets the number of components of the backward group array cluster units
		connectomeNode.l23BGAO = H3_backwardGroupArrayCluster_1[0].getOutputDimensionality();		// gets the number of components of the backward group array cluster output
		connectomeNode.l23BGC = H3_backwardGroup_1.getNumberOfClusters();				// gets the number of clusters in the layer 23 backward group
		for ( i = 0; i < backwardClusters; i++ )
			connectomeNode.l23BGAW[i] = H3_backwardGroupArrayCluster_1[i].getSynapticWeights();	// gets a pointer to the backward group array cluster [i] synaptic weights

		saveConnectomeForOctave(connectomeNode, 3, 1);							// saves the connectome

		free(connectomeNode.l4W);
		free(connectomeNode.l4P);
		free(connectomeNode.l5W);
		free(connectomeNode.l6W);

		for ( i = 0; i < forwardClusters; i++ )
			free(connectomeNode.l23FGAW[i]);

		for ( i = 0; i < backwardClusters; i++ )
			free(connectomeNode.l23BGAW[i]);

	}
}

////////////////////////////////////////// cortical node connectome saved in a Octave format file (.mat) for visualization issues






system("clear");			// clears the screen
} // end main








