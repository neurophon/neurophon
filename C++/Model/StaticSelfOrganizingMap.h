/*!
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				Author:		Dematties Dario Jesus						
				Contact:	dariodematties@hotmail.com.ar					
						dariodematties@yahoo.com.ar					
						dario.dematties@frm.utn.edu.ar					
				Project:	Engineering PhD Project						
				Institution:	Universidad de Buenos Aires					
						Facultad de Ingeniería (FIUBA)					
				Workplace:	Instituto de Ingeniería						
						Biomédica FIUBA	&						
						CCT CONICET Mendoza INCIHUSA					

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

File Name:		StaticSelfOrganizingMap.h
File Description:	StaticSelfOrganizingMap class definition. This file presents StaticSelfOrganizingMap's public
			interface without revealing the implementations of StaticSelfOrganizingMap's member functions,
			which are defined in StaticSelfOrganizingMap.cpp.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Class Description: This is a Self Organizing Map structure
(SOM).
A SOM is an unsupervised clustering algorithm.
This is a bunch of units arranged in an n-dimensional array.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
										     of an array of units in this class. */	
	std::size_t				_unitsDimensionality;		/*!< This is a natural number that defines
										     the number of units in an instance
										     of this class. */		
private:
	std::size_t				_inputDimensionality;		/**< This is a natural number that
										     defines the number of real values
										     in the input to an instance of this class. */			
	std::size_t				_updateStep;			/**< This is a natural number which counts
										     the time steps in the execution of an
										     instance of this class. */				
	twodvector<double>			_weights;			/**< This is a bidimensional vector of real numbers.
										     Each real value defines the strength of a
										     link between an input value and an unit in an
										     array of units in an instance of this class.
										     This vector has a row per each unit and
										     a column per each input member. */				
}; // end class StaticSelfOrganizingMap

#endif



