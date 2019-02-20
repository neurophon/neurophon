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

// File Name:		ComplexProcessor.cpp
// File Description:	ComplexProcessor member-function definitions. This file contains implementations of the
//			member functions prototyped in ComplexProcessor.h.

#include <fstream>
#include<algorithm>
#include <random>

#include "ComplexProcessor.h"				// includes definition of class ComplexProcessor
#include "../Libraries/Model/Random.h"
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/Templates.h"

using namespace std;

// constructor initializes populationDimensions, proximalConnectivity and dynamicUnits with variables supplied as arguments
// StaticUnits weights are initialized with random numbers between weight limits
ComplexProcessor::ComplexProcessor( const std::vector<std::size_t>& populationDimensions,
				    const std::vector<double>& proximalConnectivity,
				    const std::vector<double>& alinearityFactors,
				    const double proximalPotentialPercentage,
				    const double distalPotentialPercentage,
				    const double sparsity,
				    const std::vector<std::size_t>& dynamicUnits,
				    const std::array<double,2>& weightLimits )
	// explicitly call base-class constructor
	: DynamicProcessor(populationDimensions,
			   proximalConnectivity,
			   alinearityFactors,
			   proximalPotentialPercentage,
			   distalPotentialPercentage,
			   sparsity,
			   dynamicUnits,
			   weightLimits)
{
} // end ComplexProcessor constructor


// constructor initializes populationDimensions, proximalConnectivity and dynamicUnits with variables supplied as arguments
ComplexProcessor::ComplexProcessor( std::stringstream& inputStream,
				    const std::string& dynamicUnitsIdentification )
	// explicitly call base-class constructor
	: DynamicProcessor(inputStream, dynamicUnitsIdentification)
{
} // end ComplexProcessor constructor


// decides which units in the population to activate depending on response info.
std::vector<std::size_t>	ComplexProcessor::Activate( const responseInfo& response,
							    const twodvector<std::size_t>& linkingUnits,
							    const std::size_t numberOfExcitedUnits,
							    const double sparsity,
	       						    const bool randomness )
{
	assert(sparsity <= 1.0 && sparsity > 0.0);
	std::vector<std::size_t>	output;

	std::size_t	minimumNumberOfActiveUnits = ((1.0-sparsity)*_unitsDimensionality);
	assert(numberOfExcitedUnits > minimumNumberOfActiveUnits);
	
	std::vector<std::size_t>	excitedUnits;
	if ( !randomness ) {
		excitedUnits.insert(excitedUnits.end(),
				    response.ranking.end()-std::min(numberOfExcitedUnits,response.ranking.size()),
				    response.ranking.end());
	}
	else {
		excitedUnits = getRandomWeightedIndexes(response.excitation,
							std::min(numberOfExcitedUnits,response.excitation.size()));
	}

	auto	newResponse = DynamicProcessor::getDynamicResponse(response, linkingUnits);
	auto	newExcitations = get_elements(newResponse.excitation, excitedUnits);
	double	newExcitationMaximum = get_maximum_element(newExcitations);
	auto	maximumIndexes = get_indexes_from_value(newResponse.excitation, newExcitationMaximum);
	auto	aptToActivate = coincidence_indexes(excitedUnits,maximumIndexes);
	newExcitations.erase(std::remove(newExcitations.begin(), newExcitations.end(), newExcitationMaximum), newExcitations.end());
	while ( aptToActivate.size() < minimumNumberOfActiveUnits &&
		newExcitations.size() > 0 ) {
		newExcitationMaximum = get_maximum_element(newExcitations);
		maximumIndexes = get_indexes_from_value(newResponse.excitation, newExcitationMaximum);
		auto	partialAptToActivate = coincidence_indexes(excitedUnits,maximumIndexes);
		aptToActivate.insert(aptToActivate.end(), partialAptToActivate.begin(), partialAptToActivate.end());
		newExcitations.erase(std::remove(newExcitations.begin(), newExcitations.end(), newExcitationMaximum), newExcitations.end());
	}

	// if random behaviour is enabled then randomize the order of the active indexes in the output
	if ( ENABLE_RANDOM_BEHAVIOUR ) {
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(aptToActivate.begin(), aptToActivate.end(), g);
	}

	for ( std::size_t number = 0; number < aptToActivate.size(); number++ )
		output.push_back(excitedUnits[aptToActivate[number]]);
		
	output.shrink_to_fit();
	assert(output.size() != 0);
	return	output;
} // end function Activate


// function to save the ComplexProcessor' status in a file
void	ComplexProcessor::saveComplexProcessorStatus( const std::string& complexProcessorIdentification,
						      stringstream& outputStream )
{
	StaticProcessor::saveStaticProcessorStatus(complexProcessorIdentification,
	       							   outputStream);

	DynamicProcessor::saveDynamicProcessorStatus(complexProcessorIdentification,
								     outputStream);
} // end functiom saveComplexProcessorStatus



