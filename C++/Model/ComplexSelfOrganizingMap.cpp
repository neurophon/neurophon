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

// File Name:		ComplexSelfOrganizingMap.cpp
// File Description:	ComplexSelfOrganizingMap member-function definitions. This file contains implementations of the
//			member functions prototyped in ComplexSelfOrganizingMap.h.

#include<algorithm>
#include <random>
#include <fstream>

#include "ComplexSelfOrganizingMap.h"				// includes definition of class ComplexSelfOrganizingMap
#include "../Libraries/Model/Random.h"
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/Templates.h"

using namespace std;

// constructor initializes populationDimensions, numberOfInputs and dynamicUnits with variables supplied as arguments
ComplexSelfOrganizingMap::ComplexSelfOrganizingMap( const std::vector<std::size_t>& populationDimensions,
						    const std::size_t numberOfInputs,
						    const double potentialPercentage,
						    const std::vector<std::size_t>& dynamicUnits )
	// explicitly call base-class constructor
	: DynamicSelfOrganizingMap(populationDimensions, numberOfInputs, potentialPercentage, dynamicUnits)
{
} // end ComplexSelfOrganizingMap constructor


// constructor initializes populationDimensions, numberOfInputs and dynamicUnits with variables supplied as arguments
// StaticSelfOrganizingMap weights are initialized with random numbers between weight limits
ComplexSelfOrganizingMap::ComplexSelfOrganizingMap( const std::vector<std::size_t>& populationDimensions,
						    const std::size_t numberOfInputs,
						    const double potentialPercentage,
						    const std::vector<std::size_t>& dynamicUnits,
						    const std::array<double,2>& weightLimits )
	// explicitly call base-class constructor
	: DynamicSelfOrganizingMap(populationDimensions, numberOfInputs, potentialPercentage, dynamicUnits, weightLimits)
{
} // end ComplexSelfOrganizingMap constructor


// constructor initializes populationDimensions, numberOfInputs and dynamicUnits with variables supplied as arguments
ComplexSelfOrganizingMap::ComplexSelfOrganizingMap( std::stringstream& inputStream,
						    const std::string& dynamicUnitsIdentification )
	// explicitly call base-class constructor
	: DynamicSelfOrganizingMap(inputStream, dynamicUnitsIdentification)
{
} // end ComplexSelfOrganizingMap constructor


// decides which unit in the population to activate depending on response info.
std::vector<std::size_t>	ComplexSelfOrganizingMap::Activate( const somResponseInfo& response,
								    const twodvector<std::size_t>& linkingUnits,
								    const double activationRadius )
{
	std::vector<std::size_t>	output;

	if ( response.distances[response.ranking[1]] <= activationRadius ) {		// more than one unit is inside activationRadius
		auto	distanceIndexes = less_than_or_equal_to_indexes(response.distances,activationRadius);
		auto	newResponse = DynamicSelfOrganizingMap::getDynamicResponse(response, linkingUnits);
		auto	newDistances = get_elements(newResponse.distances, distanceIndexes);
		double	newDistanceMinimum = get_minimum_element(newDistances);
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
		auto	aptToActivate = coincidence_indexes(distanceIndexes,minimumIndexes);

		// if random behaviour is enabled then randomize the order of the active indexes in the output
		if ( ENABLE_RANDOM_BEHAVIOUR ) {
			std::random_device rd;
			std::mt19937 g(rd());
			std::shuffle(aptToActivate.begin(), aptToActivate.end(), g);
		}

		for ( std::size_t number = 0; number < aptToActivate.size(); number++ )
			output.push_back(distanceIndexes[aptToActivate[number]]);

		output.shrink_to_fit();
	}
	else if ( response.distances[response.ranking[0]] <= activationRadius ) {	// just the best matching unit is inside activationRadius
		output.push_back(response.ranking[0]);
	}

	return	output;
} // end function Activate


// decides which units in the population to activate depending on response info.
std::vector<std::size_t>	ComplexSelfOrganizingMap::Activate( const somResponseInfo& response,
								    const twodvector<std::size_t>& linkingUnits,
								    const double activationRadius,
								    const double sparsity )
{
	assert(sparsity <= 1.0 && sparsity > 0.0);
	std::vector<std::size_t>	output;

	std::size_t	minimumNumberOfActiveUnits = ((1.0-sparsity)*_unitsDimensionality);
	if ( response.distances[response.ranking[1]] <= activationRadius ) {		// more than one unit is below activationRadius
		auto	distanceIndexes = less_than_or_equal_to_indexes(response.distances,activationRadius);
		auto	newResponse = DynamicSelfOrganizingMap::getDynamicResponse(response, linkingUnits);
		auto	newDistances = get_elements(newResponse.distances, distanceIndexes);
		double	newDistanceMinimum = get_minimum_element(newDistances);
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
		auto	aptToActivate = coincidence_indexes(distanceIndexes,minimumIndexes);
		newDistances.erase(std::remove(newDistances.begin(), newDistances.end(), newDistanceMinimum), newDistances.end());
		while ( aptToActivate.size() < minimumNumberOfActiveUnits &&
			newDistances.size() > 0 ) {
			newDistanceMinimum = get_minimum_element(newDistances);
			minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
			auto	partialAptToActivate = coincidence_indexes(distanceIndexes,minimumIndexes);
			aptToActivate.insert(aptToActivate.end(), partialAptToActivate.begin(), partialAptToActivate.end());
			newDistances.erase(std::remove(newDistances.begin(), newDistances.end(), newDistanceMinimum), newDistances.end());
		}

		// if random behaviour is enabled then randomize the order of the active indexes in the output
		if ( ENABLE_RANDOM_BEHAVIOUR ) {
			std::random_device rd;
			std::mt19937 g(rd());
			std::shuffle(aptToActivate.begin(), aptToActivate.end(), g);
		}

		for ( std::size_t number = 0; number < aptToActivate.size(); number++ )
			output.push_back(distanceIndexes[aptToActivate[number]]);

		output.shrink_to_fit();
	}
	else if ( response.distances[response.ranking[0]] <= activationRadius ) {	// just the best matching unit is below activationRadius
		output.push_back(response.ranking[0]);
	}

	return	output;
} // end function Activate


// decides which units in the population to activate depending on response info.
std::vector<std::size_t>	ComplexSelfOrganizingMap::Activate( const somResponseInfo& response,
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
				    response.ranking.begin(),
				    response.ranking.begin()+std::min(numberOfExcitedUnits,response.ranking.size()));
	}
	else {
		auto	vec = response.distances;
		transform(vec.begin(), vec.end(), vec.begin(), &reciprocal<double>);
		excitedUnits = getRandomWeightedIndexes(vec,std::min(numberOfExcitedUnits,response.distances.size()));
	}

	auto	newResponse = DynamicSelfOrganizingMap::getDynamicResponse(response, linkingUnits);
	auto	newDistances = get_elements(newResponse.distances, excitedUnits);
	double	newDistanceMinimum = get_minimum_element(newDistances);
	auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
	auto	aptToActivate = coincidence_indexes(excitedUnits,minimumIndexes);
	newDistances.erase(std::remove(newDistances.begin(), newDistances.end(), newDistanceMinimum), newDistances.end());
	while ( aptToActivate.size() < minimumNumberOfActiveUnits &&
		newDistances.size() > 0 ) {
		newDistanceMinimum = get_minimum_element(newDistances);
		minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
		auto	partialAptToActivate = coincidence_indexes(excitedUnits,minimumIndexes);
		aptToActivate.insert(aptToActivate.end(), partialAptToActivate.begin(), partialAptToActivate.end());
		newDistances.erase(std::remove(newDistances.begin(), newDistances.end(), newDistanceMinimum), newDistances.end());
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


// function to save the ComplexSelfOrganizingMap' status in a file
void	ComplexSelfOrganizingMap::saveComplexSelfOrganizingMapStatus( const std::string& complexSelfOrganizingMapIdentification,
								      std::stringstream& outStream )
{
	StaticSelfOrganizingMap::saveStaticSelfOrganizingMapStatus(complexSelfOrganizingMapIdentification,
	       							   outStream);

	DynamicSelfOrganizingMap::saveDynamicSelfOrganizingMapStatus(complexSelfOrganizingMapIdentification,
								     outStream);
} // end functiom saveComplexSelfOrganizingMapStatus



