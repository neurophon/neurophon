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

// File Name:		Topology.h
// File Description:	Topology definitions. This file presents Topology's interface
//			without revealing the implementations of Topology's functions,
//			which are defined in Topology.cpp.
#include "Constants.h"


/*'neighborhood' function gets the points in the neighborhood of a point.
A point's neighborhood is the n-dimensional hypercube with sides ranging
[center - radius, center + radius], inclusive. For example, if there are two
dimensions and the radius is 3, the neighborhood is 6x6. Neighborhoods are
truncated when they are near an edge.
This is designed to be fast. In C++ it's fastest to iterate through neighbors
one by one, calculating them on-demand rather than creating a list of them.
But in Python it's faster to build up the whole list in batch via a few calls
to C code rather than calculating them on-demand with lots of calls to Python
code.
centerIndex (int)
 The index of the point. The coordinates are expressed as a single index by
 using the dimensions as a mixed radix definition. For example, in dimensions
 42x10, the point [1, 4] is index 1*420 + 4*10 = 460.
radius (int)
 The radius of this neighborhood about the centerIndex.
 dimensions (indexable sequence)
 The dimensions of the world outside this neighborhood.
returns (numpy array)
 The points in the neighborhood, including centerIndex.*/
std::vector<int>	neighborhood( const int, const std::vector<int>&, const std::vector<int>& );


/*Like 'neighborhood', 'wrappingNeighborhood' excepts that the neighborhood isn't
truncated when it's near an edge. It wraps around to the other side.
centerIndex (int)
 The index of the point. The coordinates are expressed as a single index by
 using the dimensions as a mixed radix definition. For example, in dimensions
 42x10, the point [1, 4] is index 1*420 + 4*10 = 460.
radius (int)
 The radius of this neighborhood about the centerIndex.
dimensions (indexable sequence)
 The dimensions of the world outside this neighborhood.
returns (numpy array)
 The points in the neighborhood, including centerIndex.*/
std::vector<int>	wrappingNeighborhood( const int, const std::vector<int>&, const std::vector<int>& );









