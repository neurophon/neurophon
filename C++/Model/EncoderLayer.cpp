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
#include <mpi.h>

#include "../Libraries/Model/Random.h"
#include "../Libraries/Model/Topology.h"
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/Templates.h"
#include "../Libraries/Model/OctaveInterface.h"
#include "../Libraries/Model/MatlabInterface.h"
#include "../Libraries/Model/GlobalVariables.h"
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
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();
	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	// There must be -at least- as many encoder columns as processes
	std::size_t	numberOfColumns = std::accumulate(structure.columnsArrayDimensionality.begin(),
							  structure.columnsArrayDimensionality.end(),
							  1, std::multiplies<std::size_t>());

	if (numberOfColumns < world_size) {
		if (world_rank == 0)
			std::cout << "\nEncoderLayer inconsistence: numberOfColumns < world_size\n";

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	if ( world_rank == 0 )
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

	//MPI_Barrier(MPI_COMM_WORLD);

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

	//MPI_Barrier(MPI_COMM_WORLD);

	EncoderLayer::generateColumns(structure.populationsArrayDimensionality,
				      structure.apicalPopulationsArrayDimensionality,
				      structure.potentialPercentage);

	//MPI_Barrier(MPI_COMM_WORLD);

	_afferentUpdateStep = 0;

	EncoderLayer::initializeProximalInputLimits();
	EncoderLayer::initializeWiredVariables();
} // end function encoderInitializer


//this function initializes wired variables
void	EncoderLayer::initializeWiredVariables()
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	if (_columnsDimensionality == 0) {
		if (world_rank == 0)
			std::cout << "\ninitializeWiredVariables inconsistence: _columnsDimensionality == 0\n";

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	_lateral.currentIndexes.resize(_columnsDimensionality);
	_lateral.synchronization.resize(_columnsDimensionality);
	_lateral.information.resize(_columnsDimensionality);
	for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
		_lateral.synchronization[column] = true;
		_lateral.information[column] = false;
	}

	if (_apicalDimensionality == 0) {
		if (world_rank == 0)
			std::cout << "\ninitializeWiredVariables inconsistence: _apicalDimensionality == 0\n";

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

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
	bool	error_flag = false;

	error_flag &= EncoderLayer::validateColumnsInterconnectionsParameters(afferentArrayDimensionality,
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

	error_flag &= EncoderLayer::validatePopulationParameters(populationsArrayDimensionality,
								 apicalPopulationsArrayDimensionality,
								 potentialPercentage);

	if ( error_flag )
		MPI_Abort(MPI_COMM_WORLD,1);

	//MPI_Barrier(MPI_COMM_WORLD);
} // end function validateEncoderLayer


// validates the parameters to configure the columns' interconnections
bool	EncoderLayer::validateColumnsInterconnectionsParameters( const std::vector<std::size_t>& afferentArrayDimensionality,
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
	bool	error_flag = false;

	if ( afferentArrayDimensionality.size() == 0 ||
	     apicalArrayDimensionality.size() == 0 ||
	     columnsArrayDimensionality.size() == 0 ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validateColumnsInterconnectionsParameters\n";
		std::cout << "afferentArrayDimensionality.size() = " << afferentArrayDimensionality.size() << "\n";
		std::cout << "apicalArrayDimensionality.size() = " << apicalArrayDimensionality.size() << "\n";
		std::cout << "columnsArrayDimensionality.size() = " << columnsArrayDimensionality.size() << "\n";
		error_flag = true;
	}

	for ( std::size_t dim = 0; dim < afferentArrayDimensionality.size(); dim++ ) {
		if ( afferentArrayDimensionality[dim] == 0 ) {
			std::cout << "EncoderLayer object construction inconsistence: \n";
			std::cout << "In function validateColumnsInterconnectionsParameters\n";
			std::cout << "afferentArrayDimensionality[" << dim << "] = " << afferentArrayDimensionality[dim] << "\n";
			error_flag = true;
		}
	}

	for ( std::size_t dim = 0; dim < apicalArrayDimensionality.size(); dim++ ) {
		if ( apicalArrayDimensionality[dim] == 0 ) {
			std::cout << "EncoderLayer object construction inconsistence: \n";
			std::cout << "In function validateColumnsInterconnectionsParameters\n";
			std::cout << "apicalArrayDimensionality[" << dim << "] = " << apicalArrayDimensionality[dim] << "\n";
			error_flag = true;
		}
	}

	for ( std::size_t dim = 0; dim < columnsArrayDimensionality.size(); dim++ ) {
		if ( columnsArrayDimensionality[dim] == 0 ) {
			std::cout << "EncoderLayer object construction inconsistence: \n";
			std::cout << "In function validateColumnsInterconnectionsParameters\n";
			std::cout << "columnsArrayDimensionality[" << dim << "] = " << columnsArrayDimensionality[dim] << "\n";
			error_flag = true;
		}
	}

	if ( afferentReceptiveField.size() == 0 ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validateColumnsInterconnectionsParameters\n";

		std::cout << "afferentReceptiveField.size() = " << afferentReceptiveField.size() << "\n";
		error_flag = true;
	}

	if ( (afferentArrayDimensionality.size() != afferentReceptiveField.size()) ||
	     (apicalReceptiveField.size() != 0 && apicalArrayDimensionality.size() != apicalReceptiveField.size()) ||
	     (lateralDistalReceptiveField.size() != 0 && columnsArrayDimensionality.size() != lateralDistalReceptiveField.size()) ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validateColumnsInterconnectionsParameters\n";

		std::cout << "afferentArrayDimensionality.size() = " << afferentArrayDimensionality.size() << "\n";
		std::cout << "while, afferentReceptiveField.size() = " << afferentReceptiveField.size() << "\n";

		std::cout << "apicalArrayDimensionality.size() = " << apicalArrayDimensionality.size() << "\n";
		std::cout << "while, apicalReceptiveField.size() = " << apicalReceptiveField.size() << "\n";

		std::cout << "columnsArrayDimensionality.size() = " << columnsArrayDimensionality.size() << "\n";
		std::cout << "while, lateralDistalReceptiveField.size() = " << lateralDistalReceptiveField.size() << "\n";
		error_flag = true;
	}

	for ( std::size_t dim = 0; dim < afferentReceptiveField.size(); dim++ ) {
		if (afferentArrayDimensionality[dim] % 2) {
			if ( afferentReceptiveField[dim] > afferentArrayDimensionality[dim]/2 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "afferentReceptiveField[" << dim << "] = " << afferentReceptiveField[dim] << "\n";
				std::cout << "while afferentArrayDimensionality[" << dim << "] is = "
				          << afferentArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
		else {
			if ( afferentReceptiveField[dim] > afferentArrayDimensionality[dim]/2-1 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "afferentReceptiveField[" << dim << "] = " << afferentReceptiveField[dim] << "\n";
				std::cout << "while afferentArrayDimensionality[" << dim << "] is = "
				          << afferentArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
	}

	for ( std::size_t dim = 0; dim < apicalReceptiveField.size(); dim++ ) {
		if (apicalArrayDimensionality[dim] % 2) {
			if ( apicalReceptiveField[dim] > apicalArrayDimensionality[dim]/2 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "apicalReceptiveField[" << dim << "] = " << apicalReceptiveField[dim] << "\n";
				std::cout << "while apicalArrayDimensionality[" << dim << "] is = "
				          << apicalArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
		else {
			if ( apicalReceptiveField[dim] > apicalArrayDimensionality[dim]/2-1 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "apicalReceptiveField[" << dim << "] = " << apicalReceptiveField[dim] << "\n";
				std::cout << "while apicalArrayDimensionality[" << dim << "] is = "
				          << apicalArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
	}

	for ( std::size_t dim = 0; dim < lateralDistalReceptiveField.size(); dim++ ) {
		if (columnsArrayDimensionality[dim] % 2) {
			if ( lateralDistalReceptiveField[dim] > columnsArrayDimensionality[dim]/2 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "lateralDistalReceptiveField[" << dim << "] = " << lateralDistalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
		else {
			if ( lateralDistalReceptiveField[dim] > columnsArrayDimensionality[dim]/2-1 ) {
				std::cout << "EncoderLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "lateralDistalReceptiveField[" << dim << "] = " << lateralDistalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
	}

	if ( afferentPercentage < 0.0 || afferentPercentage > 1.0 ||
	     lateralDistalPercentage < 0.0 || lateralDistalPercentage > 1.0 ||
	     apicalPercentage < 0.0 || apicalPercentage > 1.0 ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validateColumnsInterconnectionsParameters\n";
		std::cout << "afferentPercentage = " << afferentPercentage << "\n";
		std::cout << "lateralDistalPercentage = " << lateralDistalPercentage << "\n";
		std::cout << "apicalPercentage = " << apicalPercentage << "\n";
		error_flag = true;
	}

	return	error_flag;
} // end function validateColumnsInterconnectionsParameters


// validates the parameters used to generate the columns' populations
bool	EncoderLayer::validatePopulationParameters( const std::vector<std::size_t>& populationsArrayDimensionality,
						    const std::vector<std::size_t>& apicalPopulationsArrayDimensionality,
	       					    const double potentialPercentage )
{
	bool	error_flag = false;

	if ( potentialPercentage < 0.0 || potentialPercentage > 1.0 ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "potentialPercentage = "
			  << potentialPercentage << "\n";
		error_flag = true;
	}

	if ( apicalPopulationsArrayDimensionality.size() == 0 ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "apicalPopulationsArrayDimensionality.size() = "
			  << apicalPopulationsArrayDimensionality.size() << "\n";
		error_flag = true;
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
				error_flag = true;
			}
		}
	}

	if ( populationsArrayDimensionality.size() == 0 ) {
		std::cout << "EncoderLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "populationsArrayDimensionality.size() = "
			  << populationsArrayDimensionality.size() << "\n";
		error_flag = true;
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
				error_flag = true;
			}
		}
	}

	return	error_flag;
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
	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

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

	if (afferentReceptiveField.size() == 0) {
		if (world_rank == 0)
			std::cout << "\ninterconnectEncoderLayerColumns inconsistence: afferentReceptiveField.size() == 0\n";

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
		_afferentConnections.push_back(EncoderLayer::getAfferentInputs(column));

	_afferentConnections.shrink_to_fit();

	_lateralDistalReceptiveField = lateralDistalReceptiveField;
	_lateralDistalPercentage = lateralDistalPercentage;
	_lateralDistalWrapAround = lateralDistalWrapAround;

	if ( lateralDistalReceptiveField.size() != 0 ) {
		for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
			_lateralDistalConnections.push_back(EncoderLayer::getLateralDistalInputs(column));
	}

	_lateralDistalConnections.shrink_to_fit();

	_apicalReceptiveField = apicalReceptiveField;
	_apicalPercentage = apicalPercentage;
	_apicalWrapAround = apicalWrapAround;

	if ( apicalReceptiveField.size() != 0 ) {
		for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
			_apicalConnections.push_back(EncoderLayer::getApicalInputs(column));
	}

	_apicalConnections.shrink_to_fit();
} // end function interconnectEncoderLayerColumns


// generates a population for every column in the encoder
void	EncoderLayer::generateColumns( const std::vector<std::size_t>& populationsArrayDimensionality,
				       const std::vector<std::size_t>& apicalPopulationsArrayDimensionality,
	       			       const double potentialPercentage )
{
	std::vector<std::size_t>	dynamicUnits;
	
	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	if (_afferentConnections.size() == 0) {
		if (world_rank == 0)
			std::cout << "\ngenerateColumns inconsistence: _afferentConnections.size() == 0\n";

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
		auto	numberOfInputs = _afferentConnections[column/world_size].size();

		if ( _apicalConnections.size() != 0 )
			for ( std::size_t link = 0; link < _apicalConnections[column/world_size].size(); link++ ) {
				auto	auxiliary = std::accumulate(apicalPopulationsArrayDimensionality.begin(),
								    apicalPopulationsArrayDimensionality.end(),
								    1, std::multiplies<std::size_t>());
				dynamicUnits.push_back(auxiliary);
			}

		if ( _lateralDistalConnections.size() != 0 )
			for ( std::size_t link = 0; link < _lateralDistalConnections[column/world_size].size(); link++ ) {
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
	int	auxiliary;

	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &auxiliary);
	std::size_t	world_rank = auxiliary;

	if (_afferentConnections.size() == 0) {
		if (world_rank == 0)
			std::cout << "\ninitializeProximalInputLimits inconsistence: _afferentConnections.size() == 0\n";

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

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
	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	if (parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0) {
		if (world_rank == 0) {
			std::cout << "\ncomputeResponse inconsistence: parameters.proximalInformationThreshold == "
				  << parameters.proximalInformationThreshold << "\n";
		}

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	if (parameters.distalInformationThreshold < 0.0 || parameters.distalInformationThreshold > 1.0) {
		if (world_rank == 0) {
			std::cout << "\ncomputeResponse inconsistence: parameters.distalInformationThreshold == "
				  << parameters.distalInformationThreshold << "\n";
		}

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	regularLayerResponse	output;

	output.currentIndexes.resize(_columnsDimensionality);
	output.synchronization.resize(_columnsDimensionality);
	output.information.resize(_columnsDimensionality);

	#pragma omp parallel for default(none) shared(afferent, parameters, output, world_rank, world_size)
	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
		auto	proximalInputs = EncoderLayer::gatherProximalInputs(column/world_size, afferent);
		auto	distalInputs = EncoderLayer::gatherDistalInputs(column/world_size, _apical, _lateral);

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
				auto	response = _encoderColumns[column/world_size].getResponse(proximalInputs.inputs);

				//auto	activeIndexes = _encoderColumns[column/world_size].Activate(response,
												    //distalInputs.activeIndexes,
												    //parameters.activationRadius,
												    //parameters.sparsity);
			
				std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
								       std::accumulate(_populationsArrayDimensionality.begin(),
										       _populationsArrayDimensionality.end(),
										       1, std::multiplies<std::size_t>());	

				std::vector<std::size_t>	activeIndexes;
				if ( parameters.enableLearning && parameters.learning.enableDistalLearning ) {
					activeIndexes = _encoderColumns[column/world_size].Activate(response,
												    distalInputs.activeIndexes,
												    numberOfExcitedUnits,
												    parameters.sparsity,
												    true);
				}
				else {
					activeIndexes = _encoderColumns[column/world_size].Activate(response,
												    distalInputs.activeIndexes,
												    numberOfExcitedUnits,
												    parameters.sparsity);
				}

				if ( parameters.enableLearning ) {	// If learning is enabled
					if (parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0)
						throw std::runtime_error (
						"parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0");

					if ( parameters.learning.enableProximalLearning ) {
						_encoderColumns[column/world_size].learningRule(parameters.learning.proximalLearningRate,
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
							_encoderColumns[column/world_size].Update(activeIndexes,
												  distalInputs.activeIndexes,
												  true,
												  DISTAL_SYNAPTIC_THRESHOLD,
												  parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_encoderColumns[column/world_size].Update(activeIndexes,
												  distalInputs.activeIndexes,
												  true,
												  DISTAL_SYNAPTIC_THRESHOLD,
												  parameters.learning.distalLearningRate);
						}

						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveIndexes = _lateral.currentIndexes[column]; 
							_encoderColumns[column/world_size].Update(lastActiveIndexes,
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

	EncoderLayer::mergeOutputs(output);

	if ( parameters.enableLearning )	// If learning is enabled, then it calls updateProximalInputLimits member function
		EncoderLayer::updateProximalInputLimits(afferent, parameters.learning);

	return	output;
} // end function computeResponse


// computes the encoder response
regularLayerResponse	EncoderLayer::computeResponse( const encoderLayerInput& afferent,
						       const regularLayerResponse& lateral,
						       const encoderLayerParameters& parameters )
{
	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	if (parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0) {
		if (world_rank == 0) {
			std::cout << "\ncomputeResponse inconsistence: parameters.proximalInformationThreshold == "
				  << parameters.proximalInformationThreshold << "\n";
		}

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	if (parameters.distalInformationThreshold < 0.0 || parameters.distalInformationThreshold > 1.0) {
		if (world_rank == 0) {
			std::cout << "\ncomputeResponse inconsistence: parameters.distalInformationThreshold == "
				  << parameters.distalInformationThreshold << "\n";
		}

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	regularLayerResponse	output;

	output.currentIndexes.resize(_columnsDimensionality);
	output.synchronization.resize(_columnsDimensionality);
	output.information.resize(_columnsDimensionality);

	#pragma omp parallel for default(none) shared(afferent, lateral, parameters, output, world_rank, world_size)
	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
		auto	proximalInputs = EncoderLayer::gatherProximalInputs(column/world_size, afferent);
		auto	distalInputs = EncoderLayer::gatherDistalInputs(column/world_size, _apical, lateral);

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
				auto	response = _encoderColumns[column/world_size].getResponse(proximalInputs.inputs);

				//auto	activeIndexes = _encoderColumns[column/world_size].Activate(response,
												    //distalInputs.activeIndexes,
												    //parameters.activationRadius,
												    //parameters.sparsity);
			
				std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
								       std::accumulate(_populationsArrayDimensionality.begin(),
										       _populationsArrayDimensionality.end(),
										       1, std::multiplies<std::size_t>());	

				std::vector<std::size_t>	activeIndexes;
				if ( parameters.enableLearning && parameters.learning.enableDistalLearning ) {
					activeIndexes = _encoderColumns[column/world_size].Activate(response,
												    distalInputs.activeIndexes,
												    numberOfExcitedUnits,
												    parameters.sparsity,
												    true);
				}
				else {
					activeIndexes = _encoderColumns[column/world_size].Activate(response,
												    distalInputs.activeIndexes,
												    numberOfExcitedUnits,
												    parameters.sparsity);
				}

				if ( parameters.enableLearning ) {	// If learning is enabled
					if (parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0)
						throw std::runtime_error (
						"parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0");

					if ( parameters.learning.enableProximalLearning ) {
						_encoderColumns[column/world_size].learningRule(parameters.learning.proximalLearningRate,
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
							_encoderColumns[column/world_size].Update(activeIndexes,
												  distalInputs.activeIndexes,
												  true,
												  DISTAL_SYNAPTIC_THRESHOLD,
												  parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_encoderColumns[column/world_size].Update(activeIndexes,
												  distalInputs.activeIndexes,
												  true,
												  DISTAL_SYNAPTIC_THRESHOLD,
												  parameters.learning.distalLearningRate);
						}

						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveIndexes = lateral.currentIndexes[column]; 
							_encoderColumns[column/world_size].Update(lastActiveIndexes,
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

	EncoderLayer::mergeOutputs(output);

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
	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	if (parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0) {
		if (world_rank == 0) {
			std::cout << "\ncomputeResponse inconsistence: parameters.proximalInformationThreshold == "
				  << parameters.proximalInformationThreshold << "\n";
		}

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	if (parameters.distalInformationThreshold < 0.0 || parameters.distalInformationThreshold > 1.0) {
		if (world_rank == 0) {
			std::cout << "\ncomputeResponse inconsistence: parameters.distalInformationThreshold == "
				  << parameters.distalInformationThreshold << "\n";
		}

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	regularLayerResponse	output;

	output.currentIndexes.resize(_columnsDimensionality);
	output.synchronization.resize(_columnsDimensionality);
	output.information.resize(_columnsDimensionality);

	#pragma omp parallel for default(none) shared(afferent, lateral, apical, parameters, output, world_rank, world_size)
	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
		auto	proximalInputs = EncoderLayer::gatherProximalInputs(column/world_size, afferent);
		auto	distalInputs = EncoderLayer::gatherDistalInputs(column/world_size, apical, lateral);

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
				auto	response = _encoderColumns[column/world_size].getResponse(proximalInputs.inputs);

				//auto	activeIndexes = _encoderColumns[column/world_size].Activate(response,
												    //distalInputs.activeIndexes,
												    //parameters.activationRadius,
												    //parameters.sparsity);
			
				std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
								       std::accumulate(_populationsArrayDimensionality.begin(),
										       _populationsArrayDimensionality.end(),
										       1, std::multiplies<std::size_t>());	

				std::vector<std::size_t>	activeIndexes;
				if ( parameters.enableLearning && parameters.learning.enableDistalLearning ) {
					activeIndexes = _encoderColumns[column/world_size].Activate(response,
												    distalInputs.activeIndexes,
												    numberOfExcitedUnits,
												    parameters.sparsity,
												    true);
				}
				else {
					activeIndexes = _encoderColumns[column/world_size].Activate(response,
												    distalInputs.activeIndexes,
												    numberOfExcitedUnits,
												    parameters.sparsity);
				}

				if ( parameters.enableLearning ) {	// If learning is enabled
					if (parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0)
						throw std::runtime_error (
						"parameters.proximalInformationThreshold < 0.0 || parameters.proximalInformationThreshold > 1.0");

					if ( parameters.learning.enableProximalLearning ) {
						_encoderColumns[column/world_size].learningRule(parameters.learning.proximalLearningRate,
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
							_encoderColumns[column/world_size].Update(activeIndexes,
												  distalInputs.activeIndexes,
												  true,
												  DISTAL_SYNAPTIC_THRESHOLD,
												  parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_encoderColumns[column/world_size].Update(activeIndexes,
												  distalInputs.activeIndexes,
												  true,
												  DISTAL_SYNAPTIC_THRESHOLD,
												  parameters.learning.distalLearningRate);
						}

						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveIndexes = lateral.currentIndexes[column]; 
							_encoderColumns[column/world_size].Update(lastActiveIndexes,
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

	EncoderLayer::mergeOutputs(output);

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
	auto	auxiliaryConnections = EncoderLayer::gatherConnections();

	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	std::stringstream	outputStream;

	if ( world_rank == 0 ) {
		// file preamble.
		if (ENABLE_MATLAB_COMPATIBILITY) {
			save_the_header(outputStream);
		}
		else {
			outputStream << "# This is a file created by saveEncoderLayerStatus member function in EncoderLayer class from," << endl;
			outputStream << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
			outputStream << "# Author: Dematties Dario Jesus." << endl;
			outputStream << "\n\n" << endl;
		}
		
		// saves afferentArrayDimensionality
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_as_numeric_array("afferentArrayDimensionality", _afferentArrayDimensionality, outputStream);
		else
			save_vector_as_matrix("afferentArrayDimensionality", _afferentArrayDimensionality, outputStream);

		// saves afferentDimensionality
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_scalar_as_numeric_array("afferentDimensionality", _afferentDimensionality, outputStream);
		else
			save_as_scalar("afferentDimensionality", _afferentDimensionality, outputStream);

		// saves afferentReceptiveField
		if(_afferentReceptiveField.size() != 0) {
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_vector_as_numeric_array("afferentReceptiveField", _afferentReceptiveField, outputStream);
			else
				save_vector_as_matrix("afferentReceptiveField", _afferentReceptiveField, outputStream);
		}
		else {
			std::vector<int>	receptiveField(_afferentArrayDimensionality.size(),-1);
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_vector_as_numeric_array("afferentReceptiveField", receptiveField, outputStream);
			else
				save_vector_as_matrix("afferentReceptiveField", receptiveField, outputStream);
		}

		// saves afferentPercentage
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_scalar_as_numeric_array("afferentPercentage", _afferentPercentage, outputStream);
		else
			save_as_scalar("afferentPercentage", _afferentPercentage, outputStream);

		// saves afferentWrapAround
		if (ENABLE_MATLAB_COMPATIBILITY) {
			std::size_t	aux_bool;
			if (_afferentWrapAround)
				aux_bool = 1;
			else
				aux_bool = 0;

			save_scalar_as_numeric_array("afferentWrapAround", aux_bool, outputStream);
		}
		else {
			save_as_bool("afferentWrapAround", _afferentWrapAround, outputStream);
		}



		// saves apicalArrayDimensionality
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_as_numeric_array("apicalArrayDimensionality", _apicalArrayDimensionality, outputStream);
		else
			save_vector_as_matrix("apicalArrayDimensionality", _apicalArrayDimensionality, outputStream);

		// saves apicalDimensionality
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_scalar_as_numeric_array("apicalDimensionality", _apicalDimensionality, outputStream);
		else
			save_as_scalar("apicalDimensionality", _apicalDimensionality, outputStream);

		// saves apicalReceptiveField
		if(_apicalReceptiveField.size() != 0) {
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_vector_as_numeric_array("apicalReceptiveField", _apicalReceptiveField, outputStream);
			else
				save_vector_as_matrix("apicalReceptiveField", _apicalReceptiveField, outputStream);
		}
		else {
			std::vector<int>	receptiveField(_apicalArrayDimensionality.size(),-1);
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_vector_as_numeric_array("apicalReceptiveField", receptiveField, outputStream);
			else
				save_vector_as_matrix("apicalReceptiveField", receptiveField, outputStream);
		}

		// saves apicalPercentage
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_scalar_as_numeric_array("apicalPercentage", _apicalPercentage, outputStream);
		else
			save_as_scalar("apicalPercentage", _apicalPercentage, outputStream);

		// saves apicalWrapAround
		if (ENABLE_MATLAB_COMPATIBILITY) {
			std::size_t	aux_bool;
			if (_apicalWrapAround)
				aux_bool = 1;
			else
				aux_bool = 0;

			save_scalar_as_numeric_array("apicalWrapAround", aux_bool, outputStream);
		}
		else {
			save_as_bool("apicalWrapAround", _apicalWrapAround, outputStream);
		}



		// saves columnsArrayDimensionality
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_as_numeric_array("columnsArrayDimensionality", _columnsArrayDimensionality, outputStream);
		else
			save_vector_as_matrix("columnsArrayDimensionality", _columnsArrayDimensionality, outputStream);

		// saves columnsDimensionality
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_scalar_as_numeric_array("columnsDimensionality", _columnsDimensionality, outputStream);
		else
			save_as_scalar("columnsDimensionality", _columnsDimensionality, outputStream);



		// saves lateralDistalReceptiveField
		if(_lateralDistalReceptiveField.size() != 0) {
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_vector_as_numeric_array("lateralDistalReceptiveField", _lateralDistalReceptiveField, outputStream);
			else
				save_vector_as_matrix("lateralDistalReceptiveField", _lateralDistalReceptiveField, outputStream);
		}
		else {
			std::vector<int>	receptiveField(_columnsArrayDimensionality.size(),-1);
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_vector_as_numeric_array("lateralDistalReceptiveField", receptiveField, outputStream);
			else
				save_vector_as_matrix("lateralDistalReceptiveField", receptiveField, outputStream);
		}

		// saves lateralDistalPercentage
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_scalar_as_numeric_array("lateralDistalPercentage", _lateralDistalPercentage, outputStream);
		else
			save_as_scalar("lateralDistalPercentage", _lateralDistalPercentage, outputStream);

		// saves lateralDistalWrapAround
		if (ENABLE_MATLAB_COMPATIBILITY) {
			std::size_t	aux_bool;
			if (_lateralDistalWrapAround)
				aux_bool = 1;
			else
				aux_bool = 0;

			save_scalar_as_numeric_array("lateralDistalWrapAround", aux_bool, outputStream);
		}
		else {
			save_as_bool("lateralDistalWrapAround", _lateralDistalWrapAround, outputStream);
		}


		if ( auxiliaryConnections.afferentConnections.size() != 0 ) {
			// saves afferentConnections
			twodvector<bool>	afferentConnectionsBitMap;
			afferentConnectionsBitMap.resize(_columnsDimensionality);
			for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
				afferentConnectionsBitMap[column].resize(_afferentDimensionality);
				for ( std::size_t index = 0; index < auxiliaryConnections.afferentConnections[column].size(); index++ )
					afferentConnectionsBitMap[column][auxiliaryConnections.afferentConnections[column][index]] = true;
			}

			SparseMatrixElements<bool>	sparseAfferentConnectionsBitMap;
			sparseAfferentConnectionsBitMap = to_sparse(afferentConnectionsBitMap);
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_sparse_matrix_elements_as_sparse_array("afferentConnections", sparseAfferentConnectionsBitMap, outputStream);
			else
				save_sparse_matrix_elements_as_sparse_matrix("afferentConnections", sparseAfferentConnectionsBitMap, outputStream);
		}

		if ( auxiliaryConnections.lateralDistalConnections.size() != 0 ) {
			// saves lateralDistalConnections
			twodvector<bool>	lateralDistalConnectionsBitMap;
			lateralDistalConnectionsBitMap.resize(_columnsDimensionality);
			for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
				lateralDistalConnectionsBitMap[column].resize(_columnsDimensionality);
				for ( std::size_t index = 0; index < auxiliaryConnections.lateralDistalConnections[column].size(); index++ )
					lateralDistalConnectionsBitMap[column][auxiliaryConnections.lateralDistalConnections[column][index]] = true;
			}	

			SparseMatrixElements<bool>	sparseLateralDistalConnectionsBitMap;
			sparseLateralDistalConnectionsBitMap = to_sparse(lateralDistalConnectionsBitMap);
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_sparse_matrix_elements_as_sparse_array("lateralDistalConnections", sparseLateralDistalConnectionsBitMap, outputStream);
			else
				save_sparse_matrix_elements_as_sparse_matrix("lateralDistalConnections", sparseLateralDistalConnectionsBitMap, outputStream);
		}

		if ( auxiliaryConnections.apicalConnections.size() != 0 ) {
			// saves apicalConnections
			twodvector<bool>	apicalConnectionsBitMap;
			apicalConnectionsBitMap.resize(_columnsDimensionality);
			for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
				apicalConnectionsBitMap[column].resize(_apicalDimensionality);
				for ( std::size_t index = 0; index < auxiliaryConnections.apicalConnections[column].size(); index++ )
					apicalConnectionsBitMap[column][auxiliaryConnections.apicalConnections[column][index]] = true;
			}	

			SparseMatrixElements<bool>	sparseApicalConnectionsBitMap;
			sparseApicalConnectionsBitMap = to_sparse(apicalConnectionsBitMap);
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_sparse_matrix_elements_as_sparse_array("apicalConnections", sparseApicalConnectionsBitMap, outputStream);
			else
				save_sparse_matrix_elements_as_sparse_matrix("apicalConnections", sparseApicalConnectionsBitMap, outputStream);
		}


		// saves populationsArrayDimensionality
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_as_numeric_array("populationsArrayDimensionality", _populationsArrayDimensionality, outputStream);
		else
			save_vector_as_matrix("populationsArrayDimensionality", _populationsArrayDimensionality, outputStream);

		// saves apicalPopulationsArrayDimensionality
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_as_numeric_array("apicalPopulationsArrayDimensionality", _apicalPopulationsArrayDimensionality, outputStream);
		else
			save_vector_as_matrix("apicalPopulationsArrayDimensionality", _apicalPopulationsArrayDimensionality, outputStream);

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
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_vector_as_numeric_array("proximalAfferentLowerLimits", lowerAfferentLimits, outputStream);
			else
				save_vector_as_matrix("proximalAfferentLowerLimits", lowerAfferentLimits, outputStream);

			// saves proximalAfferentUpperLimits
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_vector_as_numeric_array("proximalAfferentUpperLimits", upperAfferentLimits, outputStream);
			else
				save_vector_as_matrix("proximalAfferentUpperLimits", upperAfferentLimits, outputStream);
		}

		if ( _afferentConnections.size() != 0 ) {
			// saves afferentUpdateStep
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_scalar_as_numeric_array("afferentUpdateStep", _afferentUpdateStep, outputStream);
			else
				save_as_scalar("afferentUpdateStep", _afferentUpdateStep, outputStream);
		}

		// saves potentialPercentage
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_scalar_as_numeric_array("potentialPercentage", _potentialPercentage, outputStream);
		else
			save_as_scalar("potentialPercentage", _potentialPercentage, outputStream);
	}

	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
		_encoderColumns[column/world_size].saveComplexSelfOrganizingMapStatus(std::to_string(column), outputStream);

	// File view comunication among ranks
	std::size_t	fileView = 0, fileViewNew;
	std::size_t	lengthToBeSent = outputStream.str().size();
	if ( world_rank == 0 ) {
		fileViewNew = fileView+lengthToBeSent;
		if ( world_size > 1 )
			MPI_Send(&fileViewNew, 1, my_MPI_SIZE_T,
				 (int)(world_rank+1), 3, MPI_COMM_WORLD);
	}
	else if ( world_rank == world_size-1 ) {
		MPI_Recv(&fileView, 1, my_MPI_SIZE_T,
			 (int)(world_rank-1), 3, MPI_COMM_WORLD,
			 MPI_STATUS_IGNORE);
	}
	else {
		MPI_Recv(&fileView, 1, my_MPI_SIZE_T,
			 (int)(world_rank-1), 3, MPI_COMM_WORLD,
			 MPI_STATUS_IGNORE);
		
		fileViewNew = fileView+lengthToBeSent;
		MPI_Send(&fileViewNew, 1, my_MPI_SIZE_T,
			 (int)(world_rank+1), 3, MPI_COMM_WORLD);
	}
	//MPI_Barrier(MPI_COMM_WORLD);

	std::string	name = "../../Octave/" + folderName + "/EncoderLayer.mat";
	std::remove(&name[0]);
	// open a file in write mode
	MPI::File outfile = MPI::File::Open(MPI::COMM_WORLD, (name).c_str(),
					    MPI::MODE_CREATE | MPI::MODE_WRONLY,
					    MPI::INFO_NULL);

	// sets the file view for this rank
	outfile.Set_view(fileView * sizeof(char),
			 MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR,
			 "native", MPI::INFO_NULL);

	// write stream in file
	outfile.Write(&outputStream.str()[0],
		     lengthToBeSent,
		     MPI_UNSIGNED_CHAR);

	// close the opened file.
	outfile.Close();
} // end function saveEncoderLayerStatus


// function that loads the EncoderLayer's status in a file
void	EncoderLayer::loadEncoderLayerStatus( const std::string& folderName )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	std::stringstream	inputStream;

	// open a file in read mode.
	MPI::File infile = MPI::File::Open(MPI::COMM_WORLD, ("../../Octave/" + folderName + "/EncoderLayer.mat").c_str(),
					   MPI::MODE_RDONLY,
					   MPI::INFO_NULL);

	// gets the file size
	MPI::Offset filesize = infile.Get_size(); // in bytes
	int	bufsize = filesize / sizeof(char); /* in number of char */
	std::string	auxiliaryString;
	auxiliaryString.resize(bufsize);

	// sets the files view
	infile.Set_view(0 * bufsize * sizeof(char),
			MPI_UNSIGNED_CHAR,
			MPI_UNSIGNED_CHAR,
			"native", MPI::INFO_NULL);

	// read the complete file
	infile.Read(&auxiliaryString[0], bufsize,
		    MPI_UNSIGNED_CHAR);

	inputStream.str(auxiliaryString);

	// close the opened file.
	infile.Close();

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

	if (ENABLE_MATLAB_COMPATIBILITY) {
		big_endianness = load_the_header(inputStream);
		auto	array_structure = check_next_data_structure(inputStream, big_endianness);
		while ( array_structure.more_data ) {

			STR = "afferentArrayDimensionality";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_vector(array_structure, _afferentArrayDimensionality, inputStream, big_endianness);
				check_afferentArrayDimensionality = true;
			}

			STR = "afferentDimensionality";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _afferentDimensionality, inputStream, big_endianness);
				check_afferentDimensionality = true;
			}

			STR = "afferentReceptiveField";
			if ( array_structure.name.compare(STR) == 0 ) {
				_afferentReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_numeric_array_to_vector(array_structure, receptiveField, inputStream, big_endianness);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_afferentReceptiveField.push_back(s);
				}
				check_afferentReceptiveField = true;
			}

			STR = "afferentPercentage";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _afferentPercentage, inputStream, big_endianness);
				check_afferentPercentage = true;
			}

			STR = "afferentWrapAround";
			if ( array_structure.name.compare(STR) == 0 ) {
				std::size_t	aux_bool;
				load_numeric_array_to_scalar(array_structure, aux_bool, inputStream, big_endianness);
				if (aux_bool > 0)
					_afferentWrapAround = true;
				else
					_afferentWrapAround = false;

				check_afferentWrapAround = true;
			}

			STR = "apicalArrayDimensionality";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_vector(array_structure, _apicalArrayDimensionality, inputStream, big_endianness);
				check_apicalArrayDimensionality = true;
			}

			STR = "apicalDimensionality";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _apicalDimensionality, inputStream, big_endianness);
				check_apicalDimensionality = true;
			}

			STR = "apicalReceptiveField";
			if ( array_structure.name.compare(STR) == 0 ) {
				_apicalReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_numeric_array_to_vector(array_structure, receptiveField, inputStream, big_endianness);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_apicalReceptiveField.push_back(s);
				}
				check_apicalReceptiveField = true;
			}

			STR = "apicalPercentage";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _apicalPercentage, inputStream, big_endianness);
				check_apicalPercentage = true;
			}

			STR = "apicalWrapAround";
			if ( array_structure.name.compare(STR) == 0 ) {
				std::size_t	aux_bool;
				load_numeric_array_to_scalar(array_structure, aux_bool, inputStream, big_endianness);
				if (aux_bool > 0)
					_apicalWrapAround = true;
				else
					_apicalWrapAround = false;

				check_apicalWrapAround = true;
			}

			STR = "columnsArrayDimensionality";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_vector(array_structure, _columnsArrayDimensionality, inputStream, big_endianness);
				check_columnsArrayDimensionality = true;
			}

			STR = "columnsDimensionality";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _columnsDimensionality, inputStream, big_endianness);
				check_columnsDimensionality = true;
			}

			STR = "lateralDistalReceptiveField";
			if ( array_structure.name.compare(STR) == 0 ) {
				_lateralDistalReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_numeric_array_to_vector(array_structure, receptiveField, inputStream, big_endianness);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_lateralDistalReceptiveField.push_back(s);
				}
				check_lateralDistalReceptiveField = true;
			}

			STR = "lateralDistalPercentage";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _lateralDistalPercentage, inputStream, big_endianness);
				check_lateralDistalPercentage = true;
			}

			STR = "lateralDistalWrapAround";
			if ( array_structure.name.compare(STR) == 0 ) {
				std::size_t	aux_bool;
				load_numeric_array_to_scalar(array_structure, aux_bool, inputStream, big_endianness);
				if (aux_bool > 0)
					_lateralDistalWrapAround = true;
				else
					_lateralDistalWrapAround = false;

				check_lateralDistalWrapAround = true;
			}

			STR = "afferentConnections";
			if ( array_structure.name.compare(STR) == 0 ) {
				check_afferentConnections = true;
				SparseMatrixElements<bool>	sparseAfferentConnectionsBitMap;
				load_sparse_matrix_elements_as_sparse_array(array_structure, sparseAfferentConnectionsBitMap, inputStream, big_endianness);
				twodvector<bool>	afferentConnectionsBitMap;
				afferentConnectionsBitMap = from_sparse(sparseAfferentConnectionsBitMap);
				for ( std::size_t column = world_rank; column < afferentConnectionsBitMap.size(); column=column+world_size ) {
					std::vector<std::size_t>	auxiliaryConnections;
					for ( std::size_t index = 0; index < afferentConnectionsBitMap[column].size(); index++ ) {
						if ( afferentConnectionsBitMap[column][index] == true )
							auxiliaryConnections.push_back(index);
					}

					_afferentConnections.push_back(auxiliaryConnections);
				}
				_afferentConnections.shrink_to_fit();
			}

			STR = "lateralDistalConnections";
			if ( array_structure.name.compare(STR) == 0 ) {
				check_lateralDistalConnections = true;
				SparseMatrixElements<bool>	sparseLateralDistalConnectionsBitMap;
				load_sparse_matrix_elements_as_sparse_array(array_structure, sparseLateralDistalConnectionsBitMap, inputStream, big_endianness);
				twodvector<bool>	lateralDistalConnectionsBitMap;
				lateralDistalConnectionsBitMap = from_sparse(sparseLateralDistalConnectionsBitMap);
				for ( std::size_t column = world_rank; column < lateralDistalConnectionsBitMap.size(); column=column+world_size ) {
					std::vector<std::size_t>	auxiliaryConnections;
					for ( std::size_t index = 0; index < lateralDistalConnectionsBitMap[column].size(); index++ ) {
						if ( lateralDistalConnectionsBitMap[column][index] == true )
							auxiliaryConnections.push_back(index);
					}
					_lateralDistalConnections.push_back(auxiliaryConnections);
				}
				_lateralDistalConnections.shrink_to_fit();
			}

			STR = "apicalConnections";
			if ( array_structure.name.compare(STR) == 0 ) {
				check_apicalConnections = true;
				SparseMatrixElements<bool>	sparseApicalConnectionsBitMap;
				load_sparse_matrix_elements_as_sparse_array(array_structure, sparseApicalConnectionsBitMap, inputStream, big_endianness);
				twodvector<bool>	apicalConnectionsBitMap;
				apicalConnectionsBitMap = from_sparse(sparseApicalConnectionsBitMap);
				for ( std::size_t column = world_rank; column < apicalConnectionsBitMap.size(); column=column+world_size ) {
					std::vector<std::size_t>	auxiliaryConnections;
					for ( std::size_t index = 0; index < apicalConnectionsBitMap[column].size(); index++ ) {
						if ( apicalConnectionsBitMap[column][index] == true )
							auxiliaryConnections.push_back(index);
					}
					_apicalConnections.push_back(auxiliaryConnections);
				}
				_apicalConnections.shrink_to_fit();
			}

			STR = "populationsArrayDimensionality";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_vector(array_structure, _populationsArrayDimensionality, inputStream, big_endianness);
				check_populationsArrayDimensionality = true;
			}

			STR = "apicalPopulationsArrayDimensionality";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_vector(array_structure, _apicalPopulationsArrayDimensionality, inputStream, big_endianness);
				check_apicalPopulationsArrayDimensionality = true;
			}

			STR = "proximalAfferentLowerLimits";
			if ( array_structure.name.compare(STR) == 0 ) {
				check_proximalAfferentLowerLimits = true;
				std::vector<double>	lowerLimits;
				load_numeric_array_to_vector(array_structure, lowerLimits, inputStream, big_endianness);
				auto	numberOfLimits = lowerLimits.size();
				_proximalAfferentLimits.resize(numberOfLimits);
				for ( std::size_t limit = 0; limit < numberOfLimits; limit++ )
					_proximalAfferentLimits[limit][0] = lowerLimits[limit];
			}

			STR = "proximalAfferentUpperLimits";
			if ( array_structure.name.compare(STR) == 0 ) {
				check_proximalAfferentUpperLimits = true;
				std::vector<double>	upperLimits;
				load_numeric_array_to_vector(array_structure, upperLimits, inputStream, big_endianness);
				auto	numberOfLimits = upperLimits.size();
				_proximalAfferentLimits.resize(numberOfLimits);
				for ( std::size_t limit = 0; limit < numberOfLimits; limit++ )
					_proximalAfferentLimits[limit][1] = upperLimits[limit];
			}

			STR = "afferentUpdateStep";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _afferentUpdateStep, inputStream, big_endianness);
				check_afferentUpdateStep = true;
			}

			STR = "potentialPercentage";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _potentialPercentage, inputStream, big_endianness);
				check_potentialPercentage = true;
			}

			array_structure = check_next_data_structure(inputStream,big_endianness);
		}	

	}
	else {
		while ( std::getline(inputStream, str) ) {

			STR = "# name: afferentArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_afferentArrayDimensionality, inputStream);
				check_afferentArrayDimensionality = true;
			}

			STR = "# name: afferentDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_afferentDimensionality, inputStream);
				check_afferentDimensionality = true;
			}

			STR = "# name: afferentReceptiveField";
			if ( str.compare(STR) == 0 ) {
				_afferentReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_matrix_to_vector(receptiveField, inputStream);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_afferentReceptiveField.push_back(s);
				}
				check_afferentReceptiveField = true;
			}

			STR = "# name: afferentPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_afferentPercentage, inputStream);
				check_afferentPercentage = true;
			}

			STR = "# name: afferentWrapAround";
			if ( str.compare(STR) == 0 ) {
				load_bool(_afferentWrapAround, inputStream);
				check_afferentWrapAround = true;
			}



			STR = "# name: apicalArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_apicalArrayDimensionality, inputStream);
				check_apicalArrayDimensionality = true;
			}

			STR = "# name: apicalDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_apicalDimensionality, inputStream);
				check_apicalDimensionality = true;
			}

			STR = "# name: apicalReceptiveField";
			if ( str.compare(STR) == 0 ) {
				_apicalReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_matrix_to_vector(receptiveField, inputStream);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_apicalReceptiveField.push_back(s);
				}
				check_apicalReceptiveField = true;
			}

			STR = "# name: apicalPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_apicalPercentage, inputStream);
				check_apicalPercentage = true;
			}

			STR = "# name: apicalWrapAround";
			if ( str.compare(STR) == 0 ) {
				load_bool(_apicalWrapAround, inputStream);
				check_apicalWrapAround = true;
			}



			STR = "# name: columnsArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_columnsArrayDimensionality, inputStream);
				check_columnsArrayDimensionality = true;
			}

			STR = "# name: columnsDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_columnsDimensionality, inputStream);
				check_columnsDimensionality = true;
			}



			STR = "# name: lateralDistalReceptiveField";
			if ( str.compare(STR) == 0 ) {
				_lateralDistalReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_matrix_to_vector(receptiveField, inputStream);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_lateralDistalReceptiveField.push_back(s);
				}
				check_lateralDistalReceptiveField = true;
			}

			STR = "# name: lateralDistalPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_lateralDistalPercentage, inputStream);
				check_lateralDistalPercentage = true;
			}

			STR = "# name: lateralDistalWrapAround";
			if ( str.compare(STR) == 0 ) {
				load_bool(_lateralDistalWrapAround, inputStream);
				check_lateralDistalWrapAround = true;
			}



			STR = "# name: afferentConnections";
			if ( str.compare(STR) == 0 ) {
				check_afferentConnections = true;
				SparseMatrixElements<bool>	sparseAfferentConnectionsBitMap;
				load_sparse_matrix_to_sparse_matrix_elements(sparseAfferentConnectionsBitMap, inputStream);
				twodvector<bool>	afferentConnectionsBitMap;
				afferentConnectionsBitMap = from_sparse(sparseAfferentConnectionsBitMap);
				for ( std::size_t column = world_rank; column < afferentConnectionsBitMap.size(); column=column+world_size ) {
					std::vector<std::size_t>	auxiliaryConnections;
					for ( std::size_t index = 0; index < afferentConnectionsBitMap[column].size(); index++ ) {
						if ( afferentConnectionsBitMap[column][index] == true )
							auxiliaryConnections.push_back(index);
					}

					_afferentConnections.push_back(auxiliaryConnections);
				}
				_afferentConnections.shrink_to_fit();
			}

			STR = "# name: lateralDistalConnections";
			if ( str.compare(STR) == 0 ) {
				check_lateralDistalConnections = true;
				SparseMatrixElements<bool>	sparseLateralDistalConnectionsBitMap;
				load_sparse_matrix_to_sparse_matrix_elements(sparseLateralDistalConnectionsBitMap, inputStream);
				twodvector<bool>	lateralDistalConnectionsBitMap;
				lateralDistalConnectionsBitMap = from_sparse(sparseLateralDistalConnectionsBitMap);
				for ( std::size_t column = world_rank; column < lateralDistalConnectionsBitMap.size(); column=column+world_size ) {
					std::vector<std::size_t>	auxiliaryConnections;
					for ( std::size_t index = 0; index < lateralDistalConnectionsBitMap[column].size(); index++ ) {
						if ( lateralDistalConnectionsBitMap[column][index] == true )
							auxiliaryConnections.push_back(index);
					}
					_lateralDistalConnections.push_back(auxiliaryConnections);
				}
				_lateralDistalConnections.shrink_to_fit();
			}

			STR = "# name: apicalConnections";
			if ( str.compare(STR) == 0 ) {
				check_apicalConnections = true;
				SparseMatrixElements<bool>	sparseApicalConnectionsBitMap;
				load_sparse_matrix_to_sparse_matrix_elements(sparseApicalConnectionsBitMap, inputStream);
				twodvector<bool>	apicalConnectionsBitMap;
				apicalConnectionsBitMap = from_sparse(sparseApicalConnectionsBitMap);
				for ( std::size_t column = world_rank; column < apicalConnectionsBitMap.size(); column=column+world_size ) {
					std::vector<std::size_t>	auxiliaryConnections;
					for ( std::size_t index = 0; index < apicalConnectionsBitMap[column].size(); index++ ) {
						if ( apicalConnectionsBitMap[column][index] == true )
							auxiliaryConnections.push_back(index);
					}
					_apicalConnections.push_back(auxiliaryConnections);
				}
				_apicalConnections.shrink_to_fit();
			}

			STR = "# name: populationsArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_populationsArrayDimensionality, inputStream);
				check_populationsArrayDimensionality = true;
			}

			STR = "# name: apicalPopulationsArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_apicalPopulationsArrayDimensionality, inputStream);
				check_apicalPopulationsArrayDimensionality = true;
			}



			STR = "# name: proximalAfferentLowerLimits";
			if ( str.compare(STR) == 0 ) {
				check_proximalAfferentLowerLimits = true;
				std::vector<double>	lowerLimits;
				load_matrix_to_vector(lowerLimits, inputStream);
				auto	numberOfLimits = lowerLimits.size();
				_proximalAfferentLimits.resize(numberOfLimits);
				for ( std::size_t limit = 0; limit < numberOfLimits; limit++ )
					_proximalAfferentLimits[limit][0] = lowerLimits[limit];
			}

			STR = "# name: proximalAfferentUpperLimits";
			if ( str.compare(STR) == 0 ) {
				check_proximalAfferentUpperLimits = true;
				std::vector<double>	upperLimits;
				load_matrix_to_vector(upperLimits, inputStream);
				auto	numberOfLimits = upperLimits.size();
				_proximalAfferentLimits.resize(numberOfLimits);
				for ( std::size_t limit = 0; limit < numberOfLimits; limit++ )
					_proximalAfferentLimits[limit][1] = upperLimits[limit];
			}

			STR = "# name: afferentUpdateStep";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_afferentUpdateStep, inputStream);
				check_afferentUpdateStep = true;
			}

			STR = "# name: potentialPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_potentialPercentage, inputStream);
				check_potentialPercentage = true;
			}

		}
	}

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

	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
		_encoderColumns.push_back(ComplexSelfOrganizingMap(inputStream, std::to_string(column)));

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


// this function merges all the column outputs
void	EncoderLayer::mergeOutputs( regularLayerResponse& output )
{
	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	if ( world_size > 1 ) {
		// Get the rank of the process
		std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

		for ( std::size_t root = 0; root < world_size; root++ ) {
			// First of all, computes the number of columns in the root process
			std::size_t	numberOfColumnsInRoot;
			numberOfColumnsInRoot = _columnsDimensionality/world_size;
			if ( root < _columnsDimensionality%world_size )
				numberOfColumnsInRoot++;

			// First, the root process transmits the synchronization in its columns
			// and the non-root processes receive such information
			std::vector<std::size_t>	synchronizationVector;

			if ( world_rank == root ) {
				for ( std::size_t column = root; column < _columnsDimensionality; column=column+world_size ) {
					if ( output.synchronization[column] ) {
						synchronizationVector.push_back(1);
					}
					else {
						synchronizationVector.push_back(0);
					}
				}
			}
			else {
				synchronizationVector.resize(numberOfColumnsInRoot);
			}
			
			synchronizationVector.shrink_to_fit();
			MPI_Bcast(&synchronizationVector[0], 
				  (int)synchronizationVector.size(), 
				  my_MPI_SIZE_T, (int)root, MPI_COMM_WORLD);

			if ( world_rank != root ) {
				for ( std::size_t column = root; column < _columnsDimensionality; column=column+world_size ) {
					if ( synchronizationVector[column/world_size] == 1 ) {
						output.synchronization[column] = true;
					}
					else {
						output.synchronization[column] = false;
					}
				}
			}

			// Then the root process transmits the information in its columns, and
			// the non-root processes receives such information
			std::vector<std::size_t>	informationVector;
			if ( world_rank == root ) {
				for ( std::size_t column = root; column < _columnsDimensionality; column=column+world_size ) {
					if ( output.information[column] ) {
						informationVector.push_back(1);
					}
					else {
						informationVector.push_back(0);
					}
				}
			}
			else {
				informationVector.resize(numberOfColumnsInRoot);
			}
			
			informationVector.shrink_to_fit();
			MPI_Bcast(&informationVector[0], 
				  (int)informationVector.size(), 
				  my_MPI_SIZE_T, (int)root, MPI_COMM_WORLD);
	
			if ( world_rank != root ) {
				for ( std::size_t column = root; column < _columnsDimensionality; column=column+world_size ) {
					if ( informationVector[column/world_size] == 1 ) {
						output.information[column] = true;
					}
					else {
						output.information[column] = false;
					}
				}
			}

			// Finally, the root process transmits the vectors with active indices
			// and the non-root processes receive such information
			std::vector<std::size_t>	lengthsVector;
			if ( world_rank == root ) {
				for ( std::size_t column = root; column < _columnsDimensionality; column=column+world_size )
					lengthsVector.push_back(output.currentIndexes[column].size());
			}
			else {
				lengthsVector.resize(numberOfColumnsInRoot);
			}
	
			lengthsVector.shrink_to_fit();
			MPI_Bcast(&lengthsVector[0], 
				  (int)lengthsVector.size(),
				  my_MPI_SIZE_T, (int)root, MPI_COMM_WORLD);

			if ( lengthsVector.size() != numberOfColumnsInRoot ) {
				std::cout << "\nIn process " << world_rank << "\n"; 
				std::cout << "mergeOutputs inconsistence:\n"; 
				std::cout << "lengthsVector.size(): " << lengthsVector.size() << "\n"; 
				std::cout << "numberOfColumnsInRoot: " << numberOfColumnsInRoot << "\n"; 
				MPI_Abort(MPI_COMM_WORLD,1);
			}

			std::vector<std::size_t>	indexesVector;
			if ( world_rank == root ) {
				for ( std::size_t column = root; column < _columnsDimensionality; column=column+world_size ) {
					indexesVector.insert(indexesVector.end(), 
							     output.currentIndexes[column].begin(),
							     output.currentIndexes[column].end());
				}
			}
			else {
				indexesVector.resize(std::accumulate(lengthsVector.begin(), lengthsVector.end(), 0));
			}
	
			indexesVector.shrink_to_fit();
			MPI_Bcast(&indexesVector[0], 
				  (int)indexesVector.size(),
				  my_MPI_SIZE_T, (int)root, MPI_COMM_WORLD);

			if ( world_rank != root ) {
				// Checks the consistence of the recieved data
				if ( (int)indexesVector.size() != std::accumulate(lengthsVector.begin(), lengthsVector.end(), 0) ) {
					std::cout << "\nIn process " << world_rank << "\n"; 
					std::cout << "mergeOutputs inconsistence:\n"; 
					std::cout << "indexesVector.size(): " << indexesVector.size() << "\n"; 
					std::cout << "std::accumulate(lengthsVector.begin(), lengthsVector.end(), 0): " 
						  <<    std::accumulate(lengthsVector.begin(), lengthsVector.end(), 0) 
						  << "\n"; 
					MPI_Abort(MPI_COMM_WORLD,1);
				}
				      
				for ( std::size_t column = root; column < _columnsDimensionality; column=column+world_size ) {
					if ( output.currentIndexes[column].size() != 0 ) {
						std::cout << "\nIn process " << world_rank << "\n"; 
						std::cout << "mergeOutputs inconsistence:\n"; 
						std::cout << "output.currentIndexes[column].size() != 0\n"; 
						MPI_Abort(MPI_COMM_WORLD,1);
					}
					output.currentIndexes[column].insert(output.currentIndexes[column].end(),
									     indexesVector.begin(),
									     indexesVector.begin()+lengthsVector[column/world_size]);

					indexesVector.erase(indexesVector.begin(), indexesVector.begin()+lengthsVector[column/world_size]);
				}
			}
		}
	}
} // end function mergeOutputs


// merges all connections in rank 0
connectionsSet	EncoderLayer::gatherConnections()
{
	connectionsSet	auxiliaryConnections;
	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	if ( world_size > 1 ) {
		// Get the rank of the process
		std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

		std::vector<std::size_t>	connectionsStructure;
		std::vector<std::size_t>	connections;

		if ( world_rank == 0 ) { // If this is rank 0, then gather all the other rank connections in order to save them
			int	number_amount;
			MPI_Status status;

			if ( _afferentConnections.size() != 0 ) {
				// Allocate memory for afferent connections
				auxiliaryConnections.afferentConnections.resize(_columnsDimensionality);
				// Collects rank 0 afferent connections
				for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
					auxiliaryConnections.afferentConnections[column] = _afferentConnections[column/world_size];
			}

			if ( _lateralDistalConnections.size() != 0 ) {
				// Allocate memory for lateral distal connections
				auxiliaryConnections.lateralDistalConnections.resize(_columnsDimensionality);
				// Collects rank 0 lateral distal connections
				for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
					auxiliaryConnections.lateralDistalConnections[column] = _lateralDistalConnections[column/world_size];
			}

			if ( _apicalConnections.size() != 0 ) {
				// Allocate memory for apical connections
				auxiliaryConnections.apicalConnections.resize(_columnsDimensionality);
				// Collects rank 0 apical connections
				for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
					auxiliaryConnections.apicalConnections[column] = _apicalConnections[column/world_size];
			}

			// Receives connections from other processes
			for ( std::size_t process = 1; process < world_size; process++ ) {
				if ( _afferentConnections.size() != 0 ) {
					// Receives afferent connections from process

					// Probe for an incoming message from process
					MPI_Probe((int)process, 1, MPI_COMM_WORLD, &status);

					// When probe returns, the status object has the size and other
					// attributes of the incoming message. Get the message size
					MPI_Get_count(&status, my_MPI_SIZE_T, &number_amount);

					// Allocate a buffer to hold the incoming data
					std::vector<std::size_t>	connectionsStructure(number_amount);

					// Now receive the message with the allocated buffer
					MPI_Recv(&connectionsStructure[0], number_amount, my_MPI_SIZE_T, 
						 (int)process, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

					std::size_t	numberOfConnections = std::accumulate(connectionsStructure.begin(), 
											      connectionsStructure.end(), 
											      0);

					// Allocate a buffer to hold the incoming data
					std::vector<std::size_t>	connections(numberOfConnections);

					// Receive at most numberOfConnections elements from process
					MPI_Recv(&connections[0], (int)numberOfConnections, 
						 my_MPI_SIZE_T, (int)process, 2, MPI_COMM_WORLD, &status);

					// After receiving the message, check the status to determine
					// how many elements were actually received
					MPI_Get_count(&status, my_MPI_SIZE_T, &number_amount);

					if ( number_amount != (int)numberOfConnections ) {
						std::cout << "\nIn process " << world_rank << "\n"; 
						std::cout << "\nmergeConnections inconsistence:\n"; 
						std::cout << "\nnumber_amount: " << number_amount << "\n"; 
						std::cout << "\nnumberOfConnections: " << numberOfConnections << "\n"; 
						MPI_Abort(MPI_COMM_WORLD,1);
					}

					// Collects in rank 0 afferent connections from process
					for ( std::size_t column = process; column < _columnsDimensionality; column=column+world_size ) {
						if ( auxiliaryConnections.afferentConnections[column].size() != 0 ) {
							std::cout << "\nIn process " << world_rank << "\n"; 
							std::cout << "\nmergeConnections inconsistence:\n"; 
							std::cout << "\nafferentConnections[column].size() != 0\n"; 
							MPI_Abort(MPI_COMM_WORLD,1);
						}
						auxiliaryConnections.afferentConnections[column].insert(auxiliaryConnections.afferentConnections[column].end(),
												   	connections.begin(),
												   	connections.begin()+connectionsStructure[column/world_size]);

						connections.erase(connections.begin(), connections.begin()+connectionsStructure[column/world_size]);
					}

					connectionsStructure.clear();
					connections.clear();
				}
	


				if ( _lateralDistalConnections.size() != 0 ) {
					// Receives lateral distal connections from process

					// Probe for an incoming message from process
					MPI_Probe((int)process, 1, MPI_COMM_WORLD, &status);

					// When probe returns, the status object has the size and other
					// attributes of the incoming message. Get the message size
					MPI_Get_count(&status, my_MPI_SIZE_T, &number_amount);

					// Allocate a buffer to hold the incoming data
					connectionsStructure.resize(number_amount);

					// Now receive the message with the allocated buffer
					MPI_Recv(&connectionsStructure[0], number_amount, my_MPI_SIZE_T, 
						 (int)process, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

					std::size_t	numberOfConnections = std::accumulate(connectionsStructure.begin(), 
											      connectionsStructure.end(), 
											      0);

					// Allocate a buffer to hold the incoming data
					connections.resize(numberOfConnections);

					// Receive at most numberOfConnections elements from process
					MPI_Recv(&connections[0], (int)numberOfConnections, 
						 my_MPI_SIZE_T, (int)process, 2, MPI_COMM_WORLD, &status);

					// After receiving the message, check the status to determine
					// how many elements were actually received
					MPI_Get_count(&status, my_MPI_SIZE_T, &number_amount);

					if ( number_amount != (int)numberOfConnections ) {
						std::cout << "\nIn process " << world_rank << "\n"; 
						std::cout << "\nmergeConnections inconsistence:\n"; 
						std::cout << "\nnumber_amount: " << number_amount << "\n"; 
						std::cout << "\nnumberOfConnections: " << numberOfConnections << "\n"; 
						MPI_Abort(MPI_COMM_WORLD,1);
					}

					// Collects in rank 0 lateral distal connections from process
					for ( std::size_t column = process; column < _columnsDimensionality; column=column+world_size ) {
						if ( auxiliaryConnections.lateralDistalConnections[column].size() != 0 ) {
							std::cout << "\nIn process " << world_rank << "\n"; 
							std::cout << "\nmergeConnections inconsistence:\n"; 
							std::cout << "\nlateralDistalConnections[column].size() != 0\n"; 
							MPI_Abort(MPI_COMM_WORLD,1);
						}
						auxiliaryConnections.lateralDistalConnections[column].insert(auxiliaryConnections.lateralDistalConnections[column].end(),
													     connections.begin(),
													     connections.begin()+connectionsStructure[column/world_size]);

						connections.erase(connections.begin(), connections.begin()+connectionsStructure[column/world_size]);
					}

					connectionsStructure.clear();
					connections.clear();
				}
	


				if ( _apicalConnections.size() != 0 ) {
					// Receives apical connections from process

					// Probe for an incoming message from process
					MPI_Probe((int)process, 1, MPI_COMM_WORLD, &status);

					// When probe returns, the status object has the size and other
					// attributes of the incoming message. Get the message size
					MPI_Get_count(&status, my_MPI_SIZE_T, &number_amount);

					// Allocate a buffer to hold the incoming data
					connectionsStructure.resize(number_amount);

					// Now receive the message with the allocated buffer
					MPI_Recv(&connectionsStructure[0], number_amount, my_MPI_SIZE_T, 
						 (int)process, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

					std::size_t	numberOfConnections = std::accumulate(connectionsStructure.begin(), 
											      connectionsStructure.end(), 
											      0);

					// Allocate a buffer to hold the incoming data
					connections.resize(numberOfConnections);

					// Receive at most numberOfConnections elements from process
					MPI_Recv(&connections[0], (int)numberOfConnections, 
						 my_MPI_SIZE_T, (int)process, 2, MPI_COMM_WORLD, &status);

					// After receiving the message, check the status to determine
					// how many elements were actually received
					MPI_Get_count(&status, my_MPI_SIZE_T, &number_amount);

					if ( number_amount != (int)numberOfConnections ) {
						std::cout << "\nIn process " << world_rank << "\n"; 
						std::cout << "\nmergeConnections inconsistence:\n"; 
						std::cout << "\nnumber_amount: " << number_amount << "\n"; 
						std::cout << "\nnumberOfConnections: " << numberOfConnections << "\n"; 
						MPI_Abort(MPI_COMM_WORLD,1);
					}

					// Collects in rank 0 apical connections from process
					for ( std::size_t column = process; column < _columnsDimensionality; column=column+world_size ) {
						if ( auxiliaryConnections.apicalConnections[column].size() != 0 ) {
							std::cout << "\nIn process " << world_rank << "\n"; 
							std::cout << "\nmergeConnections inconsistence:\n"; 
							std::cout << "\napicalConnections[column].size() != 0\n"; 
							MPI_Abort(MPI_COMM_WORLD,1);
						}
						auxiliaryConnections.apicalConnections[column].insert(auxiliaryConnections.apicalConnections[column].end(),
										 connections.begin(),
										 connections.begin()+connectionsStructure[column/world_size]);

						connections.erase(connections.begin(), connections.begin()+connectionsStructure[column/world_size]);
					}

					connectionsStructure.clear();
					connections.clear();

				}
			}

			// rank 0 merges all connections in auxiliaryConnections
		}
		else { // If this is not rank 0, then it sends its connections to rank 0
			if ( _afferentConnections.size() != 0 ) {
				// Sends afferent connections structure
				for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
					connectionsStructure.push_back(_afferentConnections[column/world_size].size());
					for ( std::size_t connection = 0; connection < _afferentConnections[column/world_size].size(); connection++ )
						connections.push_back(_afferentConnections[column/world_size][connection]);
				}

				MPI_Send(&connectionsStructure[0], 
					 (int)connectionsStructure.size(),
					 my_MPI_SIZE_T, 0, 1, MPI_COMM_WORLD);

				MPI_Send(&connections[0], 
					 (int)connections.size(),
					 my_MPI_SIZE_T, 0, 2, MPI_COMM_WORLD);

				connectionsStructure.clear();
				connections.clear();
			}

			if ( _lateralDistalConnections.size() != 0 ) {
				// Sends lateral distal connections structure
				for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
					connectionsStructure.push_back(_lateralDistalConnections[column/world_size].size());
					for ( std::size_t connection = 0; connection < _lateralDistalConnections[column/world_size].size(); connection++ )
						connections.push_back(_lateralDistalConnections[column/world_size][connection]);
				}

				MPI_Send(&connectionsStructure[0], 
					 (int)connectionsStructure.size(),
					 my_MPI_SIZE_T, 0, 1, MPI_COMM_WORLD);

				MPI_Send(&connections[0], 
					 (int)connections.size(),
					 my_MPI_SIZE_T, 0, 2, MPI_COMM_WORLD);

				connectionsStructure.clear();
				connections.clear();
			}
			
			if ( _apicalConnections.size() != 0 ) {
				// Sends apical connections structure
				for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
					connectionsStructure.push_back(_apicalConnections[column/world_size].size());
					for ( std::size_t connection = 0; connection < _apicalConnections[column/world_size].size(); connection++ )
						connections.push_back(_apicalConnections[column/world_size][connection]);
				}

				MPI_Send(&connectionsStructure[0], 
					 (int)connectionsStructure.size(),
					 my_MPI_SIZE_T, 0, 1, MPI_COMM_WORLD);

				MPI_Send(&connections[0], 
					 (int)connections.size(),
					 my_MPI_SIZE_T, 0, 2, MPI_COMM_WORLD);

				connectionsStructure.clear();
				connections.clear();
			}
		}
	}
	else { // if there is just one process
		auxiliaryConnections.afferentConnections = _afferentConnections;
		auxiliaryConnections.lateralDistalConnections = _lateralDistalConnections;
		auxiliaryConnections.apicalConnections = _apicalConnections;
	}

	return	auxiliaryConnections;
} // end function gatherConnections
