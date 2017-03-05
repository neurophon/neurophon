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
#include "StrongPopulation.h"				// includes definition of class StrongPopulation
#include "../Libraries/Utilities.h"
#include "../Libraries/Templates.h"

// constructor initializes populationDimensions and numberOfInputs with variables supplied as arguments
StrongPopulation::StrongPopulation( const std::vector<int>& populationDimensions, int numberOfInputs )
	// explicitly call base-class constructor
	: StrongUnits(populationDimensions, numberOfInputs)
{
} // end StrongPopulation constructor


// returns the normalized coordinates of the best active unit
// depending on response info and on _strongUnits.
std::vector<double>	StrongPopulation::Activate( const responseInfo& response, bool normalize )
{
	responseInfo	newResponse;

	newResponse = StrongPopulation::getStrongResponse(response);

	if ( normalize )
		return	StrongPopulation::Normalize(unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality));
	else {
		auto intVector = unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality);
		std::vector<double>	doubleVector(intVector.begin(), intVector.end());
		return	doubleVector;
	}
} // end function Activate


// returns the normalized coordinates of the best active unit
// depending on input and on _strongUnits.
std::vector<double>	StrongPopulation::Activate( const std::vector<double>& input, bool normalize )
{
	auto	newResponse = StrongPopulation::getStrongResponse(SelfOrganizingMap::getResponse(input));

	if ( normalize )
		return	StrongPopulation::Normalize(unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality));
	else {
		auto intVector = unravelIndex(newResponse.ranking[0], _unitsArrayDimensionality);
		std::vector<double>	doubleVector(intVector.begin(), intVector.end());
		return	doubleVector;
	}
} // end function Activate


// decides which unit in the population to activate depending on response info.
std::vector<double>	StrongPopulation::Activate( const responseInfo& response, double activationRadius, bool normalize )
{
	if ( response.distances[response.ranking[1]] <= activationRadius ) {		// more than one unit is inside activationRadius
		auto	distanceIndexes = less_than_indexes(response.distances,activationRadius);
		auto	newResponse = StrongPopulation::getStrongResponse(response);
		auto	newDistances = get_elements(newResponse.distances, distanceIndexes);
		double	newDistanceMinimum = get_minimum_element(newDistances);
		int	index = get_index_from_value(newResponse.distances, newDistanceMinimum);
		if ( normalize )
			return	StrongPopulation::Normalize(unravelIndex(index, _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(index, _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
	else if ( response.distances[response.ranking[0]] <= activationRadius ) {	// just the best matching unit is inside activationRadius
		if ( normalize )
			return	StrongPopulation::Normalize(unravelIndex(response.ranking[0], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(response.ranking[0], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
	else {										// no unit is inside activation radius
		std::vector<double>	coordinates;
		coordinates.resize(_unitsArrayDimensionality.size());
		std::fill(coordinates.begin(), coordinates.end(), -1);
		return	coordinates;
	}
} // end function Activate


// decides which unit in the population to activate depending on input.
std::vector<double>	StrongPopulation::Activate( const std::vector<double>& input, double activationRadius, bool normalize )
{
	auto	response = SelfOrganizingMap::getResponse(input);

	if ( response.distances[response.ranking[1]] <= activationRadius ) {		// more than one unit is inside activationRadius
		auto	distanceIndexes = less_than_indexes(response.distances,activationRadius);
		auto	newResponse = StrongPopulation::getStrongResponse(response);
		auto	newDistances = get_elements(newResponse.distances, distanceIndexes);
		double	newDistanceMinimum = get_minimum_element(newDistances);
		int	index = get_index_from_value(newResponse.distances, newDistanceMinimum);
		if ( normalize )
			return	StrongPopulation::Normalize(unravelIndex(index, _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(index, _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
	else if ( response.distances[response.ranking[0]] <= activationRadius ) {	// just the best matching unit is inside activationRadius
		if ( normalize )
			return	StrongPopulation::Normalize(unravelIndex(response.ranking[0], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(response.ranking[0], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
	else {										// no unit is inside activation radius
		std::vector<double>	coordinates;
		coordinates.resize(_unitsArrayDimensionality.size());
		std::fill(coordinates.begin(), coordinates.end(), -1);
		return	coordinates;
	}
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










