/*!

Author:		Dematties Dario Jesus<br>					
Contact:	dariodematties@hotmail.com.ar<br>					
		dariodematties@yahoo.com.ar<br>					
		dario.dematties@frm.utn.edu.ar<br>					
Project:	Engineering PhD Project<br>						
Institution:	Universidad de Buenos Aires<br>					
		Facultad de Ingeniería (FIUBA)<br>					
Workplace:	Instituto de Ingeniería<br>						
		Biomédica FIUBA	&<br>						
		CCT CONICET Mendoza INCIHUSA<br>					


File Name:		StaticSelfOrganizingMap.h<br>
File Description:	StaticSelfOrganizingMap class definition.<br> This file presents StaticSelfOrganizingMap's public<br>
			interface without revealing the implementations of StaticSelfOrganizingMap's member functions,<br>
			which are defined in StaticSelfOrganizingMap.cpp.<br>


Class Description: This is a
<a href="https://en.wikipedia.org/wiki/Self-organizing_map">Self Organizing Map</a>
structure (SOM).<br>
A SOM is an unsupervised clustering algorithm.<br>
This is a bunch of units arranged in an n-dimensional array
whose responses are tunned to react to inputs vectors located in
certain regions in the input space.

*/

#ifndef STATIC_SOM_H
#define STATIC_SOM_H

#include <fstream>
#include <array>

#include "Structs.h"

class StaticSelfOrganizingMap
{
public:
		StaticSelfOrganizingMap( const std::vector<std::size_t>&,
			       		 const std::size_t );				
											
											
		StaticSelfOrganizingMap( const std::vector<std::size_t>&,
			       		 const std::size_t,
				   	 const std::array<double,2>& );			
											
											
		StaticSelfOrganizingMap( std::stringstream&,
			       		 const std::string& );				
											
	void	validateObject();							

	void	learningRule( const double, const double,
		       	      const std::size_t,
			      const std::vector<double>& );				
       											

	double	learningNeighborhood( const double,
		       		      const std::size_t,
				      const std::size_t,
				      const std::string& );				
											
											
											
											
	somResponseInfo	getResponse( const std::vector<double>& );			
											
											
											
	void	saveStaticSelfOrganizingMapStatus( const std::string&,
		       				   std::stringstream& );			

	void	loadStaticSelfOrganizingMapStatus( const std::string&,
		       				   std::stringstream& );			

protected:
        std::vector<std::size_t>		_unitsArrayDimensionality;	/*!< This is a vector of natural numbers
										     that defines the dimensionality
										     of an array of units in this class.
										     For example a vector \f$[30,15]\f$
										     makes reference to a bi-dimensional
										     array of 30 by 15 units, while a vector
										      \f$[25,15,30]\f$ makes reference to a
										     three-dimensional array of 25 by
										     15 by 30 units */	

	std::size_t				_unitsDimensionality;		/*!< This is a natural number that defines
										     the number of units in an instance
										     of this class.
										     For example, if 
										     StaticSelfOrganizingMap::_unitsArrayDimensionality
										     is \f$[30,15]\f$, this means that this member
										     is equal to \f$30~15\f$, indicating an amount
										     of 450 units */		
private:
	std::size_t				_inputDimensionality;		/**< This is a natural number that
										     defines the number of real values
										     in the input to an instance of this class.
										     The input to an instance of this class is a vector
										     whose components are real numbers. */			

	std::size_t				_updateStep;			/**< This is a natural number which counts
										     the time steps in the execution of an
										     instance of this class. */				

	twodvector<double>			_weights;			/**< This is a bi-dimensional vector of real numbers.
										     Each real value defines the strength of a
										     link between a component in the input vector
										     and an unit in the array of units in an instance
										     of this class.
										     This bi-dimensional vector has a row per each unit and
										     a column per each component of the input vector. */				
}; // end class StaticSelfOrganizingMap

#endif



