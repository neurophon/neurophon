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

// File Name:		TemporalPopulation.cpp
// File Description:	TemporalPopulation member-function definitions. This file contains implementations of the
//			member functions prototyped in TemporalPopulation.h.

#include<algorithm>
#include <random>

#include "TemporalPopulation.h"				// includes definition of class TemporalPopulation
#include "../Libraries/Random.h"
#include "../Libraries/Utilities.h"
#include "../Libraries/Templates.h"

// constructor initializes populationDimensions, numberOfInputs and temporalUnits with variables supplied as arguments
TemporalPopulation::TemporalPopulation( const std::vector<int>& populationDimensions, int numberOfInputs,
					const std::vector<int>& temporalUnits )
	// explicitly call base-class constructor
	: TemporalUnits(populationDimensions, numberOfInputs, temporalUnits)
{
} // end TemporalPopulation constructor


// constructor initializes populationDimensions, numberOfInputs and temporalUnits with variables supplied as arguments
TemporalPopulation::TemporalPopulation( const std::string& fileName, const std::string& temporalUnitsIdentification )
	// explicitly call base-class constructor
	: TemporalUnits(fileName, temporalUnitsIdentification)
{
} // end TemporalPopulation constructor


// returns the normalized coordinates of the best active unit
// depending on response info and on _temporalUnits.
std::vector<double>	TemporalPopulation::Activate( const responseInfo& response,
						      const std::vector<int>& linkingUnits, const bool normalize )
{
	auto	newResponse = TemporalPopulation::getTemporalResponse(response, linkingUnits);

	auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newResponse.distances[newResponse.ranking[0]]);

	if ( minimumIndexes.size() > 1 ) {
		auto	number = rand() % minimumIndexes.size();
		if ( normalize )
			return	TemporalPopulation::Normalize(unravelIndex(minimumIndexes[number], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(minimumIndexes[number], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
	else {
		if ( normalize )
			return	TemporalPopulation::Normalize(unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
} // end function Activate


// returns the normalized coordinates of the best active unit
// depending on input and on _temporalUnits.
std::vector<double>	TemporalPopulation::Activate( const std::vector<double>& input,
						      const std::vector<int>& linkingUnits, const bool normalize )
{
	auto	newResponse = TemporalPopulation::getTemporalResponse(SelfOrganizingMap::getResponse(input), linkingUnits);

	auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newResponse.distances[newResponse.ranking[0]]);

	if ( minimumIndexes.size() > 1 ) {
		auto	number = rand() % minimumIndexes.size();
		if ( normalize )
			return	TemporalPopulation::Normalize(unravelIndex(minimumIndexes[number], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(minimumIndexes[number], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
	else {
		if ( normalize )
			return	TemporalPopulation::Normalize(unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
} // end function Activate


// returns the normalized coordinates of the "n" best active units
// depending on response info and on _temporalUnits.
twodvector<double>	TemporalPopulation::Activate( const responseInfo& response, const std::vector<int>& linkingUnits,
						      const bool normalize, int numberOfActiveUnits )
{
	twodvector<double>	output;

	if ( numberOfActiveUnits > _unitsDimensionality )
	{
		std::cout << "TemporalPopulation object inconsistence.\n";
		std::cout << "In function Activate.\n";
		std::cout << "numberOfActiveUnits = " << numberOfActiveUnits << "\n";
		std::cout << "while _unitsDimensionality = " << _unitsDimensionality << "\n";
		exit( EXIT_FAILURE );
	}

	output.resize(numberOfActiveUnits);

	auto	newResponse = TemporalPopulation::getTemporalResponse(response, linkingUnits);

	int	rankingPosition = 0;
	int	offset = 0;
	while ( numberOfActiveUnits > 0 ) {
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances,
								newResponse.distances[newResponse.ranking[rankingPosition]]);

		minimumIndexes = sampleVector(minimumIndexes,minimumIndexes.size());
		if ( (int)minimumIndexes.size() > numberOfActiveUnits ) {
			for ( int iter = 0; iter < numberOfActiveUnits; iter++ ) {
				if ( normalize )
					output[iter+offset] = TemporalPopulation::Normalize(unravelIndex(minimumIndexes[iter],
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
					output[index+offset] = TemporalPopulation::Normalize(unravelIndex(minimumIndexes[index],
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
// depending on input and on _temporalUnits.
twodvector<double>	TemporalPopulation::Activate( const std::vector<double>& input, const std::vector<int>& linkingUnits,
						      const bool normalize, int numberOfActiveUnits )
{
	twodvector<double>	output;

	if ( numberOfActiveUnits > _unitsDimensionality )
	{
		std::cout << "TemporalPopulation object inconsistence.\n";
		std::cout << "In function Activate.\n";
		std::cout << "numberOfActiveUnits = " << numberOfActiveUnits << "\n";
		std::cout << "while _unitsDimensionality = " << _unitsDimensionality << "\n";
		exit( EXIT_FAILURE );
	}

	output.resize(numberOfActiveUnits);

	auto	newResponse = TemporalPopulation::getTemporalResponse(SelfOrganizingMap::getResponse(input), linkingUnits);

	int	rankingPosition = 0;
	int	offset = 0;
	while ( numberOfActiveUnits > 0 ) {
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances,
								newResponse.distances[newResponse.ranking[rankingPosition]]);

		minimumIndexes = sampleVector(minimumIndexes,minimumIndexes.size());
		if ( (int)minimumIndexes.size() > numberOfActiveUnits ) {
			for ( int iter = 0; iter < numberOfActiveUnits; iter++ ) {
				if ( normalize )
					output[iter+offset] = TemporalPopulation::Normalize(unravelIndex(minimumIndexes[iter],
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
					output[index+offset] = TemporalPopulation::Normalize(unravelIndex(minimumIndexes[index],
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
twodvector<double>	TemporalPopulation::Activate( const responseInfo& response, const std::vector<int>& linkingUnits,
						      const double activationRadius, const bool normalize )
{
	twodvector<double>	output;

	if ( response.distances[response.ranking[1]] <= activationRadius ) {		// more than one unit is inside activationRadius
		auto	distanceIndexes = less_than_or_equal_to_indexes(response.distances,activationRadius);
		auto	newResponse = TemporalPopulation::getTemporalResponse(response, linkingUnits);
		auto	newDistances = get_elements(newResponse.distances, distanceIndexes);
		double	newDistanceMinimum = get_minimum_element(newDistances);
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
		auto	aptToActivate = coincidence_indexes(distanceIndexes,minimumIndexes);
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(aptToActivate.begin(), aptToActivate.end(), g);
		if ( normalize )
			for ( int number = 0; number < (int)aptToActivate.size(); number++ )
				output.push_back(TemporalPopulation::Normalize(unravelIndex(distanceIndexes[aptToActivate[number]], _unitsArrayDimensionality)));
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
			output.push_back(TemporalPopulation::Normalize(unravelIndex(response.ranking[0], _unitsArrayDimensionality)));
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
twodvector<double>	TemporalPopulation::Activate( const std::vector<double>& input, const std::vector<int>& linkingUnits,
						      const double activationRadius, const bool normalize )
{
	twodvector<double>	output;
	auto	response = SelfOrganizingMap::getResponse(input);

	if ( response.distances[response.ranking[1]] <= activationRadius ) {		// more than one unit is inside activationRadius
		auto	distanceIndexes = less_than_or_equal_to_indexes(response.distances,activationRadius);
		auto	newResponse = TemporalPopulation::getTemporalResponse(response, linkingUnits);
		auto	newDistances = get_elements(newResponse.distances, distanceIndexes);
		double	newDistanceMinimum = get_minimum_element(newDistances);
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
		auto	aptToActivate = coincidence_indexes(distanceIndexes,minimumIndexes);
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(aptToActivate.begin(), aptToActivate.end(), g);
		if ( normalize )
			for ( int number = 0; number < (int)aptToActivate.size(); number++ )
				output.push_back(TemporalPopulation::Normalize(unravelIndex(distanceIndexes[aptToActivate[number]], _unitsArrayDimensionality)));
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
			output.push_back(TemporalPopulation::Normalize(unravelIndex(response.ranking[0], _unitsArrayDimensionality)));
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


// returns the index of the best active unit
// depending on response info and on _temporalUnits.
int	TemporalPopulation::Activate( const responseInfo& response,
						      const std::vector<int>& linkingUnits )
{
	auto	newResponse = TemporalPopulation::getTemporalResponse(response, linkingUnits);

	auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newResponse.distances[newResponse.ranking[0]]);

	if ( minimumIndexes.size() > 1 ) {
		auto	number = rand() % minimumIndexes.size();
		return	minimumIndexes[number];
	}
	else {
		return	newResponse.ranking[0];
	}
} // end function Activate


// returns the index of the best active unit
// depending on input and on _temporalUnits.
int	TemporalPopulation::Activate( const std::vector<double>& input,
						      const std::vector<int>& linkingUnits )
{
	auto	newResponse = TemporalPopulation::getTemporalResponse(SelfOrganizingMap::getResponse(input), linkingUnits);

	auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newResponse.distances[newResponse.ranking[0]]);

	if ( minimumIndexes.size() > 1 ) {
		auto	number = rand() % minimumIndexes.size();
		return	minimumIndexes[number];
	}
	else {
		return	newResponse.ranking[0];
	}
} // end function Activate


// returns the indexes of the "n" best active units
// depending on response info and on _temporalUnits.
std::vector<int>	TemporalPopulation::Activate( const responseInfo& response, const std::vector<int>& linkingUnits,
						      int numberOfActiveUnits )
{
	std::vector<int>	output;

	if ( numberOfActiveUnits > _unitsDimensionality )
	{
		std::cout << "TemporalPopulation object inconsistence.\n";
		std::cout << "In function Activate.\n";
		std::cout << "numberOfActiveUnits = " << numberOfActiveUnits << "\n";
		std::cout << "while _unitsDimensionality = " << _unitsDimensionality << "\n";
		exit( EXIT_FAILURE );
	}

	output.resize(numberOfActiveUnits);

	auto	newResponse = TemporalPopulation::getTemporalResponse(response, linkingUnits);

	int	rankingPosition = 0;
	int	offset = 0;
	while ( numberOfActiveUnits > 0 ) {
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances,
								newResponse.distances[newResponse.ranking[rankingPosition]]);

		minimumIndexes = sampleVector(minimumIndexes,minimumIndexes.size());
		if ( (int)minimumIndexes.size() > numberOfActiveUnits ) {
			for ( int index = 0; index < numberOfActiveUnits; index++ )
				output[index+offset] = minimumIndexes[index];
		}
		else {
			for ( int index = 0; index < (int)minimumIndexes.size(); index++ )
				output[index+offset] = minimumIndexes[index];
		}
		numberOfActiveUnits -= (int)minimumIndexes.size();
		offset += (int)minimumIndexes.size();
		rankingPosition++;
	}
	return	output;
} // end function Activate


// returns the indexes of the "n" best active units
// depending on input and on _temporalUnits.
std::vector<int>	TemporalPopulation::Activate( const std::vector<double>& input, const std::vector<int>& linkingUnits,
						      int numberOfActiveUnits )
{
	std::vector<int>	output;

	auto	response = TemporalPopulation::getTemporalResponse(SelfOrganizingMap::getResponse(input), linkingUnits);

	if ( numberOfActiveUnits > _unitsDimensionality )
	{
		std::cout << "TemporalPopulation object inconsistence.\n";
		std::cout << "In function Activate.\n";
		std::cout << "numberOfActiveUnits = " << numberOfActiveUnits << "\n";
		std::cout << "while _unitsDimensionality = " << _unitsDimensionality << "\n";
		exit( EXIT_FAILURE );
	}

	output.resize(numberOfActiveUnits);

	auto	newResponse = TemporalPopulation::getTemporalResponse(response, linkingUnits);

	int	rankingPosition = 0;
	int	offset = 0;
	while ( numberOfActiveUnits > 0 ) {
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances,
								newResponse.distances[newResponse.ranking[rankingPosition]]);

		minimumIndexes = sampleVector(minimumIndexes,minimumIndexes.size());
		if ( (int)minimumIndexes.size() > numberOfActiveUnits ) {
			for ( int index = 0; index < numberOfActiveUnits; index++ )
				output[index+offset] = minimumIndexes[index];
		}
		else {
			for ( int index = 0; index < (int)minimumIndexes.size(); index++ )
				output[index+offset] = minimumIndexes[index];
		}
		numberOfActiveUnits -= (int)minimumIndexes.size();
		offset += (int)minimumIndexes.size();
		rankingPosition++;
	}
	return	output;
} // end function Activate


// decides which unit in the population to activate depending on response info.
std::vector<int>	TemporalPopulation::Activate( const responseInfo& response, const std::vector<int>& linkingUnits,
						      const double activationRadius )
{
	std::vector<int>	output;

	if ( response.distances[response.ranking[1]] <= activationRadius ) {		// more than one unit is inside activationRadius
		auto	distanceIndexes = less_than_or_equal_to_indexes(response.distances,activationRadius);
		auto	newResponse = TemporalPopulation::getTemporalResponse(response, linkingUnits);
		auto	newDistances = get_elements(newResponse.distances, distanceIndexes);
		double	newDistanceMinimum = get_minimum_element(newDistances);
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
		auto	aptToActivate = coincidence_indexes(distanceIndexes,minimumIndexes);
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(aptToActivate.begin(), aptToActivate.end(), g);

		for ( int number = 0; number < (int)aptToActivate.size(); number++ )
			output.push_back(distanceIndexes[aptToActivate[number]]);
	}
	else if ( response.distances[response.ranking[0]] <= activationRadius ) {	// just the best matching unit is inside activationRadius
		output.push_back(response.ranking[0]);
	}
	else {										// no unit is inside activation radius
		output.push_back(-1);
	}

	output.shrink_to_fit();
	return	output;
} // end function Activate


// decides which unit in the population to activate depending on input.
std::vector<int>	TemporalPopulation::Activate( const std::vector<double>& input, const std::vector<int>& linkingUnits,
						      const double activationRadius )
{
	std::vector<int>	output;
	auto	response = SelfOrganizingMap::getResponse(input);

	if ( response.distances[response.ranking[1]] <= activationRadius ) {		// more than one unit is inside activationRadius
		auto	distanceIndexes = less_than_or_equal_to_indexes(response.distances,activationRadius);
		auto	newResponse = TemporalPopulation::getTemporalResponse(response, linkingUnits);
		auto	newDistances = get_elements(newResponse.distances, distanceIndexes);
		double	newDistanceMinimum = get_minimum_element(newDistances);
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
		auto	aptToActivate = coincidence_indexes(distanceIndexes,minimumIndexes);
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(aptToActivate.begin(), aptToActivate.end(), g);

		for ( int number = 0; number < (int)aptToActivate.size(); number++ )
			output.push_back(distanceIndexes[aptToActivate[number]]);
	}
	else if ( response.distances[response.ranking[0]] <= activationRadius ) {	// just the best matching unit is inside activationRadius
		output.push_back(response.ranking[0]);
	}
	else {										// no unit is inside activation radius
		output.push_back(-1);
	}

	output.shrink_to_fit();
	return	output;
} // end function Activate


// decides which unit in the population to activate depending on response info.
std::vector<int>	TemporalPopulation::Activate( const responseInfo& response, const twodvector<int>& linkingUnits,
						      const double activationRadius )
{
	std::vector<int>	output;

	if ( response.distances[response.ranking[1]] <= activationRadius ) {		// more than one unit is inside activationRadius
		auto	distanceIndexes = less_than_or_equal_to_indexes(response.distances,activationRadius);
		auto	newResponse = TemporalPopulation::getTemporalResponse(response, linkingUnits);
		auto	newDistances = get_elements(newResponse.distances, distanceIndexes);
		double	newDistanceMinimum = get_minimum_element(newDistances);
		auto	minimumIndexes = get_indexes_from_value(newResponse.distances, newDistanceMinimum);
		auto	aptToActivate = coincidence_indexes(distanceIndexes,minimumIndexes);
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(aptToActivate.begin(), aptToActivate.end(), g);

		for ( int number = 0; number < (int)aptToActivate.size(); number++ )
			output.push_back(distanceIndexes[aptToActivate[number]]);
	}
	else if ( response.distances[response.ranking[0]] <= activationRadius ) {	// just the best matching unit is inside activationRadius
		output.push_back(response.ranking[0]);
	}
	else {										// no unit is inside activation radius
		output.push_back(-1);
	}

	output.shrink_to_fit();
	return	output;
} // end function Activate


// modifies response ussing _temporalUnits
responseInfo	TemporalPopulation::getTemporalResponse( const responseInfo& response,
							 const std::vector<int>& linkingUnits )
{
	assert(linkingUnits.size() == _temporalUnits.size());
	responseInfo	newResponse;

	newResponse.distances = response.distances;

	for ( int link = 0; link < (int)_temporalUnits.size(); link++ ) {
		if ( linkingUnits[link] != -1 )
			std::transform(newResponse.distances.begin(), newResponse.distances.end(),
				       _temporalUnits[link][linkingUnits[link]].begin(),
				       newResponse.distances.begin(), std::divides<double>());
	}

	newResponse.ranking = sort_indexes(newResponse.distances);

	return	newResponse;
} // end function getTemporalResponse


// modifies response ussing _temporalUnits
responseInfo	TemporalPopulation::getTemporalResponse( const responseInfo& response,
							 twodvector<int> linkingUnits )
{
	assert(linkingUnits.size() == _temporalUnits.size());
	std::vector<std::size_t>    aux;
	responseInfo	newResponse;

	newResponse.distances = response.distances;

	if (!is_rectangular(linkingUnits))
		make_rectangular(linkingUnits,-1,aux);

	auto	numberOfLinks = linkingUnits.size();
	auto	numberOfAlternatives = linkingUnits[0].size();
	for ( std::size_t alternative = 0; alternative < numberOfAlternatives; alternative++ ) {
		for ( std::size_t link = 0; link < numberOfLinks; link++ ) {
			if ( linkingUnits[link][alternative] != -1 )
				std::transform(newResponse.distances.begin(), newResponse.distances.end(),
					       _temporalUnits[link][linkingUnits[link][alternative]].begin(),
					       newResponse.distances.begin(), std::divides<double>());
		}
	}

	newResponse.ranking = sort_indexes(newResponse.distances);

	return	newResponse;
} // end function getTemporalResponse


// normalizes neuron coorsinates in the population
std::vector<double>	TemporalPopulation::Normalize( const std::vector<int>& coordinates )
{
	std::vector<double>	normalizedCoordinates;

	normalizedCoordinates.resize(_unitsArrayDimensionality.size());

	std::transform(coordinates.begin(), coordinates.end(), _unitsArrayDimensionality.begin(),
		       normalizedCoordinates.begin(), std::divides<double>());

	return	normalizedCoordinates;
} // end function Normalize








