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


// Save vector of vectors as sparse bool matrix type in Octave (.mat) file.
template <typename T>
void	save_vector_of_vectors_as_sparse_bool_matrix( const std::string& name, const std::vector<std::vector<T>>& matrix, unsigned int rows, unsigned int columns, ofstream &FILE )
{
	static_assert( std::is_integral<T>::value, "In save_vector_of_vectors_as_sparse_bool_matrix template: expected an integral type" );

	std::string	nameLine = "# name: ", typeLine = "# type: sparse bool matrix";
	std::vector<T>	auxiliary;
	nameLine += name;
	FILE << nameLine << endl;
	FILE << typeLine << endl;

	unsigned int	number = 0;
	for ( std::size_t i = 0; i < matrix.size(); i++ )
		number += matrix[i].size();

	FILE << "# nnz: " << number << endl;
	FILE << "# rows: " << rows << endl;
	FILE << "# columns: " << columns << endl;

	for ( std::size_t i = 0; i < matrix.size(); i++ )
	{
		auxiliary = matrix[i];
		std::sort(auxiliary.begin(), auxiliary.end());
		for ( std::size_t j = 0; j < matrix[i].size(); j++ )
			FILE << auxiliary[j]+1 << ' ' << i+1 << ' ' << "1" << endl;
	}

	FILE << "\n" << endl;
} // end template save_vector_of_vectors_as_sparse_bool_matrix


// Save vector of vectors as sparse matrix type in Octave (.mat) file.
// typename T must be any struct with -at least- two members:
// 1° A float called "value".
// 2° An unsigned int calles "index".
template <typename T>
void	save_vector_of_vectors_as_sparse_matrix( const std::string& name, const std::vector<std::vector<T>>& matrix, unsigned int rows, unsigned int columns, ofstream &FILE )
{
	std::string	nameLine = "# name: ", typeLine = "# type: sparse matrix";
	std::vector<T>	auxiliary;
	nameLine += name;
	FILE << nameLine << endl;
	FILE << typeLine << endl;

	unsigned int	number = 0;
	for ( std::size_t i = 0; i < matrix.size(); i++ )
		number += matrix[i].size();

	FILE << "# nnz: " << number << endl;
	FILE << "# rows: " << rows << endl;
	FILE << "# columns: " << columns << endl;

	for ( std::size_t i = 0; i < matrix.size(); i++ )
	{
		auxiliary = matrix[i];
		// sort auxiliary vector using a custom function object because of the struct
		// T
		struct
		{
			bool operator()(T element1, T element2)
			{   
			    return element1.index < element2.index;
			}   
		} customLess;
		std::sort(auxiliary.begin(), auxiliary.end(), customLess);
		for ( std::size_t j = 0; j < matrix[i].size(); j++ )
			FILE << auxiliary[j].index+1 << ' ' << i+1 << ' ' << auxiliary[j].value << endl;
	}

	FILE << "\n" << endl;
} // end template save_vector_of_vectors_as_sparse_matrix


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


// Load sparse bool matrix type from Octave (.mat) file into vector of vectors reference.
template <typename T>
void	load_sparse_bool_matrix_to_vector_of_vectors( std::vector<std::vector<T>>& arr, ifstream &FILE )
{
	std::size_t	nnz, input, corticalColumn;
	std::string	str, STR;

	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_sparse_bool_matrix_to_vector_of_vectors:" << endl;
		std::cout << "Cannot read line from file." << endl;
		exit( EXIT_FAILURE );
	}
	STR = "# type: sparse bool matrix";
	if ( str.compare(STR) == 0 )
	{

		str.resize(7);
		FILE.read(&str[0],7);
		STR = "# nnz: ";
		if ( str.compare(STR) == 0 )
		{
			FILE >> nnz;
			unsigned int	number = 0;
			for ( std::size_t i = 0; i < arr.size(); i++ )
			{
				number += arr[i].size();
				arr[i].erase(arr[i].begin(),arr[i].end());
				arr[i].shrink_to_fit();
			}

			if ( nnz != number )
			{
				std::cout << "\nOctaveInterface.h warning:" << endl;
				std::cout << "In function load_sparse_bool_matrix_to_vector_of_vectors:" << endl;
				std::cout << "number of non-zero elements from file isn't equal to the number of elements in arr." << endl;
				//exit( EXIT_FAILURE );
			}
		}
		else
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_sparse_bool_matrix_to_vector_of_vectors:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}

		for ( std::size_t i = 0; i < 3; i++ )
		{
			if ( !std::getline(FILE, str) )
			{
				std::cout << "\nOctaveInterface.h inconsistency:" << endl;
				std::cout << "In function load_sparse_bool_matrix_to_vector_of_vectors:" << endl;
				std::cout << "Cannot read line from file." << endl;
				exit( EXIT_FAILURE );
			}
		}

		for ( std::size_t row = 0; row < nnz; row++ )
		{
			// The vector of vectors was introduced in the file in the following way:
			// FILE << auxiliary[j]+1 << ' ' << i+1 << ' ' << "1" << endl;
			FILE >> input;
			input--;
			FILE >> corticalColumn;
			corticalColumn--;
			arr[corticalColumn].push_back(input);
			std::getline(FILE, str);
		}

		for ( std::size_t columnIndex = 0; columnIndex < arr.size(); columnIndex++ )
			arr[columnIndex].shrink_to_fit();
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_sparse_bool_matrix_to_vector_of_vectors:" << endl;
		std::cout << "arr must be of type sparse bool matrix." << endl;
		exit( EXIT_FAILURE );
	}

} // end template load_sparse_bool_matrix_to_vector_of_vectors


// Load sparse matrix type from Octave (.mat) file into vector of vectors reference.
// typename T must be any struct with -at least- two members:
// 1° A float called "value".
// 2° An unsigned int called "index".
template <typename T>
void	load_sparse_matrix_to_vector_of_vectors( std::vector<std::vector<T>>& arr, ifstream &FILE )
{
	std::size_t	nnz, index, corticalColumn;
	float	value;
	SparseMatrixElement	element;
	std::string	str, STR;

	if ( !std::getline(FILE, str) )
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_sparse_matrix_to_vector_of_vectors:" << endl;
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
		{
			FILE >> nnz;
			unsigned int	number = 0;
			for ( std::size_t i = 0; i < arr.size(); i++ )
			{
				number += arr[i].size();
				arr[i].erase(arr[i].begin(),arr[i].end());
				arr[i].shrink_to_fit();
			}

			if ( nnz != number )
			{
				std::cout << "\nOctaveInterface.h warning:" << endl;
				std::cout << "In function load_sparse_matrix_to_vector_of_vectors:" << endl;
				std::cout << "number of non-zero elements from file isn't equal to the number of elements in arr." << endl;
				//exit( EXIT_FAILURE );
			}
		}
		else
		{
			std::cout << "\nOctaveInterface.h inconsistency:" << endl;
			std::cout << "In function load_sparse_matrix_to_vector_of_vectors:" << endl;
			std::cout << "File corrupted." << endl;
			exit( EXIT_FAILURE );
		}

		for ( std::size_t i = 0; i < 3; i++ )
		{
			if ( !std::getline(FILE, str) )
			{
				std::cout << "\nOctaveInterface.h inconsistency:" << endl;
				std::cout << "In function load_sparse_matrix_to_vector_of_vectors:" << endl;
				std::cout << "Cannot read line from file." << endl;
				exit( EXIT_FAILURE );
			}
		}

		for ( std::size_t row = 0; row < nnz; row++ )
		{
			// The vector of vectors was introduced in the file in the following way:
			// FILE << auxiliary[j].index+1 << ' ' << i+1 << ' ' << auxiliary[j].value << endl;
			FILE >> index;
			index--;
			FILE >> corticalColumn;
			corticalColumn--;
			FILE >> value;
			element.index = index;
			element.value = value;
			arr[corticalColumn].push_back(element);
			std::getline(FILE, str);
		}

		for ( std::size_t columnIndex = 0; columnIndex < arr.size(); columnIndex++ )
			arr[columnIndex].shrink_to_fit();
	}
	else
	{
		std::cout << "\nOctaveInterface.h inconsistency:" << endl;
		std::cout << "In function load_sparse_matrix_to_vector_of_vectors:" << endl;
		std::cout << "arr must be of type sparse matrix." << endl;
		exit( EXIT_FAILURE );
	}

} // end template load_sparse_matrix_to_vector_of_vectors


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


