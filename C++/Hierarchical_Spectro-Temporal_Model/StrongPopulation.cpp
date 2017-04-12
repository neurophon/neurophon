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

// File Name:		StrongPopulation.cpp
// File Description:	StrongPopulation member-function definitions. This file contains implementations of the
//			member functions prototyped in StrongPopulation.h.

#include<algorithm>
#include <random>

#include "StrongPopulation.h"				// includes definition of class StrongPopulation
#include "../Libraries/Utilities.h"
#include "../Libraries/Templates.h"
#include "../Libraries/Random.h"

// constructor initializes populationDimensions and numberOfInputs with variables supplied as arguments
// this initializes _strongUnits to zero by default
StrongPopulation::StrongPopulation( const std::vector<int>& populationDimensions, const int numberOfInputs )
	// explicitly call base-class constructor
	: StrongUnits(populationDimensions, numberOfInputs)
{
} // end StrongPopulation constructor


// constructor initializes populationDimensions and numberOfInputs with variables supplied as arguments
// this initializes _strongUnits from previous vector
StrongPopulation::StrongPopulation( const std::vector<int>& populationDimensions, const int numberOfInputs,
				    const std::vector<double>& strongUnits )
	// explicitly call base-class constructor
	: StrongUnits(populationDimensions, numberOfInputs, strongUnits)
{
} // end StrongPopulation constructor


// returns the normalized coordinates of the best active unit
// depending on response info and on _strongUnits.
std::vector<double>	StrongPopulation::Activate( const responseInfo& response, const bool normalize )
{
	auto	newResponse = StrongPopulation::getStrongResponse(response);

	auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newResponse.distances[newResponse.ranking[0]]);

	if ( minimumIndexes.size() > 1 ) {
		auto	number = rand() % minimumIndexes.size();
		if ( normalize )
			return	StrongPopulation::Normalize(unravelIndex(minimumIndexes[number], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(minimumIndexes[number], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
	else {
		if ( normalize )
			return	StrongPopulation::Normalize(unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
} // end function Activate


// returns the normalized coordinates of the best active unit
// depending on input and on _strongUnits.
std::vector<double>	StrongPopulation::Activate( const std::vector<double>& input, const bool normalize )
{
	auto	newResponse = StrongPopulation::getStrongResponse(SelfOrganizingMap::getResponse(input));

	auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newResponse.distances[newResponse.ranking[0]]);

	if ( minimumIndexes.size() > 1 ) {
		auto	number = rand() % minimumIndexes.size();
		if ( normalize )
			return	StrongPopulation::Normalize(unravelIndex(minimumIndexes[number], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(minimumIndexes[number], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
	else {
		if ( normalize )
			return	StrongPopulation::Normalize(unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
} // end function Activate


// returns the normalized coordinates of the "n" best active units
// depending on response info and on _strongUnits.
twodvector<double>	StrongPopulation::Activate( const responseInfo& response, const bool normalize,
						    int numberOfActiveUnits )
{
	twodvector<double>	output;
	output.resize(numberOfActiveUnits);

	auto	newResponse = StrongPopulation::getStrongResponse(response);

	int	rankingPosition = 0;
	int	offset = 0;
	while ( numberOfActiveUnits > 0 ) {
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances,
								newResponse.distances[newResponse.ranking[rankingPosition]]);

		minimumIndexes = sampleVector(minimumIndexes,minimumIndexes.size());
		if ( (int)minimumIndexes.size() > numberOfActiveUnits ) {
			for ( int iter = 0; iter < numberOfActiveUnits; iter++ ) {
				if ( normalize )
					output[iter+offset] = StrongPopulation::Normalize(unravelIndex(minimumIndexes[iter],
												 _unitsArrayDimensionality));
				else {
					auto intVector = unravelIndex(minimumIndexes[iter], _unitsArrayDimensionality);
					std::vector<double>	doubleVector(intVector.begin(), intVector.end());
					output[iter+offset] = doubleVector;
				}
			}
		}
		else {
			for ( int index = 0; index < (int)minimumIndexes.size(); index++ ) {
				if ( normalize )
					output[index+offset] = StrongPopulation::Normalize(unravelIndex(minimumIndexes[index],
												 _unitsArrayDimensionality));
				else {
					auto intVector = unravelIndex(minimumIndexes[index], _unitsArrayDimensionality);
					std::vector<double>	doubleVector(intVector.begin(), intVector.end());
					output[index+offset] = doubleVector;
				}
			}
		}
		numberOfActiveUnits -= (int)minimumIndexes.size();
		offset += (int)minimumIndexes.size();
		rankingPosition++;
	}
	return	output;
} // end function Activate


// returns the normalized coordinates of the "n" best active units
// depending on input and on _strongUnits.
twodvector<double>	StrongPopulation::Activate( const std::vector<double>& input, const bool normalize,
						    int numberOfActiveUnits )
{
	twodvector<double>	output;
	output.resize(numberOfActiveUnits);

	auto	newResponse = StrongPopulation::getStrongResponse(SelfOrganizingMap::getResponse(input));

	int	rankingPosition = 0;
	int	offset = 0;
	while ( numberOfActiveUnits > 0 ) {
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances,
								newResponse.distances[newResponse.ranking[rankingPosition]]);

		minimumIndexes = sampleVector(minimumIndexes,minimumIndexes.size());
		if ( (int)minimumIndexes.size() > numberOfActiveUnits ) {
			for ( int iter = 0; iter < numberOfActiveUnits; iter++ ) {
				if ( normalize )
					output[iter+offset] = StrongPopulation::Normalize(unravelIndex(minimumIndexes[iter],
												 _unitsArrayDimensionality));
				else {
					auto intVector = unravelIndex(minimumIndexes[iter], _unitsArrayDimensionality);
					std::vector<double>	doubleVector(intVector.begin(), intVector.end());
					output[iter+offset] = doubleVector;
				}
			}
		}
		else {
			for ( int index = 0; index < (int)minimumIndexes.size(); index++ ) {
				if ( normalize )
					output[index+offset] = StrongPopulation::Normalize(unravelIndex(minimumIndexes[index],
												 _unitsArrayDimensionality));
				else {
					auto intVector = unravelIndex(minimumIndexes[index], _unitsArrayDimensionality);
					std::vector<double>	doubleVector(intVector.begin(), intVector.end());
					output[index+offset] = doubleVector;
				}
			}
		}
		numberOfActiveUnits -= (int)minimumIndexes.size();
		offset += (int)minimumIndexes.size();
		rankingPosition++;
	}
	return	output;
} // end function Activate


// decides which unit in the population to activate depending on response info.
twodvector<double>	StrongPopulation::Activate( const responseInfo& response, const double activationRadius,
						    const bool normalize )
{
	twodvector<double>	output;

	if ( response.distances[response.ranking[1]] <= activationRadius ) {		// more than one unit is inside activationRadius
		auto	distanceIndexes = less_than_or_equal_to_indexes(response.distances,activationRadius);
		auto	newResponse = StrongPopulation::getStrongResponse(response);
		auto	newDistances = get_elements(newResponse.distances, distanceIndexes);
		double	newDistanceMinimum = get_minimum_element(newDistances);
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
		auto	aptToActivate = coincidence_indexes(distanceIndexes,minimumIndexes);
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(aptToActivate.begin(), aptToActivate.end(), g);
		if ( normalize )
			for ( int number = 0; number < (int)aptToActivate.size(); number++ )
				output.push_back(StrongPopulation::Normalize(unravelIndex(distanceIndexes[aptToActivate[number]], _unitsArrayDimensionality)));
		else {
			for ( int number = 0; number < (int)aptToActivate.size(); number++ ) {
				auto intVector = unravelIndex(distanceIndexes[aptToActivate[number]], _unitsArrayDimensionality);
				std::vector<double>	doubleVector(intVector.begin(), intVector.end());
				output.push_back(doubleVector);
			}
		}
	}
	else if ( response.distances[response.ranking[0]] <= activationRadius ) {	// just the best matching unit is inside activationRadius
		if ( normalize )
			output.push_back(StrongPopulation::Normalize(unravelIndex(response.ranking[0], _unitsArrayDimensionality)));
		else {
			auto intVector = unravelIndex(response.ranking[0], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			output.push_back(doubleVector);
		}
	}
	else {										// no unit is inside activation radius
		std::vector<double>	coordinates;
		coordinates.resize(_unitsArrayDimensionality.size());
		std::fill(coordinates.begin(), coordinates.end(), -1);
		output.push_back(coordinates);
	}

	output.shrink_to_fit();
	return	output;
} // end function Activate


// decides which unit in the population to activate depending on input.
twodvector<double>	StrongPopulation::Activate( const std::vector<double>& input, const double activationRadius,
						    const bool normalize )
{
	twodvector<double>	output;
	auto	response = SelfOrganizingMap::getResponse(input);

	if ( response.distances[response.ranking[1]] <= activationRadius ) {		// more than one unit is inside activationRadius
		auto	distanceIndexes = less_than_or_equal_to_indexes(response.distances,activationRadius);
		auto	newResponse = StrongPopulation::getStrongResponse(response);
		auto	newDistances = get_elements(newResponse.distances, distanceIndexes);
		double	newDistanceMinimum = get_minimum_element(newDistances);
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
		auto	aptToActivate = coincidence_indexes(distanceIndexes,minimumIndexes);
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(aptToActivate.begin(), aptToActivate.end(), g);
		if ( normalize )
			for ( int number = 0; number < (int)aptToActivate.size(); number++ )
				output.push_back(StrongPopulation::Normalize(unravelIndex(distanceIndexes[aptToActivate[number]], _unitsArrayDimensionality)));
		else {
			for ( int number = 0; number < (int)aptToActivate.size(); number++ ) {
				auto intVector = unravelIndex(distanceIndexes[aptToActivate[number]], _unitsArrayDimensionality);
				std::vector<double>	doubleVector(intVector.begin(), intVector.end());
				output.push_back(doubleVector);
			}
		}
	}
	else if ( response.distances[response.ranking[0]] <= activationRadius ) {	// just the best matching unit is inside activationRadius
		if ( normalize )
			output.push_back(StrongPopulation::Normalize(unravelIndex(response.ranking[0], _unitsArrayDimensionality)));
		else {
			auto intVector = unravelIndex(response.ranking[0], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			output.push_back(doubleVector);
		}
	}
	else {										// no unit is inside activation radius
		std::vector<double>	coordinates;
		coordinates.resize(_unitsArrayDimensionality.size());
		std::fill(coordinates.begin(), coordinates.end(), -1);
		output.push_back(coordinates);
	}

	output.shrink_to_fit();
	return	output;
} // end function Activate


// normalizes neuron coorsinates in the population
std::vector<double>	StrongPopulation::Normalize( const std::vector<int>& coordinates )
{
	std::vector<double>	normalizedCoordinates;

	normalizedCoordinates.resize(_unitsArrayDimensionality.size());

	std::transform(coordinates.begin(), coordinates.end(), _unitsArrayDimensionality.begin(),
		       normalizedCoordinates.begin(), std::divides<double>());

	return	normalizedCoordinates;
} // end function Normalize


// modifies response ussing _strongUnits
responseInfo	StrongPopulation::getStrongResponse( const responseInfo& response )
{
	responseInfo	newResponse;

	newResponse.distances.resize(response.distances.size());

	std::transform(response.distances.begin(), response.distances.end(), _strongUnits.begin(),
		       newResponse.distances.begin(), std::divides<double>());

	newResponse.ranking = sort_indexes(newResponse.distances);

	return	newResponse;
} // end function Normalize










