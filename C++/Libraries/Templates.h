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
#include <unordered_map>

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


// Choose sampleSize elements randomly from a vector containing vector.size() elements.
template< typename T >
void	sample_vector( std::vector<T> vec, std::vector<T>& sample, size_t sampleSize )
{
	assert(vec.size() >= sampleSize);
	sample.clear();
	random_unique(vec.begin(), vec.end(), sampleSize);
	for ( std::size_t i = 0; i < sampleSize; i++ )
		sample.push_back (vec[i]);

	sample.shrink_to_fit();
} // end template sample_vector


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


// Adds vector elements with val value based on vector indexes
template <typename T, typename V>
void    add_elements(std::vector<T> &v, const std::vector<int> &indexes, const V &val) {

	std::for_each (indexes.begin(), indexes.end(),//
	[&v, &val](int element) {v.at(element)+=val;});

} // end template add_elements


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
void operator-=(std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    std::vector<T>	aux;
    std::transform(a.begin(), a.end(), b.begin(), 
                   std::back_inserter(aux), std::minus<T>());

    aux.shrink_to_fit();
    a=aux;
}


// Add one vector to another; operator+= overload for std::vector.
// Example, if
// A1 = {1,2,3}; and
// A2 = {4,5,6}; then
// A1+=A2; and
// A1 = {5,7,9}; and
// A2 = {4,5,6};
template <typename T>
void operator+=(std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    std::vector<T>	aux;
    std::transform(a.begin(), a.end(), b.begin(), 
                   std::back_inserter(aux), std::plus<T>());

    aux.shrink_to_fit();
    a=aux;
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


// finds element matches between two vectors,
// then returns v1 indexes of the matching elements
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


// gets the maximum element from vector 
template <typename T>
T	get_maximum_element(const std::vector<T>& v)
{
	auto	result = std::max_element(std::begin(v), std::end(v));
	return	v[std::distance(std::begin(v), result)];
} // end template get_maximum_element


// System of recursive overloaded templates
// to get the number of non-zero elements in a multidimensional vector
template <typename T>
std::size_t	get_number_of_non_zero_elements(const std::vector<T>& v)
{
	return std::count_if(v.begin(), v.end(), [](T i){return i != (T)0;});
} 

template <typename T>
std::size_t	get_number_of_non_zero_elements(const std::vector<std::vector<T>>& v)
{
	std::size_t	numberOfNonZeroElements = 0;
	
	for (const auto& s : v)
		numberOfNonZeroElements += get_number_of_non_zero_elements(s); 

	return	numberOfNonZeroElements;
} // end template get_number_of_non_zero_elements


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


// Converts from a dense vector of vectors to SparseMatrixElements
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


// checks if there is in v an element less than or equal to a 
template <typename T>
bool	check_if_less_than_or_equal_to(const std::vector<T>& v, const T a)
{
return  ( std::find_if(v.begin(), v.end(),
             [a](const T & m) -> bool { return m <= a; }) != v.end() );
} // end tempplate check_if_less_than_or_equal_to


// checks if there is in v an element less than a 
template <typename T>
bool	check_if_less_than(const std::vector<T>& v, const T a)
{
return  ( std::find_if(v.begin(), v.end(),
             [a](const T & m) -> bool { return m < a; }) != v.end() );
} // end tempplate check_if_less_than


// checks if there is in v an element equal to a 
template <typename T>
bool	check_if_equal_to(const std::vector<T>& v, const T a)
{
return  ( std::find_if(v.begin(), v.end(),
             [a](const T & m) -> bool { return m == a; }) != v.end() );
} // end tempplate check_if_equal_to


// gets an histogram from vector of vectors
template <typename T>
std::unordered_map<T,unsigned int>   get_histogram(const std::vector<std::vector<T>>& v)
{
    std::unordered_map<T,unsigned int> histogram;
    std::for_each(v.begin(), v.end(), [&histogram=histogram]( const std::vector<T>& inner_vec)
     {
          for(T val : inner_vec)
          {
            ++histogram[val];   
          }
     });
    return  histogram;
} // end template get_histogram


// gets unique elements' vector from vecto of vectors of elements 
template <typename T>
std::vector<T>   get_unique_elements(const std::vector<std::vector<T>>& v)
{
    std::vector<T>  output;
    
    for(const auto& s: v)
        for(const auto& t: s)
            output.push_back(t);
    
    output.shrink_to_fit();
    
    std::sort(output.begin(), output.end());
    auto last = std::unique(output.begin(), output.end());
    output.erase(last, output.end()); 

    return  output;
} // end template get_unique_elements


// this is a recursive overloaded system of templates to test if a
// generally nested vector of vectors of ... is rectangular
template < typename T > bool is_rectangular( const T&  ) { return true ; }

template < typename T > bool is_rectangular( const std::vector< std::vector<T> >& matrix )
{
    static const auto neq_size = []( const auto& a, const auto& b ) { return a.size() != b.size() ; } ;
    if( std::adjacent_find( std::begin(matrix), std::end(matrix), neq_size ) != std::end(matrix) ) return false ;

    for( const auto& v : matrix ) if( !is_rectangular(v) ) return false ;
    return true ;
} // end template is_rectangular


// receives a -not necessarily rectangular-
// multidimensional vector (vector of vectors of ...),
// and returns the dimensions of the corresponsing
// rectangular multidimensional vector
// TODO: Review this template. It does not work well, in some cases, when some of the dimensions are NULL.
// Yet it will never be the case in this aplication.
// So, this condition can be put as an explicit restriction in the template.
// i.e. "matrix cannot have NULL dimenssions".
template < typename T > std::vector<std::size_t> get_rectangular_indexes( const std::vector<T>& matrix )
{
    std::vector<std::size_t>    dimensions;
    dimensions.push_back(matrix.size());
    return  dimensions;
}

template < typename T > std::vector<std::size_t> get_rectangular_indexes( const std::vector<std::vector<T>>& matrix )
{
    std::vector<std::size_t>    dimensions, auxiliary;
    for(auto& v : matrix) {
        auto    temp = get_rectangular_indexes(v);
        dimensions.resize(temp.size());
        
        std::transform (dimensions.begin(), dimensions.end(), temp.begin(), std::back_inserter(auxiliary),//
        [](std::size_t element1, std::size_t element2)
        {
            if ( element1 > element2 )
                return element1;
            else
                return element2;
            
        });
        dimensions = auxiliary;
        auxiliary.clear();
    }
    dimensions.push_back(matrix.size());
    return  dimensions;
}

template < typename T > std::vector<std::size_t> get_rectangular_indexes( const std::vector<std::vector<std::vector<T>>>& matrix )
{
    std::vector<std::size_t>    dimensions, auxiliary;
    for(auto& v : matrix) {
        auto    temp = get_rectangular_indexes(v);
        dimensions.resize(temp.size());
        
        std::transform (dimensions.begin(), dimensions.end(), temp.begin(), std::back_inserter(auxiliary),//
        [](std::size_t element1, std::size_t element2)
        {
            if ( element1 > element2 )
                return element1;
            else
                return element2;
            
        });
        dimensions = auxiliary;
        auxiliary.clear();
    }
    dimensions.push_back(matrix.size());
    return  dimensions;
} // end template get_rectangular_indexes


// receives a -not necessarily rectangular-
// multidimensional vector (vector of vectors of ...),
// checks if it is non-rectangular and,
// in such a case, fill the same with a valued received as argument.
template < typename T, typename V > void make_rectangular( std::vector<T>& matrix, const V& defaultValue,
							std::vector<std::size_t> dimensions = {},
							const bool firstTime = true )
{
	if ( firstTime )
		dimensions = get_rectangular_indexes(matrix);

	matrix.resize(dimensions.back(), defaultValue);
}

template < typename T, typename V > void make_rectangular( std::vector<std::vector<T>>& matrix,
							const V& defaultValue, std::vector<std::size_t> dimensions = {},
							const bool firstTime = true )
{
	if ( firstTime )
		dimensions = get_rectangular_indexes(matrix);

	matrix.resize(dimensions.back());
	dimensions.pop_back();

	for( auto& v : matrix ) make_rectangular(v,defaultValue,dimensions,false) ;
} // end template make_rectangular


// this is a recursive overloaded system of templates to get the dimensionality
// of a generally nested vector of vectors of ...
// the template test if the data structure is rectangular and throw
// an error in the negative case
template < typename T > std::vector<std::size_t> get_dimensionality( const T&, bool ) { return {} ; }

template < typename T > std::vector<std::size_t>
get_dimensionality( const std::vector<T>& vec, bool guaranteed_to_be_rectangualar = false )
{
    if( !guaranteed_to_be_rectangualar && !is_rectangular(vec) ) throw std::domain_error(
   "get_dimensionality template inconsistence.\n"
   "Argument \"matrix\" is not rectangular.\n" ) ;

    std::vector<std::size_t> dims { vec.size() } ;

    const auto inner_dims = get_dimensionality( vec.empty() ? typename std::vector<T>::value_type{} : vec.front(), true ) ;
    dims.insert( dims.end(), inner_dims.begin(), inner_dims.end() ) ;

    return dims ;
} // end template get_dimensionality


// this is a recursive overloaded system of templates to put a
// a generally nested vector of vectors of ... (multidimensional vector)
// in a onedimensional vector
// the template tests if the data structure is rectangular and
// not empty, and throw
// an error in the negative case
template <typename T, typename V> void
to_one_dimentional_vector( const T& element,
			   std::vector<V>& v1,
			   bool )
{
    v1.push_back(element);
}

template <typename T, typename V> void
to_one_dimentional_vector( const std::vector<T>& vec,
			   std::vector<V>& v1,
			   bool guaranteed_to_be_rectangualar = false )
{
    if( !guaranteed_to_be_rectangualar && !is_rectangular(vec) ) throw std::domain_error(
   "to_one_dimentional_vector template inconsistence.\n"
   "Nested vector of vectors of ... is not rectangular.\n" ) ;

    if ( vec.empty() ) throw std::domain_error(
   "to_one_dimentional_vector template inconsistence.\n"
   "Nested vector of vectors of ... is empty.\n" ) ;
    for (const auto& v : vec)
        to_one_dimentional_vector( v, v1, true ) ;

} // end template to_one_dimentional_vector


// this is a recursive overloaded system of templates to generate a
// generally nested vector of vectors of ... (multidimensional vector)
// whose structue is determine -at compile-time- through received arguments
// the template tests if the data structure is not null, and throw
// an error in the negative case
namespace detail
{
    template< typename T, std::size_t NDIMS > struct vector_builder
    {
        using type = std::vector< typename vector_builder<T,NDIMS-1>::type > ;

        static type make( std::vector<std::size_t> dims, const T& v = {} )
        {
            const auto vec = std::vector<T>( dims.empty() ? 0 : dims.back(), v ) ;
            if( !dims.empty() ) dims.pop_back() ;
            return vector_builder< std::vector<T>, NDIMS-1 >::make( dims, vec ) ;
        }
    };

    template< typename T > struct vector_builder<T,1>
    {
        using type = std::vector<T> ;

        static type make( std::vector<std::size_t> dims, const T& v = {} )
        { return type( dims.empty() ? 0 : dims.back(), v ) ; }
    };
}

template< typename T, typename... SIZE_T >
auto make_vector( const T& v, SIZE_T... dims )
{
    static_assert( sizeof...(dims) != 0,
    "make_vector template inconsistence:\n"
    "Invalid null dimension.\n" ) ;
    std::vector<std::size_t> vec_dims{ dims... } ;
    return detail::vector_builder< T, sizeof...(dims) >::make( vec_dims, v ) ;
} // end template make_vector


// this is a recursive overloaded system of templates to put a
// onedimensional vector in a
// generally nested vector of vectors of ... (multidimensional vector)
// the template tests if the data structure is rectangular and
// not empty, and throw
// an error in the negative case
// if the one-dimensional vector is larger than needed,
// surplus elements are discarded.
// if the one-dimensional vector is smaller than needed,
// absent elements are substituted with zeros.
template<typename T>
void	to_multi_dimentional_vector( T& s, std::vector<T>& vec, bool )
{
	if ( vec.size() == 0 ) {
		s = 0;
	}
	else {
		s = vec.front();
		std::vector<T> aux(vec.begin()+1,vec.end());
		vec = aux;
	}
}

template<typename T, typename V>
void	to_multi_dimentional_vector( std::vector<T>& v, std::vector<V>& vec,
					bool guaranteed_to_be_rectangualar = false )
{
	if( !guaranteed_to_be_rectangualar && !is_rectangular(v) ) throw std::domain_error(
	"to_multi_dimentional_vector template inconsistence.\n"
	"Nested vector of vectors of ... is not rectangular.\n" ) ;

	if ( v.empty() ) throw std::domain_error(
	"to_multi_dimentional_vector template inconsistence.\n"
	"Nested vector of vectors of ... is empty.\n" ) ;

	for (auto& s : v)
		to_multi_dimentional_vector(s, vec, true);
} // end template to_multi_dimentional_vector


// this is a recursive overloaded system of templates to resize a
// generally nested vector of vectors of ... (multidimensional vector)
// according to a vector with the dimensionality information
// entered as an argument
// the template tests if the nested vector structure satisfies
// the dimensionality entered as argument, and throws
// an error in the negative case
template<typename T>
void	resize_multi_dimentional_vector( std::vector<T>& v, const std::vector<std::size_t> vec,
					const bool )
{
	assert(vec.size() == 1);
	v.resize(vec.front());
}

template<typename T>
void	resize_multi_dimentional_vector( std::vector<std::vector<T>>& v, std::vector<std::size_t> vec,
					const bool testConsistency = true )
{
	if ( testConsistency ) {
		auto dimensions = get_dimensionality(v);
		if ( dimensions.size() != vec.size() ) throw std::domain_error(
		"resize_multi_dimentional_vector template inconsistence.\n"
		"Nested vector of vectors of ... has not the\n"
		"same number of dimensions than spesified via input argument.\n" ) ;
	}

	v.resize(vec.front());
	std::vector<std::size_t> aux(vec.begin()+1,vec.end());
	vec = aux;

	for (auto& s : v)
		resize_multi_dimentional_vector(s, vec, false);
} // end template resize_multi_dimentional_vector


// Gets the sparsity of a multidimennsional rectangular vector
template <typename T>
double	get_rectangular_sparsity(const std::vector<T>& v)
{
	assert(is_rectangular(v));
	auto	dimensions = get_rectangular_indexes(v);
	auto	numberOfElements = std::accumulate(dimensions.begin(), dimensions.end(), 1, std::multiplies<double>());
	auto	numberOfNonZeroElements = get_number_of_non_zero_elements(v);
	return	1.0-(double)numberOfNonZeroElements/numberOfElements;
} // end template get_rectangular_sparsity


// computes the reciprocal of value
template <typename T>
T reciprocal ( T value )
{ return (T)1/value ; }



#endif



