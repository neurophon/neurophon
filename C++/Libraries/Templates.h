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

// File Name:		Templates.h
// File Description:	Templates definitions. This file contains Templates implementations.
//

#ifndef TEMPLATES_H
#define TEMPLATES_H


#include <iostream>


#include <algorithm>
#include <valarray>
#include <cmath>
#include <numeric>
#include <cassert>

#include "DataTypes.h"

// Fisher-Yates shuffle that stops after num_random iterations.
// This is significantly faster than std::random_shuffle when you only need
// a few random numbers out of the set, and should be just about the same
// speed even if num_random==vector.size().
template<typename bidiiter>
bidiiter random_unique(bidiiter begin, bidiiter end, size_t num_random)
{
	size_t left = std::distance(begin, end);
	while (num_random--)
	{
		bidiiter r = begin;
		std::advance(r, rand()%left);
		std::swap(*begin, *r);
		++begin;
		--left;
	}
	return begin;
} // end template random_unique


// Sort vector indexes based on vector values (ascending orfder)
template <typename T>
std::vector<int> sort_indexes(const std::vector<T> &v) {

	// initialize original index locations
	std::vector<int> idx(v.size());
	iota(idx.begin(), idx.end(), 0);

	// sort indexes based on comparing values in v
	sort(idx.begin(), idx.end(),
		[&v](int i1, int i2) {return v[i1] < v[i2];});

	return idx;
} // end template sort_indexes


// Extracts vector elements based on vector indexes
template <typename T>
std::vector<T> get_elements(const std::vector<T> &v, const std::vector<int> &indexes) {

	std::vector<T> auxiliary;

	std::transform (indexes.begin(), indexes.end(), std::back_inserter(auxiliary),//
	[&v](int element) {return v.at(element);});
	auxiliary.shrink_to_fit();

	return auxiliary;
} // end template get_elements


// Sets vector elements with val value based on vector indexes
template <typename T, typename V>
void    set_elements(std::vector<T> &v, const std::vector<int> &indexes, const V &val) {

	std::for_each (indexes.begin(), indexes.end(),//
	[&v, &val](int element) {v.at(element)=val;});

} // end template set_elements


// Does the conversion from std::vector to std::valarray<std::size_t>
template < typename T >
std::valarray<std::size_t> vector2valarray( const std::vector<T>& vec )
{
	static_assert( std::is_integral<T>::value, "expected an integral type" );

	std::valarray<std::size_t> result( vec.size() ) ;
	for( std::size_t i = 0 ; i < vec.size() ; ++i ) result[i] = vec[i] ;
	return result ;
}


// Subtract one vector to another; operator-= overload for std::vector.
// Example, if
// A1 = {1,2,3}; and
// A2 = {4,5,6}; then
// A1-=A2; and
// A1 = {-3,-3,-3}; and
// A2 = {4,5,6};
template <typename T>
std::vector<T> operator-=(std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    std::transform(a.begin(), a.end(), b.begin(), 
                   std::back_inserter(a), std::minus<T>());
    a.erase(a.begin(),a.begin()+a.size()/2);
    a.shrink_to_fit();
    return a;
}


// Add one vector to another; operator+= overload for std::vector.
// Example, if
// A1 = {1,2,3}; and
// A2 = {4,5,6}; then
// A1+=A2; and
// A1 = {5,7,9}; and
// A2 = {4,5,6};
template <typename T>
std::vector<T> operator+=(std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    std::transform(a.begin(), a.end(), b.begin(), 
                   std::back_inserter(a), std::plus<T>());
    a.erase(a.begin(),a.begin()+a.size()/2);
    a.shrink_to_fit();
    return a;
}

// Add two vectors and put the result in another vector; operator+ overload for std::vector.
// Example, if
// A1 = {1,2,3};
// A2 = {4,5,6}; and
// B; then
// B = A1+A2; and
// A1 = {1,2,3};
// A2 = {4,5,6}; and
// B = {5,7,9};
template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    std::vector<T> result;
    result.reserve(a.size());

    std::transform(a.begin(), a.end(), b.begin(), 
                   std::back_inserter(result), std::plus<T>());
    return result;
}


// Subtract two vectors and put the result in another vector; operator- overload for std::vector.
// Example, if
// A1 = {1,2,3};
// A2 = {4,5,6}; and
// B; then
// B = A1-A2; and
// A1 = {1,2,3};
// A2 = {4,5,6}; and
// B = {-3,-3,-3};
template <typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    std::vector<T> result;
    result.reserve(a.size());

    std::transform(a.begin(), a.end(), b.begin(), 
                   std::back_inserter(result), std::minus<T>());
    return result;
}


// Computes the distance between two std::vectors
template <typename T>
double	vectors_distance(const std::vector<T>& a, const std::vector<T>& b)
{
	assert(a.size() == b.size());

	std::vector<T>	auxiliary;

	std::transform (a.begin(), a.end(), b.begin(), std::back_inserter(auxiliary),//
	[](T element1, T element2) {return std::pow((element1-element2),2);});
	//auxiliary.shrink_to_fit();

	return  std::sqrt(std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0));
} // end template vectors_distance


// Finds the index positions of elements that verify less than condition.
template <typename T, typename V>
std::vector<int>	less_than_indexes(const std::vector<T>& v, const V& limit)
{
	std::vector<int> results;

	auto it = std::find_if(std::begin(v), std::end(v), [&limit](T i){return i < limit;});
	while (it != std::end(v)) {
	   results.emplace_back(std::distance(std::begin(v), it));
	   it = std::find_if(std::next(it), std::end(v), [&limit](T i){return i < limit;});
	}

	return	results;
} // end template less_than_indexes


// Finds the index positions of elements that verify less than or equal to condition.
template <typename T, typename V>
std::vector<int>	less_than_or_equal_to_indexes(const std::vector<T>& v, const V& limit)
{
	std::vector<int> results;

	auto it = std::find_if(std::begin(v), std::end(v), [&limit](T i){return i <= limit;});
	while (it != std::end(v)) {
	   results.emplace_back(std::distance(std::begin(v), it));
	   it = std::find_if(std::next(it), std::end(v), [&limit](T i){return i <= limit;});
	}

	return	results;
} // end template less_than_or_equal_to_indexes


// gets vector indexes corresponding to vector value
template <typename T>
std::vector<int>	get_indexes_from_value(const std::vector<T>& v, const T& value)
{
	std::vector<int>    auxiliary;
	auto it = find(v.begin(),v.end(),value);
	while (it != std::end(v)) {
	    auto index = std::distance(v.begin(), it);
	    assert(index < (int)v.size());
	    auxiliary.push_back(index);
	    it = find(it+1,v.end(),value);
	}
	auxiliary.shrink_to_fit();
	return	auxiliary;
} // end template get_indexes_from_value


// i owe you the comment ;)
template<typename T>
std::vector<int>    coincidence_indexes(std::vector<T> v1, std::vector<T> v2)
{
    std::vector<int>    output;
    
    auto    it = find_first_of (v1.begin(), v1.end(), v2.begin(), v2.end());
	while (it != std::end(v1)) {
	    auto index = std::distance(v1.begin(), it);
	    assert(index < (int)v1.size());
	    output.push_back(index);
	    it = find_first_of (it+1, v1.end(), v2.begin(), v2.end());
	}

    output.shrink_to_fit();
    return  output;
} // end template coincidence_indexes


// gets the minimum element from vector 
template <typename T>
T	get_minimum_element(const std::vector<T>& v)
{
	auto	result = std::min_element(std::begin(v), std::end(v));
	return	v[std::distance(std::begin(v), result)];
} // end template get_minimum_element


// Converts from SparseMatrixElements to a dense vector of vectors
template <typename T>
std::vector<std::vector<T>>	from_sparse(const SparseMatrixElements<T>& arr)
{
	std::vector<std::vector<T>>	output;

	output.resize(arr.numberOfRows);
	for ( int row = 0; row < arr.numberOfRows; row++ )
		output[row].resize(arr.numberOfColumns);

	for ( int element = 0; element < arr.numberOfNonZero; element++ )
		output[arr.rows[element]][arr.columns[element]] = arr.values[element];

	return	output;
} // end template from_sparse


// Converts from SparseMatrixElements to a dense vector of vectors
template <typename V>
SparseMatrixElements<V>	to_sparse(const std::vector<std::vector<V>>& v)
{
	SparseMatrixElements<V>	output;

	int	rows = v.size();
	int	columns = v[0].size();
	for ( int column = 0; column < columns; column++ ) {
		for ( int row = 0; row < rows; row++ ) {
			if ( v[row][column] != 0.0 ) {
				output.rows.push_back(row);
				output.columns.push_back(column);
				output.values.push_back(v[row][column]);
			}
		}
	}

	output.rows.shrink_to_fit();
	output.columns.shrink_to_fit();
	output.values.shrink_to_fit();

	output.numberOfNonZero = output.rows.size();
	output.numberOfRows = rows;
	output.numberOfColumns = columns;

	return	output;
} // end template to_sparse

#endif



