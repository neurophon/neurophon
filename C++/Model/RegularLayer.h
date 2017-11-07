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

// File Name:		RegularLayer.h
// File Description:	RegularLayer class definition. This file presents RegularLayer's public
//			interface without revealing the implementations of RegularLayer's member functions,
//			which are defined in RegularLayer.cpp.


// Class Description: ...

#ifndef REGULAR_LAYER_H
#define REGULAR_LAYER_H

#include <array>

#include "../Libraries/Model/DataTypes.h"

#include "ComplexProcessor.h"

class RegularLayer
{
public:

				RegularLayer(); // RegularLayer default constructor

				RegularLayer( const regularLayerStructure& );	// RegularLayer constructor

				RegularLayer( const std::string&,
					      const std::size_t );	// RegularLayer constructor with data from file

	void			layerInitializer( const regularLayerStructure&	);// initializes an object instantiation of this class

	void			initializeWiredVariables();	//this function initializes wired variables

	void			validateRegularLayer( const std::vector<std::size_t>&,
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
						      const std::vector<std::size_t>&,
						      const double,
						      const bool,
						      std::size_t,
						      const std::size_t,
						      const std::vector<std::size_t>&,
						      const std::vector<std::size_t>&,
						      const std::vector<std::size_t>&,
						      const double );// validates the layer

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
									   const bool,
									   const std::vector<std::size_t>&,
									   const double,
									   const bool,
									   const std::size_t );// validates the parameters to configure the columns' interconnections

	bool			validatePopulationParameters( const std::vector<std::size_t>&,
							      const std::vector<std::size_t>&,
							      const std::vector<std::size_t>&,
							      const std::size_t,
		       					      const double );	// validates the parameters used to generate the columns' populations

	void			interconnectRegularLayerColumns( const std::vector<std::size_t>&,
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
								 const std::vector<std::size_t>&,
								 const double,
								 const bool );// interconnects the layaer's populations

	void			generateColumns( const std::vector<std::size_t>&,
						 const std::vector<std::size_t>&,
						 const std::vector<std::size_t>&,
						 const std::size_t,
		       				 const double );		// generates a population for every column in the layer

	void			initializeInternalTemporallyGatheredInputs();	// initializes the internal temporal gathered inputs

	regularLayerResponse		computeResponse( const regularLayerResponse&,
							 const regularLayerParameters& );	// computes the refular response. If learning is enabled
										// this function also modifies the synaptic weights in every population

	regularLayerResponse		computeResponse( const regularLayerResponse&,
							 const regularLayerResponse&,
							 const regularLayerParameters& );	// computes the refular response. If learning is enabled
										// this function also modifies the synaptic weights in every population

	regularLayerResponse		computeResponse( const regularLayerResponse&,
							 const regularLayerResponse&,
							 const regularLayerResponse&,
							 const regularLayerParameters& );	// computes the layer response. If learning is enabled
										// this function also modifies the synaptic weights in every population

	regularLayerTemporallyGatheredResponse		temporalGatherer( const regularLayerResponse& );	// temporally gathers afferent inputs

	regularLayerProximalInput	gatherProximalInputs( const std::size_t,
							      const regularLayerResponse&,
							      const regularLayerResponse& );		// gathers proximal inputs from afferent and lateral connections

	regularLayerProximalInput	gatherProximalInputs( const std::size_t,
							      const regularLayerTemporallyGatheredResponse&,
							      const regularLayerResponse& );		// gathers proximal inputs from afferent and lateral connections


	regularLayerDistalInput	gatherDistalInputs( const std::size_t,
						    const regularLayerResponse&,
						    const regularLayerResponse& );		// gathers distal inputs from apical and lateral connections

	std::vector<std::size_t>	getAfferentInputs( const std::size_t );		// gets the afferent input components that correspond to a column.
									// It takes the index of the column as an argument and determines
									// the flat indexes of the input vector that are located within the
									// column's afferent receptive field.

	std::vector<std::size_t>	getLateralProximalInputs( const std::size_t );		// gets the lateral proximal input components that correspond to a column.
									// It takes the index of the column as an argument and determines
									// the flat indexes of the input vector that are located within the
									// column's lateral proximal receptive field.

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
									// centerInput (int)
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

	void	saveRegularLayerStatus( const std::string&,
					const std::size_t );		// function that saves the RegularLayer's status in a file

	void	loadRegularLayerStatus( const std::string&,
		       			const std::size_t );		// function that loads the RegularLayer's status from a file

	void	checkRegularLayerStructure( const regularLayerStructure& );	// this function checks the coeherence of the regular layer structure
	
	void	mergeOutputs( regularLayerResponse& output );			// this function merges all the column outputs,
										// this uses mpi inter-process-comunication
										// once the function returns, every process count on a complete
										// cortical layer output

	void	gatherConnections();						// gathers all connections in rank 0

private:
	std::vector<ComplexProcessor>		_layerColumns;

	std::size_t				_temporalGatheringAfferentValue;
	regularLayerTemporallyGatheredResponse	_internalTemporallyGatheredInputs;
	std::vector<std::size_t>		_temporalPointer;

	twodvector<std::size_t>			_afferentConnections;
	twodvector<std::size_t>			_lateralProximalConnections;
	twodvector<std::size_t>			_lateralDistalConnections;
	twodvector<std::size_t>			_apicalConnections;

	std::vector<std::size_t>		_populationsArrayDimensionality;
	std::vector<std::size_t>		_afferentPopulationsArrayDimensionality;
	std::vector<std::size_t>		_apicalPopulationsArrayDimensionality;
	double					_potentialPercentage;

        std::vector<std::size_t>		_afferentArrayDimensionality;		// vector with the dimensions of the array of afferent inputs elements
	std::size_t				_afferentDimensionality;		// flat dimensionality of the aferent inputs' vector
	std::vector<std::size_t>		_afferentReceptiveField;		// the dimensions of the afferent receptive field
	double					_afferentPercentage;			// percentage of the afferent receptive field inputs that will be really connected
	bool					_afferentWrapAround;			// wrap around afferent inputs

        std::vector<std::size_t>		_apicalArrayDimensionality;		// vector with the dimensions of the array of apical inputs
	std::size_t				_apicalDimensionality;			// flat dimensionality of the apical inputs' vector
	std::vector<std::size_t>		_apicalReceptiveField;			// the dimensions of the apical receptive field
	double					_apicalPercentage;			// percentage of the apical receptive field inputs that will be really connected
	bool					_apicalWrapAround;			// wrap around apical inputs

        std::vector<std::size_t>		_columnsArrayDimensionality;		// vector with the dimensions of the array of columns
	std::size_t				_columnsDimensionality;			// flat dimensionality of the columns' vector

	std::vector<std::size_t>		_lateralProximalReceptiveField;		// the dimensions of the lateral proximal receptive field
	double					_lateralProximalPercentage;		// percentage of the lateral proximal receptive field inputs that will be really connected
	bool					_lateralProximalWrapAround;		// wrap around lateral proximal inputs

	std::vector<std::size_t>		_lateralDistalReceptiveField;		// the dimensions of the lateral distal receptive field
	double					_lateralDistalPercentage;		// percentage of the lateral distal receptive field inputs that will be really connected
	bool					_lateralDistalWrapAround;		// wrap around lateral distal inputs

	regularLayerResponse			_lateral;				// wired variable for lateral inputs to the layer
	regularLayerResponse			_apical;				// wired variable for apical inputs to the layer
}; // end class RegularLayer

#endif

