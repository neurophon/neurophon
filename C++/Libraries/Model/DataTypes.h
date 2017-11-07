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

// File Name:		DataTypes.h
// File Description:	Data type definitions. This file contains the data type definitions to be used in the code

#ifndef DATATYPES_H
#define DATATYPES_H

#include <vector>
#include <limits.h>

// This defines a bi-dimensional vector
template <typename T> 
using	twodvector = std::vector<std::vector<T>>;

// This defines a three dimensional vector
template <typename T> 
using	threedvector = std::vector<std::vector<std::vector<T>>>;

// This defines a four dimensional vector
template <typename T> 
using	fourdvector = std::vector<std::vector<std::vector<std::vector<T>>>>;

// This structure stores values as sparse matrix elements efficiently
template <typename T> 
struct SparseMatrixElements
{
	std::size_t	numberOfNonZero;			// This is the number of non zero elements in the matrix
	std::size_t	numberOfRows;				// This is the number of rows in the matrix
	std::size_t	numberOfColumns;			// This is the number of columns in the matrix
	std::vector<std::size_t>	rows;			// This is the vector of rows
	std::vector<std::size_t>	columns;		// This is the vector of columns
	std::vector<T>			values;			// This is the vector of values corresponding to indexes

	void	clear()
	{
		numberOfNonZero = 0;
		numberOfRows = 0;
		numberOfColumns = 0;
		rows.clear();
		columns.clear();
		values.clear();
	}
}; // end structure SparseMatrixElements

#if SIZE_MAX == UCHAR_MAX
   #define my_MPI_SIZE_T MPI_UNSIGNED_CHAR
#elif SIZE_MAX == USHRT_MAX
   #define my_MPI_SIZE_T MPI_UNSIGNED_SHORT
#elif SIZE_MAX == UINT_MAX
   #define my_MPI_SIZE_T MPI_UNSIGNED
#elif SIZE_MAX == ULONG_MAX
   #define my_MPI_SIZE_T MPI_UNSIGNED_LONG
#elif SIZE_MAX == ULLONG_MAX
   #define my_MPI_SIZE_T MPI_UNSIGNED_LONG_LONG
#else
   #error "DataType error: in my_MPI_SIZE_T data type definition"
#endif

#endif
