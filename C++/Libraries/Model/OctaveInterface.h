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
#include <fstream>
#include <iterator>
#include <sstream>

#include "Templates.h"

#include "DataTypes.h"
#include "Constants.h"

using namespace std;

namespace
{
	const std::string name_tag = "# name: ";
	const std::string type_tag = "# type: ";
	const std::string rows_tag = "# rows: ";
	const std::string columns_tag = "# columns: ";
	const std::string elements_tag = "# elements: ";
	const std::string length_tag = "# length: ";
	const std::string ndims_tag = "# ndims: ";
	const std::string cellname_tag = "<cell-element>";
	const std::string nnz_tag = "# nnz: ";
}


// Saves value as scalar type in Octave (.mat) file.
template <typename T, typename V>
void	save_as_scalar( const std::string& name, T scalar, V &stm )
{
	stm << name_tag << name << endl;
	stm << type_tag << "scalar" << endl;
        stm << scalar;
	stm << "\n\n" << endl;
} // end template save_as_scalar


// Saves value as bool type in Octave (.mat) file.
template <typename T, typename V>
void	save_as_bool( const std::string& name, T boolean, V &stm )
{
	stm << name_tag << name << endl;
	stm << type_tag << "bool" << endl;
        stm << boolean;
	stm << "\n\n" << endl;
} // end template save_as_bool


// Saves value as string type in Octave (.mat) file.
template <typename T, typename V>
void	save_as_string( const std::string& name, const T& str, V &stm )
{
	stm << name_tag << name << endl;
	stm << type_tag << "string" << endl;
	stm << elements_tag << "1" << endl;
	stm << length_tag << str.length() << endl;
        stm << str;
	stm << "\n\n" << endl;
} // end template save_as_string


// Save vector as matrix type in Octave (.mat) file.
template <typename T, typename V>
void	save_vector_as_matrix( const std::string& name, const std::vector<T>& matrix, V &stm )
{
	stm << name_tag << name << "\n" 
	    << type_tag << "matrix\n" 
	    << rows_tag << "1\n"
	    << columns_tag << matrix.size() << endl;

    	std::copy(matrix.begin(), matrix.end(), std::ostream_iterator<T>(stm, " "));

	stm << "\n\n" << endl;
} // end template save_vector_as_matrix


// Save vector of vectors as matrix type in Octave (.mat) file.
template <typename T, typename V>
void	save_vector_of_vectors_as_matrix( const std::string& name, const std::vector<std::vector<T>>& matrix, V &stm )
{
	assert(is_rectangular(matrix));
	assert(get_rectangular_indexes(matrix).size()==2);

	stm << name_tag << name << "\n" 
	    << type_tag << "matrix\n" 
	    << rows_tag << matrix.size() << "\n"
	    << columns_tag << matrix[0].size() << endl;

	std::size_t	rows = matrix.size();
	for ( std::size_t row = 0; row < rows; row++ ) {
    		std::copy(matrix[row].begin(), matrix[row].end(), std::ostream_iterator<T>(stm, " "));
		stm << "\n";
	}
	stm << "\n" << endl;
} // end template save_vector_of_vectors_as_matrix


// Save vector of vectors conditionally as a sparse matrix type in Octave (.mat) file.
// First, measures the matrix sparsity, if this is greather than sparsityThreshold,
// matrix is saved as a sparse array in Octave (.mat) format.
// In the other case, matrix is saved as a normal array in Octave (.mat) format.
template <typename T, typename V>
void	save_vector_of_vectors_conditionally_as_sparse_matrix( const std::string& name, const std::vector<std::vector<T>>& matrix,
							       const double sparsityThreshold, V &stm )
{
	assert(is_rectangular(matrix));
	assert(get_rectangular_indexes(matrix).size()==2);
	if(get_rectangular_sparsity(matrix) > sparsityThreshold) {
		auto	sparseMatrix = to_sparse(matrix);
		save_sparse_matrix_elements_as_sparse_matrix(name,sparseMatrix,stm);

	}
	else {
		save_vector_of_vectors_as_matrix(name,matrix,stm);
	}
} // end template save_vector_of_vectors_conditionally_as_sparse_matrix


// Saves multidimensional vector as matrix type in Octave (.mat) file.
// multidimensional makes reference to more than two dimensions, i. e.
// the data to save is, at least, a vector of vectors of vectors.
template <typename T, typename V, typename U>
void	save_multidimensional_vector_as_matrix( const std::string& name,
						const std::vector<T>& matrix,
					       	const V&, U &stm )
{
	auto	arrayDimensionality = get_dimensionality(matrix);
	std::reverse(arrayDimensionality.begin(),arrayDimensionality.end());

	stm << name_tag << name << "\n" 
	    << type_tag << "matrix\n"
	    << ndims_tag << arrayDimensionality.size() << endl;

    	std::copy(arrayDimensionality.begin(), arrayDimensionality.end(), std::ostream_iterator<std::size_t>(stm, " "));
	stm << endl;

	std::vector<V>    v;
	to_one_dimentional_vector(matrix,v);

    	std::copy(v.begin(), v.end(), std::ostream_iterator<V>(stm, "\n"));
	stm << endl;
} // end template save_multidimensional_vector_as_matrix


// This is a recursive system of overloaded templates which
// saves a multidimensional vector as cell type in Octave (.mat) file.
// multidimensional makes reference to more than one dimension, i. e.
// the data to save is, at least, a vector of vectors.
template <typename T, typename V>
void	save_multidimensional_vector_as_cell( const std::string& name,
					      const std::vector<T>& vector, V &stm )
{
	save_vector_as_matrix(name, vector, stm);
}

template <typename T, typename V>
void	save_multidimensional_vector_as_cell( const std::string& name,
					      const std::vector<std::vector<T>>& matrix, V &stm )
{
	auto	rows = matrix.size();
	stm << name_tag << name << "\n" 
	    << type_tag << "cell\n" 
	    << rows_tag << rows << "\n"
	    << columns_tag << "1" << endl;

	for (std::size_t row = 0; row < rows; row++)
		save_multidimensional_vector_as_cell(cellname_tag,matrix[row],stm);
	
	stm << endl;
} // end template save_multidimensional_vector_as_cell


// Saves a SparseMatrixElements data as sparse matrix type in Octave (.mat) file.
template <typename T, typename V> 
void	save_sparse_matrix_elements_as_sparse_matrix( const std::string& name, const SparseMatrixElements<T>& matrix, V &stm )
{
	assert(matrix.rows.size() == matrix.values.size());
	assert(matrix.columns.size() == matrix.values.size());

	stm << name_tag << name << "\n" 
	    << type_tag << "sparse matrix\n" 
	    << nnz_tag << matrix.numberOfNonZero << "\n"
	    << rows_tag << matrix.numberOfRows << "\n"
	    << columns_tag << matrix.numberOfColumns << endl;

	if ( matrix.numberOfNonZero > 0 ) {
		for ( std::size_t element = 0; element < matrix.values.size(); element++ )
			stm << matrix.rows[element]+1 << ' ' << matrix.columns[element]+1 << ' ' << matrix.values[element] << endl;
	}
	stm << "\n" << endl;
} // end template save_sparse_matrix_elements_as_sparse_matrix


// Load scalar type from Octave (.mat) file into value reference.
template <typename T, typename V>
void	load_scalar( T& value, V &stm )
{
	std::string	str;
	if(!(std::getline(stm, str) && str == type_tag + "scalar")) goto failure;

	if(!(stm >> value)) goto failure;

	return;
	
	failure:
		value = 0.0;
		throw runtime_error("\nload_scalar error\n");
} // end template load_scalar


// Load bool type from Octave (.mat) file into value reference.
template <typename T, typename V>
void	load_bool( T& value, V &stm )
{
	std::string	str;
	if(!(std::getline(stm, str) && str == type_tag + "bool")) goto failure;

	if(!(stm >> value)) goto failure;

	return;
	
	failure:
		value = false;
		throw runtime_error("\nload_bool error\n");
} // end template load_bool


// Load matrix type from Octave (.mat) file into vector.
template <typename T, typename V>
void	load_matrix_to_vector( std::vector<T>& matrix, V &stm )
{
	std::string	str, str1;
	if(!(std::getline(stm, str) && str == type_tag + "matrix")) goto failure;
	if(!(std::getline(stm, str) && str == rows_tag + "1")) goto failure;

	std::size_t expected_size;
	if( !( stm >> str1 && stm >> str && stm >> expected_size ) )
		goto failure;

	str = str1+" "+str+" ";
	if( !( str == columns_tag ) )
		goto failure;

	matrix.clear();
	using iterator = std::istream_iterator<T>;
	std::copy_n(iterator(stm), expected_size, std::back_inserter(matrix));
	if( matrix.size() != expected_size ) goto failure;
	
	return;
	
	failure:
		matrix.clear();
		throw runtime_error("\nload_matrix_to_vector error\n");
} // end template load_matrix_to_vector


// Load matrix type from Octave (.mat) file into vector of vectors.
template <typename T, typename V>
void	load_matrix_to_vector_of_vectors( std::vector<std::vector<T>>& matrix, V &stm )
{
	std::string	str, str1;
	std::vector<std::size_t>	indexes;
	if(!(std::getline(stm, str) && str == type_tag + "matrix")) goto failure;

	std::size_t expected_rows;
	if( !( stm >> str1 && stm >> str && stm >> expected_rows ) )
		goto failure;

	str = str1+" "+str+" ";
	if( !( str == rows_tag ) )
		goto failure;

	std::size_t expected_columns;
	if( !( stm >> str1 && stm >> str && stm >> expected_columns ) )
		goto failure;

	str = str1+" "+str+" ";
	if( !( str == columns_tag ) )
		goto failure;

	matrix.clear();
	using iterator = std::istream_iterator<T>;
	matrix.resize(expected_rows);
	for ( std::size_t row = 0; row < expected_rows; row++ )
		std::copy_n(iterator(stm), expected_columns, std::back_inserter(matrix[row]));

	if(!is_rectangular(matrix)) goto failure;
	indexes = get_rectangular_indexes(matrix);
    	std::reverse(indexes.begin(), indexes.end());
	if(indexes.size()!=2) goto failure;
	if( indexes[0] != expected_rows ) goto failure;
	if( indexes[1] != expected_columns ) goto failure;

	return;
	
	failure:
		matrix.clear();
		throw runtime_error("\nload_matrix_to_vector_of_vectors error\n");
} // end template load_matrix_to_vector_of_vectors


// Load conditional sparse matrix type from Octave (.mat) file into vector of vectors.
template <typename T, typename V>
void	load_conditional_sparse_matrix_to_vector_of_vectors( std::vector<std::vector<T>>& matrix, V &stm )
{
	std::string	str, str1;
	if(!(std::getline(stm, str))) goto failure;

	if(str == type_tag + "sparse matrix") {
		std::size_t	expected_nnz;
		SparseMatrixElements<T>	s_matrix;
		if( !( stm >> str1 && stm >> str && stm >> expected_nnz ) )
			goto failure;

		str = str1+" "+str+" ";
		if( !( str == nnz_tag ) )
			goto failure;

		std::size_t	expected_rows;
		if( !( stm >> str1 && stm >> str && stm >> expected_rows ) )
			goto failure;

		str = str1+" "+str+" ";
		if( !( str == rows_tag ) )
			goto failure;

		std::size_t	expected_columns;
		if( !( stm >> str1 && stm >> str && stm >> expected_columns ) )
			goto failure;

		str = str1+" "+str+" ";
		if( !( str == columns_tag ) )
			goto failure;

		s_matrix.numberOfNonZero = expected_nnz;
		s_matrix.numberOfRows = expected_rows;
		s_matrix.numberOfColumns = expected_columns;
		std::size_t	row, column;
		T	value;
		s_matrix.rows.clear();
		s_matrix.columns.clear();
		s_matrix.values.clear();
		for ( std::size_t member = 0; member < s_matrix.numberOfNonZero; member++ )
		{
			// The vector of vectors was introduced in the file in the following way:
			// stm << row+1 << ' ' << column+1 << ' ' << s_matrix[rowIndex].value << endl;
			stm >> row;
			row--;
			stm >> column;
			column--;
			stm >> value;
			s_matrix.rows.push_back(row);
			s_matrix.columns.push_back(column);
			s_matrix.values.push_back(value);
			std::getline(stm, str);
		}

		s_matrix.rows.shrink_to_fit();
		s_matrix.columns.shrink_to_fit();
		s_matrix.values.shrink_to_fit();

		matrix = from_sparse(s_matrix);

		return;
	}
	else if (str == type_tag + "matrix") {
		std::size_t expected_rows;
		std::vector<std::size_t>	dimensions;
		if( !( stm >> str1 && stm >> str && stm >> expected_rows ) )
			goto failure;

		str = str1+" "+str+" ";
		if( !( str == rows_tag ) )
			goto failure;

		std::size_t expected_columns;
		if( !( stm >> str1 && stm >> str && stm >> expected_columns ) )
			goto failure;

		str = str1+" "+str+" ";
		if( !( str == columns_tag ) )
			goto failure;

		matrix.clear();
		using iterator = std::istream_iterator<T>;
		matrix.resize(expected_rows);
		for ( std::size_t row = 0; row < expected_rows; row++ )
			std::copy_n(iterator(stm), expected_columns, std::back_inserter(matrix[row]));

		if(!is_rectangular(matrix)) goto failure;
		dimensions = get_rectangular_indexes(matrix);
		std::reverse(dimensions.begin(), dimensions.end());
		if(dimensions.size()!=2) goto failure;
		if( dimensions[0] != expected_rows ) goto failure;
		if( dimensions[1] != expected_columns ) goto failure;

		return;
	}

	failure:
		matrix.clear();
		throw runtime_error("\nload_conditional_sparse_matrix_to_vector_of_vectors error\n");
} // end template load_conditional_sparse_matrix_to_vector_of_vectors


// This is a recursive system of overloaded templates which
// load a cell type in Octave (.mat) file to a multidimensional vector.
// multidimensional makes reference to more than one dimension, i. e.
// the data to save is, at least, a vector of vectors.
// matrix passed to the template must have the correct dimensionality in advance,
// if not, the template will return with an error
template <typename T, typename V>
void	load_cell_to_multidimensional_vector( std::vector<T>& vector, V &stm )
{
	load_matrix_to_vector(vector, stm);
}

template <typename T, typename V>
void	load_cell_to_multidimensional_vector( std::vector<std::vector<T>>& matrix, V &stm )
{
	std::size_t	row = 0;
	std::string	str, str1;
	std::vector<std::size_t>	dimensions;
	std::vector<V>	aux;
	if(!(std::getline(stm, str) && str == type_tag + "cell")) goto failure;

	std::size_t	expected_rows;
	if( !( stm >> str1 && stm >> str && stm >> expected_rows ) )
		goto failure;

	str = str1+" "+str+" ";
	if( !( str == rows_tag ) )
		goto failure;

	std::size_t	expected_columns;
	if( !( stm >> str1 && stm >> str && stm >> expected_columns ) )
		goto failure;

	str = str1+" "+str+" ";
	if( !( str == columns_tag ) )
		goto failure;

	matrix.resize(expected_rows);
	while ( std::getline(stm, str) && row < expected_rows ) {
		if ( str == name_tag + cellname_tag )
		{
			load_cell_to_multidimensional_vector(matrix[row],stm);
			row++;
		}
	}

	return;
	
	failure:
		matrix.clear();
		throw runtime_error("\nload_cell_to_multidimensional_vector error\n");
} // end template load_cell_to_multidimensional_vector


// Load  matrix to multidimensional vector from Octave (.mat) file.
// multidimensional makes reference to more than two dimensions, i. e.
// the structure of the argument "matrix" in which the data is load must
// be determined and allocated in advance (before calling the template).
template <typename T, typename V, typename U>
void	load_matrix_to_multidimensional_vector( std::vector<T>& matrix,
						const V&, U &stm )
{
	std::size_t	length;
	std::string	str, str1;
	std::vector<std::size_t>	dimensions;
	std::vector<V>	aux;
	if(!(std::getline(stm, str) && str == type_tag + "matrix")) goto failure;

	std::size_t	expected_ndims;
	if( !( stm >> str1 && stm >> str && stm >> expected_ndims ) )
		goto failure;

	str = str1+" "+str+" ";
	if( !( str == ndims_tag ) )
		goto failure;

	using iterator = std::istream_iterator<std::size_t>;
	std::copy_n(iterator(stm), expected_ndims, std::back_inserter(dimensions));
	if( dimensions.size() != expected_ndims ) goto failure;

	std::getline(stm, str);

	std::reverse(dimensions.begin(), dimensions.end());
	length = std::accumulate(dimensions.begin(), dimensions.end(), 1, std::multiplies<std::size_t>());
	using iterator1 = std::istream_iterator<V>;
	std::copy_n(iterator1(stm), length, std::back_inserter(aux));
	if( aux.size() != length ) goto failure;

	matrix.clear();
	resize_multi_dimentional_vector(matrix,dimensions);
	to_multi_dimentional_vector(matrix, aux);

	return;
	
	failure:
		matrix.clear();
		throw runtime_error("\nload_matrix_to_multidimensional_vector error\n");
} // end template load_matrix_to_multidimensional_vector


// Loads sparse matrix type in Octave (.mat) file in SparseMatrixElements data .
template <typename T, typename V>
void	load_sparse_matrix_to_sparse_matrix_elements( SparseMatrixElements<T>& matrix, V &stm )
{
	std::string	str, str1;
	if(!(std::getline(stm, str) && str == type_tag + "sparse matrix")) goto failure;

	std::size_t	expected_nnz;
	if( !( stm >> str1 && stm >> str && stm >> expected_nnz ) )
		goto failure;

	str = str1+" "+str+" ";
	if( !( str == nnz_tag ) )
		goto failure;

	std::size_t	expected_rows;
	if( !( stm >> str1 && stm >> str && stm >> expected_rows ) )
		goto failure;

	str = str1+" "+str+" ";
	if( !( str == rows_tag ) )
		goto failure;

	std::size_t	expected_columns;
	if( !( stm >> str1 && stm >> str && stm >> expected_columns ) )
		goto failure;

	str = str1+" "+str+" ";
	if( !( str == columns_tag ) )
		goto failure;

	matrix.numberOfNonZero = expected_nnz;
	matrix.numberOfRows = expected_rows;
	matrix.numberOfColumns = expected_columns;
	std::size_t	row, column;
	T	value;
	matrix.rows.clear();
	matrix.columns.clear();
	matrix.values.clear();
	for ( std::size_t member = 0; member < matrix.numberOfNonZero; member++ )
	{
		// The vector of vectors was introduced in the file in the following way:
		// stm << row+1 << ' ' << column+1 << ' ' << matrix[rowIndex].value << endl;
		stm >> row;
		row--;
		stm >> column;
		column--;
		stm >> value;
		matrix.rows.push_back(row);
		matrix.columns.push_back(column);
		matrix.values.push_back(value);
		std::getline(stm, str);
	}

	matrix.rows.shrink_to_fit();
	matrix.columns.shrink_to_fit();
	matrix.values.shrink_to_fit();

	return;
	
	failure:
		matrix.clear();
		throw runtime_error("\nload_sparse_matrix_to_sparse_matrix_elements error\n");
} // end template load_sparse_matrix_to_sparse_matrix_elements


// Load string type from Octave (.mat) file in string reference.
template <typename T, typename V>
void	load_string( T& stri, V &stm )
{
	std::string	str, str1;
	if(!(std::getline(stm, str) && str == type_tag + "string")) goto failure;

	std::size_t	expected_elements;
	if( !( stm >> str1 && stm >> str && stm >> expected_elements ) )
		goto failure;

	str = str1+" "+str+" "+std::to_string(expected_elements);
	if( !( str == elements_tag+"1" ) )
		goto failure;

	std::size_t	expected_length;
	if( !( stm >> str1 && stm >> str && stm >> expected_length ) )
		goto failure;

	str = str1+" "+str+" ";
	if( !( str == length_tag ) )
		goto failure;
	
	if(!(std::getline(stm, str))) goto failure;
				
	stri.resize(expected_length);
	stm.read(&stri[0],expected_length);
	if( stri.size() != expected_length ) goto failure;
	return;

	failure:
		stri.clear();
		throw runtime_error("\nload_string error\n");
} // end template load_string


#endif
