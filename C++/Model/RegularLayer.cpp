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

// File Name:		RegularLayer.cpp
// File Description:	RegularLayer member-function definitions. This file contains implementations of the
//			member functions prototyped in RegularLayer.h.

#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <omp.h>

#include "../Libraries/Model/Random.h"
#include "../Libraries/Model/Topology.h"
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/Templates.h"
#include "../Libraries/Model/OctaveInterface.h"
#include "RegularLayer.h"

using namespace std;


// default constructor
RegularLayer::RegularLayer() {}

// constructor that initializes ...
RegularLayer::RegularLayer( const regularLayerStructure& structure )
{
	RegularLayer::layerInitializer(structure);
} // end RegularLayer constructor


// constructor that initializes an object instantiation of this class
RegularLayer::RegularLayer( const std::string& folderName,
			    const std::size_t identifyer )
{
	RegularLayer::loadRegularLayerStatus(folderName, identifyer);
} // end RegularLayer constructor


// this function initializes an object instantiation of this class
void	RegularLayer::layerInitializer( const regularLayerStructure& structure )
{
	RegularLayer::validateRegularLayer(structure.afferentArrayDimensionality,
					   structure.apicalArrayDimensionality,
					   structure.columnsArrayDimensionality,
					   structure.afferentReceptiveField,
					   structure.afferentPercentage,
					   structure.afferentWrapAround,
					   structure.lateralProximalReceptiveField,
					   structure.lateralProximalPercentage,
					   structure.lateralProximalWrapAround,
					   structure.lateralDistalReceptiveField,
					   structure.lateralDistalPercentage,
					   structure.lateralDistalWrapAround,
					   structure.apicalReceptiveField,
					   structure.apicalPercentage,
					   structure.apicalWrapAround,
					   structure.iterationNum,
					   structure.temporalGatheringAfferentValue,
					   structure.populationsArrayDimensionality,
					   structure.afferentPopulationsArrayDimensionality,
					   structure.apicalPopulationsArrayDimensionality,
					   structure.potentialPercentage);

	RegularLayer::interconnectRegularLayerColumns(structure.afferentArrayDimensionality,
						      structure.apicalArrayDimensionality,
						      structure.columnsArrayDimensionality,
						      structure.afferentReceptiveField,
						      structure.afferentPercentage,
						      structure.afferentWrapAround,
						      structure.lateralProximalReceptiveField,
						      structure.lateralProximalPercentage,
						      structure.lateralProximalWrapAround,
						      structure.lateralDistalReceptiveField,
						      structure.lateralDistalPercentage,
						      structure.lateralDistalWrapAround,
						      structure.apicalReceptiveField,
						      structure.apicalPercentage,
						      structure.apicalWrapAround);

	RegularLayer::generateColumns(structure.populationsArrayDimensionality,
				      structure.afferentPopulationsArrayDimensionality,
				      structure.apicalPopulationsArrayDimensionality,
				      structure.temporalGatheringAfferentValue,
				      structure.potentialPercentage);

	_temporalGatheringAfferentValue = structure.temporalGatheringAfferentValue;

	if ( _temporalGatheringAfferentValue > 1 )
		RegularLayer::initializeInternalTemporallyGatheredInputs();

	RegularLayer::initializeWiredVariables();
} // end function layerInitializer


//this function initializes wired variables
void	RegularLayer::initializeWiredVariables()
{

	assert(_columnsDimensionality > 0);
	_lateral.currentIndexes.resize(_columnsDimensionality);
	_lateral.synchronization.resize(_columnsDimensionality);
	_lateral.information.resize(_columnsDimensionality);
	for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
		_lateral.synchronization[column] = true;
		_lateral.information[column] = false;
	}

	assert(_apicalDimensionality > 0);
	_apical.currentIndexes.resize(_apicalDimensionality);
	_apical.synchronization.resize(_apicalDimensionality);
	_apical.information.resize(_apicalDimensionality);
	for ( std::size_t column = 0; column < _apicalDimensionality; column++ ) {
		_apical.synchronization[column] = true;
		_apical.information[column] = false;
	}

} // end function initializeWiredVariables


// validates the layer
void	RegularLayer::validateRegularLayer(  const std::vector<std::size_t>& afferentArrayDimensionality,
					     const std::vector<std::size_t>& apicalArrayDimensionality,
					     const std::vector<std::size_t>& columnsArrayDimensionality,
					     const std::vector<std::size_t>& afferentReceptiveField,
					     const double afferentPercentage,
					     const bool afferentWrapAround,
					     const std::vector<std::size_t>& lateralProximalReceptiveField,
					     const double lateralProximalPercentage,
					     const bool lateralProximalWrapAround,
					     const std::vector<std::size_t>& lateralDistalReceptiveField,
					     const double lateralDistalPercentage,
					     const bool lateralDistalWrapAround,
					     const std::vector<std::size_t>& apicalReceptiveField,
					     const double apicalPercentage,
					     const bool apicalWrapAround,
					     std::size_t iterationNum,
					     const std::size_t temporalGatheringAfferentValue,
					     const std::vector<std::size_t>& populationsArrayDimensionality,
					     const std::vector<std::size_t>& afferentPopulationsArrayDimensionality,
					     const std::vector<std::size_t>& apicalPopulationsArrayDimensionality,
	       				     const double potentialPercentage )
{
	RegularLayer::validateComumnsInterconnectionsParameters(afferentArrayDimensionality,
								apicalArrayDimensionality,
								columnsArrayDimensionality,
								afferentReceptiveField,
							       	afferentPercentage,
								afferentWrapAround,
								lateralProximalReceptiveField,
							       	lateralProximalPercentage,
								lateralProximalWrapAround,
								lateralDistalReceptiveField,
							       	lateralDistalPercentage,
								lateralDistalWrapAround,
								apicalReceptiveField,
							       	apicalPercentage,
								apicalWrapAround,
							       	temporalGatheringAfferentValue);

	RegularLayer::validatePopulationParameters(populationsArrayDimensionality,
						   afferentPopulationsArrayDimensionality,
						   apicalPopulationsArrayDimensionality,
						   temporalGatheringAfferentValue,
						   potentialPercentage);

} // end function validateRegularLayer


// validates the parameters to configure the columns' interconnections
void	RegularLayer::validateComumnsInterconnectionsParameters( const std::vector<std::size_t>& afferentArrayDimensionality,
								 const std::vector<std::size_t>& apicalArrayDimensionality,
								 const std::vector<std::size_t>& columnsArrayDimensionality,
								 const std::vector<std::size_t>& afferentReceptiveField,
								 const double afferentPercentage,
								 const bool afferentWrapAround,
								 const std::vector<std::size_t>& lateralProximalReceptiveField,
								 const double lateralProximalPercentage,
								 const bool lateralProximalWrapAround,
								 const std::vector<std::size_t>& lateralDistalReceptiveField,
								 const double lateralDistalPercentage,
								 const bool lateralDistalWrapAround,
								 const std::vector<std::size_t>& apicalReceptiveField,
								 const double apicalPercentage,
								 const bool apicalWrapAround,
								 const std::size_t temporalGatheringAfferentValue )
{
	if ( temporalGatheringAfferentValue == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";
		std::cout << "temporalGatheringAfferentValue = " << temporalGatheringAfferentValue << "\n";
		exit( EXIT_FAILURE );
	}

	if ( afferentArrayDimensionality.size() == 0 ||
	     apicalArrayDimensionality.size() == 0 ||
	     columnsArrayDimensionality.size() == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";
		std::cout << "afferentArrayDimensionality.size() = " << afferentArrayDimensionality.size() << "\n";
		std::cout << "apicalArrayDimensionality.size() = " << apicalArrayDimensionality.size() << "\n";
		std::cout << "columnsArrayDimensionality.size() = " << columnsArrayDimensionality.size() << "\n";
		exit( EXIT_FAILURE );
	}

	for ( std::size_t dim = 0; dim < afferentArrayDimensionality.size(); dim++ ) {
		if ( afferentArrayDimensionality[dim] == 0 ) {
			std::cout << "RegularLayer object construction inconsistence: \n";
			std::cout << "In function validateComumnsInterconnectionsParameters\n";
			std::cout << "afferentArrayDimensionality[" << dim << "] = " << afferentArrayDimensionality[dim] << "\n";
			exit( EXIT_FAILURE );
		}
	}

	for ( std::size_t dim = 0; dim < apicalArrayDimensionality.size(); dim++ ) {
		if ( apicalArrayDimensionality[dim] == 0 ) {
			std::cout << "RegularLayer object construction inconsistence: \n";
			std::cout << "In function validateComumnsInterconnectionsParameters\n";
			std::cout << "apicalArrayDimensionality[" << dim << "] = " << apicalArrayDimensionality[dim] << "\n";
			exit( EXIT_FAILURE );
		}
	}

	for ( std::size_t dim = 0; dim < columnsArrayDimensionality.size(); dim++ ) {
		if ( columnsArrayDimensionality[dim] == 0 ) {
			std::cout << "RegularLayer object construction inconsistence: \n";
			std::cout << "In function validateComumnsInterconnectionsParameters\n";
			std::cout << "columnsArrayDimensionality[" << dim << "] = " << columnsArrayDimensionality[dim] << "\n";
			exit( EXIT_FAILURE );
		}
	}

	if ( afferentReceptiveField.size() == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";

		std::cout << "afferentReceptiveField.size() = " << afferentReceptiveField.size() << "\n";
	}

	if ( (afferentArrayDimensionality.size() != afferentReceptiveField.size()) ||
	     (apicalReceptiveField.size() != 0 && apicalArrayDimensionality.size() != apicalReceptiveField.size()) ||
	     (lateralProximalReceptiveField.size() != 0 && columnsArrayDimensionality.size() != lateralProximalReceptiveField.size()) ||
	     (lateralDistalReceptiveField.size() != 0 && columnsArrayDimensionality.size() != lateralDistalReceptiveField.size()) ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
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

	for ( std::size_t dim = 0; dim < afferentReceptiveField.size(); dim++ ) {
		if (afferentArrayDimensionality[dim] % 2) {
			if ( afferentReceptiveField[dim] > afferentArrayDimensionality[dim]/2 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "afferentReceptiveField[" << dim << "] = " << afferentReceptiveField[dim] << "\n";
				std::cout << "while afferentArrayDimensionality[" << dim << "] is = "
				          << afferentArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
		else {
			if ( afferentReceptiveField[dim] > afferentArrayDimensionality[dim]/2-1 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "afferentReceptiveField[" << dim << "] = " << afferentReceptiveField[dim] << "\n";
				std::cout << "while afferentArrayDimensionality[" << dim << "] is = "
				          << afferentArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	for ( std::size_t dim = 0; dim < apicalReceptiveField.size(); dim++ ) {
		if (apicalArrayDimensionality[dim] % 2) {
			if ( apicalReceptiveField[dim] > apicalArrayDimensionality[dim]/2 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "apicalReceptiveField[" << dim << "] = " << apicalReceptiveField[dim] << "\n";
				std::cout << "while apicalArrayDimensionality[" << dim << "] is = "
				          << apicalArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
		else {
			if ( apicalReceptiveField[dim] > apicalArrayDimensionality[dim]/2-1 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "apicalReceptiveField[" << dim << "] = " << apicalReceptiveField[dim] << "\n";
				std::cout << "while apicalArrayDimensionality[" << dim << "] is = "
				          << apicalArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	for ( std::size_t dim = 0; dim < lateralProximalReceptiveField.size(); dim++ ) {
		if (columnsArrayDimensionality[dim] % 2) {
			if ( lateralProximalReceptiveField[dim] > columnsArrayDimensionality[dim]/2 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "lateralProximalReceptiveField[" << dim << "] = " << lateralProximalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
		else {
			if ( lateralProximalReceptiveField[dim] > columnsArrayDimensionality[dim]/2-1 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "lateralProximalReceptiveField[" << dim << "] = " << lateralProximalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	for ( std::size_t dim = 0; dim < lateralDistalReceptiveField.size(); dim++ ) {
		if (columnsArrayDimensionality[dim] % 2) {
			if ( lateralDistalReceptiveField[dim] > columnsArrayDimensionality[dim]/2 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "lateralDistalReceptiveField[" << dim << "] = " << lateralDistalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
		else {
			if ( lateralDistalReceptiveField[dim] > columnsArrayDimensionality[dim]/2-1 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "lateralDistalReceptiveField[" << dim << "] = " << lateralDistalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	if ( afferentPercentage < 0.0 || afferentPercentage > 1.0 ||
	     lateralProximalPercentage < 0.0 || lateralProximalPercentage > 1.0 ||
	     lateralDistalPercentage < 0.0 || lateralDistalPercentage > 1.0 ||
	     apicalPercentage < 0.0 || apicalPercentage > 1.0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";
		std::cout << "afferentPercentage = " << afferentPercentage << "\n";
		std::cout << "lateralProximalPercentage = " << lateralProximalPercentage << "\n";
		std::cout << "lateralDistalPercentage = " << lateralDistalPercentage << "\n";
		std::cout << "apicalPercentage = " << apicalPercentage << "\n";
		exit( EXIT_FAILURE );
	}
} // end function validateComumnsInterconnectionsParameters


// validates the parameters used to generate the columns' populations
void	RegularLayer::validatePopulationParameters( const std::vector<std::size_t>& populationsArrayDimensionality,
						    const std::vector<std::size_t>& afferentPopulationsArrayDimensionality,
						    const std::vector<std::size_t>& apicalPopulationsArrayDimensionality,
						    const std::size_t temporalGatheringAfferentValue,
	       					    const double potentialPercentage )
{
	if ( potentialPercentage < 0.0 || potentialPercentage > 1.0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "potentialPercentage = "
			  << potentialPercentage << "\n";
		exit( EXIT_FAILURE );
	}

	if ( temporalGatheringAfferentValue == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "temporalGatheringAfferentValue = "
			  << temporalGatheringAfferentValue << "\n";
		exit( EXIT_FAILURE );
	}

	if ( afferentPopulationsArrayDimensionality.size() == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "afferentPopulationsArrayDimensionality.size() = "
			  << afferentPopulationsArrayDimensionality.size() << "\n";
		exit( EXIT_FAILURE );
	}
	else {
		for(const auto& s: afferentPopulationsArrayDimensionality) {
			if ( s == 0 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validatePopulationParameters\n";
				std::cout << "afferentPopulationsArrayDimensionality = \n";
				for(const auto& v : afferentPopulationsArrayDimensionality)
					std::cout << v << ' ';

				std::cout << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	if ( apicalPopulationsArrayDimensionality.size() == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "apicalPopulationsArrayDimensionality.size() = "
			  << apicalPopulationsArrayDimensionality.size() << "\n";
		exit( EXIT_FAILURE );
	}
	else {
		for(const auto& s: apicalPopulationsArrayDimensionality) {
			if ( s == 0 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validatePopulationParameters\n";
				std::cout << "apicalPopulationsArrayDimensionality = \n";
				for(const auto& v : apicalPopulationsArrayDimensionality)
					std::cout << v << ' ';

				std::cout << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	if ( populationsArrayDimensionality.size() == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "populationsArrayDimensionality.size() = "
			  << populationsArrayDimensionality.size() << "\n";
		exit( EXIT_FAILURE );
	}
	else {
		for(const auto& s: populationsArrayDimensionality) {
			if ( s == 0 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validatePopulationParameters\n";
				std::cout << "populationsArrayDimensionality = \n";
				for(const auto& v : populationsArrayDimensionality)
					std::cout << v << ' ';

				std::cout << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}
} // end function validatePopulationParameters


// interconnects the layaer's populations
void	RegularLayer::interconnectRegularLayerColumns( const std::vector<std::size_t>& afferentArrayDimensionality,
						       const std::vector<std::size_t>& apicalArrayDimensionality,
						       const std::vector<std::size_t>& columnsArrayDimensionality,
						       const std::vector<std::size_t>& afferentReceptiveField,
						       const double afferentPercentage,
						       const bool afferentWrapAround,
						       const std::vector<std::size_t>& lateralProximalReceptiveField,
						       const double lateralProximalPercentage,
						       const bool lateralProximalWrapAround,
						       const std::vector<std::size_t>& lateralDistalReceptiveField,
						       const double lateralDistalPercentage,
						       const bool lateralDistalWrapAround,
						       const std::vector<std::size_t>& apicalReceptiveField,
						       const double apicalPercentage,
						       const bool apicalWrapAround )
{
	_afferentArrayDimensionality = afferentArrayDimensionality;
	_apicalArrayDimensionality = apicalArrayDimensionality;
	_columnsArrayDimensionality = columnsArrayDimensionality;

	_columnsDimensionality =  std::accumulate(_columnsArrayDimensionality.begin(),
						  _columnsArrayDimensionality.end(),
						  1, std::multiplies<std::size_t>());

	_afferentDimensionality =  std::accumulate(_afferentArrayDimensionality.begin(),
						  _afferentArrayDimensionality.end(),
						  1, std::multiplies<std::size_t>());

	_apicalDimensionality =  std::accumulate(_apicalArrayDimensionality.begin(),
						 _apicalArrayDimensionality.end(),
						 1, std::multiplies<std::size_t>());

	_afferentReceptiveField = afferentReceptiveField;
	_afferentPercentage = afferentPercentage;
	_afferentWrapAround = afferentWrapAround;

	assert(afferentReceptiveField.size() != 0);
	_afferentConnections.resize(_columnsDimensionality);
	for ( std::size_t column = 0; column < _columnsDimensionality; column++ )
		_afferentConnections[column] = RegularLayer::getAfferentInputs(column);

	_lateralProximalReceptiveField = lateralProximalReceptiveField;
	_lateralProximalPercentage = lateralProximalPercentage;
	_lateralProximalWrapAround = lateralProximalWrapAround;

	if ( lateralProximalReceptiveField.size() != 0 ) {
		_lateralProximalConnections.resize(_columnsDimensionality);
		for ( std::size_t column = 0; column < _columnsDimensionality; column++ )
			_lateralProximalConnections[column] = RegularLayer::getLateralProximalInputs(column);
	}

	_lateralDistalReceptiveField = lateralDistalReceptiveField;
	_lateralDistalPercentage = lateralDistalPercentage;
	_lateralDistalWrapAround = lateralDistalWrapAround;

	if ( lateralDistalReceptiveField.size() != 0 ) {
		_lateralDistalConnections.resize(_columnsDimensionality);
		for ( std::size_t column = 0; column < _columnsDimensionality; column++ )
			_lateralDistalConnections[column] = RegularLayer::getLateralDistalInputs(column);
	}

	_apicalReceptiveField = apicalReceptiveField;
	_apicalPercentage = apicalPercentage;
	_apicalWrapAround = apicalWrapAround;

	if ( apicalReceptiveField.size() != 0 ) {
		_apicalConnections.resize(_columnsDimensionality);
		for ( std::size_t column = 0; column < _columnsDimensionality; column++ )
			_apicalConnections[column] = RegularLayer::getApicalInputs(column);
	}
} // end function interconnectRegularLayerColumns


// generates a population for every column in the layer
void	RegularLayer::generateColumns( const std::vector<std::size_t>& populationsArrayDimensionality,
				       const std::vector<std::size_t>& afferentPopulationsArrayDimensionality,
				       const std::vector<std::size_t>& apicalPopulationsArrayDimensionality,
				       const std::size_t temporalGatheringAfferentValue,
	       			       const double potentialPercentage )
{
	std::size_t			numberOfInputs;
	std::vector<std::size_t>	dynamicUnits;

	assert(_afferentConnections.size() != 0);
	for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
		numberOfInputs = 0;

		for ( std::size_t afferentInput = 0; afferentInput < _afferentConnections[column].size(); afferentInput++ )
			numberOfInputs += std::accumulate(afferentPopulationsArrayDimensionality.begin(),
							  afferentPopulationsArrayDimensionality.end(),
							  1, std::multiplies<std::size_t>()); 

		numberOfInputs *= temporalGatheringAfferentValue;

		if ( _lateralProximalConnections.size() > 0 ) {
			for ( std::size_t lateralProximalInput = 0; lateralProximalInput < _lateralProximalConnections[column].size(); lateralProximalInput++ )
				numberOfInputs += std::accumulate(populationsArrayDimensionality.begin(),
								  populationsArrayDimensionality.end(),
								  1, std::multiplies<std::size_t>());
		}

		if ( _apicalConnections.size() > 0 )
			for ( std::size_t link = 0; link < _apicalConnections[column].size(); link++ ) {
				auto	auxiliary = std::accumulate(apicalPopulationsArrayDimensionality.begin(),
									 apicalPopulationsArrayDimensionality.end(),
									 1, std::multiplies<std::size_t>());
				dynamicUnits.push_back(auxiliary);
			}

		if ( _lateralDistalConnections.size() > 0 )
			for ( std::size_t link = 0; link < _lateralDistalConnections[column].size(); link++ ) {
				auto	auxiliary = std::accumulate(populationsArrayDimensionality.begin(),
									 populationsArrayDimensionality.end(),
									 1, std::multiplies<std::size_t>());
				dynamicUnits.push_back(auxiliary);
			}

		dynamicUnits.shrink_to_fit();
		std::array<double,2>	weightLimits = {SYNAPTIC_DECREMENT,SYNAPTIC_INCREMENT};
		_layerColumns.push_back(ComplexProcessor(populationsArrayDimensionality,
							 numberOfInputs,
							 potentialPercentage,
							 WEIGHTS_SPARSITY,
							 dynamicUnits,
							 weightLimits));
		dynamicUnits.clear();
	}
	_layerColumns.shrink_to_fit();

	_populationsArrayDimensionality = populationsArrayDimensionality;
	_afferentPopulationsArrayDimensionality = afferentPopulationsArrayDimensionality;
	_apicalPopulationsArrayDimensionality = apicalPopulationsArrayDimensionality;
	_potentialPercentage = potentialPercentage;
} // end function generateColumns


// initializes the internal temporal gathered inputs
void	RegularLayer::initializeInternalTemporallyGatheredInputs()
{
	_internalTemporallyGatheredInputs.temporallyGatheredIndexes.resize(_temporalGatheringAfferentValue);
	_internalTemporallyGatheredInputs.temporallyGatheredInformation.resize(_temporalGatheringAfferentValue);

	for ( std::size_t timeStep = 0; timeStep < _temporalGatheringAfferentValue; timeStep++ ) {
		_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep].resize(_afferentDimensionality);

		auto	it = _internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].begin();
		_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].insert(it,_afferentDimensionality,false);
	}

	auto	it = _internalTemporallyGatheredInputs.synchronization.begin();
	_internalTemporallyGatheredInputs.synchronization.insert(it,_afferentDimensionality,true);

	auto	iter = _temporalPointer.begin();
	_temporalPointer.insert(iter,_afferentDimensionality,0);
} // end function initializeInternalTemporallyGatheredInputs


// computes the layer response
regularLayerResponse	RegularLayer::computeResponse( const regularLayerResponse& afferent,
						       const regularLayerParameters& parameters )
{
	assert( parameters.proximalInformationThreshold >= 0.0 && parameters.proximalInformationThreshold <= 1.0 );
	assert( parameters.distalInformationThreshold >= 0.0 && parameters.distalInformationThreshold <= 1.0 );

	regularLayerResponse	output;

	output.currentIndexes.resize(_columnsDimensionality);
	output.synchronization.resize(_columnsDimensionality);
	output.information.resize(_columnsDimensionality);

	#pragma omp parallel for default(none) shared(afferent, parameters, output)
	for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
		regularLayerProximalInput	proximalInputs;
		if ( _temporalGatheringAfferentValue > 1 ) {
			proximalInputs = RegularLayer::gatherProximalInputs(column,
				       					    RegularLayer::temporalGatherer(afferent),
									    _lateral);
		}
		else {
			proximalInputs = RegularLayer::gatherProximalInputs(column, afferent, _lateral);
		}

		auto	distalInputs = RegularLayer::gatherDistalInputs(column, _apical, _lateral);

		bool	synchronizations;
		bool	information;

		bool	proximalSynchronizations;
		bool	distalSynchronizations;

		double	proximalInformation;
		double	distalInformation;

		// computes synchronizations and information corresponding to proximal inputs
		if ( proximalInputs.synchronization.size() == 0 || proximalInputs.information.size() == 0 )
			throw std::runtime_error ("proximalInputs.synchronization.size() == 0 || proximalInputs.information.size() == 0");

		proximalSynchronizations = !std::all_of(proximalInputs.synchronization.begin(),
							       proximalInputs.synchronization.end(),
							       [](bool v) { return !v; });
		proximalInformation = truePercentage(proximalInputs.information);

		// computes synchronizations and information corresponding to distal inputs
		if ( distalInputs.synchronization.size() != 0 && distalInputs.information.size() != 0 ) {
			distalSynchronizations = !std::all_of(distalInputs.synchronization.begin(),
								       distalInputs.synchronization.end(),
								       [](bool v) { return !v; });
			distalInformation = truePercentage(distalInputs.information);
		}
		else {
			distalSynchronizations = false;
			distalInformation = 0.0;
		}

		// if distalSensitivity, then takes into account distal inputs
		// for the computation of synchronizations and information
		if ( parameters.distalSensitivity ) {
			synchronizations = proximalSynchronizations | distalSynchronizations;
			information = (proximalInformation >= parameters.proximalInformationThreshold) |
				      (distalInformation   >= parameters.distalInformationThreshold);
		}
		// if not distalSensitivity, then does not take into account distal inputs
		// for the computation of synchronizations and information
		else {
			synchronizations = proximalSynchronizations;
			information = (proximalInformation >= parameters.proximalInformationThreshold);
		}

		if ( synchronizations ) { // If there is proximal OR distal synchronization
			if ( information ) { // If there is enough proximal OR distal information
				std::vector<std::size_t>	activeIndexes;
				responseInfo		response;
				if ( parameters.enableLearning ) {
					if ( parameters.learning.enableProximalLearning ) {
						response = _layerColumns[column].learningRule(parameters.learning.proximalLearningRate,
											      parameters.learning.proximalNeighborhood,
											      parameters.learning.plasticity,
											      proximalInputs.sparseDistributedRepresentation,
											      PROXIMAL_SYNAPTIC_THRESHOLD,
											      parameters.activationHomeostasis,
											      true);
						
						_layerColumns[column].homeostasis(true,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					else {
						response = _layerColumns[column].getResponse(proximalInputs.sparseDistributedRepresentation);

						_layerColumns[column].homeostasis(false,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
											       _populationsArrayDimensionality.end(),
											       1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column].Activate(response,
										     distalInputs.activeIndexes,
										     numberOfExcitedUnits,
										     parameters.sparsity,
										     true);

					if ( parameters.learning.enableDistalLearning ) {
						if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
						  			  std::accumulate(_populationsArrayDimensionality.begin(),
											  _populationsArrayDimensionality.end(), 1,
											  std::multiplies<std::size_t>()) ) {
							_layerColumns[column].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_layerColumns[column].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     parameters.learning.distalLearningRate);
						}
						
						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveUnits = _lateral.currentIndexes[column]; 
							_layerColumns[column].Update(lastActiveUnits,
										     distalInputs.activeIndexes,
										     false, DISTAL_SYNAPTIC_THRESHOLD,
										     parameters.learning.distalLearningRate);
						}						
					}
				}
				else {
					response = _layerColumns[column].getResponse(proximalInputs.sparseDistributedRepresentation);
					
					_layerColumns[column].homeostasis(false,
									  parameters.learning.synapticHomeostasis,
									  parameters.activationHomeostasis,
									  PROXIMAL_SYNAPTIC_THRESHOLD);
					
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
											       _populationsArrayDimensionality.end(),
											       1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column].Activate(response,
										     distalInputs.activeIndexes,
										     numberOfExcitedUnits,
										     parameters.sparsity);
				}
				#pragma omp critical
				{
					if ( activeIndexes.size() == 0 ) { // If there are no active indexes
						output.synchronization[column] = true;
						output.information[column] = false;
					}
					else { // If there is -at least- one active index
						output.currentIndexes[column] = activeIndexes;
						output.synchronization[column] = true;
						output.information[column] = true;
					}
				}
			}
			else { // If there is no proximal nor distal information
				#pragma omp critical
				{
					output.synchronization[column] = true;
					output.information[column] = false;
				}
			}
		}
		else { // If there is no proximal nor distal synchronization
			#pragma omp critical
			{
				output.currentIndexes[column] = _lateral.currentIndexes[column];
				output.synchronization[column] = false;
				output.information[column] = _lateral.information[column];
			}
		}
	}

	return	output;
} // end function computeResponse


// computes the layer response
regularLayerResponse	RegularLayer::computeResponse( const regularLayerResponse& afferent,
						       const regularLayerResponse& lateral,
						       const regularLayerParameters& parameters )
{
	assert( parameters.proximalInformationThreshold >= 0.0 && parameters.proximalInformationThreshold <= 1.0 );
	assert( parameters.distalInformationThreshold >= 0.0 && parameters.distalInformationThreshold <= 1.0 );

	regularLayerResponse	output;

	output.currentIndexes.resize(_columnsDimensionality);
	output.synchronization.resize(_columnsDimensionality);
	output.information.resize(_columnsDimensionality);

	#pragma omp parallel for default(none) shared(afferent, lateral, parameters, output)
	for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
		regularLayerProximalInput	proximalInputs;
		if ( _temporalGatheringAfferentValue > 1 ) {
			proximalInputs = RegularLayer::gatherProximalInputs(column,
				       					    RegularLayer::temporalGatherer(afferent),
									    lateral);
		}
		else {
			proximalInputs = RegularLayer::gatherProximalInputs(column, afferent, lateral);
		}

		auto	distalInputs = RegularLayer::gatherDistalInputs(column, _apical, lateral);

		bool	synchronizations;
		bool	information;

		bool	proximalSynchronizations;
		bool	distalSynchronizations;

		double	proximalInformation;
		double	distalInformation;

		// computes synchronizations and information corresponding to proximal inputs
		if ( proximalInputs.synchronization.size() == 0 || proximalInputs.information.size() == 0 )
			throw std::runtime_error ("proximalInputs.synchronization.size() == 0 || proximalInputs.information.size() == 0");

		proximalSynchronizations = !std::all_of(proximalInputs.synchronization.begin(),
							       proximalInputs.synchronization.end(),
							       [](bool v) { return !v; });
		proximalInformation = truePercentage(proximalInputs.information);

		// computes synchronizations and information corresponding to distal inputs
		if ( distalInputs.synchronization.size() != 0 && distalInputs.information.size() != 0 ) {
			distalSynchronizations = !std::all_of(distalInputs.synchronization.begin(),
								       distalInputs.synchronization.end(),
								       [](bool v) { return !v; });
			distalInformation = truePercentage(distalInputs.information);
		}
		else {
			distalSynchronizations = false;
			distalInformation = 0.0;
		}

		// if distalSensitivity, then takes into account distal inputs
		// for the computation of synchronizations and information
		if ( parameters.distalSensitivity ) {
			synchronizations = proximalSynchronizations | distalSynchronizations;
			information = (proximalInformation >= parameters.proximalInformationThreshold) |
				      (distalInformation   >= parameters.distalInformationThreshold);
		}
		// if not distalSensitivity, then does not take into account distal inputs
		// for the computation of synchronizations and information
		else {
			synchronizations = proximalSynchronizations;
			information = (proximalInformation >= parameters.proximalInformationThreshold);
		}

		if ( synchronizations ) { // If there is proximal OR distal synchronization
			if ( information ) { // If there is enough proximal OR distal information
				std::vector<std::size_t>	activeIndexes;
				responseInfo		response;
				if ( parameters.enableLearning ) {
					if ( parameters.learning.enableProximalLearning ) {
						response = _layerColumns[column].learningRule(parameters.learning.proximalLearningRate,
											      parameters.learning.proximalNeighborhood,
											      parameters.learning.plasticity,
											      proximalInputs.sparseDistributedRepresentation,
											      PROXIMAL_SYNAPTIC_THRESHOLD,
											      parameters.activationHomeostasis,
											      true);
						
						_layerColumns[column].homeostasis(true,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					else {
						response = _layerColumns[column].getResponse(proximalInputs.sparseDistributedRepresentation);

						_layerColumns[column].homeostasis(false,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
												   _populationsArrayDimensionality.end(),
												   1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column].Activate(response,
										     distalInputs.activeIndexes,
										     numberOfExcitedUnits,
										     parameters.sparsity,
										     true);

					if ( parameters.learning.enableDistalLearning ) {
						if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
						  			  std::accumulate(_populationsArrayDimensionality.begin(),
											  _populationsArrayDimensionality.end(), 1,
											  std::multiplies<std::size_t>()) ) {
							_layerColumns[column].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_layerColumns[column].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     parameters.learning.distalLearningRate);
						}
						
						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveUnits = lateral.currentIndexes[column]; 
							_layerColumns[column].Update(lastActiveUnits,
										     distalInputs.activeIndexes,
										     false, DISTAL_SYNAPTIC_THRESHOLD,
										     parameters.learning.distalLearningRate);
						}						
					}
				}
				else {
					response = _layerColumns[column].getResponse(proximalInputs.sparseDistributedRepresentation);
					
					_layerColumns[column].homeostasis(false,
									  parameters.learning.synapticHomeostasis,
									  parameters.activationHomeostasis,
									  PROXIMAL_SYNAPTIC_THRESHOLD);
					
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
											       _populationsArrayDimensionality.end(),
											       1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column].Activate(response,
										     distalInputs.activeIndexes,
										     numberOfExcitedUnits,
										     parameters.sparsity);
				}
				#pragma omp critical
				{
					if ( activeIndexes.size() == 0 ) { // If there are no active indexes
						output.synchronization[column] = true;
						output.information[column] = false;
					}
					else { // If there is -at least- one active index
						output.currentIndexes[column] = activeIndexes;
						output.synchronization[column] = true;
						output.information[column] = true;
					}
				}
			}
			else { // If there is no proximal nor distal information
				#pragma omp critical
				{
					output.synchronization[column] = true;
					output.information[column] = false;
				}
			}
		}
		else { // If there is no proximal nor distal synchronization
			#pragma omp critical
			{
				output.currentIndexes[column] = lateral.currentIndexes[column];
				output.synchronization[column] = false;
				output.information[column] = lateral.information[column];
			}
		}
	}

	return	output;
} // end function computeResponse


// computes the layer response
regularLayerResponse	RegularLayer::computeResponse( const regularLayerResponse& afferent,
						       const regularLayerResponse& lateral,
					       	       const regularLayerResponse& apical,
						       const regularLayerParameters& parameters )
{
	assert( parameters.proximalInformationThreshold >= 0.0 && parameters.proximalInformationThreshold <= 1.0 );
	assert( parameters.distalInformationThreshold >= 0.0 && parameters.distalInformationThreshold <= 1.0 );

	regularLayerResponse	output;

	output.currentIndexes.resize(_columnsDimensionality);
	output.synchronization.resize(_columnsDimensionality);
	output.information.resize(_columnsDimensionality);

	#pragma omp parallel for default(none) shared(afferent, lateral, apical, parameters, output)
	for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
		regularLayerProximalInput	proximalInputs;
		if ( _temporalGatheringAfferentValue > 1 ) {
			proximalInputs = RegularLayer::gatherProximalInputs(column,
				       					    RegularLayer::temporalGatherer(afferent),
									    lateral);
		}
		else {
			proximalInputs = RegularLayer::gatherProximalInputs(column, afferent, lateral);
		}

		auto	distalInputs = RegularLayer::gatherDistalInputs(column, apical, lateral);

		bool	synchronizations;
		bool	information;

		bool	proximalSynchronizations;
		bool	distalSynchronizations;

		double	proximalInformation;
		double	distalInformation;

		// computes synchronizations and information corresponding to proximal inputs
		if ( proximalInputs.synchronization.size() == 0 || proximalInputs.information.size() == 0 )
			throw std::runtime_error ("proximalInputs.synchronization.size() == 0 || proximalInputs.information.size() == 0");

		proximalSynchronizations = !std::all_of(proximalInputs.synchronization.begin(),
							       proximalInputs.synchronization.end(),
							       [](bool v) { return !v; });
		proximalInformation = truePercentage(proximalInputs.information);

		// computes synchronizations and information corresponding to distal inputs
		if ( distalInputs.synchronization.size() != 0 && distalInputs.information.size() != 0 ) {
			distalSynchronizations = !std::all_of(distalInputs.synchronization.begin(),
								       distalInputs.synchronization.end(),
								       [](bool v) { return !v; });
			distalInformation = truePercentage(distalInputs.information);
		}
		else {
			distalSynchronizations = false;
			distalInformation = 0.0;
		}

		// if distalSensitivity, then takes into account distal inputs
		// for the computation of synchronizations and information
		if ( parameters.distalSensitivity ) {
			synchronizations = proximalSynchronizations | distalSynchronizations;
			information = (proximalInformation >= parameters.proximalInformationThreshold) |
				      (distalInformation   >= parameters.distalInformationThreshold);
		}
		// if not distalSensitivity, then does not take into account distal inputs
		// for the computation of synchronizations and information
		else {
			synchronizations = proximalSynchronizations;
			information = (proximalInformation >= parameters.proximalInformationThreshold);
		}

		if ( synchronizations ) { // If there is proximal OR distal synchronization
			if ( information ) { // If there is enough proximal OR distal information
				std::vector<std::size_t>	activeIndexes;
				responseInfo		response;
				if ( parameters.enableLearning ) {
					if ( parameters.learning.enableProximalLearning ) {
						response = _layerColumns[column].learningRule(parameters.learning.proximalLearningRate,
											      parameters.learning.proximalNeighborhood,
											      parameters.learning.plasticity,
											      proximalInputs.sparseDistributedRepresentation,
											      PROXIMAL_SYNAPTIC_THRESHOLD,
											      parameters.activationHomeostasis,
											      true);
						
						_layerColumns[column].homeostasis(true,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					else {
						response = _layerColumns[column].getResponse(proximalInputs.sparseDistributedRepresentation);

						_layerColumns[column].homeostasis(false,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
												   _populationsArrayDimensionality.end(),
												   1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column].Activate(response,
										     distalInputs.activeIndexes,
										     numberOfExcitedUnits,
										     parameters.sparsity,
										     true);

					if ( parameters.learning.enableDistalLearning ) {
						if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
						  			  std::accumulate(_populationsArrayDimensionality.begin(),
											  _populationsArrayDimensionality.end(), 1,
											  std::multiplies<std::size_t>()) ) {
							_layerColumns[column].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_layerColumns[column].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     parameters.learning.distalLearningRate);
						}
						
						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveUnits = lateral.currentIndexes[column]; 
							_layerColumns[column].Update(lastActiveUnits,
										     distalInputs.activeIndexes,
										     false, DISTAL_SYNAPTIC_THRESHOLD,
										     parameters.learning.distalLearningRate);
						}						
					}
				}
				else {
					response = _layerColumns[column].getResponse(proximalInputs.sparseDistributedRepresentation);
					
					_layerColumns[column].homeostasis(false,
									  parameters.learning.synapticHomeostasis,
									  parameters.activationHomeostasis,
									  PROXIMAL_SYNAPTIC_THRESHOLD);
					
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
											       _populationsArrayDimensionality.end(),
											       1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column].Activate(response,
										     distalInputs.activeIndexes,
										     numberOfExcitedUnits,
										     parameters.sparsity);
				}
				#pragma omp critical
				{
					if ( activeIndexes.size() == 0 ) { // If there are no active indexes
						output.synchronization[column] = true;
						output.information[column] = false;
					}
					else { // If there is -at least- one active index
						output.currentIndexes[column] = activeIndexes;
						output.synchronization[column] = true;
						output.information[column] = true;
					}
				}
			}
			else { // If there is no proximal nor distal information
				#pragma omp critical
				{
					output.synchronization[column] = true;
					output.information[column] = false;
				}
			}
		}
		else { // If there is no proximal nor distal synchronization
			#pragma omp critical
			{
				output.currentIndexes[column] = lateral.currentIndexes[column];
				output.synchronization[column] = false;
				output.information[column] = lateral.information[column];
			}
		}
	}

	return	output;
} // end function computeResponse


// temporally gathers afferent inputs
regularLayerTemporallyGatheredResponse	RegularLayer::temporalGatherer( const regularLayerResponse& input )
{
	assert(_temporalGatheringAfferentValue > 1);

	assert( input.currentIndexes.size() > 0 &&
		input.currentIndexes.size() == input.synchronization.size() &&
		input.currentIndexes.size() == input.information.size());

	regularLayerTemporallyGatheredResponse	output;

	bool	synchronization = !std::all_of(input.synchronization.begin(),
					       input.synchronization.end(),
					       [](bool v) { return !v; });

	// if the input has some synchronization activity
	if ( synchronization ) {
		// iterates through the afferent input layer columns
		for ( std::size_t column = 0; column < _afferentDimensionality; column++ ) {
			// if there is synchronization activity in this afferent column
			if ( input.synchronization[column] ) {
				// if it is the first internal time step
				if ( _temporalPointer[column] == 0 ) {
					for ( std::size_t timeStep = 0; timeStep < _temporalGatheringAfferentValue; timeStep++ ) {
						_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep][column].clear();
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
					assert(input.currentIndexes[column].size() == 0);
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
	// if there is no synchronization activity from the input
	else {
		std::fill (_internalTemporallyGatheredInputs.synchronization.begin(),
			   _internalTemporallyGatheredInputs.synchronization.end(),false);

		return	_internalTemporallyGatheredInputs;
	}
} // end function temporalGatherer


// gathers proximal inputs from afferent and lateral connections
regularLayerProximalInput	RegularLayer::gatherProximalInputs( const std::size_t column,
								    const regularLayerResponse& afferent,
								    const regularLayerResponse& lateral )
{
	regularLayerProximalInput	proximalInputs;

	std::size_t	offset = 0;
	std::size_t	SDR_offset = 0;
	assert(afferent.currentIndexes.size() != 0);
	assert(_temporalGatheringAfferentValue == 1);

	assert(_afferentConnections.size() != 0);
	std::size_t	numberOfProximalConnections = _afferentConnections[column].size();

	if ( _lateralProximalConnections.size() != 0 )
		numberOfProximalConnections += _lateralProximalConnections[column].size();

	proximalInputs.synchronization.resize(numberOfProximalConnections);
	proximalInputs.information.resize(numberOfProximalConnections);

	for ( std::size_t afferentConnection = 0; afferentConnection < _afferentConnections[column].size(); afferentConnection++ ) {
		auto	connection = _afferentConnections[column][afferentConnection];
		auto	numberOfIndexes = afferent.currentIndexes[connection].size();
		std::vector<std::size_t>	auxiliary;

		// If there is no index, then there is no information
		if ( numberOfIndexes == 0 ) {
			assert(afferent.information[connection] == false);
			assert(afferent.synchronization[connection] == true);
			proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
			proximalInputs.information[offset+afferentConnection] = afferent.information[connection];
		}
		// If there is at least one index then, gather this information
		else {
			assert(afferent.information[connection] == true);
			for (const auto& index : afferent.currentIndexes[connection]) {
				proximalInputs.sparseDistributedRepresentation.push_back(SDR_offset+index);
				auxiliary.push_back(SDR_offset+index);
			}
			proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
			proximalInputs.information[offset+afferentConnection] = afferent.information[connection];
		}
		auto	old_SDR_offset = SDR_offset;
		SDR_offset += std::accumulate(_afferentPopulationsArrayDimensionality.begin(),
					      _afferentPopulationsArrayDimensionality.end(),
					      1, std::multiplies<std::size_t>());

		for (const auto& index : auxiliary)
			assert(index >= old_SDR_offset && index < SDR_offset);
	}
	offset += _afferentConnections[column].size();

	if ( _lateralProximalConnections.size() != 0 ) {
		for ( std::size_t lateralProximalConnection = 0;
				  lateralProximalConnection < _lateralProximalConnections[column].size();
				  lateralProximalConnection++ ) {
			auto	connection = _lateralProximalConnections[column][lateralProximalConnection];
			auto	numberOfIndexes = lateral.currentIndexes[connection].size();
			std::vector<std::size_t>	auxiliary;

			// If there is no index, then there is no information
			if ( numberOfIndexes == 0 ) {
				assert(lateral.information[connection] == false);
				assert(lateral.synchronization[connection] == true);
				proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			}
			// If there is at least one index then, gather this information
			else {
				assert(lateral.information[connection] == true);
				for (const auto& index : lateral.currentIndexes[connection]) {
					proximalInputs.sparseDistributedRepresentation.push_back(SDR_offset+index);
					auxiliary.push_back(SDR_offset+index);
				}
				proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			}
			auto	old_SDR_offset = SDR_offset;
			SDR_offset += std::accumulate(_populationsArrayDimensionality.begin(),
						      _populationsArrayDimensionality.end(),
						      1, std::multiplies<std::size_t>());

			for (const auto& index : auxiliary)
				assert(index >= old_SDR_offset && index < SDR_offset);
		}
	}

	return	proximalInputs;
} // end function gatherProximalInputs


// gathers proximal inputs from afferent and lateral connections
regularLayerProximalInput	RegularLayer::gatherProximalInputs( const std::size_t column,
								    const regularLayerTemporallyGatheredResponse& afferent,
								    const regularLayerResponse& lateral )
{
	regularLayerProximalInput	proximalInputs;

	std::size_t	offset = 0;
	std::size_t	SDR_offset = 0;
	assert(afferent.temporallyGatheredIndexes.size() == _temporalGatheringAfferentValue && _temporalGatheringAfferentValue > 1);
	assert(afferent.temporallyGatheredIndexes.size() == afferent.temporallyGatheredInformation.size());

	assert(_afferentConnections.size() != 0);
	std::size_t	numberOfProximalConnections = afferent.temporallyGatheredIndexes.size() * _afferentConnections[column].size();

	if ( _lateralProximalConnections.size() != 0 )
		numberOfProximalConnections += _lateralProximalConnections[column].size();

	proximalInputs.synchronization.resize(numberOfProximalConnections);
	proximalInputs.information.resize(numberOfProximalConnections);

	for ( std::size_t timeStep = 0; timeStep < afferent.temporallyGatheredIndexes.size(); timeStep++ ) {
		for ( std::size_t afferentConnection = 0;
				  afferentConnection < _afferentConnections[column].size();
				  afferentConnection++ ) {
			auto	connection = _afferentConnections[column][afferentConnection];
			auto	numberOfIndexes = afferent.temporallyGatheredIndexes[timeStep][connection].size();
			std::vector<std::size_t>	auxiliary;

			// If there is no index, then there is no information
			if ( numberOfIndexes == 0 ) {
				assert(afferent.temporallyGatheredInformation[timeStep][connection] == false);
				assert(afferent.synchronization[connection] == true);
				proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
				proximalInputs.information[offset+afferentConnection] = afferent.temporallyGatheredInformation[timeStep][connection];
			}
			// If there is at least one index then, gather this information
			else {
				assert(afferent.temporallyGatheredInformation[timeStep][connection] == true);
				for (const auto& index : afferent.temporallyGatheredIndexes[timeStep][connection]) {
					proximalInputs.sparseDistributedRepresentation.push_back(SDR_offset+index);
					auxiliary.push_back(SDR_offset+index);
				}
				proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
				proximalInputs.information[offset+afferentConnection] = afferent.temporallyGatheredInformation[timeStep][connection];
			}
			auto	old_SDR_offset = SDR_offset;
			SDR_offset += std::accumulate(_afferentPopulationsArrayDimensionality.begin(),
						      _afferentPopulationsArrayDimensionality.end(),
						      1, std::multiplies<double>());

			for (const auto& index : auxiliary)
				assert(index >= old_SDR_offset && index < SDR_offset);
		}
		offset += _afferentConnections[column].size();
	}

	if ( _lateralProximalConnections.size() != 0 ) {
		for ( std::size_t lateralProximalConnection = 0;
				  lateralProximalConnection < _lateralProximalConnections[column].size();
				  lateralProximalConnection++ ) {
			auto	connection = _lateralProximalConnections[column][lateralProximalConnection];
			auto	numberOfIndexes = lateral.currentIndexes[connection].size();
			std::vector<std::size_t>	auxiliary;

			// If there is no index, then there is no information
			if ( numberOfIndexes == 0 ) {
				assert(lateral.information[connection] == false);
				assert(lateral.synchronization[connection] == true);
				proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			}
			// If there is at least one index then, gather this information
			else {
				assert(lateral.information[connection] == true);
				for (const auto& index : lateral.currentIndexes[connection]) {
					proximalInputs.sparseDistributedRepresentation.push_back(SDR_offset+index);
					auxiliary.push_back(SDR_offset+index);
				}
				proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			}
			auto	old_SDR_offset = SDR_offset;
			SDR_offset += std::accumulate(_populationsArrayDimensionality.begin(),
						      _populationsArrayDimensionality.end(),
						      1, std::multiplies<std::size_t>());

			for (const auto& index : auxiliary)
				assert(index >= old_SDR_offset && index < SDR_offset);
		}
	}

	return	proximalInputs;
} // end function gatherProximalInputs


// gathers distal inputs from apical and lateral connections
regularLayerDistalInput	RegularLayer::gatherDistalInputs( const std::size_t column,
							  const regularLayerResponse& apical,
							  const regularLayerResponse& lateral )
{
	regularLayerDistalInput	distalInputs;

	std::size_t	numberOfDistalConnections = 0;

	if ( _apicalConnections.size() != 0 )
		numberOfDistalConnections += _apicalConnections[column].size();

	if ( _lateralDistalConnections.size() != 0 )
		numberOfDistalConnections += _lateralDistalConnections[column].size();

	distalInputs.activeIndexes.resize(numberOfDistalConnections);
	distalInputs.synchronization.resize(numberOfDistalConnections);
	distalInputs.information.resize(numberOfDistalConnections);

	std::size_t	offset = 0;
	if ( _apicalConnections.size() != 0 ) {
		for ( std::size_t apicalConnection = 0; apicalConnection < _apicalConnections[column].size(); apicalConnection++ ) {
			auto	connection = _apicalConnections[column][apicalConnection];
			distalInputs.activeIndexes[offset+apicalConnection] =
			apical.currentIndexes[connection];
			distalInputs.synchronization[offset+apicalConnection] = apical.synchronization[connection];
			distalInputs.information[offset+apicalConnection] = apical.information[connection];
		}
		offset += _apicalConnections[column].size();
	}

	if ( _lateralDistalConnections.size() != 0 ) {
		for ( std::size_t lateralDistalConnection = 0; lateralDistalConnection < _lateralDistalConnections[column].size(); lateralDistalConnection++ ) {
			auto	connection = _lateralDistalConnections[column][lateralDistalConnection];
			distalInputs.activeIndexes[offset+lateralDistalConnection] = lateral.currentIndexes[connection];
			distalInputs.synchronization[offset+lateralDistalConnection] = lateral.synchronization[connection];
			distalInputs.information[offset+lateralDistalConnection] = lateral.information[connection];
		}
	}

	return	distalInputs;
} // end function gatherDistalInputs


// gets the afferent input components that correspond to a column.
std::vector<std::size_t>	RegularLayer::getAfferentInputs( const std::size_t index )
{
	std::size_t	afferentRecptiveFieldCenter, numAfferentInputs;
	std::vector<std::size_t> columnInputs;

	afferentRecptiveFieldCenter = RegularLayer::getReceptiveFieldCenter(index, _afferentArrayDimensionality);
	columnInputs = RegularLayer::getNeighborhood(afferentRecptiveFieldCenter, _afferentWrapAround,
									   _afferentReceptiveField,
									   _afferentArrayDimensionality);

	//Select a subset of the receptive field to serve as the
	//the established connections
	numAfferentInputs = (std::size_t)(columnInputs.size()*_afferentPercentage + 0.5);

	std::vector<std::size_t>	afferentInputs;
        sample_vector(columnInputs, afferentInputs, numAfferentInputs);

	std::sort(afferentInputs.begin(),afferentInputs.end());
	return	afferentInputs;
} // end function getAfferentInputs


// gets the lateral proximal input components that correspond to a column.
std::vector<std::size_t>	RegularLayer::getLateralProximalInputs( const std::size_t index )
{
	std::size_t	numLateralProximalInputs;
	std::vector<std::size_t> columnInputs;

	columnInputs = RegularLayer::getNeighborhood(index, _lateralProximalWrapAround,
							    _lateralProximalReceptiveField,
							    _columnsArrayDimensionality);

	//Select a subset of the receptive field to serve as the
	//the established connections
	numLateralProximalInputs = (std::size_t)(columnInputs.size()*_lateralProximalPercentage + 0.5);

	std::vector<std::size_t>	lateralProximalInputs;
        sample_vector(columnInputs, lateralProximalInputs, numLateralProximalInputs);

	std::sort(lateralProximalInputs.begin(),lateralProximalInputs.end());
	return	lateralProximalInputs;
} // end function getLateralProximalInputs


// gets the lateral distal input components that correspond to a column.
std::vector<std::size_t>	RegularLayer::getLateralDistalInputs( const std::size_t index )
{
	std::size_t	numLateralDistalInputs;
	std::vector<std::size_t> columnInputs;

	columnInputs = RegularLayer::getNeighborhood(index, _lateralDistalWrapAround,
							    _lateralDistalReceptiveField,
							    _columnsArrayDimensionality);

	//Select a subset of the receptive field to serve as the
	//the established connections
	numLateralDistalInputs = (std::size_t)(columnInputs.size()*_lateralDistalPercentage + 0.5);

	std::vector<std::size_t>	lateralDistalInputs;
        sample_vector(columnInputs, lateralDistalInputs, numLateralDistalInputs);

	std::sort(lateralDistalInputs.begin(),lateralDistalInputs.end());
	return	lateralDistalInputs;
} // end function getLateralDistalInputs


// gets the lateral distal input components that correspond to a column.
std::vector<std::size_t>	RegularLayer::getApicalInputs( const std::size_t index )
{
	std::size_t	apicalRecptiveFieldCenter, numApicalInputs;
	std::vector<std::size_t> columnInputs;

	apicalRecptiveFieldCenter = RegularLayer::getReceptiveFieldCenter(index, _apicalArrayDimensionality);
	columnInputs = RegularLayer::getNeighborhood(apicalRecptiveFieldCenter, _apicalWrapAround,
										_apicalReceptiveField,
										_apicalArrayDimensionality);

	//Select a subset of the receptive field to serve as the
	//the established connections
	numApicalInputs = (std::size_t)(columnInputs.size()*_apicalPercentage + 0.5);

	std::vector<std::size_t>	apicalInputs;
        sample_vector(columnInputs, apicalInputs, numApicalInputs);

	std::sort(apicalInputs.begin(),apicalInputs.end());
	return	apicalInputs;
} // end function getApicalInputs


// Gets neighborhood of inputs.
std::vector<std::size_t>	RegularLayer::getNeighborhood( const std::size_t centerInput,
							       const bool wrapAround,
							       const std::vector<std::size_t>& receptiveField,
							       const std::vector<std::size_t>& arrayDimensionality )
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
std::size_t	RegularLayer::getReceptiveFieldCenter( const std::size_t index,
						       const std::vector<std::size_t>& arrayDimensionality )
{
	std::vector<std::size_t>	columnCoords;
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

	std::vector<std::size_t>	inputCoords(ratios.begin(), ratios.end());

	return	ravelIndex(inputCoords, arrayDimensionality);
} // end function getReceptiveFieldCenter


// function that saves the RegularLayer's status in a file
void	RegularLayer::saveRegularLayerStatus( const std::string& folderName, const std::size_t identifyer )
{
	// open a file in write mode.
	ofstream outfile;
	outfile.open("../../Octave/" + folderName + "/RegularLayer_" + std::to_string(identifyer) + ".mat", ios::out | ios::trunc);

	// file preamble.
	outfile << "# This is a file created by saveRegularLayerStatus member function in RegularLayer class from," << endl;
	outfile << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
	outfile << "# Author: Dematties Dario Jesus." << endl;

	outfile << "\n\n" << endl;
	
	// saves afferentArrayDimensionality
	save_vector_as_matrix("afferentArrayDimensionality", _afferentArrayDimensionality, outfile);

	// saves afferentDimensionality
	save_as_scalar("afferentDimensionality", _afferentDimensionality, outfile);

	// saves afferentReceptiveField
	if(_afferentReceptiveField.size() != 0) {
		save_vector_as_matrix("afferentReceptiveField", _afferentReceptiveField, outfile);
	}
	else {
		std::vector<int>	receptiveField(_afferentArrayDimensionality.size(),-1);
		save_vector_as_matrix("afferentReceptiveField", receptiveField, outfile);
	}

	// saves afferentPercentage
	save_as_scalar("afferentPercentage", _afferentPercentage, outfile);

	// saves afferentWrapAround
	save_as_bool("afferentWrapAround", _afferentWrapAround, outfile);



	// saves apicalArrayDimensionality
	save_vector_as_matrix("apicalArrayDimensionality", _apicalArrayDimensionality, outfile);

	// saves apicalDimensionality
	save_as_scalar("apicalDimensionality", _apicalDimensionality, outfile);

	// saves apicalReceptiveField
	if(_apicalReceptiveField.size() != 0) {
		save_vector_as_matrix("apicalReceptiveField", _apicalReceptiveField, outfile);
	}
	else {
		std::vector<int>	receptiveField(_apicalArrayDimensionality.size(),-1);
		save_vector_as_matrix("apicalReceptiveField", receptiveField, outfile);
	}

	// saves apicalPercentage
	save_as_scalar("apicalPercentage", _apicalPercentage, outfile);

	// saves apicalWrapAround
	save_as_bool("apicalWrapAround", _apicalWrapAround, outfile);



	// saves columnsArrayDimensionality
	save_vector_as_matrix("columnsArrayDimensionality", _columnsArrayDimensionality, outfile);

	// saves columnsDimensionality
	save_as_scalar("columnsDimensionality", _columnsDimensionality, outfile);



	// saves lateralProximalReceptiveField
	if(_lateralProximalReceptiveField.size() != 0) {
		save_vector_as_matrix("lateralProximalReceptiveField", _lateralProximalReceptiveField, outfile);
	}
	else {
		std::vector<int>	receptiveField(_columnsArrayDimensionality.size(),-1);
		save_vector_as_matrix("lateralProximalReceptiveField", receptiveField, outfile);
	}

	// saves lateralProximalPercentage
	save_as_scalar("lateralProximalPercentage", _lateralProximalPercentage, outfile);

	// saves lateralProximalWrapAround
	save_as_bool("lateralProximalWrapAround", _lateralProximalWrapAround, outfile);



	// saves lateralDistalReceptiveField
	if(_lateralDistalReceptiveField.size() != 0) {
		save_vector_as_matrix("lateralDistalReceptiveField", _lateralDistalReceptiveField, outfile);
	}
	else {
		std::vector<int>	receptiveField(_columnsArrayDimensionality.size(),-1);
		save_vector_as_matrix("lateralDistalReceptiveField", receptiveField, outfile);
	}

	// saves lateralDistalPercentage
	save_as_scalar("lateralDistalPercentage", _lateralDistalPercentage, outfile);

	// saves lateralDistalWrapAround
	save_as_bool("lateralDistalWrapAround", _lateralDistalWrapAround, outfile);

	if ( _afferentConnections.size() > 0 ) {
		// saves afferentConnections
		twodvector<bool>	afferentConnectionsBitMap;
		afferentConnectionsBitMap.resize(_columnsDimensionality);
		for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
			afferentConnectionsBitMap[column].resize(_afferentDimensionality);
			for ( std::size_t index = 0; index < _afferentConnections[column].size(); index++ )
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
		for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
			lateralProximalConnectionsBitMap[column].resize(_columnsDimensionality);
			for ( std::size_t index = 0; index < _lateralProximalConnections[column].size(); index++ )
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
		for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
			lateralDistalConnectionsBitMap[column].resize(_columnsDimensionality);
			for ( std::size_t index = 0; index < _lateralDistalConnections[column].size(); index++ )
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
		for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
			apicalConnectionsBitMap[column].resize(_apicalDimensionality);
			for ( std::size_t index = 0; index < _apicalConnections[column].size(); index++ )
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

	// saves temporalGatheringAfferentValue
	save_as_scalar("temporalGatheringAfferentValue", _temporalGatheringAfferentValue, outfile);

	// saves potentialPercentage
	save_as_scalar("potentialPercentage", _potentialPercentage, outfile);

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
		std::size_t	aux;
		make_rectangular(_internalTemporallyGatheredInputs.temporallyGatheredIndexes, -1);
		save_multidimensional_vector_as_matrix("internalTemporallyGatheredIndexes",
							_internalTemporallyGatheredInputs.temporallyGatheredIndexes,
							aux, outfile);
	}

	for ( std::size_t column = 0; column < _columnsDimensionality; column++ )
		_layerColumns[column].saveComplexProcessorStatus(std::to_string(column), outfile);

	// close the opened file.
	outfile.close();
} // end function saveRegularLayerStatus


// function that loads the RegularLayer's status in a file
void	RegularLayer::loadRegularLayerStatus( const std::string& folderName, const std::size_t identifyer )
{
	bool	check_afferentArrayDimensionality = false;
	bool	check_afferentDimensionality = false;
	bool	check_afferentReceptiveField = false;
	bool	check_afferentPercentage = false;
	bool	check_afferentWrapAround = false;
	bool	check_apicalArrayDimensionality = false;
	bool	check_apicalDimensionality = false;
	bool	check_apicalReceptiveField = false;
	bool	check_apicalPercentage = false;
	bool	check_apicalWrapAround = false;
	bool	check_columnsArrayDimensionality = false;
	bool	check_columnsDimensionality = false;
	bool	check_lateralProximalReceptiveField = false;
	bool	check_lateralProximalPercentage = false;
	bool	check_lateralProximalWrapAround = false;
	bool	check_lateralDistalReceptiveField = false;
	bool	check_lateralDistalPercentage = false;
	bool	check_lateralDistalWrapAround = false;
	bool	check_afferentConnections = false;
	bool	check_lateralProximalConnections = false;
	bool	check_lateralDistalConnections = false;
	bool	check_apicalConnections = false;
	bool	check_populationsArrayDimensionality = false;
	bool	check_afferentPopulationsArrayDimensionality = false;
	bool	check_apicalPopulationsArrayDimensionality = false;
	bool	check_temporalGatheringAfferentValue = false;
	bool	check_temporalPointer = false;
	bool	check_internalSynchronization = false;
	bool	check_internalTemporallyGatheredInformation = false;
	bool	check_internalTemporallyGatheredIndexes = false;
	bool	check_potentialPercentage = false;

	std::string	str;
	std::string	STR;

	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + folderName + "/RegularLayer_" + std::to_string(identifyer) + ".mat", ios::in | std::ifstream::binary);

	while ( std::getline(infile, str) ) {

		STR = "# name: afferentArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_afferentArrayDimensionality, infile);
			check_afferentArrayDimensionality = true;
		}

		STR = "# name: afferentDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_afferentDimensionality, infile);
			check_afferentDimensionality = true;
		}

		STR = "# name: afferentReceptiveField";
		if ( str.compare(STR) == 0 ) {
			_afferentReceptiveField.clear();
			std::vector<int>	receptiveField;
			load_matrix_to_vector(receptiveField, infile);
			for(const auto& s : receptiveField) {
				if ( s > -1 )
					_afferentReceptiveField.push_back(s);
			}
			check_afferentReceptiveField = true;
		}

		STR = "# name: afferentPercentage";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_afferentPercentage, infile);
			check_afferentPercentage = true;
		}

		STR = "# name: afferentWrapAround";
		if ( str.compare(STR) == 0 ) {
			load_bool(_afferentWrapAround, infile);
			check_afferentWrapAround = true;
		}



		STR = "# name: apicalArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_apicalArrayDimensionality, infile);
			check_apicalArrayDimensionality = true;
		}

		STR = "# name: apicalDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_apicalDimensionality, infile);
			check_apicalDimensionality = true;
		}

		STR = "# name: apicalReceptiveField";
		if ( str.compare(STR) == 0 ) {
			_apicalReceptiveField.clear();
			std::vector<int>	receptiveField;
			load_matrix_to_vector(receptiveField, infile);
			for(const auto& s : receptiveField) {
				if ( s > -1 )
					_apicalReceptiveField.push_back(s);
			}
			check_apicalReceptiveField = true;
		}

		STR = "# name: apicalPercentage";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_apicalPercentage, infile);
			check_apicalPercentage = true;
		}

		STR = "# name: apicalWrapAround";
		if ( str.compare(STR) == 0 ) {
			load_bool(_apicalWrapAround, infile);
			check_apicalWrapAround = true;
		}



		STR = "# name: columnsArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_columnsArrayDimensionality, infile);
			check_columnsArrayDimensionality = true;
		}

		STR = "# name: columnsDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_columnsDimensionality, infile);
			check_columnsDimensionality = true;
		}



		STR = "# name: lateralProximalReceptiveField";
		if ( str.compare(STR) == 0 ) {
			_lateralProximalReceptiveField.clear();
			std::vector<int>	receptiveField;
			load_matrix_to_vector(receptiveField, infile);
			for(const auto& s : receptiveField) {
				if ( s > -1 )
					_lateralProximalReceptiveField.push_back(s);
			}
			check_lateralProximalReceptiveField = true;
		}

		STR = "# name: lateralProximalPercentage";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_lateralProximalPercentage, infile);
			check_lateralProximalPercentage = true;
		}

		STR = "# name: lateralProximalWrapAround";
		if ( str.compare(STR) == 0 ) {
			load_bool(_lateralProximalWrapAround, infile);
			check_lateralProximalWrapAround = true;
		}



		STR = "# name: lateralDistalReceptiveField";
		if ( str.compare(STR) == 0 ) {
			_lateralDistalReceptiveField.clear();
			std::vector<int>	receptiveField;
			load_matrix_to_vector(receptiveField, infile);
			for(const auto& s : receptiveField) {
				if ( s > -1 )
					_lateralDistalReceptiveField.push_back(s);
			}
			check_lateralDistalReceptiveField = true;
		}

		STR = "# name: lateralDistalPercentage";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_lateralDistalPercentage, infile);
			check_lateralDistalPercentage = true;
		}

		STR = "# name: lateralDistalWrapAround";
		if ( str.compare(STR) == 0 ) {
			load_bool(_lateralDistalWrapAround, infile);
			check_lateralDistalWrapAround = true;
		}



		STR = "# name: afferentConnections";
		if ( str.compare(STR) == 0 ) {
			SparseMatrixElements<bool>	sparseAfferentConnectionsBitMap;
			load_sparse_matrix_to_sparse_matrix_elements(sparseAfferentConnectionsBitMap, infile);
			twodvector<bool>	afferentConnectionsBitMap;
			afferentConnectionsBitMap = from_sparse(sparseAfferentConnectionsBitMap);
			_afferentConnections.resize(_columnsDimensionality);
			for ( std::size_t column = 0; column < afferentConnectionsBitMap.size(); column++ ) {
				for ( std::size_t index = 0; index < afferentConnectionsBitMap[column].size(); index++ ) {
					if ( afferentConnectionsBitMap[column][index] == true )
						_afferentConnections[column].push_back(index);

					_afferentConnections[column].shrink_to_fit();
				}
			}
			check_afferentConnections = true;
		}

		STR = "# name: lateralProximalConnections";
		if ( str.compare(STR) == 0 ) {
			SparseMatrixElements<bool>	sparseLateralProximalConnectionsBitMap;
			load_sparse_matrix_to_sparse_matrix_elements(sparseLateralProximalConnectionsBitMap, infile);
			twodvector<bool>	lateralProximalConnectionsBitMap;
			lateralProximalConnectionsBitMap = from_sparse(sparseLateralProximalConnectionsBitMap);
			_lateralProximalConnections.resize(_columnsDimensionality);
			for ( std::size_t column = 0; column < lateralProximalConnectionsBitMap.size(); column++ ) {
				for ( std::size_t index = 0; index < lateralProximalConnectionsBitMap[column].size(); index++ ) {
					if ( lateralProximalConnectionsBitMap[column][index] == true )
						_lateralProximalConnections[column].push_back(index);

					_lateralProximalConnections[column].shrink_to_fit();
				}
			}
			check_lateralProximalConnections = true;
		}

		STR = "# name: lateralDistalConnections";
		if ( str.compare(STR) == 0 ) {
			SparseMatrixElements<bool>	sparseLateralDistalConnectionsBitMap;
			load_sparse_matrix_to_sparse_matrix_elements(sparseLateralDistalConnectionsBitMap, infile);
			twodvector<bool>	lateralDistalConnectionsBitMap;
			lateralDistalConnectionsBitMap = from_sparse(sparseLateralDistalConnectionsBitMap);
			_lateralDistalConnections.resize(_columnsDimensionality);
			for ( std::size_t column = 0; column < lateralDistalConnectionsBitMap.size(); column++ ) {
				for ( std::size_t index = 0; index < lateralDistalConnectionsBitMap[column].size(); index++ ) {
					if ( lateralDistalConnectionsBitMap[column][index] == true )
						_lateralDistalConnections[column].push_back(index);

					_lateralDistalConnections[column].shrink_to_fit();
				}
			}
			check_lateralDistalConnections = true;
		}

		STR = "# name: apicalConnections";
		if ( str.compare(STR) == 0 ) {
			SparseMatrixElements<bool>	sparseApicalConnectionsBitMap;
			load_sparse_matrix_to_sparse_matrix_elements(sparseApicalConnectionsBitMap, infile);
			twodvector<bool>	apicalConnectionsBitMap;
			apicalConnectionsBitMap = from_sparse(sparseApicalConnectionsBitMap);
			_apicalConnections.resize(_columnsDimensionality);
			for ( std::size_t column = 0; column < apicalConnectionsBitMap.size(); column++ ) {
				for ( std::size_t index = 0; index < apicalConnectionsBitMap[column].size(); index++ ) {
					if ( apicalConnectionsBitMap[column][index] == true )
						_apicalConnections[column].push_back(index);

					_apicalConnections[column].shrink_to_fit();
				}
			}
			check_apicalConnections = true;
		}

		STR = "# name: populationsArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_populationsArrayDimensionality, infile);
			check_populationsArrayDimensionality = true;
		}

		STR = "# name: afferentPopulationsArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_afferentPopulationsArrayDimensionality, infile);
			check_afferentPopulationsArrayDimensionality = true;
		}

		STR = "# name: apicalPopulationsArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_apicalPopulationsArrayDimensionality, infile);
			check_apicalPopulationsArrayDimensionality = true;
		}

		STR = "# name: temporalGatheringAfferentValue";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_temporalGatheringAfferentValue, infile);
			check_temporalGatheringAfferentValue = true;
		}

		STR = "# name: temporalPointer";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_temporalPointer, infile);
			check_temporalPointer = true;
		}

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
			check_internalSynchronization = true;
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
			check_internalTemporallyGatheredInformation = true;
		}

		STR = "# name: internalTemporallyGatheredIndexes";
		if ( str.compare(STR) == 0 ) {
			std::size_t	aux;
			load_matrix_to_multidimensional_vector(_internalTemporallyGatheredInputs.temporallyGatheredIndexes,
								aux, infile);

			check_internalTemporallyGatheredIndexes = true;
		}

		STR = "# name: potentialPercentage";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_potentialPercentage, infile);
			check_potentialPercentage = true;
		}

	}
	// close the opened file.
	infile.close();

	assert(check_afferentArrayDimensionality == true);
	assert(check_afferentDimensionality == true);
	assert(check_afferentReceptiveField == true);
	assert(check_afferentPercentage == true);
	assert(check_afferentWrapAround == true);
	assert(check_apicalArrayDimensionality == true);
	assert(check_apicalDimensionality == true);
	assert(check_apicalReceptiveField == true);
	assert(check_apicalPercentage == true);
	assert(check_apicalWrapAround == true);
	assert(check_columnsArrayDimensionality == true);
	assert(check_columnsDimensionality == true);
	assert(check_lateralProximalReceptiveField == true);
	assert(check_lateralProximalPercentage == true);
	assert(check_lateralProximalWrapAround == true);
	assert(check_lateralDistalReceptiveField == true);
	assert(check_lateralDistalPercentage == true);
	assert(check_lateralDistalWrapAround == true);
	assert(check_afferentConnections == true);
	assert(check_lateralProximalConnections == true);
	assert(check_lateralDistalConnections == true);
	assert(check_apicalConnections == true);
	assert(check_populationsArrayDimensionality == true);
	assert(check_afferentPopulationsArrayDimensionality == true);
	assert(check_apicalPopulationsArrayDimensionality == true);
	assert(check_temporalGatheringAfferentValue == true);

	if ( _temporalGatheringAfferentValue > 1 ) {
		assert(check_temporalPointer == true);
		assert(check_internalSynchronization == true);
		assert(check_internalTemporallyGatheredInformation == true);
		assert(check_internalTemporallyGatheredIndexes == true);
		assert(check_potentialPercentage == true);
	}

	for ( std::size_t column = 0; column < _columnsDimensionality; column++ )
		_layerColumns.push_back(ComplexProcessor(folderName + "/RegularLayer_" + std::to_string(identifyer), std::to_string(column)));

	_layerColumns.shrink_to_fit();

	RegularLayer::initializeWiredVariables();
} // end function loadRegularLayerStatus


// this function checks the coeherence between the regular layer structure
// and a structure pass as an argument to the function from outside the object
void	RegularLayer::checkRegularLayerStructure( const regularLayerStructure& structure )
{
	assert(_afferentArrayDimensionality == structure.afferentArrayDimensionality);
	assert(_apicalArrayDimensionality == structure.apicalArrayDimensionality);
	assert(_columnsArrayDimensionality == structure.columnsArrayDimensionality);
	assert(_afferentReceptiveField == structure.afferentReceptiveField);
	assert(_afferentPercentage == structure.afferentPercentage);
	assert(_afferentWrapAround == structure.afferentWrapAround);

	assert(_lateralProximalReceptiveField == structure.lateralProximalReceptiveField);
	assert(_lateralProximalPercentage == structure.lateralProximalPercentage);
	assert(_lateralProximalWrapAround == structure.lateralProximalWrapAround);

	assert(_lateralDistalReceptiveField == structure.lateralDistalReceptiveField);
	assert(_lateralDistalPercentage == structure.lateralDistalPercentage);
	assert(_lateralDistalWrapAround == structure.lateralDistalWrapAround);

	assert(_apicalReceptiveField == structure.apicalReceptiveField);
	assert(_apicalPercentage == structure.apicalPercentage);
	assert(_apicalWrapAround == structure.apicalWrapAround);

	assert(_populationsArrayDimensionality == structure.populationsArrayDimensionality);
	assert(_afferentPopulationsArrayDimensionality == structure.afferentPopulationsArrayDimensionality);
	assert(_apicalPopulationsArrayDimensionality == structure.apicalPopulationsArrayDimensionality);
	assert(_temporalGatheringAfferentValue == structure.temporalGatheringAfferentValue);
	assert(_potentialPercentage == structure.potentialPercentage);
} // end function checkregularLayerStructure



