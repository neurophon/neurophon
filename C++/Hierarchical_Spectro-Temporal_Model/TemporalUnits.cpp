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

// constructor initializes populationDimensions and numberOfInputs with variables supplied as arguments
TemporalUnits::TemporalUnits( const std::vector<int>& populationDimensions, int numberOfInputs,
			      const std::vector<int>& temporalUnits )
	// explicitly call base-class constructor
	: SelfOrganizingMap(populationDimensions, numberOfInputs)
{
	int	rows, columns;

	_updateStep = 0;
	_temporalUnits.resize(temporalUnits.size());
	for ( int link = 0; link < (int)temporalUnits.size(); link++ ) {
		rows = temporalUnits[link];
		_temporalUnits[link].resize(rows);
		columns = _unitsDimensionality;
		for ( int row = 0; row < rows; row++ )
			_temporalUnits[link][row].resize(columns);
	}
} // end TemporalUnits constructor


// constructor initializes populationDimensions and numberOfInputs with variables supplied as arguments.
// This loads _temporalUnits with previous vector supplied as argument too
TemporalUnits::TemporalUnits( const std::vector<int>& populationDimensions, int numberOfInputs,
			      const ThreeD_Vector_Double& temporalUnits )
	// explicitly call base-class constructor
	: SelfOrganizingMap(populationDimensions, numberOfInputs)
{
	_updateStep = 0;
	_temporalUnits.resize(temporalUnits.size());
	for ( int link = 0; link < (int)temporalUnits.size(); link++ ) {
		auto	rows = temporalUnits[link].size();
		_temporalUnits[link].resize(rows);
		for ( int row = 0; row < (int)rows; row++ ) {
			auto	columns = temporalUnits[link][row].size();
			for ( int column = 0; column < (int)columns; column++ ) {
				_temporalUnits[link][row].push_back(temporalUnits[link][row][column]);
			}
			_temporalUnits[link][row].shrink_to_fit();
		}
	}
} // end TemporalUnits constructor


// updates _temporalUnits depending on response info.
void	TemporalUnits::Update( const responseInfo& response, const std::vector<int>& linkingUnits,
			       const bool increment, const double threshold )
{
	double	sum;

	auto	minimumDistanceIndexes = get_indexes_from_value(response.distances, response.distances[response.ranking[0]]);

	int	number;
	if ( minimumDistanceIndexes.size() > 1 ) {
		number = rand() % minimumDistanceIndexes.size();
		if ( increment ) {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] =
				_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] + SYNAPTIC_INCREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] = 0.0;
			}
		}
		else {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] =
				_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] - SYNAPTIC_DECREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] = 0.0;
			}
		}

	}
	else {
		if ( increment ) {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				_temporalUnits[link][linkingUnits[link]][response.ranking[0]] =
				_temporalUnits[link][linkingUnits[link]][response.ranking[0]] + SYNAPTIC_INCREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][response.ranking[0]] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] = 0.0;
			}
		}
		else {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				_temporalUnits[link][linkingUnits[link]][response.ranking[0]] =
				_temporalUnits[link][linkingUnits[link]][response.ranking[0]] - SYNAPTIC_DECREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][response.ranking[0]] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] = 0.0;
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
			       const bool increment, const double threshold )
{
	double	sum;
	responseInfo	response;

	response = SelfOrganizingMap::getResponse(input);

	auto	minimumDistanceIndexes = get_indexes_from_value(response.distances, response.distances[response.ranking[0]]);

	int	number;
	if ( minimumDistanceIndexes.size() > 1 ) {
		number = rand() % minimumDistanceIndexes.size();
		if ( increment ) {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] =
				_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] + SYNAPTIC_INCREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] = 0.0;
			}
		}
		else {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] =
				_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] - SYNAPTIC_DECREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][minimumDistanceIndexes[number]] = 0.0;
			}
		}

	}
	else {
		if ( increment ) {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				_temporalUnits[link][linkingUnits[link]][response.ranking[0]] =
				_temporalUnits[link][linkingUnits[link]][response.ranking[0]] + SYNAPTIC_INCREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][response.ranking[0]] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] = 0.0;
			}
		}
		else {
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				_temporalUnits[link][linkingUnits[link]][response.ranking[0]] =
				_temporalUnits[link][linkingUnits[link]][response.ranking[0]] - SYNAPTIC_DECREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][response.ranking[0]] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][response.ranking[0]] = 0.0;
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
void	TemporalUnits::Update( int index, const std::vector<int>& linkingUnits,
			       const bool increment, const double threshold )
{
	double	sum;

	if ( index > _unitsDimensionality || index < 0 ) {
		cout << "TemporalUnits bad method parameter:" << endl;
		cout << "Wrong index (" << index << ") in Update function." << endl;
		exit( EXIT_FAILURE );
	}

	if ( increment ) {
		for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
			_temporalUnits[link][linkingUnits[link]][index] =
			_temporalUnits[link][linkingUnits[link]][index] + SYNAPTIC_INCREMENT;
			if ( _temporalUnits[link][linkingUnits[link]][index] < 0.0 )
				_temporalUnits[link][linkingUnits[link]][index] = 0.0;
		}
	}
	else {
		for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
			_temporalUnits[link][linkingUnits[link]][index] =
			_temporalUnits[link][linkingUnits[link]][index] - SYNAPTIC_DECREMENT;
			if ( _temporalUnits[link][linkingUnits[link]][index] < 0.0 )
				_temporalUnits[link][linkingUnits[link]][index] = 0.0;
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


// updates _temporalUnits neighborhood depending on response info.
void	TemporalUnits::Update( const responseInfo& response, double neighborParameter, const std::vector<int>& linkingUnits,
			       const bool increment, const double threshold, const std::string& str )
{
	double	sum, neighborhoodValue;

	auto	minimumDistanceIndexes = get_indexes_from_value(response.distances, response.distances[response.ranking[0]]);

	if ( minimumDistanceIndexes.size() > 1 ) {
		auto	number = rand() % minimumDistanceIndexes.size();
		if ( increment ) {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, minimumDistanceIndexes[number], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
										       + neighborhoodValue*SYNAPTIC_INCREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
				}
			}
		}
		else {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, minimumDistanceIndexes[number], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
										       - neighborhoodValue*SYNAPTIC_DECREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
				}
			}
		}
	}
	else {
		if ( increment ) {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, response.ranking[0], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
										       + neighborhoodValue*SYNAPTIC_INCREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
				}
			}
		}
		else {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, response.ranking[0], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
										       - neighborhoodValue*SYNAPTIC_DECREMENT;
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


// updates _temporalUnits neighborhood depending on input.
void	TemporalUnits::Update( const std::vector<double>& input, double neighborParameter, const std::vector<int>& linkingUnits,
			       const bool increment, const double threshold, const std::string& str )
{
	double	sum, neighborhoodValue;
	responseInfo	response;

	response = SelfOrganizingMap::getResponse(input);

	auto	minimumDistanceIndexes = get_indexes_from_value(response.distances, response.distances[response.ranking[0]]);

	if ( minimumDistanceIndexes.size() > 1 ) {
		auto	number = rand() % minimumDistanceIndexes.size();
		if ( increment ) {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, minimumDistanceIndexes[number], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
										       + neighborhoodValue*SYNAPTIC_INCREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
				}
			}
		}
		else {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, minimumDistanceIndexes[number], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
										       - neighborhoodValue*SYNAPTIC_DECREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
				}
			}
		}
	}
	else {
		if ( increment ) {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, response.ranking[0], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
										       + neighborhoodValue*SYNAPTIC_INCREMENT;
					if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
						_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
				}
			}
		}
		else {
			for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
				neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, response.ranking[0], unit, str);
				for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
					_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
										       - neighborhoodValue*SYNAPTIC_DECREMENT;
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


// updates _temporalUnits neighborhood depending on unit index.
void	TemporalUnits::Update( int index, double neighborParameter, const std::vector<int>& linkingUnits,
			       const bool increment, const double threshold, const std::string& str )
{
	double	sum, neighborhoodValue;

	if ( index > _unitsDimensionality || index < 0 ) {
		cout << "TemporalUnits bad method parameter:" << endl;
		cout << "Wrong index (" << index << ") in Update function." << endl;
		exit( EXIT_FAILURE );
	}

	if ( increment ) {
		for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
			neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, index, unit, str);
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
									       + neighborhoodValue*SYNAPTIC_INCREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
			}
		}
	}
	else {
		for ( int unit = 0; unit < _unitsDimensionality; unit++ ) {
			neighborhoodValue = SelfOrganizingMap::learningNeighborhood(neighborParameter, index, unit, str);
			for ( int link = 0; link < (int)linkingUnits.size(); link++ ) {
				_temporalUnits[link][linkingUnits[link]][unit] = _temporalUnits[link][linkingUnits[link]][unit]
									       - neighborhoodValue*SYNAPTIC_DECREMENT;
				if ( _temporalUnits[link][linkingUnits[link]][unit] < 0.0 )
					_temporalUnits[link][linkingUnits[link]][unit] = 0.0;
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


// function to save the Self Organizing Map's status in a file
void	TemporalUnits::saveTemporalUnitsStatus()
{
        // open a file in write mode.
        ofstream outfile;
        outfile.open("../../Octave/TU_Status.mat", ios::out | ios::trunc);

        // file preamble.
        outfile << "# This is a file created by saveTemporalUnitsStatus member function in TemporalUnits class." << endl;
        outfile << "# Author: Dematties Dario Jesus." << endl;

        outfile << "\n\n" << endl;

	std::vector<SparseMatrixElements<double>>	sparseMatrixes;
	sparseMatrixes.resize(_temporalUnits.size());
	std::string	str = "temporalUnits";
	std::string	STR;
	for ( int link = 0; link < (int)_temporalUnits.size(); link++ ) {

		// convert from dense to sparse matrix
		//sparseMatrixes[link] = to_sparse<double>(_temporalUnits[link]);
		sparseMatrixes[link] = to_sparse(_temporalUnits[link]);

		// saves _temporalUnits
		STR = std::to_string(link);
		save_sparse_matrix_elements_as_sparse_matrix(str+STR, sparseMatrixes[link], outfile);
		STR.clear();
	}
	// close the opened file.
	outfile.close();
} // end functiom saveWeights












