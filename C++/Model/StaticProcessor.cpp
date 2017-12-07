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

// File Name:		StaticProcessor.cpp
// File Description:	StaticProcessor member-function definitions. This file contains implementations of the
//			member functions prototyped in StaticProcessor.h.

#include <iostream>
#include <algorithm>
#include <omp.h>

#include "StaticProcessor.h"				// include definition of class StaticProcessor
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/OctaveInterface.h"
#include "../Libraries/Model/MatlabInterface.h"
#include "../Libraries/Model/GlobalVariables.h"
#include "../Libraries/Model/Templates.h"
#include "../Libraries/Model/DataTypes.h"
#include "../Libraries/Model/Random.h"
#include "../Libraries/Model/Topology.h"

using namespace std;

// constructor that initializes _weights at random
StaticProcessor::StaticProcessor( const std::vector<std::size_t>& unitsArrayDimensionality,
				  const std::size_t inputDimensionality,
				  const double potentialPercentage,
	       		  	  const double sparsity,
				  const std::array<double,2>& weightLimits )
{
	assert(weightLimits[0] <= weightLimits[1]);
	assert(sparsity >= 0.0 && sparsity <= 1.0);

	_updateStep = 0;
	_inputDimensionality = inputDimensionality;
	_unitsDimensionality = std::accumulate(unitsArrayDimensionality.begin(), unitsArrayDimensionality.end(), 1, std::multiplies<std::size_t>());
	_unitsArrayDimensionality.resize(unitsArrayDimensionality.size());
	for ( std::size_t dim = 0; dim < unitsArrayDimensionality.size(); dim++ )
		_unitsArrayDimensionality[dim] = unitsArrayDimensionality[dim];

	_potentialPercentage = potentialPercentage;

	StaticProcessor::validateObject();

	_weights.resize(_unitsDimensionality);
	_potentialDimensionality = _potentialPercentage*_inputDimensionality;
	for ( std::size_t row = 0; row < _unitsDimensionality; row++ )
		_weights[row].resize(_potentialDimensionality);

	for ( std::size_t row = 0; row < _unitsDimensionality; row++ ) {
		for ( std::size_t column = 0; column < _potentialDimensionality; column++ ) {
			if ( sparsity < randomFromDoubleInterval(0.0,1.0) ) {			
				_weights[row][column] = randomFromDoubleInterval(weightLimits[0],weightLimits[1]);
			}
			else {
				_weights[row][column] = 0.0; 
			}
		}
	}

	assert(is_rectangular(_weights));
	_unitsActivity.resize(_unitsDimensionality,1);
	_activationBoosting.resize(_unitsDimensionality,1.0);

	_weightsSparsity.resize(_unitsDimensionality);
	for ( std::size_t row = 0; row < _unitsDimensionality; row++ )
		_weightsSparsity[row] = (get_rectangular_sparsity(_weights[row]) > SPARSITY_THRESHOLD);

	std::vector<std::size_t>	inputRange(_inputDimensionality);
	std::iota(std::begin(inputRange), std::end(inputRange), 0);
	_potentialConnections.resize(_unitsDimensionality);
	for ( auto& potentialConnections : _potentialConnections ) {
		sample_vector(inputRange, potentialConnections, _potentialDimensionality);
		assert(!is_there_duplicate(potentialConnections));
	}

	assert(is_rectangular(_potentialConnections));
	assert(_weights.size() == _potentialConnections.size());
	for ( std::size_t row = 0; row < _weights.size(); row++ )
		assert(_weights[row].size() == _potentialConnections[row].size());
} // end StaticProcessor constructor


// constructor that initializes _weights with previous values from file
StaticProcessor::StaticProcessor( std::stringstream& inputStream,
				  const std::string& staticUnitsIdentifier )
{
	inputStream.clear();
	inputStream.str(inputStream.str());
	StaticProcessor::loadStaticProcessorStatus(staticUnitsIdentifier, inputStream);
} // end StaticProcessor explicit constructor


// function to validate the object created of this class
void	StaticProcessor::validateObject()
{
	if ( _inputDimensionality == 0 || _unitsDimensionality == 0 || _unitsArrayDimensionality.size() == 0 )
	{
		cout << "StaticProcessor object inconsistence: inputDimensionality = " << _inputDimensionality << "\n" << endl;
		cout << "StaticProcessor object inconsistence: unitsArrayDimensionality.size() = " << _unitsArrayDimensionality.size() << "\n" << endl;
		cout << "StaticProcessor object inconsistence: unitsDimensionality = " << _unitsDimensionality << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	for ( std::size_t dim = 0; dim < _unitsArrayDimensionality.size(); dim++ ) {
		if ( _unitsArrayDimensionality[dim] <= 0 ) {
			cout << "StaticProcessor object inconsistence: unitsArrayDimensionality in its dim " << dim << "\n";
			cout << "has a wrong value: " << _unitsArrayDimensionality[dim] << "\n" << endl;
			exit( EXIT_FAILURE );
		}
	}

	if ( _inputDimensionality < _unitsArrayDimensionality.size() )
	{
		cout << "StaticProcessor object inconsistence: inputDimensionality = " << _inputDimensionality
		     << " must be greater than or -at least- equal to unitsArrayDimensionality.size() = "
		     << _unitsArrayDimensionality.size() << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	std::size_t product = std::accumulate(_unitsArrayDimensionality.begin(),
					      _unitsArrayDimensionality.end(), 1,
					      std::multiplies<std::size_t>());

	if ( _unitsDimensionality != product )
	{
		cout << "StaticProcessor object inconsistence: unitsDimensionality = " << _unitsDimensionality
		     << " must be equal to the product of the elements in unitsArrayDimensionality which is = "
		     << product << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( _potentialPercentage < 0.0 || _potentialPercentage > 1.0 )
	{
		cout << "StaticProcessor object inconsistence: potentialPercentage = " << _potentialPercentage
		     << "potentialPercentage must be inside the folowing interval [0.0,1.0]." << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// function to modify the synapticWeights matrix throught the use of the learning rule
responseInfo	StaticProcessor::learningRule( const double learningRate,
					       const double neighborParameter,
					       const double plasticity,
					       const std::vector<std::size_t>& inputIndexes,
					       const double synapticThreshold,
					       const bool activationHomeostasis,
	       				       const bool randomness )
{
	assert(plasticity <= 1.0 && plasticity > 0.0);
	assert(std::all_of(inputIndexes.begin(), inputIndexes.end(), [&](std::size_t i){return i < _inputDimensionality;}));
	
	auto	response = StaticProcessor::getResponse(inputIndexes);

	auto	numberOfAffectedUnits = (std::size_t)(plasticity*_unitsDimensionality);
	std::vector<std::size_t>	unitsWinnerPositions;
	if (activationHomeostasis) {
		std::vector<double>	excitation(_unitsDimensionality);
		std::transform( response.excitation.begin(), response.excitation.end(),
				_activationBoosting.begin(), excitation.begin(), 
				std::multiplies<double>() );
		
		if (!randomness) {
			auto	ranking = sort_indexes(excitation);
			unitsWinnerPositions.insert(unitsWinnerPositions.end(),
						    ranking.end()-std::min(numberOfAffectedUnits,ranking.size()),
						    ranking.end());
		}
		else {
			unitsWinnerPositions = getRandomWeightedIndexes(excitation,numberOfAffectedUnits);
		}
	}
	else {
		if (!randomness) {
			unitsWinnerPositions.insert(unitsWinnerPositions.end(),
						    response.ranking.end()-std::min(numberOfAffectedUnits,response.ranking.size()),
						    response.ranking.end());
		}
		else {
			unitsWinnerPositions = getRandomWeightedIndexes(response.excitation,numberOfAffectedUnits);
		}
	}

	std::string	gaussian = "gaussian";
	for (const auto& unitsWinnerPosition : unitsWinnerPositions) { 
		assert(unitsWinnerPosition < _unitsDimensionality);
		#pragma omp parallel for default(none) shared(unitsWinnerPosition, inputIndexes, gaussian)
		for ( std::size_t row = 0; row < _unitsDimensionality; row++ ) {	// the index row corresponds to the unit
			auto	neighborhoodValue = StaticProcessor::learningNeighborhood(neighborParameter, unitsWinnerPosition, row, gaussian);
			if ( neighborhoodValue > PROXIMAL_SYNAPTIC_THRESHOLD ) {  
				// This is the learning rule to update the synaptic weights
				auto	increment = learningRate*neighborhoodValue*SYNAPTIC_INCREMENT;
				auto	decrement = learningRate*neighborhoodValue*SYNAPTIC_DECREMENT;

				std::vector<std::size_t>	potentialIndexes;
				for (const auto& inputIndex : inputIndexes) {
					auto	potentialIndex = find_first_coincident_index(_potentialConnections[row], inputIndex);
					if ( potentialIndex < _potentialDimensionality ) {
						_weights[row][potentialIndex] += increment;
						potentialIndexes.push_back(potentialIndex);
					}

				}
				potentialIndexes.shrink_to_fit();

				// this is just done when the random behaviour is enabled
				if ( ENABLE_RANDOM_BEHAVIOUR && !_weightsSparsity[row] ) {
					auto	numberOfAffectedSynapses = (std::size_t)(MASSIVE_PLASTICITY*_potentialDimensionality); 
					std::vector<std::size_t>	inactiveIndexes;
					for ( std::size_t randomChoise = 0; randomChoise < numberOfAffectedSynapses; randomChoise++ ) {
						std::size_t	choise = rand() % _potentialDimensionality;
						std::size_t	securityScape = 0;
						while ( std::find(potentialIndexes.begin(), potentialIndexes.end(), choise) != potentialIndexes.end() ||
							std::find(inactiveIndexes.begin(), inactiveIndexes.end(), choise) != inactiveIndexes.end() ) {
							choise = rand() % _potentialDimensionality;
							if (securityScape > _inputDimensionality*0.1)
								break;
							
							securityScape++;
						}
						inactiveIndexes.push_back(choise);
						
						if ( _weights[row][choise] > 0.0 ) {
							_weights[row][choise] -= decrement;
							if ( _weights[row][choise] < 0.0 )
								_weights[row][choise] = 0.0;
						}
						if ( _weights[row][choise] < 0.0 ) {
							throw std::runtime_error ("_weights[row][choise] < 0.0");
						}
					}
				}
			}
		}
	}
	
	for (const auto& unitsWinnerPosition : unitsWinnerPositions) { 
		assert(unitsWinnerPosition < _unitsDimensionality);
		_unitsActivity[unitsWinnerPosition]++;
	}
	
	return	response;
} // end function learningRule


// function to compute the neighborhood value in the lateral interaction between units in the array for learning process
double	StaticProcessor::learningNeighborhood( const double widthParameter,
					       const std::size_t winnerPosition,
					       const std::size_t otherPosition,
					       const std::string& str )
{
	assert(winnerPosition < _unitsDimensionality && otherPosition < _unitsDimensionality);
	std::vector<std::size_t>	winnerPositionArray, otherPositionArray, auxiliary;

	winnerPositionArray = unravelIndex(winnerPosition, _unitsArrayDimensionality);		// gets a vector with the array coordinates
       												// corresponding to the winnerPosition
	otherPositionArray = unravelIndex(otherPosition, _unitsArrayDimensionality);		// gets a vector with the array coordinates
       												// corresponding to the otherPosition

	auxiliary.resize(winnerPositionArray.size());
	for ( std::size_t index = 0; index < winnerPositionArray.size(); index++ ) {
		if ( winnerPositionArray[index] > otherPositionArray[index] )
			auxiliary[index] = winnerPositionArray[index]-otherPositionArray[index];
		else
			auxiliary[index] = otherPositionArray[index]-winnerPositionArray[index];
	}

	std::size_t distance = std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0);

	std::string	STR;
	STR = "gaussian";
	if ( str.compare(STR) == 0 ) {
		return exp(-(pow((double)distance, (double)2)) / (2*widthParameter));		// returns the neighborhood value (gaussian)
	}
	STR = "mex";
	if ( str.compare(STR) == 0 ) {
		return 2/(sqrt(3*widthParameter)*pow(M_PI,(double)0.25)) *
		       (1 - (pow((double)distance,(double)2))/(pow(widthParameter,(double)2))) *
		       exp(-(pow((double)distance,(double)2))/(2*pow(widthParameter,(double)2)));	// returns the neighborhood value (mexican hat)
	}

	cout << "StaticProcessor object inconsistence: bad function option: " << str << ".\n" << endl;
	exit( EXIT_FAILURE );
} // end function learningNeighborhood


// function to get the response information from the input
responseInfo	StaticProcessor::getResponse( const std::vector<std::size_t>& inputIndexes )
{
	responseInfo	response;
	
	for (const auto& inputIndex : inputIndexes)
		if ( inputIndex >= _inputDimensionality )
			throw std::runtime_error ("inputIndex >= _inputDimensionality");

	response.excitation.resize(_unitsDimensionality);
	#pragma omp parallel for default(none) shared(response, inputIndexes)
	for ( std::size_t row = 0; row < _unitsDimensionality; row++ ) {					// the index row corresponds to the units
		double	innerProduct = 0.0;
		for (const auto& inputIndex : inputIndexes) {
			auto	potentialIndex = find_first_coincident_index(_potentialConnections[row], inputIndex);
			if ( potentialIndex < _potentialDimensionality )
				innerProduct += _weights[row][potentialIndex];
		}

		response.excitation[row] = innerProduct;
	}
	response.ranking = sort_indexes(response.excitation);

	return response;										// returns the units response
} // end function getResponse


// applies homeostasis to the object
void	StaticProcessor::homeostasis( const bool learning,
				      const bool synapticHomeostasis,
				      const bool activationHomeostasis,
				      const double synapticThreshold )
{
	if (activationHomeostasis && (positiveMod(_updateStep,5) == 0)) {
		StaticProcessor::activationHomeostasis(10*BUSTING);
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		if ( learning ) {
			if (synapticHomeostasis) {
				StaticProcessor::synapticHomeostasis(synapticThreshold);	
			}
			else {
				StaticProcessor::synapticGrowthLimitation(synapticThreshold);
			}
		}

		auto	result = std::min_element(_unitsActivity.begin(), _unitsActivity.end());
		std::size_t	minimum = _unitsActivity[std::distance(_unitsActivity.begin(), result)];
		transform(_unitsActivity.begin(), _unitsActivity.end(), _unitsActivity.begin(),
			  bind2nd(std::minus<std::size_t>(), minimum));

		#pragma omp parallel for default(none)
		for ( std::size_t row = 0; row < _unitsDimensionality; row++ )
			_weightsSparsity[row] = (get_rectangular_sparsity(_weights[row]) > SPARSITY_THRESHOLD);

		_updateStep = 0;
	}
	_updateStep++;
} // end function homeostasis


// computes synaptic homeostasis over the weights
void	StaticProcessor::synapticHomeostasis( const double synapticThreshold )
{
	auto	averageActivity = (double)std::accumulate(_unitsActivity.begin(), _unitsActivity.end(),0)/(double)_unitsActivity.size();
	assert(averageActivity > 0.0);

	StaticProcessor::synapticGrowthLimitation(synapticThreshold);
	for ( std::size_t row = 0; row < _unitsDimensionality; row++ ) {
		if ( (double)_unitsActivity[row] < 0.1*averageActivity ) {	// if this unit has had not enough activity
		       // tries to fill its weights with neighbor units synaptic weights
			std::vector<std::size_t>	radius(_unitsArrayDimensionality.size(),1);
			auto	neighborUnits = neighborhood(row,radius,_unitsArrayDimensionality);
			for ( const auto& neighborUnit : neighborUnits )
				_weights[row] += _weights[neighborUnit];

			// it does the following work only if random behaviour is enabled
			if ( ENABLE_RANDOM_BEHAVIOUR ) {
				// tries to fill its weights at random, with random asynaptic weights
				auto	numberOfChoises = (std::size_t)(MASSIVE_PLASTICITY*_potentialDimensionality); 
				std::vector<std::size_t>	randomChoises;
				for ( std::size_t randomChoise = 0; randomChoise < numberOfChoises; randomChoise++ ) {
					std::size_t	choise = rand() % _potentialDimensionality;
					std::size_t	securityScape = 0;
					while ( std::find(randomChoises.begin(), randomChoises.end(), choise) != randomChoises.end() ) {
						choise = rand() % _potentialDimensionality;
						if (securityScape > 0.1*_inputDimensionality)
							break;
							
						securityScape++;
					}
					randomChoises.push_back(choise);
					_weights[row][choise] += randomFromDoubleInterval(SYNAPTIC_DECREMENT,SYNAPTIC_INCREMENT);
				}
			}
		}
	}
} // end function synapticHomeostasis


// computes synaptic growth limitation over the weights
void	StaticProcessor::synapticGrowthLimitation( const double synapticThreshold )
{
	#pragma omp parallel for default(none)
	for ( std::size_t row = 0; row < _unitsDimensionality; row++ ) {
		auto	sum = std::accumulate(_weights[row].begin(), _weights[row].end(), 0.0);
		if ( sum > 1.0 ) {
			std::transform(_weights[row].begin(), _weights[row].end(), _weights[row].begin(),
			std::bind2nd(std::divides<double>(),sum));
		}

		// prunes the lowest synaptic weights
		auto	indexes = less_than_indexes(_weights[row], synapticThreshold);
		set_elements(_weights[row], indexes, 0.0);
	}
} // end function synapticGrowthLimitation


// computes activation homeostasis over the units
void	StaticProcessor::activationHomeostasis( const double boostingFactor )
{
	auto	averageActivity = (double)std::accumulate(_unitsActivity.begin(), _unitsActivity.end(),0)/(double)_unitsActivity.size();

	assert(averageActivity > 0.0);
	#pragma omp parallel for default(none) shared(averageActivity)
	for ( std::size_t row = 0; row < _unitsDimensionality; row++ ) {
		if (_updateStep > 0)
			_activationBoosting[row] = exp(-boostingFactor*((double)_unitsActivity[row]-(double)averageActivity)/(double)_updateStep);
		else
			_activationBoosting[row] = exp(-boostingFactor*((double)_unitsActivity[row]-(double)averageActivity));
	}
} // end function activationHomeostasis


// function to save the Self Organizing Map's status in a file
void	StaticProcessor::saveStaticProcessorStatus( const std::string& staticUnitsIdentifier,
						    stringstream& outfile )
{
	std::string	str = "StaticProcessor_";
	std::string	STR;
	str += staticUnitsIdentifier;
	str += "_";

        // saves _inputDimensionality
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_scalar_as_numeric_array(str + "inputDimensionality", _inputDimensionality, outfile);
	else
		save_as_scalar(str + "inputDimensionality", _inputDimensionality, outfile);

        // saves _unitsDimensionality
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_scalar_as_numeric_array(str + "unitsDimensionality", _unitsDimensionality, outfile);
	else
		save_as_scalar(str + "unitsDimensionality", _unitsDimensionality, outfile);

        // saves _unitsArrayDimensionality
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_vector_as_numeric_array(str + "unitsArrayDimensionality", _unitsArrayDimensionality, outfile);
	else
		save_vector_as_matrix(str + "unitsArrayDimensionality", _unitsArrayDimensionality, outfile);

        // saves _weights
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_vector_of_vectors_conditionally_as_sparse_array(str + "weights",_weights,SPARSITY_THRESHOLD,outfile);
	else
		save_vector_of_vectors_conditionally_as_sparse_matrix(str + "weights",_weights,SPARSITY_THRESHOLD,outfile);

        // saves _updateStep
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_scalar_as_numeric_array(str + "updateStep", _updateStep, outfile);
	else
		save_as_scalar(str + "updateStep", _updateStep, outfile);

	// saves _unitsActivity
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_vector_as_numeric_array(str + "unitsActivity", _unitsActivity, outfile);
	else
		save_vector_as_matrix(str + "unitsActivity", _unitsActivity, outfile);

	// saves _activationBoosting
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_vector_as_numeric_array(str + "activationBusting", _activationBoosting, outfile);
	else
		save_vector_as_matrix(str + "activationBusting", _activationBoosting, outfile);

	// saves _weightsSparsity
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_vector_as_numeric_array(str + "weightsSparsity", _weightsSparsity, outfile);
	else
		save_vector_as_matrix(str + "weightsSparsity", _weightsSparsity, outfile);

        // saves _potentialPercentage
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_scalar_as_numeric_array(str + "potentialPercentage", _potentialPercentage, outfile);
	else
		save_as_scalar(str + "potentialPercentage", _potentialPercentage, outfile);

        // saves _potentialDimensionality
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_scalar_as_numeric_array(str + "potentialDimensionality", _potentialDimensionality, outfile);
	else
		save_as_scalar(str + "potentialDimensionality", _potentialDimensionality, outfile);

	// saves _potentialConnections
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_vector_of_vectors_as_numeric_array(str + "potentialConnections", _potentialConnections, outfile);
	else
		save_vector_of_vectors_as_matrix(str + "potentialConnections", _potentialConnections, outfile);

} // end functiom saveStaticProcessorStatus


// function to load the Self Organizing Map's status from a file
void	StaticProcessor::loadStaticProcessorStatus( const std::string& staticUnitsIdentifier,
						    stringstream& infile )
{
	std::string	str;
	std::string	STR = "StaticProcessor_";
	std::string	auxiliary;
	STR += staticUnitsIdentifier;
	STR += "_";

	bool	check_inputDimensionality = false;
	bool	check_unitsDimensionality = false;
	bool	check_unitsArrayDimensionality = false;
	bool	check_weights = false;
	bool	check_updateStep = false;
	bool	check_unitsActivity = false;
	bool	check_activationBusting = false;
	bool	check_weightsSparsity = false;
	bool	check_potentialPercentage = false;
	bool	check_potentialDimensionality = false;
	bool	check_potentialConnections = false;

	if (ENABLE_MATLAB_COMPATIBILITY) {
		auto	array_structure = check_next_data_structure(infile, big_endianness);
		while ( array_structure.more_data ) {

			auxiliary = STR + "inputDimensionality";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _inputDimensionality, infile, big_endianness);
				check_inputDimensionality = true;
			}

			auxiliary = STR + "unitsDimensionality";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _unitsDimensionality, infile, big_endianness);
				check_unitsDimensionality = true;
			}

			auxiliary = STR + "unitsArrayDimensionality";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_vector(array_structure, _unitsArrayDimensionality, infile, big_endianness);
				check_unitsArrayDimensionality = true;
			}

			auxiliary = STR + "weights";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_conditionally_sparse_array_to_vector_of_vectors(array_structure, _weights, infile, big_endianness);
				check_weights = true;
			}

			auxiliary = STR + "updateStep";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _updateStep, infile, big_endianness);
				check_updateStep = true;
			}

			auxiliary = STR + "unitsActivity";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_vector(array_structure, _unitsActivity, infile, big_endianness);
				check_unitsActivity = true;
			}

			auxiliary = STR + "activationBusting";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_vector(array_structure, _activationBoosting, infile, big_endianness);
				check_activationBusting = true;
			}

			auxiliary = STR + "weightsSparsity";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				std::vector<std::size_t>	weightsSparsity;
				load_numeric_array_to_vector(array_structure, weightsSparsity, infile, big_endianness);
				for (const auto& aux : weightsSparsity) {
					if (aux == 0)
						_weightsSparsity.push_back(false);
					else
						_weightsSparsity.push_back(true);
				}
				_weightsSparsity.shrink_to_fit();
				check_weightsSparsity = true;
			}

			auxiliary = STR + "potentialPercentage";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _potentialPercentage, infile, big_endianness);
				check_potentialPercentage = true;
			}

			auxiliary = STR + "potentialDimensionality";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_scalar(array_structure, _potentialDimensionality, infile, big_endianness);
				check_potentialDimensionality = true;
			}

			auxiliary = STR + "potentialConnections";
			if ( array_structure.name.compare(auxiliary) == 0 ) {
				load_numeric_array_to_vector_of_vectors(array_structure, _potentialConnections, infile, big_endianness);
				check_potentialConnections = true;
			}

			array_structure = check_next_data_structure(infile,big_endianness);
		}	


	}
	else {
		while ( std::getline(infile, str) ) {

			auxiliary = "# name: " + STR + "inputDimensionality";
			if ( str.compare(auxiliary) == 0 ) {
				load_scalar(_inputDimensionality, infile);
				check_inputDimensionality = true;
			}

			auxiliary = "# name: " + STR + "unitsDimensionality";
			if ( str.compare(auxiliary) == 0 ) {
				load_scalar(_unitsDimensionality, infile);
				check_unitsDimensionality = true;
			}

			auxiliary = "# name: " + STR + "unitsArrayDimensionality";
			if ( str.compare(auxiliary) == 0 ) {
				load_matrix_to_vector(_unitsArrayDimensionality, infile);
				check_unitsArrayDimensionality = true;
			}

			auxiliary = "# name: " + STR + "weights";
			if ( str.compare(auxiliary) == 0 ) {
				load_conditional_sparse_matrix_to_vector_of_vectors(_weights,infile);
				check_weights = true;
			}

			auxiliary = "# name: " + STR + "updateStep";
			if ( str.compare(auxiliary) == 0 ) {
				load_scalar(_updateStep, infile);
				check_updateStep = true;
			}

			auxiliary = "# name: " + STR + "unitsActivity";
			if ( str.compare(auxiliary) == 0 ) {
				load_matrix_to_vector(_unitsActivity, infile);
				check_unitsActivity = true;
			}

			auxiliary = "# name: " + STR + "activationBusting";
			if ( str.compare(auxiliary) == 0 ) {
				load_matrix_to_vector(_activationBoosting, infile);
				check_activationBusting = true;
			}

			auxiliary = "# name: " + STR + "weightsSparsity";
			if ( str.compare(auxiliary) == 0 ) {
				std::vector<std::size_t>	weightsSparsity;
				load_matrix_to_vector(weightsSparsity, infile);
				for (const auto& aux : weightsSparsity) {
					if (aux == 0)
						_weightsSparsity.push_back(false);
					else
						_weightsSparsity.push_back(true);
				}
				_weightsSparsity.shrink_to_fit();
				check_weightsSparsity = true;
			}

			auxiliary = "# name: " + STR + "potentialPercentage";
			if ( str.compare(auxiliary) == 0 ) {
				load_scalar(_potentialPercentage, infile);
				check_potentialPercentage = true;
			}

			auxiliary = "# name: " + STR + "potentialDimensionality";
			if ( str.compare(auxiliary) == 0 ) {
				load_scalar(_potentialDimensionality, infile);
				check_potentialDimensionality = true;
			}

			auxiliary = "# name: " + STR + "potentialConnections";
			if ( str.compare(auxiliary) == 0 ) {
				load_matrix_to_vector_of_vectors(_potentialConnections, infile);
				check_potentialConnections = true;
			}
		}
	}

	assert(check_inputDimensionality == true);
	assert(check_unitsDimensionality == true);
	assert(check_unitsArrayDimensionality == true);
	assert(check_weights == true);
	assert(check_updateStep == true);
	assert(check_unitsActivity == true);
	assert(check_activationBusting == true);
	assert(check_weightsSparsity == true);
	assert(check_potentialPercentage == true);
	assert(check_potentialDimensionality == true);
	assert(check_potentialConnections == true);

} // end functiom loadStaticProcessorStatus


