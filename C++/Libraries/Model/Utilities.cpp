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

// File Name:		Utilities.cpp
// File Description:	Utilities member-function definitions. This file contains implementations of the
//			functions prototyped in Utilities.h.
//
#include <iostream>
#include <array>
#include <algorithm>

#include "Utilities.h"				// include definition of function Utilities
#include "Constants.h"

using namespace std;

// Converts a flat index into an vector whose components are array coordinates.
std::vector<std::size_t>	unravelIndex( std::size_t index,
	       				      const std::vector<std::size_t>& dimensionsOfArray)
{
	std::size_t	numberOfCoordinates, arrayLength = 1;
	std::vector<std::size_t>	coordinates;

	coordinates.resize(dimensionsOfArray.size());

	for(const auto& s: dimensionsOfArray)
        	arrayLength *= s;

	if ( index >= arrayLength )
	{
		std::cout << "\nUtilities inconsistency:" << endl;
		std::cout << "\nIn function unravelIndex:" << endl;
		cout << "index is bigger than the number of elements in the array\n" << endl;
		cout << "you have, index = " << index << ", and the number of elements in the array is " << arrayLength << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	numberOfCoordinates = dimensionsOfArray.size();		// Returns the number of elements in the container

	if ( numberOfCoordinates == 1 )
	{
		coordinates[0] = index;
		return	coordinates;
	}
	else
	{
		for ( int i = (int)numberOfCoordinates-1; i >= 0; i-- )
		{
			if ( dimensionsOfArray[i] == 0 )
			{
				std::cout << "\nUtilities inconsistency:" << endl;
				std::cout << "\nIn function unravelIndex:" << endl;
				cout << "at least one array coordinate is <= 0\n" << endl;
				exit( EXIT_FAILURE );
			}
			coordinates[i] = index % dimensionsOfArray[i];
			index = index/dimensionsOfArray[i];
		}
	}

	return	coordinates;
} // end function unravelIndex


// Converts array coordinates into an array flat index.
std::size_t	ravelIndex( const std::vector<std::size_t>& coordinatesOfArray,
			    const std::vector<std::size_t>& dimensionsOfArray )
{
	std::size_t	index = 0, product, numberOfCoordinates;

	numberOfCoordinates = dimensionsOfArray.size();		// Returns the number of elements in the container

	if ( numberOfCoordinates == 1 )
	{
		index = coordinatesOfArray[0];
		return	index;
	}
	else
	{
		for ( std::size_t i = 0; i < numberOfCoordinates; i++ )
		{
			if ( dimensionsOfArray[i] == 0 || coordinatesOfArray[i] >= dimensionsOfArray[i] )
			{
				std::cout << "\nUtilities inconsistency:" << endl;
				std::cout << "\nIn function ravelIndex:" << endl;
				cout << "at least one array coordinate is <= 0, or some coordinate >= some dimension" << endl;
				exit( EXIT_FAILURE );
			}

			product = 1;
			if ( i < numberOfCoordinates )
			{
				for ( std::size_t j = 1+i; j < numberOfCoordinates; j++ )
					product *= dimensionsOfArray[j];
			}

			index += product*coordinatesOfArray[i];
		}
	}

	return	index;
} // end function ravelIndex


// Irregular cartesian product, equivalent to a nested for-loop.
// note: This function only receives inferior and superior limits of every interval (not the complete intervals).
// This feature allows the function to receive irregular intervals for each dimension
// (not all the intervals have to have the same number of elements).
std::vector<std::size_t>	cartesianProductI( const twodvector<std::size_t>& intervals )
{
	std::size_t	step = 1;
	std::vector<std::size_t>::iterator	it;
	std::vector<std::size_t>	coordinates, aux, temp;

	for ( int i = (int)intervals.size()-1; i >= 0; i--)
	{
		if ( step == 1 )
		{
			if ( intervals[i][0] > intervals[i][1] )
			{
				std::cout << "\nUtilities inconsistency:" << endl;
				std::cout << "\nIn function cartesianProductI:" << endl;
				cout << "Corrupt intervals" << endl;
				exit( EXIT_FAILURE );
			}

			for ( std::size_t j = intervals[i][0]; j <= intervals[i][1]; j++ )
			{
				coordinates.push_back(j);
			}
			step++;
		}
		else
		{
			aux.clear();
			if ( intervals[i][0] > intervals[i][1] )
			{
				std::cout << "\nUtilities inconsistency:" << endl;
				std::cout << "\nIn function cartesianProductI:" << endl;
				cout << "Corrupt intervals" << endl;
				exit( EXIT_FAILURE );
			}

			for ( std::size_t j = intervals[i][0]; j <= intervals[i][1]; j++ )
			{
				temp = coordinates;
				for ( it = temp.begin(); it < temp.end(); it += step )
					it = temp.insert (it, j);

				aux.reserve(aux.size() + temp.size());
				aux.insert(aux.end(), temp.begin(), temp.end());
			}
			step++;
			coordinates = aux;
		}
	}
	coordinates.shrink_to_fit();
	return	coordinates;
} // end function cartesianProductI


// Cartesian product, equivalent to a nested for-loop.
std::vector<std::size_t>	cartesianProduct( const twodvector<std::size_t>& intervals )
{
	std::size_t	step = 1;//, size;
	std::vector<std::size_t>::iterator	it;
	std::vector<std::size_t>	coordinates, aux, temp;

	//size = intervals[0].size();
	for ( int i = (int)intervals.size()-1; i >= 0; i--)
	{
		/*if ( size != (int)intervals[i].size() )
		{
			std::cout << "\nUtilities inconsistency:" << endl;
			std::cout << "\nIn function cartesianProduct:" << endl;
			cout << "Corrupt intervals" << endl;
			exit( EXIT_FAILURE );
		}*/

		if ( step == 1 )
		{
			coordinates.reserve(intervals[i].size());
			coordinates.insert(coordinates.end(), intervals[i].begin(), intervals[i].end());
			step++;
		}
		else
		{
			aux.clear();
			for ( std::size_t j = 0; j < intervals[i].size(); j++ )
			{
				temp = coordinates;
				for ( it = temp.begin(); it < temp.end(); it += step )
					it = temp.insert (it, intervals[i][j]);

				aux.reserve(aux.size() + temp.size());
				aux.insert(aux.end(), temp.begin(), temp.end());
			}
			step++;
			coordinates = aux;
		}
	}
	coordinates.shrink_to_fit();
	return	coordinates;
} // end function cartesianProduct


// Returns the positive of (i % n)
int	positiveMod(int i, int n)
{
    return (i % n + n) % n;
} // end function positiveMod


// Returns a vector with the indexes whose vector values are true
std::vector<std::size_t>	trueIndexes( const std::vector<bool> &vector )
{
	std::vector<std::size_t>	indices;

	auto it = std::find_if(std::begin(vector), std::end(vector), [](bool element){return element;});
	while (it != std::end(vector)) {
	   indices.emplace_back(std::distance(std::begin(vector), it));
	   it = std::find_if(std::next(it), std::end(vector), [](bool element){return element;});
	}
	indices.shrink_to_fit();

	return	indices;
} // end function trueIndexes


// computes the percentage of true elements in vector of bool
double	truePercentage( const std::vector<bool>& v )
{
	return	(double)(std::count(v.begin(), v.end(), true))/(double)v.size();
} // end function truePercentage


// this is for OS auto-detection
std::string getOsName()
{
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __unix || __unix__
    return "Unix";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #else
    return "Other";
    #endif
} 


// this checkts if this machine is little endian
int	is_big_endian()
{
	union {
		uint32_t i;
		char c[4];
	} bint = {0x01020304};

	return bint.c[0] == 1; 
}



