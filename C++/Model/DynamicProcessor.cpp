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

// File Name:		DynamicProcessor.cpp
// File Description:	DynamicProcessor member-function definitions. This file contains implementations of the
//			member functions prototyped in DynamicProcessor.h.

#include <fstream>
#include <algorithm>
#include <string>

#include "DynamicProcessor.h"				// includes definition of class DynamicProcessor
#include "../Libraries/Model/Templates.h"
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/Constants.h"
#include "../Libraries/Model/OctaveInterface.h"
#include "../Libraries/Model/MatlabInterface.h"
#include "../Libraries/Model/GlobalVariables.h"

using namespace std;

// constructor initializes populationDimensions, numberOfInputs and dynamicUnits with variables supplied as arguments
// The weights in the self Processor will be initialized with random values between weightLimits
DynamicProcessor::DynamicProcessor( const std::vector<std::size_t>& populationDimensions,
				    const std::size_t numberOfInputs,
				    const double potentialPercentage,
				    const double sparsity,
				    const std::vector<std::size_t>& dynamicUnits,
				    const std::array<double,2>& weightLimits )
	// explicitly call base-class constructor
	: StaticProcessor(populationDimensions, numberOfInputs, potentialPercentage, sparsity, weightLimits)
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

} // end DynamicProcessor constructor


// constructor initializes populationDimensions and numberOfInputs with variables supplied as arguments.
// This loads _dynamicUnits with previous vector supplied as argument too
DynamicProcessor::DynamicProcessor( std::stringstream& inputStream,
				    const std::string& dynamicProcessorIdentification )
	// explicitly call base-class constructor
	: StaticProcessor(inputStream, dynamicProcessorIdentification)
{
	inputStream.clear();
	inputStream.str(inputStream.str());
	DynamicProcessor::loadDynamicProcessorStatus(dynamicProcessorIdentification, inputStream);
} // end DynamicProcessor constructor


// updates a group of _dynamicUnits depending on a set of unit indexes.
// on the other hand, every link -from which information is coming- is a vector
// hence, it could contain a set of linking units as well as a unique linking unit.
void	DynamicProcessor::Update( const std::vector<std::size_t>& indexes,
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
responseInfo	DynamicProcessor::getDynamicResponse( const responseInfo& response,
						      const twodvector<std::size_t>& linkingUnits )
{
	assert(linkingUnits.size() == _dynamicUnits.size());
	assert(response.excitation.size() == _unitsDimensionality);

	std::vector<double>	totalDynamicProcessor;
	responseInfo	newResponse;

	totalDynamicProcessor.resize(_unitsDimensionality);
	newResponse = response;

	#pragma omp parallel for default(none) shared(linkingUnits,totalDynamicProcessor) num_threads(1)
	for( std::size_t row = 0; row < _unitsDimensionality; row++ ) {
		double	auxiliary = 0.0;
		for( std::size_t link = 0; link < _numberOfLinks; link++) {
			for ( std::size_t connection = 0; connection < linkingUnits[link].size(); connection++ ) {
				auto	potentialIndex =
					find_first_coincident_index(_potentialConnections[link][row],
								    linkingUnits[link][connection]);

				if ( potentialIndex < _potentialDimensionality[link] )
					auxiliary += _dynamicUnits[link][row][potentialIndex];

			}
		}
		totalDynamicProcessor[row] += auxiliary;
	}

	std::transform(newResponse.excitation.begin(), newResponse.excitation.end(),
		       totalDynamicProcessor.begin(),
		       newResponse.excitation.begin(), std::multiplies<double>());

	newResponse.ranking = sort_indexes(newResponse.excitation);

	return	newResponse;
} // end function getDynamicResponse


// function to save the DynamicProcessor' status in a file
void	DynamicProcessor::saveDynamicProcessorStatus( const std::string dynamicProcessorIdentification,
						      stringstream& outfile )
{
	std::string	str = "DynamicProcessor_";
	std::string	STR;
	str += dynamicProcessorIdentification;
	str += "_";
	for ( std::size_t link = 0; link < _dynamicUnits.size(); link++ ) {
		// saves _dynamicUnits
		STR = "dynamicUnits_" + std::to_string(link);
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_of_vectors_conditionally_as_sparse_array(str+STR,_dynamicUnits[link],SPARSITY_THRESHOLD,outfile);
		else
			save_vector_of_vectors_conditionally_as_sparse_matrix(str+STR,_dynamicUnits[link],SPARSITY_THRESHOLD,outfile);

		STR.clear();
	}

	for ( std::size_t link = 0; link < _potentialConnections.size(); link++ ) {
		// saves _potentialConnections
		STR = "potentialConnections_" + std::to_string(link);
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_of_vectors_as_numeric_array(str+STR,_potentialConnections[link],outfile);
		else
			save_vector_of_vectors_as_matrix(str+STR,_potentialConnections[link],outfile);

		STR.clear();
	}

        // saves _numberOfLinks
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_scalar_as_numeric_array(str + "numberOfLinks", _numberOfLinks, outfile);
	else
		save_as_scalar(str + "numberOfLinks", _numberOfLinks, outfile);

        // saves _potentialDimensionality
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_vector_as_numeric_array(str + "potentialDimensionality", _potentialDimensionality, outfile);
	else
		save_vector_as_matrix(str + "potentialDimensionality", _potentialDimensionality, outfile);

        // saves _potentialPercentage
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_scalar_as_numeric_array(str + "potentialPercentage", _potentialPercentage, outfile);
	else
		save_as_scalar(str + "potentialPercentage", _potentialPercentage, outfile);

        // saves _updateStep
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_scalar_as_numeric_array(str + "updateStep", _updateStep, outfile);
	else
		save_as_scalar(str + "updateStep", _updateStep, outfile);

} // end functiom saveDynamicProcessorStatus


// function to load the DynamicProcessor' status from a file
void	DynamicProcessor::loadDynamicProcessorStatus( const std::string dynamicProcessorIdentification,
						      stringstream& infile )
{
	std::string	str;
	std::string	STR = "DynamicProcessor_";
	std::string	auxiliary;
	STR += dynamicProcessorIdentification;
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
} // end functiom loadDynamicProcessorStatus






