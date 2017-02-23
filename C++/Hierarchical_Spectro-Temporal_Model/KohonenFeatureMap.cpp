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

// File Name:		KohonenFeatureMap.cpp
// File Description:	KohonenFeatureMap member-function definitions. This file contains implementations of the
//			member functions prototyped in KohonenFeatureMap.h.

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "KohonenFeatureMap.h"				// include definition of class KohonenFeatureMap

using namespace std;

// constructor initializes unitsArrayDimensionality, inputDimensionality, unitsDimensionality and synapticWeights with variables supplied as arguments
KohonenFeatureMap::KohonenFeatureMap( int arrayDimension, int inputDimension, int unitsDimension, int outputDimension )
{
	setUnitsArrayDimensionality( arrayDimension );	// calls set function to initialize unitsArrayDimensionality
	setInputDimensionality( inputDimension );	// calls set function to initialize inputDimensionality
	setUnitsDimensionality( unitsDimension );	// calls set function to initialize unitsDimensionality
	setOutputDimensionality( outputDimension );	// calls set function to initialize outputDimensionality
	validateObject();				// calls function to validate the object
} // end KohonenFeatureMap constructor


// function to validate the object created of this class
void	KohonenFeatureMap::validateObject()
{
	int inputDim, outputDim, base, power, result, auxiliarint;
	double auxiliardouble;

	inputDim = KohonenFeatureMap::getInputDimensionality();
	power = KohonenFeatureMap::getUnitsArrayDimensionality();
	result = KohonenFeatureMap::getUnitsDimensionality();
	outputDim = KohonenFeatureMap::getOutputDimensionality();
	if ( inputDim <= 0 || result <= 0 || power <= 0 || outputDim <= 0 )
	{
		cout << "KohonenFeatureMap object inconsistence: inputDimensionality = " << inputDim << "\n" << endl;
		cout << "KohonenFeatureMap object inconsistence: unitsArrayDimensionality = " << power << "\n" << endl;
		cout << "KohonenFeatureMap object inconsistence: unitsDimensionality = " << result << "\n" << endl;
		cout << "KohonenFeatureMap object inconsistence: outputDimensionality = " << outputDim << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( inputDim < power )
	{
		cout << "KohonenFeatureMap object inconsistence: inputDimensionality = " << inputDim << " must be greater or -at least- equal than unitsArrayDimensionality = " << power << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	auxiliardouble = log10(result)/power;
	base = (int)(pow(10,auxiliardouble)+0.01);

	if ( base == 1 && result != 1 )
	{
		cout << "KohonenFeatureMap object inconsistence: unitsDimensionality = " << result << " is not a power of unitsArrayDimensionality = " << power << "\n" << endl;
		exit( EXIT_FAILURE );
	}

	if ( base != 1 )
	{
		auxiliarint = result;
		while (result % base == 0)
		{
	    		result /= base;
		}
		if ( result != 1 )
		{
			cout << "KohonenFeatureMap object inconsistence: unitsDimensionality = " << auxiliarint << " is not a power of unitsArrayDimensionality = " << power << "\n" << endl;
			exit( EXIT_FAILURE );
		}
	}

	if ( outputDim != power )
	{
		cout << "KohonenFeatureMap object inconsistence: outputDimensionality = " << outputDim << " is not equal to power = " << power << ".\n" << endl;
		exit( EXIT_FAILURE );
	}
		
} // end function validateObject


// function to reserve memory for the object arrays
void	KohonenFeatureMap::reserveMemory( double *weights )
{
	KohonenFeatureMap::reservSynapticWeights();			// calls function to reserve space for the weights matrix
	KohonenFeatureMap::setSynapticWeights( weights );		// calls set function to initialize the array of weights
	KohonenFeatureMap::reservInputVector();				// calls function to reserve space for the inputs vector
	KohonenFeatureMap::reservUnitsVector();				// calls function to reserve space for the units vector
	KohonenFeatureMap::reservOutputVector();			// calls function to reserve space for the output vector
} // end function reserveMemory


// function to modify the synapticWeights matrix throught the use of the Kohonen's learning rule
void	KohonenFeatureMap::learningRuleFunction( double learningRate, double neighborParameter, int unitsWinnerPosition )
{
	int i, j, numberOfInputs, numberOfUnits;
	double *input, *weightsMatrixPointer, *deltaWeightsMatrix, neighborhoodValue;

	numberOfInputs = KohonenFeatureMap::getInputDimensionality();				// gets the number of input units
	numberOfUnits = KohonenFeatureMap::getUnitsDimensionality();				// gets the number of units units

	deltaWeightsMatrix = (double*)calloc((numberOfInputs*numberOfUnits), sizeof(double));	// reserves space for the deltaWeightsMatrix

	weightsMatrixPointer = KohonenFeatureMap::getSynapticWeights();				// gets the synapticWeights matrix
	input = KohonenFeatureMap::getInputVector();						// gets the inputVector pointer

	for (i=0; i < numberOfUnits; i++)							// the index i corresponds to the unit
	{
		neighborhoodValue = KohonenFeatureMap::learningNeighborhoodFunction(neighborParameter, unitsWinnerPosition, (i+1));
		for (j=0; j < numberOfInputs; j++)						// the index j corresponds to the input unit
		{
			// This is the Kohonen learning rule to update the synaptic weights
			*((deltaWeightsMatrix + i*numberOfInputs) + j) = learningRate*neighborhoodValue*( (*(input + j)) - *((weightsMatrixPointer + i*numberOfInputs) + j) );
			*((weightsMatrixPointer + i*numberOfInputs) + j) = *((weightsMatrixPointer + i*numberOfInputs) + j) + *((deltaWeightsMatrix + i*numberOfInputs) + j);
		}
	}
	KohonenFeatureMap::setSynapticWeights(weightsMatrixPointer);
	free(input);										// frees the space used by the vector to which input points
	free(weightsMatrixPointer);								// frees the space used by the array to which weightsMatrixPointer points
	free(deltaWeightsMatrix);								// frees the space used by the array deltaWeightsMatrix
} // end function learningRuleFunction


// function to compute the neighborhood value in the lateral interaction between units in the array for learning process
double	KohonenFeatureMap::learningNeighborhoodFunction( double widthParameter, int winnerPosition, int otherPosition )
{
	int i, *winnerPositionArray, *otherPositionArray, distance, auxiliaryVariableInt, dimensionality;

	dimensionality = getUnitsArrayDimensionality();								// gets the number of dimensions of the array

	winnerPositionArray = (int*)calloc(dimensionality, sizeof(int));					// reserves space to be used by the method KohonenFeatureMap::unitsArrayMapping for the coordinates of the units array corresponding to the location of the unit, and
	KohonenFeatureMap::unitsArrayMapping(winnerPosition, winnerPositionArray);				// gets a vector with the array coordinates corresponding to the winnerPosition

	otherPositionArray = (int*)calloc(dimensionality, sizeof(int));						// reserves space to be used by the method KohonenFeatureMap::unitsArrayMapping for the coordinates of the units array corresponding to the location of the unit, and = (int*)calloc(dimensionality, sizeof(int));
	KohonenFeatureMap::unitsArrayMapping(otherPosition, otherPositionArray);				// gets a vector with the array coordinates corresponding to the otherPosition

	distance = 0;												// initializes distance accumulator to zero
	for (i=0; i < dimensionality; i++)
	{
		auxiliaryVariableInt = (int)abs(*(winnerPositionArray + i) - *(otherPositionArray + i));	// extracts the absolute value of the difference between the array indexes of winnerPositionArray and otherPositionArray
		distance = distance + auxiliaryVariableInt;							// accumulate this value in the variable distance
	}
	free(winnerPositionArray);										// frees the space used by the method KohonenFeatureMap::unitsArrayMapping for the coordinates of the units array corresponding to the location of the unit
	free(otherPositionArray);										// frees the space used by the method KohonenFeatureMap::unitsArrayMapping for the coordinates of the units array corresponding to the location of the unit
	return exp(-(pow((double)distance, (double)2)) / (2*widthParameter));					// returns the neighborhood value
} // end function learningNeighborhoodFunction


// function to compute the the array coordinates corresponding to the unit position given as a parameter
void	KohonenFeatureMap::unitsArrayMapping( int unit, int *unitsArrayCoordinates )
{
	int i, flag, dimensionality, numberOfOutpusUnits, unitsPerDimension, auxiliaryVariableInt, remainder, quotient, *divisors;
	float auxiliaryVariableFloat;
	double auxiliaryVariableDouble;

	dimensionality = getUnitsArrayDimensionality();						// gets the number of dimensions of the array
	numberOfOutpusUnits = getUnitsDimensionality();						// gets the number of units

	auxiliaryVariableDouble = (double)1 / (double)dimensionality;				// gets the inverse of the dimensionality number
	auxiliaryVariableDouble = pow((double)numberOfOutpusUnits, auxiliaryVariableDouble);	// computes the dimensionality root of the number of units
	auxiliaryVariableDouble = auxiliaryVariableDouble + 0.5;
	unitsPerDimension = (int)auxiliaryVariableDouble;					// in this way, the number of units per dimension is obtained. Then, unitsPerDimension*dimensionality is the total number of output units.

	if (dimensionality == 1)								// if the dimensionality is one, then the program
	{
		*unitsArrayCoordinates = unit;							// unit is just passed to the unique element in unitsArrayCoordinates
	}
	else											// otherwise, the program folows the next steps
	{
		divisors = (int*)calloc((dimensionality - 1), sizeof(int));			// reserves space for the divisors
		auxiliaryVariableInt = numberOfOutpusUnits;
		for (i = 1; i < dimensionality; i++)
		{
			auxiliaryVariableInt = (int)(((double)auxiliaryVariableInt / (double)unitsPerDimension) + 0.5);
			*(divisors + (i-1)) = auxiliaryVariableInt;				// divisors is a vector with the total number of units in a part of the units array of dimensionality-1 dimensions, dimensionality-2 dimensions and so on until the total number of units in one dimension (that is unitsPerDimension)
		}

		flag = 0;
		for (i = 1; i < dimensionality; i++)						// this loop extracts every coordinate of the units array corresponding to the location of the unit
		{
			if (flag == 0)								// if flag is zero, the normal scenario occurs. That means that the remainder is not zero
			{
				auxiliaryVariableFloat = fmod( unit, *(divisors + (i-1)) );
				auxiliaryVariableFloat = auxiliaryVariableFloat + 0.5;
				remainder = (int)auxiliaryVariableFloat;			// gets the remainder of the quotient between the number of the location of the unit and one of the divisors

				auxiliaryVariableDouble = (double)unit / (double)(*(divisors + (i-1)));
				auxiliaryVariableDouble = auxiliaryVariableDouble;
				quotient = (int)auxiliaryVariableDouble;			// gets the quotient between the number of the location of the unit and one of the divisors

				if (remainder == 0)						// if the remainder is zero, that means the unit is in the extreme of the array regarding to the extra coordinates
				{
					flag = 1;						// the flag is established, and
					*(unitsArrayCoordinates + (i-1)) = quotient;		// the quotient is passed to the units array coordinate
				}
				else								// in the other way,
				{
					*(unitsArrayCoordinates + (i-1)) = (quotient + 1);	// quotient plus one is passed to the units array coordinate
				}
			}
			else									// if, on the contrary, flag is zero, the remainder is zero and special actions have to be taken
			{
				*(unitsArrayCoordinates + (i-1)) = unitsPerDimension;		// in this case, every extra coordinate has to be established to the variable unitsPerDimension
			}

			if (i == (dimensionality - 1))						// the last coordinate of the vector has to be established with
			{
				if (flag == 1)
					*(unitsArrayCoordinates + i) = unitsPerDimension;	// unitsPerDimension if the remainder was zero
				else
					*(unitsArrayCoordinates + i) = remainder;		// or with the remainder in a normal situation				
			}
			unit = remainder;							// the remainder of the quotient becomes in the new unit for the following iteration
		}
		free(divisors);									// frees the space used by the divisors
	}
} // end function unitsArrayMapping


// function to set the dimensionality of the array of units
void	KohonenFeatureMap::setUnitsArrayDimensionality( int dimension )
{
	unitsArrayDimensionality = dimension;				// store the dimensionality of the array of units in the object
} // end function setUnitsArrayDimensionality


// function to get the dimensionality of the array of units
int	KohonenFeatureMap::getUnitsArrayDimensionality()
{
	return unitsArrayDimensionality;				// return object's unitsArrayDimensionality
} // end function getUnitsArrayDimensionality


// function to set the dimensionality of the input vector
void	KohonenFeatureMap::setInputDimensionality( int dimension )
{
	inputDimensionality = dimension;				// store the dimensionality of the input vector
} // end function setInputDimensionality


// function to get the dimensionality of the input vector
int	KohonenFeatureMap::getInputDimensionality()
{
	return inputDimensionality;					// return object's inputDimensionality
} // end function getInputDimensionality


// function to set the dimensionality of the units vector
void	KohonenFeatureMap::setUnitsDimensionality( int dimension )
{
	unitsDimensionality = dimension;				// store the dimensionality of the units vector
} // end function setUnitsDimensionality


// function to get the dimensionality of the units vector
int	KohonenFeatureMap::getUnitsDimensionality()
{
	return unitsDimensionality;					// return object's unitsDimensionality
} // end function getUnitsDimensionality


// function to set the dimensionality of the output vector
void	KohonenFeatureMap::setOutputDimensionality( int dimension )
{
	outputDimensionality = dimension;				// store the dimensionality of the output vector
} // end function setOutputDimensionality


// function to get the dimensionality of the output vector
int	KohonenFeatureMap::getOutputDimensionality()
{
	return outputDimensionality;					// return object's outputDimensionality
} // end function getOutputDimensionality


// function to reserve space for the weights matrix
void	KohonenFeatureMap::reservSynapticWeights()
{
	int numberOfInputs, numberOfUnits;

	numberOfInputs = KohonenFeatureMap::getInputDimensionality();		// gets the number of input units
	numberOfUnits = KohonenFeatureMap::getUnitsDimensionality();		// gets the number of units

	synapticWeights = (double*)calloc(numberOfUnits*numberOfInputs, sizeof(double));	// reserves space for the synapticWeights array
} // end function reserveSynapticWeights


// function to set the weights matrix
void	KohonenFeatureMap::setSynapticWeights( double *weights )
{
	int i, j, numberOfInputs, numberOfUnits;

	numberOfInputs = KohonenFeatureMap::getInputDimensionality();		// gets the number of input units
	numberOfUnits = KohonenFeatureMap::getUnitsDimensionality();		// gets the number of units

	for (i = 0; i < numberOfUnits; i++)
		for (j = 0; j < numberOfInputs; j++)
			*((synapticWeights + i*numberOfInputs) + j) = *((weights + i*numberOfInputs) + j);	// store the weights in the synapticWeights array
} // end function setSynapticWeights


// function to get the weights matrix
double	*KohonenFeatureMap::getSynapticWeights()
{
	int i, j, numberOfInputs, numberOfUnits;
	double *vector;

	numberOfInputs = KohonenFeatureMap::getInputDimensionality();		// gets the number of input units
	numberOfUnits = KohonenFeatureMap::getUnitsDimensionality();		// gets the number of units

	vector = (double*)calloc(numberOfUnits*numberOfInputs, sizeof(double));	// reserves space for the vector array

	for (i = 0; i < numberOfUnits; i++)
		for (j = 0; j < numberOfInputs; j++)
			*((vector + i*numberOfInputs) + j) = *((synapticWeights + i*numberOfInputs) + j);	// store the synapticWeights in the vector array

	return vector;							// returns vector pointer
} // end function getSynapticWeights


// function to reserve space for the inputs vector
void	KohonenFeatureMap::reservInputVector()
{
	int numberOfInputs;

	numberOfInputs = KohonenFeatureMap::getInputDimensionality();		// gets the number of input units

	inputVector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for the inputVector vector
} // end function reserveInputVector


// function to set the inputs vector
void	KohonenFeatureMap::setInputVector( double *inputs )
{
	int j, numberOfInputs;

	numberOfInputs = KohonenFeatureMap::getInputDimensionality();		// gets the number of input units

	for (j = 0; j < numberOfInputs; j++)
		*(inputVector + j) = *(inputs + j);				// store the inputs in the inputVector vector
} // end function setInputVector


// function to get the inputs vector
double	*KohonenFeatureMap::getInputVector()
{
	int j, numberOfInputs;
	double *vector;

	numberOfInputs = KohonenFeatureMap::getInputDimensionality();		// gets the number of input units

	vector = (double*)calloc(numberOfInputs, sizeof(double));		// reserves space for vector

	for (j = 0; j < numberOfInputs; j++)
		*(vector + j) = *(inputVector + j);				// stores the inputVector in vector

	return vector;								// returns vector pointer
} // end function getInputVector


// function to reserve space for the units vector
void	KohonenFeatureMap::reservUnitsVector()
{
	int numberOfUnits;

	numberOfUnits = KohonenFeatureMap::getUnitsDimensionality();		// gets the number of units

	unitsVector = (bool*)calloc(numberOfUnits, sizeof(bool));		// reserves space for the unitsVector vector
} // end function reservUnitsVector


// function to set the units vector
void	KohonenFeatureMap::setUnitsVector( bool *units )
{
	int i, numberOfUnits;

	numberOfUnits = KohonenFeatureMap::getUnitsDimensionality();		// gets the number of units

	for (i = 0; i < numberOfUnits; i++)
		*(unitsVector + i) = *(units + i);				// store the units in the unitsVector vector
} // end function setUnitsVector


// function to get the units vector
bool	*KohonenFeatureMap::getUnitsVector()
{
	int i, numberOfUnits;
	bool *vector;

	numberOfUnits = KohonenFeatureMap::getUnitsDimensionality();		// gets the number of units

	vector = (bool*)calloc(numberOfUnits, sizeof(bool));			// reserves space for vector

	for (i = 0; i < numberOfUnits; i++)
		*(vector + i) = *(unitsVector + i);				// stores the unitsVector in vector

	return vector;								// returns vector pointer
} // end function getUnitsVector


// function to reserve space for the output vector
void	KohonenFeatureMap::reservOutputVector()
{
	int numberOfOutput;

	numberOfOutput = KohonenFeatureMap::getOutputDimensionality();		// gets the number of output units

	outputVector = (double*)calloc(numberOfOutput, sizeof(double));	// reserves space for the outputVector vector
} // end function reservOutputVector


// function to set the output vector
void	KohonenFeatureMap::setOutputVector( double *output )
{
	int i, numberOfOutput;

	numberOfOutput = KohonenFeatureMap::getOutputDimensionality();		// gets the number of output units

	for (i = 0; i < numberOfOutput; i++)
		*(outputVector + i) = *(output + i);				// store the output in the outputVector vector
} // end function setOutputVector


// function to get the output vector
double	*KohonenFeatureMap::getOutputVector()
{
	int i, numberOfOutput;
	double *vector;

	numberOfOutput = KohonenFeatureMap::getOutputDimensionality();		// gets the number of output units

	vector = (double*)calloc(numberOfOutput, sizeof(double));		// reserves space for vector

	for (i = 0; i < numberOfOutput; i++)
		*(vector + i) = *(outputVector + i);				// stores the outputVector in vector

	return vector;								// returns vector pointer
} // end function getOutputVector





