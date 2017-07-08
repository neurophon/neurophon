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
#include <cassert>
#include <random>

#include "Templates.h"


// Choose sampleSize elements randomly from a vector containing vector.size() elements.
std::vector<int>	sampleVector( std::vector<int> vec, size_t sampleSize )
{
	assert(vec.size() >= sampleSize);
	std::vector<int>	sample;

	random_unique(vec.begin(), vec.end(), sampleSize);
	for ( std::size_t i = 0; i < sampleSize; i++ )
		sample.push_back (vec[i]);

	sample.shrink_to_fit();
	return	sample;
} // end function sampleVector


// chooses a random double number which satisfies:
// a <= returned < b
double	randomFromDoubleInterval( double a, double b )
{
    assert(a<=b);
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
} // end function randomFromDoubleInterval


// this function produces random indices on the interval of indices of v (let's say [0 n)),
// where the probability of each individual index i is defined by 
// the weight of the value which corresponds to the ith index in v with respect to
// the sum of all n weights (values), in v.
// This function uses std::discrete_distribution which
// satisfies all requirements of RandomNumberDistribution
std::vector<int>	getRandomWeightedIndexes( std::vector<double> v,
	       					  const std::size_t n )
{
	assert(n < v.size());
	std::vector<int>	v1;
	std::random_device rd;
	std::mt19937 gen(rd());
	for(std::size_t i = 0; i < n; i++) {
		if ( std::all_of(v.begin(), v.end(), [](double i) { return i==0.0; }) ) {
			std::vector<double>    v2(v.size(),1.0);
			set_elements(v2,v1,0.0);
			auto    size = v1.size();
			while (size < n) {
				std::discrete_distribution<> d1(v2.begin(),v2.end());
				auto    index = d1(gen);
				v2[index] = 0.0;
				v1.push_back(index);
				size++;
			}
			break;
		}

		std::discrete_distribution<> d(v.begin(),v.end());
		auto    index = d(gen);
		v[index] = 0.0;
		v1.push_back(index);
	}
	v1.shrink_to_fit();
	assert(v1.size() == n);

	return	v1;
} // end function getRandomWeightedIndexes
