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

// File Name:		KohonenFeatureMapPred.cpp
// File Description:	KohonenFeatureMapPred member-function definitions. This file contains implementations of the
//			member functions prototyped in KohonenFeatureMapPred.h.

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "KohonenFeatureMapPred.h"				// includes definition of class KohonenFeatureMapPred


// constructor initializes unitsArrayDimensionality, inputDimensionality, unitsDimensionality, outputDimensionality and synapticWeights with variables supplied as arguments
KohonenFeatureMapPred::KohonenFeatureMapPred( int arrayDimension, int inputDimension, int unitsDimension, int outputDimension )
	// explicitly call base-class constructor
	: KohonenFeatureMap(arrayDimension, inputDimension, unitsDimension, outputDimension)
{
	KohonenFeatureMapPred::setLastWinner(unitsDimension+1);
} // end KohonenFeatureMapPred constructor


// function to reserve memory for the object arrays
void	KohonenFeatureMapPred::reserveMemory( double *weights )
{
	KohonenFeatureMap::reserveMemory( weights );			// calls function to reserve space for arrays in KohonenFeatureMap object

	KohonenFeatureMapPred::reservPredictionSynapsis();		// calls function to reserve space for predictionSynapsis in the cluster
	KohonenFeatureMapPred::reservStrongUnits();			// calls function to reserve space for strongUnits in the cluster
} // end function reserveMemory


// function to compute the outputs of the network and updates the synaptic weights matrix through the Kohonen's learning rule
void	KohonenFeatureMapPred::kohonenFeatureMapping( bool enableLearning, bool enableLearningPred, double learningRate, bool firing, double learningNeighborParameter, double firingNeighborParameter, double predictionThreshold )
{
	firingInformation fire;

	fire = KohonenFeatureMapPred::firingFunction(firing, enableLearning, enableLearningPred, firingNeighborParameter, learningNeighborParameter, learningRate, predictionThreshold);		// calls firingFunction

	if ( enableLearning && fire.winner != 0 )								// only if the learning is enabled and winner unit is distinct from zero
		KohonenFeatureMap::learningRuleFunction(learningRate, learningNeighborParameter, fire.winner);	// calls learningRuleFunction to update the weights matrix
} // end function kohonenFeatureMapping


// function to determine the unit winner position and the firing level in every unit in response to an input vector instance
firingInformation	KohonenFeatureMapPred::firingFunction( bool firing, bool learning, bool enableLearningPred,  double neighborParameter, double neighborLearning, double learningRate, double predictionThreshold )
{
	int i, j;
	int unitsWinnerPosition, numberOfInputs, numberOfUnits, oldWinner;
	double	auxiliaryVariableDouble, minimum, maximum, neighborhoodValue, prediction, maximumNeighborhood;
	double *input, *distanceVector, *weightsMatrixPointer;
	bool predictionState, activeState;
	bool *units, *auxiliaryUnits, *unitsPointer;
	double *strong;
	firingInformation fire;

	maximumNeighborhood = neighborParameter;
	activeState = false;
	predictionState = false;

	numberOfInputs = KohonenFeatureMap::getInputDimensionality();				// gets the number of input units
	numberOfUnits = KohonenFeatureMap::getUnitsDimensionality();				// gets the number of units units

	units = (bool*)calloc(numberOfUnits, sizeof(bool));					// reserves space for the units
	distanceVector = (double*)calloc(numberOfUnits, sizeof(double));			// reserves space for the distanceVector

	input = KohonenFeatureMap::getInputVector();						// gets the inputVector
	unitsPointer = KohonenFeatureMap::getUnitsVector();					// gets the unitsVector pointer
	weightsMatrixPointer = KohonenFeatureMap::getSynapticWeights();				// gets the synapticWeights pointer

	oldWinner = KohonenFeatureMapPred::getLastWinner();					// gets the last winner in the cluster

	if ( firing )										// if firing is true
	{
		minimum = DBL_MAX;								// minimum is initialized with the maximum double representation
		for (i = 0; i < numberOfUnits; i++)						// the index i corresponds to the units unit
		{
			for (j = 0; j < numberOfInputs; j++)					// the index j corresponds to the input unit
			{
				// Computes the Euclidean distances between every unit an the input vector
				auxiliaryVariableDouble = *((weightsMatrixPointer + i*numberOfInputs) + j) - *(input + j);
				auxiliaryVariableDouble = pow(auxiliaryVariableDouble, 2);
				*(distanceVector + i) = *(distanceVector + i) + auxiliaryVariableDouble;
			}
			*(distanceVector + i) = sqrt(*(distanceVector + i));
		}

		for (i = 0; i < numberOfUnits; i++)						// the index i corresponds to the units unit
		{
			if ( *(distanceVector + i) < minimum || i == 0 )
			{
				// Finds the index of the unit that is closest to the input
				minimum = *(distanceVector + i);
				unitsWinnerPosition = (i + 1);
			}
		}

		neighborhoodValue = neighborParameter-0.1;
		while ( !activeState && neighborhoodValue < maximumNeighborhood )
		{
			neighborhoodValue = neighborhoodValue + 0.1;					// increments neighbohoodValue
			for (i = 0; i < numberOfUnits; i++)						// the index i corresponds to the units unit
			{
				if ( *(distanceVector + i) <= neighborhoodValue )			// if unit i+1 is inside the radio of neighborhoodValue then
				{
					*(units + i) = true;						// sets that unit and
					if ( !activeState )
						activeState = true;					// sets activeState

					prediction = KohonenFeatureMapPred::getPrediction(i+1);		// gets a specific prediction value
					if ( prediction > predictionThreshold && !predictionState )	// this is the threshold which determines the prediction state
						predictionState = true;
				}
			}
		}

		maximum = 0;
		if ( activeState )								// if some unit is inside the radio determined by neighborhoodValue
		{
			if ( predictionState )							// if the cluster is in prediction state
			{
				//strong = getStrongUnits();					// function that gets strongUnits in the cluster
				auxiliaryUnits = (bool*)calloc(numberOfUnits, sizeof(bool));	// reserves space for the auxiliaryUnits
				for (i = 0; i < numberOfUnits; i++)				// the index i corresponds to the units unit
				{
					prediction = KohonenFeatureMapPred::getPrediction(i+1);	// function that gets a specific prediction value
					if ( *(units + i) && prediction > predictionThreshold )// acts only if unit number i+1 is active and if it is in predictive state
					{
						// Finds the maximum firing predisposition
						auxiliaryVariableDouble = 1 - (1/neighborhoodValue) * (*(distanceVector + i));	// computes this value for the firing predisposition
						//if ( (*(strong+i))*prediction*auxiliaryVariableDouble >= maximum )
						if ( prediction*auxiliaryVariableDouble >= maximum )
						{
							if ( prediction*auxiliaryVariableDouble == maximum )			// if it is in the same level than maximum
							{
								*(auxiliaryUnits + i) = true;					// activates another unit
							}
							else									// if it is above maximum
							{
								// resets auxiliaryUnits and activates the -so far- unique maximum firing predisposition unit
								free(auxiliaryUnits);						// frees auxiliaryUnits
								auxiliaryUnits = (bool*)calloc(numberOfUnits, sizeof(bool));	// reserves space for the auxiliaryUnits
								maximum = prediction*auxiliaryVariableDouble;			// updates the new maximum
								*(auxiliaryUnits + i) = true;					// activates the corresponding unit
							}
						}
					}
				}

				KohonenFeatureMap::setUnitsVector(auxiliaryUnits);			// sets the vector of units in the map with the most forthcoming units to the firing state
				KohonenFeatureMapPred::firingOutput();					// computes the firing outputs based in the vector of units in this map
				auxiliaryVariableDouble = 1 - (1/neighborhoodValue) * (*(distanceVector + (unitsWinnerPosition-1))); // computes this value for the belief

				//if ( learning )
				//{
					//KohonenFeatureMapPred::updatePredictionSynapsis(false, unitsWinnerPosition, neighborLearning, learningRate, distanceVector);	// function that updates predictionSynapsis in the cluster

					//KohonenFeatureMapPred::updateStrongUnits(unitsWinnerPosition, neighborLearning, learningRate, distanceVector);	// function that updates strongUnits in the cluster

					if ( enableLearningPred )
						KohonenFeatureMapPred::updatePredictionSynapsis(false, unitsWinnerPosition, 1, 0.05, distanceVector);	// function that updates predictionSynapsis in the cluster

					//KohonenFeatureMapPred::updateStrongUnits(unitsWinnerPosition, 1, 0.05, distanceVector);	// function that updates strongUnits in the cluster
				//}

				KohonenFeatureMapPred::setLastWinner(unitsWinnerPosition);	// lastWinner is set to the current value of unitsWinnerPosition
				free(auxiliaryUnits);						// frees the space used for auxiliaryUnits
				auxiliaryUnits = NULL;
				//free(strong);							// frees strong vector
				//strong = NULL;
			}
			else									// if the cluster is not in prediction state
			{
				KohonenFeatureMap::setUnitsVector(units);			// sets the vector of units in the map. In this situation all active units fire
				KohonenFeatureMapPred::firingOutput();				// computes the firing outputs based in the vector of units in this map
				auxiliaryVariableDouble = 1 - (1/neighborhoodValue) * (*(distanceVector + (unitsWinnerPosition-1))); // computes this value for the belief

				//if ( learning )
				//{
					//KohonenFeatureMapPred::updatePredictionSynapsis(false, unitsWinnerPosition, neighborLearning, learningRate, distanceVector);	// updates predictionSynapsis in the cluster with unitsWinnerPosition

					//KohonenFeatureMapPred::updateStrongUnits(unitsWinnerPosition, neighborLearning, learningRate, distanceVector);	// function that updates strongUnits in the cluster

					if ( enableLearningPred )
						KohonenFeatureMapPred::updatePredictionSynapsis(false, unitsWinnerPosition, 1, 0.05, distanceVector);	// function that updates predictionSynapsis in the cluster

					//KohonenFeatureMapPred::updateStrongUnits(unitsWinnerPosition, 1, 0.05, distanceVector);	// function that updates strongUnits in the cluster
				//}

				KohonenFeatureMapPred::setLastWinner(unitsWinnerPosition);	// lastWinner is set to the current value of unitsWinnerPosition
			}
		}
		else										// if no unit is inside the radio determined by neighborhoodValue
		{
			KohonenFeatureMap::setUnitsVector(units);				// sets the vector of units in the map. In this situation, no unit is firing.
			KohonenFeatureMapPred::firingOutput();					// computes the firing outputs based in the vector of units in this map
			auxiliaryVariableDouble = 0;						// sets auxiliaryVariableDouble to zero because there is no belief in the winner

			//if ( learning )
			//{
				//KohonenFeatureMapPred::updatePredictionSynapsis(false, unitsWinnerPosition, neighborLearning, learningRate, distanceVector);	// updates predictionSynapsis in the cluster

				//KohonenFeatureMapPred::updateStrongUnits(unitsWinnerPosition, neighborLearning, learningRate, distanceVector);	// function that updates strongUnits in the cluster

				if ( enableLearningPred )
					KohonenFeatureMapPred::updatePredictionSynapsis(false, unitsWinnerPosition, 1, 0.05, distanceVector);	// function that updates predictionSynapsis in the cluster

				//KohonenFeatureMapPred::updateStrongUnits(unitsWinnerPosition, 1, 0.05, distanceVector);	// function that updates strongUnits in the cluster
			//}

			KohonenFeatureMapPred::setLastWinner(unitsWinnerPosition);		// the winner is not firing, but lastWinner is set to unitsWinnerPosition
		}
	}
	else											// if firing is false
	{
		KohonenFeatureMap::setUnitsVector(units);					// sets the vector of units in the map. In this situation, no unit is firing
		KohonenFeatureMapPred::firingOutput();						// computes the firing outputs based in the vector of units in this map
		unitsWinnerPosition = 0;							// sets unitsWinnerPosition to zero
		auxiliaryVariableDouble = 0;							// sets auxiliaryVariableDouble to zero and
		KohonenFeatureMapPred::setLastWinner(numberOfUnits+1);				// sets lastWinner to numberOfUnits+1
	}

	fire.winner = unitsWinnerPosition;
	fire.active = activeState;
	fire.believe = auxiliaryVariableDouble;
	fire.neighbor = neighborhoodValue;

	free(input);										// frees the space used by the vector to which input points
	free(unitsPointer);									// frees the space used be the vector to which unitsPointer points
	free(weightsMatrixPointer);								// frees the space used by the array to which weightsMatrixPointer points
	free(units);										// frees the space used for units
	free(distanceVector);									// frees the space used for the array distanceVector
	return fire;										// returns the units winner position
} // end function firingFunction


// function to set the firing output vector based in the firing of the units array
void	KohonenFeatureMapPred::firingOutput()
{
	int	i, j, index;
	int	arrayDim, inputDim, unitsDim, outputDim;
	int	base, activeUnits;
	int	*coordinates;
	bool	*units;
	double	exponent;
	double	auxiliaryDoubleVariable;
	double	*outputs;

	arrayDim = KohonenFeatureMap::getUnitsArrayDimensionality();				// gets the dimensionality of the array of units

	outputDim = KohonenFeatureMap::getOutputDimensionality();				// gets the dimensionality of the output vector
	outputs = (double*)calloc(outputDim, sizeof(double));					// reserves memory for the outputs

	unitsDim = KohonenFeatureMap::getUnitsDimensionality();					// gets the units dimensionalities
	units = KohonenFeatureMap::getUnitsVector();						// gets the array of units in the map

	exponent = log10(unitsDim)/arrayDim;
	base = (int)(pow(10,exponent)+0.01);							// gets the base in: unitsDim = base^arrayDim

	coordinates = (int*)calloc(arrayDim, sizeof(int));					// reserves space to be used by the method KohonenFeatureMap::unitsArrayMapping for the coordinates of the units array corresponding to the location of the unit, and = (int*)calloc(dimensionality, sizeof(int));

	activeUnits = 0;
	for (i = 0; i < unitsDim; i++)
	{
		if ( *(units + i) )								// if the unit number i+1 is active
		{
			activeUnits = activeUnits + 1;						// adds one to activeUnits
			KohonenFeatureMap::unitsArrayMapping(i+1, coordinates);			// gets a vector with the array coordinates corresponding to the otherPosition
			for (j = 0; j < arrayDim; j++ )
			{
				*(outputs + j) = *(outputs + j) + ((double)(*(coordinates + j)))/((double)base);
			}
		}
	}

	if ( activeUnits > 0 && activeUnits <= 1 )						// if there are more than zero and less than five active units
	{
		for (j = 0; j < arrayDim; j++ )							// takes the average of all the outputs
		{
			*(outputs + j) = *(outputs + j)/((double)activeUnits);
		}
		setOutputVector(outputs);							// sets the output vector
	}
	else											// if not;
	{
		free(outputs);									// frees outputs vector
		outputs = NULL;
		outputs = (double*)calloc(outputDim, sizeof(double));				// reserves memory for outputs again, and
		setOutputVector(outputs);							// resets the output vector
	}

	free(outputs);
	free(units);
	free(coordinates);
} // end function firingOutput


// function to set the last winner unit in the cluster
void	KohonenFeatureMapPred::setLastWinner( int winner )
{
	lastWinner = winner;								// sets the last winner in the cluster
} // end function setLastWinner


// function to get the last winner unit in the cluster
int	KohonenFeatureMapPred::getLastWinner()
{
	return lastWinner;								// returns the last winner in the cluster
} // end function getLastWinner


// function to reserve space for the prediction synaptic array in the cluster
void	KohonenFeatureMapPred::reservPredictionSynapsis()
{
	int number;

	number = KohonenFeatureMapPred::getUnitsDimensionality();			// gets the number of units in the cluster

	predictionSynapsis = (double*)calloc(number*number+number, sizeof(double));	// reserves space for predictionSynapsis in the cluster
} // end function reservPredictionSynapsis


// function to set the prediction synaptic array in the cluster
void	KohonenFeatureMapPred::setPredictionSynapsis( double *array )
{
	int i, j, number;

	number = KohonenFeatureMapPred::getUnitsDimensionality();			// gets the number of units in the cluster

	for (i = 0; i < number+1; i++)
		for (j = 0; j < number; j++)
			*((predictionSynapsis + i*number) + j) = *((array + i*number) + j);	// stores array in the predictionSynapsis array
} // end function setPredictionSynapsis


// function to get the prediction synaptic array in the cluster
double	*KohonenFeatureMapPred::getPredictionSynapsis()
{
	int i, j, number;
	double *array;

	number = KohonenFeatureMapPred::getUnitsDimensionality();			// gets the number of units in the cluster

	array = (double*)calloc(number*number+number, sizeof(double));			// reserves space for array

	for (i = 0; i < number+1; i++)
		for (j = 0; j < number; j++)
			*((array + i*number) + j) = *((predictionSynapsis + i*number) + j);	// stores predictionSynapsis in array

	return array;									// return array's pointer
} // end function getPredictionSynapsis


// function to update predictionSynapsis in the cluster
void	KohonenFeatureMapPred::updatePredictionSynapsis( bool predictionFault, int winner, double neighborParameter, double learningRate, double *dist )
{
	int	j, number;
	int	oldWinner;
	double	neighborhoodValue, update, accum = 0;

	number = KohonenFeatureMapPred::getUnitsDimensionality();				// gets the number of units in the cluster
	oldWinner = KohonenFeatureMapPred::getLastWinner();					// gets the last winner in the cluster

	if ( predictionFault )
	{
		for (j = 0; j < number; j++)
		{
			//neighborhoodValue = 0.001*KohonenFeatureMapPred::predictiveNeighborhoodFunction(true, neighborParameter, winner, (j+1));
			neighborhoodValue = KohonenFeatureMapPred::predictiveNeighborhoodFunction(true, neighborParameter, winner, (j+1));

			update = 5*learningRate*(neighborhoodValue)*(*(dist+j));
			*((predictionSynapsis + (oldWinner-1)*number) + j) = *((predictionSynapsis + (oldWinner-1)*number) + j) + update;	// updates *((predictionSynapsis + (oldWinner-1)*number) + j)

			if ( *((predictionSynapsis + (oldWinner-1)*number) + j) < 0 )
				*((predictionSynapsis + (oldWinner-1)*number) + j) = 0;

			accum += *((predictionSynapsis + (oldWinner-1)*number) + j);

			//if ( *((predictionSynapsis + (oldWinner-1)*number) + j) > 100 )
			//	*((predictionSynapsis + (oldWinner-1)*number) + j) = 100;
		}
	}
	else
	{
		for (j = 0; j < number; j++)
		{
			//neighborhoodValue = 0.001*(KohonenFeatureMapPred::predictiveNeighborhoodFunction(false, neighborParameter, winner, (j+1)) - 0.001);
			neighborhoodValue = KohonenFeatureMapPred::predictiveNeighborhoodFunction(false, neighborParameter, winner, (j+1)) - 0.001;

			//if ( neighborhoodValue < 0 )
			//	neighborhoodValue = 0.1*neighborhoodValue;

			update = learningRate*(neighborhoodValue)*(*(dist+j));
			*((predictionSynapsis + (oldWinner-1)*number) + j) = *((predictionSynapsis + (oldWinner-1)*number) + j) + update;	// updates *((predictionSynapsis + (oldWinner-1)*number) + j)

			if ( *((predictionSynapsis + (oldWinner-1)*number) + j) < 0 )
				*((predictionSynapsis + (oldWinner-1)*number) + j) = 0;

			accum += *((predictionSynapsis + (oldWinner-1)*number) + j);

			//if ( *((predictionSynapsis + (oldWinner-1)*number) + j) > 100 )
			//	*((predictionSynapsis + (oldWinner-1)*number) + j) = 100;

		}
	}

	if ( accum > 100 )
	{
		for (j = 0; j < number; j++)
			*((predictionSynapsis + (oldWinner-1)*number) + j) = 100 * *((predictionSynapsis + (oldWinner-1)*number) + j)/accum;			// normalize the predictionSynapsis
	}

} // end function updatePredictionSynapsis


// function to get a specific prediction value
double	KohonenFeatureMapPred::getPrediction( int unit )
{
	int	j, number;
	int	oldWinner;
	double	prediction;

	number = KohonenFeatureMapPred::getUnitsDimensionality();				// gets the number of units in the cluster
	oldWinner = KohonenFeatureMapPred::getLastWinner();					// gets the last winner in the cluster

	prediction = *((predictionSynapsis + (oldWinner-1)*number) + (unit-1));			// this is the prediction done

	return	prediction;
} // end function getPrediction


// function to compute the predictive neighborhood value in the lateral interaction between units in the array for learning process
double	KohonenFeatureMapPred::predictiveNeighborhoodFunction( bool predictionFault, double widthParameter, int winnerPosition, int otherPosition )
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

	if ( predictionFault )
	{
		return exp(-(pow((double)distance, (double)2)) / (2*widthParameter));				// returns the neighborhood value (Gauss bell)
	}
	else
	{
		return 2/(sqrt(3*widthParameter)*pow(M_PI,(double)0.25)) * (1 - (pow(distance,(double)2))/(pow(widthParameter,(double)2))) * exp(-(pow((double)distance, (double)2)) / (2*pow(widthParameter,(double)2)));		// returns the neighborhood value (mexican hat)
	}
} // end function predictiveNeighborhoodFunction


// function to reserve space for strongUnits vector in the cluster
void	KohonenFeatureMapPred::reservStrongUnits()
{
	int number;

	number = KohonenFeatureMapPred::getUnitsDimensionality();			// gets the number of units in the cluster

	strongUnits = (double*)calloc(number, sizeof(double));				// reserves space for strongUnits in the cluster
} // end function reservStrongUnits


// function to set setStrongUnits vector in the cluster
void	KohonenFeatureMapPred::setStrongUnits( double *vector )
{
	int i, number;

	number = KohonenFeatureMapPred::getUnitsDimensionality();			// gets the number of units in the cluster

	for (i = 0; i < number; i++)
		*(strongUnits + i) = *(vector + i);					// stores vector in the strongUnits vector
} // end function setStrongUnits


// function to get strongUnits vector in the cluster
double	*KohonenFeatureMapPred::getStrongUnits()
{
	int i, number;
	double *vector;

	number = KohonenFeatureMapPred::getUnitsDimensionality();			// gets the number of units in the cluster

	vector = (double*)calloc(number, sizeof(double));				// reserves space for vector

	for (i = 0; i < number; i++)
		*(vector + i) = *(strongUnits + i);					// stores strongUnits in vector

	return vector;									// return vector's pointer
} // end function getStrongUnits


// function to update strongUnits in the cluster
void	KohonenFeatureMapPred::updateStrongUnits( int winner, double neighborParameter, double learningRate, double *dist )
{
	int	i, number;
	int	oldWinner;
	double	neighborhoodValue, update;
	double	*vector;

	number = KohonenFeatureMapPred::getUnitsDimensionality();				// gets the number of units in the cluster

	vector = KohonenFeatureMapPred::getStrongUnits();					// gets strongUnits vector

	for (i = 0; i < number; i++)
	{
		neighborhoodValue = KohonenFeatureMapPred::strongNeighborhoodFunction(neighborParameter, winner, (i+1));

		if ( neighborhoodValue < 0 )
			neighborhoodValue = 0.1*neighborhoodValue;

		update = learningRate*(neighborhoodValue)*(*(dist+i));
		*(vector + i) = *(vector + i) + update;						// updates *(vector + i)

		if ( *(vector + i) < 0 )
			*(vector + i) = 0;
	}

	KohonenFeatureMapPred::setStrongUnits(vector);						// sets strongUnits vector
	free(vector);										// frees vector
} // end function updateStrongUnits


// function to compute the strong units neighborhood value in the lateral interaction between units in the array for learning process
double	KohonenFeatureMapPred::strongNeighborhoodFunction( double widthParameter, int winnerPosition, int otherPosition )
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

	return 2/(sqrt(3*widthParameter)*pow(M_PI,(double)0.25)) * (1 - (pow(distance,(double)2))/(pow(widthParameter,(double)2))) * exp(-(pow((double)distance, (double)2)) / (2*pow(widthParameter,(double)2)));		// returns the neighborhood value (mexican hat)
} // end function strongNeighborhoodFunction































