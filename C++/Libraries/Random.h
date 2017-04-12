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

// File Name:		Random.h
// File Description:	Random definitions. This file presents Random's interface
//			without revealing the implementations of Random's functions,
//			which are defined in Random.cpp.

#include <vector>

#include "Constants.h"


// Choose sampleSize elements randomly from a vector containing vector.size() elements.
// This function uses random_unique template.
std::vector<int>	sampleVector( std::vector<int>, size_t );


// chooses a random double number which satisfies:
// a <= returned < b
double	randomFromDoubleInterval( double, double );
