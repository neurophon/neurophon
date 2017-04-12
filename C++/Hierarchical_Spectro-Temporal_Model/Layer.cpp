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

// File Name:		Layer.cpp
// File Description:	Layer member-function definitions. This file contains implementations of the
//			member functions prototyped in Layer.h.

#include <iostream>
#include <fstream>
#include <algorithm>

#include "../Libraries/Random.h"
#include "../Libraries/Topology.h"
#include "../Libraries/Utilities.h"
#include "../Libraries/Templates.h"
#include "../Libraries/OctaveInterface.h"
#include "Layer.h"

using namespace std;


// constructor that initializes ...
Layer::Layer( const std::vector<int>& afferentArrayDimensionality, const std::vector<int>& apicalArrayDimensionality,
	      const std::vector<int>& columnsArrayDimensionality,
	      const std::vector<int>& afferentReceptiveField, const double afferentPercentage,
	      const bool afferentWrapAround,
	      const std::vector<int>& lateralProximalReceptiveField, const double lateralProximalPercentage,
	      const bool lateralProximalWrapAround,
	      const std::vector<int>& lateralDistalReceptiveField, const double lateralDistalPercentage,
	      const bool lateralDistalWrapAround,
	      const std::vector<int>& apicalReceptiveField, const double apicalPercentage,
	      const bool apicalWrapAround,
	      int iterationNum,
	      const std::size_t temporalGatheringAfferentValue,
	      const std::vector<int>& populationsArrayDimensionality,
	      const std::vector<int>& afferentPopulationsArrayDimensionality,
	      const std::vector<int>& apicalPopulationsArrayDimensionality )
{
	Layer::validateLayer(afferentArrayDimensionality, apicalArrayDimensionality,
			     columnsArrayDimensionality,
			     afferentReceptiveField, afferentPercentage,
			     afferentWrapAround,
			     lateralProximalReceptiveField, lateralProximalPercentage,
			     lateralProximalWrapAround,
			     lateralDistalReceptiveField, lateralDistalPercentage,
			     lateralDistalWrapAround,
			     apicalReceptiveField, apicalPercentage,
			     apicalWrapAround,
			     iterationNum,
			     temporalGatheringAfferentValue,
		      	     populationsArrayDimensionality,
			     afferentPopulationsArrayDimensionality,
			     apicalPopulationsArrayDimensionality);

	Layer::interconnectLayerColumns(afferentArrayDimensionality, apicalArrayDimensionality,
					 columnsArrayDimensionality,
					 afferentReceptiveField, afferentPercentage,
					 afferentWrapAround,
					 lateralProximalReceptiveField, lateralProximalPercentage,
					 lateralProximalWrapAround,
					 lateralDistalReceptiveField, lateralDistalPercentage,
					 lateralDistalWrapAround,
					 apicalReceptiveField, apicalPercentage,
					 apicalWrapAround);

	Layer::generateColumns(populationsArrayDimensionality,
			      afferentPopulationsArrayDimensionality,
			      apicalPopulationsArrayDimensionality,
			      temporalGatheringAfferentValue);

	_temporalGatheringAfferentValue = temporalGatheringAfferentValue;

	_afferentUpdateStep = 0;
	_lateralUpdateStep = 0;

	Layer::initializeProximalInputLimits(temporalGatheringAfferentValue);

	if ( _temporalGatheringAfferentValue > 1 )
		Layer::initializeInternalTemporallyGatheredInputs();
} // end Layer constructor


// constructor that initializes ...
Layer::Layer( std::string& fileName )
{
	Layer::loadLayerStatus(fileName);
} // end Layer constructor


// validates the layer
void	Layer::validateLayer(  const std::vector<int>& afferentArrayDimensionality, const std::vector<int>& apicalArrayDimensionality,
			      const std::vector<int>& columnsArrayDimensionality,
			      const std::vector<int>& afferentReceptiveField, const double afferentPercentage,
			      const bool afferentWrapAround,
			      const std::vector<int>& lateralProximalReceptiveField, const double lateralProximalPercentage,
			      const bool lateralProximalWrapAround,
			      const std::vector<int>& lateralDistalReceptiveField, const double lateralDistalPercentage,
			      const bool lateralDistalWrapAround,
			      const std::vector<int>& apicalReceptiveField, const double apicalPercentage,
			      const bool apicalWrapAround,
			      int iterationNum,
			      const std::size_t temporalGatheringAfferentValue,
			      const std::vector<int>&	populationsArrayDimensionality,
			      const std::vector<int>&	afferentPopulationsArrayDimensionality,
			      const std::vector<int>&	apicalPopulationsArrayDimensionality )
{
	Layer::validateComumnsInterconnectionsParameters(afferentArrayDimensionality, apicalArrayDimensionality,
							 columnsArrayDimensionality,
							 afferentReceptiveField, afferentPercentage,
							 afferentWrapAround,
							 lateralProximalReceptiveField, lateralProximalPercentage,
							 lateralProximalWrapAround,
							 lateralDistalReceptiveField, lateralDistalPercentage,
							 lateralDistalWrapAround,
							 apicalReceptiveField, apicalPercentage,
							 apicalWrapAround, temporalGatheringAfferentValue);

	Layer::validatePopulationParameters(populationsArrayDimensionality,
					afferentPopulationsArrayDimensionality,
					apicalPopulationsArrayDimensionality,
					temporalGatheringAfferentValue);

} // end function validateLayer


// validates the parameters to configure the columns' interconnections
void	Layer::validateComumnsInterconnectionsParameters( const std::vector<int>& afferentArrayDimensionality, const std::vector<int>& apicalArrayDimensionality,
							const std::vector<int>& columnsArrayDimensionality,
							const std::vector<int>& afferentReceptiveField, const double afferentPercentage,
							const bool afferentWrapAround,
							const std::vector<int>& lateralProximalReceptiveField, const double lateralProximalPercentage,
							const bool lateralProximalWrapAround,
							const std::vector<int>& lateralDistalReceptiveField, const double lateralDistalPercentage,
							const bool lateralDistalWrapAround,
							const std::vector<int>& apicalReceptiveField, const double apicalPercentage,
							const bool apicalWrapAround, const std::size_t temporalGatheringAfferentValue )
{
	if ( temporalGatheringAfferentValue == 0 ) {
		std::cout << "Layer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";
		std::cout << "temporalGatheringAfferentValue = " << temporalGatheringAfferentValue << "\n";
		exit( EXIT_FAILURE );
	}

	if ( afferentArrayDimensionality.size() == 0 ||
	     apicalArrayDimensionality.size() == 0 ||
	     columnsArrayDimensionality.size() == 0 ) {
		std::cout << "Layer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";
		std::cout << "afferentArrayDimensionality.size() = " << afferentArrayDimensionality.size() << "\n";
		std::cout << "apicalArrayDimensionality.size() = " << apicalArrayDimensionality.size() << "\n";
		std::cout << "columnsArrayDimensionality.size() = " << columnsArrayDimensionality.size() << "\n";
		exit( EXIT_FAILURE );
	}

	for ( int dim = 0; dim < (int)afferentArrayDimensionality.size(); dim++ ) {
		if ( afferentArrayDimensionality[dim] <= 0 ) {
			std::cout << "Layer object construction inconsistence: \n";
			std::cout << "In function validateComumnsInterconnectionsParameters\n";
			std::cout << "afferentArrayDimensionality[" << dim << "] = " << afferentArrayDimensionality[dim] << "\n";
			exit( EXIT_FAILURE );
		}
	}

	for ( int dim = 0; dim < (int)apicalArrayDimensionality.size(); dim++ ) {
		if ( apicalArrayDimensionality[dim] <= 0 ) {
			std::cout << "Layer object construction inconsistence: \n";
			std::cout << "In function validateComumnsInterconnectionsParameters\n";
			std::cout << "apicalArrayDimensionality[" << dim << "] = " << apicalArrayDimensionality[dim] << "\n";
			exit( EXIT_FAILURE );
		}
	}

	for ( int dim = 0; dim < (int)columnsArrayDimensionality.size(); dim++ ) {
		if ( columnsArrayDimensionality[dim] <= 0 ) {
			std::cout << "Layer object construction inconsistence: \n";
			std::cout << "In function validateComumnsInterconnectionsParameters\n";
			std::cout << "columnsArrayDimensionality[" << dim << "] = " << columnsArrayDimensionality[dim] << "\n";
			exit( EXIT_FAILURE );
		}
	}

	if ( afferentArrayDimensionality.size() != afferentReceptiveField.size() ||
	     apicalArrayDimensionality.size() != apicalReceptiveField.size() ||
	     columnsArrayDimensionality.size() != lateralProximalReceptiveField.size() ||
	     columnsArrayDimensionality.size() != lateralDistalReceptiveField.size() ) {
		std::cout << "Layer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";

		std::cout << "afferentArrayDimensionality.size() = " << afferentArrayDimensionality.size() << "\n";
		std::cout << "while, afferentReceptiveField.size() = " << afferentReceptiveField.size() << "\n";

		std::cout << "apicalArrayDimensionality.size() = " << apicalArrayDimensionality.size() << "\n";
		std::cout << "while, apicalReceptiveField.size() = " << apicalReceptiveField.size() << "\n";

		std::cout << "columnsArrayDimensionality.size() = " << columnsArrayDimensionality.size() << "\n";
		std::cout << "while, lateralProximalReceptiveField.size() = " << lateralProximalReceptiveField.size() << "\n";
		std::cout << "and lateralDistalReceptiveField.size() = " << lateralDistalReceptiveField.size() << "\n";
		exit( EXIT_FAILURE );
	}

	std::vector<int>	test;
	test.assign(afferentReceptiveField.size(),-1);
	if ( afferentReceptiveField != test ) {
		for ( int dim = 0; dim < (int)afferentReceptiveField.size(); dim++ ) {
			if ( afferentReceptiveField[dim] < 0 ) {
				std::cout << "Layer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "afferentReceptiveField[" << dim << "] = " << afferentReceptiveField[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	test.assign(apicalReceptiveField.size(),-1);
	if ( apicalReceptiveField != test ) {
		for ( int dim = 0; dim < (int)apicalReceptiveField.size(); dim++ ) {
			if ( apicalReceptiveField[dim] < 0 ) {
				std::cout << "Layer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "apicalReceptiveField[" << dim << "] = " << apicalReceptiveField[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	test.assign(lateralProximalReceptiveField.size(),-1);
	if ( lateralProximalReceptiveField != test ) {
		for ( int dim = 0; dim < (int)lateralProximalReceptiveField.size(); dim++ ) {
			if ( lateralProximalReceptiveField[dim] < 0 ) {
				std::cout << "Layer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "lateralProximalReceptiveField[" << dim << "] = " << lateralProximalReceptiveField[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	test.assign(lateralDistalReceptiveField.size(),-1);
	if ( lateralDistalReceptiveField != test ) {
		for ( int dim = 0; dim < (int)lateralDistalReceptiveField.size(); dim++ ) {
			if ( lateralDistalReceptiveField[dim] < 0 ) {
				std::cout << "Layer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "lateralDistalReceptiveField[" << dim << "] = " << lateralDistalReceptiveField[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	if ( afferentPercentage < 0.0 || afferentPercentage > 1.0 ||
	     lateralProximalPercentage < 0.0 || lateralProximalPercentage > 1.0 ||
	     lateralDistalPercentage < 0.0 || lateralDistalPercentage > 1.0 ||
	     apicalPercentage < 0.0 || apicalPercentage > 1.0 ) {
		std::cout << "Layer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";
		std::cout << "afferentPercentage = " << afferentPercentage << "\n";
		std::cout << "lateralProximalPercentage = " << lateralProximalPercentage << "\n";
		std::cout << "lateralDistalPercentage = " << lateralDistalPercentage << "\n";
		std::cout << "apicalPercentage = " << apicalPercentage << "\n";
		exit( EXIT_FAILURE );
	}
} // end function validateComumnsInterconnectionsParameters


// validates the parameters used to generate the columns' populations
void	Layer::validatePopulationParameters(	const std::vector<int>&	populationsArrayDimensionality,
						const std::vector<int>&	afferentPopulationsArrayDimensionality,
						const std::vector<int>&	apicalPopulationsArrayDimensionality,
						const std::size_t temporalGatheringAfferentValue )
{
	if ( temporalGatheringAfferentValue == 0 ) {
		std::cout << "Layer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "temporalGatheringAfferentValue = "
			  << temporalGatheringAfferentValue << "\n";
		exit( EXIT_FAILURE );
	}

	if ( afferentPopulationsArrayDimensionality.size() == 0 ) {
		std::cout << "Layer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "afferentPopulationsArrayDimensionality.size() = "
			  << afferentPopulationsArrayDimensionality.size() << "\n";
		exit( EXIT_FAILURE );
	}
	else {
		if ( check_if_less_than_or_equal_to(afferentPopulationsArrayDimensionality, 0) ) {
			std::cout << "Layer object construction inconsistence: \n";
			std::cout << "In function validatePopulationParameters\n";
			std::cout << "afferentPopulationsArrayDimensionality = \n";
			for(const auto& s: afferentPopulationsArrayDimensionality)
				std::cout << s << ' ';

			std::cout << "\n";
			exit( EXIT_FAILURE );
		}
	}

	if ( apicalPopulationsArrayDimensionality.size() == 0 ) {
		std::cout << "Layer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "apicalPopulationsArrayDimensionality.size() = "
			  << apicalPopulationsArrayDimensionality.size() << "\n";
		exit( EXIT_FAILURE );
	}
	else {
		if ( check_if_less_than_or_equal_to(apicalPopulationsArrayDimensionality, 0) ) {
			std::cout << "Layer object construction inconsistence: \n";
			std::cout << "In function validatePopulationParameters\n";
			std::cout << "apicalPopulationsArrayDimensionality = \n";
			for(const auto& s: apicalPopulationsArrayDimensionality)
				std::cout << s << ' ';

			std::cout << "\n";
			exit( EXIT_FAILURE );
		}
	}

	if ( populationsArrayDimensionality.size() == 0 ) {
		std::cout << "Layer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "populationsArrayDimensionality.size() = "
			  << populationsArrayDimensionality.size() << "\n";
		exit( EXIT_FAILURE );
	}
	else {
		if ( check_if_less_than_or_equal_to(populationsArrayDimensionality, 0) ) {
			std::cout << "Layer object construction inconsistence: \n";
			std::cout << "In function validatePopulationParameters\n";
			std::cout << "populationsArrayDimensionality = \n";
			for(const auto& s: populationsArrayDimensionality)
				std::cout << s << ' ';

			std::cout << "\n";
			exit( EXIT_FAILURE );
		}
	}
} // end function validatePopulationParameters


// interconnects the layaer's populations
void	Layer::interconnectLayerColumns( const std::vector<int>& afferentArrayDimensionality, const std::vector<int>& apicalArrayDimensionality,
					const std::vector<int>& columnsArrayDimensionality,
					const std::vector<int>& afferentReceptiveField, const double afferentPercentage,
					const bool afferentWrapAround,
					const std::vector<int>& lateralProximalReceptiveField, const double lateralProximalPercentage,
					const bool lateralProximalWrapAround,
					const std::vector<int>& lateralDistalReceptiveField, const double lateralDistalPercentage,
					const bool lateralDistalWrapAround,
					const std::vector<int>& apicalReceptiveField, const double apicalPercentage,
					const bool apicalWrapAround )
{
	_afferentArrayDimensionality = afferentArrayDimensionality;
	_apicalArrayDimensionality = apicalArrayDimensionality;
	_columnsArrayDimensionality = columnsArrayDimensionality;

	_columnsDimensionality =  std::accumulate(_columnsArrayDimensionality.begin(),
						  _columnsArrayDimensionality.end(),
						  1, std::multiplies<int>());

	_afferentDimensionality =  std::accumulate(_afferentArrayDimensionality.begin(),
						  _afferentArrayDimensionality.end(),
						  1, std::multiplies<int>());

	_apicalDimensionality =  std::accumulate(_apicalArrayDimensionality.begin(),
						 _apicalArrayDimensionality.end(),
						 1, std::multiplies<int>());

	_afferentReceptiveField = afferentReceptiveField;
	_afferentPercentage = afferentPercentage;
	_afferentWrapAround = afferentWrapAround;

	if ( std::find(_afferentReceptiveField.begin(),
		       _afferentReceptiveField.end(), -1) == _afferentReceptiveField.end() ) {
		_afferentConnections.resize(_columnsDimensionality);
		for ( int column = 0; column < _columnsDimensionality; column++ )
			_afferentConnections[column] = Layer::getAfferentInputs(column);
	}

	_lateralProximalReceptiveField = lateralProximalReceptiveField;
	_lateralProximalPercentage = lateralProximalPercentage;
	_lateralProximalWrapAround = lateralProximalWrapAround;

	if ( std::find(_lateralProximalReceptiveField.begin(),
		       _lateralProximalReceptiveField.end(), -1) == _lateralProximalReceptiveField.end() ) {
		_lateralProximalConnections.resize(_columnsDimensionality);
		for ( int column = 0; column < _columnsDimensionality; column++ )
			_lateralProximalConnections[column] = Layer::getLateralProximalInputs(column);
	}

	_lateralDistalReceptiveField = lateralDistalReceptiveField;
	_lateralDistalPercentage = lateralDistalPercentage;
	_lateralDistalWrapAround = lateralDistalWrapAround;

	if ( std::find(_lateralDistalReceptiveField.begin(),
		       _lateralDistalReceptiveField.end(), -1) == _lateralDistalReceptiveField.end() ) {
		_lateralDistalConnections.resize(_columnsDimensionality);
		for ( int column = 0; column < _columnsDimensionality; column++ )
			_lateralDistalConnections[column] = Layer::getLateralDistalInputs(column);
	}

	_apicalReceptiveField = apicalReceptiveField;
	_apicalPercentage = apicalPercentage;
	_apicalWrapAround = apicalWrapAround;

	if ( std::find(_apicalReceptiveField.begin(),
		       _apicalReceptiveField.end(), -1) == _apicalReceptiveField.end() ) {
		_apicalConnections.resize(_columnsDimensionality);
		for ( int column = 0; column < _columnsDimensionality; column++ )
			_apicalConnections[column] = Layer::getApicalInputs(column);
	}
} // end function interconnectLayer


// generates a population for every column in the layer
void	Layer::generateColumns( const std::vector<int>&	populationsArrayDimensionality,
			      const std::vector<int>&	afferentPopulationsArrayDimensionality,
			      const std::vector<int>&	apicalPopulationsArrayDimensionality,
			      const std::size_t temporalGatheringAfferentValue )
{
	int			numberOfInputs;
	std::vector<int>	temporalUnits;
	for ( int column = 0; column < _columnsDimensionality; column++ ) {
		numberOfInputs = 0;

		if ( _afferentConnections.size() > 0 )
			for ( int afferentInput = 0; afferentInput < (int)_afferentConnections[column].size(); afferentInput++ )
				numberOfInputs += (int)afferentPopulationsArrayDimensionality.size();

		numberOfInputs *= temporalGatheringAfferentValue;

		if ( _lateralProximalConnections.size() > 0 )
			for ( int lateralProximalInput = 0; lateralProximalInput < (int)_lateralProximalConnections[column].size(); lateralProximalInput++ )
				numberOfInputs += (int)populationsArrayDimensionality.size();

		if ( _apicalConnections.size() > 0 )
			for ( int link = 0; link < (int)_apicalConnections[column].size(); link++ ) {
				auto	auxiliary = (int)std::accumulate(apicalPopulationsArrayDimensionality.begin(),
									apicalPopulationsArrayDimensionality.end(),
									1, std::multiplies<int>());
				temporalUnits.push_back(auxiliary);
			}

		if ( _lateralDistalConnections.size() > 0 )
			for ( int link = 0; link < (int)_lateralDistalConnections[column].size(); link++ ) {
				auto	auxiliary = (int)std::accumulate(populationsArrayDimensionality.begin(),
									populationsArrayDimensionality.end(),
									1, std::multiplies<int>());
				temporalUnits.push_back(auxiliary);
			}

		temporalUnits.shrink_to_fit();
		_layerColumns.push_back(TemporalPopulation(populationsArrayDimensionality, numberOfInputs, temporalUnits));
		temporalUnits.clear();
	}
	_layerColumns.shrink_to_fit();

	_populationsArrayDimensionality = populationsArrayDimensionality;
	_afferentPopulationsArrayDimensionality = afferentPopulationsArrayDimensionality;
	_apicalPopulationsArrayDimensionality = apicalPopulationsArrayDimensionality;
} // end function generateColumns


// initializes the proximal input limits from afferent and lateral connections
void	Layer::initializeProximalInputLimits( const std::size_t temporalGatheringAfferentValue )
{
	if ( _afferentConnections.size() != 0 ) {
		_proximalAfferentLimits.resize(temporalGatheringAfferentValue);
		auto	numberOfAfferentCoordinates = _afferentPopulationsArrayDimensionality.size();
		for ( std::size_t timeStep = 0; timeStep < temporalGatheringAfferentValue; timeStep++ ) {
			_proximalAfferentLimits[timeStep].resize(_afferentDimensionality);
			for ( int afferentInput = 0; afferentInput < _afferentDimensionality; afferentInput++ ) {
				_proximalAfferentLimits[timeStep][afferentInput].resize(numberOfAfferentCoordinates);
				for ( int coordinate = 0; coordinate < (int)numberOfAfferentCoordinates; coordinate++ ) {
					_proximalAfferentLimits[timeStep][afferentInput][coordinate][0] = 0.0;
					_proximalAfferentLimits[timeStep][afferentInput][coordinate][1] = 1.0;
				}
			}
		}
	}

	if ( _lateralProximalConnections.size() != 0 ) {
		_proximalLateralLimits.resize(_columnsDimensionality);
		auto	numberOfLateralCoordinates = _populationsArrayDimensionality.size();
		for ( int lateralInput = 0; lateralInput < _columnsDimensionality; lateralInput++ ) {
			_proximalLateralLimits[lateralInput].resize(numberOfLateralCoordinates);
			for ( int coordinate = 0; coordinate < (int)numberOfLateralCoordinates; coordinate++ ) {
				_proximalLateralLimits[lateralInput][coordinate][0] = 0.0;
				_proximalLateralLimits[lateralInput][coordinate][1] = 1.0;
			}
		}
	}
} // end function initializeProximalInputLimits


// initializes the internal temporal gathered inputs
void	Layer::initializeInternalTemporallyGatheredInputs()
{
	_internalTemporallyGatheredInputs.temporallyGatheredIndexes.resize(_temporalGatheringAfferentValue);
	_internalTemporallyGatheredInputs.temporallyGatheredInformation.resize(_temporalGatheringAfferentValue);
	for ( std::size_t timeStep = 0; timeStep < _temporalGatheringAfferentValue; timeStep++ ) {
		_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep].resize(_afferentDimensionality);
		auto	it = _internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].begin();
		_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].insert(it,_afferentDimensionality,false);
		for ( int column = 0; column < _afferentDimensionality; column++ )
			_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep][column].push_back(-1);
	}
	auto	it = _internalTemporallyGatheredInputs.synchronization.begin();
	_internalTemporallyGatheredInputs.synchronization.insert(it,_afferentDimensionality,true);
	auto	iter = _temporalPointer.begin();
	_temporalPointer.insert(iter,_afferentDimensionality,0);
} // end function initializeInternalTemporallyGatheredInputs


// loads input information
std::vector<layerResponse>	Layer::loadInputs( const std::string& fileName )
{
	std::string	str;
	std::string	STR;
	twodvector<double>	input;
	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + fileName + ".mat", ios::in | std::ifstream::binary);

	while ( std::getline(infile, str) ) {
		STR = "# name: inputs";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector_of_vectors(input, infile);
	}
	// close the opened file.
	infile.close();

	auto	timeSteps = input.size();
	auto	indexes = input[0].size();
	std::vector<layerResponse>	inputLayer(timeSteps);
	for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {
		inputLayer[timeStep].inputs.resize(indexes);
		inputLayer[timeStep].synchronization.resize(indexes);
		inputLayer[timeStep].information.resize(indexes);
		for ( size_t index = 0; index < indexes; index++ ) {
			inputLayer[timeStep].inputs[index] = input[timeStep][index];
			inputLayer[timeStep].synchronization[index] = true;
			if ( std::abs(input[timeStep][index]) >= INPUT_THRESHOLD )
				inputLayer[timeStep].information[index] = true;
			else
				inputLayer[timeStep].information[index] = false;
		}
	}

	return	inputLayer;
} // end function loadInput


// computes the layer response
layerResponse	Layer::computeResponse( const layerResponse& afferent, const layerResponse& lateral,
					const layerResponse& apical, const layerParameters& parameters )
{
	assert( parameters.proximalInformationThreshold >= 0.0 && parameters.proximalInformationThreshold <= 1.0 );
	assert( parameters.distalInformationThreshold >= 0.0 && parameters.distalInformationThreshold <= 1.0 );

	layerResponse	output;

	output.currentIndexes.resize(_columnsDimensionality);
	output.synchronization.resize(_columnsDimensionality);
	output.information.resize(_columnsDimensionality);

	for ( int column = 0; column < _columnsDimensionality; column++ ) {
		auto	proximalInputs = Layer::gatherProximalInputs(column, afferent, lateral);
		auto	distalInputs = Layer::gatherDistalInputs(column, apical, lateral);

		bool	proximalSynchronizations = !std::all_of(proximalInputs.synchronization.begin(),
							       proximalInputs.synchronization.end(),
							       [](bool v) { return !v; });

		bool	distalSynchronizations = !std::all_of(distalInputs.synchronization.begin(),
							       distalInputs.synchronization.end(),
							       [](bool v) { return !v; });

		auto	proximalInformation = truePercentage(proximalInputs.information);
		auto	distalInformation = truePercentage(proximalInputs.information);
		if ( proximalSynchronizations || distalSynchronizations ) { // If there is proximal OR distal synchronization
			if ( proximalInformation >= parameters.proximalInformationThreshold ||
			     distalInformation >= parameters.distalInformationThreshold ) { // If there is enough proximal or distal information

				std::vector<double>	coordinatesInputSoFar;
				twodvector<double>	proximalInputAlternatives;
				Layer::computeProximalAlternatives(proximalInputs.coordinates, 0, coordinatesInputSoFar, proximalInputAlternatives);
				assert(proximalInputAlternatives.size() != 0 && is_rectangular(proximalInputAlternatives));

				//std::vector<int>	indexesInputSoFar;
				//twodvector<int>		distalInputAlternatives;
				//Layer::computeDistalAlternatives(distalInputs.currentIndexes, 0, indexesInputSoFar, distalInputAlternatives);
				//assert(distalInputAlternatives.size() != 0 && is_rectangular(distalInputAlternatives));

				//std::cout << "\n proximalInputAlternatives.size() is " << proximalInputAlternatives.size() << "\n";

				// if there are more than 1000 alternatives, then
				// samples the proximalInputAlternatives so its size is 1000
				if ( proximalInputAlternatives.size() > 1000 ) {
					twodvector<double>	sample;
					sample_vector(proximalInputAlternatives, sample, 1000);
					proximalInputAlternatives = sample;
				}

				std::vector<responseInfo>	responseAlternatives;
				for ( int alternative = 0; alternative < (int)proximalInputAlternatives.size(); alternative++ )
					responseAlternatives.push_back(_layerColumns[column].getResponse(proximalInputAlternatives[alternative]));

				responseAlternatives.shrink_to_fit();

				std::vector<int>	bestResponsesIndexes =
								Layer::chooseBestResponsesIndexes(responseAlternatives,
										parameters.numberOfBestResponses,
										parameters.selectionCriteria);

				twodvector<int>	newResponseIndexesAlternatives;
				//for ( int bestAlternative = 0; bestAlternative < (int)bestResponsesIndexes.size(); bestAlternative++ )
				//	for ( int distalInputAlternative = 0; distalInputAlternative < (int)distalInputAlternatives.size(); distalInputAlternative++ )
				//		newResponseIndexesAlternatives.push_back(
				//			_layerColumns[column].Activate(responseAlternatives[bestResponsesIndexes[bestAlternative]],
				//			distalInputAlternatives[distalInputAlternative],
				//			parameters.activationRadius));

				for ( std::size_t bestAlternative = 0; bestAlternative < bestResponsesIndexes.size(); bestAlternative++ )
					newResponseIndexesAlternatives.push_back(
						_layerColumns[column].Activate(responseAlternatives[bestResponsesIndexes[bestAlternative]],
						distalInputs.currentIndexes,
						parameters.activationRadius));

				auto	bestIndexes = Layer::chooseBestIndexes(newResponseIndexesAlternatives);

				if ( parameters.enableLearning )	// If learning is enabled, then it calls learn member function
					Layer::learn(column, bestResponsesIndexes, responseAlternatives, proximalInputAlternatives, 
							bestIndexes, distalInputs.currentIndexes,
							proximalInformation, parameters.learning);

				if ( bestIndexes.size() == 1 ) {	// If there is just one best index
					if ( bestIndexes[0] == -1 ) {	// If this index offers no information
						output.currentIndexes[column].resize(1);
						output.currentIndexes[column][0] = -1;
						output.synchronization[column] = true;
						output.information[column] = false;
					}
					else {				// If this index offers information
						output.currentIndexes[column] = bestIndexes;
						output.synchronization[column] = true;
						output.information[column] = true;
					}
				}
				else { // If there is more than one best index, processes just the ones which offer information
					for(auto& s: bestIndexes)
						if ( s != -1 ) // Stack only indexes that offers information
							output.currentIndexes[column].push_back(s);

					output.synchronization[column] = true;
					output.information[column] = true;
				}
			}
			else { // If there is no proximal nor distal information
				output.currentIndexes[column].resize(1);
				output.currentIndexes[column][0] = -1;
				output.synchronization[column] = true;
				output.information[column] = false;
			}
		}
		else { // If there is no proximal nor distal synchronization
			output.currentIndexes[column] = lateral.currentIndexes[column];
			output.synchronization[column] = false;
			output.information[column] = lateral.information[column];
		}
	}

	if ( parameters.enableLearning )	// If learning is enabled, then it calls updateProximalInputLimits member function
		Layer::updateProximalInputLimits(afferent, lateral, parameters.learning);

	return	output;
} // end function computeResponse


// temporally gathers afferent inputs
layerResponse	Layer::temporalGatherer( const layerResponse& input )
{
	assert( input.inputs.size() == 0 &&
		input.temporallyGatheredIndexes.size() == 0 &&
		input.coordinates.size() == 0 &&
		input.temporallyGatheredInformation.size() == 0 &&
		_temporalGatheringAfferentValue > 1 );

	assert( input.currentIndexes.size() > 0 &&
		input.currentIndexes.size() == input.synchronization.size() &&
		input.currentIndexes.size() == input.information.size());

	layerResponse	output;

	bool	synchronization = !std::all_of(input.synchronization.begin(),
						input.synchronization.end(),
						[](bool v) { return !v; });

	// if the afferent input layer has some synchronization activity
	if ( synchronization ) {
		// iterates through the afferent input layer columns
		for ( int column = 0; column < _afferentDimensionality; column++ ) {
			// if there is synchronization activity in this afferent column
			if ( input.synchronization[column] ) {
				// if it is the first internal time step
				if ( _temporalPointer[column] == 0 ) {
					for ( std::size_t timeStep = 0; timeStep < _temporalGatheringAfferentValue; timeStep++ ) {
						_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep][column].clear();
						_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep][column].push_back(-1);
						_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep][column] = false;
					}
					_internalTemporallyGatheredInputs.synchronization[column] = false;
				}
				// if there is information comming from this afferent column
				if ( input.information[column] ) {
					_internalTemporallyGatheredInputs.temporallyGatheredIndexes[_temporalPointer[column]][column] =
												input.currentIndexes[column];
					_internalTemporallyGatheredInputs.temporallyGatheredInformation[_temporalPointer[column]][column] = true;
					_temporalPointer[column]++;
					_temporalPointer[column] = positiveMod(_temporalPointer[column],_temporalGatheringAfferentValue);
					if ( _temporalPointer[column] == 0 )
						_internalTemporallyGatheredInputs.synchronization[column] = true;
					else
						_internalTemporallyGatheredInputs.synchronization[column] = false;
				}
				// if there is no information comming from this afferent column
				else {
					assert(input.currentIndexes[column].size() == 1 && input.currentIndexes[column][0] == -1);
					_temporalPointer[column] = 0;
					_internalTemporallyGatheredInputs.synchronization[column] = true;
				}
			}
			// if there is no synchronization activity in this afferent column
			else {
				_internalTemporallyGatheredInputs.synchronization[column] = false;
			}
		}
		return	_internalTemporallyGatheredInputs;
	}
	// if there is no synchronization activity from the afferent input layer
	else {
		std::fill (_internalTemporallyGatheredInputs.synchronization.begin(),
				_internalTemporallyGatheredInputs.synchronization.end(),false);

		return	_internalTemporallyGatheredInputs;
	}
} // end function temporalGatherer


// modifies proximal and distal synaptic weights
// in certain population indexed by column agument
void	Layer::learn( const int column, const std::vector<int>& bestResponsesIndexes,
			const std::vector<responseInfo>& responseAlternatives,
			const twodvector<double>& proximalInputAlternatives,
			const std::vector<int>& bestIndexes, const twodvector<int>& linkingUnits,
			const double proximalInformation, const layerLearningParameters& parameters )
{
	assert( parameters.proximalInformationThreshold >= 0.0 && parameters.proximalInformationThreshold <= 1.0 );

	if ( parameters.enableProximalLearning && proximalInformation >= parameters.proximalInformationThreshold ) {
		_layerColumns[column].learningRule(parameters.proximalLearningRate, parameters.proximalNeighborhood,
						responseAlternatives[bestResponsesIndexes[0]].ranking[0],
						proximalInputAlternatives[bestResponsesIndexes[0]]);
	}

	if ( parameters.enableDistalLearning ) {
		// if the massive activation is greater then certain threshold,
		// it is considered as a prediction fault, then special distal
		// learning considerations are taken
		if ( bestIndexes.size() > PREDICTION_FAULT_THRESHOLD ) {
			_layerColumns[column].Update(bestIndexes, linkingUnits, true, parameters.distalSynapsesThreshold,
										parameters.distalLearningRate*BUSTING);
		}
		else {
			_layerColumns[column].Update(bestIndexes, linkingUnits, true, parameters.distalSynapsesThreshold,
										parameters.distalLearningRate);
		}
	}
} // end function learn


// gathers proximal inputs from afferent and lateral connections
layerResponse	Layer::gatherProximalInputs( const int column, const layerResponse& afferent, const layerResponse& lateral )
{
	layerResponse	proximalInputs;

	int	offset = 0;
	if ( afferent.inputs.size() != 0 ) {
		assert(afferent.temporallyGatheredIndexes.size() == 0 &&
			afferent.temporallyGatheredInformation.size() == 0 &&
			afferent.currentIndexes.size() == 0 &&
			afferent.coordinates.size() == 0 &&
			_temporalGatheringAfferentValue == 1);

		size_t	numberOfProximalConnections = 0;

		if ( _afferentConnections.size() != 0 )
			numberOfProximalConnections += _afferentConnections[column].size();

		if ( _lateralProximalConnections.size() != 0 )
			numberOfProximalConnections += _lateralProximalConnections[column].size();

		proximalInputs.coordinates.resize(numberOfProximalConnections);
		proximalInputs.synchronization.resize(numberOfProximalConnections);
		proximalInputs.information.resize(numberOfProximalConnections);

		for ( int afferentConnection = 0; afferentConnection < (int)_afferentConnections[column].size(); afferentConnection++ ) {
			auto	connection = _afferentConnections[column][afferentConnection];
			proximalInputs.coordinates[offset+afferentConnection].resize(1);
			if ( afferent.information[connection] ) {
				proximalInputs.coordinates[offset+afferentConnection][0].push_back(
				afferent.inputs[connection]);
				proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
				proximalInputs.information[offset+afferentConnection] = afferent.information[connection];
			}
			else {
				proximalInputs.coordinates[offset+afferentConnection][0] =
				Layer::getRandomInputCoordinates(_proximalAfferentLimits[0][connection]);
				proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
				proximalInputs.information[offset+afferentConnection] = afferent.information[connection];
			}
		}
		offset += (int)_afferentConnections[column].size();
	}
	else if ( afferent.currentIndexes.size() != 0 ) {
		assert(afferent.temporallyGatheredIndexes.size() == 0 &&
			afferent.temporallyGatheredInformation.size() == 0 &&
			afferent.coordinates.size() == 0 &&
			_temporalGatheringAfferentValue == 1);

		size_t	numberOfProximalConnections = 0;

		if ( _afferentConnections.size() != 0 )
			numberOfProximalConnections += _afferentConnections[column].size();

		if ( _lateralProximalConnections.size() != 0 )
			numberOfProximalConnections += _lateralProximalConnections[column].size();

		proximalInputs.coordinates.resize(numberOfProximalConnections);
		proximalInputs.synchronization.resize(numberOfProximalConnections);
		proximalInputs.information.resize(numberOfProximalConnections);

		if ( _afferentConnections.size() != 0 ) { // TODO: Add this control statement to the first if
				for ( int afferentConnection = 0; afferentConnection < (int)_afferentConnections[column].size(); afferentConnection++ ) {
					auto	connection = _afferentConnections[column][afferentConnection];
					auto	numberOfIndexes = afferent.currentIndexes[connection].size();

					// If there is just one index and it doesn't bring information
					if ( numberOfIndexes == 1 && afferent.currentIndexes[connection][0] == -1 ) {
						assert(afferent.information[connection] == false);
						proximalInputs.coordinates[offset+afferentConnection].push_back(
						Layer::getRandomInputCoordinates(_proximalAfferentLimits[0][connection]));
						proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
						proximalInputs.information[offset+afferentConnection] = afferent.information[connection];
					}
					// If there is more than one index, but all the alternatives are equal to -1
					else if ( std::all_of(afferent.currentIndexes[connection].begin(),
								afferent.currentIndexes[connection].end(), [](int i){return i == -1;}) ) {
						assert(numberOfIndexes > 1);
						assert(afferent.information[connection] == false);
						proximalInputs.coordinates[offset+afferentConnection].push_back(
						Layer::getRandomInputCoordinates(_proximalAfferentLimits[0][connection]));
						proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
						proximalInputs.information[offset+afferentConnection] = afferent.information[connection];
					}
					// If there is more than one index, and not all are equal to -1, then, iterates through them
					else {
						assert(afferent.information[connection] == true);
						for ( int alternative = 0; alternative < (int)numberOfIndexes; alternative++ ) {
							// Only adds the coordinates whose indexes bring information
							if ( afferent.currentIndexes[connection][alternative] != -1 )
								proximalInputs.coordinates[offset+afferentConnection].push_back(
								Layer::Normalize(unravelIndex(afferent.currentIndexes[connection][alternative],
												_afferentPopulationsArrayDimensionality),
												_afferentPopulationsArrayDimensionality));
						}
						proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
						proximalInputs.information[offset+afferentConnection] = afferent.information[connection];
					}
				}
				offset += (int)_afferentConnections[column].size();
		}
	}
	else {
		assert(afferent.temporallyGatheredIndexes.size() == _temporalGatheringAfferentValue && _temporalGatheringAfferentValue > 1);
		assert(afferent.temporallyGatheredIndexes.size() == afferent.temporallyGatheredInformation.size());
		assert(afferent.coordinates.size() == 0 &&
			afferent.synchronization.size() == 0 &&
			afferent.information.size() == 0);

		size_t	numberOfProximalConnections = 0;

		if ( _afferentConnections.size() != 0 )
			numberOfProximalConnections += afferent.temporallyGatheredIndexes.size() * _afferentConnections[column].size();

		if ( _lateralProximalConnections.size() != 0 )
			numberOfProximalConnections += _lateralProximalConnections[column].size();

		proximalInputs.coordinates.resize(numberOfProximalConnections);
		proximalInputs.synchronization.resize(numberOfProximalConnections);
		proximalInputs.information.resize(numberOfProximalConnections);

		if ( _afferentConnections.size() != 0 ) {
			for ( int timeStep = 0; timeStep < (int)afferent.temporallyGatheredIndexes.size(); timeStep++ ) {
				for ( int afferentConnection = 0; afferentConnection < (int)_afferentConnections[column].size(); afferentConnection++ ) {
					auto	connection = _afferentConnections[column][afferentConnection];
					auto	numberOfIndexes = afferent.temporallyGatheredIndexes[timeStep][connection].size();

					// If there is just one index and it doesn't bring information
					if ( numberOfIndexes == 1 && afferent.temporallyGatheredIndexes[timeStep][connection][0] == -1 ) {
						proximalInputs.coordinates[offset+afferentConnection].push_back(
						Layer::getRandomInputCoordinates(_proximalAfferentLimits[timeStep][connection]));
						proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
						proximalInputs.information[offset+afferentConnection] = afferent.temporallyGatheredInformation[timeStep][connection];
					}
					// If there is more than one index, but all the alternatives are equal to -1
					else if ( std::all_of(afferent.temporallyGatheredIndexes[timeStep][connection].begin(),
								afferent.temporallyGatheredIndexes[timeStep][connection].end(),
								[](int i){return i == -1;}) ) {
						proximalInputs.coordinates[offset+afferentConnection].push_back(
						Layer::getRandomInputCoordinates(_proximalAfferentLimits[timeStep][connection]));
						proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
						proximalInputs.information[offset+afferentConnection] = afferent.temporallyGatheredInformation[timeStep][connection];
					}
					// If there is more than one index, and not all are equal to -1, then, iterates through them
					else {
						for ( int alternative = 0; alternative < (int)numberOfIndexes; alternative++ ) {
							// Only adds the coordinates whose indexes bring information
							if ( afferent.temporallyGatheredIndexes[timeStep][connection][alternative] != -1 )
								proximalInputs.coordinates[offset+afferentConnection].push_back(
								Layer::Normalize(unravelIndex(afferent.temporallyGatheredIndexes[timeStep][connection][alternative],
												_afferentPopulationsArrayDimensionality),
												_afferentPopulationsArrayDimensionality));
						}
						proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
						proximalInputs.information[offset+afferentConnection] = afferent.temporallyGatheredInformation[timeStep][connection];
					}
				}
				offset += (int)_afferentConnections[column].size();
			}
		}
	}

	if ( _lateralProximalConnections.size() != 0 ) {
		for ( int lateralProximalConnection = 0; lateralProximalConnection < (int)_lateralProximalConnections[column].size(); lateralProximalConnection++ ) {
			auto	connection = _lateralProximalConnections[column][lateralProximalConnection];
			auto	numberOfIndexes = lateral.currentIndexes[connection].size();

			// If there is just one index and it doesn't bring information
			if ( numberOfIndexes == 1 && lateral.currentIndexes[connection][0] == -1 ) {
				proximalInputs.coordinates[offset+lateralProximalConnection].push_back(
				Layer::getRandomInputCoordinates(_proximalLateralLimits[connection]));
				proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			}
			// If there is more than one index, but all the alternatives are equal to -1
			else if ( std::all_of(lateral.currentIndexes[connection].begin(),
						lateral.currentIndexes[connection].end(), [](int i){return i == -1;}) ) {
				proximalInputs.coordinates[offset+lateralProximalConnection].push_back(
				Layer::getRandomInputCoordinates(_proximalLateralLimits[connection]));
				proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			}
			// If there is more than one index, and not all are equal to -1, then, iterates through them
			else {
				proximalInputs.coordinates[offset+lateralProximalConnection].resize(numberOfIndexes);
				for ( int alternative = 0; alternative < (int)numberOfIndexes; alternative++ ) {
					proximalInputs.coordinates[offset+lateralProximalConnection][alternative] =
					Layer::Normalize(unravelIndex(lateral.currentIndexes[connection][alternative], _populationsArrayDimensionality),
									_populationsArrayDimensionality);
				}
				proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			}
		}
	}

	return	proximalInputs;
} // end function gatherProximalInputs


// gathers distal inputs from apical and lateral connections
layerResponse	Layer::gatherDistalInputs( const int column, const layerResponse& apical, const layerResponse& lateral )
{
	assert(apical.temporallyGatheredIndexes.size() == 0 &&
		lateral.temporallyGatheredIndexes.size() == 0);

	layerResponse	distalInputs;

	size_t	numberOfDistalConnections = 0;

	if ( _apicalConnections.size() != 0 )
		numberOfDistalConnections += _apicalConnections[column].size();

	if ( _lateralDistalConnections.size() != 0 )
		numberOfDistalConnections += _lateralDistalConnections[column].size();

	distalInputs.currentIndexes.resize(numberOfDistalConnections);
	distalInputs.synchronization.resize(numberOfDistalConnections);
	distalInputs.information.resize(numberOfDistalConnections);

	int	offset = 0;
	if ( _apicalConnections.size() != 0 ) {
		for ( int apicalConnection = 0; apicalConnection < (int)_apicalConnections[column].size(); apicalConnection++ ) {
			auto	connection = _apicalConnections[column][apicalConnection];
			distalInputs.currentIndexes[offset+apicalConnection] =
			apical.currentIndexes[connection];
			distalInputs.synchronization[offset+apicalConnection] = apical.synchronization[connection];
			distalInputs.information[offset+apicalConnection] = apical.information[connection];
		}
		offset += (int)_apicalConnections[column].size();
	}

	if ( _lateralDistalConnections.size() != 0 ) {
		for ( int lateralDistalConnection = 0; lateralDistalConnection < (int)_lateralDistalConnections[column].size(); lateralDistalConnection++ ) {
			auto	connection = _lateralDistalConnections[column][lateralDistalConnection];
			distalInputs.currentIndexes[offset+lateralDistalConnection] = lateral.currentIndexes[connection];
			distalInputs.synchronization[offset+lateralDistalConnection] = lateral.synchronization[connection];
			distalInputs.information[offset+lateralDistalConnection] = lateral.information[connection];
		}
	}

	return	distalInputs;
} // end function gatherDistalInputs


// computes all proximal inputs alternatives
void	Layer::computeProximalAlternatives( const threedvector<double>& coordinates,
					size_t vecIndex, std::vector<double> inputSoFar,
					twodvector<double>& inputAlternatives )
{
	if ( vecIndex >= coordinates.size() ) {
		inputSoFar.shrink_to_fit();
		inputAlternatives.push_back(inputSoFar);
		return;
	}
	for ( size_t alternative = 0; alternative < coordinates[vecIndex].size(); alternative++ ) {
		auto	auxiliary = inputSoFar;
		auxiliary.insert(auxiliary.end(), coordinates[vecIndex][alternative].begin(),
						  coordinates[vecIndex][alternative].end());

		Layer::computeProximalAlternatives(coordinates, vecIndex+1, auxiliary, inputAlternatives);
	}
} // end function computeProximalAlternatives


// computes all distal inputs alternatives
void	Layer::computeDistalAlternatives( const twodvector<int>& indexes,
					size_t vecIndex, std::vector<int> inputSoFar,
					twodvector<int>& inputAlternatives )
{
	if ( vecIndex >= indexes.size() ) {
		inputSoFar.shrink_to_fit();
		inputAlternatives.push_back(inputSoFar);
		return;
	}
	for ( size_t alternative = 0; alternative < indexes[vecIndex].size(); alternative++ ) {
		auto	auxiliary = inputSoFar;
		auxiliary.push_back(indexes[vecIndex][alternative]);

		Layer::computeDistalAlternatives(indexes, vecIndex+1, auxiliary, inputAlternatives);
	}
} // end function computeDistalAlternatives


// gets random input coordinates between statistical limits
std::vector<double>	Layer::getRandomInputCoordinates( const std::vector<std::array<double, 2>>& proximalLimits )
{
	std::vector<double>	output;

	for ( int coordinate = 0; coordinate < (int)proximalLimits.size(); coordinate++ )
		output.push_back(randomFromDoubleInterval(proximalLimits[coordinate][0], proximalLimits[coordinate][1]));

	output.shrink_to_fit();

	return	output;
} // end function getRandomInputCoordinates


// updates the proximal input limits from afferent and lateral connections
void	Layer::updateProximalInputLimits( const layerResponse& afferent, const layerResponse& lateral,
					  const layerLearningParameters& parameters )
{
	assert( parameters.proximalInformationThreshold >= 0.0 && parameters.proximalInformationThreshold <= 1.0 );

	if ( parameters.limitsLearningRate < 0.0 && parameters.limitsLearningRate > 1.0 ) {
		std::cout << "Layer object inconsistence: \n";
		std::cout << "In function updateProximalInputLimits\n";
		std::cout << "limitsLearningRate must be: 0 <= limitsLearningRate <= 1.\n";
		std::cout << "Yet, limitsLearningRate = " << parameters.limitsLearningRate << "\n";
		exit( EXIT_FAILURE );
	}

	if ( _afferentConnections.size() != 0 ) {
		double	afferentInformation = 0.0;
		if ( _temporalGatheringAfferentValue == 1 ) {
			assert(afferent.temporallyGatheredInformation.size() == 0);
			afferentInformation = truePercentage(afferent.information);
		}
		else if ( _temporalGatheringAfferentValue > 1 ) {
			assert(afferent.information.size() == 0);
			for(auto& info : afferent.temporallyGatheredInformation)
				afferentInformation += truePercentage(info);

			afferentInformation /=  _temporalGatheringAfferentValue;
		}

		if ( afferentInformation >= parameters.proximalInformationThreshold && _afferentUpdateStep > UPDATE_PERIOD ) {
			if ( afferent.inputs.size() != 0 ) { // ask if the information is entering through the first input
				assert(afferent.temporallyGatheredIndexes.size() == 0); // in such a case this condition has to be satisfied
				assert(afferent.currentIndexes.size() == 0); // and this one too
				assert(afferent.coordinates.size() == 0); // and this one too
				assert(afferent.temporallyGatheredInformation.size() == 0); // and this one too
				for ( int afferentInput = 0; afferentInput < _afferentDimensionality; afferentInput++ ) {
					if ( afferent.information[afferentInput] ) { // updates just in the case of true information from the first input
						auto	coordinates = afferent.inputs[afferentInput];

						if ( coordinates >= _proximalAfferentLimits[0][afferentInput][0][0] &&
						     coordinates <= _proximalAfferentLimits[0][afferentInput][0][1] ) { // is it between the limits?
							_proximalAfferentLimits[0][afferentInput][0][0] += parameters.limitsLearningRate*
							(coordinates - _proximalAfferentLimits[0][afferentInput][0][0]);
							_proximalAfferentLimits[0][afferentInput][0][1] -= parameters.limitsLearningRate*
							(_proximalAfferentLimits[0][afferentInput][0][1] - coordinates);
						}
						else if ( coordinates < _proximalAfferentLimits[0][afferentInput][0][0] ) { // is it below the lower limit?
							_proximalAfferentLimits[0][afferentInput][0][0] -= parameters.limitsLearningRate*
							(_proximalAfferentLimits[0][afferentInput][0][0] - coordinates);
						}
						else { // then it is above the upper limit
							_proximalAfferentLimits[0][afferentInput][0][1] += parameters.limitsLearningRate*
							(coordinates - _proximalAfferentLimits[0][afferentInput][0][1]);
						}
						assert(_proximalAfferentLimits[0][afferentInput][0][0] <
						       _proximalAfferentLimits[0][afferentInput][0][1]);
					}
				}
			}
			else if ( afferent.currentIndexes.size() != 0 ) { // ask if the information is entering through the current indexes
				assert(afferent.temporallyGatheredIndexes.size() == 0); // in such a case this condition has to be satisfied
				assert(afferent.coordinates.size() == 0); // and this one too
				assert(afferent.temporallyGatheredInformation.size() == 0); // and this one too
				for ( int afferentInput = 0; afferentInput < _afferentDimensionality; afferentInput++ ) {
					if ( afferent.currentIndexes[afferentInput].size() == 1 &&
					     afferent.currentIndexes[afferentInput][0] != -1 ) {	// updates just if there is one alternative, and
													// if such alternative represents true information
						auto	coordinates = Layer::Normalize(unravelIndex(afferent.currentIndexes[afferentInput][0],
													_afferentPopulationsArrayDimensionality),
													_afferentPopulationsArrayDimensionality);

						for ( int coordinate = 0; coordinate < (int)coordinates.size(); coordinate++ ) {
							if ( coordinates[coordinate] >= _proximalAfferentLimits[0][afferentInput][coordinate][0] &&
							     coordinates[coordinate] <= _proximalAfferentLimits[0][afferentInput][coordinate][1] ) { // is it between the limits?
								_proximalAfferentLimits[0][afferentInput][coordinate][0] += parameters.limitsLearningRate*
								(coordinates[coordinate] - _proximalAfferentLimits[0][afferentInput][coordinate][0]);
								_proximalAfferentLimits[0][afferentInput][coordinate][1] -= parameters.limitsLearningRate*
								(_proximalAfferentLimits[0][afferentInput][coordinate][1] - coordinates[coordinate]);
							}
							else if ( coordinates[coordinate] < _proximalAfferentLimits[0][afferentInput][coordinate][0] ) { // is it below the lower limit?
								_proximalAfferentLimits[0][afferentInput][coordinate][0] -= parameters.limitsLearningRate*
								(_proximalAfferentLimits[0][afferentInput][coordinate][0] - coordinates[coordinate]);
							}
							else { // then it is above the upper limit
								_proximalAfferentLimits[0][afferentInput][coordinate][1] += parameters.limitsLearningRate*
								(coordinates[coordinate] - _proximalAfferentLimits[0][afferentInput][coordinate][1]);
							}
							assert(_proximalAfferentLimits[0][afferentInput][coordinate][0] <
							       _proximalAfferentLimits[0][afferentInput][coordinate][1]);
						}
					}
				}
			}
			else { // then information is entering through the temporally gathered indexes
				assert(afferent.coordinates.size() == 0); // in such a case this condition has to be satisfied
				assert(afferent.information.size() == 0); // and this one too
				for ( int timeStep = 0; timeStep < (int)afferent.temporallyGatheredIndexes.size(); timeStep++ ) {
					for ( int afferentInput = 0; afferentInput < _afferentDimensionality; afferentInput++ ) {
						if ( afferent.temporallyGatheredIndexes[timeStep][afferentInput].size() == 1 &&
						     afferent.temporallyGatheredIndexes[timeStep][afferentInput][0] != -1 ) {	// updates just if there is one alternative, and
																// if such alternative represents true information
							auto	coordinates = Layer::Normalize(unravelIndex(afferent.temporallyGatheredIndexes[timeStep][afferentInput][0],
														_afferentPopulationsArrayDimensionality),
														_afferentPopulationsArrayDimensionality);

							for ( int coordinate = 0; coordinate < (int)coordinates.size(); coordinate++ ) {
								if ( coordinates[coordinate] >= _proximalAfferentLimits[timeStep][afferentInput][coordinate][0] &&
								     coordinates[coordinate] <= _proximalAfferentLimits[timeStep][afferentInput][coordinate][1] ) { // is it between the limits?
									_proximalAfferentLimits[timeStep][afferentInput][coordinate][0] += parameters.limitsLearningRate*
									(coordinates[coordinate] - _proximalAfferentLimits[timeStep][afferentInput][coordinate][0]);
									_proximalAfferentLimits[timeStep][afferentInput][coordinate][1] -= parameters.limitsLearningRate*
									(_proximalAfferentLimits[timeStep][afferentInput][coordinate][1] - coordinates[coordinate]);
								}
								else if ( coordinates[coordinate] < _proximalAfferentLimits[timeStep][afferentInput][coordinate][0] ) { // is it below the lower limit?
									_proximalAfferentLimits[timeStep][afferentInput][coordinate][0] -= parameters.limitsLearningRate*
									(_proximalAfferentLimits[timeStep][afferentInput][coordinate][0] - coordinates[coordinate]);
								}
								else { // then it is above the upper limit
									_proximalAfferentLimits[timeStep][afferentInput][coordinate][1] += parameters.limitsLearningRate*
									(coordinates[coordinate] - _proximalAfferentLimits[timeStep][afferentInput][coordinate][1]);
								}
								assert(_proximalAfferentLimits[timeStep][afferentInput][coordinate][0] <
								       _proximalAfferentLimits[timeStep][afferentInput][coordinate][1]);
							}
						}
					}
				}
			}
			_afferentUpdateStep = 0;
		}
		else {
			_afferentUpdateStep++;
		}
	}

	if ( _lateralProximalConnections.size() != 0 ) {
		auto	lateralInformation = truePercentage(lateral.information);

		if ( lateralInformation >= parameters.proximalInformationThreshold && _lateralUpdateStep > UPDATE_PERIOD ) {
			for ( int lateralInput = 0; lateralInput < _columnsDimensionality; lateralInput++ ) {
				if ( lateral.currentIndexes[lateralInput].size() == 1 &&
				     lateral.currentIndexes[lateralInput][0] != -1 ) {			// updates just if there is one alternative, and
													// if such alternative represents true information
					auto	coordinates = Layer::Normalize(unravelIndex(lateral.currentIndexes[lateralInput][0],
												_populationsArrayDimensionality),
												_populationsArrayDimensionality);

					for ( int coordinate = 0; coordinate < (int)coordinates.size(); coordinate++ ) {
						if ( coordinates[coordinate] >= _proximalLateralLimits[lateralInput][coordinate][0] &&
						     coordinates[coordinate] <= _proximalLateralLimits[lateralInput][coordinate][1] ) { // is it between the limits?
							_proximalLateralLimits[lateralInput][coordinate][0] += parameters.limitsLearningRate*
							(coordinates[coordinate] - _proximalLateralLimits[lateralInput][coordinate][0]);
							_proximalLateralLimits[lateralInput][coordinate][1] -= parameters.limitsLearningRate*
							(_proximalLateralLimits[lateralInput][coordinate][1] - coordinates[coordinate]);
						}
						else if ( coordinates[coordinate] < _proximalLateralLimits[lateralInput][coordinate][0] ) { // is it below the lower limit?
							_proximalLateralLimits[lateralInput][coordinate][0] -= parameters.limitsLearningRate*
							(_proximalLateralLimits[lateralInput][coordinate][0] - coordinates[coordinate]);
						}
						else { // then it is above the upper limit
							_proximalLateralLimits[lateralInput][coordinate][1] += parameters.limitsLearningRate*
							(coordinates[coordinate] - _proximalLateralLimits[lateralInput][coordinate][1]);
						}
						assert(_proximalLateralLimits[lateralInput][coordinate][0] <
						       _proximalLateralLimits[lateralInput][coordinate][1]);
					}
				}
			}
			_lateralUpdateStep = 0;
		}
		else {
			_lateralUpdateStep++;
		}
	}
} // end function updateProximalInputLimits


// choose a number of best responses alternatives
std::vector<int>	Layer::chooseBestResponsesIndexes( const std::vector<responseInfo>& responseAlternatives,
							const int numberOfBestResponses,
							const std::string& criterion )
{
	std::string	str;
	std::vector<int>	output;


	str = "BMUsRanking";
	if ( str.compare(criterion) == 0 ) {
		std::vector<double>		lowerDistances;

		for(const auto& s: responseAlternatives)
			lowerDistances.push_back(s.distances[0]);

		auto	responsesRanking = sort_indexes(lowerDistances);

		if ( numberOfBestResponses >= (int)responseAlternatives.size() ) {
			for ( int index = 0; index < (int)responseAlternatives.size(); index++ )
				output.push_back(responsesRanking[index]);
		}
		else {
			for ( int index = 0; index < numberOfBestResponses; index++ )
				output.push_back(responsesRanking[index]);
		}

		output.shrink_to_fit();
		return	output;
	}

	str = "EnergyRanking";
	if ( str.compare(criterion) == 0 ) {
		std::vector<double>		energies;

		for(const auto& s: responseAlternatives)
			energies.push_back(std::accumulate(s.distances.begin(), s.distances.end(), 0));

		auto	responsesRanking = sort_indexes(energies);

		if ( numberOfBestResponses >= (int)responseAlternatives.size() ) {
			for ( int index = 0; index < (int)responseAlternatives.size(); index++ )
				output.push_back(responsesRanking[index]);
		}
		else {
			for ( int index = 0; index < numberOfBestResponses; index++ )
				output.push_back(responsesRanking[index]);
		}

		output.shrink_to_fit();
		return	output;
	}

	if ( true ) {
		std::vector<double>		lowerDistances;

		for(const auto& s: responseAlternatives)
			lowerDistances.push_back(s.distances[0]);

		auto	responsesRanking = sort_indexes(lowerDistances);

		if ( numberOfBestResponses >= (int)responseAlternatives.size() ) {
			for ( int index = 0; index < (int)responseAlternatives.size(); index++ )
				output.push_back(responsesRanking[index]);
		}
		else {
			for ( int index = 0; index < numberOfBestResponses; index++ )
				output.push_back(responsesRanking[index]);
		}

		output.shrink_to_fit();
		return	output;
	}

} // end function chooseBestResponsesIndexes


// chooses the best indexes
std::vector<int>	Layer::chooseBestIndexes( const twodvector<int>& setsOfIndexesAlternatives )
{
	std::vector<int>	bestIndexes;
	auto	uniqueIndexes = get_unique_elements(setsOfIndexesAlternatives);
	auto	histogram = get_histogram(setsOfIndexesAlternatives);

	size_t	score = 0;
	for(const auto& s: uniqueIndexes) {
		if ( score == histogram[s] ) {
			bestIndexes.push_back(s);
		}
		else if ( score < histogram[s] ) {
			bestIndexes.clear();
			bestIndexes.push_back(s);
			score = histogram[s];
		}
	}

	return	bestIndexes;
} // end function chooseBestIndexes


// gets the afferent input components that correspond to a column.
std::vector<int>	Layer::getAfferentInputs( const int index )
{
	int	afferentRecptiveFieldCenter, numAfferentInputs;
	std::vector<int> columnInputs;

	afferentRecptiveFieldCenter = Layer::getReceptiveFieldCenter(index, _afferentArrayDimensionality);
	columnInputs = Layer::getNeighborhood(afferentRecptiveFieldCenter, _afferentWrapAround,
									   _afferentReceptiveField,
									   _afferentArrayDimensionality);

	//Select a subset of the receptive field to serve as the
	//the established connections
	numAfferentInputs = (int)(columnInputs.size()*_afferentPercentage + 0.5);

	std::vector<int>	afferentInputs = sampleVector(columnInputs, numAfferentInputs);
	std::sort(afferentInputs.begin(),afferentInputs.end());
	return	afferentInputs;
} // end function getAfferentInputs


// gets the lateral proximal input components that correspond to a column.
std::vector<int>	Layer::getLateralProximalInputs( const int index )
{
	int	numLateralProximalInputs;
	std::vector<int> columnInputs;

	columnInputs = Layer::getNeighborhood(index, _lateralProximalWrapAround,
						     _lateralProximalReceptiveField,
						     _columnsArrayDimensionality);

	//Select a subset of the receptive field to serve as the
	//the established connections
	numLateralProximalInputs = (int)(columnInputs.size()*_lateralProximalPercentage + 0.5);

	std::vector<int>	lateralProximalInputs = sampleVector(columnInputs, numLateralProximalInputs);
	std::sort(lateralProximalInputs.begin(),lateralProximalInputs.end());
	return	lateralProximalInputs;
} // end function getLateralProximalInputs


// gets the lateral distal input components that correspond to a column.
std::vector<int>	Layer::getLateralDistalInputs( const int index )
{
	int	numLateralDistalInputs;
	std::vector<int> columnInputs;

	columnInputs = Layer::getNeighborhood(index, _lateralDistalWrapAround,
						     _lateralDistalReceptiveField,
						     _columnsArrayDimensionality);

	//Select a subset of the receptive field to serve as the
	//the established connections
	numLateralDistalInputs = (int)(columnInputs.size()*_lateralDistalPercentage + 0.5);

	std::vector<int>	lateralDistalInputs = sampleVector(columnInputs, numLateralDistalInputs);
	std::sort(lateralDistalInputs.begin(),lateralDistalInputs.end());
	return	lateralDistalInputs;
} // end function getLateralDistalInputs


// gets the lateral distal input components that correspond to a column.
std::vector<int>	Layer::getApicalInputs( const int index )
{
	int	apicalRecptiveFieldCenter, numApicalInputs;
	std::vector<int> columnInputs;

	apicalRecptiveFieldCenter = Layer::getReceptiveFieldCenter(index, _apicalArrayDimensionality);
	columnInputs = Layer::getNeighborhood(apicalRecptiveFieldCenter, _apicalWrapAround,
									 _apicalReceptiveField,
									 _apicalArrayDimensionality);

	//Select a subset of the receptive field to serve as the
	//the established connections
	numApicalInputs = (int)(columnInputs.size()*_apicalPercentage + 0.5);

	std::vector<int>	apicalInputs = sampleVector(columnInputs, numApicalInputs);
	std::sort(apicalInputs.begin(),apicalInputs.end());
	return	apicalInputs;
} // end function getApicalInputs


// Gets neighborhood of inputs.
std::vector<int>	Layer::getNeighborhood( const int centerInput, const bool wrapAround,
						const std::vector<int>& receptiveField,
						const std::vector<int>& arrayDimensionality )
{
	if ( wrapAround )
	{
		return wrappingNeighborhood(centerInput, receptiveField, arrayDimensionality);
	}
	else
	{
		return neighborhood(centerInput, receptiveField, arrayDimensionality);
	}
} // end function getNeighborhood


// gets the receptive field center corresponding to a column.
int	Layer::getReceptiveFieldCenter( const int index, const std::vector<int>& arrayDimensionality )
{
	std::vector<int>	columnCoords;
	std::vector<double>	ratios, auxiliary;

	ratios.resize(_columnsArrayDimensionality.size());
	auxiliary.resize(_columnsArrayDimensionality.size());

	columnCoords = unravelIndex(index, _columnsArrayDimensionality);

	std::transform(columnCoords.begin(), columnCoords.end(),
		       _columnsArrayDimensionality.begin(),
		       ratios.begin(), std::divides<double>());

	std::transform(arrayDimensionality.begin(), arrayDimensionality.end(),
		       ratios.begin(),
		       ratios.begin(), std::multiplies<double>());

	std::transform(arrayDimensionality.begin(), arrayDimensionality.end(),
		       _columnsArrayDimensionality.begin(),
		       auxiliary.begin(), std::divides<double>());

	std::transform(auxiliary.begin(), auxiliary.end(), auxiliary.begin(),
		       std::bind1st(std::multiplies<double>(),0.5));

	ratios += auxiliary;

	std::vector<int>	inputCoords(ratios.begin(), ratios.end());

	return	ravelIndex(inputCoords, arrayDimensionality);
} // end function getReceptiveFieldCenter


// function that saves the Layer's status in a file
void	Layer::saveLayerStatus( const std::string& fileName )
{
	// open a file in write mode.
	ofstream outfile;
	outfile.open("../../Octave/" + fileName + ".mat", ios::out | ios::trunc);

	// file preamble.
	outfile << "# This is a file created by saveLayerStatus member function in Layer class from," << endl;
	outfile << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
	outfile << "# Author: Dematties Dario Jesus." << endl;

	outfile << "\n\n" << endl;

	// saves afferentArrayDimensionality
	save_vector_as_matrix("afferentArrayDimensionality", _afferentArrayDimensionality, outfile);

	// saves afferentDimensionality
	save_as_scalar("afferentDimensionality", _afferentDimensionality, outfile);

	// saves afferentReceptiveField
	save_vector_as_matrix("afferentReceptiveField", _afferentReceptiveField, outfile);

	// saves afferentPercentage
	save_as_scalar("afferentPercentage", _afferentPercentage, outfile);

	// saves afferentWrapAround
	save_as_bool("afferentWrapAround", _afferentWrapAround, outfile);



	// saves apicalArrayDimensionality
	save_vector_as_matrix("apicalArrayDimensionality", _apicalArrayDimensionality, outfile);

	// saves apicalDimensionality
	save_as_scalar("apicalDimensionality", _apicalDimensionality, outfile);

	// saves apicalReceptiveField
	save_vector_as_matrix("apicalReceptiveField", _apicalReceptiveField, outfile);

	// saves apicalPercentage
	save_as_scalar("apicalPercentage", _apicalPercentage, outfile);

	// saves apicalWrapAround
	save_as_bool("apicalWrapAround", _apicalWrapAround, outfile);



	// saves columnsArrayDimensionality
	save_vector_as_matrix("columnsArrayDimensionality", _columnsArrayDimensionality, outfile);

	// saves columnsDimensionality
	save_as_scalar("columnsDimensionality", _columnsDimensionality, outfile);



	// saves lateralProximalReceptiveField
	save_vector_as_matrix("lateralProximalReceptiveField", _lateralProximalReceptiveField, outfile);

	// saves lateralProximalPercentage
	save_as_scalar("lateralProximalPercentage", _lateralProximalPercentage, outfile);

	// saves lateralProximalWrapAround
	save_as_bool("lateralProximalWrapAround", _lateralProximalWrapAround, outfile);



	// saves lateralDistalReceptiveField
	save_vector_as_matrix("lateralDistalReceptiveField", _lateralDistalReceptiveField, outfile);

	// saves lateralDistalPercentage
	save_as_scalar("lateralDistalPercentage", _lateralDistalPercentage, outfile);

	// saves lateralDistalWrapAround
	save_as_bool("lateralDistalWrapAround", _lateralDistalWrapAround, outfile);

	if ( _afferentConnections.size() > 0 ) {
		// saves afferentConnections
		twodvector<bool>	afferentConnectionsBitMap;
		afferentConnectionsBitMap.resize(_columnsDimensionality);
		for ( int column = 0; column < (int)_columnsDimensionality; column++ ) {
			afferentConnectionsBitMap[column].resize(_afferentDimensionality);
			for ( int index = 0; index < (int)_afferentConnections[column].size(); index++ )
				afferentConnectionsBitMap[column][_afferentConnections[column][index]] = true;
		}

		SparseMatrixElements<bool>	sparseAfferentConnectionsBitMap;
		sparseAfferentConnectionsBitMap = to_sparse(afferentConnectionsBitMap);
		save_sparse_matrix_elements_as_sparse_matrix("afferentConnections", sparseAfferentConnectionsBitMap, outfile);
	}

	if ( _lateralProximalConnections.size() > 0 ) {
		// saves lateralProximalConnections
		twodvector<bool>	lateralProximalConnectionsBitMap;
		lateralProximalConnectionsBitMap.resize(_columnsDimensionality);
		for ( int column = 0; column < (int)_columnsDimensionality; column++ ) {
			lateralProximalConnectionsBitMap[column].resize(_columnsDimensionality);
			for ( int index = 0; index < (int)_lateralProximalConnections[column].size(); index++ )
				lateralProximalConnectionsBitMap[column][_lateralProximalConnections[column][index]] = true;
		}	

		SparseMatrixElements<bool>	sparseLateralProximalConnectionsBitMap;
		sparseLateralProximalConnectionsBitMap = to_sparse(lateralProximalConnectionsBitMap);
		save_sparse_matrix_elements_as_sparse_matrix("lateralProximalConnections", sparseLateralProximalConnectionsBitMap, outfile);
	}

	if ( _lateralDistalConnections.size() > 0 ) {
		// saves lateralDistalConnections
		twodvector<bool>	lateralDistalConnectionsBitMap;
		lateralDistalConnectionsBitMap.resize(_columnsDimensionality);
		for ( int column = 0; column < (int)_columnsDimensionality; column++ ) {
			lateralDistalConnectionsBitMap[column].resize(_columnsDimensionality);
			for ( int index = 0; index < (int)_lateralDistalConnections[column].size(); index++ )
				lateralDistalConnectionsBitMap[column][_lateralDistalConnections[column][index]] = true;
		}	

		SparseMatrixElements<bool>	sparseLateralDistalConnectionsBitMap;
		sparseLateralDistalConnectionsBitMap = to_sparse(lateralDistalConnectionsBitMap);
		save_sparse_matrix_elements_as_sparse_matrix("lateralDistalConnections", sparseLateralDistalConnectionsBitMap, outfile);
	}

	if ( _apicalConnections.size() > 0 ) {
		// saves apicalConnections
		twodvector<bool>	apicalConnectionsBitMap;
		apicalConnectionsBitMap.resize(_columnsDimensionality);
		for ( int column = 0; column < (int)_columnsDimensionality; column++ ) {
			apicalConnectionsBitMap[column].resize(_apicalDimensionality);
			for ( int index = 0; index < (int)_apicalConnections[column].size(); index++ )
				apicalConnectionsBitMap[column][_apicalConnections[column][index]] = true;
		}	

		SparseMatrixElements<bool>	sparseApicalConnectionsBitMap;
		sparseApicalConnectionsBitMap = to_sparse(apicalConnectionsBitMap);
		save_sparse_matrix_elements_as_sparse_matrix("apicalConnections", sparseApicalConnectionsBitMap, outfile);
	}



	// saves populationsArrayDimensionality
	save_vector_as_matrix("populationsArrayDimensionality", _populationsArrayDimensionality, outfile);

	// saves afferentPopulationsArrayDimensionality
	save_vector_as_matrix("afferentPopulationsArrayDimensionality", _afferentPopulationsArrayDimensionality, outfile);

	// saves apicalPopulationsArrayDimensionality
	save_vector_as_matrix("apicalPopulationsArrayDimensionality", _apicalPopulationsArrayDimensionality, outfile);

	if ( _afferentConnections.size() != 0 ) {
		threedvector<double>	lowerAfferentLimits, upperAfferentLimits;
		auto	timeSteps = _proximalAfferentLimits.size();
		lowerAfferentLimits.resize(timeSteps);
		upperAfferentLimits.resize(timeSteps);
		for ( int timeStep = 0; timeStep < (int)timeSteps; timeStep++ ) {
			auto	numberOfLimits = _proximalAfferentLimits[timeStep].size();
			lowerAfferentLimits[timeStep].resize(numberOfLimits);
			upperAfferentLimits[timeStep].resize(numberOfLimits);
			for ( int limit = 0; limit < (int)numberOfLimits; limit++ ) {
				auto	numberOfCoordinates = _proximalAfferentLimits[timeStep][limit].size();
				lowerAfferentLimits[timeStep][limit].resize(numberOfCoordinates);
				upperAfferentLimits[timeStep][limit].resize(numberOfCoordinates);
				for ( int coordinate = 0; coordinate < (int)numberOfCoordinates; coordinate++ ) {
					lowerAfferentLimits[timeStep][limit][coordinate] =
					_proximalAfferentLimits[timeStep][limit][coordinate][0];
					upperAfferentLimits[timeStep][limit][coordinate] =
					_proximalAfferentLimits[timeStep][limit][coordinate][1];
				}
			}
			// saves proximalAfferentLowerLimits
			save_vector_of_vectors_as_matrix("proximalAfferentLowerLimits_"+std::to_string(timeStep), lowerAfferentLimits[timeStep], outfile);

			// saves proximalAfferentUpperLimits
			save_vector_of_vectors_as_matrix("proximalAfferentUpperLimits_"+std::to_string(timeStep), upperAfferentLimits[timeStep], outfile);
		}
	}

	if ( _lateralProximalConnections.size() != 0 ) {
		twodvector<double>	lowerLimits, upperLimits;
		auto	numberOfLimits = _proximalLateralLimits.size();
		lowerLimits.resize(numberOfLimits);
		upperLimits.resize(numberOfLimits);
		for ( int limit = 0; limit < (int)numberOfLimits; limit++ ) {
			auto	numberOfCoordinates = _proximalLateralLimits[limit].size();
			lowerLimits[limit].resize(numberOfCoordinates);
			upperLimits[limit].resize(numberOfCoordinates);
			for ( int coordinate = 0; coordinate < (int)numberOfCoordinates; coordinate++ ) {
				lowerLimits[limit][coordinate] = _proximalLateralLimits[limit][coordinate][0];
				upperLimits[limit][coordinate] = _proximalLateralLimits[limit][coordinate][1];
			}
		}

		// saves proximalLateralLowerLimits
		save_vector_of_vectors_as_matrix("proximalLateralLowerLimits", lowerLimits, outfile);

		// saves proximalLateralUpperLimits
		save_vector_of_vectors_as_matrix("proximalLateralUpperLimits", upperLimits, outfile);
	}

	if ( _afferentConnections.size() != 0 ) {
		// saves afferentUpdateStep
		save_as_scalar("afferentUpdateStep", _afferentUpdateStep, outfile);
	}

	if ( _lateralProximalConnections.size() != 0 ) {
		// saves lateralUpdateStep
		save_as_scalar("lateralUpdateStep", _lateralUpdateStep, outfile);
	}

	// saves temporalGatheringAfferentValue
	save_as_scalar("temporalGatheringAfferentValue", _temporalGatheringAfferentValue, outfile);

	if ( _temporalGatheringAfferentValue > 1 ) {
		// saves _internalTemporallyGatheredInputs; then:

		// saves temporalPointer
		save_vector_as_matrix("temporalPointer", _temporalPointer, outfile);

		// saves synchronization
		save_vector_as_matrix("internalSynchronization",
					_internalTemporallyGatheredInputs.synchronization,
					outfile);

		// saves temporallyGatheredInformation
		save_vector_of_vectors_as_matrix("internalTemporallyGatheredInformation",
					_internalTemporallyGatheredInputs.temporallyGatheredInformation,
					outfile);

		// saves temporallyGatheredIndexes
		int	aux;
		make_rectangular(_internalTemporallyGatheredInputs.temporallyGatheredIndexes, -1);
		save_multidimensional_vector_as_matrix("internalTemporallyGatheredIndexes",
							_internalTemporallyGatheredInputs.temporallyGatheredIndexes,
							aux, outfile);
	}

	for ( int column = 0; column < _columnsDimensionality; column++ ) {
		_layerColumns[column].saveTemporalUnitsStatus(std::to_string(column), outfile);
		_layerColumns[column].saveSelfOrganizingMapStatus(std::to_string(column), outfile);
	}
	// close the opened file.
	outfile.close();
} // end function saveLayerStatus


// function that loads the Layer's status in a file
void	Layer::loadLayerStatus( const std::string& fileName )
{
	int	lowerTimeStep = 0;
	int	upperTimeStep = 0;

	std::string	str;
	std::string	STR;

	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + fileName + ".mat", ios::in | std::ifstream::binary);

	while ( std::getline(infile, str) ) {

		STR = "# name: afferentArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(_afferentArrayDimensionality, infile);

		STR = "# name: afferentDimensionality";
		if ( str.compare(STR) == 0 )
			load_scalar(_afferentDimensionality, infile);

		STR = "# name: afferentReceptiveField";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(_afferentReceptiveField, infile);

		STR = "# name: afferentPercentage";
		if ( str.compare(STR) == 0 )
			load_scalar(_afferentPercentage, infile);

		STR = "# name: afferentWrapAround";
		if ( str.compare(STR) == 0 )
			load_bool(_afferentWrapAround, infile);



		STR = "# name: apicalArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(_apicalArrayDimensionality, infile);

		STR = "# name: apicalDimensionality";
		if ( str.compare(STR) == 0 )
			load_scalar(_apicalDimensionality, infile);

		STR = "# name: apicalReceptiveField";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(_apicalReceptiveField, infile);

		STR = "# name: apicalPercentage";
		if ( str.compare(STR) == 0 )
			load_scalar(_apicalPercentage, infile);

		STR = "# name: apicalWrapAround";
		if ( str.compare(STR) == 0 )
			load_bool(_apicalWrapAround, infile);



		STR = "# name: columnsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(_columnsArrayDimensionality, infile);

		STR = "# name: columnsDimensionality";
		if ( str.compare(STR) == 0 )
			load_scalar(_columnsDimensionality, infile);



		STR = "# name: lateralProximalReceptiveField";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(_lateralProximalReceptiveField, infile);

		STR = "# name: lateralProximalPercentage";
		if ( str.compare(STR) == 0 )
			load_scalar(_lateralProximalPercentage, infile);

		STR = "# name: lateralProximalWrapAround";
		if ( str.compare(STR) == 0 )
			load_bool(_lateralProximalWrapAround, infile);



		STR = "# name: lateralDistalReceptiveField";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(_lateralDistalReceptiveField, infile);

		STR = "# name: lateralDistalPercentage";
		if ( str.compare(STR) == 0 )
			load_scalar(_lateralDistalPercentage, infile);

		STR = "# name: lateralDistalWrapAround";
		if ( str.compare(STR) == 0 )
			load_bool(_lateralDistalWrapAround, infile);



		STR = "# name: afferentConnections";
		if ( str.compare(STR) == 0 ) {
			SparseMatrixElements<bool>	sparseAfferentConnectionsBitMap;
			load_sparse_matrix_to_sparse_matrix_elements(sparseAfferentConnectionsBitMap, infile);
			twodvector<bool>	afferentConnectionsBitMap;
			afferentConnectionsBitMap = from_sparse(sparseAfferentConnectionsBitMap);
			_afferentConnections.resize(_columnsDimensionality);
			for ( int column = 0; column < (int)afferentConnectionsBitMap.size(); column++ ) {
				for ( int index = 0; index < (int)afferentConnectionsBitMap[column].size(); index++ ) {
					if ( afferentConnectionsBitMap[column][index] == true )
						_afferentConnections[column].push_back(index);

					_afferentConnections[column].shrink_to_fit();
				}
			}

		}

		STR = "# name: lateralProximalConnections";
		if ( str.compare(STR) == 0 ) {
			SparseMatrixElements<bool>	sparseLateralProximalConnectionsBitMap;
			load_sparse_matrix_to_sparse_matrix_elements(sparseLateralProximalConnectionsBitMap, infile);
			twodvector<bool>	lateralProximalConnectionsBitMap;
			lateralProximalConnectionsBitMap = from_sparse(sparseLateralProximalConnectionsBitMap);
			_lateralProximalConnections.resize(_columnsDimensionality);
			for ( int column = 0; column < (int)lateralProximalConnectionsBitMap.size(); column++ ) {
				for ( int index = 0; index < (int)lateralProximalConnectionsBitMap[column].size(); index++ ) {
					if ( lateralProximalConnectionsBitMap[column][index] == true )
						_lateralProximalConnections[column].push_back(index);

					_lateralProximalConnections[column].shrink_to_fit();
				}
			}

		}

		STR = "# name: lateralDistalConnections";
		if ( str.compare(STR) == 0 ) {
			SparseMatrixElements<bool>	sparseLateralDistalConnectionsBitMap;
			load_sparse_matrix_to_sparse_matrix_elements(sparseLateralDistalConnectionsBitMap, infile);
			twodvector<bool>	lateralDistalConnectionsBitMap;
			lateralDistalConnectionsBitMap = from_sparse(sparseLateralDistalConnectionsBitMap);
			_lateralDistalConnections.resize(_columnsDimensionality);
			for ( int column = 0; column < (int)lateralDistalConnectionsBitMap.size(); column++ ) {
				for ( int index = 0; index < (int)lateralDistalConnectionsBitMap[column].size(); index++ ) {
					if ( lateralDistalConnectionsBitMap[column][index] == true )
						_lateralDistalConnections[column].push_back(index);

					_lateralDistalConnections[column].shrink_to_fit();
				}
			}

		}

		STR = "# name: apicalConnections";
		if ( str.compare(STR) == 0 ) {
			SparseMatrixElements<bool>	sparseApicalConnectionsBitMap;
			load_sparse_matrix_to_sparse_matrix_elements(sparseApicalConnectionsBitMap, infile);
			twodvector<bool>	apicalConnectionsBitMap;
			apicalConnectionsBitMap = from_sparse(sparseApicalConnectionsBitMap);
			_apicalConnections.resize(_columnsDimensionality);
			for ( int column = 0; column < (int)apicalConnectionsBitMap.size(); column++ ) {
				for ( int index = 0; index < (int)apicalConnectionsBitMap[column].size(); index++ ) {
					if ( apicalConnectionsBitMap[column][index] == true )
						_apicalConnections[column].push_back(index);

					_apicalConnections[column].shrink_to_fit();
				}
			}
		}

		STR = "# name: populationsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(_populationsArrayDimensionality, infile);

		STR = "# name: afferentPopulationsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(_afferentPopulationsArrayDimensionality, infile);

		STR = "# name: apicalPopulationsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(_apicalPopulationsArrayDimensionality, infile);



		STR = "# name: proximalAfferentLowerLimits_"+std::to_string(lowerTimeStep);
		if ( str.compare(STR) == 0 ) {
			if ( lowerTimeStep+1 > (int)_proximalAfferentLimits.size() )
				_proximalAfferentLimits.resize(lowerTimeStep+1);

			twodvector<double>	lowerLimits;
			load_matrix_to_vector_of_vectors(lowerLimits, infile);
			auto	numberOfLimits = lowerLimits.size();
			_proximalAfferentLimits[lowerTimeStep].resize(numberOfLimits);
			for ( int limit = 0; limit < (int)numberOfLimits; limit++ ) {
				auto	numberOfCoordinates = lowerLimits[limit].size();
				_proximalAfferentLimits[lowerTimeStep][limit].resize(numberOfCoordinates);
				for ( int coordinate = 0; coordinate < (int)numberOfCoordinates; coordinate++ ) {
					_proximalAfferentLimits[lowerTimeStep][limit][coordinate][0] = lowerLimits[limit][coordinate];
				}
			}
			lowerTimeStep++;
		}

		STR = "# name: proximalAfferentUpperLimits_"+std::to_string(upperTimeStep);
		if ( str.compare(STR) == 0 ) {
			if ( upperTimeStep+1 > (int)_proximalAfferentLimits.size() )
				_proximalAfferentLimits.resize(upperTimeStep+1);

			twodvector<double>	upperLimits;
			load_matrix_to_vector_of_vectors(upperLimits, infile);
			auto	numberOfLimits = upperLimits.size();
			_proximalAfferentLimits[upperTimeStep].resize(numberOfLimits);
			for ( int limit = 0; limit < (int)numberOfLimits; limit++ ) {
				auto	numberOfCoordinates = upperLimits[limit].size();
				_proximalAfferentLimits[upperTimeStep][limit].resize(numberOfCoordinates);
				for ( int coordinate = 0; coordinate < (int)numberOfCoordinates; coordinate++ ) {
					_proximalAfferentLimits[upperTimeStep][limit][coordinate][1] = upperLimits[limit][coordinate];
				}
			}
			upperTimeStep++;
		}

		STR = "# name: proximalLateralLowerLimits";
		if ( str.compare(STR) == 0 ) {
			twodvector<double>	lowerLimits;
			load_matrix_to_vector_of_vectors(lowerLimits, infile);
			auto	numberOfLimits = lowerLimits.size();
			_proximalLateralLimits.resize(numberOfLimits);
			for ( int limit = 0; limit < (int)numberOfLimits; limit++ ) {
				auto	numberOfCoordinates = lowerLimits[limit].size();
				_proximalLateralLimits[limit].resize(numberOfCoordinates);
				for ( int coordinate = 0; coordinate < (int)numberOfCoordinates; coordinate++ ) {
					_proximalLateralLimits[limit][coordinate][0] = lowerLimits[limit][coordinate];
				}
			}
		}

		STR = "# name: proximalLateralUpperLimits";
		if ( str.compare(STR) == 0 ) {
			twodvector<double>	upperLimits;
			load_matrix_to_vector_of_vectors(upperLimits, infile);
			auto	numberOfLimits = upperLimits.size();
			_proximalLateralLimits.resize(numberOfLimits);
			for ( int limit = 0; limit < (int)numberOfLimits; limit++ ) {
				auto	numberOfCoordinates = upperLimits[limit].size();
				_proximalLateralLimits[limit].resize(numberOfCoordinates);
				for ( int coordinate = 0; coordinate < (int)numberOfCoordinates; coordinate++ ) {
					_proximalLateralLimits[limit][coordinate][1] = upperLimits[limit][coordinate];
				}
			}
		}

		STR = "# name: afferentUpdateStep";
		if ( str.compare(STR) == 0 )
			load_scalar(_afferentUpdateStep, infile);

		STR = "# name: lateralUpdateStep";
		if ( str.compare(STR) == 0 )
			load_scalar(_lateralUpdateStep, infile);

		STR = "# name: temporalGatheringAfferentValue";
		if ( str.compare(STR) == 0 )
			load_scalar(_temporalGatheringAfferentValue, infile);

		STR = "# name: temporalPointer";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(_temporalPointer, infile);

		STR = "# name: internalSynchronization";
		if ( str.compare(STR) == 0 ) {
			std::vector<std::size_t>	auxiliary;
			load_matrix_to_vector(auxiliary, infile);
			for(const auto& aux : auxiliary) {
				if ( aux == 0 )
					_internalTemporallyGatheredInputs.synchronization.push_back(false);
				else
					_internalTemporallyGatheredInputs.synchronization.push_back(true);
			}
			_internalTemporallyGatheredInputs.synchronization.shrink_to_fit();
		}

		STR = "# name: internalTemporallyGatheredInformation";
		if ( str.compare(STR) == 0 ) {
			twodvector<std::size_t>	auxiliary;
			load_matrix_to_vector_of_vectors(auxiliary, infile);
			_internalTemporallyGatheredInputs.temporallyGatheredInformation.resize(auxiliary.size());
			for( std::size_t timeStep = 0; timeStep < auxiliary.size(); timeStep++ ) {
				for(const auto& aux : auxiliary[timeStep]) {
					if ( aux == 0 )
						_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].push_back(false);
					else
						_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].push_back(true);
				}
			}
		}

		STR = "# name: internalTemporallyGatheredIndexes";
		if ( str.compare(STR) == 0 ) {
			int	aux;
			load_matrix_to_multidimensional_vector(_internalTemporallyGatheredInputs.temporallyGatheredIndexes,
								aux, infile);
		}
	}
	// close the opened file.
	infile.close();

	for ( int column = 0; column < _columnsDimensionality; column++ )
		_layerColumns.push_back(TemporalPopulation(fileName, std::to_string(column)));

	_layerColumns.shrink_to_fit();
} // end function loadLayerStatus


// normalizes neuron coordinates in the certain population
std::vector<double>	Layer::Normalize( const std::vector<int>& coordinates,
					const std::vector<int>& arrayDimensionality )
{
	std::vector<double>	normalizedCoordinates;

	normalizedCoordinates.resize(arrayDimensionality.size());

	std::transform(coordinates.begin(), coordinates.end(), arrayDimensionality.begin(),
		       normalizedCoordinates.begin(), std::divides<double>());

	return	normalizedCoordinates;
} // end function Normalize







































