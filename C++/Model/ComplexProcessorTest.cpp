// Example program
// This main program test how ComplexProcessor class works
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <omp.h>

#include "ComplexProcessor.h"				// include definition of class ComplexProcessor
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/OctaveInterface.h"
#include "../Libraries/Model/MatlabInterface.h"
#include "../Libraries/Model/Templates.h"
#include "../Libraries/Model/DataTypes.h"
#include "../Libraries/Model/Constants.h"
#include "../Libraries/Model/Random.h"
#include "../Libraries/Model/Timer.h"

using namespace std;

bool	big_endianness;

int main()
{
// Initialize the MPI environment.
MPI::Init_thread(MPI_THREAD_MULTIPLE);
std::size_t	iterations = 5;
std::size_t	numberOfInputs, inputDim = 1100, dim = 15, numberOfLinks = 20;
regularLayerParameters	parameters;

// These are the regularColumn parameters 
parameters.enableLearning = true;
parameters.distalSensitivity = false;
parameters.activationHomeostasis = true;
parameters.proximalInformationThreshold = 0.5;
parameters.distalInformationThreshold = 0.5;
parameters.sparsity = 0.99;
parameters.learning.enableProximalLearning = true;
parameters.learning.enableDistalLearning = true;
parameters.learning.synapticHomeostasis = true;
parameters.learning.proximalLearningRate = 0.1;
parameters.learning.proximalNeighborhood = 0.05;
parameters.learning.plasticity = 0.1;
parameters.learning.spikeTimeDependentSynapticPlasticity = true;
parameters.learning.distalLearningRate = 0.001;

double		proximalPotentialPercentage = 0.1, distalPotentialPercentage = 0.1, sparsity = 0.01;
std::string     str;
std::string     STR;
std::array<double,2>		weightLimits = {SYNAPTIC_DECREMENT,SYNAPTIC_INCREMENT};
std::vector<std::size_t>	dimensions = {dim,dim};
std::vector<std::size_t>	dynamicUnits(numberOfLinks,std::accumulate(dimensions.begin(),
									   dimensions.end(),
									   1, std::multiplies<std::size_t>()));

twodvector<std::size_t>		proximalInputs;
threedvector<std::size_t>	distalInputs;

std::cout << "This is a test performed on a ComplexProcessor object instance" << std::endl;
std::cout << "Loading the input" << std::endl;
ifstream infile; 
infile.open("../../Octave/inputs.mat", ios::in | std::ifstream::binary);

bool	check_proximal = false;
bool	check_distal = false;
if (ENABLE_MATLAB_COMPATIBILITY) {
	big_endianness = load_the_header(infile);
	auto	array_structure = check_next_data_structure(infile, big_endianness);
	while ( array_structure.more_data ) {
		STR = "proximalInputs";
		if ( array_structure.name.compare(STR) == 0 ) {
			load_cell_array_to_multidimensional_vector(array_structure, proximalInputs, infile, big_endianness);
			check_proximal = true;
		}

		STR = "distalInputs";
		if ( array_structure.name.compare(STR) == 0 ) {
			load_cell_array_to_multidimensional_vector(array_structure, distalInputs, infile, big_endianness);
			check_distal = true;
		}

		array_structure = check_next_data_structure(infile,big_endianness);
	}
}
else {
	while ( std::getline(infile, str) ) {
		STR = "# name: proximalInputs";
		if ( str.compare(STR) == 0 ) {
			load_cell_to_multidimensional_vector(proximalInputs, infile);
			check_proximal = true;
		}

		STR = "# name: distalInputs";
		if ( str.compare(STR) == 0 ) {
			load_cell_to_multidimensional_vector(distalInputs, infile);
			check_distal = true;
		}
	}
}


assert(check_proximal == true);
assert(check_distal == true);

std::cout << "Input loaded.\n";

assert(proximalInputs.size() == distalInputs.size());
numberOfInputs = proximalInputs.size();

std::cout << "Creatin regularColumn object.\n";
ComplexProcessor regularColumn(dimensions, inputDim, proximalPotentialPercentage, distalPotentialPercentage, sparsity, dynamicUnits, weightLimits);
std::cout << "regularColumn object created.\n";

std::cout << "Processing data.\n";
std::vector<std::size_t>	lastActiveIndexes; 
timer::MarkStartEvent("ComplexProcessor Test");
for ( std::size_t it = 0; it < iterations; it++ ) {
	for ( std::size_t row = 0; row < numberOfInputs; row++ ) {
		parameters.learning.proximalLearningRate = 0.9 * std::pow((0.01/0.9),(double)(it*numberOfInputs + row)/
										     (double)numberOfInputs*iterations);
		parameters.learning.proximalNeighborhood = 5 *	std::pow(0.01,(double)(it*numberOfInputs + row)/
									      (double)numberOfInputs*iterations);

		responseInfo		response;
		if ( parameters.enableLearning ) {
			if ( parameters.learning.enableProximalLearning ) {
				response = regularColumn.learningRule(parameters.learning.proximalLearningRate,
								      parameters.learning.proximalNeighborhood,
								      parameters.learning.plasticity,
								      proximalInputs[row],
								      PROXIMAL_SYNAPTIC_THRESHOLD,
								      parameters.activationHomeostasis,
								      true);
				
				regularColumn.homeostasis(true,
							  parameters.learning.synapticHomeostasis,
							  parameters.activationHomeostasis,
							  PROXIMAL_SYNAPTIC_THRESHOLD);
			}
			else {
				response = regularColumn.getResponse(proximalInputs[row]);

				regularColumn.homeostasis(false,
							  parameters.learning.synapticHomeostasis,
							  parameters.activationHomeostasis,
							  PROXIMAL_SYNAPTIC_THRESHOLD);
			}
			std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
							       std::accumulate(dimensions.begin(),
									       dimensions.end(),
									       1, std::multiplies<std::size_t>());	

			std::vector<std::size_t>	activeIndexes;
			activeIndexes = regularColumn.Activate(response,
								distalInputs[row],
								numberOfExcitedUnits,
								parameters.sparsity,
								true);

			if ( parameters.learning.enableDistalLearning ) {
				if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
							  std::accumulate(dimensions.begin(),
									  dimensions.end(), 1,
									  std::multiplies<std::size_t>()) ) {
					regularColumn.Update(activeIndexes,
							     distalInputs[row],
							     true, DISTAL_SYNAPTIC_THRESHOLD,
							     parameters.learning.distalLearningRate*BUSTING);
				}
				else {
					regularColumn.Update(activeIndexes,
							     distalInputs[row],
							     true, DISTAL_SYNAPTIC_THRESHOLD,
							     parameters.learning.distalLearningRate);
				}
				
				if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
					regularColumn.Update(lastActiveIndexes,
							     distalInputs[row],
							     false, DISTAL_SYNAPTIC_THRESHOLD,
							     parameters.learning.distalLearningRate);

					lastActiveIndexes = activeIndexes;
				}						
			}
		}
		else {
			response = regularColumn.getResponse(proximalInputs[row]);
			
			regularColumn.homeostasis(false,
						  parameters.learning.synapticHomeostasis,
						  parameters.activationHomeostasis,
						  PROXIMAL_SYNAPTIC_THRESHOLD);
			
			std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
							       std::accumulate(dimensions.begin(),
									       dimensions.end(),
									       1, std::multiplies<std::size_t>());	

			std::vector<std::size_t>	activeIndexes;
			activeIndexes = regularColumn.Activate(response,
								distalInputs[row],
								numberOfExcitedUnits,
								parameters.sparsity);
		}
	}
}

for ( std::size_t it = 0; it < iterations; it++ ) {
	for ( std::size_t row = 0; row < numberOfInputs; row++ ) {
		parameters.learning.proximalLearningRate = 0.1 * std::pow((0.01/0.9),(double)(it*numberOfInputs + row)/
										     (double)numberOfInputs*iterations);
		parameters.learning.proximalNeighborhood = 0.5 *std::pow(0.01,(double)(it*numberOfInputs + row)/
									      (double)numberOfInputs*iterations);

		responseInfo		response;
		if ( parameters.enableLearning ) {
			if ( parameters.learning.enableProximalLearning ) {
				response = regularColumn.learningRule(parameters.learning.proximalLearningRate,
								      parameters.learning.proximalNeighborhood,
								      parameters.learning.plasticity,
								      proximalInputs[row],
								      PROXIMAL_SYNAPTIC_THRESHOLD,
								      parameters.activationHomeostasis,
								      true);
				
				regularColumn.homeostasis(true,
							  parameters.learning.synapticHomeostasis,
							  parameters.activationHomeostasis,
							  PROXIMAL_SYNAPTIC_THRESHOLD);
			}
			else {
				response = regularColumn.getResponse(proximalInputs[row]);

				regularColumn.homeostasis(false,
							  parameters.learning.synapticHomeostasis,
							  parameters.activationHomeostasis,
							  PROXIMAL_SYNAPTIC_THRESHOLD);
			}
			std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
							       std::accumulate(dimensions.begin(),
									       dimensions.end(),
									       1, std::multiplies<std::size_t>());	

			std::vector<std::size_t>	activeIndexes;
			activeIndexes = regularColumn.Activate(response,
								distalInputs[row],
								numberOfExcitedUnits,
								parameters.sparsity,
								true);

			if ( parameters.learning.enableDistalLearning ) {
				if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
							  std::accumulate(dimensions.begin(),
									  dimensions.end(), 1,
									  std::multiplies<std::size_t>()) ) {
					regularColumn.Update(activeIndexes,
							     distalInputs[row],
							     true, DISTAL_SYNAPTIC_THRESHOLD,
							     parameters.learning.distalLearningRate*BUSTING);
				}
				else {
					regularColumn.Update(activeIndexes,
							     distalInputs[row],
							     true, DISTAL_SYNAPTIC_THRESHOLD,
							     parameters.learning.distalLearningRate);
				}
				
				if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
					regularColumn.Update(lastActiveIndexes,
							     distalInputs[row],
							     false, DISTAL_SYNAPTIC_THRESHOLD,
							     parameters.learning.distalLearningRate);

					lastActiveIndexes = activeIndexes;
				}						
			}
		}
		else {
			response = regularColumn.getResponse(proximalInputs[row]);
			
			regularColumn.homeostasis(false,
						  parameters.learning.synapticHomeostasis,
						  parameters.activationHomeostasis,
						  PROXIMAL_SYNAPTIC_THRESHOLD);
			
			std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
							       std::accumulate(dimensions.begin(),
									       dimensions.end(),
									       1, std::multiplies<std::size_t>());	

			std::vector<std::size_t>	activeIndexes;
			activeIndexes = regularColumn.Activate(response,
								distalInputs[row],
								numberOfExcitedUnits,
								parameters.sparsity);
		}
	}
}

parameters.learning.proximalLearningRate = 0.1*(0.01/0.9);
parameters.learning.proximalNeighborhood = 0.5*0.01;
for ( std::size_t it = 0; it < iterations; it++ ) {
	for ( std::size_t row = 0; row < numberOfInputs; row++ ) {
		responseInfo		response;
		if ( parameters.enableLearning ) {
			if ( parameters.learning.enableProximalLearning ) {
				response = regularColumn.learningRule(parameters.learning.proximalLearningRate,
								      parameters.learning.proximalNeighborhood,
								      parameters.learning.plasticity,
								      proximalInputs[row],
								      PROXIMAL_SYNAPTIC_THRESHOLD,
								      parameters.activationHomeostasis,
								      true);
				
				regularColumn.homeostasis(true,
							  parameters.learning.synapticHomeostasis,
							  parameters.activationHomeostasis,
							  PROXIMAL_SYNAPTIC_THRESHOLD);
			}
			else {
				response = regularColumn.getResponse(proximalInputs[row]);

				regularColumn.homeostasis(false,
							  parameters.learning.synapticHomeostasis,
							  parameters.activationHomeostasis,
							  PROXIMAL_SYNAPTIC_THRESHOLD);
			}
			std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
							       std::accumulate(dimensions.begin(),
									       dimensions.end(),
									       1, std::multiplies<std::size_t>());	

			std::vector<std::size_t>	activeIndexes;
			activeIndexes = regularColumn.Activate(response,
								distalInputs[row],
								numberOfExcitedUnits,
								parameters.sparsity,
								true);

			if ( parameters.learning.enableDistalLearning ) {
				if ( activeIndexes.size() > PREDICTION_FAULT_THRESHOLD_PERCENTAGE*
							  std::accumulate(dimensions.begin(),
									  dimensions.end(), 1,
									  std::multiplies<std::size_t>()) ) {
					regularColumn.Update(activeIndexes,
							     distalInputs[row],
							     true, DISTAL_SYNAPTIC_THRESHOLD,
							     parameters.learning.distalLearningRate*BUSTING);
				}
				else {
					regularColumn.Update(activeIndexes,
							     distalInputs[row],
							     true, DISTAL_SYNAPTIC_THRESHOLD,
							     parameters.learning.distalLearningRate);
				}
				
				if ( parameters.learning.spikeTimeDependentSynapticPlasticity ) {
					regularColumn.Update(lastActiveIndexes,
							     distalInputs[row],
							     false, DISTAL_SYNAPTIC_THRESHOLD,
							     parameters.learning.distalLearningRate);

					lastActiveIndexes = activeIndexes;
				}						
			}
		}
		else {
			response = regularColumn.getResponse(proximalInputs[row]);
			
			regularColumn.homeostasis(false,
						  parameters.learning.synapticHomeostasis,
						  parameters.activationHomeostasis,
						  PROXIMAL_SYNAPTIC_THRESHOLD);
			
			std::size_t	numberOfExcitedUnits = PROXIMAL_ACTIVATION_PERCENTAGE*
							       std::accumulate(dimensions.begin(),
									       dimensions.end(),
									       1, std::multiplies<std::size_t>());	

			std::vector<std::size_t>	activeIndexes;
			activeIndexes = regularColumn.Activate(response,
								distalInputs[row],
								numberOfExcitedUnits,
								parameters.sparsity);
		}
	}
}
timer::MarkEndEvent("ComplexProcessor Test");

std::cout << "Saving Object.\n";
std::string	processorIdentifier = "regularColumn", fileName = "regularColumn_Status";

// stream to put the formated data to be saved in the file
std::stringstream	outstream;

// file preamble.
if (ENABLE_MATLAB_COMPATIBILITY) {
	save_the_header(outstream);
}
else {
	outstream << "# This is a file created by ComplexProcessorTest function from," << endl;
	outstream << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
	outstream << "# Author: Dematties Dario Jesus." << endl;

	outstream << "\n\n" << endl;
}
	
regularColumn.saveComplexProcessorStatus(processorIdentifier,outstream);



// open a file in write mode.
ofstream outfile;
outfile.open("../../Octave/" + fileName + ".mat", ios::out | ios::trunc);

// write the file with the stream
outfile << outstream.rdbuf();

// close the opened file.
outfile.close();
std::cout << "Object saved.\n";

// Print time statistics
timer::PrintLog(std::cout, MPI_COMM_WORLD);

MPI::Finalize();
return	0;
}
