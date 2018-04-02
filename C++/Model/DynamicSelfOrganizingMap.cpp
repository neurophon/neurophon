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

// File Name:		DynamicSelfOrganizingMap.cpp
// File Description:	DynamicSelfOrganizingMap member-function definitions. This file contains implementations of the
//			member functions prototyped in DynamicSelfOrganizingMap.h.

#include <fstream>
#include <algorithm>
#include <string>

#include "DynamicSelfOrganizingMap.h"				// includes definition of class DynamicSelfOrganizingMap
#include "../Libraries/Model/Templates.h"
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/Constants.h"
#include "../Libraries/Model/OctaveInterface.h"
#include "../Libraries/Model/MatlabInterface.h"
#include "../Libraries/Model/GlobalVariables.h"

using namespace std;

// constructor initializes populationDimensions, numberOfInputs and dynamicUnits with variables supplied as arguments
DynamicSelfOrganizingMap::DynamicSelfOrganizingMap( const std::vector<std::size_t>& populationDimensions,
						    const std::size_t numberOfInputs,
						    const double potentialPercentage,
						    const std::vector<std::size_t>& dynamicUnits )
	// explicitly call base-class constructor
	: StaticSelfOrganizingMap(populationDimensions, numberOfInputs)
{
	assert(potentialPercentage > 0.0 && potentialPercentage < 1.0);
	_numberOfLinks = dynamicUnits.size();
	_updateStep = 0;
	_dynamicUnits.resize(_numberOfLinks);
	_potentialConnections.resize(_numberOfLinks);
	_potentialDimensionality.resize(_numberOfLinks);
	_potentialPercentage = potentialPercentage;
	for ( std::size_t link = 0; link < _numberOfLinks; link++ ) {
		_dynamicUnits[link].resize(_unitsDimensionality);
		_potentialConnections[link].resize(_unitsDimensionality);
		_potentialDimensionality[link] = potentialPercentage*dynamicUnits[link];
		std::vector<std::size_t>	inputRange(dynamicUnits[link]);
		std::iota(std::begin(inputRange), std::end(inputRange), 0);
		for ( std::size_t row = 0; row < _unitsDimensionality; row++ ) {
			_dynamicUnits[link][row].resize(_potentialDimensionality[link]);
			sample_vector(inputRange, _potentialConnections[link][row],
						  _potentialDimensionality[link]);
		}
			
	}
	
	assert(is_rectangular(_dynamicUnits));
	assert(is_rectangular(_potentialConnections));
	assert(_dynamicUnits.size() == _potentialConnections.size());
	for ( std::size_t link = 0; link < _dynamicUnits.size(); link++ ) {
		assert(_dynamicUnits[link].size() == _potentialConnections[link].size());
		for ( std::size_t row = 0; row < _dynamicUnits[link].size(); row++ )
			assert(_dynamicUnits[link][row].size() == _potentialConnections[link][row].size());
	}

} // end DynamicSelfOrganizingMap constructor


// constructor initializes populationDimensions, numberOfInputs and dynamicUnits with variables supplied as arguments
// The weights in the self SelfOrganizingMap will be initialized with random values between weightLimits
DynamicSelfOrganizingMap::DynamicSelfOrganizingMap( const std::vector<std::size_t>& populationDimensions,
						    const std::size_t numberOfInputs,
						    const double potentialPercentage,
						    const std::vector<std::size_t>& dynamicUnits,
						    const std::array<double,2>& weightLimits )
	// explicitly call base-class constructor
	: StaticSelfOrganizingMap(populationDimensions, numberOfInputs, weightLimits)
{
	assert(potentialPercentage > 0.0 && potentialPercentage < 1.0);
	_numberOfLinks = dynamicUnits.size();
	_updateStep = 0;
	_dynamicUnits.resize(_numberOfLinks);
	_potentialConnections.resize(_numberOfLinks);
	_potentialDimensionality.resize(_numberOfLinks);
	_potentialPercentage = potentialPercentage;
	for ( std::size_t link = 0; link < _numberOfLinks; link++ ) {
		_dynamicUnits[link].resize(_unitsDimensionality);
		_potentialConnections[link].resize(_unitsDimensionality);
		_potentialDimensionality[link] = potentialPercentage*dynamicUnits[link];
		std::vector<std::size_t>	inputRange(dynamicUnits[link]);
		std::iota(std::begin(inputRange), std::end(inputRange), 0);
		for ( std::size_t row = 0; row < _unitsDimensionality; row++ ) {
			_dynamicUnits[link][row].resize(_potentialDimensionality[link]);
			sample_vector(inputRange, _potentialConnections[link][row],
						  _potentialDimensionality[link]);
		}
			
	}
	
	assert(is_rectangular(_dynamicUnits));
	assert(is_rectangular(_potentialConnections));
	assert(_dynamicUnits.size() == _potentialConnections.size());
	for ( std::size_t link = 0; link < _dynamicUnits.size(); link++ ) {
		assert(_dynamicUnits[link].size() == _potentialConnections[link].size());
		for ( std::size_t row = 0; row < _dynamicUnits[link].size(); row++ )
			assert(_dynamicUnits[link][row].size() == _potentialConnections[link][row].size());
	}

} // end DynamicSelfOrganizingMap constructor


// constructor initializes populationDimensions and numberOfInputs with variables supplied as arguments.
// This loads _dynamicUnits with previous vector supplied as argument too
DynamicSelfOrganizingMap::DynamicSelfOrganizingMap( std::stringstream& inputStream,
						    const std::string& dynamicSelfOrganizingMapIdentification )
	// explicitly call base-class constructor
	: StaticSelfOrganizingMap(inputStream, dynamicSelfOrganizingMapIdentification)
{
	inputStream.clear();
	inputStream.str(inputStream.str());
	DynamicSelfOrganizingMap::loadDynamicSelfOrganizingMapStatus(dynamicSelfOrganizingMapIdentification, inputStream);
} // end DynamicSelfOrganizingMap constructor


// updates a group of _dynamicUnits depending on a set of unit indexes.
// on the other hand, every link -from which information is coming- is a vector
// hence, it could contain a set of linking units as well as a unique linking unit.
void	DynamicSelfOrganizingMap::Update( const std::vector<std::size_t>& indexes,
					  const twodvector<std::size_t>& linkingUnits,
					  const bool increment,
					  const double threshold,
					  const double learningRate )
{
	auto	links = linkingUnits.size();
	if ( links != _dynamicUnits.size() )
		throw std::runtime_error ("links != _dynamicUnits.size()");

	if ( increment ) { // if increment is true
		for(const auto& index : indexes) {
			if ( index > _unitsDimensionality )
				throw std::runtime_error ("index > _unitsDimensionality");

			#pragma omp parallel for default(none) shared(index, links, linkingUnits) num_threads(1)
			for ( std::size_t link = 0; link < links; link++ ) {
				for ( std::size_t connection = 0; connection < linkingUnits[link].size(); connection++ ) {
					auto	potentialIndex =
					       	find_first_coincident_index(_potentialConnections[link][index],
									    linkingUnits[link][connection]);

					if ( potentialIndex < _potentialDimensionality[link] ) {
						_dynamicUnits[link][index][potentialIndex] +=
									learningRate*SYNAPTIC_INCREMENT;
					}
				}
			}
		}
	}
	else { // if increment is false, then decrements the synapses
		for(const auto& index : indexes) {
			if ( index > _unitsDimensionality )
				throw std::runtime_error ("index > _unitsDimensionality");

			#pragma omp parallel for default(none) shared(index, links, linkingUnits) num_threads(1)
			for ( std::size_t link = 0; link < links; link++ ) {
				for ( std::size_t connection = 0; connection < linkingUnits[link].size(); connection++ ) {
					auto	potentialIndex =
					       	find_first_coincident_index(_potentialConnections[link][index],
									    linkingUnits[link][connection]);

					if ( potentialIndex < _potentialDimensionality[link] ) {
						_dynamicUnits[link][index][potentialIndex] -=
									learningRate*SYNAPTIC_DECREMENT;

						// there must not exist negative unit synapses
						if ( _dynamicUnits[link][index][potentialIndex] < 0.0 )
							_dynamicUnits[link][index][potentialIndex] = 0.0;

					}

				}
			}
		}
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		#pragma omp parallel for default(none) shared(links) num_threads(1)
		for ( std::size_t link = 0; link < links; link++ ) {
			for ( std::size_t row = 0; row < _unitsDimensionality; row++ ) {
				auto	sum = std::accumulate(_dynamicUnits[link][row].begin(), _dynamicUnits[link][row].end(), 0.0);
				if ( sum > 1 )
					std::transform(_dynamicUnits[link][row].begin(), _dynamicUnits[link][row].end(), _dynamicUnits[link][row].begin(),
					std::bind2nd(std::divides<double>(),sum));

				auto	indexes = less_than_indexes(_dynamicUnits[link][row], threshold);
				set_elements(_dynamicUnits[link][row], indexes, 0.0);
			}
		}

		_updateStep = 0;
	}
	_updateStep++;
} // end function Update


// modifies response ussing _dynamicUnits
somResponseInfo	DynamicSelfOrganizingMap::getDynamicResponse( const somResponseInfo& response,
							      const twodvector<std::size_t>& linkingUnits )
{
	assert(linkingUnits.size() == _dynamicUnits.size());
	assert(response.distances.size() == _unitsDimensionality);

	std::vector<std::size_t>	totalDynamicSelfOrganizingMap;
	somResponseInfo	newResponse;

	totalDynamicSelfOrganizingMap.resize(_unitsDimensionality);
	newResponse = response;

	#pragma omp parallel for default(none) shared(linkingUnits,totalDynamicSelfOrganizingMap) num_threads(1)
	for( std::size_t row = 0; row < _unitsDimensionality; row++ ) {
		std::size_t	auxiliary = 0;
		for( std::size_t link = 0; link < _numberOfLinks; link++) {
			double	link_accum = 0.0;
			for ( std::size_t connection = 0; connection < linkingUnits[link].size(); connection++ ) {
				auto	potentialIndex =
					find_first_coincident_index(_potentialConnections[link][row],
								    linkingUnits[link][connection]);

				if ( potentialIndex < _potentialDimensionality[link] )
					link_accum += _dynamicUnits[link][row][potentialIndex];

			}
			if ( link_accum > 100*DISTAL_SYNAPTIC_THRESHOLD )
				auxiliary++;
		}
		totalDynamicSelfOrganizingMap[row] += auxiliary;
	}

	std::transform(newResponse.distances.begin(), newResponse.distances.end(),
		       totalDynamicSelfOrganizingMap.begin(),
		       newResponse.distances.begin(), std::divides<double>());

	newResponse.ranking = sort_indexes(newResponse.distances);

	return	newResponse;
} // end function getDynamicResponse


// function to save the DynamicSelfOrganizingMap' status in a file
void	DynamicSelfOrganizingMap::saveDynamicSelfOrganizingMapStatus( const std::string& dynamicSelfOrganizingMapIdentification,
								      std::stringstream& outStream )
{
	std::string	str = "DynamicSelfOrganizingMap_";
	std::string	STR;
	str += dynamicSelfOrganizingMapIdentification;
	str += "_";
	for ( std::size_t link = 0; link < _dynamicUnits.size(); link++ ) {
		// saves _dynamicUnits
		STR = "dynamicUnits_" + std::to_string(link);
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_of_vectors_conditionally_as_sparse_array(str+STR,_dynamicUnits[link],SPARSITY_THRESHOLD,outStream);
		else
			save_vector_of_vectors_conditionally_as_sparse_matrix(str+STR,_dynamicUnits[link],SPARSITY_THRESHOLD,outStream);

		STR.clear();
	}

	for ( std::size_t link = 0; link < _potentialConnections.size(); link++ ) {
		// saves _potentialConnections
		STR = "potentialConnections_" + std::to_string(link);
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_of_vectors_as_numeric_array(str+STR,_potentialConnections[link],outStream);
		else
			save_vector_of_vectors_as_matrix(str+STR,_potentialConnections[link],outStream);

		STR.clear();
	}

        // saves _numberOfLinks
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_scalar_as_numeric_array(str + "numberOfLinks", _numberOfLinks, outStream);
	else
		save_as_scalar(str + "numberOfLinks", _numberOfLinks, outStream);

        // saves _potentialDimensionality
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_vector_as_numeric_array(str + "potentialDimensionality", _potentialDimensionality, outStream);
	else
		save_vector_as_matrix(str + "potentialDimensionality", _potentialDimensionality, outStream);

        // saves _potentialPercentage
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_scalar_as_numeric_array(str + "potentialPercentage", _potentialPercentage, outStream);
	else
		save_as_scalar(str + "potentialPercentage", _potentialPercentage, outStream);

        // saves _updateStep
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_scalar_as_numeric_array(str + "updateStep", _updateStep, outStream);
	else
		save_as_scalar(str + "updateStep", _updateStep, outStream);

} // end functiom saveDynamicSelfOrganizingMapStatus


// function to load the DynamicSelfOrganizingMap' status from a file
void	DynamicSelfOrganizingMap::loadDynamicSelfOrganizingMapStatus( const std::string& dynamicSelfOrganizingMapIdentification,
								      std::stringstream& infile )
{
	std::string	str;
	std::string	STR = "DynamicSelfOrganizingMap_";
	std::string	auxiliary;
	STR += dynamicSelfOrganizingMapIdentification;
	STR += "_";

	bool	check_numberOfLinks = false;
	bool	check_potentialDimensionality = false;
	bool	check_potentialPercentage = false;
	bool	check_updateStep = false;
	std::size_t	check_dynamicUnits = 0;
	std::size_t	check_potentialConnections = 0;

	std::size_t	link1 = 0;
	std::size_t	link2 = 0;

	if (ENABLE_MATLAB_COMPATIBILITY) {
		auto	array_structure = check_next_data_structure(infile, big_endianness);
		while ( array_structure.more_data ) {

			auxiliary = STR + "dynamicUnits_" + std::to_string(link1);
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				// load _dynamicUnits
				twodvector<double>	dynamicUnits;
				load_conditionally_sparse_array_to_vector_of_vectors(array_structure, dynamicUnits, infile, big_endianness);
				_dynamicUnits.push_back(dynamicUnits);

				link1++;
				check_dynamicUnits++;
			}

			auxiliary = STR + "potentialConnections_" + std::to_string(link2);
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				// load _potentialConnections
				twodvector<std::size_t>	potentialConnections;
				load_numeric_array_to_vector_of_vectors(array_structure, potentialConnections, infile, big_endianness);
				_potentialConnections.push_back(potentialConnections);

				link2++;
				check_potentialConnections++;
			}

			auxiliary = STR + "numberOfLinks";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _numberOfLinks, infile, big_endianness);
				check_numberOfLinks = true;
			}

			auxiliary = STR + "potentialDimensionality";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_vector(array_structure, _potentialDimensionality, infile, big_endianness);
				check_potentialDimensionality = true;
			}

			auxiliary = STR + "potentialPercentage";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _potentialPercentage, infile, big_endianness);
				check_potentialPercentage = true;
			}

			auxiliary = STR + "updateStep";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _updateStep, infile, big_endianness);
				check_updateStep = true;
			}

			array_structure = check_next_data_structure(infile,big_endianness);
		}	

	}
	else {
		while ( std::getline(infile, str) ) {
			auxiliary = "# name: " + STR + "dynamicUnits_" + std::to_string(link1);
			if ( str.compare(auxiliary) == 0 ) {
				// load _dynamicUnits
				twodvector<double>	dynamicUnits;
				load_conditional_sparse_matrix_to_vector_of_vectors(dynamicUnits,infile);
				_dynamicUnits.push_back(dynamicUnits);

				link1++;
				check_dynamicUnits++;
			}

			auxiliary = "# name: " + STR + "potentialConnections_" + std::to_string(link2);
			if ( str.compare(auxiliary) == 0 ) {
				// load _potentialConnections
				twodvector<std::size_t>	potentialConnections;
				load_matrix_to_vector_of_vectors(potentialConnections,infile);
				_potentialConnections.push_back(potentialConnections);

				link2++;
				check_potentialConnections++;
			}

			auxiliary = "# name: " + STR + "numberOfLinks";
			if ( str.compare(auxiliary) == 0 ) {
				load_scalar(_numberOfLinks, infile);
				check_numberOfLinks = true;
			}

			auxiliary = "# name: " + STR + "potentialDimensionality";
			if ( str.compare(auxiliary) == 0 ) {
				load_matrix_to_vector(_potentialDimensionality, infile);
				check_potentialDimensionality = true;
			}

			auxiliary = "# name: " + STR + "potentialPercentage";
			if ( str.compare(auxiliary) == 0 ) {
				load_scalar(_potentialPercentage, infile);
				check_potentialPercentage = true;
			}

			auxiliary = "# name: " + STR + "updateStep";
			if ( str.compare(auxiliary) == 0 ) {
				load_scalar(_updateStep, infile);
				check_updateStep = true;
			}
		}
	}
	_dynamicUnits.shrink_to_fit();
	_potentialConnections.shrink_to_fit();


	assert(check_numberOfLinks == true);
	assert(check_potentialDimensionality == true);
	assert(check_potentialPercentage == true);
	assert(check_updateStep == true);
	assert(check_dynamicUnits == _numberOfLinks);
	assert(check_potentialConnections == _numberOfLinks);
} // end functiom loadDynamicSelfOrganizingMapStatus






