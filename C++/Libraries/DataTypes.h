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


// This defines a bi-dimensional vector
template <typename T> 
using	twodvector = std::vector<std::vector<T>>;

// This defines a three dimensional vector
template <typename T> 
using	threedvector = std::vector<std::vector<std::vector<T>>>;

// This structure stores values as sparse matrix elements efficiently
template <typename T> 
struct SparseMatrixElements
{
	int	numberOfNonZero;			// This is the number of non zero elements in the matrix
	int	numberOfRows;				// This is the number of rows in the matrix
	int	numberOfColumns;			// This is the number of columns in the matrix
	std::vector<int>	rows;			// This is the vector of rows
	std::vector<int>	columns;		// This is the vector of columns
	std::vector<T>		values;			// This is the vector of values corresponding to indexes
}; // end structure SparseMatrixElements


#endif
