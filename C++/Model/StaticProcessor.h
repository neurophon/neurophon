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


File Name:		StaticProcessor.h<br>
File Description:	StaticProcessor class definition.<br> This file presents StaticProcessor's public<br>
			interface without revealing the implementations of StaticProcessor's member functions,<br>
			which are defined in StaticProcessor.cpp.<br>


Class Description:
...
*/

#ifndef STATIC_PROCESSOR_H
#define STATIC_PROCESSOR_H

#include <fstream>
#include <array>

#include "Structs.h"

class StaticProcessor
{
public:
		StaticProcessor( const std::vector<std::size_t>&,
				 const std::size_t,
				 const double,
				 const double,
				 const std::array<double,2>& weightLimits = {0.0,1.0} );// constructor that initializes _weights with
											// random numbers between weight limits
											// this function guaranties certain level of sparsity
											// in the weights matrix

		StaticProcessor( std::stringstream&,
				 const std::string& );					// constructor that initializes _weights with previous
											// from file
	void	validateObject();							// function that validates the object
	
	responseInfo	learningRule( const double,
				      const double,
				      const double,
				      const std::vector<std::size_t>&,
				      const double,
		       		      const bool activationHomeostasis = false,
				      const bool randomness = false );			// function that modifies _weights members through learning rule
	
	double	learningNeighborhood( const double,
				      const std::size_t,
				      const std::size_t,
				      const std::string& );				// function that computes the neighborhood lateral influence
											// for the learning process.
											// Specific neighborhood functions can be chosen by means of
											// a string parameter. i.e. "gaussian" for a Gaussian bell,
											// "mex" for a Mexican hat function, etc

	responseInfo	getResponse( const std::vector<std::size_t>& );			// function that gets the response information from the input.
											// the response is compose by a vector with the inverse of the inner product
											// between the input and every unit in the class and a
											// vector with the indexes of such units in ascending value order
	void	homeostasis( const bool,
			     const bool,
			     const bool,
		       	     const double );						// applies homeostasis to the object
	void	synapticHomeostasis( const double );					// computes synaptic homeostasis over the weights
	
	void	synapticGrowthLimitation( const double );				// computes synaptic growth limitation over the weights

	void	activationHomeostasis( const double );					// computes activation homeostasis over the units

	void	saveStaticProcessorStatus( const std::string&,
					   std::stringstream& );			// function that saves the Self Organizing Map's status in a file

	void	loadStaticProcessorStatus( const std::string&,
					   std::stringstream& );			// function to load the Self Organizing Map's status from a file

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
										     StaticProcessor::_unitsArrayDimensionality
										     is \f$[30,15]\f$, this means that this member
										     is equal to \f$30~15\f$, indicating an amount
										     of 450 units */		
private:
	std::size_t				_updateStep;			/**< This is a natural number which counts
										     the time steps in the execution of an
										     instance of this class. */				

	std::size_t				_inputDimensionality;		/**< This is a natural number that
										     defines the number of binary values
										     in the input to an instance of this class.
										     The input to an instance of this class is a vector
										     whose components are binary numbers. */			

	double					_potentialPercentage;		/**< Only a percentage of the binary components
										     in the input vector can be potentially connected
										     with every unit in an instance of this class.
										     This real number specifies such percentage. */	  

	std::size_t				_potentialDimensionality;	/**< This is a natural number which specifies the
       										     number of potential connections between each
									     	     unit in an instance of this class and the binary
										     components in the input vector.
										     This natural number is the result of the product
										     batween
										     StaticProcessor::_potentialPercentage and
										     StaticProcessor::_inputDimensionality */

	twodvector<std::size_t>			_potentialConnections;		/**< This is a bi-dimensional vector of natural numbers.
       										     This vector has
									     	     StaticProcessor::_unitsDimensionality number of rows
										     and
										     StaticProcessor::_potentialDimensionality number of
										     columns.
										     Each row of this vector corresponds to
										     a unit in an object of this class.
										     Each natural number in a row of this vector identifies
										     a potential connection between the corresponding unit in
										     such row and a binary element in the input vector.
										     For example:
										     Let's suppose that the input is a vector of 10 binary components,
										     that is,
										     StaticProcessor::_inputDimensionality is equal to 10.
										     Also, let's suppose that
										     StaticProcessor::_potentialPercentage is 0.5,
										     this means that each unit in the object is potentially
										     connected with half of the binary components in the input
										     vector.
										     Then suppose that the row number 8 in
										     StaticProcessor::_potentialConnections
										     is \f$[1,4,2,8,7]\f$.
										     This means that the unit number 8 in such object instance of
										     this class is portentially connected with binary components
										     number 1, 2, 4, 7 and 8 in the input vector, and that the stregth
										     of such potential connections can growth
										     as a result of learning */	

	std::vector<bool>			_weightsSparsity;		/**< This is a vector of binary values.
										     The number of components in this vector is equal to
										     the number of units in an instance of this class.
										     Each component in this vector indicates if the corresponding
										     row of
										     StaticProcessor::_weights
										     is sparse or not sparse.
										     If such component is 1, that means that the corresponding
										     row in
										     StaticProcessor::_weights
										     is sparse. */

	twodvector<double>			_weights;			/**< This is a bi-dimensional vector of real numbers.
										     Each real value defines the strength of a
										     link between a component in the
										     StaticProcessor::_potentialConnections vector
										     and an unit in the array of units in an instance
										     of this class.
										     This bi-dimensional vector has one row per each unit and
										     one column per each component of the
										     StaticProcessor::_potentialConnections. */

	std::vector<std::size_t>		_unitsActivity;				// registers the count of the unit's activity

	std::vector<double>			_activationBoosting;			// units activation busting
}; // end class SelfOrganizingMap

#endif











