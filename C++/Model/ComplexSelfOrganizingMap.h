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

// File Name:		ComplexSelfOrganizingMap.h
// File Description:	ComplexSelfOrganizingMap class definition. This file presents ComplexSelfOrganizingMap' public
//			interface without revealing the implementations of ComplexSelfOrganizingMap' member functions,
//			which are defined in ComplexSelfOrganizingMap.cpp.


// ComplexSelfOrganizingMap class derived from class DynamicSelfOrganizingMap through inheritance.
// Class Description: This is a self organizing feature map kind class.
// It represents a bunch of neurons (units) arranged in an d-dimensional array.
// Its activaton function determines a unique active neuron per ejecution.

#ifndef COMPLEX_SOM_H
#define COMPLEX_SOM_H

#include <fstream>

#include "Structs.h"									// includes structs difinition
#include "DynamicSelfOrganizingMap.h"								// DynamicSelfOrganizingMap class declaration

class ComplexSelfOrganizingMap : public DynamicSelfOrganizingMap
{
public:
				ComplexSelfOrganizingMap( const std::vector<std::size_t>&,
							  const std::size_t,
							  const double,
							  const std::vector<std::size_t>& );	// constructor that initializes ComplexSelfOrganizingMap
												// _dynamicUnits are initialized to zero

				ComplexSelfOrganizingMap( const std::vector<std::size_t>&,
							  const std::size_t,
							  const double,
							  const std::vector<std::size_t>&,
							  const std::array<double,2>& );	// constructor that initializes ComplexSelfOrganizingMap
												// _dynamicUnits are initialized to zero
												// SelfOrganizingMap weights are initialized with random
												// values between weight limits

				ComplexSelfOrganizingMap( std::stringstream&,
							  const std::string& );		// constructor that initializes ComplexSelfOrganizingMap
											// _dynamicUnits are initialized from previous vectors

	std::vector<std::size_t>	Activate( const somResponseInfo&,
						  const twodvector<std::size_t>&,
						  const double );			// decides which unit in the population to activate
											// depending on response info.
											// in this case we have a vector of vectors of linking units,
											// all those linking units are used to modify response
	std::vector<std::size_t>	Activate( const somResponseInfo&,
						  const twodvector<std::size_t>&,
						  const double,
						  const double );			// decides which units in the population to activate
											// depending on response info.
											// in this case we have a vector of vectors of linking units,
											// all those linking units are used to modify response
	
	std::vector<std::size_t>	Activate( const somResponseInfo&,
						  const twodvector<std::size_t>&,
						  const std::size_t,
						  const double,
	       					  const bool randomness = false );	// decides which units in the population to activate
       											// depending on response info.
											// in this case we have a vector of vectors of linking units,
											// all those linking units are used to modify response.
											// For this function there exist the posibility of introduce
											// weighted randomness in the process of selection of the
											// excited units
	void	saveComplexSelfOrganizingMapStatus( const std::string&,
						    std::stringstream& );		// function to save the ComplexSelfOrganizingMap' status in a file


}; // end class ComplexSelfOrganizingMap

#endif

