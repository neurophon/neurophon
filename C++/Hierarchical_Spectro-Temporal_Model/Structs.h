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

#include <vector>
#include <string>

#include "../Libraries/DataTypes.h"

struct	responseInfo
{
	std::vector<int>	ranking;		// This is a vector with a ranking of the units' indexes distances (smaller distance first)
	std::vector<double>	distances;		// This is a vector with the distance between every unit and the inputVector
	std::vector<double>	excitation;		// This is a vector with the excitation on every unit due the inputVector
};

struct	layerResponse					// Holds layer's response information
{
	std::vector<double>	inputs;				// inputs to the first layer
								// every element is a coordinate
	twodvector<int>		currentIndexes;			// Current columns indexes for active units.
								// The first index corresponds to the column from which
								// information is received,
								// the second index corresponds to the unit index alternative
								// from the population that information is received.
	threedvector<int>	temporallyGatheredIndexes;	// Temporally gathered columns indexes for active units.
								// First index corresponds to the time step,
								// second index corresponds to the column from which
								// information is received, and
								// third index corresponds to the unit index alternative
								// from the population that information is received.
	threedvector<double>	coordinates;			// Columns coordinates for active units.
								// First index corresponds to the column from which
								// information is received,
								// second index corresponds to the unit index alternative
								// from the population that information is received, and
								// third index corresponds to the coordinate value that satisfies
								// the unit's position from the population from which information is received.
	std::vector<std::size_t>	sparseDistributedRepresentation;
								// Sparse Distributive Representation (SDR).
								// Every element contains an active element index from the binary unit representation.	
	std::vector<bool>	synchronization;		// Columns synchronization outputs
	std::vector<bool>	information;			// Columns information outputs
	twodvector<bool>	temporallyGatheredInformation;	// Temporally gathered column information
								// First index corresponds to the time step,
								// second index corresponds to the column from which
								// information is received.
};

struct	layerLearningParameters				// Specifies layer's learning parameters
{
	bool	enableProximalLearning;				// enable proximal synapses learning
	bool	enableDistalLearning;				// enable distal synapses learning
	bool	synapticHomeostasis;				// enable synaptic homeostasis in the learning process
	double	proximalLearningRate;				// learning rate for proximal synapses
	double	proximalNeighborhood;				// neighborhood for lateral interaction in the
								// learning process of the proximal synapses
	double	plasticity;					// percentage of population units affected by proximal
								// synapses
	bool	spikeTimeDependentSynapticPlasticity;		// enable the aplication of the Spike Time-Dependent
								// Synaptic Plasticity (STDP) phenomena
	double	distalLearningRate;				// learning rate for distal synapses
	double	distalNeighborhood;				// neighborhood for lateral interaction in the
	std::string	distalNeighborhoodFunction;		// distal neighborhood function to implement
								// lateral interaction in the update of distal synapses
								// learning process of the distal synapses
								// the options are: "gaussian" and "mex"
	double	distalSynapsesThreshold;			// if a distal synapsis is under this value,
								// it must be set to zero in the update period
	double	proximalInformationThreshold;			// information threshold from which
								// proximal synapses are modified
	double	limitsLearningRate;				// learning rate for the afferent and lateral
								// proximal input limits
};

struct	layerParameters					// Specifies layer's parameters
{
	int		numberOfBestResponses;			// Sets a maximum number of responses to be considered from all the responses
								// returned from SelfOrganizingMap.getResponse
	bool		enableLearning;				// Enables general learning in the layer
	bool		distalSensitivity;			// if true, then takes into account distal inputs
								// for the computation of of synchronizations and information
	bool		activationHomeostasis;			// enable homeostasis in the units' activation processes
	double		proximalInformationThreshold;		// Sets the proximal inputs' information threshold a column needs
								// in order to process it								
	double		distalInformationThreshold;		// Sets the distal inputs' information threshold a column needs
								// in order to process it
	double		activationRadius;			// Sets the minimum distance any unit in the population has to be from the
								// proximal input in order to be considered for activation issues
								// in TemporalPopulation.Activate
	double		sparsity;				// activation population sparsity
	std::string	selectionCriteria;			// Set the selection criteria of the best responses returned
								// by SelfOrganizingMap.getResponse,
								// the options are: "BMUsRanking" and "EnergyRanking".
	layerLearningParameters	learning;			// This is an structure with all the necessary parameters for
								// the learning process in the layer
};


#endif
