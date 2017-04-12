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

// File Name:		Layer.h
// File Description:	Layer class definition. This file presents Layer's public
//			interface without revealing the implementations of Layer's member functions,
//			which are defined in Layer.cpp.


// Class Description: ...

#ifndef LAYER_H
#define LAYER_H

#include <array>

#include "../Libraries/DataTypes.h"

#include "TemporalPopulation.h"

class Layer
{
public:

				Layer( const std::vector<int>&, const std::vector<int>&,
					const std::vector<int>&,
					const std::vector<int>&, const double, const bool,
					const std::vector<int>&, const double, const bool,
					const std::vector<int>&, const double, const bool,
					const std::vector<int>&, const double, const bool,
					int,
					const std::size_t,
					const std::vector<int>&,
					const std::vector<int>&,
					const std::vector<int>& );	// Layer constructor

				Layer( std::string& );			// Layer constructor with data from file

	void			validateLayer(  const std::vector<int>&, const std::vector<int>&,
						const std::vector<int>&,
						const std::vector<int>&, const double,
						const bool,
						const std::vector<int>&, const double,
						const bool,
						const std::vector<int>&, const double,
						const bool,
						const std::vector<int>&, const double,
						const bool,
						int,
						const std::size_t,
						const std::vector<int>&,
						const std::vector<int>&,
						const std::vector<int>& );// validates the layer

	void			validateComumnsInterconnectionsParameters( const std::vector<int>&, const std::vector<int>&,
									const std::vector<int>&,
									const std::vector<int>&, const double,
									const bool,
									const std::vector<int>&, const double,
									const bool,
									const std::vector<int>&, const double,
									const bool,
									const std::vector<int>&, const double,
									const bool,
									const std::size_t );// validates the parameters to configure the columns' interconnections

	void			validatePopulationParameters( const std::vector<int>&,
							const std::vector<int>&,
							const std::vector<int>&,
							const std::size_t );// validates the parameters used to generate the columns' populations

	void			interconnectLayerColumns( const std::vector<int>&, const std::vector<int>&,
							   const std::vector<int>&,
							   const std::vector<int>&, const double,
							   const bool,
							   const std::vector<int>&, const double,
							   const bool,
							   const std::vector<int>&, const double,
							   const bool,
							   const std::vector<int>&, const double,
							   const bool );// interconnects the layaer's populations

	void			generateColumns( const std::vector<int>&,
					      const std::vector<int>&,
					      const std::vector<int>&,
					      const std::size_t );		// generates a population for every column in the layer

	void			initializeProximalInputLimits( const std::size_t );	// initializes the proximal input limits from
										// afferent and lateral connections
	void			initializeInternalTemporallyGatheredInputs();	// initializes the internal temporal gathered inputs

	std::vector<layerResponse>	loadInputs( const std::string& );	// loads input information.
										// puts the inputs in a vector of layerResponse structures
										// every layerResponse structure corresponds to a time step
										// in the run

	layerResponse		computeResponse( const layerResponse&, const layerResponse&, const layerResponse&,
						const layerParameters& );	// computes the layer response. If learning is enabled
										// this function also modifies the synaptic weights in every population

	layerResponse		temporalGatherer( const layerResponse& );	// temporally gathers afferent inputs

	void			learn( const int, const std::vector<int>&,
					const std::vector<responseInfo>&,
					const twodvector<double>&,
					const std::vector<int>&, const twodvector<int>&,
					const double,
					const layerLearningParameters& );	// modifies proximal and distal synaptic weights in
										// certain population indexed by column agument

	layerResponse		gatherProximalInputs( const int, const layerResponse&,
						const layerResponse& );		// gathers proximal inputs from afferent and lateral connections

	layerResponse		gatherDistalInputs( const int, const layerResponse&,
						const layerResponse& );		// gathers distal inputs from apical and lateral connections

	void			computeProximalAlternatives( const threedvector<double>&,
					size_t, const std::vector<double>,
					twodvector<double>& );			// computes all proximal inputs alternatives

	void			computeDistalAlternatives( const twodvector<int>&,
					size_t, const std::vector<int>,
					twodvector<int>& );			// computes all distal inputs alternatives

	std::vector<double>	getRandomInputCoordinates( const std::vector<std::array<double, 2>>& );
										// gets random input coordinates between statistical limits


	void			updateProximalInputLimits( const layerResponse&,
							const layerResponse&,
							const layerLearningParameters& );	// updates the proximal input limits from afferent and lateral connections

	std::vector<int>	chooseBestResponsesIndexes( const std::vector<responseInfo>&,
							const int,
							const std::string& );	// choose a number of best responses alternatives, and
										// returns a vector with its indexes ordered in a ranking
										// such ranking depends on the selection criteria

	std::vector<int>	chooseBestIndexes( const twodvector<int>& );	// chooses the best indexes
										// In case that more than one index is returned,
										// the elements in the returned vector are
										// not ordered in a ranking

	std::vector<int>	getAfferentInputs( const int );		// gets the afferent input components that correspond to a column.
									// It takes the index of the column as an argument and determines
									// the flat indexes of the input vector that are located within the
									// column's afferent receptive field.

	std::vector<int>	getLateralProximalInputs( const int );		// gets the lateral proximal input components that correspond to a column.
									// It takes the index of the column as an argument and determines
									// the flat indexes of the input vector that are located within the
									// column's lateral proximal receptive field.

	std::vector<int>	getLateralDistalInputs( const int );		// gets the lateral distal input components that correspond to a column.
									// It takes the index of the column as an argument and determines
									// the flat indexes of the input vector that are located within the
									// column's lateral distal receptive field.

	std::vector<int>	getApicalInputs( const int );		// gets the apical input components that correspond to a column.
									// It takes the index of the column as an argument and determines
									// the flat indexes of the input vector that are located within the
									// column's apical receptive field.

	std::vector<int>	getNeighborhood( const int,
						 const bool,
						 const std::vector<int>&,
						 const std::vector<int>& );	// Gets a neighborhood of inputs.
									// Simply calls topology.wrappingNeighborhood or topology.neighborhood.
									// centerInput (int)
									// The center of the neighborhood.

	int	getReceptiveFieldCenter( const int,
					 const std::vector<int>& );	// gets the receptive field center corresponding to a column.
									// Such center is an input index.
									// It receives a column's index as an argument and determines
									// the flat index of the input vector that is the center of
									// the column's receptive field.
									// This function distributes the columns over the inputs
									// uniformly. The return value is an integer representing the index of the
									// input component.

	void	saveLayerStatus( const std::string& );			// function that saves the Layer's status in a file

	void	loadLayerStatus( const std::string& );			// function that loads the Layer's status from a file

	std::vector<double>	Normalize( const std::vector<int>&,
					const std::vector<int>& );	// normalizes neuron coordinates in the certain population
									// whose dimensionality information is provided as an
									// argument

private:
	std::vector<TemporalPopulation>		_layerColumns;

	std::size_t				_temporalGatheringAfferentValue;
	layerResponse				_internalTemporallyGatheredInputs;
	std::vector<std::size_t>		_temporalPointer;

	twodvector<int>				_afferentConnections;
	twodvector<int>				_lateralProximalConnections;
	twodvector<int>				_lateralDistalConnections;
	twodvector<int>				_apicalConnections;

	std::vector<int>			_populationsArrayDimensionality;
	std::vector<int>			_afferentPopulationsArrayDimensionality;
	std::vector<int>			_apicalPopulationsArrayDimensionality;

        std::vector<int>			_afferentArrayDimensionality;		// vector with the dimensions of the array of afferent inputs elements
        int					_afferentDimensionality;		// flat dimensionality of the aferent inputs' vector
	std::vector<int>			_afferentReceptiveField;		// the dimensions of the afferent receptive field
	double					_afferentPercentage;			// percentage of the afferent receptive field inputs that will be really connected
	bool					_afferentWrapAround;			// wrap around afferent inputs

        std::vector<int>			_apicalArrayDimensionality;		// vector with the dimensions of the array of apical inputs
        int					_apicalDimensionality;			// flat dimensionality of the apical inputs' vector
	std::vector<int>			_apicalReceptiveField;			// the dimensions of the apical receptive field
	double					_apicalPercentage;			// percentage of the apical receptive field inputs that will be really connected
	bool					_apicalWrapAround;			// wrap around apical inputs

        std::vector<int>			_columnsArrayDimensionality;		// vector with the dimensions of the array of columns
        int					_columnsDimensionality;			// flat dimensionality of the columns' vector

	std::vector<int>			_lateralProximalReceptiveField;		// the dimensions of the lateral proximal receptive field
	double					_lateralProximalPercentage;		// percentage of the lateral proximal receptive field inputs that will be really connected
	bool					_lateralProximalWrapAround;		// wrap around lateral proximal inputs

	std::vector<int>			_lateralDistalReceptiveField;		// the dimensions of the lateral distal receptive field
	double					_lateralDistalPercentage;		// percentage of the lateral distal receptive field inputs that will be really connected
	bool					_lateralDistalWrapAround;		// wrap around lateral distal inputs

	threedvector<std::array<double, 2>>	_proximalAfferentLimits;
	int					_afferentUpdateStep;
	twodvector<std::array<double, 2>>	_proximalLateralLimits;
	int					_lateralUpdateStep;
}; // end class Layer

#endif

