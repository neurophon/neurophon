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

// File Name:		Utilities.h
// File Description:	Utilities definitions. This file presents Utilities's interface
//			without revealing the implementations of Utilities's functions,
//			which are defined in Utilities.cpp.

#ifndef UTILITIES_H
#define UTILITIES_H

#include <array>
#include <vector>
#include "Constants.h"
#include "DataTypes.h"

/* Converts a flat index into an array whose components are array coordinates.
   Parameters:
   ----------------------------
   index:             The flat index.
   dimensionsOfArray: An array with DIM elements holding the dimensions of the array to unravel.*/
std::vector<std::size_t>	unravelIndex( std::size_t,
	       				      const std::vector<std::size_t>& );


/* Converts array coordinates into an array flat index.
   Parameters:
   ----------------------------
   coordinatesOfArray: An array with DIM elements holding the coordinates of the array to ravel.
   dimensionsOfArray: An array with DIM elements holding the dimensions of the array to ravel.*/
std::size_t	ravelIndex( const std::vector<std::size_t>&,
			    const std::vector<std::size_t>& );


// Irregular cartesian product, equivalent to a nested for-loop
std::vector<std::size_t>	cartesianProductI( const twodvector<std::size_t>& );


// Cartesian product, equivalent to a nested for-loop.
std::vector<std::size_t>	cartesianProduct( const twodvector<std::size_t>& );


// Returns the positive of (i % n)
int positiveMod( int , int );


// Returns a vector with the indexes whose vector values are true
std::vector<std::size_t>	trueIndexes( const std::vector<bool>& );

// computes the percentage of true elements in vector of bool
double	truePercentage( const std::vector<bool>& );

#endif
