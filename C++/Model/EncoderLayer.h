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

// File Name:		EncoderLayer.h
// File Description:	EncoderLayer class definition. This file presents EncoderLayer's public
//			interface without revealing the implementations of EncoderLayer's member functions,
//			which are defined in EncoderLayer.cpp.


// Class Description: ...

#ifndef ENCODER_LAYER_H
#define ENCODER_LAYER_H

#include <array>

#include "../Libraries/Model/DataTypes.h"

#include "ComplexSelfOrganizingMap.h"

class EncoderLayer
{
public:

				EncoderLayer(); // EncoderLayer default constructor

				EncoderLayer( const encoderLayerStructure& );	// EncoderLayer constructor

				EncoderLayer( const std::string& );		// EncoderLayer constructor with data from file

	void			encoderInitializer( const encoderLayerStructure& );	// initializes an object instantiation of this class

	void			initializeWiredVariables();	//this function initializes wired variables

	void			validateEncoderLayer( const std::vector<std::size_t>&,
						      const std::vector<std::size_t>&,
						      const std::vector<std::size_t>&,
						      const std::vector<std::size_t>&,
						      const double,
						      const bool,
						      const std::vector<std::size_t>&,
						      const double,
						      const bool,
						      const std::vector<std::size_t>&,
						      const double,
						      const bool,
						      std::size_t,
						      const std::vector<std::size_t>&,
						      const std::vector<std::size_t>&,
		       				      const double );// validates the encoder

	bool			validateColumnsInterconnectionsParameters( const std::vector<std::size_t>&,
									   const std::vector<std::size_t>&,
									   const std::vector<std::size_t>&,
									   const std::vector<std::size_t>&,
									   const double,
									   const bool,
									   const std::vector<std::size_t>&,
									   const double,
									   const bool,
									   const std::vector<std::size_t>&,
									   const double,
									   const bool );// validates the parameters to configure the columns' interconnections
											// returns true if there is some inconsistence

	bool			validatePopulationParameters( const std::vector<std::size_t>&,
							      const std::vector<std::size_t>&,
		       					      const double );// validates the parameters used to generate the columns' populations

	void			interconnectEncoderLayerColumns( const std::vector<std::size_t>&,
								 const std::vector<std::size_t>&,
								 const std::vector<std::size_t>&,
								 const std::vector<std::size_t>&,
								 const double,
								 const bool,
								 const std::vector<std::size_t>&,
								 const double,
								 const bool,
								 const std::vector<std::size_t>&,
								 const double,
								 const bool );// interconnects the layaer's populations

	void			generateColumns( const std::vector<std::size_t>&,
						 const std::vector<std::size_t>&,
		       				 const double );		// generates a population for every column in the encoder

	void			initializeProximalInputLimits();		// initializes the proximal input limits from
										// afferent and lateral connections

	regularLayerResponse		computeResponse( const encoderLayerInput&,
							 const encoderLayerParameters& );	// computes the encoder response. If learning is enabled
										// this function also modifies the synaptic weights in every population

	regularLayerResponse		computeResponse( const encoderLayerInput&,
							 const regularLayerResponse&,
							 const encoderLayerParameters& );	// computes the encoder response. If learning is enabled
										// this function also modifies the synaptic weights in every population

	regularLayerResponse		computeResponse( const encoderLayerInput&,
							 const regularLayerResponse&,
							 const regularLayerResponse&,
							 const encoderLayerParameters& );	// computes the encoder response. If learning is enabled
										// this function also modifies the synaptic weights in every population

	encoderLayerProximalInput	gatherProximalInputs( const std::size_t,
							      const encoderLayerInput& );// gathers proximal inputs from afferent and lateral connections

	encoderLayerDistalInput		gatherDistalInputs( const std::size_t,
							    const regularLayerResponse&,
							    const regularLayerResponse& );	// gathers distal inputs from apical and lateral connections

	void			updateProximalInputLimits( const encoderLayerInput&,
							   const encoderLayerLearningParameters& );	// updates the proximal input limits
       													// from afferent and lateral connections

	std::vector<std::size_t>	getAfferentInputs( const std::size_t );		// gets the afferent input components that correspond to a column.
									// It takes the index of the column as an argument and determines
									// the flat indexes of the input vector that are located within the
									// column's afferent receptive field.

	std::vector<std::size_t>	getLateralDistalInputs( const std::size_t );		// gets the lateral distal input components that correspond to a column.
									// It takes the index of the column as an argument and determines
									// the flat indexes of the input vector that are located within the
									// column's lateral distal receptive field.

	std::vector<std::size_t>	getApicalInputs( const std::size_t );		// gets the apical input components that correspond to a column.
									// It takes the index of the column as an argument and determines
									// the flat indexes of the input vector that are located within the
									// column's apical receptive field.

	std::vector<std::size_t>	getNeighborhood( const std::size_t,
							 const bool,
							 const std::vector<std::size_t>&,
							 const std::vector<std::size_t>& );	// Gets a neighborhood of inputs.
									// Simply calls topology.wrappingNeighborhood or topology.neighborhood.
									// centerInput
									// The center of the neighborhood.

	std::size_t	getReceptiveFieldCenter( const std::size_t,
						 const std::vector<std::size_t>& );	// gets the receptive field center corresponding to a column.
									// Such center is an input index.
									// It receives a column's index as an argument and determines
									// the flat index of the input vector that is the center of
									// the column's receptive field.
									// This function distributes the columns over the inputs
									// uniformly. The return value is an integer representing the index of the
									// input component.

	void	saveEncoderLayerStatus( const std::string& );			// function that saves the EncoderLayer's status in a file

	void	loadEncoderLayerStatus( const std::string& );			// function that loads the EncoderLayer's status from a file

	void	checkEncoderLayerStructure( const encoderLayerStructure& );	// this function checks the coeherence of the encoder layer structure

	void	mergeOutputs( regularLayerResponse& output );			// this function merges all the column outputs,
										// this uses mpi inter-process-comunication
										// once the function returns, every process count on a complete
										// cortical layer output

	connectionsSet	gatherConnections();					// gathers all connections in rank 0

private:
	std::vector<ComplexSelfOrganizingMap>		_encoderColumns;

	twodvector<std::size_t>				_afferentConnections;
	twodvector<std::size_t>				_lateralDistalConnections;
	twodvector<std::size_t>				_apicalConnections;

	std::vector<std::size_t>			_populationsArrayDimensionality;
	std::vector<std::size_t>			_apicalPopulationsArrayDimensionality;
	double						_potentialPercentage;

        std::vector<std::size_t>			_afferentArrayDimensionality;		// vector with the dimensions of the array of afferent inputs elements
	std::size_t					_afferentDimensionality;		// flat dimensionality of the aferent inputs' vector
	std::vector<std::size_t>			_afferentReceptiveField;		// the dimensions of the afferent receptive field
	double						_afferentPercentage;			// percentage of the afferent receptive field inputs that will 
												// be really connected
	bool						_afferentWrapAround;			// wrap around afferent inputs

        std::vector<std::size_t>			_apicalArrayDimensionality;		// vector with the dimensions of the array of apical inputs
	std::size_t					_apicalDimensionality;			// flat dimensionality of the apical inputs' vector
	std::vector<std::size_t>			_apicalReceptiveField;			// the dimensions of the apical receptive field
	double						_apicalPercentage;			// percentage of the apical receptive field inputs that will be
       												// really connected
	bool						_apicalWrapAround;			// wrap around apical inputs

        std::vector<std::size_t>			_columnsArrayDimensionality;		// vector with the dimensions of the array of columns
	std::size_t					_columnsDimensionality;			// flat dimensionality of the columns' vector

	std::vector<std::size_t>			_lateralDistalReceptiveField;		// the dimensions of the lateral distal receptive field
	double						_lateralDistalPercentage;		// percentage of the lateral distal receptive field inputs that
       												// will be really connected
	bool						_lateralDistalWrapAround;		// wrap around lateral distal inputs

	std::vector<std::array<double, 2>>		_proximalAfferentLimits;		// limits between which a random input is chosen 
												// when there is no input information
	std::size_t					_afferentUpdateStep;			// counts the time steps in the process

	regularLayerResponse				_lateral;				// wired variable for lateral inputs to the layer
	regularLayerResponse				_apical;				// wired variable for apical inputs to the layer
}; // end class EncoderLayer

#endif

