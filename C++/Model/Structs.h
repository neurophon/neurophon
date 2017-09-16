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

#include "../Libraries/Model/DataTypes.h"

struct	somResponseInfo		// Self Organizing Map response information
{
	std::vector<std::size_t>	ranking;		// This is a vector with a ranking of the units' indexes distances (smaller distance first)
	std::vector<double>		distances;		// This is a vector with the distance between every unit and the inputVector
};

struct	responseInfo
{
	std::vector<std::size_t>	ranking;		// This is a vector with a ranking of the units' indexes excitation (smaller excitation first)
	std::vector<double>		excitation;		// This is a vector with the excitation on every unit due the inputVector
};

struct	encoderLayerInput					// Holds encoder's input information
{
	std::vector<double>	inputs;				// inputs to the encoder
								// every element is a coordinate
	std::vector<bool>	synchronization;		// input synchronization
	std::vector<bool>	information;			// input information
};

struct	encoderLayerProximalInput				// Holds encoder's proximal input information
{
	std::vector<double>	inputs;				// inputs to the encoder
								// every element is a coordinate
	std::vector<bool>	synchronization;		// input synchronization
	std::vector<bool>	information;			// input information
};

struct	encoderLayerDistalInput					// Holds encoder's distal input information
{
	twodvector<std::size_t>		activeIndexes;		// Active columns indexes for active units.
								// First index corresponds to the column from which
								// information is received,
								// second index corresponds to the unit index 
								// from the column that information is received.
	std::vector<bool>	synchronization;		// input synchronization for every column in the encoder
	std::vector<bool>	information;			// input information for every column in the encoder
};

struct	regularLayerProximalInput					// Holds encoder's proximal input information
{
	std::vector<std::size_t>	sparseDistributedRepresentation;
								// Sparse Distributive Representation (SDR).
	std::vector<bool>	synchronization;		// input synchronization
	std::vector<bool>	information;			// input information
};

struct	regularLayerDistalInput					// Holds encoder's distal input information
{
	twodvector<std::size_t>		activeIndexes;			// Active columns indexes for active units.
								// First index corresponds to the column from which
								// information is received,
								// second index corresponds to the unit index 
								// from the column that information is received.
	std::vector<bool>	synchronization;		// input synchronization for every column in the encoder
	std::vector<bool>	information;			// input information for every column in the encoder
};

struct	regularLayerResponse					// Holds layer's response information
{
	twodvector<std::size_t>		currentIndexes;			// Current columns indexes for active units.
								// First index corresponds to the column from which
								// information is received,
								// second index corresponds to the unit index alternative
								// from the column that information is received.
	std::vector<bool>	synchronization;		// Columns synchronization outputs
	std::vector<bool>	information;			// Columns information outputs
};

struct	regularLayerTemporallyGatheredResponse			// Holds layer's response information
{
	threedvector<std::size_t>	temporallyGatheredIndexes;	// Temporally gathered columns indexes for active units.
								// First index corresponds to the time step,
								// second index corresponds to the column from which
								// information is received, and
								// third index corresponds to the unit index alternative
								// from the population that information is received.
	std::vector<bool>	synchronization;		// Columns synchronization outputs
	twodvector<bool>	temporallyGatheredInformation;	// Temporally gathered column information
								// First index corresponds to the time step,
								// second index corresponds to the column from which
								// information is received.
};

struct	regularLayerLearningParameters			// Specifies layer's learning parameters
{
	bool	enableProximalLearning;				// enable proximal synapses learning
	bool	enableDistalLearning;				// enable distal synapses learning
	bool	synapticHomeostasis;				// enable synaptic homeostasis in the learning process
	double	proximalLearningRate;				// learning rate for proximal synapses
	double	proximalNeighborhood;				// neighborhood for lateral interaction in the
								// learning process of the proximal synapses
	double	plasticity;					// percentage of population units affected by proximal
								// synapses
	bool	spikeTimeDependentSynapticPlasticity;		// enable the application of the Spike Time-Dependent
								// Synaptic Plasticity (STDP) phenomena
	double	distalLearningRate;				// learning rate for distal synapses
};

struct	regularLayerParameters				// Specifies layer's parameters
{
	bool		enableLearning;				// Enables general learning in the layer
	bool		distalSensitivity;			// if true, then takes into account distal inputs
								// for the computation of of synchronizations and information
	bool		activationHomeostasis;			// enable homeostasis in the units' activation processes
	double		proximalInformationThreshold;		// Sets the proximal inputs' information threshold a column needs
								// in order to process it								
	double		distalInformationThreshold;		// Sets the distal inputs' information threshold a column needs
								// in order to process it
	double		sparsity;				// activation population sparsity
	regularLayerLearningParameters	learning;		// This is an structure with all the necessary parameters for
								// the learning process in the layer
};

struct	encoderLayerLearningParameters			// Specifies layer's learning parameters
{
	bool	enableProximalLearning;				// enable proximal synapses learning
	bool	enableDistalLearning;				// enable distal synapses learning
	double	proximalLearningRate;				// learning rate for proximal synapses
	double	proximalNeighborhood;				// neighborhood for lateral interaction in the
								// learning process of the proximal synapses
	bool	spikeTimeDependentSynapticPlasticity;		// enable the application of the Spike Time-Dependent
								// Synaptic Plasticity (STDP) phenomena
	double	distalLearningRate;				// learning rate for distal synapses
	double	limitsLearningRate;				// learning rate for the afferent and lateral
								// proximal input limits
};

struct	encoderLayerParameters				// Specifies layer's parameters
{
	bool		enableLearning;				// Enables general learning in the layer
	bool		distalSensitivity;			// if true, then takes into account distal inputs
								// for the computation of of synchronizations and information
	double		proximalInformationThreshold;		// Sets the proximal inputs' information threshold a column needs
								// in order to process it								
	double		distalInformationThreshold;		// Sets the distal inputs' information threshold a column needs
								// in order to process it
	double		activationRadius;			// Sets the minimum distance any unit in the population has to be from the
								// proximal input in order to be considered for activation issues
								// in Activate function in ComplexEncoder or in ComplexProcessor
	double		sparsity;				// activation population sparsity
	encoderLayerLearningParameters	learning;		// This is an structure with all the necessary parameters for
								// the learning process in the layer
};

struct	regularLayerStructure					// Specifies layer's structure
{
	std::vector<std::size_t>	afferentArrayDimensionality;		// a vector with the dimensions of the afferent array
	std::vector<std::size_t>	apicalArrayDimensionality;		// a vector with the dimensions of the apical array
	std::vector<std::size_t>	columnsArrayDimensionality;		// a vector with the dimensions of these cortical columns array

	std::vector<std::size_t>	afferentReceptiveField;			// a vector with the dimensions of the afferent receptive field
	double				afferentPercentage;			// percentage of the afferent connections that are effective
	bool				afferentWrapAround;

	std::vector<std::size_t>	lateralProximalReceptiveField;		// a vector with the dimensions of the lateral proximal receptive field
	double				lateralProximalPercentage;		// percentage of the lateral proximal connections that are effective
	bool				lateralProximalWrapAround;

	std::vector<std::size_t>	lateralDistalReceptiveField;		// a vector with the dimensions of the lateral distal receptive field
	double				lateralDistalPercentage;		// percentage of the lateral distal connections that are effective
	bool				lateralDistalWrapAround;

	std::vector<std::size_t>	apicalReceptiveField;			// a vector with the dimensions of the apical receptive field
	double				apicalPercentage;			// percentage of the apical connections that are effective
	bool				apicalWrapAround;

	std::size_t			iterationNum = 0;

	std::vector<std::size_t>	populationsArrayDimensionality;		// a vector with the dimensions of the populations array
	std::vector<std::size_t>	afferentPopulationsArrayDimensionality;	// a vector with the dimensions of the afferent populations array
	std::vector<std::size_t>	apicalPopulationsArrayDimensionality;	// a vector with the dimensions of the apical populations array

	std::size_t			temporalGatheringAfferentValue;
	double				potentialPercentage;			// the percentage of potential connections that a unit takes from a column
};

struct	encoderLayerStructure					// Specifies encoder's structure
{
	std::vector<std::size_t>	afferentArrayDimensionality;		// a vector with the dimensions of the afferent array
	std::vector<std::size_t>	apicalArrayDimensionality;		// a vector with the dimensions of the apical array
	std::vector<std::size_t>	columnsArrayDimensionality;		// a vector with the dimensions of these cortical columns array

	std::vector<std::size_t>	afferentReceptiveField;			// a vector with the dimensions of the afferent receptive field
	double				afferentPercentage;			// percentage of the afferent connections that are effective
	bool				afferentWrapAround;

	std::vector<std::size_t>	lateralDistalReceptiveField;		// a vector with the dimensions of the lateral distal receptive field
	double				lateralDistalPercentage;		// percentage of the lateral distal connections that are effective
	bool				lateralDistalWrapAround;

	std::vector<std::size_t>	apicalReceptiveField;			// a vector with the dimensions of the apical receptive field
	double				apicalPercentage;			// percentage of the apical connections that are effective
	bool				apicalWrapAround;

	std::size_t			iterationNum = 0;

	std::vector<std::size_t>	populationsArrayDimensionality;		// a vector with the dimensions of the populations array
	std::vector<std::size_t>	apicalPopulationsArrayDimensionality;	// a vector with the dimensions of the apical populations array
	double				potentialPercentage;			// the percentage of potential connections that a unit takes from a column
};

struct	modelStructure						// Specifies the complete model structure
{
	bool					encoderIncorporation;		// indicates if an encoder is incorporated to the model  
										// true: encoder incorporated as a initial layer
										// false: encoder is not incorporated
	bool					newEncoder;			// indicates if the encoder has to be created
										// true: a new encoder has to be created
										// false: the encoder has to be loaded from file
	std::size_t				numberOfLayers;			// the number of layers in the model 0-n
	std::size_t				newLayerAt;			// indicates from which level in the hierarchy a new layer has to be
										// created. Layers below this one have to be loaded from file
};

#endif