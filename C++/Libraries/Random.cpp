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

// File Name:		Random.cpp
// File Description:	Functions member-function definitions. This file contains implementations of the
//			functions prototyped in Functions.h.
//
#include <vector>
#include <cstdlib>
#include <iterator>

#include "Templates.h"


// Choose sampleSize elements randomly from a vector containing vector.size() elements.
std::vector<int>	sampleVector( std::vector<int> vector, size_t sampleSize )
{
	std::vector<int>	sample;

	random_unique(vector.begin(), vector.end(), sampleSize);
	for ( int i = 0; i < (int)sampleSize; i++ )
		sample.push_back (vector[i]);

	sample.shrink_to_fit();
	return	sample;
} // end function sampleVector


// chooses a random double number which satisfies:
// a <= returned < b
double	randomFromDoubleInterval( double a, double b )
{
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
} // end function randomFromDoubleInterval

