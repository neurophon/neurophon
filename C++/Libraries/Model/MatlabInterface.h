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

// File Name:		MatlabInterface.h
// File Description:	MatlabInterface definitions. This file presents MatlabInterface's interface
//			without revealing the implementations of MatlabInterface's functions,
//			which are defined in MatlabInterface.cpp.

#ifndef MATLABIMTERFACE_H
#define MATLABIMTERFACE_H

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

#include "../../Model/Structs.h"

// reads count number of characters from stream into string
template <typename V>
bool	read_from_stm_to_str( std::string& str, V &stm, uint32_t count)
{
	str.clear();
	std::vector<char>	result(count);  // Because vector is guranteed to be contiguous in C++03
	stm.read(&result[0], count);

	if ( stm.gcount() == count ) {
		str = std::string(&result[0], &result[count]);
		return	true;
	}
	else {
		stm.clear();
		return	false;
	}
}


// reads double from stream
template <typename T, typename V>
bool	read_from_stm_to_number( T& number, V &stm)
{
        double read;

	stm.read(reinterpret_cast<char*>(&read), sizeof(read));

	if ( stm.gcount() == sizeof(read) ) {
		number = read;
		return	true;
	}
	else {
		return	false;
	}

}



// this template reads in binary format from a stream
// and put the value in a natural number 
template <typename V>
bool	read_size_t_as_ubinary_number( std::size_t& number,
				       const std::size_t numberOfBytes,
				       V &stm,
				       const bool big_endian )
{
	if ( numberOfBytes > 8 ) {
		std::cout << "\nIn function read_size_t_as_ubinary_number" << std::endl;
		throw std::runtime_error ("numberOfBytes argument exceeds the binary number representation capacity");
	}

	if ( numberOfBytes != 1 && numberOfBytes % 2 != 0 ) {
		std::cout << "\nIn function read_size_t_as_ubinary_number" << std::endl;
		throw std::runtime_error ("numberOfBytes argument must be a power of two");
	}

	if ( numberOfBytes == 0 ) {
		std::cout << "\nIn function read_size_t_as_ubinary_number" << std::endl;
		throw std::runtime_error ("numberOfBytes argument cannot be 0");
	}

	std::string	str;
	switch (numberOfBytes) {
		case 1: 
			if ( !read_from_stm_to_str(str,stm,1) )
				return	false;
		break;
		case 2: 
			if ( !read_from_stm_to_str(str,stm,2) )
				return	false;
		break;
		case 4: 
			if ( !read_from_stm_to_str(str,stm,4) )
				return	false;
		break;
		case 8: 
			if ( !read_from_stm_to_str(str,stm,8) )
				return	false;
		break;
	}

	if ( !big_endian )
		std::reverse(str.begin(),str.end());

	number = 0;
	for ( std::size_t i = 0; i < numberOfBytes; i++ ) {
		auto	back = str.back();
		auto	auxiliary = (std::uint8_t)back;
		number += auxiliary * std::pow(2,i*8);
		str.pop_back();
	}

	return	true;
} // end template read_size_t_as_ubinary_number	


// this template reads in binary format from a stream
// and put the value in a natural number 
template <typename V>
bool	read_size_t_as_ubinary_number( std::size_t& number,
				       const std::size_t numberOfBytes,
				       V &stm,
				       const bool big_endian,
				       const std::vector<std::uint8_t>& mask )
{
	if ( numberOfBytes > 8 ) {
		std::cout << "\nIn function read_size_t_as_ubinary_number" << std::endl;
		throw std::runtime_error ("numberOfBytes argument exceeds the binary number representation capacity");
	}

	if ( numberOfBytes != 1 && numberOfBytes % 2 != 0 ) {
		std::cout << "\nIn function read_size_t_as_ubinary_number" << std::endl;
		throw std::runtime_error ("numberOfBytes argument must be a power of two");
	}

	if ( numberOfBytes == 0 ) {
		std::cout << "\nIn function read_size_t_as_ubinary_number" << std::endl;
		throw std::runtime_error ("numberOfBytes argument cannot be 0");
	}

	if ( mask.size() != numberOfBytes ) {
		std::cout << "\nIn function read_size_t_as_ubinary_number" << std::endl;
		throw std::runtime_error ("mask vector must have numberOfBytes elements");
	}

	std::string	str;
	switch (numberOfBytes) {
		case 1: 
			if ( !read_from_stm_to_str(str,stm,1) )
				return	false;
		break;
		case 2: 
			if ( !read_from_stm_to_str(str,stm,2) )
				return	false;
		break;
		case 4: 
			if ( !read_from_stm_to_str(str,stm,4) )
				return	false;
		break;
		case 8: 
			if ( !read_from_stm_to_str(str,stm,8) )
				return	false;
		break;
	}

	if ( !big_endian )
		std::reverse(str.begin(),str.end());

	number = 0;
	for ( std::size_t i = 0; i < numberOfBytes; i++ ) {
		auto	back = str.back();
		auto	auxiliary = (std::size_t)back;
		auxiliary &= mask[i];
		number += auxiliary * std::pow(2,i*8);
		str.pop_back();
	}

	return	true;
} // end template read_size_t_as_ubinary_number	


// this template writes a natural number in binary format on a atream
template <typename V>
void	write_size_t_as_ubinary_number( std::size_t numberOfBytes,
					const std::size_t number,
				       	V &stm,
					const bool big_endian )
{
	if ( numberOfBytes > 8 ) {
		std::cout << "\nIn function write_size_t_as_ubinary_number" << std::endl;
		throw std::runtime_error ("numberOfBytes argument exceeds the binary number representation capacity");
	}

	if ( numberOfBytes != 1 && numberOfBytes % 2 != 0 ) {
		std::cout << "\nIn function write_size_t_as_ubinary_number" << std::endl;
		throw std::runtime_error ("numberOfBytes argument must be a power of two");
	}

	if ( std::pow(2,numberOfBytes*8) <= number ) {
		std::cout << "\nIn function write_size_t_as_ubinary_number" << std::endl;
		throw std::runtime_error ("number argument exceeds the binary number capacity");
	}

	std::string	str;
	std::uint8_t	padding = 0;
	while ( std::pow(2,(numberOfBytes-1)*8) > number ) {
		str.push_back(padding);
		numberOfBytes--;
		if ( numberOfBytes == 0 )
			break;
	}

	std::uint8_t	residue8 = number;
	std::uint16_t	residue16 = number;
	std::uint32_t	residue32 = number;
	std::uint64_t	residue64 = number;
	switch (numberOfBytes) {
		case 0:
		break;
		case 1: 
			residue8 = number;
			str.push_back(residue8);
		break;
		case 2: 
			residue16 = number;
			std::array<std::uint8_t,2>	splited2;
			for ( std::size_t i = 0; i < 2; i++ ) {
				splited2[i] = residue16;
				residue16 = residue16/256;
			}
			for ( std::size_t i = 2; i > 0; i-- )
				str.push_back(splited2[i-1]);
		break;
		case 3: 
			residue32 = number;
			std::array<std::uint8_t,3>	splited3;
			for ( std::size_t i = 0; i < 3; i++ ) {
				splited3[i] = residue32;
				residue32 = residue32/256;
			}
			for ( std::size_t i = 3; i > 0; i-- )
				str.push_back(splited3[i-1]);
		break;
		case 4: 
			residue32 = number;
			std::array<std::uint8_t,4>	splited4;
			for ( std::size_t i = 0; i < 4; i++ ) {
				splited4[i] = residue32;
				residue32 = residue32/256;
			}
			for ( std::size_t i = 4; i > 0; i-- )
				str.push_back(splited4[i-1]);
		break;
		case 5: 
			residue64 = number;
			std::array<std::uint8_t,5>	splited5;
			for ( std::size_t i = 0; i < 5; i++ ) {
				splited5[i] = residue64;
				residue64 = residue64/256;
			}
			for ( std::size_t i = 5; i > 0; i-- )
				str.push_back(splited5[i-1]);
		break;
		case 6: 
			residue64 = number;
			std::array<std::uint8_t,6>	splited6;
			for ( std::size_t i = 0; i < 6; i++ ) {
				splited6[i] = residue64;
				residue64 = residue64/256;
			}
			for ( std::size_t i = 6; i > 0; i-- )
				str.push_back(splited6[i-1]);
		break;
		case 7: 
			residue64 = number;
			std::array<std::uint8_t,7>	splited7;
			for ( std::size_t i = 0; i < 7; i++ ) {
				splited7[i] = residue64;
				residue64 = residue64/256;
			}
			for ( std::size_t i = 7; i > 0; i-- )
				str.push_back(splited7[i-1]);
		break;
		case 8: 
			residue64 = number;
			std::array<std::uint8_t,8>	splited8;
			for ( std::size_t i = 0; i < 8; i++ ) {
				splited8[i] = residue64;
				residue64 = residue64/256;
			}
			for ( std::size_t i = 8; i > 0; i-- )
				str.push_back(splited8[i-1]);
		break;
	}

	if ( big_endian ) {
		stm << str;
	}
	else {
		std::reverse(str.begin(),str.end());
		stm << str;
	}
} // end template write_size_t_as_ubinary_number	


// writes numeric array preamble
template <typename V>
void	wirte_numeric_array_preamble( const std::string& name,
				      const std::array<std::size_t,2>& size,
				      V &stm )
{
	std::size_t	name_size;
	// this saves a vector as a miMATRIX
	if ( name.size() > 4 ) { // naormal name format
		name_size = name.size()/8;
		name_size *= 8;
		if ( name.size()%8 != 0 )
			name_size += 8;
	}
	else { // compressed name format
		name_size = 4;
	}

	std::size_t	array_size = size[0]*size[1];
	std::size_t	total_length;
	if ( name.size() > 4 ) { // normal name format
		total_length = 6*8+name_size+array_size*8;
	}
	else { // compressed name format
		total_length = 6*8+array_size*8;
	}

	bool	big_endianness = is_big_endian();

	// miMATRIX tag
	write_size_t_as_ubinary_number(4, 14, stm, big_endianness);
	write_size_t_as_ubinary_number(4, total_length, stm, big_endianness);

	// Array fags
	write_size_t_as_ubinary_number(4, 6, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 8, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 6, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 0, stm, big_endianness);

	// Dimensions array
	write_size_t_as_ubinary_number(4, 5, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 8, stm, big_endianness);
	write_size_t_as_ubinary_number(4, size[0], stm, big_endianness);
	write_size_t_as_ubinary_number(4, size[1], stm, big_endianness);

	// Array name
	if ( name.size() > 4 ) { // normal name format
		write_size_t_as_ubinary_number(4, 1, stm, big_endianness);
		write_size_t_as_ubinary_number(4, name.size(), stm, big_endianness);
		stm << name;
		for ( std::size_t i = 0; i < name_size-name.size(); i++ )
			write_size_t_as_ubinary_number(1, 0, stm, big_endianness);
	}
	else { // compressed name format
		std::size_t	code = name.size()*std::pow(2,16)+1;
		write_size_t_as_ubinary_number(4, code, stm, big_endianness);
		stm << name;
		for ( std::size_t i = 0; i < name_size-name.size(); i++ )
			write_size_t_as_ubinary_number(1, 0, stm, big_endianness);
	}

	// miDOUBLE numbers
	write_size_t_as_ubinary_number(4, 9, stm, big_endianness);
	write_size_t_as_ubinary_number(4, array_size*8, stm, big_endianness);
} // end template wirte_numeric_array_preamble


// writes sparse array preamble
template <typename V>
void	wirte_sparse_array_preamble( const std::string& name,
				     const std::array<std::size_t,2>& size,
				     V &stm,
				     std::size_t nnz,
				     std::vector<std::size_t> rows,
				     std::vector<std::size_t> columns )
{
	assert(nnz==rows.size());
	assert(nnz==columns.size());

	// computes the memory used by the row indices
	std::size_t	size_row_indices = rows.size()*4;
	if ( size_row_indices%8 != 0 )
		size_row_indices += 4;

	// computes the memory used by the column indices
	std::size_t	size_column_indices = (size[1]+1)*4;
	if ( size_column_indices%8 != 0 )
		size_column_indices += 4;

	std::size_t	name_size;
	// this saves a vector as a miMATRIX
	if ( name.size() > 4 ) { // naormal name format
		name_size = name.size()/8;
		name_size *= 8;
		if ( name.size()%8 != 0 )
			name_size += 8;
	}
	else { // compressed name format
		name_size = 4;
	}

	std::size_t	total_length;
	if ( name.size() > 4 ) { // normal name format
		total_length = 7*8+8+name_size+size_row_indices
					      +size_column_indices
					      +nnz*8;
	}
	else { // compressed name format
		total_length = 7*8+8+size_row_indices
				    +size_column_indices
				    +nnz*8;
	}

	bool	big_endianness = is_big_endian();

	// miMATRIX tag
	write_size_t_as_ubinary_number(4, 14, stm, big_endianness);
	write_size_t_as_ubinary_number(4, total_length, stm, big_endianness);

	// Array fags
	write_size_t_as_ubinary_number(4, 6, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 8, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 5, stm, big_endianness);
	if (nnz == 0)
		write_size_t_as_ubinary_number(4, 1, stm, big_endianness);
	else
		write_size_t_as_ubinary_number(4, nnz, stm, big_endianness);

	// Dimensions array
	write_size_t_as_ubinary_number(4, 5, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 8, stm, big_endianness);
	write_size_t_as_ubinary_number(4, size[0], stm, big_endianness);
	write_size_t_as_ubinary_number(4, size[1], stm, big_endianness);

	// Array name
	if ( name.size() > 4 ) { // normal name format
		write_size_t_as_ubinary_number(4, 1, stm, big_endianness);
		write_size_t_as_ubinary_number(4, name.size(), stm, big_endianness);
		stm << name;
		for ( std::size_t i = 0; i < name_size-name.size(); i++ )
			write_size_t_as_ubinary_number(1, 0, stm, big_endianness);
	}
	else { // compressed name format
		std::size_t	code = name.size()*std::pow(2,16)+1;
		write_size_t_as_ubinary_number(4, code, stm, big_endianness);
		stm << name;
		for ( std::size_t i = 0; i < name_size-name.size(); i++ )
			write_size_t_as_ubinary_number(1, 0, stm, big_endianness);
	}
	
	// ir (row indices)
	write_size_t_as_ubinary_number(4, 5, stm, big_endianness);
	write_size_t_as_ubinary_number(4, nnz*4, stm, big_endianness);
	for ( std::size_t row = 0; row < rows.size(); row++ )
		write_size_t_as_ubinary_number(4, rows[row], stm, big_endianness);
		
	for ( std::size_t i = 0; i < size_row_indices-rows.size()*4; i++ )
		write_size_t_as_ubinary_number(1, 0, stm, big_endianness);

	// jc (column indices)
	write_size_t_as_ubinary_number(4, 5, stm, big_endianness);
	write_size_t_as_ubinary_number(4, (size[1]+1)*4, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 0, stm, big_endianness);
	std::size_t	last_column = 0;
	std::size_t	column_accum = 0;
	for ( std::size_t column = 0; column < columns.size(); column++ ) {
		if ( columns[column] == last_column ) {
			column_accum++;
		}
		else {
			for ( std::size_t i = 0; i < columns[column]-last_column; i++ )
				write_size_t_as_ubinary_number(4, column_accum, stm, big_endianness);

			last_column = columns[column];
			column_accum++;
		}
	}
	for ( std::size_t i = 0; i < size[1]-last_column; i++ )
		write_size_t_as_ubinary_number(4, column_accum, stm, big_endianness);
	
	for ( std::size_t i = 0; i < size_column_indices-(size[1]+1)*4; i++ )
		write_size_t_as_ubinary_number(1, 0, stm, big_endianness);
	
	// miDOUBLE numbers
	write_size_t_as_ubinary_number(4, 9, stm, big_endianness);
	write_size_t_as_ubinary_number(4, nnz*8, stm, big_endianness);
} // end template wirte_sparse_array_preamble


// gets the length of a vector to be saved
// as a numeric array in Matlab (.mat)
// compatible file format.
template <typename T>
std::size_t	get_length_of_vector_as_numeric_array( const std::string& name,
						       const std::vector<T>& vector )
{
	std::size_t	name_size;
	if ( name.size() > 4 ) { // normal name format
		name_size = name.size()/8;
		name_size *= 8;
		if ( name.size()%8 != 0 )
			name_size += 8;
	}
	else { // compressed name format
		name_size = 4;
	}

	std::size_t	total_length;
	if ( name.size() > 4 ) { // normal name format
		total_length = 5*8+8+name_size+vector.size()*8;
	}
	else { // compressed name format
		total_length = 5*8+4+name_size+vector.size()*8;
	}

	return	total_length;
} // end template get_length_of_vector_as_numeric_array


// This is a recursive system of overloaded templates which
// gets the length -in Bytes- of a multidimensional vector
// to be saved as cell type in Matlab (.mat) format.
// Multidimensional makes reference to more than one dimension, i. e.
// the data to be scrutinised is, at least, a vector of vectors.
template <typename T>
std::size_t	get_length_of_multidimensional_vector_as_cell( const std::string& name,
							       const std::vector<T>& vector )
{
	return	get_length_of_vector_as_numeric_array(name, vector);
}

template <typename T>
std::size_t	get_length_of_multidimensional_vector_as_cell( const std::string& name,
							       const std::vector<std::vector<T>>& matrix )
{
	std::size_t	name_size;
	if ( name.size() > 4 ) { // normal name format
		name_size = name.size()/8;
		name_size *= 8;
		if ( name.size()%8 != 0 )
			name_size += 8;
	}
	else { // compressed name format
		name_size = 4;
	}

	std::size_t	total_length;
	if ( name.size() > 4 ) { // normal name format
		total_length = 4*8+8+name_size;
	}
	else { // compressed name format
		total_length = 4*8+4+name_size;
	}

	auto	rows = matrix.size();
	for (std::size_t row = 0; row < rows; row++)
		total_length = total_length + 8 + get_length_of_multidimensional_vector_as_cell("<cell-element>",matrix[row]);
	
	return	total_length;
} // end template get_length_of_multidimensional_vector_as_cell


// writes cell array preamble
template <typename T, typename V>
void	wirte_cell_array_preamble( const std::string& name,
				   const std::vector<std::vector<T>>& matrix, V &stm )
{
	std::size_t	name_size;
	// this saves a vector as a miMATRIX
	if ( name.size() > 4 ) { // naormal name format
		name_size = name.size()/8;
		name_size *= 8;
		if ( name.size()%8 != 0 )
			name_size += 8;
	}
	else { // compressed name format
		name_size = 4;
	}

	std::size_t	total_length = get_length_of_multidimensional_vector_as_cell(name, matrix);

	bool	big_endianness = is_big_endian();

	// miMATRIX tag
	write_size_t_as_ubinary_number(4, 14, stm, big_endianness);
	write_size_t_as_ubinary_number(4, total_length, stm, big_endianness);

	// Array fags
	write_size_t_as_ubinary_number(4, 6, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 8, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 1, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 0, stm, big_endianness);

	// Dimensions array
	write_size_t_as_ubinary_number(4, 5, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 8, stm, big_endianness);
	write_size_t_as_ubinary_number(4, matrix.size(), stm, big_endianness);
	write_size_t_as_ubinary_number(4, 1, stm, big_endianness);

	// Array name
	if ( name.size() > 4 ) { // normal name format
		write_size_t_as_ubinary_number(4, 1, stm, big_endianness);
		write_size_t_as_ubinary_number(4, name.size(), stm, big_endianness);
		stm << name;
		for ( std::size_t i = 0; i < name_size-name.size(); i++ )
			write_size_t_as_ubinary_number(1, 0, stm, big_endianness);
	}
	else { // compressed name format
		std::size_t	code = name.size()*std::pow(2,16)+1;
		write_size_t_as_ubinary_number(4, code, stm, big_endianness);
		stm << name;
		for ( std::size_t i = 0; i < name_size-name.size(); i++ )
			write_size_t_as_ubinary_number(1, 0, stm, big_endianness);
	}
} // end template wirte_cell_array_preamble


// writes multidimensional numeric array preamble
template <typename V>
void	wirte_multidimensional_numeric_array_preamble( const std::string& name,
						       const std::vector<std::size_t>& size,
						       V &stm )
{
	std::size_t	name_size;
	// this saves a vector as a miMATRIX
	if ( name.size() > 4 ) { // naormal name format
		name_size = name.size()/8;
		name_size *= 8;
		if ( name.size()%8 != 0 )
			name_size += 8;
	}
	else { // compressed name format
		name_size = 4;
	}

	std::size_t	array_size = std::accumulate(size.begin(),
						     size.end(),
						     1, std::multiplies<std::size_t>());

	std::size_t	size_size = size.size()*4;
	if ( size.size()%2 != 0 )
		size_size += 4;

	std::size_t	total_length;
	if ( name.size() > 4 ) { // normal name format
		total_length = 4*8+8+name_size+size_size+array_size*8;
	}
	else { // compressed name format
		total_length = 4*8+8+size_size+array_size*8;
	}

	bool	big_endianness = is_big_endian();

	// miMATRIX tag
	write_size_t_as_ubinary_number(4, 14, stm, big_endianness);
	write_size_t_as_ubinary_number(4, total_length, stm, big_endianness);

	// Array flags
	write_size_t_as_ubinary_number(4, 6, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 8, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 6, stm, big_endianness);
	write_size_t_as_ubinary_number(4, 0, stm, big_endianness);

	// Dimensions array
	write_size_t_as_ubinary_number(4, 5, stm, big_endianness);
	write_size_t_as_ubinary_number(4, size.size()*4, stm, big_endianness);
	for (const auto& s : size)
		write_size_t_as_ubinary_number(4, s, stm, big_endianness);

	for ( std::size_t i = 0; i < size_size-(size.size()*4); i++ )
		write_size_t_as_ubinary_number(1, 0, stm, big_endianness);
	
	// Array name
	if ( name.size() > 4 ) { // normal name format
		write_size_t_as_ubinary_number(4, 1, stm, big_endianness);
		write_size_t_as_ubinary_number(4, name.size(), stm, big_endianness);
		stm << name;
		for ( std::size_t i = 0; i < name_size-name.size(); i++ )
			write_size_t_as_ubinary_number(1, 0, stm, big_endianness);
	}
	else { // compressed name format
		std::size_t	code = name.size()*std::pow(2,16)+1;
		write_size_t_as_ubinary_number(4, code, stm, big_endianness);
		stm << name;
		for ( std::size_t i = 0; i < name_size-name.size(); i++ )
			write_size_t_as_ubinary_number(1, 0, stm, big_endianness);
	}

	// miDOUBLE numbers
	write_size_t_as_ubinary_number(4, 9, stm, big_endianness);
	write_size_t_as_ubinary_number(4, array_size*8, stm, big_endianness);
} // end template wirte_multidimensional_numeric_array_preamble


// Saves the file header in Matlab (.mat) file.
template <typename T>
void	save_the_header( T &stm )
{
	// this is for the date information
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,sizeof(buffer),"%d-%m-%Y %I:%M:%S",timeinfo);
	std::string str_time(buffer);

	// writes the first 116 bytes of the header
	stm << "MATLAB 5.0 MAT-file, Platform: "
	    << getOsName()
	    << ", Created on: "
	    << str_time;

	std::size_t	stm_size = stm.tellp();
	assert(stm_size <= 116);
	if ( stm_size < 116 ) {
		for ( std::size_t i = 0; i < 116-stm_size; i++ )
			stm << " ";
	}
	stm_size = stm.tellp();
	assert(stm_size == 116);

	bool	big_endianness = is_big_endian();

	// writes the Header Subsystem Data Offset Field	
	write_size_t_as_ubinary_number(8, 0, stm, big_endianness);
	stm_size = stm.tellp();
	assert(stm_size == 124);

	// writes the Header Version	
	write_size_t_as_ubinary_number(2, 1, stm, true);
	stm_size = stm.tellp();
	assert(stm_size == 126);

	// writes the Endian Indicator
	if ( big_endianness ) {
		stm << "MI";
	}
	else {
		stm << "IM";
	}
	stm_size = stm.tellp();
	assert(stm_size == 128);
} // end template save_the_header


// Loads the file header in Matlab (.mat) file.
template <typename T>
bool	load_the_header( T &stm )
{
	std::size_t	number;
	std::string	buffer;

	// reads the first 19 of the 116 characters
	if ( !read_from_stm_to_str(buffer,stm,19) ) {
		std::cout << "\nIn function load_the_header" << std::endl;
		throw std::runtime_error ("Wrong file header");
	}
	if (buffer.compare("MATLAB 5.0 MAT-file") != 0) {
		std::cout << "\nIn function load_the_header" << std::endl;
		throw std::runtime_error ("Wrong file header");
	}

	// reads the first rest of the 116 characters
	if ( !read_from_stm_to_str(buffer,stm,97) ) {
		std::cout << "\nIn function load_the_header" << std::endl;
		throw std::runtime_error ("Wrong file header");
	}
		

	// reads the subsys data offset but it does not check it
	if ( !read_from_stm_to_str(buffer,stm,8) ) {
		std::cout << "\nIn function load_the_header" << std::endl;
		throw std::runtime_error ("Wrong file header");
	}
		

	// reads the version; this must be one
	if ( !read_size_t_as_ubinary_number(number,2,stm,true) ) {
		std::cout << "\nIn function load_the_header" << std::endl;
		throw std::runtime_error ("Wrong file header");
	}
	if (number != 1) {
		std::cout << "\nIn function load_the_header" << std::endl;
		throw std::runtime_error ("Wrong file version");
	}

	// reads the endian indicator
	if ( !read_from_stm_to_str(buffer,stm,2) ) {
		std::cout << "\nIn function load_the_header" << std::endl;
		throw std::runtime_error ("Wrong file header");
	}
		
	if (buffer.compare("MI") == 0) { 
		return	true;
	}
	else if (buffer.compare("IM") == 0) {
		return	false;
	}
	else {
		std::cout << "\nIn function load_the_header" << std::endl;
		throw std::runtime_error ("Wrong file header");
	}
} // end template load_the_header


// checks for the next data name
template <typename T>
matlabData	check_next_data_structure( T &stm, const bool big_endianness )
{
	matlabData	output;
	std::size_t	number;
	int	total_number_of_bytes;

	miMATRIX:
	// reads the miMatrix code (14 in Matlab)
	do {
		if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
			output.more_data = false;
			return	output;
		}
	}
	while (number != 14);

	output.more_data = true;
	// next 4 bytes contain the number of bytes in this data structure
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function check_next_data_structure" << std::endl;
		throw std::runtime_error ("Wrong data format (truncated data)");
	}
	total_number_of_bytes = number;
	assert(total_number_of_bytes > 0);

	// reads Array flags tag code (6 in Matlab)
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function check_next_data_structure" << std::endl;
		throw std::runtime_error ("Wrong data format (truncated data)");
	}
	if (number != 6) {
		goto	miMATRIX;
	}
	// next 4 bytes must have the number 8 
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function check_next_data_structure" << std::endl;
		throw std::runtime_error ("Wrong data format (truncated data)");
	}
	if (number != 8) {
		goto	miMATRIX;
	}

	total_number_of_bytes -= 8;
	assert(total_number_of_bytes > 0);

	// reads Array flags code 
	std::vector<std::uint8_t>	mask;
	mask.push_back(255);
	mask.push_back(0);
	mask.push_back(0);
	mask.push_back(0);
	if ( !read_size_t_as_ubinary_number(output.array_type,4,stm,big_endianness,mask) ) {
		std::cout << "\nIn function check_next_data_structure" << std::endl;
		throw std::runtime_error ("Wrong data format (truncated data)");
	}
	total_number_of_bytes -= 4;
	assert(total_number_of_bytes > 0);

	std::size_t	name_length, total_name_length, total_number_of_elements,
			number_of_dimensions;
	std::string	garbage;
	switch (output.array_type) {
		case 6: 
			// mxDOUBLE_CLASS (Double precision array)
			// next 4 bytes must be ignored 
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			total_number_of_bytes -= 4;
			assert(total_number_of_bytes > 0);

			// reads the Array dimensions
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			if (number != 5) {
				goto	miMATRIX;
			}
			// next 4 bytes have the number of dimensions in the Array 
			if ( !read_size_t_as_ubinary_number(number_of_dimensions,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			total_number_of_bytes -= 8;
			assert(total_number_of_bytes > 0);

			// number of dimensions has to be multiple of 4
			if (number_of_dimensions%4 != 0) {
				goto	miMATRIX;
			}
			number_of_dimensions /= 4;

			// reads the dimensions
			for ( std::size_t dimension = 0; dimension < number_of_dimensions; dimension++ ) {
				std::size_t	auxiliary;
				if ( !read_size_t_as_ubinary_number(auxiliary,4,stm,big_endianness) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				total_number_of_bytes -= 4;
				assert(total_number_of_bytes > 0);

				output.dimensions.push_back(auxiliary);
			}

			if ( number_of_dimensions%2 != 0 ) { // if number_of_dimensions is not even
				// dismiss the padding
				std::size_t	auxiliary;
				if ( !read_size_t_as_ubinary_number(auxiliary,4,stm,big_endianness) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				total_number_of_bytes -= 4;
				assert(total_number_of_bytes > 0);
			}

			// reads the Array name tag
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}

			// if number is equal to 1, then the name of the array
			// is in normal form (not compressed)
			if (number == 1) {
				// next 4 bytes have the number of bytes in the name of the Array 
				if ( !read_size_t_as_ubinary_number(name_length,4,stm,big_endianness) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				total_number_of_bytes -= 8;
				assert(total_number_of_bytes > 0);

				total_name_length = name_length/8;
				total_name_length *= 8;
				if ( name_length%8 != 0 )
					total_name_length += 8;

				// reads the name of the Array
				if ( !read_from_stm_to_str(output.name,stm,name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				// pops the padding of the name
				if ( !read_from_stm_to_str(garbage,stm,total_name_length-name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}

				total_number_of_bytes -= total_name_length;
				assert(total_number_of_bytes > 0);

				total_number_of_elements = std::accumulate(output.dimensions.begin(),
									   output.dimensions.end(),
									   1, std::multiplies<std::size_t>());

				// checks if total_number_of_bytes is correct
				if (total_number_of_bytes-8-8*total_number_of_elements != 0) {
					goto	miMATRIX;
				}
			}
			// if number is not equal to 1, then the name of the array
			// is in compressed form
			else {
				// in this case name_length = number - 256
				name_length = (number-1)/(std::pow(2,16));

				total_number_of_bytes -= 4;
				assert(total_number_of_bytes > 0);

				total_name_length = 4;

				// reads the name of the Array
				if ( !read_from_stm_to_str(output.name,stm,name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				// pops the padding of the name
				if ( !read_from_stm_to_str(garbage,stm,total_name_length-name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}

				total_number_of_bytes -= total_name_length;
				assert(total_number_of_bytes > 0);

				total_number_of_elements = std::accumulate(output.dimensions.begin(),
									   output.dimensions.end(),
									   1, std::multiplies<std::size_t>());

				// checks if total_number_of_bytes is correct
				if (total_number_of_bytes-8-8*total_number_of_elements != 0) {
					goto	miMATRIX;
				}
			}
		break;
		case 5: 
			// mxSPARSE_CLASS (Sparse array)	
			// next 4 bytes may contain th number of non-zero elements in the sparse array 
			if ( !read_size_t_as_ubinary_number(output.nnz,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			total_number_of_bytes -= 4;
			assert(total_number_of_bytes > 0);

			// reads the Array dimensions
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			if (number != 5) {
				goto	miMATRIX;
			}
			// next 4 bytes have the number of dimensions in the Array 
			if ( !read_size_t_as_ubinary_number(number_of_dimensions,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			if (number_of_dimensions != 8) {
				goto	miMATRIX;
			}

			total_number_of_bytes -= 8;
			assert(total_number_of_bytes > 0);

			number_of_dimensions /= 4;
			// reads the dimensions
			for ( std::size_t dimension = 0; dimension < number_of_dimensions; dimension++ ) {
				std::size_t	auxiliary;
				if ( !read_size_t_as_ubinary_number(auxiliary,4,stm,big_endianness) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				total_number_of_bytes -= 4;
				assert(total_number_of_bytes > 0);

				output.dimensions.push_back(auxiliary);
			}

			// reads the Array name tag
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			total_number_of_bytes -= 4;
			assert(total_number_of_bytes > 0);

			// if number is equal to 1, then the name of the array
			// is in normal form (not compressed)
			if (number == 1) {
				// next 4 bytes have the number of bytes in the name of the Array 
				if ( !read_size_t_as_ubinary_number(name_length,4,stm,big_endianness) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				total_number_of_bytes -= 4;
				assert(total_number_of_bytes > 0);

				total_name_length = name_length/8;
				total_name_length *= 8;
				if ( name_length%8 != 0 )
					total_name_length += 8;

				// reads the name of the Array
				if ( !read_from_stm_to_str(output.name,stm,name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				// pops the padding of the name
				if ( !read_from_stm_to_str(garbage,stm,total_name_length-name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}

				total_number_of_bytes -= total_name_length;
				assert(total_number_of_bytes > 0);

				total_number_of_elements = output.nnz;

			}
			// if number is not equal to 1, then the name of the array
			// is in compressed form
			else {
				// in this case name_length = number - 256
				name_length = (number-1)/(std::pow(2,16));

				total_name_length = 4;

				// reads the name of the Array
				if ( !read_from_stm_to_str(output.name,stm,name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				// pops the padding of the name
				if ( !read_from_stm_to_str(garbage,stm,total_name_length-name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}

				total_number_of_bytes -= total_name_length;
				assert(total_number_of_bytes > 0);

				total_number_of_elements = output.nnz;

			}


			// reads the row indices (ir) tag
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			if (number != 5) {
				goto	miMATRIX;
			}
	
			// reads the individual row indices of the non-zero elements
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			if (number%4 != 0) {
				goto	miMATRIX;
			}
			
			total_number_of_bytes -= 8;
			assert(total_number_of_bytes > 0);

			number /= 4;
			output.row_indices.resize(0);
			for ( std::size_t row = 0; row < number; row++ ) {
				std::size_t	auxiliary;
				if ( !read_size_t_as_ubinary_number(auxiliary,4,stm,big_endianness) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				total_number_of_bytes -= 4;
				assert(total_number_of_bytes > 0);

				output.row_indices.push_back(auxiliary);
			}

			// if the number of row indices is not even
			// then do padding in the last 4 bytes
			if (number%2 != 0) {
				if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				total_number_of_bytes -= 4;
				assert(total_number_of_bytes > 0);

			}

			// reads the column indices (jc) tag
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			if (number != 5) {
				goto	miMATRIX;
			}
	
			// reads the individual column indices of the non-zero elements
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			total_number_of_bytes -= 8;
			assert(total_number_of_bytes > 0);

			if (number%4 != 0 || number != (1+output.dimensions[1])*4 ||
			    output.dimensions.size() != 2 ) {
				goto	miMATRIX;
			}

			output.column_indices.resize(0);
			for ( std::size_t column = 0; column < (1+output.dimensions[1]); column++ ) {
				std::size_t	auxiliary;
				if ( !read_size_t_as_ubinary_number(auxiliary,4,stm,big_endianness) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				total_number_of_bytes -= 4;
				assert(total_number_of_bytes > 0);

				output.column_indices.push_back(auxiliary);
			}

			number /= 4;
			// if the number of column indices is not even
			// then do padding in the last 4 bytes
			if (number%2 != 0) {
				if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				total_number_of_bytes -= 4;
				assert(total_number_of_bytes > 0);
			}

 			// checks if total_number_of_bytes is correct
			if(total_number_of_elements == 1) {
				if (total_number_of_bytes-8-8*1 != 0 && 
				    total_number_of_bytes-8-8*0 != 0) {
					goto	miMATRIX;
				}
				if (total_number_of_bytes-8-8*0 == 0)
					output.nnz = 0;
			}
			else {
				if (total_number_of_bytes-8-8*total_number_of_elements != 0) {
					goto	miMATRIX;
				}
			}
		break;
		case 1: 
			// mxCELL_CLASS (Cell array)
			// next 4 bytes contain no information and can be descerted
			// for the cell array case 
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			total_number_of_bytes -= 4;
			assert(total_number_of_bytes > 0);

			// reads the Array dimensions
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			if (number != 5) {
				goto	miMATRIX;
			}

			// next 4 bytes have the number of dimensions in the Array times 4 (2*4) 
			if ( !read_size_t_as_ubinary_number(number_of_dimensions,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}
			if (number_of_dimensions != 8) {
				goto	miMATRIX;
			}
			total_number_of_bytes -= 8;
			assert(total_number_of_bytes > 0);

			number_of_dimensions /= 4;
			// reads the dimensions
			for ( std::size_t dimension = 0; dimension < number_of_dimensions; dimension++ ) {
				std::size_t	auxiliary;
				if ( !read_size_t_as_ubinary_number(auxiliary,4,stm,big_endianness) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				total_number_of_bytes -= 4;
				assert(total_number_of_bytes > 0);

				output.dimensions.push_back(auxiliary);
			}


			// reads the Array name tag
			if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
				std::cout << "\nIn function check_next_data_structure" << std::endl;
				throw std::runtime_error ("Wrong data format (truncated data)");
			}

			// if number is equal to 1, then the name of the array
			// is in normal form (not compressed)
			if (number == 1) {
				// next 4 bytes have the number of bytes in the name of the Array 
				if ( !read_size_t_as_ubinary_number(name_length,4,stm,big_endianness) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				total_number_of_bytes -= 8;
				assert(total_number_of_bytes > 0);

				total_name_length = name_length/8;
				total_name_length *= 8;
				if ( name_length%8 != 0 )
					total_name_length += 8;

				// reads the name of the Array
				if ( !read_from_stm_to_str(output.name,stm,name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				// pops the padding of the name
				if ( !read_from_stm_to_str(garbage,stm,total_name_length-name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}

				total_number_of_bytes -= total_name_length;
				assert(total_number_of_bytes > 0);

				total_number_of_elements = std::accumulate(output.dimensions.begin(),
									   output.dimensions.end(),
									   1, std::multiplies<std::size_t>());
			}
			// if number is not equal to 1, then the name of the array
			// is in compressed form
			else {
				// in this case name_length = number - 256
				name_length = (number-1)/(std::pow(2,16));

				total_number_of_bytes -= 4;
				assert(total_number_of_bytes > 0);

				total_name_length = 4;

				// reads the name of the Array
				if ( !read_from_stm_to_str(output.name,stm,name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}
				// pops the padding of the name
				if ( !read_from_stm_to_str(garbage,stm,total_name_length-name_length) ) {
					std::cout << "\nIn function check_next_data_structure" << std::endl;
					throw std::runtime_error ("Wrong data format (truncated data)");
				}

				total_number_of_bytes -= total_name_length;
				assert(total_number_of_bytes > 0);

				total_number_of_elements = std::accumulate(output.dimensions.begin(),
									   output.dimensions.end(),
									   1, std::multiplies<std::size_t>());
			}
		break;
		default: 
			goto	miMATRIX;
		break;
	}

	return	output;	
} // end template check_next_data_structure


// Saves scalar in binary Matlab (.mat) file.
template <typename T, typename V>
void	save_scalar_as_numeric_array( const std::string& name, const T scalar, V &stm )
{
	// this saves a scalar as a miMATRIX
	std::array<std::size_t,2>	size = {1,1};
	wirte_numeric_array_preamble(name, size, stm);

	double	auxiliary = (double)scalar;
	stm.write(reinterpret_cast<const char*>(&auxiliary), sizeof(auxiliary));
} // end template save_scalar_as_numeric_array


// Loads numeric array into scalar from binary Matlab (.mat) file.
template <typename T, typename V>
void	load_numeric_array_to_scalar( const matlabData& array_info, T &scalar, V &stm,
				      const bool big_endianness )
{
	std::size_t	number;

	// reads the miDOUBLE code (9 in Matlab)
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function load_numeric_array_to_scalar" << std::endl;
		throw std::runtime_error ("Wrong Tag format (truncated data)");
	}
	if (number != 9) {
		std::cout << "\nIn function load_numeric_array_to_scalar" << std::endl;
		throw std::runtime_error ("Wrong data format (only miDOUBLE is allowed");
	}
	
	// the next 4 bytes must have the number eight
	// indicating that just one element is expected
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function load_numeric_array_to_scalar" << std::endl;
		throw std::runtime_error ("Wrong Tag format (truncated data)");
	}
	if (number != 8) {
		std::cout << "\nIn function load_numeric_array_to_scalar" << std::endl;
		throw std::runtime_error ("Wrong data format (only 8 is allowed)");
	}
	
	// loads the scalar
	if ( !read_from_stm_to_number(scalar,stm) ) {
		std::cout << "\nIn function load_numeric_array_to_scalar" << std::endl;
		throw std::runtime_error ("Could not load the scalar");
	}
} // end template load_numeric_array_to_scalar


// Save vector of elements in binary Matlab (.mat) file.
template <typename T, typename V>
void	save_vector_as_numeric_array( const std::string& name, const std::vector<T>& array, V &stm )
{
	// this saves a vector as a miMATRIX
	std::array<std::size_t,2>	size = {1,array.size()};
	wirte_numeric_array_preamble(name, size, stm);

	for ( std::size_t element = 0; element < array.size(); element++ ) {
		double	auxiliary = (double)array[element];
		stm.write(reinterpret_cast<const char*>(&auxiliary), sizeof(auxiliary));
	}
} // end template save_vector_as_numeric_array


// Loads numeric array into vector from binary Matlab (.mat) file.
template <typename T, typename V>
void	load_numeric_array_to_vector( const matlabData& array_info,
				      std::vector<T> &vector, V &stm,
				      const bool big_endianness )
{
	std::size_t	number;

	// reads the miDOUBLE code (9 in Matlab)
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function load_numeric_array_to_vector" << std::endl;
		throw std::runtime_error ("Wrong Tag format (truncated data)");
	}
	if (number != 9) {
		std::cout << "\nIn function load_numeric_array_to_vector" << std::endl;
		throw std::runtime_error ("Wrong data format (only miDOUBLE is allowed");
	}
	
	// next 4 bytes must contain a number that is equal
	// to the number of elements according to the dimensions
	// in array info struct
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function load_numeric_array_to_vector" << std::endl;
		throw std::runtime_error ("Wrong Tag format (truncated data)");
	}

	if ( array_info.dimensions.size() != 2 ||
	     array_info.dimensions[0] != 1 ) {
		std::cout << "\nIn function load_numeric_array_to_vector" << std::endl;
		throw std::runtime_error ("dimensions in array info does not correspond to a vector");
	}

	if (number != array_info.dimensions[1]*8) {
		std::cout << "\nIn function load_numeric_array_to_vector" << std::endl;
		throw std::runtime_error ("Wrong data format (number == array_info.dimensions[1]*8) not satisfied");
	}
	
	// loads the vector
	vector.clear();
	vector.resize(array_info.dimensions[1]);
	for (auto& v : vector) {
		if ( !read_from_stm_to_number(v,stm) ) {
			std::cout << "\nIn function load_numeric_array_to_vector" << std::endl;
			throw std::runtime_error ("Could not load the vector");
		}
	}
} // end template load_numeric_array_to_vector


// Save vector of vectors of elements in binary Matlab (.mat) file.
template <typename T, typename V>
void	save_vector_of_vectors_as_numeric_array( const std::string& name,
						 const std::vector<std::vector<T>>& array,
						 V &stm )
{
	std::vector<std::size_t>	dimensions = get_dimensionality(array);
	assert(dimensions.size() == 2);
	// this saves a vector as a miMATRIX
	std::array<std::size_t,2>	size;
	size[0]=dimensions[0];
	size[1]=dimensions[1];

	wirte_numeric_array_preamble(name, size, stm);

	for ( std::size_t column = 0; column < size[1]; column++ ) {
		for ( std::size_t row = 0; row < size[0]; row++ ) {
			double	auxiliary = (double)array[row][column];
			stm.write(reinterpret_cast<const char*>(&auxiliary), sizeof(auxiliary));
		}
	}
} // end template save_vector_of_vectors_as_numeric_array


// Loads numeric array into vector of vectors from binary Matlab (.mat) file.
template <typename T, typename V>
void	load_numeric_array_to_vector_of_vectors( const matlabData& array_info,
						 std::vector<std::vector<T>> &vector, V &stm,
						 const bool big_endianness )
{
	std::size_t	number;

	// reads the miDOUBLE code (9 in Matlab)
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function load_numeric_array_to_vector_of_vectors" << std::endl;
		throw std::runtime_error ("Wrong Tag format (truncated data)");
	}
	if (number != 9) {
		std::cout << "\nIn function load_numeric_array_to_vector_of_vectors" << std::endl;
		throw std::runtime_error ("Wrong data format (only miDOUBLE is allowed");
	}
	
	// next 4 bytes must contain a number that is equal
	// to the number of elements according to the dimensions
	// in array info struct
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function load_numeric_array_to_vector_of_vectors" << std::endl;
		throw std::runtime_error ("Wrong Tag format (truncated data)");
	}

	if ( array_info.dimensions.size() != 2 ) {
		std::cout << "\nIn function load_numeric_array_to_vector_of_vectors" << std::endl;
		throw std::runtime_error ("dimensions in array info does not correspond to a vector of vectors");
	}

	std::size_t	number_of_elements = std::accumulate(array_info.dimensions.begin(),
							     array_info.dimensions.end(),
							     1, std::multiplies<std::size_t>());

	if (number != number_of_elements*8) {
		std::cout << "\nIn function load_numeric_array_to_vector_of_vectors" << std::endl;
		throw std::runtime_error ("Wrong data format (number != number_of_elements*8) not satisfied");
	}
	
	// loads the vector
	vector.clear();
	vector.resize(array_info.dimensions[0]);
	for (auto& v : vector)
		v.resize(array_info.dimensions[1]);

	for ( std::size_t column = 0; column < array_info.dimensions[1]; column++ ) {
		for ( std::size_t row = 0; row < array_info.dimensions[0]; row++ ) {
			if ( !read_from_stm_to_number(vector[row][column],stm) ) {
				std::cout << "\nIn function load_numeric_array_to_vector_of_vectors" << std::endl;
				throw std::runtime_error ("Could not load the vector");
			}
		}
	}
} // end template load_numeric_array_to_vector_of_vectors


// Save vector of vectors conditionally as a sparse array type in Matlab (.mat) file.
// First, measures the array sparsity, if this is greather than sparsityThreshold,
// array is saved as a sparse array in Matlab (.mat) format.
// In the other case, array is saved as a normal array in Matlab (.mat) format.
template <typename T, typename V>
void	save_vector_of_vectors_conditionally_as_sparse_array( const std::string& name, const std::vector<std::vector<T>>& array,
							      const double sparsityThreshold, V &stm )
{
	assert(is_rectangular(array));
	assert(get_rectangular_indexes(array).size()==2);
	if(get_rectangular_sparsity(array) > sparsityThreshold) {
		auto	sparseMatrix = to_sparse(array);
		save_sparse_matrix_elements_as_sparse_array(name,sparseMatrix,stm);

	}
	else {
		save_vector_of_vectors_as_numeric_array(name,array,stm);
	}
} // end template save_vector_of_vectors_conditionally_as_sparse_array


// Loads sparse matrix of elements conditionally from sparse array in binary Matlab (.mat) file.
template <typename T, typename V>
void	load_conditionally_sparse_array_to_vector_of_vectors( const matlabData& array_info,
							     std::vector<std::vector<T>>& array, V &stm,
							     const bool big_endianness )
{
	if ( array_info.array_type == 5 ) {
		SparseMatrixElements<T>	s_matrix;
		load_sparse_matrix_elements_as_sparse_array(array_info, s_matrix, stm, big_endianness);
		array = from_sparse(s_matrix);
	}
	else if ( array_info.array_type == 6 ) {
		load_numeric_array_to_vector_of_vectors(array_info, array, stm, big_endianness);
	}
	else {
		std::cout << "\nIn function load_conditionaly_sparse_array_to_vector_of_vectors" << std::endl;
		throw std::runtime_error ("Wrong Tag format (incorrect array_type)");
	}
} // end template load_conditionaly_sparse_array_to_vector_of_vectors


// Saves sparse matrix of elements in binary Matlab (.mat) file.
template <typename T, typename V>
void	save_sparse_matrix_elements_as_sparse_array( const std::string& name,
						     const SparseMatrixElements<T>& matrix,
						     V &stm )
{
	assert(matrix.rows.size() == matrix.values.size());
	assert(matrix.columns.size() == matrix.values.size());

	std::array<std::size_t,2>	size;
	size[0] = matrix.numberOfRows;
	size[1] = matrix.numberOfColumns;
	wirte_sparse_array_preamble(name, size, stm, matrix.numberOfNonZero,
						     matrix.rows,
						     matrix.columns);

	for ( std::size_t element = 0; element < matrix.numberOfNonZero; element++ ) {
		double	auxiliary = (double)matrix.values[element];
		stm.write(reinterpret_cast<const char*>(&auxiliary), sizeof(auxiliary));
	}
} // end template save_sparse_matrix_elements_as_sparse_array


// Loads sparse matrix of elements from sparse array in binary Matlab (.mat) file.
template <typename T, typename V>
void	load_sparse_matrix_elements_as_sparse_array( const matlabData& array_info,
						     SparseMatrixElements<T>& matrix, V &stm,
						     const bool big_endianness )
{
	std::size_t	number;

	// reads the miDOUBLE code (9 in Matlab)
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function load_sparse_matrix_elements_as_sparse_array" << std::endl;
		throw std::runtime_error ("Wrong Tag format (truncated data)");
	}
	if (number != 9) {
		std::cout << "\nIn function load_sparse_matrix_elements_as_sparse_array" << std::endl;
		throw std::runtime_error ("Wrong data format (only miDOUBLE is allowed");
	}
	
	// next 4 bytes must contain a number that is equal
	// to the number of non-zero elements according to the dimensions
	// in array info struct
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function load_sparse_matrix_elements_as_sparse_array" << std::endl;
		throw std::runtime_error ("Wrong Tag format (truncated data)");
	}

	if ( array_info.dimensions.size() != 2 ) {
		std::cout << "\nIn function load_sparse_matrix_elements_as_sparse_array" << std::endl;
		throw std::runtime_error ("dimensions in array info does not correspond to a vector of vectors");
	}

	if (number != 8*array_info.nnz) {
		std::cout << "\nIn function load_sparse_matrix_elements_as_sparse_array" << std::endl;
		throw std::runtime_error ("Wrong data format (number == array_info.nnz) not satisfied");
	}
	
	auto	auxiliary = array_info.column_indices;
	if (auxiliary.size() > 0) {
		for ( std::size_t i = auxiliary.size()-1; i > 0; i-- ) {
			auxiliary[i] -= auxiliary[i-1];
		}
	}

	std::vector<std::size_t>	column_indices;
	for ( std::size_t i = 0; i < auxiliary.size(); i++ ) {
		while ( auxiliary[i] > 0 ) {
			column_indices.push_back(i-1);
			auxiliary[i]--;
		}
	}
	assert(column_indices.size() == array_info.row_indices.size());

	matrix.clear();
	matrix.numberOfNonZero = array_info.nnz;
	matrix.numberOfRows = array_info.dimensions[0];
	matrix.numberOfColumns = array_info.dimensions[1];
	assert(array_info.dimensions.size());
	T	value;
	for ( std::size_t member = 0; member < matrix.numberOfNonZero; member++ )
	{
		// The vector of vectors was introduced in the file in the following way:
		// stm << row+1 << ' ' << column+1 << ' ' << matrix[rowIndex].value << endl;
		if ( !read_from_stm_to_number(value,stm) ) {
			std::cout << "\nIn function load_sparse_matrix_elements_as_sparse_array" << std::endl;
			throw std::runtime_error ("Could not load sparse array member");
		}
		matrix.rows.push_back(array_info.row_indices[member]);
		matrix.columns.push_back(column_indices[member]);
		matrix.values.push_back(value);
	}
	matrix.rows.shrink_to_fit();
	matrix.columns.shrink_to_fit();
	matrix.values.shrink_to_fit();
} // end template load_sparse_matrix_elements_as_sparse_array


// This is a recursive system of overloaded templates which
// saves a multidimensional vector as cell array type in Matlab (.mat) file.
// multidimensional makes reference to more than one dimension, i. e.
// the data to save is, at least, a vector of vectors.
template <typename T, typename V>
void	save_multidimensional_vector_as_cell_array( const std::string& name,
						    const std::vector<T>& vector, V &stm )
{
	save_vector_as_numeric_array(name, vector, stm);
}

template <typename T, typename V>
void	save_multidimensional_vector_as_cell_array( const std::string& name,
						    const std::vector<std::vector<T>>& matrix, V &stm )
{
	wirte_cell_array_preamble(name, matrix, stm);

	std::size_t	rows = matrix.size();
	for (std::size_t row = 0; row < rows; row++)
		save_multidimensional_vector_as_cell_array("<cell-element>",matrix[row],stm);
} // end template save_multidimensional_vector_as_cell


// This is a recursive system of overloaded templates which
// load a cell type in Matlab (.mat) file to a multidimensional vector.
// multidimensional makes reference to more than one dimension, i. e.
// the data to save is, at least, a vector of vectors.
// matrix passed to the template must have the correct dimensionality in advance,
// if not, the template will return with an error
template <typename T, typename V>
void	load_cell_array_to_multidimensional_vector( const matlabData& array_info,
						    std::vector<T>& vector, V &stm,
						    const bool big_endianness )
{
	load_numeric_array_to_vector(array_info, vector, stm, big_endianness);
}

template <typename T, typename V>
void	load_cell_array_to_multidimensional_vector( const matlabData& array_info,
						    std::vector<std::vector<T>>& matrix, V &stm,
						    const bool big_endianness )
{
	if ( array_info.dimensions.size() != 2 ) {
		std::cout << "\nIn function load_cell_array_to_multidimensional_vector" << std::endl;
		throw std::runtime_error ("array_info.dimensions argument does not meet the required format");
	}

	if ( array_info.array_type == 1 &&
	      array_info.dimensions[1] != 1 ) {
		std::cout << "\nIn function load_cell_array_to_multidimensional_vector" << std::endl;
		throw std::runtime_error ("array_info.dimensions argument does not meet the required format");
	}

	if ( array_info.array_type == 6 &&
	      array_info.dimensions[0] != 1 ) {
		std::cout << "\nIn function load_cell_array_to_multidimensional_vector" << std::endl;
		throw std::runtime_error ("array_info.dimensions argument does not meet the required format");
	}


	std::size_t	row = 0;
	matrix.resize(array_info.dimensions[0]);
	while ( array_info.more_data && row < array_info.dimensions[0] ) {
		auto	array_structure = check_next_data_structure(stm,big_endianness);
		load_cell_array_to_multidimensional_vector(array_structure, matrix[row], stm, big_endianness);
		row++;
	}

} // end template load_cell_array_to_multidimensional_vector


// Saves multidimensional vector of elements in binary Matlab (.mat) file.
template <typename T, typename V, typename U>
void	save_multidimensional_vector_as_numeric_array( const std::string& name,
						       const std::vector<T>& array,
						       const V&, U &stm )
{
	// this saves a vector as a miMATRIX
	std::vector<std::size_t>	size = get_dimensionality(array);
	std::reverse(size.begin(),size.end());
	wirte_multidimensional_numeric_array_preamble(name, size, stm);

	std::vector<V>    v;
	to_one_dimentional_vector(array,v);

	for ( std::size_t element = 0; element < v.size(); element++ ) {
		double	auxiliary = (double)v[element];
		stm.write(reinterpret_cast<const char*>(&auxiliary), sizeof(auxiliary));
	}
} // end template save_multidimensional_vector_as_numeric_array


// Load array to multidimensional vector from Matlab (.mat) file.
// multidimensional makes reference to more than two dimensions, i. e.
// the structure of the argument "array" in which the data is load must
// be determined and allocated in advance (before calling the template).
template <typename T, typename V, typename U>
void	load_array_to_multidimensional_vector( const matlabData& array_info,
					       std::vector<T>& matrix, const V&,
					       U &stm,
					       const bool big_endianness )
{
	std::size_t	number;
	// reads the miDOUBLE code (9 in Matlab)
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function load_array_to_multidimensional_vector" << std::endl;
		throw std::runtime_error ("Wrong Tag format (truncated data)");
	}
	if (number != 9) {
		std::cout << "\nIn function load_array_to_multidimensional_vector" << std::endl;
		throw std::runtime_error ("Wrong data format (only miDOUBLE is allowed");
	}
	
	// next 4 bytes must contain a number that is equal
	// to the number of elements according to the dimensions
	// in array info struct
	if ( !read_size_t_as_ubinary_number(number,4,stm,big_endianness) ) {
		std::cout << "\nIn function load_array_to_multidimensional_vector" << std::endl;
		throw std::runtime_error ("Wrong Tag format (truncated data)");
	}

	std::size_t	length;
	std::vector<V>	aux;

	length = std::accumulate(array_info.dimensions.begin(), array_info.dimensions.end(), 1, std::multiplies<std::size_t>());

	if (number != length*8) {
		std::cout << "\nIn function load_array_to_multidimensional_vector" << std::endl;
		throw std::runtime_error ("Wrong data format (number != length*8) not satisfied");
	}
	
	aux.resize(length);
	for (auto& v : aux) {
		if ( !read_from_stm_to_number(v,stm) ) {
			std::cout << "\nIn function load_array_to_multidimensional_vector" << std::endl;
			throw std::runtime_error ("Could not load array member");
		}
	}

	matrix.clear();
	auto	dimensions = array_info.dimensions;
	std::reverse(dimensions.begin(), dimensions.end());
	resize_multi_dimentional_vector(matrix,dimensions);
	to_multi_dimentional_vector(matrix, aux);
} // end template load_array_to_multidimensional_vector


#endif
