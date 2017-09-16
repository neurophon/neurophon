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

// File Name:		Topology.cpp
// File Description:	Topology member-function definitions. This file contains implementations of the
//			functions prototyped in Topology.h.
//
#include <iostream>
#include <vector>

#include "Topology.h"				// include definition of function Topology
#include "Utilities.h"
#include "Constants.h"

using namespace std;

// Gets the points in the neighborhood of a point.
std::vector<std::size_t>	neighborhood( const std::size_t centerIndex,
	       				      const std::vector<std::size_t>& radius,
					      const std::vector<std::size_t>& dimensions )
{
	int	left, right;
	std::vector<std::size_t>	coords, neighbors;
	std::vector<std::size_t>	centerPosition, aux;
	std::vector<std::vector<std::size_t>>	intervals;

	intervals.resize(dimensions.size());
	aux.resize(dimensions.size());

	centerPosition = unravelIndex(centerIndex, dimensions);

	for ( std::size_t i = 0; i < dimensions.size(); i++ )
	{
		left = max(0,((int)centerPosition[i]-(int)radius[i]));
		right = min(((int)dimensions[i]-1),((int)centerPosition[i]+(int)radius[i]));

		intervals[i].push_back(left);
		intervals[i].push_back(right);
	}

	coords = cartesianProductI(intervals);
	for ( int i = 0; i < (int)(coords.size()/dimensions.size()); i++ )
	{
		for ( int j = 0; j < (int)dimensions.size(); j++ )
			aux[j] = coords[j+i*dimensions.size()];

		neighbors.push_back(ravelIndex(aux, dimensions));
	}
	neighbors.shrink_to_fit();
	return	neighbors;
} // end function neighborhood


// Like 'neighborhood', 'wrappingNeighborhood' excepts that the neighborhood isn't
// truncated when it's near an edge. It wraps around to the other side.
std::vector<std::size_t>	wrappingNeighborhood( const std::size_t centerIndex,
	       					      const std::vector<std::size_t>& radius,
						      const std::vector<std::size_t>& dimensions )
{
	int	left, right;
	std::vector<std::size_t>	coords, neighbors;
	std::vector<std::size_t>	centerPosition, aux;
	std::vector<std::vector<std::size_t>>	intervals;

	intervals.resize(dimensions.size());
	centerPosition = unravelIndex(centerIndex, dimensions);
	aux.resize(dimensions.size());

	for ( int i = 0; i < (int)dimensions.size(); i++ )
	{
		left = int((int)centerPosition[i]-(int)radius[i]);
		right = min(left+((int)dimensions[i]-1),((int)centerPosition[i]+(int)radius[i]));

		for ( int j = left; j <= right; j++ )
			intervals[i].push_back(positiveMod(j,dimensions[i]));

		intervals[i].shrink_to_fit();
	}

	coords = cartesianProduct(intervals);
	for ( int i = 0; i < (int)(coords.size()/dimensions.size()); i++ )
	{
		for ( int j = 0; j < (int)dimensions.size(); j++ )
			aux[j] = coords[j+i*dimensions.size()];

		neighbors.push_back(ravelIndex(aux, dimensions));
	}
	neighbors.shrink_to_fit();
	return	neighbors;
} // end function wrappingNeighborhood





