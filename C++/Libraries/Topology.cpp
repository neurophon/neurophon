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
std::vector<int>	neighborhood( const int centerIndex, const std::vector<int>& radius, const std::vector<int>& dimensions )
{
	int	left, right;
	std::vector<int>	coords, neighbors;
	std::vector<int>	centerPosition, aux;
	std::vector<std::vector<int>>	intervals;

	intervals.resize(dimensions.size());
	aux.resize(dimensions.size());

	centerPosition = unravelIndex(centerIndex, dimensions);

	for ( int i = 0; i < (int)dimensions.size(); i++ )
	{
		left = max(0,int(centerPosition[i]-radius[i]));
		right = min(int(dimensions[i]-1),int(centerPosition[i]+radius[i]));

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
std::vector<int>	wrappingNeighborhood( const int centerIndex, const std::vector<int>& radius, const std::vector<int>& dimensions )
{
	int	left, right;
	std::vector<int>	coords, neighbors;
	std::vector<int>	centerPosition, aux;
	std::vector<std::vector<int>>	intervals;

	intervals.resize(dimensions.size());
	centerPosition = unravelIndex(centerIndex, dimensions);
	aux.resize(dimensions.size());

	for ( int i = 0; i < (int)dimensions.size(); i++ )
	{
		left = int(centerPosition[i]-radius[i]);
		right = min(left+int(dimensions[i]-1),int(centerPosition[i]+radius[i]));

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





