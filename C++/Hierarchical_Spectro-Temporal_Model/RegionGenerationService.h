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

// File Name:		RegionGenerationServiceNew.h
// File Description: This file contains a series of functions and definitions used by programs of node generation.

#include <stdio.h>

// This is for the region dimensionality
#define	INPUTREGION			28
#define	OUTPUTREGION			8

// This is for the nodes' dimensionality of the first hierarchical layer in the region
#define	H1_FORWARDINPUTNODE		11
#define	H1_FORWARDOUTPUTNODE		12
#define	H1_BACKWARDINPUTNODE		4
#define	H1_BACKWARDOUTPUTNODE		2
#define H1_BACKWARDEXTRAOUTPUTNODE	2

#define H1_CLUSTERDIMENSIONALITY	2
#define H1_CLUSTERGROUPDIMENSIONALITY	2
#define	H1_OUTPUTSPERCLUSTER		2
#define H1_OUTPUTSPERGROUPCLUSTER	2
#define H1_NEURONSPERCLUSTER		900 //30*30
#define H1_NEURONSPERGROUPCLUSTER	900 //30*30
#define H1_CLUSTERSPERFORWARDGROUP	2
#define H1_CLUSTERSPERBACKWARDGROUP	6
#define	H1_TEMPORALPOOLING		3

// This is for the nodes' dimensionality of the second hierarchical layer in the region
#define	H2_FORWARDINPUTNODE		24
#define	H2_FORWARDOUTPUTNODE		12
#define	H2_BACKWARDINPUTNODE		4
#define	H2_BACKWARDOUTPUTNODE		2
#define H2_BACKWARDEXTRAOUTPUTNODE	2

#define H2_CLUSTERDIMENSIONALITY	2
#define H2_CLUSTERGROUPDIMENSIONALITY	2
#define	H2_OUTPUTSPERCLUSTER		2
#define H2_OUTPUTSPERGROUPCLUSTER	2
#define H2_NEURONSPERCLUSTER		900 //30*30
#define H2_NEURONSPERGROUPCLUSTER	900 //30*30
#define H2_CLUSTERSPERFORWARDGROUP	2
#define H2_CLUSTERSPERBACKWARDGROUP	6
#define	H2_TEMPORALPOOLING		3

// This is for the nodes' dimensionality of the third hierarchical layer in the region
#define	H3_FORWARDINPUTNODE		24
#define	H3_FORWARDOUTPUTNODE		8
#define	H3_BACKWARDINPUTNODE		2
#define	H3_BACKWARDOUTPUTNODE		2
#define H3_BACKWARDEXTRAOUTPUTNODE	2

#define H3_CLUSTERDIMENSIONALITY	2
#define H3_CLUSTERGROUPDIMENSIONALITY	2
#define	H3_OUTPUTSPERCLUSTER		2
#define H3_OUTPUTSPERGROUPCLUSTER	2
#define H3_NEURONSPERCLUSTER		900 //30*30
#define H3_NEURONSPERGROUPCLUSTER	900 //30*30
#define H3_CLUSTERSPERFORWARDGROUP	2
#define H3_CLUSTERSPERBACKWARDGROUP	4
#define	H3_TEMPORALPOOLING		2

// General parameters
#define CLUSTERSPERFORWARDGROUP		2
#define CLUSTERSPERBACKWARDGROUP	6

struct connectome				// This structure will contain the pointers to every synaptic weights matrix in the node
{
	int	l4I;				// layer 4 cluster input
	int	l4U;				// layer 4 cluster units
	int	l4O;				// layer 4 cluster output
	double *l4W;				// layer 4 cluster weights
	double *l4P;				// layer 4 cluster prediction weights
	double *l4S;				// layer 4 cluster strong units

	int	l5I;				// layer 5 cluster input
	int	l5U;				// layer 5 cluster units
	int	l5O;				// layer 5 cluster output
	double *l5W;				// layer 5 cluster weights
	double *l5S;				// layer 5 cluster strong units

	int	l6I;				// layer 6 cluster input
	int	l6U;				// layer 6 cluster units
	int	l6O;				// layer 6 cluster output
	double *l6W;				// layer 6 cluster weights
	double *l6S;				// layer 6 cluster strong units

	int	l23FGAI;			// layer 23 forward group array cluster input
	int	l23FGAU;			// layer 23 forward group array cluster units
	int	l23FGAO;			// layer 23 forward group array cluster output
	int	l23FGC;				// layer 23 forward group clusters
	double *l23FGAW[CLUSTERSPERFORWARDGROUP];	// layer 23 forward group array cluster weights
	double *l23FGAS[CLUSTERSPERFORWARDGROUP];	// layer 23 forward group array cluster strong units

	int	l23BGAI;			// layer 23 backward group array cluster input
	int	l23BGAU;			// layer 23 forward group array cluster units
	int	l23BGAO;			// layer 23 backward group array cluster output
	int	l23BGC;				// layer 23 backward group clusters
	double *l23BGAW[CLUSTERSPERBACKWARDGROUP];	// layer 23 backward group array cluster weights
	double *l23BGAS[CLUSTERSPERBACKWARDGROUP];	// layer 23 backward group array cluster strong units
};

struct inputData				// This structure will contain the pointer to the input data matrix to feed the node
{
	int	filters;			// Number of MFCC
	int	chunks;				// Number of chunks in the signal

	double	*inputs;
};

// function to generate a two dimensional random matrix of weights
void	generateWeights( double, double, int, int, double * );

// function to print the title screen
void	titleScreen();

// function to save the connectome of the cortical node
void	saveConnectome( connectome, int, int );

// function to load the connectome of the cortical node
connectome	loadConnectome( int, int );

// function to pass to the new line from the file reading process
void	passNewLine(FILE *);

// function to save the connectome of the cortical node for visualization issues on Octave format
void	saveConnectomeForOctave( connectome, int, int );

// function to decompose an integer number in its individual digits
char	*digitsString( int );

// function to load the input data matrix
inputData	loadInputData();

// function to compute the energy variation between two matrices
double	deltaEnergy( double *, double *, int, int );


// function to save the energy evolution of the cortical node for visualization issues on Octave format
void	saveEnergyForOctave( double *, int, int );


// function to save the firing of a cluster for Octave visualization issues
void	saveFiringForOctave( double *, int, int, double *, int, int, std::string );










