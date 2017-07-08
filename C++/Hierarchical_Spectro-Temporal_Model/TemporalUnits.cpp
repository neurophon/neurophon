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

// File Name:		TemporalUnits.cpp
// File Description:	TemporalUnits member-function definitions. This file contains implementations of the
//			member functions prototyped in TemporalUnits.h.

#include <fstream>
#include <algorithm>
#include <string>

#include "TemporalUnits.h"				// includes definition of class TemporalUnits
#include "../Libraries/Templates.h"
#include "../Libraries/Utilities.h"
#include "../Libraries/Constants.h"
#include "../Libraries/OctaveInterface.h"

using namespace std;

// constructor initializes populationDimensions, numberOfInputs and temporalUnits with variables supplied as arguments
TemporalUnits::TemporalUnits( const std::vector<int>& populationDimensions, int numberOfInputs,
			      const std::vector<int>& temporalUnits )
	// explicitly call base-class constructor
	: StaticUnits(populationDimensions, numberOfInputs)
{
	int	rows;

	_updateStep = 0;
	_temporalUnits.resize(temporalUnits.size());
	for ( int link = 0; link < (int)temporalUnits.size(); link++ ) {
		rows = temporalUnits[link];
		_temporalUnits[link].resize(rows);
		for ( int row = 0; row < rows; row++ )
			_temporalUnits[link][row].resize(_unitsDimensionality);
	}
} // end TemporalUnits constructor


// constructor initializes populationDimensions, numberOfInputs and temporalUnits with variables supplied as arguments
// The weights in the self StaticUnits will be initialized with random values between weightLimits
TemporalUnits::TemporalUnits( const std::vector<int>& populationDimensions, int numberOfInputs,
			      const std::vector<int>& temporalUnits, const std::array<double,2>& weightLimits )
	// explicitly call base-class constructor
	: StaticUnits(populationDimensions, numberOfInputs, 0.6, weightLimits, true)
{
	int	rows;

	_updateStep = 0;
	_temporalUnits.resize(temporalUnits.size());
	for ( int link = 0; link < (int)temporalUnits.size(); link++ ) {
		rows = temporalUnits[link];
		_temporalUnits[link].resize(rows);
		for ( int row = 0; row < rows; row++ )
			_temporalUnits[link][row].resize(_unitsDimensionality);
	}
} // end TemporalUnits constructor


// constructor initializes populationDimensions and numberOfInputs with variables supplied as arguments.
// This loads _temporalUnits with previous vector supplied as argument too
TemporalUnits::TemporalUnits( const std::string& fileName, const std::string& temporalUnitsIdentification )
	// explicitly call base-class constructor
	: StaticUnits(fileName, temporalUnitsIdentification)
{
	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + fileName + ".mat", ios::in | std::ifstream::binary);

	TemporalUnits::loadTemporalUnitsStatus(temporalUnitsIdentification, infile);

	// close the opened file.
	infile.close();
} // end TemporalUnits constructor


// updates _temporalUnits depending on response info.
void	TemporalUnits::Update( const responseInfo& response, const std::vector<int>& linkingUnits,
			       const bool increment, const double threshold, const double learningRate )
{
	double	sum;

	auto	minimumDistanceIndexes = get_indexes_from_value(response.distances, response.distances[response.ranking[0]]);

	int	number;
	if ( minimumDistanceIndexes.size() > 1 ) {
		number = rand() % minimumDistanceIndexes.size();
		if ( increment ) {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				if ( linkingUnits[link] != -1 ) {
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] =
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] + learningRate*SYNAPTIC_INCREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] = 0.0;
				}
			}
		}
		else {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				if ( linkingUnits[link] != -1 ) {
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] =
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] - learningRate*SYNAPTIC_DECREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] = 0.0;
				}
			}
		}

	}
	else {
		if ( increment ) {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				if ( linkingUnits[link] != -1 ) {
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] =
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] + learningRate*SYNAPTIC_INCREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][response.ranking[0]] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][response.ranking[0]] = 0.0;
				}
			}
		}
		else {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				if ( linkingUnits[link] != -1 ) {
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] =
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] - learningRate*SYNAPTIC_DECREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][response.ranking[0]] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][response.ranking[0]] = 0.0;
				}
			}
		}
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		std::vector<int>	indexes;
		for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
			for ( int row = 0; row < (int)_temporalUnits[link].size(); row++ ) {
				sum = std::accumulate(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), 0.0);
				if ( sum > 1 )
					std::transform(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), _temporalUnits[link][row].begin(),
					std::bind2nd(std::divides<double>(),sum));

				indexes = less_than_indexes(_temporalUnits[link][row], threshold);
				set_elements(_temporalUnits[link][row], indexes, 0.0);
			}
		}

		_updateStep = 0;
	}
	_updateStep++;
} // end function Update


// updates _temporalUnits depending on input.
void	TemporalUnits::Update( const std::vector<double>& input, const std::vector<int>& linkingUnits,
			       const bool increment, const double threshold, const double learningRate )
{
	double	sum;
	responseInfo	response;

	response = StaticUnits::getResponse(input);

	auto	minimumDistanceIndexes = get_indexes_from_value(response.distances, response.distances[response.ranking[0]]);

	int	number;
	if ( minimumDistanceIndexes.size() > 1 ) {
		number = rand() % minimumDistanceIndexes.size();
		if ( increment ) {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				if ( linkingUnits[link] != -1 ) {
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] =
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] + learningRate*SYNAPTIC_INCREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] = 0.0;
				}
			}
		}
		else {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				if ( linkingUnits[link] != -1 ) {
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] =
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] - learningRate*SYNAPTIC_DECREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] = 0.0;
				}
			}
		}

	}
	else {
		if ( increment ) {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				if ( linkingUnits[link] != -1 ) {
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] =
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] + learningRate*SYNAPTIC_INCREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][response.ranking[0]] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][response.ranking[0]] = 0.0;
				}
			}
		}
		else {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				if ( linkingUnits[link] != -1 ) {
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] =
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] - learningRate*SYNAPTIC_DECREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][response.ranking[0]] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][response.ranking[0]] = 0.0;
				}
			}
		}
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		std::vector<int>	indexes;
		for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
			for ( int row = 0; row < (int)_temporalUnits[link].size(); row++ ) {
				sum = std::accumulate(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), 0.0);
				if ( sum > 1 )
					std::transform(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), _temporalUnits[link][row].begin(),
					std::bind2nd(std::divides<double>(),sum));

				indexes = less_than_indexes(_temporalUnits[link][row], threshold);
				set_elements(_temporalUnits[link][row], indexes, 0.0);
			}
		}

		_updateStep = 0;
	}
	_updateStep++;
} // end function Update


// updates _temporalUnits depending on unit index.
void	TemporalUnits::Update( const int index, const std::vector<int>& linkingUnits,
			       const bool increment, const double threshold, const double learningRate )
{
	double	sum;

	if ( index > _unitsDimensionality || index < 0 ) {
		cout << "TemporalUnits bad method parameter:" << endl;
		cout << "Wrong index (" << index << ") in Update function." << endl;
		exit( EXIT_FAILURE );
	}

	if ( increment ) {
		for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
			if ( linkingUnits[link] != -1 ) {
				_temporalUnits[link][linkingUnits[link]][index] =
				_temporalUnits[link][linkingUnits[link]][index] + learningRate*SYNAPTIC_INCREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][index] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][index] = 0.0;
			}
		}
	}
	else {
		for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
			if ( linkingUnits[link] != -1 ) {
				_temporalUnits[link][linkingUnits[link]][index] =
				_temporalUnits[link][linkingUnits[link]][index] - learningRate*SYNAPTIC_DECREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][index] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][index] = 0.0;
			}
		}
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		std::vector<int>	indexes;
		for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
			for ( int row = 0; row < (int)_temporalUnits[link].size(); row++ ) {
				sum = std::accumulate(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), 0.0);
				if ( sum > 1 )
					std::transform(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), _temporalUnits[link][row].begin(),
					std::bind2nd(std::divides<double>(),sum));

				indexes = less_than_indexes(_temporalUnits[link][row], threshold);
				set_elements(_temporalUnits[link][row], indexes, 0.0);
			}
		}

		_updateStep = 0;
	}
	_updateStep++;
} // end function Update


// updates _temporalUnits depending on individual unit indexes.
void	TemporalUnits::Update( const int index, const int link, const int linkingUnit,
			       const bool increment, const double threshold, const double learningRate )
{
	double	sum;

	if ( index > _unitsDimensionality || index < 0 ) {
		cout << "TemporalUnits bad method parameter:" << endl;
		cout << "Wrong index (" << index << ") in Update function." << endl;
		exit( EXIT_FAILURE );
	}

	if ( increment ) {
		_temporalUnits[link][linkingUnit][index] =
		_temporalUnits[link][linkingUnit][index] + learningRate*SYNAPTIC_INCREMENT;
		if ( _temporalUnits[link][linkingUnit][index] < 0.0 )
			_temporalUnits[link][linkingUnit][index] = 0.0;
	}
	else {
		_temporalUnits[link][linkingUnit][index] =
		_temporalUnits[link][linkingUnit][index] - learningRate*SYNAPTIC_DECREMENT;
		if ( _temporalUnits[link][linkingUnit][index] < 0.0 )
			_temporalUnits[link][linkingUnit][index] = 0.0;
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		std::vector<int>	indexes;
		for ( int link = 0; link < (int)_temporalUnits.size(); link++ ) {
			for ( int row = 0; row < (int)_temporalUnits[link].size(); row++ ) {
				sum = std::accumulate(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), 0.0);
				if ( sum > 1 )
					std::transform(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), _temporalUnits[link][row].begin(),
					std::bind2nd(std::divides<double>(),sum));

				indexes = less_than_indexes(_temporalUnits[link][row], threshold);
				set_elements(_temporalUnits[link][row], indexes, 0.0);
			}
		}

		_updateStep = 0;
	}
	_updateStep++;
} // end function Update


// updates _temporalUnits neighborhood depending on response info.
void	TemporalUnits::Update( const responseInfo& response, double neighborParameter, const std::vector<int>& linkingUnits,
			       const bool increment, const double threshold, const std::string& str, const double learningRate )
{
	double	sum, neighborhoodValue;

	auto	minimumDistanceIndexes = get_indexes_from_value(response.distances, response.distances[response.ranking[0]]);

	if ( minimumDistanceIndexes.size() > 1 ) {
		auto	number = rand() % minimumDistanceIndexes.size();
		if ( increment ) {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, minimumDistanceIndexes[number], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					if ( linkingUnits[link] != -1 ) {
						_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
											       + neighborhoodValue*learningRate*SYNAPTIC_INCREMENT;
						if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
							_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
					}
				}
			}
		}
		else {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, minimumDistanceIndexes[number], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					if ( linkingUnits[link] != -1 ) {
						_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
											       - neighborhoodValue*learningRate*SYNAPTIC_DECREMENT;
						if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
							_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
					}
				}
			}
		}
	}
	else {
		if ( increment ) {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, response.ranking[0], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					if ( linkingUnits[link] != -1 ) {
						_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
											       + neighborhoodValue*learningRate*SYNAPTIC_INCREMENT;
						if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
							_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
					}
				}
			}
		}
		else {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, response.ranking[0], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					if ( linkingUnits[link] != -1 ) {
						_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
											       - neighborhoodValue*learningRate*SYNAPTIC_DECREMENT;
						if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
							_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
					}
				}
			}
		}
	}


	if ( _updateStep > UPDATE_PERIOD ) {
		std::vector<int>	indexes;
		for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
			for ( int row = 0; row < (int)_temporalUnits[link].size(); row++ ) {
				sum = std::accumulate(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), 0.0);
				if ( sum > 1 )
					std::transform(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), _temporalUnits[link][row].begin(),
					std::bind2nd(std::divides<double>(),sum));

				indexes = less_than_indexes(_temporalUnits[link][row], threshold);
				set_elements(_temporalUnits[link][row], indexes, 0.0);
			}
		}

		_updateStep = 0;
	}
	_updateStep++;
} // end function Update


// updates _temporalUnits neighborhood depending on input.
void	TemporalUnits::Update( const std::vector<double>& input, double neighborParameter, const std::vector<int>& linkingUnits,
			       const bool increment, const double threshold, const std::string& str, const double learningRate )
{
	double	sum, neighborhoodValue;
	responseInfo	response;

	response = StaticUnits::getResponse(input);

	auto	minimumDistanceIndexes = get_indexes_from_value(response.distances, response.distances[response.ranking[0]]);

	if ( minimumDistanceIndexes.size() > 1 ) {
		auto	number = rand() % minimumDistanceIndexes.size();
		if ( increment ) {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, minimumDistanceIndexes[number], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					if ( linkingUnits[link] != -1 ) {
						_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
											       + neighborhoodValue*learningRate*SYNAPTIC_INCREMENT;
						if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
							_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
					}
				}
			}
		}
		else {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, minimumDistanceIndexes[number], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					if ( linkingUnits[link] != -1 ) {
						_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
											       - neighborhoodValue*learningRate*SYNAPTIC_DECREMENT;
						if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
							_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
					}
				}
			}
		}
	}
	else {
		if ( increment ) {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, response.ranking[0], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					if ( linkingUnits[link] != -1 ) {
						_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
											       + neighborhoodValue*learningRate*SYNAPTIC_INCREMENT;
						if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
							_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
					}
				}
			}
		}
		else {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, response.ranking[0], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					if ( linkingUnits[link] != -1 ) {
						_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
											       - neighborhoodValue*learningRate*SYNAPTIC_DECREMENT;
						if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
							_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
					}
				}
			}
		}
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		std::vector<int>	indexes;
		for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
			for ( int row = 0; row < (int)_temporalUnits[link].size(); row++ ) {
				sum = std::accumulate(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), 0.0);
				if ( sum > 1 )
					std::transform(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), _temporalUnits[link][row].begin(),
					std::bind2nd(std::divides<double>(),sum));

				indexes = less_than_indexes(_temporalUnits[link][row], threshold);
				set_elements(_temporalUnits[link][row], indexes, 0.0);
			}
		}

		_updateStep = 0;
	}
	_updateStep++;
} // end function Update


// updates _temporalUnits neighborhood depending on unit index.
void	TemporalUnits::Update( const int index, double neighborParameter, const std::vector<int>& linkingUnits,
			       const bool increment, const double threshold, const std::string& str, const double learningRate )
{
	double	sum, neighborhoodValue;

	if ( index > _unitsDimensionality || index < 0 ) {
		cout << "TemporalUnits bad method parameter:" << endl;
		cout << "Wrong index (" << index << ") in Update function." << endl;
		exit( EXIT_FAILURE );
	}

	if ( increment ) {
		for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
			neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, index, unit, str);
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				if ( linkingUnits[link] != -1 ) {
					_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
										       + neighborhoodValue*learningRate*SYNAPTIC_INCREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
				}
			}
		}
	}
	else {
		for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
			neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, index, unit, str);
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				if ( linkingUnits[link] != -1 ) {
					_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
										       - neighborhoodValue*learningRate*SYNAPTIC_DECREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
				}
			}
		}
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		std::vector<int>	indexes;
		for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
			for ( int row = 0; row < (int)_temporalUnits[link].size(); row++ ) {
				sum = std::accumulate(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), 0.0);
				if ( sum > 1 )
					std::transform(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), _temporalUnits[link][row].begin(),
					std::bind2nd(std::divides<double>(),sum));

				indexes = less_than_indexes(_temporalUnits[link][row], threshold);
				set_elements(_temporalUnits[link][row], indexes, 0.0);
			}
		}

		_updateStep = 0;
	}
	_updateStep++;
} // end function Update


// updates _temporalUnits neighborhood depending on individual unit indexes.
void	TemporalUnits::Update( const int index, double neighborParameter, const int link, const int linkingUnit,
			       const bool increment, const double threshold, const std::string& str, const double learningRate )
{
	double	sum, neighborhoodValue;

	if ( index > _unitsDimensionality || index < 0 ) {
		cout << "TemporalUnits bad method parameter:" << endl;
		cout << "Wrong index (" << index << ") in Update function." << endl;
		exit( EXIT_FAILURE );
	}

	if ( increment ) {
		for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
			neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, index, unit, str);
			_temporalUnits[link][linkingUnit][unit] = _temporalUnits[link][linkingUnit][unit]
								       + neighborhoodValue*learningRate*SYNAPTIC_INCREMENT;
			if ( _temporalUnits[link][linkingUnit][unit] < 0.0 )
				_temporalUnits[link][linkingUnit][unit] = 0.0;
		}
	}
	else {
		for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
			neighborhoodValue = StaticUnits::learningNeighborhood(neighborParameter, index, unit, str);
			_temporalUnits[link][linkingUnit][unit] = _temporalUnits[link][linkingUnit][unit]
								       - neighborhoodValue*learningRate*SYNAPTIC_DECREMENT;
			if ( _temporalUnits[link][linkingUnit][unit] < 0.0 )
				_temporalUnits[link][linkingUnit][unit] = 0.0;
		}
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		std::vector<int>	indexes;
		for ( int link = 0; link < (int)_temporalUnits.size(); link++ ) {
			for ( int row = 0; row < (int)_temporalUnits[link].size(); row++ ) {
				sum = std::accumulate(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), 0.0);
				if ( sum > 1 )
					std::transform(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), _temporalUnits[link][row].begin(),
					std::bind2nd(std::divides<double>(),sum));

				indexes = less_than_indexes(_temporalUnits[link][row], threshold);
				set_elements(_temporalUnits[link][row], indexes, 0.0);
			}
		}

		_updateStep = 0;
	}
	_updateStep++;
} // end function Update


// updates a group of _temporalUnits depending on a set of unit indexes.
// on the other hand, every link -from which information is coming- is a vector
// hence, it could contain a set of linking units as well as a unique linking unit.
void	TemporalUnits::Update( const std::vector<int>& indexes, const twodvector<int>& linkingUnits,
			       const bool increment, const double threshold, const double learningRate )
{
	if ( increment ) { // if increment is true
		for(const auto& index : indexes) {
			if ( index >= 0 ) {
				if ( index > _unitsDimensionality || index < -1 ) {
					cout << "TemporalUnits bad method argument:" << endl;
					cout << "Wrong index (" << index << ") in Update function." << endl;
					exit( EXIT_FAILURE );
				}

				for ( std::size_t link = 0; link < linkingUnits.size(); link++ ) {
					for ( std::size_t alternative = 0; alternative < linkingUnits[link].size(); alternative++ ) {
						if ( linkingUnits[link][alternative] >= 0 ) {
							_temporalUnits[link][linkingUnits[link][alternative]][index] +=
										learningRate*SYNAPTIC_INCREMENT;
						}
					}
				}
			}
		}
	}
	else { // if increment is false, then decrements the synapses
		for(const auto& index : indexes) {
			if ( index >= 0 ) {
				if ( index > _unitsDimensionality || index < -1 ) {
					cout << "TemporalUnits bad method argument:" << endl;
					cout << "Wrong index (" << index << ") in Update function." << endl;
					exit( EXIT_FAILURE );
				}

				for ( std::size_t link = 0; link < linkingUnits.size(); link++ ) {
					for ( std::size_t alternative = 0; alternative < linkingUnits[link].size(); alternative++ ) {
						if ( linkingUnits[link][alternative] >= 0 ) {
							_temporalUnits[link][linkingUnits[link][alternative]][index] -=
										learningRate*SYNAPTIC_DECREMENT;
							// there must not exist negative unit synapses
							if ( _temporalUnits[link][linkingUnits[link][alternative]][index] < 0.0 )
								_temporalUnits[link][linkingUnits[link][alternative]][index] = 0.0;
						}
					}
				}
			}
		}
	}

	if ( _updateStep > UPDATE_PERIOD ) {
		std::vector<int>	indexes;
		for ( int link = 0; link < (int)_temporalUnits.size(); link++ ) {
			for ( int row = 0; row < (int)_temporalUnits[link].size(); row++ ) {
				auto	sum = std::accumulate(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), 0.0);
				if ( sum > 1 )
					std::transform(_temporalUnits[link][row].begin(), _temporalUnits[link][row].end(), _temporalUnits[link][row].begin(),
					std::bind2nd(std::divides<double>(),sum));

				indexes = less_than_indexes(_temporalUnits[link][row], threshold);
				set_elements(_temporalUnits[link][row], indexes, 0.0);
			}
		}

		_updateStep = 0;
	}
	_updateStep++;
} // end function Update


// function to save the TemporalUnits' status in a file
void	TemporalUnits::saveTemporalUnitsStatus( const std::string temporalUnitsIdentification, ofstream& outfile )
{
	std::vector<SparseMatrixElements<double>>	sparseMatrixes;
	sparseMatrixes.resize(_temporalUnits.size());
	std::string	str = "TemporalUnits_";
	std::string	STR;
	str += temporalUnitsIdentification;
	str += "_";
	for ( int link = 0; link < (int)_temporalUnits.size(); link++ ) {
		// convert from dense to sparse matrix
		sparseMatrixes[link] = to_sparse(_temporalUnits[link]);

		// saves _temporalUnits
		STR = std::to_string(link);
		save_sparse_matrix_elements_as_sparse_matrix(str+STR, sparseMatrixes[link], outfile);
		STR.clear();
	}

        // saves _updateStep
	save_as_scalar(str + "updateStep", _updateStep, outfile);
} // end functiom saveTemporalUnitsStatus


// function to load the TemporalUnits' status from a file
void	TemporalUnits::loadTemporalUnitsStatus( const std::string temporalUnitsIdentification, ifstream& infile )
{
	SparseMatrixElements<double>	sparseMatrix;
	std::string	str;
	std::string	STR = "TemporalUnits_";
	STR += temporalUnitsIdentification;
	STR += "_";

	int	link = 0;
	while ( std::getline(infile, str) ) {
		auto	auxiliary = "# name: " + STR + std::to_string(link);
		if ( str.compare(auxiliary) == 0 ) {
			// load _temporalUnits
			load_sparse_matrix_to_sparse_matrix_elements(sparseMatrix, infile);

			// convert from sparse to dense matrix
			_temporalUnits.push_back(from_sparse(sparseMatrix));
			sparseMatrix.rows.clear();
			sparseMatrix.columns.clear();
			sparseMatrix.values.clear();
			link++;
		}

		auxiliary = "# name: " + STR + "updateStep";
		if ( str.compare(auxiliary) == 0 )
			load_scalar(_updateStep, infile);
	}
	_temporalUnits.shrink_to_fit();
} // end functiom loadTemporalUnitsStatus










