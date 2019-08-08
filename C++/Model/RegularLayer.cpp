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
#include <mpi.h>

#include "../Libraries/Model/Random.h"
#include "../Libraries/Model/Topology.h"
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/Templates.h"
#include "../Libraries/Model/OctaveInterface.h"
#include "../Libraries/Model/MatlabInterface.h"
#include "../Libraries/Model/GlobalVariables.h"
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
			std::cout << "\nRegularLayer inconsistence: numberOfColumns < world_size\n";

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	if ( world_rank == 0 )
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
						   structure.proximalPotentialPercentage,
						   structure.distalPotentialPercentage);

	//MPI_Barrier(MPI_COMM_WORLD);

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

	//MPI_Barrier(MPI_COMM_WORLD);

	RegularLayer::generateColumns(structure.populationsArrayDimensionality,
				      structure.afferentPopulationsArrayDimensionality,
				      structure.apicalPopulationsArrayDimensionality,
				      structure.temporalGatheringAfferentValue,
				      structure.proximalPotentialPercentage,
				      structure.distalPotentialPercentage);

	//MPI_Barrier(MPI_COMM_WORLD);

	_temporalGatheringAfferentValue = structure.temporalGatheringAfferentValue;

	if ( _temporalGatheringAfferentValue > 1 )
		RegularLayer::initializeInternalTemporallyGatheredInputs();

	//MPI_Barrier(MPI_COMM_WORLD);
	
	RegularLayer::initializeWiredVariables();
} // end function layerInitializer


//this function initializes wired variables
void	RegularLayer::initializeWiredVariables()
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
	       				     const double proximalPotentialPercentage, 
	       				     const double distalPotentialPercentage )
{
	bool	error_flag = false;

	error_flag &= RegularLayer::validateColumnsInterconnectionsParameters(afferentArrayDimensionality,
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

	error_flag &= RegularLayer::validatePopulationParameters(populationsArrayDimensionality,
								 afferentPopulationsArrayDimensionality,
								 apicalPopulationsArrayDimensionality,
								 temporalGatheringAfferentValue,
								 proximalPotentialPercentage,
								 distalPotentialPercentage);

	if ( error_flag )
		MPI_Abort(MPI_COMM_WORLD,1);

	//MPI_Barrier(MPI_COMM_WORLD);
} // end function validateRegularLayer


// validates the parameters to configure the columns' interconnections
bool	RegularLayer::validateColumnsInterconnectionsParameters( const std::vector<std::size_t>& afferentArrayDimensionality,
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
	bool	error_flag = false;

	if ( temporalGatheringAfferentValue == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validateColumnsInterconnectionsParameters\n";
		std::cout << "temporalGatheringAfferentValue = " << temporalGatheringAfferentValue << "\n";
		error_flag = true;
	}

	if ( afferentArrayDimensionality.size() == 0 ||
	     apicalArrayDimensionality.size() == 0 ||
	     columnsArrayDimensionality.size() == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validateColumnsInterconnectionsParameters\n";
		std::cout << "afferentArrayDimensionality.size() = " << afferentArrayDimensionality.size() << "\n";
		std::cout << "apicalArrayDimensionality.size() = " << apicalArrayDimensionality.size() << "\n";
		std::cout << "columnsArrayDimensionality.size() = " << columnsArrayDimensionality.size() << "\n";
		error_flag = true;
	}

	for ( std::size_t dim = 0; dim < afferentArrayDimensionality.size(); dim++ ) {
		if ( afferentArrayDimensionality[dim] == 0 ) {
			std::cout << "RegularLayer object construction inconsistence: \n";
			std::cout << "In function validateColumnsInterconnectionsParameters\n";
			std::cout << "afferentArrayDimensionality[" << dim << "] = " << afferentArrayDimensionality[dim] << "\n";
			error_flag = true;
		}
	}

	for ( std::size_t dim = 0; dim < apicalArrayDimensionality.size(); dim++ ) {
		if ( apicalArrayDimensionality[dim] == 0 ) {
			std::cout << "RegularLayer object construction inconsistence: \n";
			std::cout << "In function validateColumnsInterconnectionsParameters\n";
			std::cout << "apicalArrayDimensionality[" << dim << "] = " << apicalArrayDimensionality[dim] << "\n";
			error_flag = true;
		}
	}

	for ( std::size_t dim = 0; dim < columnsArrayDimensionality.size(); dim++ ) {
		if ( columnsArrayDimensionality[dim] == 0 ) {
			std::cout << "RegularLayer object construction inconsistence: \n";
			std::cout << "In function validateColumnsInterconnectionsParameters\n";
			std::cout << "columnsArrayDimensionality[" << dim << "] = " << columnsArrayDimensionality[dim] << "\n";
			error_flag = true;
		}
	}

	if ( afferentReceptiveField.size() == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validateColumnsInterconnectionsParameters\n";

		std::cout << "afferentReceptiveField.size() = " << afferentReceptiveField.size() << "\n";
		error_flag = true;
	}

	if ( (afferentArrayDimensionality.size() != afferentReceptiveField.size()) ||
	     (apicalReceptiveField.size() != 0 && apicalArrayDimensionality.size() != apicalReceptiveField.size()) ||
	     (lateralProximalReceptiveField.size() != 0 && columnsArrayDimensionality.size() != lateralProximalReceptiveField.size()) ||
	     (lateralDistalReceptiveField.size() != 0 && columnsArrayDimensionality.size() != lateralDistalReceptiveField.size()) ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validateColumnsInterconnectionsParameters\n";

		std::cout << "afferentArrayDimensionality.size() = " << afferentArrayDimensionality.size() << "\n";
		std::cout << "while, afferentReceptiveField.size() = " << afferentReceptiveField.size() << "\n";

		std::cout << "apicalArrayDimensionality.size() = " << apicalArrayDimensionality.size() << "\n";
		std::cout << "while, apicalReceptiveField.size() = " << apicalReceptiveField.size() << "\n";

		std::cout << "columnsArrayDimensionality.size() = " << columnsArrayDimensionality.size() << "\n";
		std::cout << "while, lateralProximalReceptiveField.size() = " << lateralProximalReceptiveField.size() << "\n";
		std::cout << "and lateralDistalReceptiveField.size() = " << lateralDistalReceptiveField.size() << "\n";
		error_flag = true;
	}

	for ( std::size_t dim = 0; dim < afferentReceptiveField.size(); dim++ ) {
		if (afferentArrayDimensionality[dim] % 2) {
			if ( afferentReceptiveField[dim] > afferentArrayDimensionality[dim]/2 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "afferentReceptiveField[" << dim << "] = " << afferentReceptiveField[dim] << "\n";
				std::cout << "while afferentArrayDimensionality[" << dim << "] is = "
				          << afferentArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
		else {
			if ( afferentReceptiveField[dim] > afferentArrayDimensionality[dim]/2-1 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
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
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "apicalReceptiveField[" << dim << "] = " << apicalReceptiveField[dim] << "\n";
				std::cout << "while apicalArrayDimensionality[" << dim << "] is = "
				          << apicalArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
		else {
			if ( apicalReceptiveField[dim] > apicalArrayDimensionality[dim]/2-1 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "apicalReceptiveField[" << dim << "] = " << apicalReceptiveField[dim] << "\n";
				std::cout << "while apicalArrayDimensionality[" << dim << "] is = "
				          << apicalArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
	}

	for ( std::size_t dim = 0; dim < lateralProximalReceptiveField.size(); dim++ ) {
		if (columnsArrayDimensionality[dim] % 2) {
			if ( lateralProximalReceptiveField[dim] > columnsArrayDimensionality[dim]/2 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "lateralProximalReceptiveField[" << dim << "] = " << lateralProximalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
		else {
			if ( lateralProximalReceptiveField[dim] > columnsArrayDimensionality[dim]/2-1 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "lateralProximalReceptiveField[" << dim << "] = " << lateralProximalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
	}

	for ( std::size_t dim = 0; dim < lateralDistalReceptiveField.size(); dim++ ) {
		if (columnsArrayDimensionality[dim] % 2) {
			if ( lateralDistalReceptiveField[dim] > columnsArrayDimensionality[dim]/2 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "lateralDistalReceptiveField[" << dim << "] = " << lateralDistalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
		else {
			if ( lateralDistalReceptiveField[dim] > columnsArrayDimensionality[dim]/2-1 ) {
				std::cout << "RegularLayer object construction inconsistence: \n";
				std::cout << "In function validateColumnsInterconnectionsParameters\n";
				std::cout << "lateralDistalReceptiveField[" << dim << "] = " << lateralDistalReceptiveField[dim] << "\n";
				std::cout << "while columnsArrayDimensionality[" << dim << "] is = "
				          << columnsArrayDimensionality[dim] << "\n";
				error_flag = true;
			}
		}
	}

	if ( afferentPercentage < 0.0 || afferentPercentage > 1.0 ||
	     lateralProximalPercentage < 0.0 || lateralProximalPercentage > 1.0 ||
	     lateralDistalPercentage < 0.0 || lateralDistalPercentage > 1.0 ||
	     apicalPercentage < 0.0 || apicalPercentage > 1.0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validateColumnsInterconnectionsParameters\n";
		std::cout << "afferentPercentage = " << afferentPercentage << "\n";
		std::cout << "lateralProximalPercentage = " << lateralProximalPercentage << "\n";
		std::cout << "lateralDistalPercentage = " << lateralDistalPercentage << "\n";
		std::cout << "apicalPercentage = " << apicalPercentage << "\n";
		error_flag = true;
	}

	return	error_flag;
} // end function validateColumnsInterconnectionsParameters


// validates the parameters used to generate the columns' populations
bool	RegularLayer::validatePopulationParameters( const std::vector<std::size_t>& populationsArrayDimensionality,
						    const std::vector<std::size_t>& afferentPopulationsArrayDimensionality,
						    const std::vector<std::size_t>& apicalPopulationsArrayDimensionality,
						    const std::size_t temporalGatheringAfferentValue,
	       					    const double proximalPotentialPercentage, 
	       					    const double distalPotentialPercentage )
{
	bool	error_flag = false;

	if ( proximalPotentialPercentage < 0.0 || proximalPotentialPercentage > 1.0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "proximalPotentialPercentage = "
			  << proximalPotentialPercentage << "\n";
		error_flag = true;
	}

	if ( distalPotentialPercentage < 0.0 || distalPotentialPercentage > 1.0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "distalPotentialPercentage = "
			  << distalPotentialPercentage << "\n";
		error_flag = true;
	}

	if ( temporalGatheringAfferentValue == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "temporalGatheringAfferentValue = "
			  << temporalGatheringAfferentValue << "\n";
		error_flag = true;
	}

	if ( afferentPopulationsArrayDimensionality.size() == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "afferentPopulationsArrayDimensionality.size() = "
			  << afferentPopulationsArrayDimensionality.size() << "\n";
		error_flag = true;
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
				error_flag = true;
			}
		}
	}

	if ( apicalPopulationsArrayDimensionality.size() == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "apicalPopulationsArrayDimensionality.size() = "
			  << apicalPopulationsArrayDimensionality.size() << "\n";
		error_flag = true;
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
				error_flag = true;
			}
		}
	}

	if ( populationsArrayDimensionality.size() == 0 ) {
		std::cout << "RegularLayer object construction inconsistence: \n";
		std::cout << "In function validatePopulationParameters\n";
		std::cout << "populationsArrayDimensionality.size() = "
			  << populationsArrayDimensionality.size() << "\n";
		error_flag = true;
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
				error_flag = true;
			}
		}
	}

	return	error_flag;
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
			std::cout << "\ninterconnectRegularLayerColumns inconsistence: afferentReceptiveField.size() == 0\n";

		//MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD,1);
	}

	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
		_afferentConnections.push_back(RegularLayer::getAfferentInputs(column));

	_afferentConnections.shrink_to_fit();

	_lateralProximalReceptiveField = lateralProximalReceptiveField;
	_lateralProximalPercentage = lateralProximalPercentage;
	_lateralProximalWrapAround = lateralProximalWrapAround;

	if ( lateralProximalReceptiveField.size() != 0 ) {
		for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
			_lateralProximalConnections.push_back(RegularLayer::getLateralProximalInputs(column));
	}

	_lateralProximalConnections.shrink_to_fit();

	_lateralDistalReceptiveField = lateralDistalReceptiveField;
	_lateralDistalPercentage = lateralDistalPercentage;
	_lateralDistalWrapAround = lateralDistalWrapAround;

	if ( lateralDistalReceptiveField.size() != 0 ) {
		for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
			_lateralDistalConnections.push_back(RegularLayer::getLateralDistalInputs(column));
	}

	_lateralDistalConnections.shrink_to_fit();

	_apicalReceptiveField = apicalReceptiveField;
	_apicalPercentage = apicalPercentage;
	_apicalWrapAround = apicalWrapAround;

	if ( apicalReceptiveField.size() != 0 ) {
		for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
			_apicalConnections.push_back(RegularLayer::getApicalInputs(column));
	}

	_apicalConnections.shrink_to_fit();
} // end function interconnectRegularLayerColumns


// generates a population for every column in the layer
void	RegularLayer::generateColumns( const std::vector<std::size_t>& populationsArrayDimensionality,
				       const std::vector<std::size_t>& afferentPopulationsArrayDimensionality,
				       const std::vector<std::size_t>& apicalPopulationsArrayDimensionality,
				       const std::size_t temporalGatheringAfferentValue,
	       			       const double proximalPotentialPercentage, 
	       			       const double distalPotentialPercentage )
{
	//std::size_t			numberOfInputs;
	std::vector<std::size_t>	proximalConnectivity, dynamicUnits;
	std::vector<double>		alinearityFactors;

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

	auto	afferentPopulationDimensionality = std::accumulate(afferentPopulationsArrayDimensionality.begin(),
								   afferentPopulationsArrayDimensionality.end(),
								   1, std::multiplies<std::size_t>());

	std::size_t	afferentPotentialDimensionality = proximalPotentialPercentage*afferentPopulationDimensionality;

	std::vector<double>	afferentFactors(afferentPopulationsArrayDimensionality.begin(), afferentPopulationsArrayDimensionality.end());
	std::transform(afferentFactors.begin(), afferentFactors.end(), afferentFactors.begin(), std::bind1st(std::multiplies<double>(), 0.25));

	auto	populationDimensionality = std::accumulate(populationsArrayDimensionality.begin(),
							   populationsArrayDimensionality.end(),
							   1, std::multiplies<std::size_t>());

	std::size_t	potentialDimensionality = proximalPotentialPercentage*populationDimensionality;

	std::vector<double>	factors(populationsArrayDimensionality.begin(), populationsArrayDimensionality.end());
	std::transform(factors.begin(), factors.end(), factors.begin(), std::bind1st(std::multiplies<double>(), 0.25));

	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
		//numberOfInputs = 0;

		for ( std::size_t afferentInput = 0; afferentInput < _afferentConnections[column/world_size].size(); afferentInput++ ) {
			std::vector<std::size_t>	inputRange(afferentPopulationDimensionality);
			std::iota(std::begin(inputRange), std::end(inputRange), 0);
			std::vector<std::size_t>	potentialConnections;
			sample_vector(inputRange, potentialConnections, afferentPotentialDimensionality);
			assert(!is_there_duplicate(potentialConnections));

			for ( auto& potentialConnection : potentialConnections ) {
				auto	partialConnectivity = unravelIndex(potentialConnection,afferentPopulationsArrayDimensionality);
				proximalConnectivity.insert(std::end(proximalConnectivity), std::begin(partialConnectivity), std::end(partialConnectivity));
				alinearityFactors.insert(std::end(alinearityFactors), std::begin(afferentFactors), std::end(afferentFactors));
			}
		}

		//numberOfInputs *= temporalGatheringAfferentValue;

		if ( _lateralProximalConnections.size() > 0 ) {
			for ( std::size_t lateralProximalInput = 0;
					  lateralProximalInput < _lateralProximalConnections[column/world_size].size();
					  lateralProximalInput++ ) {
				std::vector<std::size_t>	inputRange(populationDimensionality);
				std::iota(std::begin(inputRange), std::end(inputRange), 0);
				std::vector<std::size_t>	potentialConnections;
				sample_vector(inputRange, potentialConnections, potentialDimensionality);
				assert(!is_there_duplicate(potentialConnections));

				for ( auto& potentialConnection : potentialConnections ) {
					auto	partialConnectivity = unravelIndex(potentialConnection,populationsArrayDimensionality);
					proximalConnectivity.insert(std::end(proximalConnectivity), std::begin(partialConnectivity), std::end(partialConnectivity));
					alinearityFactors.insert(std::end(alinearityFactors), std::begin(factors), std::end(factors));
				}
			}
		}

		if ( _apicalConnections.size() > 0 )
			for ( std::size_t link = 0; link < _apicalConnections[column/world_size].size(); link++ ) {
				auto	auxiliary = std::accumulate(apicalPopulationsArrayDimensionality.begin(),
									 apicalPopulationsArrayDimensionality.end(),
									 1, std::multiplies<std::size_t>());
				dynamicUnits.push_back(auxiliary);
			}

		if ( _lateralDistalConnections.size() > 0 )
			for ( std::size_t link = 0; link < _lateralDistalConnections[column/world_size].size(); link++ ) {
				auto	auxiliary = std::accumulate(populationsArrayDimensionality.begin(),
									 populationsArrayDimensionality.end(),
									 1, std::multiplies<std::size_t>());
				dynamicUnits.push_back(auxiliary);
			}

		dynamicUnits.shrink_to_fit();
		std::array<double,2>	weightLimits = {-1,1};
		//std::array<double,2>	weightLimits = {SYNAPTIC_DECREMENT,SYNAPTIC_INCREMENT};
		std::vector<double>	auxiliary(proximalConnectivity.begin(), proximalConnectivity.end());
		_layerColumns.push_back(ComplexProcessor(populationsArrayDimensionality,
							 auxiliary,
							 alinearityFactors,
							 proximalPotentialPercentage,
							 distalPotentialPercentage,
							 WEIGHTS_SPARSITY,
							 dynamicUnits,
							 weightLimits));
		proximalConnectivity.clear();
		alinearityFactors.clear();
		dynamicUnits.clear();
	}
	_layerColumns.shrink_to_fit();

	_populationsArrayDimensionality = populationsArrayDimensionality;
	_afferentPopulationsArrayDimensionality = afferentPopulationsArrayDimensionality;
	_apicalPopulationsArrayDimensionality = apicalPopulationsArrayDimensionality;
	_proximalPotentialPercentage = proximalPotentialPercentage;
	_distalPotentialPercentage = distalPotentialPercentage;
} // end function generateColumns


// initializes the internal temporal gathered inputs
void	RegularLayer::initializeInternalTemporallyGatheredInputs()
{
	_internalTemporallyGatheredInputs.temporallyGatheredIndexes.resize(_temporalGatheringAfferentValue);
	_internalTemporallyGatheredInputs.temporallyGatheredInformation.resize(_temporalGatheringAfferentValue);
	_internalTemporallyGatheredInputs.synchronization.resize(_temporalGatheringAfferentValue);

	for ( std::size_t timeStep = 0; timeStep < _temporalGatheringAfferentValue; timeStep++ ) {
		_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep].resize(_afferentDimensionality);

		auto	it = _internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].begin();
		_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].insert(it,_afferentDimensionality,false);

		it = _internalTemporallyGatheredInputs.synchronization[timeStep].begin();
		_internalTemporallyGatheredInputs.synchronization[timeStep].insert(it,_afferentDimensionality,true);

	}
} // end function initializeInternalTemporallyGatheredInputs


// computes the layer response
regularLayerResponse	RegularLayer::computeResponse( const regularLayerResponse& afferent,
						       const regularLayerParameters& parameters )
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

	regularLayerResponse	temporalGatheredAfferent;
	if ( _temporalGatheringAfferentValue > 1 )
		temporalGatheredAfferent = RegularLayer::temporalGatherer(afferent);

	#pragma omp parallel for default(none) shared(afferent, temporalGatheredAfferent, parameters, output, world_rank, world_size) num_threads(9)
	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
		regularLayerProximalInput	proximalInputs;
		if ( _temporalGatheringAfferentValue > 1 ) {
			proximalInputs = RegularLayer::gatherProximalInputs(column/world_size,
									    temporalGatheredAfferent,
									    _lateral);
		}
		else {
			proximalInputs = RegularLayer::gatherProximalInputs(column/world_size, afferent, _lateral);
		}

		auto	distalInputs = RegularLayer::gatherDistalInputs(column/world_size, _apical, _lateral);

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
						response = _layerColumns[column/world_size].learningRule(parameters.learning.proximalLearningRate,
											      parameters.learning.proximalNeighborhood,
											      parameters.learning.plasticity,
											      proximalInputs.inputs,
											      PROXIMAL_SYNAPTIC_THRESHOLD,
											      parameters.activationHomeostasis,
											      true);
						
						_layerColumns[column/world_size].homeostasis(true,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					else {
						response = _layerColumns[column/world_size].getResponse(proximalInputs.inputs);

						_layerColumns[column/world_size].homeostasis(false,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
											       _populationsArrayDimensionality.end(),
											       1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column/world_size].Activate(response,
										     distalInputs.activeIndexes,
										     numberOfExcitedUnits,
										     parameters.sparsity,
										     true);

					if ( parameters.learning.enableDistalLearning ) {
						if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
						  			  std::accumulate(_populationsArrayDimensionality.begin(),
											  _populationsArrayDimensionality.end(), 1,
											  std::multiplies<std::size_t>()) ) {
							_layerColumns[column/world_size].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     //parameters.learning.distalLearningRate);
										     parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_layerColumns[column/world_size].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     //parameters.learning.distalLearningRate*BUSTING);
										     parameters.learning.distalLearningRate);
						}
						
						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveUnits = _lateral.currentIndexes[column]; 
							_layerColumns[column/world_size].Update(lastActiveUnits,
										     distalInputs.activeIndexes,
										     false, DISTAL_SYNAPTIC_THRESHOLD,
										     //parameters.learning.distalLearningRate*BUSTING);
										     parameters.learning.distalLearningRate);
						}						
					}
				}
				else {
					response = _layerColumns[column/world_size].getResponse(proximalInputs.inputs);
					
					_layerColumns[column/world_size].homeostasis(false,
									  parameters.learning.synapticHomeostasis,
									  parameters.activationHomeostasis,
									  PROXIMAL_SYNAPTIC_THRESHOLD);
					
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
											       _populationsArrayDimensionality.end(),
											       1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column/world_size].Activate(response,
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

	RegularLayer::mergeOutputs(output);

	return	output;
} // end function computeResponse


// computes the layer response
regularLayerResponse	RegularLayer::computeResponse( const regularLayerResponse& afferent,
						       const regularLayerResponse& lateral,
						       const regularLayerParameters& parameters )
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

	regularLayerResponse	temporalGatheredAfferent;
	if ( _temporalGatheringAfferentValue > 1 )
		temporalGatheredAfferent = RegularLayer::temporalGatherer(afferent);

	#pragma omp parallel for default(none) shared(afferent, temporalGatheredAfferent, lateral, parameters, output, world_rank, world_size) num_threads(9)
	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
		regularLayerProximalInput	proximalInputs;
		if ( _temporalGatheringAfferentValue > 1 ) {
			proximalInputs = RegularLayer::gatherProximalInputs(column/world_size,
									    temporalGatheredAfferent,
									    lateral);
		}
		else {
			proximalInputs = RegularLayer::gatherProximalInputs(column/world_size, afferent, lateral);
		}

		auto	distalInputs = RegularLayer::gatherDistalInputs(column/world_size, _apical, lateral);

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
						response = _layerColumns[column/world_size].learningRule(parameters.learning.proximalLearningRate,
											      parameters.learning.proximalNeighborhood,
											      parameters.learning.plasticity,
											      proximalInputs.inputs,
											      PROXIMAL_SYNAPTIC_THRESHOLD,
											      parameters.activationHomeostasis,
											      true);
						
						_layerColumns[column/world_size].homeostasis(true,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					else {
						response = _layerColumns[column/world_size].getResponse(proximalInputs.inputs);

						_layerColumns[column/world_size].homeostasis(false,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
											       _populationsArrayDimensionality.end(),
											       1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column/world_size].Activate(response,
										     distalInputs.activeIndexes,
										     numberOfExcitedUnits,
										     parameters.sparsity,
										     true);

					if ( parameters.learning.enableDistalLearning ) {
						if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
						  			  std::accumulate(_populationsArrayDimensionality.begin(),
											  _populationsArrayDimensionality.end(), 1,
											  std::multiplies<std::size_t>()) ) {
							_layerColumns[column/world_size].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     //parameters.learning.distalLearningRate);
										     parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_layerColumns[column/world_size].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     //parameters.learning.distalLearningRate*BUSTING);
										     parameters.learning.distalLearningRate);
						}
						
						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveUnits = lateral.currentIndexes[column]; 
							_layerColumns[column/world_size].Update(lastActiveUnits,
										     distalInputs.activeIndexes,
										     false, DISTAL_SYNAPTIC_THRESHOLD,
										     //parameters.learning.distalLearningRate*BUSTING);
										     parameters.learning.distalLearningRate);
						}						
					}
				}
				else {
					response = _layerColumns[column/world_size].getResponse(proximalInputs.inputs);
					
					_layerColumns[column/world_size].homeostasis(false,
									  parameters.learning.synapticHomeostasis,
									  parameters.activationHomeostasis,
									  PROXIMAL_SYNAPTIC_THRESHOLD);
					
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
											       _populationsArrayDimensionality.end(),
											       1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column/world_size].Activate(response,
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

	RegularLayer::mergeOutputs(output);

	return	output;
} // end function computeResponse


// computes the layer response
regularLayerResponse	RegularLayer::computeResponse( const regularLayerResponse& afferent,
						       const regularLayerResponse& lateral,
					       	       const regularLayerResponse& apical,
						       const regularLayerParameters& parameters )
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

	regularLayerResponse	temporalGatheredAfferent;
	if ( _temporalGatheringAfferentValue > 1 )
		temporalGatheredAfferent = RegularLayer::temporalGatherer(afferent);

	#pragma omp parallel for default(none) shared(afferent, temporalGatheredAfferent, lateral, apical, parameters, output, world_rank, world_size) num_threads(9)
	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
		regularLayerProximalInput	proximalInputs;
		if ( _temporalGatheringAfferentValue > 1 ) {
			proximalInputs = RegularLayer::gatherProximalInputs(column/world_size,
									    temporalGatheredAfferent,
									    lateral);
		}
		else {
			proximalInputs = RegularLayer::gatherProximalInputs(column/world_size, afferent, lateral);
		}

		auto	distalInputs = RegularLayer::gatherDistalInputs(column/world_size, apical, lateral);

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
						response = _layerColumns[column/world_size].learningRule(parameters.learning.proximalLearningRate,
											      parameters.learning.proximalNeighborhood,
											      parameters.learning.plasticity,
											      proximalInputs.inputs,
											      PROXIMAL_SYNAPTIC_THRESHOLD,
											      parameters.activationHomeostasis,
											      true);
						
						_layerColumns[column/world_size].homeostasis(true,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					else {
						response = _layerColumns[column/world_size].getResponse(proximalInputs.inputs);

						_layerColumns[column/world_size].homeostasis(false,
										  parameters.learning.synapticHomeostasis,
										  parameters.activationHomeostasis,
										  PROXIMAL_SYNAPTIC_THRESHOLD);
					}
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
											       _populationsArrayDimensionality.end(),
											       1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column/world_size].Activate(response,
										     distalInputs.activeIndexes,
										     numberOfExcitedUnits,
										     parameters.sparsity,
										     true);

					if ( parameters.learning.enableDistalLearning ) {
						if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
						  			  std::accumulate(_populationsArrayDimensionality.begin(),
											  _populationsArrayDimensionality.end(), 1,
											  std::multiplies<std::size_t>()) ) {
							_layerColumns[column/world_size].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     //parameters.learning.distalLearningRate);
										     parameters.learning.distalLearningRate*BUSTING);
						}
						else {
							_layerColumns[column/world_size].Update(activeIndexes,
										     distalInputs.activeIndexes,
										     true, DISTAL_SYNAPTIC_THRESHOLD,
										     //parameters.learning.distalLearningRate*BUSTING);
										     parameters.learning.distalLearningRate);
						}
						
						if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
							auto	lastActiveUnits = lateral.currentIndexes[column]; 
							_layerColumns[column/world_size].Update(lastActiveUnits,
										     distalInputs.activeIndexes,
										     false, DISTAL_SYNAPTIC_THRESHOLD,
										     //parameters.learning.distalLearningRate*BUSTING);
										     parameters.learning.distalLearningRate);
						}						
					}
				}
				else {
					response = _layerColumns[column/world_size].getResponse(proximalInputs.inputs);
					
					_layerColumns[column/world_size].homeostasis(false,
									  parameters.learning.synapticHomeostasis,
									  parameters.activationHomeostasis,
									  PROXIMAL_SYNAPTIC_THRESHOLD);
					
					std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
									       std::accumulate(_populationsArrayDimensionality.begin(),
											       _populationsArrayDimensionality.end(),
											       1, std::multiplies<std::size_t>());	

					activeIndexes = _layerColumns[column/world_size].Activate(response,
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

	RegularLayer::mergeOutputs(output);

	return	output;
} // end function computeResponse


// temporally gathers afferent inputs
regularLayerResponse	RegularLayer::temporalGatherer( const regularLayerResponse& input )
{
	assert(_temporalGatheringAfferentValue > 1);

	assert( input.currentIndexes.size() > 0 &&
		input.currentIndexes.size() == input.synchronization.size() &&
		input.currentIndexes.size() == input.information.size());

	regularLayerResponse	output;
	output.currentIndexes.resize(input.currentIndexes.size());
	output.synchronization.resize(input.synchronization.size());
	output.information.resize(input.information.size());

	for ( int timeStep = (int)_temporalGatheringAfferentValue-1; timeStep >= 0; timeStep-- ) {
		if (timeStep == 0) {
			_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep] = input.currentIndexes;
			_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep] = input.information;
			_internalTemporallyGatheredInputs.synchronization[timeStep] = input.synchronization;

			for ( std::size_t column = 0; column < output.currentIndexes.size(); column++ )
				output.currentIndexes[column].insert(std::end(output.currentIndexes[column]),
								     std::begin(_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep][column]),
								     std::end(_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep][column]));

			std::vector<bool>      aux;
			std::transform(output.synchronization.begin(),
				       output.synchronization.end(),
				       _internalTemporallyGatheredInputs.synchronization[timeStep].begin(),
				       std::back_inserter(aux),
				       std::logical_or<bool>());
			aux.shrink_to_fit();
			output.synchronization=aux;

			aux.clear();
			std::transform(output.information.begin(),
				       output.information.end(),
				       _internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].begin(),
				       std::back_inserter(aux),
				       std::logical_or<bool>());
			aux.shrink_to_fit();
			output.information=aux;
		}
		else {
			_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep] =
			_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep-1];
			_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep] =
			_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep-1];
			_internalTemporallyGatheredInputs.synchronization[timeStep] =
			_internalTemporallyGatheredInputs.synchronization[timeStep-1];

			for ( std::size_t column = 0; column < output.currentIndexes.size(); column++ )
				output.currentIndexes[column].insert(std::end(output.currentIndexes[column]),
								     std::begin(_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep][column]),
								     std::end(_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep][column]));

			std::vector<bool>      aux;
			std::transform(output.synchronization.begin(),
				       output.synchronization.end(),
				       _internalTemporallyGatheredInputs.synchronization[timeStep].begin(),
				       std::back_inserter(aux),
				       std::logical_or<bool>());
			aux.shrink_to_fit();
			output.synchronization=aux;

			aux.clear();
			std::transform(output.information.begin(),
				       output.information.end(),
				       _internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].begin(),
				       std::back_inserter(aux),
				       std::logical_or<bool>());
			aux.shrink_to_fit();
			output.information=aux;
		}
	}

	return	output;








	//regularLayerTemporallyGatheredResponse	output;

	//bool	synchronization = !std::all_of(input.synchronization.begin(),
					       //input.synchronization.end(),
					       //[](bool v) { return !v; });

	//// if the input has some synchronization activity
	//if ( synchronization ) {
		//// iterates through the afferent input layer columns
		//for ( std::size_t column = 0; column < _afferentDimensionality; column++ ) {
			//// if there is synchronization activity in this afferent column
			//if ( input.synchronization[column] ) {
				//// if it is the first internal time step
				//if ( _temporalPointer[column] == 0 ) {
					//for ( std::size_t timeStep = 0; timeStep < _temporalGatheringAfferentValue; timeStep++ ) {
						//_internalTemporallyGatheredInputs.temporallyGatheredIndexes[timeStep][column].clear();
						//_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep][column] = false;
					//}
					//_internalTemporallyGatheredInputs.synchronization[column] = false;
				//}
				//// if there is information comming from this afferent column
				//if ( input.information[column] ) {
					//_internalTemporallyGatheredInputs.temporallyGatheredIndexes[_temporalPointer[column]][column] =
												//input.currentIndexes[column];
					//_internalTemporallyGatheredInputs.temporallyGatheredInformation[_temporalPointer[column]][column] = true;
					//_temporalPointer[column]++;
					//_temporalPointer[column] = positiveMod(_temporalPointer[column],_temporalGatheringAfferentValue);
					//if ( _temporalPointer[column] == 0 )
						//_internalTemporallyGatheredInputs.synchronization[column] = true;
					//else
						//_internalTemporallyGatheredInputs.synchronization[column] = false;
				//}
				//// if there is no information comming from this afferent column
				//else {
					//assert(input.currentIndexes[column].size() == 0);
					//_temporalPointer[column] = 0;
					//_internalTemporallyGatheredInputs.synchronization[column] = true;
				//}
			//}
			//// if there is no synchronization activity in this afferent column
			//else {
				//_internalTemporallyGatheredInputs.synchronization[column] = false;
			//}
		//}
		//return	_internalTemporallyGatheredInputs;
	//}
	//// if there is no synchronization activity from the input
	//else {
		//std::fill (_internalTemporallyGatheredInputs.synchronization.begin(),
			   //_internalTemporallyGatheredInputs.synchronization.end(),false);

		//return	_internalTemporallyGatheredInputs;
	//}
} // end function temporalGatherer


// gathers proximal inputs from afferent and lateral connections
regularLayerProximalInput	RegularLayer::gatherProximalInputs( const std::size_t column,
								    const regularLayerResponse& afferent,
								    const regularLayerResponse& lateral )
{
	regularLayerProximalInput	proximalInputs;

	std::size_t	offset = 0;
	assert(afferent.currentIndexes.size() != 0);
	//assert(_temporalGatheringAfferentValue == 1);

	assert(_afferentConnections.size() != 0);
	std::size_t	numberOfProximalConnections = _afferentConnections[column].size();

	if ( _lateralProximalConnections.size() != 0 )
		numberOfProximalConnections += _lateralProximalConnections[column].size();

	proximalInputs.synchronization.resize(numberOfProximalConnections);
	proximalInputs.information.resize(numberOfProximalConnections);

	auto	afferentPopulationDimensionality = std::accumulate(_afferentPopulationsArrayDimensionality.begin(),
								   _afferentPopulationsArrayDimensionality.end(),
								   1, std::multiplies<std::size_t>());

	std::size_t	afferentPotentialDimensionality = _proximalPotentialPercentage*afferentPopulationDimensionality;

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

			std::size_t	randomUnit = std::rand() % afferentPopulationDimensionality;
			auto	randomUnitCoordinates = unravelIndex(randomUnit,_afferentPopulationsArrayDimensionality);
			std::vector<std::size_t>	inputs;
			for ( std::size_t afferentPotentialConnection = 0;
					  afferentPotentialConnection < afferentPotentialDimensionality;
					  afferentPotentialConnection++ )
				inputs.insert(std::end(inputs), std::begin(randomUnitCoordinates), std::end(randomUnitCoordinates));

			for (const auto& input : inputs)
				proximalInputs.inputs.push_back((double)input);
		}
		// If there is at least one index then, gather this information
		else {
			assert(afferent.information[connection] == true);
			std::vector<std::size_t>	unitCoordinates(_afferentPopulationsArrayDimensionality.size(),0);
			for (const auto& index : afferent.currentIndexes[connection])
				unitCoordinates += unravelIndex(index,_afferentPopulationsArrayDimensionality);

			std::vector<std::size_t>	inputs;
			for ( std::size_t afferentPotentialConnection = 0;
					  afferentPotentialConnection < afferentPotentialDimensionality;
					  afferentPotentialConnection++ )
				inputs.insert(std::end(inputs), std::begin(unitCoordinates), std::end(unitCoordinates));

			for (const auto& input : inputs)
				proximalInputs.inputs.push_back((double)input/afferent.currentIndexes[connection].size());

			proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
			proximalInputs.information[offset+afferentConnection] = afferent.information[connection];
		}
	}
	offset += _afferentConnections[column].size();

	if ( _lateralProximalConnections.size() != 0 ) {
		auto	populationDimensionality = std::accumulate(_populationsArrayDimensionality.begin(),
								   _populationsArrayDimensionality.end(),
								   1, std::multiplies<std::size_t>());

		std::size_t	potentialDimensionality = _proximalPotentialPercentage*populationDimensionality;

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

				std::size_t	randomUnit = std::rand() % populationDimensionality;
				auto	randomUnitCoordinates = unravelIndex(randomUnit,_populationsArrayDimensionality);
				std::vector<std::size_t>	inputs;
				for ( std::size_t potentialConnection = 0;
						  potentialConnection < potentialDimensionality;
						  potentialConnection++ )
					inputs.insert(std::end(inputs), std::begin(randomUnitCoordinates), std::end(randomUnitCoordinates));

				for (const auto& input : inputs)
					proximalInputs.inputs.push_back((double)input);
			}
			// If there is at least one index then, gather this information
			else {
				assert(lateral.information[connection] == true);
				std::vector<std::size_t>	unitCoordinates(_populationsArrayDimensionality.size(),0);
				for (const auto& index : lateral.currentIndexes[connection])
					unitCoordinates += unravelIndex(index,_populationsArrayDimensionality);

				std::vector<std::size_t>	inputs;
				for ( std::size_t potentialConnection = 0;
						  potentialConnection < potentialDimensionality;
						  potentialConnection++ )
					inputs.insert(std::end(inputs), std::begin(unitCoordinates), std::end(unitCoordinates));

				for (const auto& input : inputs)
					proximalInputs.inputs.push_back((double)input/lateral.currentIndexes[connection].size());

				proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			}
		}
	}
	proximalInputs.inputs.shrink_to_fit(); 
	return	proximalInputs;
} // end function gatherProximalInputs


//// gathers proximal inputs from afferent and lateral connections
//regularLayerProximalInput	RegularLayer::gatherProximalInputs( const std::size_t column,
								    //const regularLayerResponse& afferent,
								    //const regularLayerResponse& lateral )
//{
	//regularLayerProximalInput	proximalInputs;

	//std::size_t	offset = 0;
	//std::size_t	SDR_offset = 0;
	//assert(afferent.currentIndexes.size() != 0);
	//assert(_temporalGatheringAfferentValue == 1);

	//assert(_afferentConnections.size() != 0);
	//std::size_t	numberOfProximalConnections = _afferentConnections[column].size();

	//if ( _lateralProximalConnections.size() != 0 )
		//numberOfProximalConnections += _lateralProximalConnections[column].size();

	//proximalInputs.synchronization.resize(numberOfProximalConnections);
	//proximalInputs.information.resize(numberOfProximalConnections);

	//for ( std::size_t afferentConnection = 0; afferentConnection < _afferentConnections[column].size(); afferentConnection++ ) {
		//auto	connection = _afferentConnections[column][afferentConnection];
		//auto	numberOfIndexes = afferent.currentIndexes[connection].size();
		//std::vector<std::size_t>	auxiliary;

		//// If there is no index, then there is no information
		//if ( numberOfIndexes == 0 ) {
			//assert(afferent.information[connection] == false);
			//assert(afferent.synchronization[connection] == true);
			//proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
			//proximalInputs.information[offset+afferentConnection] = afferent.information[connection];
		//}
		//// If there is at least one index then, gather this information
		//else {
			//assert(afferent.information[connection] == true);
			//for (const auto& index : afferent.currentIndexes[connection]) {
				//proximalInputs.sparseDistributedRepresentation.push_back(SDR_offset+index);
				//auxiliary.push_back(SDR_offset+index);
			//}
			//proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[connection];
			//proximalInputs.information[offset+afferentConnection] = afferent.information[connection];
		//}
		//auto	old_SDR_offset = SDR_offset;
		//SDR_offset += std::accumulate(_afferentPopulationsArrayDimensionality.begin(),
					      //_afferentPopulationsArrayDimensionality.end(),
					      //1, std::multiplies<std::size_t>());

		//for (const auto& index : auxiliary)
			//assert(index >= old_SDR_offset && index < SDR_offset);
	//}
	//offset += _afferentConnections[column].size();

	//if ( _lateralProximalConnections.size() != 0 ) {
		//for ( std::size_t lateralProximalConnection = 0;
				  //lateralProximalConnection < _lateralProximalConnections[column].size();
				  //lateralProximalConnection++ ) {
			//auto	connection = _lateralProximalConnections[column][lateralProximalConnection];
			//auto	numberOfIndexes = lateral.currentIndexes[connection].size();
			//std::vector<std::size_t>	auxiliary;

			//// If there is no index, then there is no information
			//if ( numberOfIndexes == 0 ) {
				//assert(lateral.information[connection] == false);
				//assert(lateral.synchronization[connection] == true);
				//proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				//proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			//}
			//// If there is at least one index then, gather this information
			//else {
				//assert(lateral.information[connection] == true);
				//for (const auto& index : lateral.currentIndexes[connection]) {
					//proximalInputs.sparseDistributedRepresentation.push_back(SDR_offset+index);
					//auxiliary.push_back(SDR_offset+index);
				//}
				//proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				//proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			//}
			//auto	old_SDR_offset = SDR_offset;
			//SDR_offset += std::accumulate(_populationsArrayDimensionality.begin(),
						      //_populationsArrayDimensionality.end(),
						      //1, std::multiplies<std::size_t>());

			//for (const auto& index : auxiliary)
				//assert(index >= old_SDR_offset && index < SDR_offset);
		//}
	//}

	//return	proximalInputs;
//} // end function gatherProximalInputs


//// gathers proximal inputs from afferent and lateral connections
//regularLayerProximalInput	RegularLayer::gatherProximalInputs( const std::size_t column,
								    //const regularLayerTemporallyGatheredResponse& afferent,
								    //const regularLayerResponse& lateral )
//{
	//regularLayerProximalInput	proximalInputs;

	//std::size_t	offset = 0;
	//std::size_t	SDR_offset = 0;
	//assert(afferent.temporallyGatheredIndexes.size() == _temporalGatheringAfferentValue && _temporalGatheringAfferentValue > 1);
	//assert(afferent.temporallyGatheredIndexes.size() == afferent.temporallyGatheredInformation.size());
	//assert(afferent.temporallyGatheredIndexes.size() == afferent.synchronization.size());

	//assert(_afferentConnections.size() != 0);
	//std::size_t	numberOfProximalConnections = afferent.temporallyGatheredIndexes.size() * _afferentConnections[column].size();

	//if ( _lateralProximalConnections.size() != 0 )
		//numberOfProximalConnections += _lateralProximalConnections[column].size();

	//proximalInputs.synchronization.resize(numberOfProximalConnections);
	//proximalInputs.information.resize(numberOfProximalConnections);

	//for ( std::size_t timeStep = 0; timeStep < afferent.temporallyGatheredIndexes.size(); timeStep++ ) {
		//for ( std::size_t afferentConnection = 0;
				  //afferentConnection < _afferentConnections[column].size();
				  //afferentConnection++ ) {
			//auto	connection = _afferentConnections[column][afferentConnection];
			//auto	numberOfIndexes = afferent.temporallyGatheredIndexes[timeStep][connection].size();
			//std::vector<std::size_t>	auxiliary;

			//// If there is no index, then there is no information
			//if ( numberOfIndexes == 0 ) {
				//assert(afferent.temporallyGatheredInformation[timeStep][connection] == false);
				//assert(afferent.synchronization[timeStep][connection] == true);
				//proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[timeStep][connection];
				//proximalInputs.information[offset+afferentConnection] = afferent.temporallyGatheredInformation[timeStep][connection];
			//}
			//// If there is at least one index then, gather this information
			//else {
				//assert(afferent.temporallyGatheredInformation[timeStep][connection] == true);
				//for (const auto& index : afferent.temporallyGatheredIndexes[timeStep][connection]) {
					//proximalInputs.sparseDistributedRepresentation.push_back(SDR_offset+index);
					//auxiliary.push_back(SDR_offset+index);
				//}
				//proximalInputs.synchronization[offset+afferentConnection] = afferent.synchronization[timeStep][connection];
				//proximalInputs.information[offset+afferentConnection] = afferent.temporallyGatheredInformation[timeStep][connection];
			//}
			//auto	old_SDR_offset = SDR_offset;
			//SDR_offset += std::accumulate(_afferentPopulationsArrayDimensionality.begin(),
						      //_afferentPopulationsArrayDimensionality.end(),
						      //1, std::multiplies<double>());

			//for (const auto& index : auxiliary)
				//assert(index >= old_SDR_offset && index < SDR_offset);
		//}
		//offset += _afferentConnections[column].size();
	//}

	//if ( _lateralProximalConnections.size() != 0 ) {
		//for ( std::size_t lateralProximalConnection = 0;
				  //lateralProximalConnection < _lateralProximalConnections[column].size();
				  //lateralProximalConnection++ ) {
			//auto	connection = _lateralProximalConnections[column][lateralProximalConnection];
			//auto	numberOfIndexes = lateral.currentIndexes[connection].size();
			//std::vector<std::size_t>	auxiliary;

			//// If there is no index, then there is no information
			//if ( numberOfIndexes == 0 ) {
				//assert(lateral.information[connection] == false);
				//assert(lateral.synchronization[connection] == true);
				//proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				//proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			//}
			//// If there is at least one index then, gather this information
			//else {
				//assert(lateral.information[connection] == true);
				//for (const auto& index : lateral.currentIndexes[connection]) {
					//proximalInputs.sparseDistributedRepresentation.push_back(SDR_offset+index);
					//auxiliary.push_back(SDR_offset+index);
				//}
				//proximalInputs.synchronization[offset+lateralProximalConnection] = lateral.synchronization[connection];
				//proximalInputs.information[offset+lateralProximalConnection] = lateral.information[connection];
			//}
			//auto	old_SDR_offset = SDR_offset;
			//SDR_offset += std::accumulate(_populationsArrayDimensionality.begin(),
						      //_populationsArrayDimensionality.end(),
						      //1, std::multiplies<std::size_t>());

			//for (const auto& index : auxiliary)
				//assert(index >= old_SDR_offset && index < SDR_offset);
		//}
	//}

	//return	proximalInputs;
//} // end function gatherProximalInputs


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
	auto	auxiliaryConnections = RegularLayer::gatherConnections();

	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	std::stringstream	outputStream;

	if ( world_rank == 0 ) {
		// file preamble.
		if (ENABLE_MATLAB_COMPATIBILITY) {
			save_the_header(outputStream);
		}
		else {
			outputStream << "# This is a file created by saveRegularLayerStatus member function in RegularLayer class from," << endl;
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



		// saves lateralProximalReceptiveField
		if(_lateralProximalReceptiveField.size() != 0) {
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_vector_as_numeric_array("lateralProximalReceptiveField", _lateralProximalReceptiveField, outputStream);
			else
				save_vector_as_matrix("lateralProximalReceptiveField", _lateralProximalReceptiveField, outputStream);
		}
		else {
			std::vector<int>	receptiveField(_columnsArrayDimensionality.size(),-1);
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_vector_as_numeric_array("lateralProximalReceptiveField", receptiveField, outputStream);
			else
				save_vector_as_matrix("lateralProximalReceptiveField", receptiveField, outputStream);
		}

		// saves lateralProximalPercentage
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_scalar_as_numeric_array("lateralProximalPercentage", _lateralProximalPercentage, outputStream);
		else
			save_as_scalar("lateralProximalPercentage", _lateralProximalPercentage, outputStream);

		// saves lateralProximalWrapAround
		if (ENABLE_MATLAB_COMPATIBILITY) {
			std::size_t	aux_bool;
			if (_lateralProximalWrapAround)
				aux_bool = 1;
			else
				aux_bool = 0;

			save_scalar_as_numeric_array("lateralProximalWrapAround", aux_bool, outputStream);
		}
		else {
			save_as_bool("lateralProximalWrapAround", _lateralProximalWrapAround, outputStream);
		}



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

		if ( auxiliaryConnections.afferentConnections.size() > 0 ) {
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

		if ( auxiliaryConnections.lateralProximalConnections.size() > 0 ) {
			// saves lateralProximalConnections
			twodvector<bool>	lateralProximalConnectionsBitMap;
			lateralProximalConnectionsBitMap.resize(_columnsDimensionality);
			for ( std::size_t column = 0; column < _columnsDimensionality; column++ ) {
				lateralProximalConnectionsBitMap[column].resize(_columnsDimensionality);
				for ( std::size_t index = 0; index < auxiliaryConnections.lateralProximalConnections[column].size(); index++ )
					lateralProximalConnectionsBitMap[column][auxiliaryConnections.lateralProximalConnections[column][index]] = true;
			}	

			SparseMatrixElements<bool>	sparseLateralProximalConnectionsBitMap;
			sparseLateralProximalConnectionsBitMap = to_sparse(lateralProximalConnectionsBitMap);
			if (ENABLE_MATLAB_COMPATIBILITY)
				save_sparse_matrix_elements_as_sparse_array("lateralProximalConnections", sparseLateralProximalConnectionsBitMap, outputStream);
			else
				save_sparse_matrix_elements_as_sparse_matrix("lateralProximalConnections", sparseLateralProximalConnectionsBitMap, outputStream);
		}

		if ( auxiliaryConnections.lateralDistalConnections.size() > 0 ) {
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

		if ( auxiliaryConnections.apicalConnections.size() > 0 ) {
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

		// saves afferentPopulationsArrayDimensionality
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_as_numeric_array("afferentPopulationsArrayDimensionality", _afferentPopulationsArrayDimensionality, outputStream);
		else
			save_vector_as_matrix("afferentPopulationsArrayDimensionality", _afferentPopulationsArrayDimensionality, outputStream);

		// saves apicalPopulationsArrayDimensionality
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_as_numeric_array("apicalPopulationsArrayDimensionality", _apicalPopulationsArrayDimensionality, outputStream);
		else
			save_vector_as_matrix("apicalPopulationsArrayDimensionality", _apicalPopulationsArrayDimensionality, outputStream);

		// saves temporalGatheringAfferentValue
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_scalar_as_numeric_array("temporalGatheringAfferentValue", _temporalGatheringAfferentValue, outputStream);
		else
			save_as_scalar("temporalGatheringAfferentValue", _temporalGatheringAfferentValue, outputStream);

		// saves proximalPotentialPercentage
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_scalar_as_numeric_array("proximalPotentialPercentage", _proximalPotentialPercentage, outputStream);
		else
			save_as_scalar("proximalPotentialPercentage", _proximalPotentialPercentage, outputStream);

		// saves distalPotentialPercentage
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_scalar_as_numeric_array("distalPotentialPercentage", _distalPotentialPercentage, outputStream);
		else
			save_as_scalar("distalPotentialPercentage", _distalPotentialPercentage, outputStream);

		//if ( _temporalGatheringAfferentValue > 1 ) {
			//// saves _internalTemporallyGatheredInputs; then:

			////// saves temporalPointer
			////if (ENABLE_MATLAB_COMPATIBILITY)
				////save_vector_as_numeric_array("temporalPointer", _temporalPointer, outputStream);
			////else
				////save_vector_as_matrix("temporalPointer", _temporalPointer, outputStream);

			//// saves synchronization
			//if (ENABLE_MATLAB_COMPATIBILITY)
				//save_vector_of_vectors_as_numeric_array("internalSynchronization",
									//_internalTemporallyGatheredInputs.synchronization,
									//outputStream);
			//else
				//save_vector_of_vectors_as_matrix("internalSynchronization",
								 //_internalTemporallyGatheredInputs.synchronization,
								 //outputStream);
			////if (ENABLE_MATLAB_COMPATIBILITY)
				////save_vector_as_numeric_array("internalSynchronization",
							////_internalTemporallyGatheredInputs.synchronization,
							////outputStream);
			////else
				////save_vector_as_matrix("internalSynchronization",
							////_internalTemporallyGatheredInputs.synchronization,
							////outputStream);

			//// saves temporallyGatheredInformation
			//if (ENABLE_MATLAB_COMPATIBILITY)
				//save_vector_of_vectors_as_numeric_array("internalTemporallyGatheredInformation",
									//_internalTemporallyGatheredInputs.temporallyGatheredInformation,
									//outputStream);
			//else
				//save_vector_of_vectors_as_matrix("internalTemporallyGatheredInformation",
								 //_internalTemporallyGatheredInputs.temporallyGatheredInformation,
								 //outputStream);

			//// saves temporallyGatheredIndexes
			//std::size_t	aux;
			//make_rectangular(_internalTemporallyGatheredInputs.temporallyGatheredIndexes, -1);
			//if (ENABLE_MATLAB_COMPATIBILITY)
				//save_multidimensional_vector_as_numeric_array("internalTemporallyGatheredIndexes",
									      //_internalTemporallyGatheredInputs.temporallyGatheredIndexes,
									      //aux, outputStream);
			//else
				//save_multidimensional_vector_as_matrix("internalTemporallyGatheredIndexes",
									//_internalTemporallyGatheredInputs.temporallyGatheredIndexes,
									//aux, outputStream);

		//}
	}

	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
		_layerColumns[column/world_size].saveComplexProcessorStatus(std::to_string(column), outputStream);

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

	std::string	name = COMMON_PATH + folderName + "/RegularLayer_" + std::to_string(identifyer) + ".mat";
	std::remove(&name[0]);
	MPI_Barrier(MPI_COMM_WORLD);

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
} // end function saveRegularLayerStatus


// function that loads the RegularLayer's status in a file
void	RegularLayer::loadRegularLayerStatus( const std::string& folderName, const std::size_t identifyer )
{
	// Get the number of processes
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	std::stringstream	inputStream;

        // open a file in read mode.
	MPI::File infile = MPI::File::Open(MPI::COMM_WORLD, (COMMON_PATH + folderName + "/RegularLayer_" + std::to_string(identifyer) + ".mat").c_str(),
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
	bool	check_proximalPotentialPercentage = false;
	bool	check_distalPotentialPercentage = false;

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

			STR = "lateralProximalReceptiveField";
			if ( array_structure.name.compare(STR) == 0 ) {
				_lateralProximalReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_numeric_array_to_vector(array_structure, receptiveField, inputStream, big_endianness);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_lateralProximalReceptiveField.push_back(s);
				}
				check_lateralProximalReceptiveField = true;
			}

			STR = "lateralProximalPercentage";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _lateralProximalPercentage, inputStream, big_endianness);
				check_lateralProximalPercentage = true;
			}

			STR = "lateralProximalWrapAround";
			if ( array_structure.name.compare(STR) == 0 ) {
				std::size_t	aux_bool;
				load_numeric_array_to_scalar(array_structure, aux_bool, inputStream, big_endianness);
				if (aux_bool > 0)
					_lateralProximalWrapAround = true;
				else
					_lateralProximalWrapAround = false;

				check_lateralProximalWrapAround = true;
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

			STR = "lateralProximalConnections";
			if ( array_structure.name.compare(STR) == 0 ) {
				check_lateralProximalConnections = true;
				SparseMatrixElements<bool>	sparseLateralProximalConnectionsBitMap;
				load_sparse_matrix_elements_as_sparse_array(array_structure, sparseLateralProximalConnectionsBitMap, inputStream, big_endianness);
				twodvector<bool>	lateralProximalConnectionsBitMap;
				lateralProximalConnectionsBitMap = from_sparse(sparseLateralProximalConnectionsBitMap);
				for ( std::size_t column = world_rank; column < lateralProximalConnectionsBitMap.size(); column=column+world_size ) {
					std::vector<std::size_t>	auxiliaryConnections;
					for ( std::size_t index = 0; index < lateralProximalConnectionsBitMap[column].size(); index++ ) {
						if ( lateralProximalConnectionsBitMap[column][index] == true )
							auxiliaryConnections.push_back(index);
					}
					_lateralProximalConnections.push_back(auxiliaryConnections);
				}
				_lateralProximalConnections.shrink_to_fit();
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

			STR = "afferentPopulationsArrayDimensionality";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_vector(array_structure, _afferentPopulationsArrayDimensionality, inputStream, big_endianness);
				check_afferentPopulationsArrayDimensionality = true;
			}

			STR = "apicalPopulationsArrayDimensionality";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_vector(array_structure, _apicalPopulationsArrayDimensionality, inputStream, big_endianness);
				check_apicalPopulationsArrayDimensionality = true;
			}

			STR = "temporalGatheringAfferentValue";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _temporalGatheringAfferentValue, inputStream, big_endianness);
				check_temporalGatheringAfferentValue = true;
			}

			//STR = "temporalPointer";
			//if ( array_structure.name.compare(STR) == 0 ) {
				//load_numeric_array_to_vector(array_structure, _temporalPointer, inputStream, big_endianness);
				//check_temporalPointer = true;
			//}

			//STR = "internalSynchronization";
			//if ( array_structure.name.compare(STR) == 0 ) {
				//twodvector<std::size_t>	auxiliary;
				//load_numeric_array_to_vector_of_vectors(array_structure, auxiliary, inputStream, big_endianness);
				//_internalTemporallyGatheredInputs.synchronization.resize(auxiliary.size());
				//for( std::size_t timeStep = 0; timeStep < auxiliary.size(); timeStep++ ) {
					//for(const auto& aux : auxiliary[timeStep]) {
						//if ( aux == 0 )
							//_internalTemporallyGatheredInputs.synchronization[timeStep].push_back(false);
						//else
							//_internalTemporallyGatheredInputs.synchronization[timeStep].push_back(true);
					//}
				//}
				//check_internalTemporallyGatheredInformation = true;
			//}
			////if ( array_structure.name.compare(STR) == 0 ) {
				////std::vector<std::size_t>	auxiliary;
				////load_numeric_array_to_vector(array_structure, auxiliary, inputStream, big_endianness);
				////for(const auto& aux : auxiliary) {
					////if ( aux == 0 )
						////_internalTemporallyGatheredInputs.synchronization.push_back(false);
					////else
						////_internalTemporallyGatheredInputs.synchronization.push_back(true);
				////}
				////_internalTemporallyGatheredInputs.synchronization.shrink_to_fit();
				////check_internalSynchronization = true;
			////}

			//STR = "internalTemporallyGatheredInformation";
			//if ( array_structure.name.compare(STR) == 0 ) {
				//twodvector<std::size_t>	auxiliary;
				//load_numeric_array_to_vector_of_vectors(array_structure, auxiliary, inputStream, big_endianness);
				//_internalTemporallyGatheredInputs.temporallyGatheredInformation.resize(auxiliary.size());
				//for( std::size_t timeStep = 0; timeStep < auxiliary.size(); timeStep++ ) {
					//for(const auto& aux : auxiliary[timeStep]) {
						//if ( aux == 0 )
							//_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].push_back(false);
						//else
							//_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].push_back(true);
					//}
				//}
				//check_internalTemporallyGatheredInformation = true;
			//}

			//STR = "internalTemporallyGatheredIndexes";
			//if ( array_structure.name.compare(STR) == 0 ) {
				//std::size_t	aux;

				//load_array_to_multidimensional_vector(array_structure,
								      //_internalTemporallyGatheredInputs.temporallyGatheredIndexes,
								      //aux, inputStream, big_endianness);

				//check_internalTemporallyGatheredIndexes = true;
			//}

			STR = "proximalPotentialPercentage";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _proximalPotentialPercentage, inputStream, big_endianness);
				check_proximalPotentialPercentage = true;
			}

			STR = "distalPotentialPercentage";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _distalPotentialPercentage, inputStream, big_endianness);
				check_distalPotentialPercentage = true;
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



			STR = "# name: lateralProximalReceptiveField";
			if ( str.compare(STR) == 0 ) {
				_lateralProximalReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_matrix_to_vector(receptiveField, inputStream);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_lateralProximalReceptiveField.push_back(s);
				}
				check_lateralProximalReceptiveField = true;
			}

			STR = "# name: lateralProximalPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_lateralProximalPercentage, inputStream);
				check_lateralProximalPercentage = true;
			}

			STR = "# name: lateralProximalWrapAround";
			if ( str.compare(STR) == 0 ) {
				load_bool(_lateralProximalWrapAround, inputStream);
				check_lateralProximalWrapAround = true;
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

			STR = "# name: lateralProximalConnections";
			if ( str.compare(STR) == 0 ) {
				check_lateralProximalConnections = true;
				SparseMatrixElements<bool>	sparseLateralProximalConnectionsBitMap;
				load_sparse_matrix_to_sparse_matrix_elements(sparseLateralProximalConnectionsBitMap, inputStream);
				twodvector<bool>	lateralProximalConnectionsBitMap;
				lateralProximalConnectionsBitMap = from_sparse(sparseLateralProximalConnectionsBitMap);
				for ( std::size_t column = world_rank; column < lateralProximalConnectionsBitMap.size(); column=column+world_size ) {
					std::vector<std::size_t>	auxiliaryConnections;
					for ( std::size_t index = 0; index < lateralProximalConnectionsBitMap[column].size(); index++ ) {
						if ( lateralProximalConnectionsBitMap[column][index] == true )
							auxiliaryConnections.push_back(index);
					}
					_lateralProximalConnections.push_back(auxiliaryConnections);
				}
				_lateralProximalConnections.shrink_to_fit();
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

			STR = "# name: afferentPopulationsArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_afferentPopulationsArrayDimensionality, inputStream);
				check_afferentPopulationsArrayDimensionality = true;
			}

			STR = "# name: apicalPopulationsArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_apicalPopulationsArrayDimensionality, inputStream);
				check_apicalPopulationsArrayDimensionality = true;
			}

			STR = "# name: temporalGatheringAfferentValue";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_temporalGatheringAfferentValue, inputStream);
				check_temporalGatheringAfferentValue = true;
			}

			//STR = "# name: temporalPointer";
			//if ( str.compare(STR) == 0 ) {
				//load_matrix_to_vector(_temporalPointer, inputStream);
				//check_temporalPointer = true;
			//}

			//STR = "# name: internalSynchronization";
			//if ( str.compare(STR) == 0 ) {
				//twodvector<std::size_t>	auxiliary;
				//load_matrix_to_vector_of_vectors(auxiliary, inputStream);
				//_internalTemporallyGatheredInputs.synchronization.resize(auxiliary.size());
				//for( std::size_t timeStep = 0; timeStep < auxiliary.size(); timeStep++ ) {
					//for(const auto& aux : auxiliary[timeStep]) {
						//if ( aux == 0 )
							//_internalTemporallyGatheredInputs.synchronization[timeStep].push_back(false);
						//else
							//_internalTemporallyGatheredInputs.synchronization[timeStep].push_back(true);
					//}
				//}
				//check_internalTemporallyGatheredInformation = true;
			//}
			////if ( str.compare(STR) == 0 ) {
				////std::vector<std::size_t>	auxiliary;
				////load_matrix_to_vector(auxiliary, inputStream);
				////for(const auto& aux : auxiliary) {
					////if ( aux == 0 )
						////_internalTemporallyGatheredInputs.synchronization.push_back(false);
					////else
						////_internalTemporallyGatheredInputs.synchronization.push_back(true);
				////}
				////_internalTemporallyGatheredInputs.synchronization.shrink_to_fit();
				////check_internalSynchronization = true;
			////}

			//STR = "# name: internalTemporallyGatheredInformation";
			//if ( str.compare(STR) == 0 ) {
				//twodvector<std::size_t>	auxiliary;
				//load_matrix_to_vector_of_vectors(auxiliary, inputStream);
				//_internalTemporallyGatheredInputs.temporallyGatheredInformation.resize(auxiliary.size());
				//for( std::size_t timeStep = 0; timeStep < auxiliary.size(); timeStep++ ) {
					//for(const auto& aux : auxiliary[timeStep]) {
						//if ( aux == 0 )
							//_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].push_back(false);
						//else
							//_internalTemporallyGatheredInputs.temporallyGatheredInformation[timeStep].push_back(true);
					//}
				//}
				//check_internalTemporallyGatheredInformation = true;
			//}

			//STR = "# name: internalTemporallyGatheredIndexes";
			//if ( str.compare(STR) == 0 ) {
				//std::size_t	aux;
				//load_matrix_to_multidimensional_vector(_internalTemporallyGatheredInputs.temporallyGatheredIndexes,
									//aux, inputStream);

				//check_internalTemporallyGatheredIndexes = true;
			//}

			STR = "# name: proximalPotentialPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_proximalPotentialPercentage, inputStream);
				check_proximalPotentialPercentage = true;
			}

			STR = "# name: distalPotentialPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_distalPotentialPercentage, inputStream);
				check_distalPotentialPercentage = true;
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
	assert(check_lateralProximalReceptiveField == true);
	assert(check_lateralProximalPercentage == true);
	assert(check_lateralProximalWrapAround == true);
	assert(check_lateralDistalReceptiveField == true);
	assert(check_lateralDistalPercentage == true);
	assert(check_lateralDistalWrapAround == true);
	assert(check_afferentConnections == true);

	if( _lateralProximalReceptiveField.size() != 0 ) {
		assert(check_lateralProximalConnections == true);
	}

	if( _lateralDistalReceptiveField.size() != 0 ) {
		assert(check_lateralDistalConnections == true);
	}

	if( _apicalReceptiveField.size() != 0 ) {
		assert(check_apicalConnections == true);
	}

	assert(check_populationsArrayDimensionality == true);
	assert(check_afferentPopulationsArrayDimensionality == true);
	assert(check_apicalPopulationsArrayDimensionality == true);
	assert(check_proximalPotentialPercentage == true);
	assert(check_distalPotentialPercentage == true);

	assert(check_temporalGatheringAfferentValue == true);

	//if ( _temporalGatheringAfferentValue > 1 ) {
		//assert(check_temporalPointer == true);
		//assert(check_internalSynchronization == true);
		//assert(check_internalTemporallyGatheredInformation == true);
		//assert(check_internalTemporallyGatheredIndexes == true);
	//}

	if ( _temporalGatheringAfferentValue > 1 )
		RegularLayer::initializeInternalTemporallyGatheredInputs();

	for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
		_layerColumns.push_back(ComplexProcessor(inputStream, std::to_string(column)));

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
	assert(_proximalPotentialPercentage == structure.proximalPotentialPercentage);
	assert(_distalPotentialPercentage == structure.distalPotentialPercentage);
} // end function checkregularLayerStructure


// this function merges all the column outputs
void	RegularLayer::mergeOutputs( regularLayerResponse& output )
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
connectionsSet	RegularLayer::gatherConnections()
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

			if ( _lateralProximalConnections.size() != 0 ) {
				// Allocate memory for lateral distal connections
				auxiliaryConnections.lateralProximalConnections.resize(_columnsDimensionality);
				// Collects rank 0 lateral distal connections
				for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size )
					auxiliaryConnections.lateralProximalConnections[column] = _lateralProximalConnections[column/world_size];
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
	


				if ( _lateralProximalConnections.size() != 0 ) {
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
						if ( auxiliaryConnections.lateralProximalConnections[column].size() != 0 ) {
							std::cout << "\nIn process " << world_rank << "\n"; 
							std::cout << "\nmergeConnections inconsistence:\n"; 
							std::cout << "\nlateralProximalConnections[column].size() != 0\n"; 
							MPI_Abort(MPI_COMM_WORLD,1);
						}
						auxiliaryConnections.lateralProximalConnections[column].insert(
						auxiliaryConnections.lateralProximalConnections[column].end(),
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
						auxiliaryConnections.lateralDistalConnections[column].insert(
						auxiliaryConnections.lateralDistalConnections[column].end(),
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
						auxiliaryConnections.apicalConnections[column].insert(
						auxiliaryConnections.apicalConnections[column].end(),
						connections.begin(),
						connections.begin()+connectionsStructure[column/world_size]);

						connections.erase(connections.begin(), connections.begin()+connectionsStructure[column/world_size]);
					}

					connectionsStructure.clear();
					connections.clear();

				}
			}

			// rank 0 merges all connections in its connections members

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

			if ( _lateralProximalConnections.size() != 0 ) {
				// Sends lateral distal connections structure
				for ( std::size_t column = world_rank; column < _columnsDimensionality; column=column+world_size ) {
					connectionsStructure.push_back(_lateralProximalConnections[column/world_size].size());
					for ( std::size_t connection = 0; connection < _lateralProximalConnections[column/world_size].size(); connection++ )
						connections.push_back(_lateralProximalConnections[column/world_size][connection]);
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
		auxiliaryConnections.lateralProximalConnections = _lateralProximalConnections;
		auxiliaryConnections.lateralDistalConnections = _lateralDistalConnections;
		auxiliaryConnections.apicalConnections = _apicalConnections;
	}

	return	auxiliaryConnections;
} // end function gatherConnections
