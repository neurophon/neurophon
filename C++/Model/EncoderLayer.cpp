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

// File Name:		EncoderLayer.cpp
// File Description:	EncoderLayer member-function definitions. This file contains implementations of the
//			member functions prototyped in EncoderLayer.h.

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
#include "EncoderLayer.h"

using namespace std;


// default constructor
EncoderLayer::EncoderLayer() {}

// constructor that initializes EncoderLayer
EncoderLayer::EncoderLayer( const encoderLayerStructure& structure )
{
	EncoderLayer::encoderInitializer(structure);
} // end EncoderLayer constructor


// constructor that initializes an object instantiation of this class
EncoderLayer::EncoderLayer( const std::string& folderName )
{
	EncoderLayer::loadEncoderLayerStatus(folderName);
} // end EncoderLayer constructor


// this function initializes an object instantiation of this class
void	EncoderLayer::encoderInitializer( const encoderLayerStructure& structure )
{
	EncoderLayer::validateEncoderLayer(structure.afferentArrayDimensionality,
					   structure.apicalArrayDimensionality,
					   structure.columnsArrayDimensionality,
					   structure.afferentReceptiveField,
					   structure.afferentPercentage,
					   structure.afferentWrapAround,
					   structure.lateralDistalReceptiveField,
					   structure.lateralDistalPercentage,
					   structure.lateralDistalWrapAround,
					   structure.apicalReceptiveField,
					   structure.apicalPercentage,
					   structure.apicalWrapAround,
					   structure.iterationNum,
		     			   structure.populationsArrayDimensionality,
					   structure.apicalPopulationsArrayDimensionality,
					   structure.potentialPercentage);

	EncoderLayer::interconnectEncoderLayerColumns(structure.afferentArrayDimensionality,
						      structure.apicalArrayDimensionality,
						      structure.columnsArrayDimensionality,
						      structure.afferentReceptiveField,
						      structure.afferentPercentage,
						      structure.afferentWrapAround,
						      structure.lateralDistalReceptiveField,
						      structure.lateralDistalPercentage,
						      structure.lateralDistalWrapAround,
						      structure.apicalReceptiveField,
						      structure.apicalPercentage,
						      structure.apicalWrapAround);

	EncoderLayer::generateColumns(structure.populationsArrayDimensionality,
				      structure.apicalPopulationsArrayDimensionality,
				      structure.potentialPercentage);

	_afferentUpdateStep = 0;

	EncoderLayer::initializeProximalInputLimits();
	EncoderLayer::initializeWiredVariables();
} // end function encoderInitializer


//this function initializes wired variables
void	EncoderLayer::initializeWiredVariables()
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


// validates the encoder
void	EncoderLayer::validateEncoderLayer( const std::vector<std::size_t>& afferentArrayDimensionality,
					    const std::vector<std::size_t>& apicalArrayDimensionality,
					    const std::vector<std::size_t>& columnsArrayDimensionality,
					    const std::vector<std::size_t>& afferentReceptiveField,
					    const double afferentPercentage,
					    const bool afferentWrapAround,
					    const std::vector<std::size_t>& lateralDistalReceptiveField,
					    const double lateralDistalPercentage,
					    const bool lateralDistalWrapAround,
					    const std::vector<std::size_t>& apicalReceptiveField,
					    const double apicalPercentage,
					    const bool apicalWrapAround,
					    std::size_t iterationNum,
					    const std::vector<std::size_t>& populationsArrayDimensionality,
					    const std::vector<std::size_t>& apicalPopulationsArrayDimensionality,
	       				    const double potentialPercentage )
{
	EncoderLayer::validateComumnsInterconnectionsParameters(afferentArrayDimensionality,
								apicalArrayDimensionality,
								columnsArrayDimensionality,
								afferentReceptiveField,
							       	afferentPercentage,
								afferentWrapAround,
								lateralDistalReceptiveField,
							       	lateralDistalPercentage,
								lateralDistalWrapAround,
								apicalReceptiveField,
							       	apicalPercentage,
								apicalWrapAround);

	EncoderLayer::validatePopulationParameters(populationsArrayDimensionality,
						   apicalPopulationsArrayDimensionality,
						   potentialPercentage);

} // end function validateEncoderLayer


// validates the parameters to configure the columns' interconnections
void	EncoderLayer::validateComumnsInterconnectionsParameters( const std::vector<std::size_t>& afferentArrayDimensionality,
								 const std::vector<std::size_t>& apicalArrayDimensionality,
								 const std::vector<std::size_t>& columnsArrayDimensionality,
								 const std::vector<std::size_t>& afferentReceptiveField,
								 const double afferentPercentage,
								 const bool afferentWrapAround,
								 const std::vector<std::size_t>& lateralDistalReceptiveField,
								 const double lateralDistalPercentage,
								 const bool lateralDistalWrapAround,
								 const std::vector<std::size_t>& apicalReceptiveField,
								 const double apicalPercentage,
								 const bool apicalWrapAround )
{
	if ( afferentArrayDimensionality.size() == 0 ||
	     apicalArrayDimensionality.size() == 0 ||
	     columnsArrayDimensionality.size() == 0 ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";
		std::cout << "afferentArrayDimensionality.size() = " << afferentArrayDimensionality.size() << "\n";
		std::cout << "apicalArrayDimensionality.size() = " << apicalArrayDimensionality.size() << "\n";
		std::cout << "columnsArrayDimensionality.size() = " << columnsArrayDimensionality.size() << "\n";
		exit( EXIT_FAILURE );
	}

	for ( std::size_t dim = 0; dim < afferentArrayDimensionality.size(); dim++ ) {
		if ( afferentArrayDimensionality[dim] == 0 ) {
			std::cout << "EncoderLayer object construction inconsistence: \n";
			std::cout << "In function validateComumnsInterconnectionsParameters\n";
			std::cout << "afferentArrayDimensionality[" << dim << "] = " << afferentArrayDimensionality[dim] << "\n";
			exit( EXIT_FAILURE );
		}
	}

	for ( std::size_t dim = 0; dim < apicalArrayDimensionality.size(); dim++ ) {
		if ( apicalArrayDimensionality[dim] == 0 ) {
			std::cout << "EncoderLayer object construction inconsistence: \n";
			std::cout << "In function validateComumnsInterconnectionsParameters\n";
			std::cout << "apicalArrayDimensionality[" << dim << "] = " << apicalArrayDimensionality[dim] << "\n";
			exit( EXIT_FAILURE );
		}
	}

	for ( std::size_t dim = 0; dim < columnsArrayDimensionality.size(); dim++ ) {
		if ( columnsArrayDimensionality[dim] == 0 ) {
			std::cout << "EncoderLayer object construction inconsistence: \n";
			std::cout << "In function validateComumnsInterconnectionsParameters\n";
			std::cout << "columnsArrayDimensionality[" << dim << "] = " << columnsArrayDimensionality[dim] << "\n";
			exit( EXIT_FAILURE );
		}
	}

	if ( afferentReceptiveField.size() == 0 ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";

		std::cout << "afferentReceptiveField.size() = " << afferentReceptiveField.size() << "\n";
		exit( EXIT_FAILURE );
	}

	if ( (afferentArrayDimensionality.size() != afferentReceptiveField.size()) ||
	     (apicalReceptiveField.size() != 0 && apicalArrayDimensionality.size() != apicalReceptiveField.size()) ||
	     (lateralDistalReceptiveField.size() != 0 && columnsArrayDimensionality.size() != lateralDistalReceptiveField.size()) ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";

		std::cout << "afferentArrayDimensionality.size() = " << afferentArrayDimensionality.size() << "\n";
		std::cout << "while, afferentReceptiveField.size() = " << afferentReceptiveField.size() << "\n";

		std::cout << "apicalArrayDimensionality.size() = " << apicalArrayDimensionality.size() << "\n";
		std::cout << "while, apicalReceptiveField.size() = " << apicalReceptiveField.size() << "\n";

		std::cout << "columnsArrayDimensionality.size() = " << columnsArrayDimensionality.size() << "\n";
		std::cout << "while, lateralDistalReceptiveField.size() = " << lateralDistalReceptiveField.size() << "\n";
		exit( EXIT_FAILURE );
	}

	for ( std::size_t dim = 0; dim < afferentReceptiveField.size(); dim++ ) {
		if (afferentArrayDimensionality[dim] % 2) {
			if ( afferentReceptiveField[dim] > afferentArrayDimensionality[dim]/2 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "afferentReceptiveField[" << dim << "] = " << afferentReceptiveField[dim] << "\n";
				std::cout << "while afferentArrayDimensionality[" << dim << "] is = "
				          << afferentArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
		else {
			if ( afferentReceptiveField[dim] > afferentArrayDimensionality[dim]/2-1 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
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
				std::cout << "EncoderLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "apicalReceptiveField[" << dim << "] = " << apicalReceptiveField[dim] << "\n";
				std::cout << "while apicalArrayDimensionality[" << dim << "] is = "
				          << apicalArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
		else {
			if ( apicalReceptiveField[dim] > apicalArrayDimensionality[dim]/2-1 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "apicalReceptiveField[" << dim << "] = " << apicalReceptiveField[dim] << "\n";
				std::cout << "while apicalArrayDimensionality[" << dim << "] is = "
				          << apicalArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	for ( std::size_t dim = 0; dim < lateralDistalReceptiveField.size(); dim++ ) {
		if (columnsArrayDimensionality[dim] % 2) {
			if ( lateralDistalReceptiveField[dim] > columnsArrayDimensionality[dim]/2 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "lateralDistalReceptiveField[" << dim << "] = " << lateralDistalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
		else {
			if ( lateralDistalReceptiveField[dim] > columnsArrayDimensionality[dim]/2-1 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
				std::cout << "In function validateComumnsInterconnectionsParameters\n";
				std::cout << "lateralDistalReceptiveField[" << dim << "] = " << lateralDistalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				exit( EXIT_FAILURE );
			}
		}
	}

	if ( afferentPercentage < 0.0 || afferentPercentage > 1.0 ||
	     lateralDistalPercentage < 0.0 || lateralDistalPercentage > 1.0 ||
	     apicalPercentage < 0.0 || apicalPercentage > 1.0 ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validateComumnsInterconnectionsParameters\n";
		std::cout << "afferentPercentage = " << afferentPercentage << "\n";
		std::cout << "lateralDistalPercentage = " << lateralDistalPercentage << "\n";
		std::cout << "apicalPercentage = " << apicalPercentage << "\n";
		exit( EXIT_FAILURE );
	}
} // end function validateComumnsInterconnectionsParameters


// validates the parameters used to generate the columns' populations
void	EncoderLayer::validatePopulationParameters( const std::vector<std::size_t>& populationsArrayDimensionality,
						    const std::vector<std::size_t>& apicalPopulationsArrayDimensionality,
	       					    const double potentialPercentage )
{
	if ( potentialPercentage < 0.0 || potentialPercentage > 1.0 ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "potentialPercentage = "
			  << potentialPercentage << "\n";
		exit( EXIT_FAILURE );
	}

	if ( apicalPopulationsArrayDimensionality.size() == 0 ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "apicalPopulationsArrayDimensionality.size() = "
			  << apicalPopulationsArrayDimensionality.size() << "\n";
		exit( EXIT_FAILURE );
	}
	else {
		for(const auto& s: apicalPopulationsArrayDimensionality) {
			if ( s == 0 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
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
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "populationsArrayDimensionality.size() = "
			  << populationsArrayDimensionality.size() << "\n";
		exit( EXIT_FAILURE );
	}
	else {
		for(const auto& s: populationsArrayDimensionality) {
			if ( s == 0 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
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
void	EncoderLayer::interconnectEncoderLayerColumns( const std::vector<std::size_t>& afferentArrayDimensionality,
						       const std::vector<std::size_t>& apicalArrayDimensionality,
						       const std::vector<std::size_t>& columnsArrayDimensionality,
						       const std::vector<std::size_t>& afferentReceptiveField,
						       const double afferentPercentage,
						       const bool afferentWrapAround,
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
		_afferentConnections[column] = EncoderLayer::getAfferentInputs(column);

	_lateralDistalReceptiveField = lateralDistalReceptiveField;
	_lateralDistalPercentage = lateralDistalPercentage;
	_lateralDistalWrapAround = lateralDistalWrapAround;

	if ( lateralDistalReceptiveField.size() != 0 ) {
		_lateralDistalConnections.resize(_columnsDimensionality);
		for ( std::size_t column = 0; column < _columnsDimensionality; column++ )
			_lateralDistalConnections[column] = EncoderLayer::getLateralDistalInputs(column);
	}

	_apicalReceptiveField = apicalReceptiveField;
	_apicalPercentage = apicalPercentage;
	_apicalWrapAround = apicalWrapAround;

	if ( apicalReceptiveField.size() != 0 ) {
		_apicalConnections.resize(_columnsDimensionality);
		for ( std::size_t column = 0; column < _columnsDimensionality; column++ )
			_apicalConnections[column] = EncoderLayer::getApicalInputs(column);
	}
} // end function interconnectEncoderLayerColumns


// generates a population for every column in the encoder
void	EncoderLayer::generateColumns( const std::vector<std::size_t>& populationsArrayDimensionality,
				       const std::vector<std::size_t>& apicalPopulationsArrayDimensionality,
	       			       const double potentialPercentage )
{
	std::size_t			numberOfInputs;
	std::vector<std::size_t>	dynamicUnits;

	assert(_afferentConnections.size() != 0);
	for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
		numberOfInputs = 0;

		numberOfInputs += _afferentConnections[column].size();

		if ( _apicalConnections.size() != 0 )
			for ( std::size_t link = 0; link < _apicalConnections[column].size(); link++ ) {
				auto	auxiliary = std::accumulate(apicalPopulationsArrayDimensionality.begin(),
								    apicalPopulationsArrayDimensionality.end(),
								    1, std::multiplies<std::size_t>());
				dynamicUnits.push_back(auxiliary);
			}

		if ( _lateralDistalConnections.size() != 0 )
			for ( std::size_t link = 0; link < _lateralDistalConnections[column].size(); link++ ) {
				auto	auxiliary = std::accumulate(populationsArrayDimensionality.begin(),
								    populationsArrayDimensionality.end(),
								    1, std::multiplies<std::size_t>());
				dynamicUnits.push_back(auxiliary);
			}

		dynamicUnits.shrink_to_fit();
		_encoderColumns.push_back(ComplexSelfOrganizingMap(populationsArrayDimensionality,
								   numberOfInputs,
								   potentialPercentage,
								   dynamicUnits));
		dynamicUnits.clear();
	}
	_encoderColumns.shrink_to_fit();

	_populationsArrayDimensionality = populationsArrayDimensionality;
	_apicalPopulationsArrayDimensionality = apicalPopulationsArrayDimensionality;
	_potentialPercentage = potentialPercentage;
} // end function generateColumns


// initializes the proximal input limits from afferent and lateral connections
void	EncoderLayer::initializeProximalInputLimits()
{
	assert(_afferentConnections.size() != 0);
       
	_proximalAfferentLimits.resize(_afferentDimensionality);
	for ( std::size_t afferentInput = 0; afferentInput < _afferentDimensionality; afferentInput++ ) {
		_proximalAfferentLimits[afferentInput][0] = 0.0;
		_proximalAfferentLimits[afferentInput][1] = 1.0;
	}
} // end function initializeProximalInputLimits


// computes the encoder response
regularLayerResponse	EncoderLayer::computeResponse( const encoderLayerInput& afferent,
						       const encoderLayerParameters& parameters )
{
	assert( parameters.proximalInformationThreshold >= 0.0 && parameters.proximalInformationThreshold <= 1.0 );
	assert( parameters.distalInformationThreshold >= 0.0 && parameters.distalInformationThreshold <= 1.0 );

	regularLayerResponse	output;

	output.currentIndexes.resize(_columnsDimensionality);
	output.synchronization.resize(_columnsDimensionality);
	output.information.resize(_columnsDimensionality);

	#pragma omp parallel for default(none) shared(afferent, parameters, output)
	for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
		auto	proximalInputs = EncoderLayer::gatherProximalInputs(column, afferent);
		auto	distalInputs = EncoderLayer::gatherDistalInputs(column, _apical, _lateral);

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
				auto	response = _encoderColumns[column].getResponse(proximalInputs.inputs);

				//auto	activeIndexes = _encoderColumns[column].Activate(response,
											 //distalInputs.activeIndexes,
											 //parameters.activationRadius,
											 //parameters.sparsity);
			
				std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
								       std::accumulate(_populationsArrayDimensionality.begin(),
										       _populationsArrayDimensionality.end(),
										       1, std::multiplies<std::size_t>());	

				std::vector<std::size_t>	activeIndexes;
				if ( parameters.enableLearning && parameters.learning.enableDistalLearning ) {
					activeIndexes = _encoderColumns[column].Activate(response,
											 distalInputs.activeIndexes,
											 numberOfExcitedUnits,
											 parameters.sparsity,
											 true);
				}
				else {
					activeIndexes = _encoderColumns[column].Activate(response,
											 distalInputs.activeIndexes,
											 numberOfExcitedUnits,
											 parameters.sparsity);
				}

				if ( parameters.enableLearning ) {	// If learning is enabled
					if (parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0)
						throw std::runtime_error (
						"parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0");

					if ( parameters.learning.enableProximalLearning ) {
						_encoderColumns[column].learningRule(parameters.learning.proximalLearningRate,
										     parameters.learning.proximalNeighborhood,
										     response.ranking[0],
										     proximalInputs.inputs);
					}

					if ( parameters.learning.enableDistalLearning ) {
						// if the massive activation is greater than certain threshold,
						// it is considered as a prediction fault, then special distal
						// learning considerations are taken
						if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
						  			    std::accumulate(_populationsArrayDimensionality.begin(),
											    _populationsArrayDimensionality.end(), 1,
											    std::multiplies<std::size_t>()) ) {
							_encoderColumns[column].Update(activeIndexes,
										       distalInputs.activeIndexes,
										       true,
										       DISTAL_SYNAPTIC_THRESHOLD,
										       parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_encoderColumns[column].Update(activeIndexes,
										       distalInputs.activeIndexes,
										       true,
										       DISTAL_SYNAPTIC_THRESHOLD,
										       parameters.learning.distalLearningRate);
						}

						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveIndexes = _lateral.currentIndexes[column]; 
							_encoderColumns[column].Update(lastActiveIndexes,
										       distalInputs.activeIndexes,
										       false,
										       DISTAL_SYNAPTIC_THRESHOLD,
										       parameters.learning.distalLearningRate);
						}
					}
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

	if ( parameters.enableLearning )	// If learning is enabled, then it calls updateProximalInputLimits member function
		EncoderLayer::updateProximalInputLimits(afferent, parameters.learning);

	return	output;
} // end function computeResponse


// computes the encoder response
regularLayerResponse	EncoderLayer::computeResponse( const encoderLayerInput& afferent,
						       const regularLayerResponse& lateral,
						       const encoderLayerParameters& parameters )
{
	assert( parameters.proximalInformationThreshold >= 0.0 && parameters.proximalInformationThreshold <= 1.0 );
	assert( parameters.distalInformationThreshold >= 0.0 && parameters.distalInformationThreshold <= 1.0 );

	regularLayerResponse	output;

	output.currentIndexes.resize(_columnsDimensionality);
	output.synchronization.resize(_columnsDimensionality);
	output.information.resize(_columnsDimensionality);

	#pragma omp parallel for default(none) shared(afferent, lateral, parameters, output)
	for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
		auto	proximalInputs = EncoderLayer::gatherProximalInputs(column, afferent);
		auto	distalInputs = EncoderLayer::gatherDistalInputs(column, _apical, lateral);

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
				auto	response = _encoderColumns[column].getResponse(proximalInputs.inputs);

				auto	activeIndexes = _encoderColumns[column].Activate(response,
											 distalInputs.activeIndexes,
											 parameters.activationRadius,
											 parameters.sparsity);
			

				if ( parameters.enableLearning ) {	// If learning is enabled
					if (parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0)
						throw std::runtime_error (
						"parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0");

					if ( parameters.learning.enableProximalLearning ) {
						_encoderColumns[column].learningRule(parameters.learning.proximalLearningRate,
										     parameters.learning.proximalNeighborhood,
										     response.ranking[0],
										     proximalInputs.inputs);
					}

					if ( parameters.learning.enableDistalLearning ) {
						// if the massive activation is greater than certain threshold,
						// it is considered as a prediction fault, then special distal
						// learning considerations are taken
						if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
						  			    std::accumulate(_populationsArrayDimensionality.begin(),
											    _populationsArrayDimensionality.end(), 1,
											    std::multiplies<std::size_t>()) ) {
							_encoderColumns[column].Update(activeIndexes,
										       distalInputs.activeIndexes,
										       true,
										       DISTAL_SYNAPTIC_THRESHOLD,
										       parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_encoderColumns[column].Update(activeIndexes,
										       distalInputs.activeIndexes,
										       true,
										       DISTAL_SYNAPTIC_THRESHOLD,
										       parameters.learning.distalLearningRate);
						}

						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveIndexes = lateral.currentIndexes[column]; 
							_encoderColumns[column].Update(lastActiveIndexes,
										       distalInputs.activeIndexes,
										       false,
										       DISTAL_SYNAPTIC_THRESHOLD,
										       parameters.learning.distalLearningRate);
						}
					}
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

	if ( parameters.enableLearning )	// If learning is enabled, then it calls updateProximalInputLimits member function
		EncoderLayer::updateProximalInputLimits(afferent, parameters.learning);

	return	output;
} // end function computeResponse


// computes the encoder response
regularLayerResponse	EncoderLayer::computeResponse( const encoderLayerInput& afferent,
						       const regularLayerResponse& lateral,
						       const regularLayerResponse& apical,
						       const encoderLayerParameters& parameters )
{
	assert( parameters.proximalInformationThreshold >= 0.0 && parameters.proximalInformationThreshold <= 1.0 );
	assert( parameters.distalInformationThreshold >= 0.0 && parameters.distalInformationThreshold <= 1.0 );

	regularLayerResponse	output;

	output.currentIndexes.resize(_columnsDimensionality);
	output.synchronization.resize(_columnsDimensionality);
	output.information.resize(_columnsDimensionality);

	#pragma omp parallel for default(none) shared(afferent, lateral, apical, parameters, output)
	for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
		auto	proximalInputs = EncoderLayer::gatherProximalInputs(column, afferent);
		auto	distalInputs = EncoderLayer::gatherDistalInputs(column, apical, lateral);

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
				auto	response = _encoderColumns[column].getResponse(proximalInputs.inputs);

				auto	activeIndexes = _encoderColumns[column].Activate(response,
											 distalInputs.activeIndexes,
											 parameters.activationRadius,
											 parameters.sparsity);
			

				if ( parameters.enableLearning ) {	// If learning is enabled
					if (parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0)
						throw std::runtime_error (
						"parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0");

					if ( parameters.learning.enableProximalLearning ) {
						_encoderColumns[column].learningRule(parameters.learning.proximalLearningRate,
										     parameters.learning.proximalNeighborhood,
										     response.ranking[0],
										     proximalInputs.inputs);
					}

					if ( parameters.learning.enableDistalLearning ) {
						// if the massive activation is greater than certain threshold,
						// it is considered as a prediction fault, then special distal
						// learning considerations are taken
						if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
						  			    std::accumulate(_populationsArrayDimensionality.begin(),
											    _populationsArrayDimensionality.end(), 1,
											    std::multiplies<std::size_t>()) ) {
							_encoderColumns[column].Update(activeIndexes,
										       distalInputs.activeIndexes,
										       true,
										       DISTAL_SYNAPTIC_THRESHOLD,
										       parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_encoderColumns[column].Update(activeIndexes,
										       distalInputs.activeIndexes,
										       true,
										       DISTAL_SYNAPTIC_THRESHOLD,
										       parameters.learning.distalLearningRate);
						}

						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveIndexes = lateral.currentIndexes[column]; 
							_encoderColumns[column].Update(lastActiveIndexes,
										       distalInputs.activeIndexes,
										       false,
										       DISTAL_SYNAPTIC_THRESHOLD,
										       parameters.learning.distalLearningRate);
						}
					}
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

	if ( parameters.enableLearning )	// If learning is enabled, then it calls updateProximalInputLimits member function
		EncoderLayer::updateProximalInputLimits(afferent, parameters.learning);

	return	output;
} // end function computeResponse


// gathers proximal inputs from afferent and lateral connections
encoderLayerProximalInput	EncoderLayer::gatherProximalInputs( const std::size_t column,
								    const encoderLayerInput& afferent )
{
	encoderLayerProximalInput	proximalInputs;

	assert(afferent.inputs.size() != 0);
	assert(_afferentConnections.size() != 0);
	std::size_t	numberOfProximalConnections = _afferentConnections[column].size();

	proximalInputs.inputs.resize(numberOfProximalConnections);
	proximalInputs.synchronization.resize(numberOfProximalConnections);
	proximalInputs.information.resize(numberOfProximalConnections);

	for ( std::size_t afferentConnection = 0; afferentConnection < numberOfProximalConnections; afferentConnection++ ) {
		auto	connection = _afferentConnections[column][afferentConnection];
		if ( afferent.information[connection] ) {
			proximalInputs.inputs[afferentConnection] = afferent.inputs[connection];
			proximalInputs.synchronization[afferentConnection] = afferent.synchronization[connection];
			proximalInputs.information[afferentConnection] = afferent.information[connection];
		}
		else {
			proximalInputs.inputs[afferentConnection] =
			randomFromDoubleInterval(_proximalAfferentLimits[connection][0], _proximalAfferentLimits[connection][1]);
			proximalInputs.synchronization[afferentConnection] = afferent.synchronization[connection];
			proximalInputs.information[afferentConnection] = afferent.information[connection];
		}
	}

	return	proximalInputs;
} // end function gatherProximalInputs


// gathers distal inputs from apical and lateral connections
encoderLayerDistalInput	EncoderLayer::gatherDistalInputs( const std::size_t column,
							  const regularLayerResponse& apical,
							  const regularLayerResponse& lateral )
{
       /* std::cout << "\n_apicalConnections:"  << _apicalConnections.size() << "\n";*/
	//std::cout << "\n_lateralDistalConnections:"  << _lateralDistalConnections.size() << "\n";

	//std::cout << "\napical.currentIndexes.size():"  << apical.currentIndexes.size() << "\n";
	//std::cout << "\napical.synchronization.size():"  << apical.synchronization.size() << "\n";
	//std::cout << "\napical.information.size():"  << apical.information.size() << "\n";

	//std::cout << "\nlateral.currentIndexes.size():"  << lateral.currentIndexes.size() << "\n";
	//std::cout << "\nlateral.synchronization.size():"  << lateral.synchronization.size() << "\n";
	//std::cout << "\nlateral.information.size():"  << lateral.information.size() << "\n";

	encoderLayerDistalInput	distalInputs;

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


// updates the proximal input limits from afferent connections
void	EncoderLayer::updateProximalInputLimits( const encoderLayerInput& afferent,
						 const encoderLayerLearningParameters& parameters )
{
	assert(_afferentConnections.size() != 0);

	if ( parameters.limitsLearningRate < 0.0 && parameters.limitsLearningRate > 1.0 ) {
		std::cout << "EncoderLayer object inconsistence: \n";
		std::cout << "In function updateProximalInputLimits\n";
		std::cout << "limitsLearningRate must be: 0 <= limitsLearningRate <= 1.\n";
		std::cout << "Yet, limitsLearningRate = " << parameters.limitsLearningRate << "\n";
		exit( EXIT_FAILURE );
	}

	if ( _afferentUpdateStep > UPDATE_PERIOD ) {
		for ( std::size_t afferentInput = 0; afferentInput < _afferentDimensionality; afferentInput++ ) {
			if ( afferent.information[afferentInput] ) { // updates just in the case of true information from the afferent input
				auto	value = afferent.inputs[afferentInput];

				if ( value >= _proximalAfferentLimits[afferentInput][0] &&
				     value <= _proximalAfferentLimits[afferentInput][1] ) { // is it between the limits?
					_proximalAfferentLimits[afferentInput][0] += parameters.limitsLearningRate*
					(value - _proximalAfferentLimits[afferentInput][0]);
					_proximalAfferentLimits[afferentInput][1] -= parameters.limitsLearningRate*
					(_proximalAfferentLimits[afferentInput][1] - value);
				}
				else if ( value < _proximalAfferentLimits[afferentInput][0] ) { // is it below the lower limit?
					_proximalAfferentLimits[afferentInput][0] -= parameters.limitsLearningRate*
					(_proximalAfferentLimits[afferentInput][0] - value);
				}
				else { // then it is above the upper limit
					_proximalAfferentLimits[afferentInput][1] += parameters.limitsLearningRate*
					(value - _proximalAfferentLimits[afferentInput][1]);
				}
				assert(_proximalAfferentLimits[afferentInput][0] <
				       _proximalAfferentLimits[afferentInput][1]);
			}
		}
		_afferentUpdateStep = 0;
	}
	else {
		_afferentUpdateStep++;
	}
} // end function updateProximalInputLimits


// gets the afferent input components that correspond to a column.
std::vector<std::size_t>	EncoderLayer::getAfferentInputs( const std::size_t index )
{
	std::size_t	afferentRecptiveFieldCenter, numAfferentInputs;
	std::vector<std::size_t> columnInputs;

	afferentRecptiveFieldCenter = EncoderLayer::getReceptiveFieldCenter(index, _afferentArrayDimensionality);
	columnInputs = EncoderLayer::getNeighborhood(afferentRecptiveFieldCenter, _afferentWrapAround,
									   _afferentReceptiveField,
									   _afferentArrayDimensionality);

	//Select a subset of the receptive field to serve as the
	//the established connections
	numAfferentInputs = (columnInputs.size()*_afferentPercentage + 0.5);

	std::vector<std::size_t>	afferentInputs;
	sample_vector(columnInputs, afferentInputs, numAfferentInputs);
	std::sort(afferentInputs.begin(),afferentInputs.end());
	return	afferentInputs;
} // end function getAfferentInputs


// gets the lateral distal input components that correspond to a column.
std::vector<std::size_t>	EncoderLayer::getLateralDistalInputs( const std::size_t index )
{
	std::size_t	numLateralDistalInputs;
	std::vector<std::size_t> columnInputs;

	columnInputs = EncoderLayer::getNeighborhood(index, _lateralDistalWrapAround,
						     _lateralDistalReceptiveField,
						     _columnsArrayDimensionality);

	//Select a subset of the receptive field to serve as the
	//the established connections
	numLateralDistalInputs = (columnInputs.size()*_lateralDistalPercentage + 0.5);

	std::vector<std::size_t>	lateralDistalInputs;
	sample_vector(columnInputs, lateralDistalInputs, numLateralDistalInputs);
	std::sort(lateralDistalInputs.begin(),lateralDistalInputs.end());
	return	lateralDistalInputs;
} // end function getLateralDistalInputs


// gets the lateral distal input components that correspond to a column.
std::vector<std::size_t>	EncoderLayer::getApicalInputs( const std::size_t index )
{
	std::size_t	apicalRecptiveFieldCenter, numApicalInputs;
	std::vector<std::size_t> columnInputs;

	apicalRecptiveFieldCenter = EncoderLayer::getReceptiveFieldCenter(index, _apicalArrayDimensionality);
	columnInputs = EncoderLayer::getNeighborhood(apicalRecptiveFieldCenter, _apicalWrapAround,
									 _apicalReceptiveField,
									 _apicalArrayDimensionality);

	//Select a subset of the receptive field to serve as the
	//the established connections
	numApicalInputs = (columnInputs.size()*_apicalPercentage + 0.5);

	std::vector<std::size_t>	apicalInputs;
	sample_vector(columnInputs, apicalInputs, numApicalInputs);
	std::sort(apicalInputs.begin(),apicalInputs.end());
	return	apicalInputs;
} // end function getApicalInputs


// Gets neighborhood of inputs.
std::vector<std::size_t>	EncoderLayer::getNeighborhood( const std::size_t centerInput,
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
std::size_t	EncoderLayer::getReceptiveFieldCenter( const std::size_t index,
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


// function that saves the EncoderLayer's status in a file
void	EncoderLayer::saveEncoderLayerStatus( const std::string& folderName )
{
	// open a file in write mode.
	ofstream outfile;
	outfile.open("../../Octave/" + folderName + "/EncoderLayer.mat", ios::out | ios::trunc);

	// file preamble.
	outfile << "# This is a file created by saveEncoderLayerStatus member function in EncoderLayer class from," << endl;
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

	if ( _afferentConnections.size() != 0 ) {
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

	if ( _lateralDistalConnections.size() != 0 ) {
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

	if ( _apicalConnections.size() != 0 ) {
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

	// saves apicalPopulationsArrayDimensionality
	save_vector_as_matrix("apicalPopulationsArrayDimensionality", _apicalPopulationsArrayDimensionality, outfile);

	if ( _afferentConnections.size() != 0 ) {
		std::vector<double>	lowerAfferentLimits, upperAfferentLimits;
		auto	numberOfLimits = _proximalAfferentLimits.size();
		lowerAfferentLimits.resize(numberOfLimits);
		upperAfferentLimits.resize(numberOfLimits);
		for ( std::size_t limit = 0; limit < numberOfLimits; limit++ ) {
				lowerAfferentLimits[limit] =
				_proximalAfferentLimits[limit][0];
				upperAfferentLimits[limit] =
				_proximalAfferentLimits[limit][1];
		}
		// saves proximalAfferentLowerLimits
		save_vector_as_matrix("proximalAfferentLowerLimits", lowerAfferentLimits, outfile);

		// saves proximalAfferentUpperLimits
		save_vector_as_matrix("proximalAfferentUpperLimits", upperAfferentLimits, outfile);
	}

	if ( _afferentConnections.size() != 0 ) {
		// saves afferentUpdateStep
		save_as_scalar("afferentUpdateStep", _afferentUpdateStep, outfile);
	}

	// saves potentialPercentage
	save_as_scalar("potentialPercentage", _potentialPercentage, outfile);

	for ( std::size_t column = 0; column < _columnsDimensionality; column++ )
		_encoderColumns[column].saveComplexSelfOrganizingMapStatus(std::to_string(column), outfile);

	// close the opened file.
	outfile.close();
} // end function saveEncoderLayerStatus


// function that loads the EncoderLayer's status in a file
void	EncoderLayer::loadEncoderLayerStatus( const std::string& folderName )
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
	bool	check_lateralDistalReceptiveField = false;
	bool	check_lateralDistalPercentage = false;
	bool	check_lateralDistalWrapAround = false;
	bool	check_afferentConnections = false;
	bool	check_lateralDistalConnections = false;
	bool	check_apicalConnections = false;
	bool	check_populationsArrayDimensionality = false;
	bool	check_apicalPopulationsArrayDimensionality = false;
	bool	check_proximalAfferentLowerLimits = false;
	bool	check_proximalAfferentUpperLimits = false;
	bool	check_afferentUpdateStep = false;
	bool	check_potentialPercentage = false;

	std::string	str;
	std::string	STR;

	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + folderName + "/EncoderLayer.mat", ios::in | std::ifstream::binary);

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
			check_afferentConnections = true;
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

		}

		STR = "# name: lateralDistalConnections";
		if ( str.compare(STR) == 0 ) {
			check_lateralDistalConnections = true;
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

		}

		STR = "# name: apicalConnections";
		if ( str.compare(STR) == 0 ) {
			check_apicalConnections = true;
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
		}

		STR = "# name: populationsArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_populationsArrayDimensionality, infile);
			check_populationsArrayDimensionality = true;
		}

		STR = "# name: apicalPopulationsArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_apicalPopulationsArrayDimensionality, infile);
			check_apicalPopulationsArrayDimensionality = true;
		}



		STR = "# name: proximalAfferentLowerLimits";
		if ( str.compare(STR) == 0 ) {
			check_proximalAfferentLowerLimits = true;
			std::vector<double>	lowerLimits;
			load_matrix_to_vector(lowerLimits, infile);
			auto	numberOfLimits = lowerLimits.size();
			_proximalAfferentLimits.resize(numberOfLimits);
			for ( std::size_t limit = 0; limit < numberOfLimits; limit++ )
				_proximalAfferentLimits[limit][0] = lowerLimits[limit];
		}

		STR = "# name: proximalAfferentUpperLimits";
		if ( str.compare(STR) == 0 ) {
			check_proximalAfferentUpperLimits = true;
			std::vector<double>	upperLimits;
			load_matrix_to_vector(upperLimits, infile);
			auto	numberOfLimits = upperLimits.size();
			_proximalAfferentLimits.resize(numberOfLimits);
			for ( std::size_t limit = 0; limit < numberOfLimits; limit++ )
				_proximalAfferentLimits[limit][1] = upperLimits[limit];
		}

		STR = "# name: afferentUpdateStep";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_afferentUpdateStep, infile);
			check_afferentUpdateStep = true;
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
	assert(check_lateralDistalReceptiveField == true);
	assert(check_lateralDistalPercentage == true);
	assert(check_lateralDistalWrapAround == true);
	assert(check_afferentConnections == true);
	assert(check_lateralDistalConnections == true);
	assert(check_apicalConnections == true);
	assert(check_populationsArrayDimensionality == true);
	assert(check_apicalPopulationsArrayDimensionality == true);
	assert(check_proximalAfferentLowerLimits == true);
	assert(check_proximalAfferentUpperLimits == true);
	assert(check_afferentUpdateStep == true);
	assert(check_potentialPercentage == true);

	for ( std::size_t column = 0; column < _columnsDimensionality; column++ )
		_encoderColumns.push_back(ComplexSelfOrganizingMap(folderName + "/EncoderLayer", std::to_string(column)));

	_encoderColumns.shrink_to_fit();

	EncoderLayer::initializeWiredVariables();
} // end function loadEncoderLayerStatus


// this function checks the coeherence between the encoder layer structure
// and a structure pass as an argument to the function from outside the object
void	EncoderLayer::checkEncoderLayerStructure( const encoderLayerStructure& structure )
{
	assert(_afferentArrayDimensionality == structure.afferentArrayDimensionality);
	assert(_apicalArrayDimensionality == structure.apicalArrayDimensionality);
	assert(_columnsArrayDimensionality == structure.columnsArrayDimensionality);
	assert(_afferentReceptiveField == structure.afferentReceptiveField);
	assert(_afferentPercentage == structure.afferentPercentage);
	assert(_afferentWrapAround == structure.afferentWrapAround);

	assert(_lateralDistalReceptiveField == structure.lateralDistalReceptiveField);
	assert(_lateralDistalPercentage == structure.lateralDistalPercentage);
	assert(_lateralDistalWrapAround == structure.lateralDistalWrapAround);

	assert(_apicalReceptiveField == structure.apicalReceptiveField);
	assert(_apicalPercentage == structure.apicalPercentage);
	assert(_apicalWrapAround == structure.apicalWrapAround);

	assert(_populationsArrayDimensionality == structure.populationsArrayDimensionality);
	assert(_apicalPopulationsArrayDimensionality == structure.apicalPopulationsArrayDimensionality);

	assert(_potentialPercentage == structure.potentialPercentage);
} // end function checkEncoderLayerStructure







