#include <iostream>
#include <fstream>
#include <time.h>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <unistd.h>

#include "../Libraries/OctaveInterface.h"
#include "../Libraries/Templates.h"

#include "Structs.h"
#include "Layer.h"

using namespace std;

int main()
{
srand(time(NULL));

// general layer parameters initialization
// for Hierarchy_1_Layer_4 object
layerParameters	parameters_H1_L4;
parameters_H1_L4.sparsity = 0.99;
parameters_H1_L4.numberOfBestResponses = 10;
parameters_H1_L4.enableLearning = true;
parameters_H1_L4.distalSensitivity = false;
parameters_H1_L4.activationHomeostasis = false;
parameters_H1_L4.proximalInformationThreshold = 0.1;
parameters_H1_L4.distalInformationThreshold = 0.5;
parameters_H1_L4.activationRadius = 0.8;
parameters_H1_L4.selectionCriteria = "BMUsRanking";
// learning parameters
parameters_H1_L4.learning.enableProximalLearning = true;
parameters_H1_L4.learning.enableDistalLearning = true;
parameters_H1_L4.learning.plasticity = 0.01;
parameters_H1_L4.learning.spikeTimeDependentSynapticPlasticity = true;
parameters_H1_L4.learning.synapticHomeostasis = false;
parameters_H1_L4.learning.proximalLearningRate = 0.9;
parameters_H1_L4.learning.proximalNeighborhood = 5;
parameters_H1_L4.learning.distalLearningRate = 1;
parameters_H1_L4.learning.distalNeighborhood = 5;
parameters_H1_L4.learning.distalNeighborhoodFunction = "mex";
parameters_H1_L4.learning.proximalInformationThreshold = 0.2;
parameters_H1_L4.learning.limitsLearningRate = 0.1;

// general layer parameters initialization
// for Hierarchy_1_Layer_23 object
layerParameters	parameters_H1_L23;
parameters_H1_L23.sparsity = 0.99;
parameters_H1_L23.numberOfBestResponses = 10;
parameters_H1_L23.enableLearning = true;
parameters_H1_L23.distalSensitivity = false;
parameters_H1_L23.activationHomeostasis = true;
parameters_H1_L23.proximalInformationThreshold = 0.5;
parameters_H1_L23.distalInformationThreshold = 0.5;
parameters_H1_L23.activationRadius = 1.0;
parameters_H1_L23.selectionCriteria = "BMUsRanking";
// learning parameters
parameters_H1_L23.learning.enableProximalLearning = true;
parameters_H1_L23.learning.enableDistalLearning = true;
parameters_H1_L23.learning.plasticity = 0.01;
parameters_H1_L23.learning.spikeTimeDependentSynapticPlasticity = true;
parameters_H1_L23.learning.synapticHomeostasis = true;
parameters_H1_L23.learning.proximalLearningRate = 0.9;
parameters_H1_L23.learning.proximalNeighborhood = 5;
parameters_H1_L23.learning.distalLearningRate = 1;
parameters_H1_L23.learning.distalNeighborhood = 5;
parameters_H1_L23.learning.distalNeighborhoodFunction = "mex";
parameters_H1_L23.learning.proximalInformationThreshold = 0.15;
parameters_H1_L23.learning.limitsLearningRate = 0.1;

// general layer parameters initialization
// for Hierarchy_2_Layer_4 object
layerParameters	parameters_H2_L4;
parameters_H2_L4.sparsity = 0.98;
parameters_H2_L4.numberOfBestResponses = 10;
parameters_H2_L4.enableLearning = true;
parameters_H2_L4.distalSensitivity = false;
parameters_H2_L4.activationHomeostasis = false;
parameters_H2_L4.proximalInformationThreshold = 0.2;
parameters_H2_L4.distalInformationThreshold = 0.5;
parameters_H2_L4.activationRadius = 0.8;
parameters_H2_L4.selectionCriteria = "BMUsRanking";
// learning parameters
parameters_H2_L4.learning.enableProximalLearning = true;
parameters_H2_L4.learning.enableDistalLearning = true;
parameters_H2_L4.learning.plasticity = 0.01;
parameters_H2_L4.learning.spikeTimeDependentSynapticPlasticity = false;
parameters_H2_L4.learning.synapticHomeostasis = false;
parameters_H2_L4.learning.proximalLearningRate = 0.9;
parameters_H2_L4.learning.proximalNeighborhood = 5;
parameters_H2_L4.learning.distalLearningRate = 1;
parameters_H2_L4.learning.distalNeighborhood = 5;
parameters_H2_L4.learning.distalNeighborhoodFunction = "mex";
parameters_H2_L4.learning.proximalInformationThreshold = 0.2;
parameters_H2_L4.learning.limitsLearningRate = 0.1;

//*

std::size_t	startLearningAt = 2, finishLearningAt = 2;

assert( startLearningAt <= finishLearningAt );

std::vector<layerResponse>	input;		// this is the input to the model

if ( startLearningAt <= 1 ) {
	// This is Hierarchical Layer 1
	// Cortical Layer 4

	size_t	iterations = 2;

	std::vector<int>	afferentArrayDimensionality = {5,128};
	std::vector<int>	apicalArrayDimensionality = {1,1};
	std::vector<int>	columnsArrayDimensionality = {9,9};

	std::vector<int>	afferentReceptiveField = {2,63};
	double			afferentPercentage = 0.03;
	bool			afferentWrapAround = true;

	std::vector<int>	lateralProximalReceptiveField = {-1,-1};
	double			lateralProximalPercentage = 0.09;
	bool			lateralProximalWrapAround = true;

	std::vector<int>	lateralDistalReceptiveField = {4,4};
	double			lateralDistalPercentage = 0.09;
	bool			lateralDistalWrapAround = true;

	std::vector<int>	apicalReceptiveField = {-1,-1};
	double			apicalPercentage = 0.5;
	bool			apicalWrapAround = false;

	int			iterationNum = 0;

	std::vector<int>	populationsArrayDimensionality;
	populationsArrayDimensionality = {15,15};

	std::vector<int>	afferentPopulationsArrayDimensionality;
	afferentPopulationsArrayDimensionality = {1};

	std::vector<int>	apicalPopulationsArrayDimensionality;
	apicalPopulationsArrayDimensionality = {1};

	int	temporalGatheringAfferentValue = 1;
	bool	binaryProcessing = false;
	std::string	name = "Hierarchy_1_Layer_4";

	// This is for the first creation of the layer and its training
	// This belongs to the Hierarchical layer 1 Cortical layer 4
	std::cout << "\n";
	std::cout << "Creating object layer 4 in hierarchy 1.";
	std::cout << "\n";
	Layer	Hierarchy_1_Layer_4(afferentArrayDimensionality, apicalArrayDimensionality, columnsArrayDimensionality,
		      	afferentReceptiveField, afferentPercentage, afferentWrapAround,
			lateralProximalReceptiveField, lateralProximalPercentage, lateralProximalWrapAround,
		      	lateralDistalReceptiveField, lateralDistalPercentage, lateralDistalWrapAround,
		      	apicalReceptiveField, apicalPercentage, apicalWrapAround,
		      	iterationNum,
			temporalGatheringAfferentValue,
			populationsArrayDimensionality,
			afferentPopulationsArrayDimensionality,
			apicalPopulationsArrayDimensionality,
			binaryProcessing);

	system("clear");
	std::cout << "\n";
	std::cout << "Layer 4 in hierarchy 1 created.";
	std::cout << "\n";
	
	if ( finishLearningAt >= 1 ) {
		// Loads the inputs to the first hierarchical layer
		std::cout << "\n";
		std::cout << "Loading Input data.";
		std::cout << "\n";
		input = Hierarchy_1_Layer_4.loadInputs("inputs");
		std::cout << "\n";
		std::cout << "Input data loaded.";
		std::cout << "\n";

		layerResponse	lateral_H1_L4, apical_H1_L4;

		// lateral input initialization
		// for Hierarchy_1_Layer_4 object
		auto	numberOfColumns = std::accumulate(columnsArrayDimensionality.begin(),
							  columnsArrayDimensionality.end(),
							  1, std::multiplies<int>());
		lateral_H1_L4.currentIndexes.resize(numberOfColumns);
		lateral_H1_L4.synchronization.resize(numberOfColumns);
		lateral_H1_L4.information.resize(numberOfColumns);

		for ( int column = 0; column < numberOfColumns; column++ ) {
			lateral_H1_L4.currentIndexes[column].push_back(-1);
			lateral_H1_L4.synchronization[column] = true;
			lateral_H1_L4.information[column] = false;
		}

		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_1_Layer_4 processing loaded data.\n";
		std::cout << "Learning process is being carried out.\n";
		std::cout << "This corresponds to the first training process.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";

		// Processes the input information through  the layer
		// Hierarchy_1_Layer_4
		auto	timeSteps = input.size();

		// This is the a coarse training
		// for a first neuronal configuration
		for ( size_t iteration = 0; iteration < iterations; iteration++ ) {
			for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {

				lateral_H1_L4 = Hierarchy_1_Layer_4.computeResponse(input[timeStep], lateral_H1_L4, apical_H1_L4, parameters_H1_L4);

				parameters_H1_L4.learning.proximalLearningRate = 0.9 *
							std::pow((0.01/0.9),(timeStep+iteration*timeSteps)/(timeSteps*iterations));
				parameters_H1_L4.learning.proximalNeighborhood = 5 *
							std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));

				parameters_H1_L4.learning.distalLearningRate = 1 *
							std::pow((0.01),(timeStep+iteration*timeSteps)/(timeSteps*iterations));
				parameters_H1_L4.learning.distalNeighborhood = 5 *
							std::pow(0.2,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
			}
		}

		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_1_Layer_4 processing loaded data.\n";
		std::cout << "Learning process is being carried out.\n";
		std::cout << "This corresponds to the second training process.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";

		parameters_H1_L4.learning.proximalLearningRate = 0.1;
		parameters_H1_L4.learning.proximalNeighborhood = 0.5;
		parameters_H1_L4.learning.distalLearningRate = 0.1;
		parameters_H1_L4.learning.distalNeighborhood = 1;
		for ( size_t iteration = 0; iteration < iterations; iteration++ ) {
			for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {

				lateral_H1_L4 = Hierarchy_1_Layer_4.computeResponse(input[timeStep], lateral_H1_L4, apical_H1_L4, parameters_H1_L4);

				parameters_H1_L4.learning.proximalLearningRate = 0.1 *
							std::pow((0.01/0.9),(timeStep+iteration*timeSteps)/(timeSteps*iterations));
				parameters_H1_L4.learning.proximalNeighborhood = 0.5 *
							std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));

				parameters_H1_L4.learning.distalLearningRate = 0.1 *
							std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
			}
		}

		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_1_Layer_4 processing loaded data.\n";
		std::cout << "Learning process is being carried out.\n";
		std::cout << "This corresponds to the third training process.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";

		parameters_H1_L4.learning.proximalLearningRate = 0.1*(0.01/0.9);
		parameters_H1_L4.learning.proximalNeighborhood = 0.5*0.01;
		parameters_H1_L4.learning.distalLearningRate = 0.1*0.01;
		parameters_H1_L4.learning.distalNeighborhood = 1;
		for ( size_t iteration = 0; iteration < iterations*2; iteration++ ) {
			for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {
				lateral_H1_L4 = Hierarchy_1_Layer_4.computeResponse(input[timeStep], lateral_H1_L4, apical_H1_L4, parameters_H1_L4);
			}
		}
	}

	if ( parameters_H1_L4.enableLearning &&
	(parameters_H1_L4.learning.enableProximalLearning || parameters_H1_L4.learning.enableDistalLearning) ) {
		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Saving Hierarchy_1_Layer_4 status data.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";
		// Saves the layer status
		Hierarchy_1_Layer_4.saveLayerStatus(name);
		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_1_Layer_4 status data saved.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";
	}
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

if ( startLearningAt <= 2 ) {

	size_t	iterations = 2;

	system("clear");
	// This belongs to the Hierarchical layer 1 Cortical layer 4
	std::string	name = "Hierarchy_1_Layer_4";
	std::cout << "\n";
	std::cout << "Loading object layer 4 in hierarchy 1.";
	std::cout << "\n";
	Layer	Hierarchy_1_Layer_4(name);

	std::cout << "\n";
	std::cout << "Layer 4 in hierarchy 1 loaded.";
	std::cout << "\n";

	if ( input.size() == 0 ) {
		// Loads the inputs to the first hierarchical layer
		std::cout << "\n";
		std::cout << "Loading Input data.";
		std::cout << "\n";
		input = Hierarchy_1_Layer_4.loadInputs("inputs");
		std::cout << "\n";
		std::cout << "Input data loaded.";
		std::cout << "\n";
	}

	// desable learning process
	// for Hierarchy_1_Layer_4 object
	parameters_H1_L4.enableLearning = false;

	layerResponse	lateral_H1_L4, apical_H1_L4;

	// lateral input initialization
	// for Hierarchy_1_Layer_4 object
	std::string	str;
	std::string	STR;

	// open a file in read mode.
	ifstream infile;
	name = "Hierarchy_1_Layer_4";
	infile.open("../../Octave/" + name + ".mat", ios::in | std::ifstream::binary);

	std::vector<int>	columnsArrayDimensionality;
	while ( std::getline(infile, str) ) {

		STR = "# name: columnsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(columnsArrayDimensionality, infile);

	}
	// close the opened file.
	infile.close();

	auto	numberOfColumns = std::accumulate(columnsArrayDimensionality.begin(),
						  columnsArrayDimensionality.end(),
						  1, std::multiplies<int>());
	lateral_H1_L4.currentIndexes.resize(numberOfColumns);
	lateral_H1_L4.synchronization.resize(numberOfColumns);
	lateral_H1_L4.information.resize(numberOfColumns);

	for ( int column = 0; column < numberOfColumns; column++ ) {
		lateral_H1_L4.currentIndexes[column].push_back(-1);
		lateral_H1_L4.synchronization[column] = true;
		lateral_H1_L4.information[column] = false;
	}

	// This is Hierarchical Layer 1
	// Cortical Layer 23
	std::vector<int>	afferentArrayDimensionality = {9,9};
	std::vector<int>	apicalArrayDimensionality = {1,1};
				columnsArrayDimensionality = {9,9};

	std::vector<int>	afferentReceptiveField = {4,4};
	double			afferentPercentage = 0.09;
	bool			afferentWrapAround = true;

	std::vector<int>	lateralProximalReceptiveField = {4,4};
	double			lateralProximalPercentage = 0.09;
	bool			lateralProximalWrapAround = true;

	std::vector<int>	lateralDistalReceptiveField = {4,4};
	double			lateralDistalPercentage = 0.09;
	bool			lateralDistalWrapAround = true;

	std::vector<int>	apicalReceptiveField = {-1,-1};
	double			apicalPercentage = 0.5;
	bool			apicalWrapAround = false;

	int			iterationNum = 0;

	std::vector<int>	populationsArrayDimensionality;
	populationsArrayDimensionality = {15,15};

	std::vector<int>	afferentPopulationsArrayDimensionality;
	afferentPopulationsArrayDimensionality = {15,15};

	std::vector<int>	apicalPopulationsArrayDimensionality;
	apicalPopulationsArrayDimensionality = {1};

	int	temporalGatheringAfferentValue = 1;
	bool	binaryProcessing = true;
	name = "Hierarchy_1_Layer_23";

	// This is for the first creation of the layer and its training
	// This belongs to the Hierarchical layer 1 Cortical layer 23
	std::cout << "\n";
	std::cout << "Creating object layer 23 in hierarchy 1.";
	std::cout << "\n";
	Layer	Hierarchy_1_Layer_23(afferentArrayDimensionality, apicalArrayDimensionality, columnsArrayDimensionality,
		      	afferentReceptiveField, afferentPercentage, afferentWrapAround,
			lateralProximalReceptiveField, lateralProximalPercentage, lateralProximalWrapAround,
		      	lateralDistalReceptiveField, lateralDistalPercentage, lateralDistalWrapAround,
		      	apicalReceptiveField, apicalPercentage, apicalWrapAround,
		      	iterationNum,
			temporalGatheringAfferentValue,
			populationsArrayDimensionality,
			afferentPopulationsArrayDimensionality,
			apicalPopulationsArrayDimensionality,
			binaryProcessing);

	std::cout << "\n";
	std::cout << "Layer 23 in hierarchy 1 created.";
	std::cout << "\n";

	if ( finishLearningAt >= 2 ) {
		layerResponse	lateral_H1_L23, apical_H1_L23;

		// lateral input initialization
		// for the Hierarchy_1_Layer_23 object
		numberOfColumns = std::accumulate(columnsArrayDimensionality.begin(),
							  columnsArrayDimensionality.end(),
							  1, std::multiplies<int>());
		lateral_H1_L23.currentIndexes.resize(numberOfColumns);
		lateral_H1_L23.synchronization.resize(numberOfColumns);
		lateral_H1_L23.information.resize(numberOfColumns);

		for ( int column = 0; column < numberOfColumns; column++ ) {
			lateral_H1_L23.currentIndexes[column].push_back(-1);
			lateral_H1_L23.synchronization[column] = true;
			lateral_H1_L23.information[column] = false;
		}

		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_1_Layer_23 processing loaded data.\n";
		std::cout << "Learning process is being carried out.\n";
		std::cout << "This corresponds to the first training process.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";

		// Processes the input information through  the layer
		// Hierarchy_1_Layer_23
		auto	timeSteps = input.size();

		// This is the a coarse training
		// for a first neuronal configuration
		for ( size_t iteration = 0; iteration < iterations; iteration++ ) {
			for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {
				lateral_H1_L4 = Hierarchy_1_Layer_4.computeResponse(input[timeStep], lateral_H1_L4, apical_H1_L4, parameters_H1_L4);
				lateral_H1_L23 = Hierarchy_1_Layer_23.computeResponse(lateral_H1_L4, lateral_H1_L23, apical_H1_L23, parameters_H1_L23);

				parameters_H1_L23.learning.proximalLearningRate = 0.9 *
							std::pow((0.01/0.9),(timeStep+iteration*timeSteps)/(timeSteps*iterations));
				parameters_H1_L23.learning.proximalNeighborhood = 5 *
							std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));

				parameters_H1_L23.learning.distalLearningRate = 1 *
							std::pow((0.01),(timeStep+iteration*timeSteps)/(timeSteps*iterations));
				parameters_H1_L23.learning.distalNeighborhood = 5 *
							std::pow(0.2,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
			}
		}

		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_1_Layer_23 processing loaded data.\n";
		std::cout << "Learning process is being carried out.\n";
		std::cout << "This corresponds to the second training process.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";

		parameters_H1_L23.learning.proximalLearningRate = 0.1;
		parameters_H1_L23.learning.proximalNeighborhood = 0.5;
		parameters_H1_L23.learning.distalLearningRate = 0.1;
		parameters_H1_L23.learning.distalNeighborhood = 1;
		for ( size_t iteration = 0; iteration < iterations; iteration++ ) {
			for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {
				lateral_H1_L4 = Hierarchy_1_Layer_4.computeResponse(input[timeStep], lateral_H1_L4, apical_H1_L4, parameters_H1_L4);
				lateral_H1_L23 = Hierarchy_1_Layer_23.computeResponse(lateral_H1_L4, lateral_H1_L23, apical_H1_L23, parameters_H1_L23);

				parameters_H1_L23.learning.proximalLearningRate = 0.1 *
							std::pow((0.01/0.9),(timeStep+iteration*timeSteps)/(timeSteps*iterations));
				parameters_H1_L23.learning.proximalNeighborhood = 0.5 *
							std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));

				parameters_H1_L23.learning.distalLearningRate = 0.1 *
							std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
			}
		}

		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_1_Layer_23 processing loaded data.\n";
		std::cout << "Learning process is being carried out.\n";
		std::cout << "This corresponds to the third training process.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";

		parameters_H1_L23.learning.proximalLearningRate = 0.1*(0.01/0.9);
		parameters_H1_L23.learning.proximalNeighborhood = 0.5*0.01;
		parameters_H1_L23.learning.distalLearningRate = 0.1*0.01;
		parameters_H1_L23.learning.distalNeighborhood = 1;
		for ( size_t iteration = 0; iteration < iterations*2; iteration++ ) {
			for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {
				lateral_H1_L4 = Hierarchy_1_Layer_4.computeResponse(input[timeStep], lateral_H1_L4, apical_H1_L4, parameters_H1_L4);
				lateral_H1_L23 = Hierarchy_1_Layer_23.computeResponse(lateral_H1_L4, lateral_H1_L23, apical_H1_L23, parameters_H1_L23);
			}
		}
	}

	if ( parameters_H1_L23.enableLearning &&
	(parameters_H1_L23.learning.enableProximalLearning || parameters_H1_L23.learning.enableDistalLearning) ) {
		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Saving Hierarchy_1_Layer_23 status data.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";
		// Saves the layer status
		Hierarchy_1_Layer_23.saveLayerStatus(name);
		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_1_Layer_23 status data saved.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";
	}
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

if ( startLearningAt <= 3 ) {

	size_t	iterations = 2;

	system("clear");
	// This belongs to the Hierarchical layer 1 Cortical layer 4
	std::string	name = "Hierarchy_1_Layer_4";
	std::cout << "\n";
	std::cout << "Loading object layer 4 in hierarchy 1.";
	std::cout << "\n";
	Layer	Hierarchy_1_Layer_4(name);

	std::cout << "\n";
	std::cout << "Layer 4 in hierarchy 1 loaded.";
	std::cout << "\n";

	if ( input.size() == 0 ) {
		// Loads the inputs to the first hierarchical layer
		std::cout << "\n";
		std::cout << "Loading Input data.";
		std::cout << "\n";
		input = Hierarchy_1_Layer_4.loadInputs("inputs");
		std::cout << "\n";
		std::cout << "Input data loaded.";
		std::cout << "\n";
	}

	// desable learning process
	// for Hierarchy_1_Layer_4 object
	parameters_H1_L4.enableLearning = false;

	layerResponse	lateral_H1_L4, apical_H1_L4;

	// lateral input initialization
	// for Hierarchy_1_Layer_4 object
	std::string	str;
	std::string	STR;

	// open a file in read mode.
	ifstream infile;
	name = "Hierarchy_1_Layer_4";
	infile.open("../../Octave/" + name + ".mat", ios::in | std::ifstream::binary);

	std::vector<int>	columnsArrayDimensionality;
	while ( std::getline(infile, str) ) {

		STR = "# name: columnsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(columnsArrayDimensionality, infile);

	}
	// close the opened file.
	infile.close();

	auto	numberOfColumns = std::accumulate(columnsArrayDimensionality.begin(),
						  columnsArrayDimensionality.end(),
						  1, std::multiplies<int>());
	lateral_H1_L4.currentIndexes.resize(numberOfColumns);
	lateral_H1_L4.synchronization.resize(numberOfColumns);
	lateral_H1_L4.information.resize(numberOfColumns);

	for ( int column = 0; column < numberOfColumns; column++ ) {
		lateral_H1_L4.currentIndexes[column].push_back(-1);
		lateral_H1_L4.synchronization[column] = true;
		lateral_H1_L4.information[column] = false;
	}


	// This belongs to the Hierarchical layer 1 Cortical layer 23
	name = "Hierarchy_1_Layer_23";
	std::cout << "\n";
	std::cout << "Loading object layer 23 in hierarchy 1.";
	std::cout << "\n";
	Layer	Hierarchy_1_Layer_23(name);

	std::cout << "\n";
	std::cout << "Layer 23 in hierarchy 1 loaded.";
	std::cout << "\n";

	// desable learning process
	// for Hierarchy_1_Layer_23 object
	parameters_H1_L23.enableLearning = false;

	layerResponse	lateral_H1_L23, apical_H1_L23;

	// lateral input initialization
	// for Hierarchy_1_Layer_23 object

	// open a file in read mode.
	name = "Hierarchy_1_Layer_23";
	infile.open("../../Octave/" + name + ".mat", ios::in | std::ifstream::binary);

	while ( std::getline(infile, str) ) {

		STR = "# name: columnsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(columnsArrayDimensionality, infile);

	}
	// close the opened file.
	infile.close();

	numberOfColumns = std::accumulate(columnsArrayDimensionality.begin(),
						  columnsArrayDimensionality.end(),
						  1, std::multiplies<int>());
	lateral_H1_L23.currentIndexes.resize(numberOfColumns);
	lateral_H1_L23.synchronization.resize(numberOfColumns);
	lateral_H1_L23.information.resize(numberOfColumns);

	for ( int column = 0; column < numberOfColumns; column++ ) {
		lateral_H1_L23.currentIndexes[column].push_back(-1);
		lateral_H1_L23.synchronization[column] = true;
		lateral_H1_L23.information[column] = false;
	}

	// This is Hierarchical Layer 2
	// Cortical Layer 4
	std::vector<int>	afferentArrayDimensionality = {9,9};
	std::vector<int>	apicalArrayDimensionality = {1,1};
				columnsArrayDimensionality = {9,9};

	std::vector<int>	afferentReceptiveField = {0,0};
	double			afferentPercentage = 1.0;
	bool			afferentWrapAround = true;

	std::vector<int>	lateralProximalReceptiveField = {-1,-1};
	double			lateralProximalPercentage = 0.5;
	bool			lateralProximalWrapAround = true;

	std::vector<int>	lateralDistalReceptiveField = {0,0};
	double			lateralDistalPercentage = 1.0;
	bool			lateralDistalWrapAround = true;

	std::vector<int>	apicalReceptiveField = {-1,-1};
	double			apicalPercentage = 0.5;
	bool			apicalWrapAround = false;

	int			iterationNum = 0;

	std::vector<int>	populationsArrayDimensionality;
	populationsArrayDimensionality = {15,15};

	std::vector<int>	afferentPopulationsArrayDimensionality;
	afferentPopulationsArrayDimensionality = {10,10};

	std::vector<int>	apicalPopulationsArrayDimensionality;
	apicalPopulationsArrayDimensionality = {1};

	int	temporalGatheringAfferentValue = 4;
	bool	binaryProcessing = false;
	name = "Hierarchy_2_Layer_4";

	// This is for the first creation of the layer and its training
	// This belongs to the Hierarchical layer 1 Cortical layer 23
	std::cout << "\n";
	std::cout << "Creating object layer 4 in hierarchy 2.";
	std::cout << "\n";
	Layer	Hierarchy_2_Layer_4(afferentArrayDimensionality, apicalArrayDimensionality, columnsArrayDimensionality,
		      	afferentReceptiveField, afferentPercentage, afferentWrapAround,
			lateralProximalReceptiveField, lateralProximalPercentage, lateralProximalWrapAround,
		      	lateralDistalReceptiveField, lateralDistalPercentage, lateralDistalWrapAround,
		      	apicalReceptiveField, apicalPercentage, apicalWrapAround,
		      	iterationNum,
			temporalGatheringAfferentValue,
			populationsArrayDimensionality,
			afferentPopulationsArrayDimensionality,
			apicalPopulationsArrayDimensionality,
			binaryProcessing);

	std::cout << "\n";
	std::cout << "Layer 4 in hierarchy 2 created.";
	std::cout << "\n";

	if ( finishLearningAt >= 3 ) {
		layerResponse	lateral_H2_L4, apical_H2_L4;

		// lateral input initialization
		// for the Hierarchy_2_Layer_4 object
		numberOfColumns = std::accumulate(columnsArrayDimensionality.begin(),
							  columnsArrayDimensionality.end(),
							  1, std::multiplies<int>());
		lateral_H2_L4.currentIndexes.resize(numberOfColumns);
		lateral_H2_L4.synchronization.resize(numberOfColumns);
		lateral_H2_L4.information.resize(numberOfColumns);

		for ( int column = 0; column < numberOfColumns; column++ ) {
			lateral_H2_L4.currentIndexes[column].push_back(-1);
			lateral_H2_L4.synchronization[column] = true;
			lateral_H2_L4.information[column] = false;
		}

		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_2_Layer_4 processing loaded data.\n";
		std::cout << "Learning process is being carried out.\n";
		std::cout << "This corresponds to the first training process.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";

		// Processes the input information through  the layer
		// Hierarchy_2_Layer_4
		auto	timeSteps = input.size();

		// This is the a coarse training
		// for a first neuronal configuration
		for ( size_t iteration = 0; iteration < iterations; iteration++ ) {
			for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {
				lateral_H1_L4 = Hierarchy_1_Layer_4.computeResponse(input[timeStep], lateral_H1_L4, apical_H1_L4, parameters_H1_L4);
				lateral_H1_L23 = Hierarchy_1_Layer_23.computeResponse(lateral_H1_L4, lateral_H1_L23, apical_H1_L23, parameters_H1_L23);

				auto	input_H2_L4 = Hierarchy_2_Layer_4.temporalGatherer(lateral_H1_L23);
				lateral_H2_L4 = Hierarchy_2_Layer_4.computeResponse(input_H2_L4, lateral_H2_L4, apical_H2_L4, parameters_H2_L4);

				parameters_H2_L4.learning.proximalLearningRate = 0.9 *
							std::pow((0.01/0.9),(timeStep+iteration*timeSteps)/(timeSteps*iterations));
				parameters_H2_L4.learning.proximalNeighborhood = 5 *
							std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));

				parameters_H2_L4.learning.distalLearningRate = 1 *
							std::pow((0.01),(timeStep+iteration*timeSteps)/(timeSteps*iterations));
				parameters_H2_L4.learning.distalNeighborhood = 5 *
							std::pow(0.2,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
			}
		}

		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_2_Layer_4 processing loaded data.\n";
		std::cout << "Learning process is being carried out.\n";
		std::cout << "This corresponds to the second training process.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";

		parameters_H2_L4.learning.proximalLearningRate = 0.1;
		parameters_H2_L4.learning.proximalNeighborhood = 0.5;
		parameters_H2_L4.learning.distalLearningRate = 0.1;
		parameters_H2_L4.learning.distalNeighborhood = 1;
		for ( size_t iteration = 0; iteration < iterations; iteration++ ) {
			for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {
				lateral_H1_L4 = Hierarchy_1_Layer_4.computeResponse(input[timeStep], lateral_H1_L4, apical_H1_L4, parameters_H1_L4);
				lateral_H1_L23 = Hierarchy_1_Layer_23.computeResponse(lateral_H1_L4, lateral_H1_L23, apical_H1_L23, parameters_H1_L23);

				auto	input_H2_L4 = Hierarchy_2_Layer_4.temporalGatherer(lateral_H1_L23);
				lateral_H2_L4 = Hierarchy_2_Layer_4.computeResponse(input_H2_L4, lateral_H2_L4, apical_H2_L4, parameters_H2_L4);

				parameters_H2_L4.learning.proximalLearningRate = 0.1 *
							std::pow((0.01/0.9),(timeStep+iteration*timeSteps)/(timeSteps*iterations));
				parameters_H2_L4.learning.proximalNeighborhood = 0.5 *
							std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));

				parameters_H2_L4.learning.distalLearningRate = 0.1 *
							std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
			}
		}

		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_2_Layer_4 processing loaded data.\n";
		std::cout << "Learning process is being carried out.\n";
		std::cout << "This corresponds to the third training process.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";

		parameters_H2_L4.learning.proximalLearningRate = 0.1*(0.01/0.9);
		parameters_H2_L4.learning.proximalNeighborhood = 0.5*0.01;
		parameters_H2_L4.learning.distalLearningRate = 0.1*0.01;
		parameters_H2_L4.learning.distalNeighborhood = 1;
		for ( size_t iteration = 0; iteration < iterations*2; iteration++ ) {
			for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {
				lateral_H1_L4 = Hierarchy_1_Layer_4.computeResponse(input[timeStep], lateral_H1_L4, apical_H1_L4, parameters_H1_L4);
				lateral_H1_L23 = Hierarchy_1_Layer_23.computeResponse(lateral_H1_L4, lateral_H1_L23, apical_H1_L23, parameters_H1_L23);
				auto	input_H2_L4 = Hierarchy_2_Layer_4.temporalGatherer(lateral_H1_L23);
				lateral_H2_L4 = Hierarchy_2_Layer_4.computeResponse(input_H2_L4, lateral_H2_L4, apical_H2_L4, parameters_H2_L4);
			}
		}
	}

	if ( parameters_H2_L4.enableLearning &&
	(parameters_H2_L4.learning.enableProximalLearning || parameters_H2_L4.learning.enableDistalLearning) ) {
		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Saving Hierarchy_2_Layer_4 status data.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";
		// Saves the layer status
		Hierarchy_2_Layer_4.saveLayerStatus(name);
		std::cout << "\n";
		std::cout << "******************************************************************************************\n";
		std::cout << "Hierarchy_2_Layer_4 status data saved.\n";
		std::cout << "******************************************************************************************";
		std::cout << "\n";
	}
}//*/






























/*
// This is for the use of the previously created layers and its training

std::size_t	finishProcessingAt = 2;

system("clear");
size_t	iterations = 1;
bool	learn = false;

// This belongs to the Hierarchical layer 1 Cortical layer 4
std::cout << "\n";
std::cout << "Loading object layer 4 in hierarchy 1.";
std::cout << "\n";
std::string	name = "Hierarchy_1_Layer_4";
Layer	Hierarchy_1_Layer_4(name);
std::cout << "\n";
std::cout << "Layer 4 in hierarchy 1 loaded.";
std::cout << "\n";

std::cout << "\n";
std::cout << "Loading input from object layer 4 in hierarchy 1.";
std::cout << "\n";
name = "inputs";
auto	input = Hierarchy_1_Layer_4.loadInputs(name);
std::cout << "\n";
std::cout << "input loaded.";
std::cout << "\n";

// This belongs to the Hierarchical layer 1 Cortical layer 23
std::cout << "\n";
std::cout << "Loading object layer 23 in hierarchy 1.";
std::cout << "\n";
name = "Hierarchy_1_Layer_23";
Layer	Hierarchy_1_Layer_23(name);
std::cout << "\n";
std::cout << "Layer 23 in hierarchy 1 loaded.";
std::cout << "\n";

// This belongs to the Hierarchical layer 2 Cortical layer 4
std::cout << "\n";
std::cout << "Loading object layer 4 in hierarchy 2.";
std::cout << "\n";
name = "Hierarchy_2_Layer_4";
Layer	Hierarchy_2_Layer_4(name);
std::cout << "\n";
std::cout << "Layer 4 in hierarchy 2 loaded.";
std::cout << "\n";



layerResponse	lateral_H1_L4, apical_H1_L4;

// lateral input initialization
// for Hierarchy_1_Layer_4 object
std::string	str;
std::string	STR;

// open a file in read mode.
ifstream infile;
name = "Hierarchy_1_Layer_4";
infile.open("../../Octave/" + name + ".mat", ios::in | std::ifstream::binary);

std::vector<int>	columnsArrayDimensionality;
while ( std::getline(infile, str) ) {

	STR = "# name: columnsArrayDimensionality";
	if ( str.compare(STR) == 0 )
		load_matrix_to_vector(columnsArrayDimensionality, infile);

}
// close the opened file.
infile.close();

auto	numberOfColumns = std::accumulate(columnsArrayDimensionality.begin(),
					  columnsArrayDimensionality.end(),
					  1, std::multiplies<int>());
lateral_H1_L4.currentIndexes.resize(numberOfColumns);
lateral_H1_L4.synchronization.resize(numberOfColumns);
lateral_H1_L4.information.resize(numberOfColumns);

for ( int column = 0; column < numberOfColumns; column++ ) {
	lateral_H1_L4.currentIndexes[column].push_back(-1);
	lateral_H1_L4.synchronization[column] = true;
	lateral_H1_L4.information[column] = false;
}




layerResponse	lateral_H1_L23, apical_H1_L23;

// lateral input initialization
// for Hierarchy_1_Layer_23 object

// open a file in read mode.
name = "Hierarchy_1_Layer_23";
infile.open("../../Octave/" + name + ".mat", ios::in | std::ifstream::binary);

while ( std::getline(infile, str) ) {

	STR = "# name: columnsArrayDimensionality";
	if ( str.compare(STR) == 0 )
		load_matrix_to_vector(columnsArrayDimensionality, infile);

}
// close the opened file.
infile.close();

numberOfColumns = std::accumulate(columnsArrayDimensionality.begin(),
					  columnsArrayDimensionality.end(),
					  1, std::multiplies<int>());
lateral_H1_L23.currentIndexes.resize(numberOfColumns);
lateral_H1_L23.synchronization.resize(numberOfColumns);
lateral_H1_L23.information.resize(numberOfColumns);

for ( int column = 0; column < numberOfColumns; column++ ) {
	lateral_H1_L23.currentIndexes[column].push_back(-1);
	lateral_H1_L23.synchronization[column] = true;
	lateral_H1_L23.information[column] = false;
}



layerResponse	lateral_H2_L4, apical_H2_L4;

// lateral input initialization
// for Hierarchy_2_Layer_4 object

// open a file in read mode.
name = "Hierarchy_2_Layer_4";
infile.open("../../Octave/" + name + ".mat", ios::in | std::ifstream::binary);

while ( std::getline(infile, str) ) {

	STR = "# name: columnsArrayDimensionality";
	if ( str.compare(STR) == 0 )
		load_matrix_to_vector(columnsArrayDimensionality, infile);

}
// close the opened file.
infile.close();

numberOfColumns = std::accumulate(columnsArrayDimensionality.begin(),
					  columnsArrayDimensionality.end(),
					  1, std::multiplies<int>());
lateral_H2_L4.currentIndexes.resize(numberOfColumns);
lateral_H2_L4.synchronization.resize(numberOfColumns);
lateral_H2_L4.information.resize(numberOfColumns);

for ( int column = 0; column < numberOfColumns; column++ ) {
	lateral_H2_L4.currentIndexes[column].push_back(-1);
	lateral_H2_L4.synchronization[column] = true;
	lateral_H2_L4.information[column] = false;
}





// general layer parameters initialization
// for the Hierarchy_1_Layer_4 object
parameters_H1_L4.enableLearning = learn;

parameters_H1_L4.learning.proximalLearningRate = 0.1*(0.01/0.9);
parameters_H1_L4.learning.proximalNeighborhood = 0.5*0.01;
parameters_H1_L4.learning.distalLearningRate = 0.1*0.01;
parameters_H1_L4.learning.distalNeighborhood = 1;

parameters_H1_L4.activationHomeostasis = false;
parameters_H1_L4.learning.synapticHomeostasis = false;

// general layer parameters initialization
// for the Hierarchy_1_Layer_23 object
parameters_H1_L23.enableLearning = learn;

parameters_H1_L23.learning.proximalLearningRate = 0.1*(0.01/0.9);
parameters_H1_L23.learning.proximalNeighborhood = 0.5*0.01;
parameters_H1_L23.learning.distalLearningRate = 0.1*0.01;
parameters_H1_L23.learning.distalNeighborhood = 1;

parameters_H1_L23.activationHomeostasis = false;
parameters_H1_L23.learning.synapticHomeostasis = false;

// general layer parameters initialization
// for the Hierarchy_2_Layer_4 object
parameters_H2_L4.enableLearning = learn;

parameters_H2_L4.learning.proximalLearningRate = 0.1*(0.01/0.9);
parameters_H2_L4.learning.proximalNeighborhood = 0.5*0.01;
parameters_H2_L4.learning.distalLearningRate = 0.1*0.01;
parameters_H2_L4.learning.distalNeighborhood = 1;

parameters_H2_L4.activationHomeostasis = false;
parameters_H2_L4.learning.synapticHomeostasis = false;




// Holds output information through
// hierwrchy 1 layer 4 processing
threedvector<int>	layerOutput_H1_L4;

// Holds output information through
// hierwrchy 1 layer 23 processing
threedvector<int>	layerOutput_H1_L23;

// Holds output information through
// hierwrchy 2 layer 4 processing
threedvector<int>	layerOutput_H2_L4;

std::cout << "\n";
std::cout << "Processing data.";
std::cout << "\n";
auto	timeSteps = input.size();
for ( size_t iteration = 0; iteration < iterations; iteration++ ) {
	for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {
		if ( finishProcessingAt >= 1 ) { 
			lateral_H1_L4 = Hierarchy_1_Layer_4.computeResponse(input[timeStep], lateral_H1_L4, apical_H1_L4, parameters_H1_L4);
			layerOutput_H1_L4.push_back(lateral_H1_L4.currentIndexes);
		}

		if ( finishProcessingAt >= 2 ) { 
			lateral_H1_L23 = Hierarchy_1_Layer_23.computeResponse(lateral_H1_L4, lateral_H1_L23, apical_H1_L23, parameters_H1_L23);
			layerOutput_H1_L23.push_back(lateral_H1_L23.currentIndexes);
		}

		if ( finishProcessingAt >= 3 ) { 
			auto	input_H2_L4 = Hierarchy_2_Layer_4.temporalGatherer(lateral_H1_L23);
			lateral_H2_L4 = Hierarchy_2_Layer_4.computeResponse(input_H2_L4, lateral_H2_L4, apical_H2_L4, parameters_H2_L4);
			layerOutput_H2_L4.push_back(lateral_H2_L4.currentIndexes);
		}
	}
}
layerOutput_H1_L4.shrink_to_fit();
layerOutput_H1_L23.shrink_to_fit();
layerOutput_H2_L4.shrink_to_fit();

// Saves the hierarchy 1 layer 4 status
if ( parameters_H1_L4.enableLearning )
	Hierarchy_1_Layer_4.saveLayerStatus("Hierarchy_1_Layer_4");

if ( !parameters_H1_L4.enableLearning && !parameters_H1_L23.enableLearning &&
     !parameters_H2_L4.enableLearning && finishProcessingAt >= 1) {
	// open a file in write mode.
	std::string	outputFileName = "outputLayer_H1_L4";
	ofstream outfile;
	outfile.open("../../Octave/" + outputFileName + ".mat", ios::out | ios::trunc);

	// file preamble.
	outfile << "# This is a file created by Test main tester program from," << endl;
	outfile << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
	outfile << "# Author: Dematties Dario Jesus." << endl;

	outfile << "\n\n" << endl;

	std::string	str;
	std::string	STR;

	// open a file in read mode.
	ifstream infile;
	name = "Hierarchy_1_Layer_4";
	infile.open("../../Octave/" + name + ".mat", ios::in | std::ifstream::binary);

	std::vector<int>	populationsArrayDimensionality;
	while ( std::getline(infile, str) ) {

		STR = "# name: columnsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(columnsArrayDimensionality, infile);

		STR = "# name: populationsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(populationsArrayDimensionality, infile);

	}
	// close the opened file.
	infile.close();

	// saves columnsArrayDimensionality
	save_vector_as_matrix("columnsArrayDimensionality", columnsArrayDimensionality, outfile);

	// saves populationsArrayDimensionality
	save_vector_as_matrix("populationsArrayDimensionality", populationsArrayDimensionality, outfile);

	// saves layerOutput
	std::cout << "\nsaving layerOutput_H1_L4\n";
	save_multidimensional_vector_as_cell("layerOutput", layerOutput_H1_L4, outfile);
	std::cout << "\nlayerOutput_H1_L4 saved\n";

	// close the opened file.
	outfile.close();
}

// Saves the hierarchy 1 layer 23 status
if ( parameters_H1_L23.enableLearning )
	Hierarchy_1_Layer_23.saveLayerStatus("Hierarchy_1_Layer_23");

if ( !parameters_H1_L23.enableLearning && !parameters_H1_L4.enableLearning &&
     !parameters_H2_L4.enableLearning  && finishProcessingAt >= 2 ) {
	// open a file in write mode.
	std::string	outputFileName = "outputLayer_H1_L23";
	ofstream outfile;
	outfile.open("../../Octave/" + outputFileName + ".mat", ios::out | ios::trunc);

	// file preamble.
	outfile << "# This is a file created by Test main tester program from," << endl;
	outfile << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
	outfile << "# Author: Dematties Dario Jesus." << endl;

	outfile << "\n\n" << endl;

	std::string	str;
	std::string	STR;

	// open a file in read mode.
	ifstream infile;
	name = "Hierarchy_1_Layer_23";
	infile.open("../../Octave/" + name + ".mat", ios::in | std::ifstream::binary);
	std::vector<int>	populationsArrayDimensionality;
	while ( std::getline(infile, str) ) {

		STR = "# name: columnsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(columnsArrayDimensionality, infile);

		STR = "# name: populationsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(populationsArrayDimensionality, infile);

	}
	// close the opened file.
	infile.close();

	// saves columnsArrayDimensionality
	save_vector_as_matrix("columnsArrayDimensionality", columnsArrayDimensionality, outfile);

	// saves populationsArrayDimensionality
	save_vector_as_matrix("populationsArrayDimensionality", populationsArrayDimensionality, outfile);

	// saves layerOutput
	std::cout << "\nsaving layerOutput_H1_L23\n";
	save_multidimensional_vector_as_cell("layerOutput", layerOutput_H1_L23, outfile);
	std::cout << "\nlayerOutput_H1_L23 saved\n";

	// close the opened file.
	outfile.close();
}

// Saves the hierarchy 1 layer 23 status
if ( parameters_H2_L4.enableLearning )
	Hierarchy_2_Layer_4.saveLayerStatus("Hierarchy_2_Layer_4");

if ( !parameters_H1_L4.enableLearning && !parameters_H1_L23.enableLearning &&
     !parameters_H2_L4.enableLearning && finishProcessingAt >= 3 ) {
	// open a file in write mode.
	std::string	outputFileName = "outputLayer_H2_L4";
	ofstream outfile;
	outfile.open("../../Octave/" + outputFileName + ".mat", ios::out | ios::trunc);

	// file preamble.
	outfile << "# This is a file created by Test main tester program from," << endl;
	outfile << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
	outfile << "# Author: Dematties Dario Jesus." << endl;

	outfile << "\n\n" << endl;

	std::string	str;
	std::string	STR;

	// open a file in read mode.
	ifstream infile;
	name = "Hierarchy_2_Layer_4";
	infile.open("../../Octave/" + name + ".mat", ios::in | std::ifstream::binary);
	std::vector<int>	populationsArrayDimensionality;
	while ( std::getline(infile, str) ) {

		STR = "# name: columnsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(columnsArrayDimensionality, infile);

		STR = "# name: populationsArrayDimensionality";
		if ( str.compare(STR) == 0 )
			load_matrix_to_vector(populationsArrayDimensionality, infile);

	}
	// close the opened file.
	infile.close();

	// saves columnsArrayDimensionality
	save_vector_as_matrix("columnsArrayDimensionality", columnsArrayDimensionality, outfile);

	// saves populationsArrayDimensionality
	save_vector_as_matrix("populationsArrayDimensionality", populationsArrayDimensionality, outfile);

	// saves layerOutput
	std::cout << "\nsaving layerOutput_H2_L4\n";
	save_multidimensional_vector_as_cell("layerOutput", layerOutput_H2_L4, outfile);
	std::cout << "\nlayerOutput_H2_L4 saved\n";

	// close the opened file.
	outfile.close();
}//*/







std::cout << '\a';




}
