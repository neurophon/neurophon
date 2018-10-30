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

// File Name:		ComplexProcessor.h
// File Description:	ComplexProcessor class definition. This file presents ComplexProcessor' public
//			interface without revealing the implementations of ComplexProcessor' member functions,
//			which are defined in ComplexProcessor.cpp.


// ComplexProcessor class derived from class DynamicProcessor through inheritance.
// Class Description: This is a self organizing feature map kind class.
// It represents a bunch of neurons (units) arranged in an d-dimensional array.
// Its activaton function determines a unique active neuron per ejecution.

#ifndef COMPLEX_PROCESSOR_H
#define COMPLEX_PROCESSOR_H

#include <fstream>

#include "Structs.h"									// includes structs difinition
#include "DynamicProcessor.h"								// DynamicProcessor class declaration

class ComplexProcessor : public DynamicProcessor
{
public:
				ComplexProcessor( const std::vector<std::size_t>&,
						  const std::size_t,
						  const double,
						  const double,
						  const double,
						  const std::vector<std::size_t>&,
						  const std::array<double,2>& );	// constructor that initializes ComplexProcessor
											// _dynamicUnits are initialized to zero
											// Processor weights are initialized with random
											// values between weight limits

				ComplexProcessor( std::stringstream&,
						  const std::string& );			// constructor that initializes ComplexProcessor
											// _dynamicUnits are initialized from previous vectors

	std::vector<std::size_t>	Activate( const responseInfo&,
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
	void	saveComplexProcessorStatus( const std::string&,
					    std::stringstream& );			// function to save the ComplexProcessor' status in a file


}; // end class ComplexProcessor

#endif

