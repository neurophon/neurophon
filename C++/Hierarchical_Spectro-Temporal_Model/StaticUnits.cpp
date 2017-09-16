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

// File Name:		StaticUnits.cpp
// File Description:	StaticUnits member-function definitions. This file contains implementations of the
//			member functions prototyped in StaticUnits.h.

#include <iostream>
#include <algorithm>
#include <omp.h>

#include "StaticUnits.h"				// include definition of class StaticUnits
#include "../Libraries/Utilities.h"
#include "../Libraries/OctaveInterface.h"
#include "../Libraries/Templates.h"
#include "../Libraries/DataTypes.h"
#include "../Libraries/Random.h"
#include "../Libraries/Topology.h"

using namespace std;

// constructor that initializes _weights at random
StaticUnits::StaticUnits( const std::vector<int>& unitsArrayDimensionality, const int inputDimensionality,
	       		  const std::array<double,2>& weightLimits, const bool binaryProcessing )
{
	assert(weightLimits[0] <= weightLimits[1]);

	_binaryProcessing = binaryProcessing;
	_updateStep = 0;
	_inputDimensionality = inputDimensionality;
	_unitsDimensionality = std::accumulate(unitsArrayDimensionality.begin(), unitsArrayDimensionality.end(), 1, std::multiplies<int>());
	_unitsArrayDimensionality.resize(unitsArrayDimensionality.size());
	for ( unsigned int dim = 0; dim < unitsArrayDimensionality.size(); dim++ )
		_unitsArrayDimensionality[dim] = unitsArrayDimensionality[dim];

	StaticUnits::validateObject();

	_weights.resize(_unitsDimensionality);
	for ( int row = 0; row < _unitsDimensionality; row++ )
		_weights[row].resize(_inputDimensionality);

	for ( int row = 0; row < _unitsDimensionality; row++ )
		for ( int column = 0; column < _inputDimensionality; column++ )
			_weights[row][column] = randomFromDoubleInterval(weightLimits[0],weightLimits[1]);

	if ( _binaryProcessing ) {
		_unitsActivity.resize(_unitsDimensionality,1);
		_activationBoosting.resize(_unitsDimensionality,1.0);

		_weightsSparsity.resize(_unitsDimensionality);
		for ( int row = 0; row < _unitsDimensionality; row++ )
			_weightsSparsity[row] = (get_rectangular_sparsity(_weights[row]) > SPARSITY_THRESHOLD);
	}
} // end StaticUnits default constructor


// constructor that initializes _weights at random
StaticUnits::StaticUnits( const std::vector<int>& unitsArrayDimensionality, const int inputDimensionality,
	       		  const double sparsity, const std::array<double,2>& weightLimits,
			  const bool binaryProcessing )
{
	assert(weightLimits[0] <= weightLimits[1]);
	assert(sparsity >= 0.0 && sparsity <= 1.0);

	_binaryProcessing = binaryProcessing;
	_updateStep = 0;
	_inputDimensionality = inputDimensionality;
	_unitsDimensionality = std::accumulate(unitsArrayDimensionality.begin(), unitsArrayDimensionality.end(), 1, std::multiplies<int>());
	_unitsArrayDimensionality.resize(unitsArrayDimensionality.size());
	for ( unsigned int dim = 0; dim < unitsArrayDimensionality.size(); dim++ )
		_unitsArrayDimensionality[dim] = unitsArrayDimensionality[dim];

	StaticUnits::validateObject();

	_weights.resize(_unitsDimensionality);
	for ( int row = 0; row < _unitsDimensionality; row++ )
		_weights[row].resize(_inputDimensionality);

	for ( int row = 0; row < _unitsDimensionality; row++ )
		for ( int column = 0; column < _inputDimensionality; column++ ) {
			if ( sparsity < randomFromDoubleInterval(0.0,1.0) ) {			
				_weights[row][column] = randomFromDoubleInterval(weightLimits[0],weightLimits[1]);
			}
			else {
				_weights[row][column] = 0.0; 
			}
		}

	if ( _binaryProcessing ) {
		_unitsActivity.resize(_unitsDimensionality,1);
		_activationBoosting.resize(_unitsDimensionality,1.0);

		_weightsSparsity.resize(_unitsDimensionality);
		for ( int row = 0; row < _unitsDimensionality; row++ )
			_weightsSparsity[row] = (get_rectangular_sparsity(_weights[row]) > SPARSITY_THRESHOLD);
	}
} // end StaticUnits default constructor


// constructor that initializes _weights with previous values from file
StaticUnits::StaticUnits( const std::string& fileName, const std::string& staticUnitsIdentifier )
{
	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + fileName + ".mat", ios::in | std::ifstream::binary);

	StaticUnits::loadStaticUnitsStatus(staticUnitsIdentifier, infile);

	// close the opened file.
	infile.close();
} // end StaticUnits explicit constructor


// function to validate the object created of this class
void	StaticUnits::validateObject()
{
	if ( _inputDimensionality <= 0 || _unitsDimensionality <= 0 || _unitsArrayDimensionality.size() <= 0 )
	{
		cout << "StaticUnits object inconsistence: inputDimensionality = " << _inputDimensionality << "\n" << endl;
		cout << "StaticUnits object inconsistence: unitsArrayDimensionality.size() = " << _unitsArrayDimensionality.size() << "\n" << endl;
		cout << "StaticUnits object inconsistence: unitsDimensionality = " << _unitsDimensionality << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	for ( unsigned int dim = 0; dim < _unitsArrayDimensionality.size(); dim++ ) {
		if ( _unitsArrayDimensionality[dim] <= 0 ) {
			cout << "StaticUnits object inconsistence: unitsArrayDimensionality in its dim " << dim << "\n";
			cout << "has a wrong value: " << _unitsArrayDimensionality[dim] << "\n" << endl;
			exit( EXIT_FAILURE );
		}
	}

	if ( _inputDimensionality < (int)_unitsArrayDimensionality.size() )
	{
		cout << "StaticUnits object inconsistence: inputDimensionality = " << _inputDimensionality
		     << " must be greater than or -at least- equal to unitsArrayDimensionality.size() = "
		     << _unitsArrayDimensionality.size() << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	int product = std::accumulate(_unitsArrayDimensionality.begin(), _unitsArrayDimensionality.end(), 1, std::multiplies<int>());

	if ( _unitsDimensionality != product )
	{
		cout << "StaticUnits object inconsistence: unitsDimensionality = " << _unitsDimensionality
		     << " must be equal to the product of the elements in unitsArrayDimensionality which is = "
		     << product << "\n" << endl;
		exit( EXIT_FAILURE );
	}
} // end function validateObject


// function to modify the synapticWeights matrix throught the use of the Kohonen's learning rule
void	StaticUnits::learningRule( const double learningRate, const double neighborParameter, const int unitsWinnerPosition,
					 const std::vector<double>& input )
{
	assert((int)input.size() == _inputDimensionality);
	assert(unitsWinnerPosition < _unitsDimensionality);
	std::vector<std::vector<double>>	deltaWeights;

	deltaWeights.resize(_unitsDimensionality);

	std::string	gaussian = "gaussian";
	//omp_set_num_threads(NUM_THREADS);
	//#pragma omp parallel for default(none) shared(deltaWeights, input, gaussian)
	for ( int row = 0; row < _unitsDimensionality; row++ ) {	// the index row corresponds to the unit
		double	neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, unitsWinnerPosition, row, gaussian);
		// This is the Kohonen learning rule to update the synaptic weights
		deltaWeights[row] = input - _weights[row];
		std::transform(deltaWeights[row].begin(), deltaWeights[row].end(), deltaWeights[row].begin(),
		std::bind1st(std::multiplies<double>(),learningRate*neighborhoodValue));
		_weights[row]+=deltaWeights[row];
	}
} // end function learningRule


// function to modify the synapticWeights matrix throught the use of the learning rule
responseInfo	StaticUnits::learningRule( const double learningRate, const double neighborParameter, const double plasticity,
					   const std::vector<std::size_t>& inputIndexes, const double synapticThreshold,
					   const bool activationHomeostasis,
	       				   const bool randomness )
{
	assert(plasticity <= 1.0 && plasticity > 0.0);
	
	auto	response = StaticUnits::getResponse(inputIndexes);
	auto	numberOfAffectedUnits = (int)(plasticity*_unitsDimensionality);
	std::vector<int>	unitsWinnerPositions;
	if (activationHomeostasis) {
		std::vector<double>	excitation(_unitsDimensionality);
		std::transform( response.excitation.begin(), response.excitation.end(),
				_activationBoosting.begin(), excitation.begin(), 
				std::multiplies<double>() );
		
		if (!randomness) {
			auto	ranking = sort_indexes(excitation);
			unitsWinnerPositions.insert(unitsWinnerPositions.end(),
						    ranking.end()-std::min(numberOfAffectedUnits,(int)ranking.size()),
						    ranking.end());
		}
		else {
			unitsWinnerPositions = getRandomWeightedIndexes(excitation,numberOfAffectedUnits);
		}
	}
	else {
		if (!randomness) {
			unitsWinnerPositions.insert(unitsWinnerPositions.end(),
						    response.ranking.end()-std::min(numberOfAffectedUnits,(int)response.ranking.size()),
						    response.ranking.end());
		}
		else {
			unitsWinnerPositions = getRandomWeightedIndexes(response.excitation,numberOfAffectedUnits);
		}
	}

	std::string	gaussian = "gaussian";
	for (const auto& unitsWinnerPosition : unitsWinnerPositions) { 
		assert(unitsWinnerPosition < _unitsDimensionality);
		//omp_set_num_threads(NUM_THREADS);
		//#pragma omp parallel for default(none) shared(unitsWinnerPosition, inputIndexes, gaussian)
		for ( int row = 0; row < _unitsDimensionality; row++ ) {						// the index row corresponds to the unit
			auto	neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, unitsWinnerPosition, row, gaussian);
			auto	increment = learningRate*neighborhoodValue*SYNAPTIC_INCREMENT;
			auto	decrement = learningRate*neighborhoodValue*SYNAPTIC_DECREMENT;
			if ( neighborhoodValue > PROXIMAL_SYNAPTIC_THRESHOLD ) {  
				// This is the learning rule to update the synaptic weights
				for (const auto& inputIndex : inputIndexes)
					_weights[row][inputIndex] += increment;

				if ( !_weightsSparsity[row] ) {
					auto	numberOfAffectedSynapses = (std::size_t)(MASSIVE_PLASTICITY*_inputDimensionality); 
					std::vector<std::size_t>	inactiveIndexes;
					for ( std::size_t randomChoise = 0; randomChoise < numberOfAffectedSynapses; randomChoise++ ) {
						std::size_t	choise = rand() % _inputDimensionality;
						std::size_t	securityScape = 0;
						while ( std::find(inputIndexes.begin(), inputIndexes.end(), choise) != inputIndexes.end() ||
							std::find(inactiveIndexes.begin(), inactiveIndexes.end(), choise) != inactiveIndexes.end() ) {
							choise = rand() % _inputDimensionality;
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
						//assert(_weights[row][choise] >= 0.0);
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
double	StaticUnits::learningNeighborhood( const double widthParameter, const int winnerPosition,
					   const int otherPosition, const std::string& str )
{
	assert(winnerPosition < _unitsDimensionality && otherPosition < _unitsDimensionality);
	std::vector<int>	winnerPositionArray, otherPositionArray, auxiliary;

	winnerPositionArray = unravelIndex(winnerPosition, _unitsArrayDimensionality);		// gets a vector with the array coordinates
       												// corresponding to the winnerPosition
	otherPositionArray = unravelIndex(otherPosition, _unitsArrayDimensionality);		// gets a vector with the array coordinates
       												// corresponding to the otherPosition

	auxiliary = winnerPositionArray - otherPositionArray;
	std::transform(auxiliary.begin(), auxiliary.end(), auxiliary.begin(),
		       static_cast<double(*)(double)>(&std::abs));
	int distance = std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0);

	std::string	STR;
	STR = "gaussian";
	if ( str.compare(STR) == 0 ) {
		return exp(-(pow((double)distance, (double)2)) / (2*widthParameter));		// returns the neighborhood value (gaussian)
	}
	STR = "mex";
	if ( str.compare(STR) == 0 ) {
		return 2/(sqrt(3*widthParameter)*pow(M_PI,(double)0.25)) *
		       (1 - (pow(distance,(double)2))/(pow(widthParameter,(double)2))) *
		       exp(-(pow(distance,(double)2))/(2*pow(widthParameter,(double)2)));	// returns the neighborhood value (mexican hat)
	}

	cout << "StaticUnits object inconsistence: bad function option: " << str << ".\n" << endl;
	exit( EXIT_FAILURE );
} // end function learningNeighborhoodFunction


// function to get the response information from the input
responseInfo	StaticUnits::getResponse( const std::vector<double>& input )
{
	assert((int)input.size() == _inputDimensionality);
	responseInfo response;

	response.distances.resize(_unitsDimensionality);

	//omp_set_num_threads(NUM_THREADS);
	//#pragma omp parallel for default(none) shared(response, input)
	for ( int row = 0; row < _unitsDimensionality; row++ )						// the index row corresponds to the units
		response.distances[row] = vectors_distance(_weights[row], input);

	response.ranking = sort_indexes(response.distances);

	return response;										// returns the units response
} // end function getResponse


// function to get the response information from the input
responseInfo	StaticUnits::getResponse( const std::vector<std::size_t>& inputIndexes )
{
	responseInfo	response;
	
	response.excitation.resize(_unitsDimensionality);
	//omp_set_num_threads(NUM_THREADS);
	//#pragma omp parallel for default(none) shared(response, inputIndexes)
	for ( int row = 0; row < _unitsDimensionality; row++ ) {					// the index row corresponds to the units
		double	innerProduct = 0.0;
		for (const auto& inputIndex : inputIndexes) {
			//assert((int)inputIndex < _inputDimensionality);
			innerProduct += _weights[row][inputIndex];
		}

		response.excitation[row] = innerProduct;
	}
	response.ranking = sort_indexes(response.excitation);

	return response;										// returns the units response
} // end function getResponse


// applies homeostasis to the object
void	StaticUnits::homeostasis( const bool learning, const bool synapticHomeostasis,
				  const bool activationHomeostasis, const double synapticThreshold )
{
	if (activationHomeostasis && (positiveMod(_updateStep,5) == 0)) {
		StaticUnits::activationHomeostasis(10*BUSTING);
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		if ( learning ) {
			if (synapticHomeostasis) {
				StaticUnits::synapticHomeostasis(synapticThreshold);	
			}
			else {
				StaticUnits::synapticGrowthLimitation(synapticThreshold);
			}
		}

		auto	result = std::min_element(_unitsActivity.begin(), _unitsActivity.end());
		int	minimum = _unitsActivity[std::distance(_unitsActivity.begin(), result)];
		minimum = minimum-1;
		transform(_unitsActivity.begin(), _unitsActivity.end(), _unitsActivity.begin(),
			  bind2nd(std::minus<int>(), minimum));

		//omp_set_num_threads(NUM_THREADS);
		//#pragma omp parallel for default(none)
		for ( int row = 0; row < _unitsDimensionality; row++ )
			_weightsSparsity[row] = (get_rectangular_sparsity(_weights[row]) > SPARSITY_THRESHOLD);

		_updateStep = 0;
	}
	_updateStep++;
} // end function homeostasis


// computes synaptic homeostasis over the weights
void	StaticUnits::synapticHomeostasis( const double synapticThreshold )
{
	auto	averageActivity = (double)std::accumulate(_unitsActivity.begin(), _unitsActivity.end(),0)/(double)_unitsActivity.size();
	assert(averageActivity > 0.0);

	StaticUnits::synapticGrowthLimitation(synapticThreshold);
	for ( int row = 0; row < _unitsDimensionality; row++ ) {
		if ( (double)_unitsActivity[row] < 0.1*averageActivity ) {	// if this unit has had not enough activity
		       // tries to fill its weights with neighbor units synaptic weights
			std::vector<int>	radius(_unitsArrayDimensionality.size(),1);
			auto	neighborUnits = neighborhood(row,radius,_unitsArrayDimensionality);
			for ( const auto& neighborUnit : neighborUnits )
				_weights[row] += _weights[neighborUnit];

		       	// tries to fill its weights at random, with random asynaptic weights
			auto	numberOfChoises = (std::size_t)(MASSIVE_PLASTICITY*_inputDimensionality); 
			std::vector<std::size_t>	randomChoises;
			for ( std::size_t randomChoise = 0; randomChoise < numberOfChoises; randomChoise++ ) {
				std::size_t	choise = rand() % _inputDimensionality;
				std::size_t	securityScape = 0;
				while ( std::find(randomChoises.begin(), randomChoises.end(), choise) != randomChoises.end() ) {
					choise = rand() % _inputDimensionality;
					if (securityScape > 0.1*_inputDimensionality)
						break;
						
					securityScape++;
				}
				randomChoises.push_back(choise);
				_weights[row][choise] += randomFromDoubleInterval(SYNAPTIC_DECREMENT,SYNAPTIC_INCREMENT);
			}
		}
	}
} // end function synapticHomeostasis


// computes synaptic growth limitation over the weights
void	StaticUnits::synapticGrowthLimitation( const double synapticThreshold )
{
	for ( int row = 0; row < _unitsDimensionality; row++ ) {
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
void	StaticUnits::activationHomeostasis( const double boostingFactor )
{
	auto	averageActivity = (double)std::accumulate(_unitsActivity.begin(), _unitsActivity.end(),0)/(double)_unitsActivity.size();

	assert(averageActivity > 0.0);
	for ( int row = 0; row < _unitsDimensionality; row++ ) {
		if (_updateStep > 0)
			_activationBoosting[row] = exp(-boostingFactor*(_unitsActivity[row]-averageActivity)/_updateStep);
		else
			_activationBoosting[row] = exp(-boostingFactor*(_unitsActivity[row]-averageActivity));
	}
} // end function activationHomeostasis


// function to save the Self Organizing Map's status in a file
void	StaticUnits::saveStaticUnitsStatus( const std::string& staticUnitsIdentifier, ofstream& outfile )
{
	std::string	str = "StaticUnits_";
	std::string	STR;
	str += staticUnitsIdentifier;
	str += "_";

        // saves _inputDimensionality
	save_as_scalar(str + "inputDimensionality", _inputDimensionality, outfile);

        // saves _unitsDimensionality
	save_as_scalar(str + "unitsDimensionality", _unitsDimensionality, outfile);

        // saves _unitsArrayDimensionality
	save_vector_as_matrix(str + "unitsArrayDimensionality", _unitsArrayDimensionality, outfile);

        // saves _weights
	save_vector_of_vectors_conditionally_as_sparse_matrix(str + "weights",_weights,SPARSITY_THRESHOLD,outfile);

        // saves _updateStep
	save_as_scalar(str + "updateStep", _updateStep, outfile);

        // saves _binaryProcessing
	save_as_bool(str + "binaryProcessing", _binaryProcessing, outfile);

	if ( _binaryProcessing ) {
	        // saves _unitsActivity
		save_vector_as_matrix(str + "unitsActivity", _unitsActivity, outfile);

	        // saves _activationBoosting
		save_vector_as_matrix(str + "activationBusting", _activationBoosting, outfile);

		// saves _weightsSparsity
		save_vector_as_matrix(str + "weightsSparsity", _weightsSparsity, outfile);
	}
} // end functiom saveStaticUnitsStatus


// function to load the Self Organizing Map's status from a file
void	StaticUnits::loadStaticUnitsStatus( const std::string& staticUnitsIdentifier, ifstream& infile )
{
	std::string	str;
	std::string	STR = "StaticUnits_";
	STR += staticUnitsIdentifier;
	STR += "_";

	while ( std::getline(infile, str) ) {

		auto	auxiliary = "# name: " + STR + "inputDimensionality";
		if ( str.compare(auxiliary) == 0 )
			load_scalar(_inputDimensionality, infile);

		auxiliary = "# name: " + STR + "unitsDimensionality";
		if ( str.compare(auxiliary) == 0 )
			load_scalar(_unitsDimensionality, infile);

		auxiliary = "# name: " + STR + "unitsArrayDimensionality";
		if ( str.compare(auxiliary) == 0 )
			load_matrix_to_vector(_unitsArrayDimensionality, infile);

		auxiliary = "# name: " + STR + "weights";
		if ( str.compare(auxiliary) == 0 )
			load_conditional_sparse_matrix_to_vector_of_vectors(_weights,infile);

		auxiliary = "# name: " + STR + "updateStep";
		if ( str.compare(auxiliary) == 0 )
			load_scalar(_updateStep, infile);

		auxiliary = "# name: " + STR + "binaryProcessing";
		if ( str.compare(auxiliary) == 0 )
			load_bool(_binaryProcessing, infile);

		auxiliary = "# name: " + STR + "unitsActivity";
		if ( str.compare(auxiliary) == 0 )
			load_matrix_to_vector(_unitsActivity, infile);

		auxiliary = "# name: " + STR + "activationBusting";
		if ( str.compare(auxiliary) == 0 )
			load_matrix_to_vector(_activationBoosting, infile);

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
		}
	}
} // end functiom loadStaticUnitsStatus


