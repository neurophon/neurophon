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

// File Name:		OctaveInterface.h
// File Description:	OctaveInterface definitions. This file presents OctaveInterface's interface
//			without revealing the implementations of OctaveInterface's functions,
//			which are defined in OctaveInterface.cpp.

#ifndef OCTAVEINTERFACE_H
#define OCTAVEINTERFACE_H

#include <iostream>
#include <algorithm>
#include <numeric>
#include <cassert>

#include "../Libraries/Templates.h"

#include "DataTypes.h"
#include "Constants.h"

using namespace std;

// Saves value as scalar type in Octave (.mat) file.
template <typename T>
void	save_as_scalar( const std::string& name, T scalar, ofstream &FILE )
{
	std::string nameLine = "# name: ", typeLine = "# type: scalar";
	nameLine += name;
	FILE << nameLine << endl;
	FILE << typeLine << endl;
        FILE << scalar;
	FILE << "\n\n" << endl;
} // end template save_as_scalar


// Saves value as bool type in Octave (.mat) file.
template <typename T>
void	save_as_bool( const std::string& name, T boolean, ofstream &FILE )
{
	std::string nameLine = "# name: ", typeLine = "# type: bool";
	nameLine += name;
	FILE << nameLine << endl;
	FILE << typeLine << endl;
        FILE << boolean;
	FILE << "\n\n" << endl;
} // end template save_as_bool


// Saves value as string type in Octave (.mat) file.
template <typename T>
void	save_as_string( const std::string& name, const T& str, ofstream &FILE )
{
	std::string nameLine = "# name: ", typeLine = "# type: string";
	nameLine += name;
	FILE << nameLine << endl;
	FILE << typeLine << endl;
	FILE << "# elements: 1" << endl;
	FILE << "# length: " << str.length() << endl;
        FILE << str;
	FILE << "\n\n" << endl;
} // end template save_as_string


// Save vector as matrix type in Octave (.mat) file.
template <typename T>
void	save_vector_as_matrix( const std::string& name, const std::vector<T>& matrix, ofstream &FILE )
{
	std::size_t	columns;
	columns = matrix.size();
	std::string	nameLine = "# name: ", typeLine = "# type: matrix";
	std::string	rowsLine = "# rows: 1", columnsLine = "# columns: ";
	nameLine += name;
	FILE << nameLine << endl;
	FILE << typeLine << endl;
	FILE << rowsLine << endl;
	FILE << columnsLine << columns << endl;

	for ( std::size_t column = 0; column < columns; column++ )
		FILE << " " << matrix[column];

	FILE << "\n\n" << endl;
} // end template save_vector_as_matrix


// Save vector of vectors as matrix type in Octave (.mat) file.
template <typename T>
void	save_vector_of_vectors_as_matrix( const std::string& name, const std::vector<std::vector<T>>& matrix, ofstream &FILE )
{
	assert(is_rectangular(matrix));
	assert(get_rectangular_indexes(matrix).size()==2);
	std::size_t	rows, columns;
	rows = matrix.size();
	columns = matrix[0].size();
	std::string	nameLine = "# name: ", typeLine = "# type: matrix";
	std::string	rowsLine = "# rows: ", columnsLine = "# columns: ";
	nameLine += name;
	FILE << nameLine << endl;
	FILE << typeLine << endl;
	FILE << rowsLine << rows << endl;
	FILE << columnsLine << columns << endl;
	for ( std::size_t row = 0; row < rows; row++ ) {
		for ( std::size_t column = 0; column < columns; column++ )
			FILE << " " << matrix[row][column];

		FILE << "\n";
	}
	FILE << "\n" << endl;
} // end template save_vector_of_vectors_as_matrix


// Save vector of vectors conditionally as a sparse matrix type in Octave (.mat) file.
// First, measures the matrix sparsity, if this is greather than sparsityThreshold,
// matrix is saved as a sparse array in Octave (.mat) format.
// In the other case, matrix is saved as a normal array in Octave (.mat) format.
template <typename T>
void	save_vector_of_vectors_conditionally_as_sparse_matrix( const std::string& name, const std::vector<std::vector<T>>& matrix,
							       const double sparsityThreshold, ofstream &FILE )
{
	assert(is_rectangular(matrix));
	assert(get_rectangular_indexes(matrix).size()==2);
	if(get_rectangular_sparsity(matrix) > sparsityThreshold) {
		auto	sparseMatrix = to_sparse(matrix);
		save_sparse_matrix_elements_as_sparse_matrix(name,sparseMatrix,FILE);

	}
	else {
		save_vector_of_vectors_as_matrix(name,matrix,FILE);
	}
} // end template save_vector_of_vectors_conditionally_as_sparse_matrix


// Saves multidimensional vector as matrix type in Octave (.mat) file.
// multidimensional makes reference to more than two dimensions, i. e.
// the data to save is, at least, a vector of vectors of vectors.
template <typename T, typename V>
void	save_multidimensional_vector_as_matrix( const std::string& name,
						const std::vector<T>& matrix, const V&, ofstream &FILE )
{
	std::string	nameLine = "# name: ", typeLine = "# type: matrix";
	std::string	ndimsLine = "# ndims: ";
	nameLine += name;
	FILE << nameLine << endl;
	FILE << typeLine << endl;

	auto	arrayDimensionality = get_dimensionality(matrix);

	std::reverse(arrayDimensionality.begin(),arrayDimensionality.end());
	FILE << ndimsLine << arrayDimensionality.size() << endl;
	for (const auto d : arrayDimensionality)
		FILE << " " << d;

	FILE << endl;

	std::vector<V>    v;

	to_one_dimentional_vector(matrix,v);

	for (const auto& s : v)
		FILE << s << endl;

	FILE << endl;
} // end template save_multidimensional_vector_as_matrix


// This is a recursive system of overloaded templates which
// saves a multidimensional vector as cell type in Octave (.mat) file.
// multidimensional makes reference to more than one dimension, i. e.
// the data to save is, at least, a vector of vectors.
template <typename T>
void	save_multidimensional_vector_as_cell( const std::string& name,
					      const std::vector<T>& vector, ofstream &FILE )
{
	save_vector_as_matrix(name, vector, FILE);
}

template <typename T>
void	save_multidimensional_vector_as_cell( const std::string& name,
					      const std::vector<std::vector<T>>& matrix, ofstream &FILE )
{
	std::string	newName = "<cell-element>";
	std::string	nameLine = "# name: ", typeLine = "# type: cell";
	std::string	rowsLine = "# rows: ", columnsLine = "# columns: 1";
	nameLine += name;
	auto	rows = matrix.size();
	FILE << nameLine << endl;
	FILE << typeLine << endl;
	FILE << rowsLine << rows << endl;
	FILE << columnsLine << endl;

	for (std::size_t row = 0; row < rows; row++)
		save_multidimensional_vector_as_cell(newName,matrix[row],FILE);
	
	FILE << endl;
} // end template save_multidimensional_vector_as_cell


// Saves a SparseMatrixElements data as sparse matrix type in Octave (.mat) file.
template <typename T> 
void	save_sparse_matrix_elements_as_sparse_matrix( const std::string& name, const SparseMatrixElements<T>& matrix, ofstream &FILE )
{
	std::string	nameLine = "# name: ", typeLine = "# type: sparse matrix";

	assert(matrix.rows.size() == matrix.values.size());
	assert(matrix.columns.size() == matrix.values.size());

	nameLine += name;
	FILE << nameLine << endl;
	FILE << typeLine << endl;

	FILE << "# nnz: " << matrix.numberOfNonZero << endl;
	FILE << "# rows: " << matrix.numberOfRows << endl;
	FILE << "# columns: " << matrix.numberOfColumns << endl;

	/*if ( matrix.numberOfNonZero > 0 ) {
		std::vector<int>	columnIndexes;
		std::vector<int>	rowIndexes;
		std::vector<int>	coincidence;
		std::vector<int>::iterator	it;
		int	rowIndex;
		for ( int column = 0; column < matrix.numberOfColumns; column++ ) {
			columnIndexes = get_indexes_from_value(matrix.columns, column);
			if ( columnIndexes.size() > 0 ) {
				for ( int row = 0; row < matrix.numberOfRows; row++ ) {
					rowIndexes = get_indexes_from_value(matrix.rows, row);
					if ( rowIndexes.size() > 0 ) {
						it = find_first_of (rowIndexes.begin(), rowIndexes.end(), columnIndexes.begin(), columnIndexes.end());
						auto	auxiliaryIndex = std::distance(rowIndexes.begin(), it);
						if ( auxiliaryIndex < rowIndexes.size() ) {
							rowIndex = rowIndexes[auxiliaryIndex];
							FILE << row+1 << ' ' << column+1 << ' ' << matrix.values[rowIndex] << endl;
						}
						rowIndexes.clear();
					}
				}
				columnIndexes.clear();
			}
		}
	}*/

	if ( matrix.numberOfNonZero > 0 ) {
		for ( int element = 0; element < (int)matrix.values.size(); element++ )
			FILE << matrix.rows[element]+1 << ' ' << matrix.columns[element]+1 << ' ' << matrix.values[element] << endl;
	}
	FILE << "\n" << endl;
} // end template save_sparse_matrix_elements_as_sparse_matrix


// Load scalar type from Octave (.mat) file into value reference.
template <typename T>
void	load_scalar( T& value, ifstream &FILE )
{
	std::string	str, STR;
	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In template load_scalar:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# type: scalar";
	if ( str.compare(STR) == 0 )
	{
		FILE >> value;
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_scalar:" << endl;
		std::cout << "value must be of type scalar." << endl;
		std::cout << "Yet, it is: " << str << endl;
		exit( EXIT_FAILURE );
	}
} // end template load_scalar


// Load bool type from Octave (.mat) file into value reference.
template <typename T>
void	load_bool( T& value, ifstream &FILE )
{
	std::string	str, STR;
	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In template load_bool:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# type: bool";
	if ( str.compare(STR) == 0 )
	{
		FILE >> value;
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_bool:" << endl;
		std::cout << "value must be of type bool." << endl;
		std::cout << "Yet, it is: " << str << endl;
		exit( EXIT_FAILURE );
	}
} // end template load_bool


// Load matrix type from Octave (.mat) file into vector.
template <typename T>
void	load_matrix_to_vector( std::vector<T>& arr, ifstream &FILE )
{
	std::size_t	columns;
	std::string	str, STR;

	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_matrix_to_vector:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# type: matrix";
	if ( str.compare(STR) == 0 )
	{
		if ( !std::getline(FILE, str) )
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_matrix_to_vector:" << endl;
			std::cout << "Cannot read line from file." << endl;
			exit( EXIT_FAILURE );
		}
		STR = "# rows: 1";
		if ( str.compare(STR) != 0 )
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_matrix_to_vector:" << endl;
			std::cout << "matrix type does not fulfill function conditions." << endl;
			std::cout << "It must be: " << STR << endl;
			std::cout << "Yet, it is: " << str << endl;
			exit( EXIT_FAILURE );
		}

		str.resize(11);
		FILE.read(&str[0],11);
		STR = "# columns: ";
		if ( str.compare(STR) == 0 )
		{
			FILE >> columns;
		}
		else
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_matrix_to_vector:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		arr.resize(columns);
		for ( std::size_t column = 0; column < columns; column++ )
			FILE >> arr[column];
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_matrix_to_vector:" << endl;
		std::cout << "arr must be of type matrix." << endl;
		exit( EXIT_FAILURE );
	}
} // end template load_matrix_to_vector


// Load matrix type from Octave (.mat) file into vector of vectors.
template <typename T>
void	load_matrix_to_vector_of_vectors( std::vector<std::vector<T>>& arr, ifstream &FILE )
{
	std::size_t	rows, columns;
	std::string	str, STR;

	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_matrix_to_vector_of_vectors:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# type: matrix";
	if ( str.compare(STR) == 0 )
	{
		str.resize(8);
		FILE.read(&str[0],8);
		STR = "# rows: ";
		if ( str.compare(STR) == 0 )
		{
			FILE >> rows;
		}
		else
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_matrix_to_vector_of_vectors:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		str.resize(11);
		FILE.read(&str[0],11);
		STR = "# columns: ";
		if ( str.compare(STR) == 0 )
		{
			FILE >> columns;
		}
		else
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_matrix_to_vector_of_vectors:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		arr.resize(rows);
		for ( std::size_t row = 0; row < rows; row++ ) {
			arr[row].resize(columns);
			for ( std::size_t column = 0; column < columns; column++ )
				FILE >> arr[row][column];

			std::getline(FILE, str);
		}
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_matrix_to_vector_of_vectors:" << endl;
		std::cout << "arr must be of type matrix." << endl;
		exit( EXIT_FAILURE );
	}
} // end template load_matrix_to_vector_of_vectors


// Load conditional sparse matrix type from Octave (.mat) file into vector of vectors.
template <typename T>
void	load_conditional_sparse_matrix_to_vector_of_vectors( std::vector<std::vector<T>>& arr, ifstream &FILE )
{
	std::size_t	rows, columns;
	std::string	str, STR, STR1;

	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_conditional_sparse_matrix_to_vector_of_vectors:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# type: matrix";
	STR1 = "# type: sparse matrix";
	if ( str.compare(STR) == 0 )
	{
		str.resize(8);
		FILE.read(&str[0],8);
		STR = "# rows: ";
		if ( str.compare(STR) == 0 )
		{
			FILE >> rows;
		}
		else
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_conditional_sparse_matrix_to_vector_of_vectors:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		str.resize(11);
		FILE.read(&str[0],11);
		STR = "# columns: ";
		if ( str.compare(STR) == 0 )
		{
			FILE >> columns;
		}
		else
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_conditional_sparse_matrix_to_vector_of_vectors:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		arr.resize(rows);
		for ( std::size_t row = 0; row < rows; row++ ) {
			arr[row].resize(columns);
			for ( std::size_t column = 0; column < columns; column++ )
				FILE >> arr[row][column];

			std::getline(FILE, str);
		}
	}
	else if ( str.compare(STR1) == 0 )
	{
		SparseMatrixElements<T>	sparseArr;
		str.resize(7);
		FILE.read(&str[0],7);
		STR = "# nnz: ";
		if ( str.compare(STR) == 0 )
			FILE >> sparseArr.numberOfNonZero;
		else {
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_conditional_sparse_matrix_to_vector_of_vectors:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		str.resize(8);
		FILE.read(&str[0],8);
		STR = "# rows: ";
		if ( str.compare(STR) == 0 )
			FILE >> sparseArr.numberOfRows;
		else {
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_conditional_sparse_matrix_to_vector_of_vectors:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		str.resize(11);
		FILE.read(&str[0],11);
		STR = "# columns: ";
		if ( str.compare(STR) == 0 )
			FILE >> sparseArr.numberOfColumns;
		else {
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_conditional_sparse_matrix_to_vector_of_vectors:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		int	row, column;
		T	value;
		sparseArr.rows.clear();
		sparseArr.columns.clear();
		sparseArr.values.clear();
		for ( int member = 0; member < sparseArr.numberOfNonZero; member++ )
		{
			// The vector of vectors was introduced in the file in the following way:
			// FILE << row+1 << ' ' << column+1 << ' ' << matrix[rowIndex].value << endl;
			FILE >> row;
			row--;
			FILE >> column;
			column--;
			FILE >> value;
			sparseArr.rows.push_back(row);
			sparseArr.columns.push_back(column);
			sparseArr.values.push_back(value);
			std::getline(FILE, str);
		}

		sparseArr.rows.shrink_to_fit();
		sparseArr.columns.shrink_to_fit();
		sparseArr.values.shrink_to_fit();
		arr = from_sparse(sparseArr);
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_conditional_sparse_matrix_to_vector_of_vectors:" << endl;
		std::cout << "arr must be of type matrix or sparse matrix." << endl;
		exit( EXIT_FAILURE );
	}
} // end template load_conditional_sparse_matrix_to_vector_of_vectors


// This is a recursive system of overloaded templates which
// load a cell type in Octave (.mat) file to a multidimensional vector.
// multidimensional makes reference to more than one dimension, i. e.
// the data to save is, at least, a vector of vectors.
// matrix passed to the template must have the correct dimensionality in advance,
// if not, the template will return with an error
template <typename T>
void	load_cell_to_multidimensional_vector( std::vector<T>& vector, ifstream &FILE )
{
	load_matrix_to_vector(vector, FILE);
}

template <typename T>
void	load_cell_to_multidimensional_vector( std::vector<std::vector<T>>& matrix, ifstream &FILE )
{
	std::size_t	rows;
	std::string	str, STR;

	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_cell_to_multidimensional_vector:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# type: cell";
	if ( str.compare(STR) == 0 )
	{
		str.resize(8);
		FILE.read(&str[0],8);
		STR = "# rows: ";
		if ( str.compare(STR) == 0 )
		{
			FILE >> rows;
		}
		else
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_cell_to_multidimensional_vector:" << endl;
			std::cout << "File corrupted." << endl;
			std::cout << "Last reading must be: " << STR << endl;
			std::cout << "Yet, it is: " << str << endl;
			exit( EXIT_FAILURE );
		}
		
		std::getline(FILE, str);
		if ( !std::getline(FILE, str) )
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_cell_to_multidimensional_vector:" << endl;
			std::cout << "Cannot read line from file." << endl;
			exit( EXIT_FAILURE );
		}

		STR = "# columns: 1";
		if ( str.compare(STR) != 0 )
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_cell_to_multidimensional_vector:" << endl;
			std::cout << "matrix type does not fulfill function conditions." << endl;
			std::cout << "It must be: " << STR << endl;
			std::cout << "Yet, it is: " << str << endl;
			exit( EXIT_FAILURE );
		}

		matrix.resize(rows);
		std::size_t	row = 0;
		while ( std::getline(FILE, str) && row < rows ) {
			STR = "# name: <cell-element>";
			if ( str.compare(STR) == 0 )
			{
				load_cell_to_multidimensional_vector(matrix[row],FILE);
				row++;
			}
		}
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_cell_to_multidimensional_vector:" << endl;
		std::cout << "arr must be of type cell." << endl;
		exit( EXIT_FAILURE );
	}
} // end template load_cell_to_multidimensional_vector


// Load  matrix to multidimensional vector from Octave (.mat) file.
// multidimensional makes reference to more than two dimensions, i. e.
// the structure of the argument "matrix" in which the data is load must
// be determined and allocated in advance (before calling the template).
template <typename T, typename V>
void	load_matrix_to_multidimensional_vector( std::vector<T>& matrix, const V&, ifstream &FILE )
{
	std::size_t	ndims;
	std::string	str, STR;

	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_matrix_to_multidimensional_vector:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# type: matrix";
	if ( str.compare(STR) == 0 )
	{
		str.resize(9);
		FILE.read(&str[0],9);
		STR = "# ndims: ";
		if ( str.compare(STR) == 0 )
		{
			FILE >> ndims;
		}
		else
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_matrix_to_multidimensional_vector:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		std::vector<size_t> dimensions;
		dimensions.resize(ndims);
		for ( size_t dim = 0; dim < ndims; dim++ )
			FILE >> dimensions[dim];
		
		std::getline(FILE, str);

		std::reverse(dimensions.begin(), dimensions.end());
		auto	length = std::accumulate(dimensions.begin(), dimensions.end(), 1, std::multiplies<size_t>());
		std::vector<V>	aux(length);
		for(auto& v : aux)
			FILE >> v;

		resize_multi_dimentional_vector(matrix,dimensions);
		to_multi_dimentional_vector(matrix, aux);
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_matrix_to_multidimensional_vector:" << endl;
		std::cout << "loaded data must be of type matrix." << endl;
		exit( EXIT_FAILURE );
	}
} // end template load_matrix_to_multidimensional_vector


// Loads sparse matrix type in Octave (.mat) file in SparseMatrixElements data .
template <typename T>
void	load_sparse_matrix_to_sparse_matrix_elements( SparseMatrixElements<T>& arr, ifstream &FILE )
{
	std::string	str, STR;

	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_sparse_matrix_to_sparse_matrix_elements:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# type: sparse matrix";
	if ( str.compare(STR) == 0 )
	{

		str.resize(7);
		FILE.read(&str[0],7);
		STR = "# nnz: ";
		if ( str.compare(STR) == 0 )
			FILE >> arr.numberOfNonZero;
		else {
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_sparse_matrix_to_sparse_matrix_elements:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		str.resize(8);
		FILE.read(&str[0],8);
		STR = "# rows: ";
		if ( str.compare(STR) == 0 )
			FILE >> arr.numberOfRows;
		else {
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_sparse_matrix_to_sparse_matrix_elements:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		str.resize(11);
		FILE.read(&str[0],11);
		STR = "# columns: ";
		if ( str.compare(STR) == 0 )
			FILE >> arr.numberOfColumns;
		else {
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_sparse_matrix_to_sparse_matrix_elements:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		int	row, column;
		T	value;
		arr.rows.clear();
		arr.columns.clear();
		arr.values.clear();
		for ( int member = 0; member < arr.numberOfNonZero; member++ )
		{
			// The vector of vectors was introduced in the file in the following way:
			// FILE << row+1 << ' ' << column+1 << ' ' << matrix[rowIndex].value << endl;
			FILE >> row;
			row--;
			FILE >> column;
			column--;
			FILE >> value;
			arr.rows.push_back(row);
			arr.columns.push_back(column);
			arr.values.push_back(value);
			std::getline(FILE, str);
		}

		arr.rows.shrink_to_fit();
		arr.columns.shrink_to_fit();
		arr.values.shrink_to_fit();
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_sparse_matrix_to_sparse_matrix_elements:" << endl;
		std::cout << "arr must be of type sparse matrix." << endl;
		exit( EXIT_FAILURE );
	}
} // end template load_sparse_matrix_to_sparse_matrix_elements


// Load string type from Octave (.mat) file in string reference.
template <typename T>
void	load_string( T& stri, ifstream &FILE )
{
	std::size_t	length;
	std::string	str, STR;
	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In template load_string:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# type: string";
	if ( !str.compare(STR) == 0 )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_string:" << endl;
		std::cout << "value must be of type string." << endl;
		std::cout << "Yet, it is: " << str << endl;
		exit( EXIT_FAILURE );
	}

	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In template load_string:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# elements: 1";
	if ( !str.compare(STR) == 0 )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_string:" << endl;
		std::cout << "read line must be # elements: 1." << endl;
		std::cout << "Yet, it is: " << str << endl;
		exit( EXIT_FAILURE );
	}

	str.resize(10);
	FILE.read(&str[0],10);
	STR = "# length: ";
	if ( str.compare(STR) == 0 )
	{
		FILE >> length;
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_string:" << endl;
		std::cout << "File corrupted." << endl;
		exit( EXIT_FAILURE );
	}
	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In template load_string:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	stri.resize(length);
	FILE.read(&stri[0],length);
} // end template load_string

#endif
