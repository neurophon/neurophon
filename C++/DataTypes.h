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
//														//
//		This is a C++ implementation code of Numenta Platform for Intelligent Computing (NuPIC)		//
//														//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// File Name:		DataTypes.h
// File Description:	Data type definitions. This file contains the data type definitions to be used in the code

#ifndef DATATYPES_H
#define DATATYPES_H


enum verb_lev {zero, one, two, three};


struct SparseMatrixElement				// This structure stores values as sparse matrix elements, efficiently
{
	unsigned int	index;				// This is an input index in the column
	float	value;					// This is the value corresponding to input index 'index' in this column
}; // end structure SparsePermanencesMatrix

#endif
