#include <iostream>
#include <time.h>
#include <algorithm>

#include "Layer.h"

using namespace std;

int main()
{
srand(time(NULL));

std::vector<int>	afferentArrayDimensionality = {15,30};
std::vector<int>	apicalArrayDimensionality = {10,20};
std::vector<int>	columnsArrayDimensionality = {10,20};

std::vector<int>	afferentReceptiveField = {3,5};
double			afferentPercentage = 0.5;
bool			afferentWrapAround = false;

std::vector<int>	lateralProximalReceptiveField = {1,2};
double			lateralProximalPercentage = 0.5;
bool			lateralProximalWrapAround = false;

std::vector<int>	lateralDistalReceptiveField = {2,4};
double			lateralDistalPercentage = 0.5;
bool			lateralDistalWrapAround = false;

std::vector<int>	apicalReceptiveField = {2,4};
double			apicalPercentage = 0.5;
bool			apicalWrapAround = false;

int			iterationNum = 0;

auto	numberOfColumns = std::accumulate(columnsArrayDimensionality.begin(),
					columnsArrayDimensionality.end(),
					1, std::multiplies<int>());

auto	numberOfAfferentColumns = std::accumulate(afferentArrayDimensionality.begin(),
						afferentArrayDimensionality.end(),
						1, std::multiplies<int>());

auto	numberOfApicalColumns = std::accumulate(apicalArrayDimensionality.begin(),
						apicalArrayDimensionality.end(),
						1, std::multiplies<int>());

twodvector<int>	populationsArrayDimensionality;
populationsArrayDimensionality.resize(numberOfColumns);
for ( int column = 0; column < numberOfColumns; column++ )
	populationsArrayDimensionality[column] = {10,10};

twodvector<int>	afferentPopulationsArrayDimensionality;
afferentPopulationsArrayDimensionality.resize(numberOfAfferentColumns);
for ( int column = 0; column < numberOfAfferentColumns; column++ )
	afferentPopulationsArrayDimensionality[column] = {10,10};

twodvector<int>	apicalPopulationsArrayDimensionality;
apicalPopulationsArrayDimensionality.resize(numberOfApicalColumns);
for ( int column = 0; column < numberOfApicalColumns; column++ )
	apicalPopulationsArrayDimensionality[column] = {10,10};

Layer	MyLayer(afferentArrayDimensionality, apicalArrayDimensionality, columnsArrayDimensionality,
	      	afferentReceptiveField, afferentPercentage, afferentWrapAround,
		lateralProximalReceptiveField, lateralProximalPercentage, lateralProximalWrapAround,
	      	lateralDistalReceptiveField, lateralDistalPercentage, lateralDistalWrapAround,
	      	apicalReceptiveField, apicalPercentage, apicalWrapAround,
	      	iterationNum, 
		populationsArrayDimensionality,
		afferentPopulationsArrayDimensionality,
		apicalPopulationsArrayDimensionality);

std::string	name = "MyLayer";
MyLayer.saveLayerStatus(name);
}
