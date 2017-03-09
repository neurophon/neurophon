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
#include <cassert>

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


// Save array as matrix type in Octave (.mat) file.
template <typename T>
void	save_array_as_matrix( const std::string& name, const std::array<T,DIM>& matrix, ofstream &FILE )
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
} // end template save_array_as_matrix


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

	if ( matrix.numberOfNonZero > 0 ) {
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


// Load matrix type from Octave (.mat) file into array.
template <typename T>
void	load_matrix_to_array( std::array<T,DIM>& arr, ifstream &FILE )
{
	std::size_t	columns;
	std::string	str, STR;

	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_matrix_to_array:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# type: matrix";
	if ( str.compare(STR) == 0 )
	{
		if ( !std::getline(FILE, str) )
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_matrix_to_array:" << endl;
			std::cout << "Cannot read line from file." << endl;
			exit( EXIT_FAILURE );
		}
		STR = "# rows: 1";
		if ( str.compare(STR) != 0 )
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_matrix_to_array:" << endl;
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
			std::cout << "In function load_matrix_to_array:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}
		std::getline(FILE, str);

		for ( std::size_t column = 0; column < columns; column++ )
			FILE >> arr[column];
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_matrix_to_array:" << endl;
		std::cout << "arr must be of type matrix." << endl;
		exit( EXIT_FAILURE );
	}
} // end template load_matrix_to_array


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

		for ( std::size_t row = 0; row < rows; row++ ) {
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


// Loads sparse matrix type in Octave (.mat) file in SparseMatrixElements data .
template <typename T>
void	load_sparse_matrix_to_sparse_matrix_elements( const SparseMatrixElements<T>& arr, ifstream &FILE )
{
	std::size_t	nnz, index, corticalColumn;
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
		for ( std::size_t member = 0; member < nnz; member++ )
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
