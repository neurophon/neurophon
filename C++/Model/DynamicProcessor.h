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

// File Name:		DynamicProcessor.h
// File Description:	DynamicProcessor class definition. This file presents DynamicProcessor' public
//			interface without revealing the implementations of DynamicProcessor' member functions,
//			which are defined in DynamicProcessor.cpp.


// DynamicProcessor class derived from class StaticProcessor through inheritance.
// Class Description: This is a self organizing feature map kind class.
// It elaborates a statistic relationship between different units activity.
// This is about the frequency with which certain units activity happens.
// This is specially useful to represent the statistical relationship between the units activities
// produced in different time steps. 

#ifndef DYNAMIC_PROCESSOR_H
#define DYNAMIC_PROCESSOR_H

#include <fstream>

#include "Structs.h"							// includes struct difinitions
#include "../Libraries/Model/DataTypes.h"
#include "StaticProcessor.h"						// StaticProcessor class declaration

class DynamicProcessor : public StaticProcessor
{
public:
		DynamicProcessor( const std::vector<std::size_t>&,
				  const std::size_t,
				  const double,
				  const double,
				  const std::vector<std::size_t>&,
				  const std::array<double,2>& );		// constructor that initializes _dynamicUnits by default to zero
										// the first parameter is the dimensionality of the array of units,
										// for example (20,10,5) (a prisma with 1000 units);
										// the second one is the number of input components and the
										// third one is a vector in which every component is the number of units
										// that can be connected from the same or another group of units with this one.
										// The StaticProcessor weights willbe initialized with random numbers
										// between weightLimits

		DynamicProcessor( const std::string&,
				  const std::string& );				// constructor that initializes _dynamicUnits by means of previous
										// values from file

	void	Update( const std::vector<std::size_t>&,
		       	const twodvector<std::size_t>&,
			const bool,
		       	const double, const double );				// updates a group of _dynamicUnits depending on a set of unit indexes.
										// on the other hand, every link -from which information is coming- is a vector
										// hence, it could contain a set of linking units as well as a unique linking unit.
	responseInfo	getDynamicResponse( const responseInfo&,
					    const twodvector<std::size_t>& );	// modifies response ussing _dynamicUnits
										// the response is a responseInfo type structure which is compose of
										// a vector with distances and an vector with indexes sorthed in ascending order
										// corresponding to the vector of distances
	void	saveDynamicProcessorStatus( const std::string,
					    std::ofstream& );			// function that saves the DynamicProcessor' status in a file

	void	loadDynamicProcessorStatus( const std::string,
					    std::ifstream& );			// function to save the DynamicProcessor' status from a file

protected:
	threedvector<double>			_dynamicUnits;				// matrixes with dynamic units stats

private:
	std::size_t				_numberOfLinks;				// number of links to other arrays in _dynamicUnits and _potentialConnections
	std::size_t				_updateStep;				// accumulates the steps elapsed until now in order to check UPDATE_PERIOD
	double					_potentialPercentage;			// this is the percentage of potential connections to the linking inputs
	std::vector<std::size_t>		_potentialDimensionality;		// dimensionality of the potential linking connections
	threedvector<std::size_t>		_potentialConnections;			// matrix with the potential connections to the linking inputs


}; // end class DynamicProcessor

#endif

