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

// File Name:		SimplePopulation.cpp
// File Description:	SimplePopulation member-function definitions. This file contains implementations of the
//			member functions prototyped in SimplePopulation.h.

#include<algorithm>
#include "SimplePopulation.h"				// includes definition of class SimplePopulation
#include "../Libraries/Utilities.h"

// constructor initializes populationDimensions and numberOfInputs with variables supplied as arguments
SimplePopulation::SimplePopulation( const std::vector<int>& populationDimensions, int numberOfInputs )
	// explicitly call base-class constructor
	: SelfOrganizingMap(populationDimensions, numberOfInputs)
{
} // end SimplePopulation constructor


// returns the normalized coordinates of the best matching unit using response information.
std::vector<double>	SimplePopulation::Activate( const responseInfo& response, bool normalize )
{
	if ( normalize )
		return	SimplePopulation::Normalize(unravelIndex(response.ranking[0], _unitsArrayDimensionality));
	else {
		auto intVector = unravelIndex(response.ranking[0], _unitsArrayDimensionality);
		std::vector<double>	doubleVector(intVector.begin(), intVector.end());
		return	doubleVector;
	}
} // end function Activate


// returns the normalized coordinates of the best matching unit using input.
std::vector<double>	SimplePopulation::Activate( const std::vector<double>& input, bool normalize )
{
	responseInfo	response;

	response = SelfOrganizingMap::getResponse(input);

	if ( normalize )
		return	SimplePopulation::Normalize(unravelIndex(response.ranking[0], _unitsArrayDimensionality));
	else {
		auto intVector = unravelIndex(response.ranking[0], _unitsArrayDimensionality);
		std::vector<double>	doubleVector(intVector.begin(), intVector.end());
		return	doubleVector;
	}
} // end function Activate


// decides which units in the population to activate depending on response information.
std::vector<double>	SimplePopulation::Activate( const responseInfo& response, double activationRadius, bool normalize )
{
	if ( response.distances[response.ranking[0]] <= activationRadius ) {
		if ( normalize )
			return	SimplePopulation::Normalize(unravelIndex(response.ranking[0], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(response.ranking[0], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
	else {
		std::vector<double>	coordinates;
		coordinates.resize(_unitsArrayDimensionality.size());
		std::fill(coordinates.begin(), coordinates.end(), -1);
		return	coordinates;
	}
} // end function Activate


// decides which units in the population to activate depending on input.
std::vector<double>	SimplePopulation::Activate( const std::vector<double>& input, double activationRadius, bool normalize )
{
	auto	response = SelfOrganizingMap::getResponse(input);

	if ( response.distances[response.ranking[0]] <= activationRadius ) {
		if ( normalize )
			return	SimplePopulation::Normalize(unravelIndex(response.ranking[0], _unitsArrayDimensionality));
		else {
			auto intVector = unravelIndex(response.ranking[0], _unitsArrayDimensionality);
			std::vector<double>	doubleVector(intVector.begin(), intVector.end());
			return	doubleVector;
		}
	}
	else {
		std::vector<double>	coordinates;
		coordinates.resize(_unitsArrayDimensionality.size());
		std::fill(coordinates.begin(), coordinates.end(), -1);
		return	coordinates;
	}
} // end function Activate


// normalizes neuron coorsinates in the population
std::vector<double>	SimplePopulation::Normalize( const std::vector<int>& coordinates )
{
	std::vector<double>	normalizedCoordinates;

	normalizedCoordinates.resize(_unitsArrayDimensionality.size());

	std::transform(coordinates.begin(), coordinates.end(), _unitsArrayDimensionality.begin(),
		       normalizedCoordinates.begin(), std::divides<double>());

	return	normalizedCoordinates;
} // end function Normalize


