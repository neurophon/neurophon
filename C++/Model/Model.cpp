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

// File Name:		Model.cpp
// File Description:	Model member-function definitions. This file contains implementations of the
//			member functions prototyped in Model.h.


#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <mpi.h>

#include "../Libraries/Model/OctaveInterface.h"
#include "Model.h"

using namespace std;


// constructor that initializes an object instantiation of this class
Model::Model( std::string& folderName )
{
	Model::loadModelStatus(folderName);
} // end Model constructor


// run the model
void	Model::run( const std::string& folderName )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	regularLayerResponse				encoderLayerOutput;	// output from the encoder layer
	std::vector<regularLayerResponse>		regularLayerOutputs;	// outputs from the regular layers
	regularLayerOutputs.resize(_modelStructure.numberOfLayers);

	//MPI_Barrier(MPI::COMM_WORLD);
	if ( _modelStructure.encoderIncorporation ) {	// if there is encoder

		// Holds output information that comes from
		// the encoder layer and from the regular layers
		threedvector<std::size_t>	cumulativeEncoderLayerOutput;
		fourdvector<std::size_t>	cumulativeRegularLayerOutputs;
		cumulativeRegularLayerOutputs.resize(_modelStructure.numberOfLayers);

		if ( world_rank == 0 )
			std::cout << "\nIn this model we have encoder layer.\n";

		// then, loads the input information to feed the encoder layer
		if ( world_rank == 0 )
			std::cout << "\nLoading Encoder Layer Input data.\n";

		auto	encoderLayerInput = Model::loadEncoderInputs("inputs");	

		if ( world_rank == 0 )
			std::cout << "\nEncoder Layer Input data loaded.\n";

		//MPI_Barrier(MPI::COMM_WORLD);

		auto	timeSteps = encoderLayerInput.size();

		if ( world_rank == 0 )
			std::cout << "\nProcessing the Input data.\n";

		// starts to process the input data one time step at a time
		//MPI_Barrier(MPI::COMM_WORLD);
		for ( std::size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {

			// the data is processed by the encoder layer
			if ( timeStep == 0 ) { // in the first iteration there is no lateral nor apical information
				encoderLayerOutput = _encoderLayer.computeResponse(encoderLayerInput[timeStep],
										   _encoderLayerParameters);

			}
			else if ( _modelStructure.numberOfLayers > 0 ) { // if there is at least one regular layer, then there is apical information
				encoderLayerOutput = _encoderLayer.computeResponse(encoderLayerInput[timeStep],
										   encoderLayerOutput,
										   regularLayerOutputs[0],
										   _encoderLayerParameters);

			}
			else { // if there are no regular layers, then there is no apical information
				encoderLayerOutput = _encoderLayer.computeResponse(encoderLayerInput[timeStep],
										   encoderLayerOutput,
										   _encoderLayerParameters);

			}

			//MPI_Barrier(MPI::COMM_WORLD);
			// regular layers processes the data in turns
			// one regular layer at a time
			for ( std::size_t layer = 0;
					  layer < _modelStructure.numberOfLayers;
					  layer++ ) {

				if ( layer == 0 ) { // if layer is 0, information comes from encoder layer
					if ( timeStep == 0 ) { // in the first iteration there is no
					       					 // lateral information
										 // neither apical
						regularLayerOutputs[layer] =
						_regularLayers[layer].computeResponse(
								     encoderLayerOutput,
								     _regularLayerParameters[layer]);
					}
					else {
						if ( layer < _modelStructure.numberOfLayers-1 ) {
							regularLayerOutputs[layer] =
							_regularLayers[layer].computeResponse(
									     encoderLayerOutput,
									     regularLayerOutputs[layer],
									     regularLayerOutputs[layer+1],
									     _regularLayerParameters[layer]);
						}
						else { // for the last layer in the model there is no apical information
							regularLayerOutputs[layer] =
							_regularLayers[layer].computeResponse(
									     encoderLayerOutput,
									     regularLayerOutputs[layer],
									     _regularLayerParameters[layer]);
						}
					}
				}
				else {
					if ( timeStep == 0 ) { // in the first iteration there is no lateral information
						regularLayerOutputs[layer] =
						_regularLayers[layer].computeResponse(
								     regularLayerOutputs[layer-1],
								     _regularLayerParameters[layer]);
					}
					else {
						if ( layer < _modelStructure.numberOfLayers-1 ) {
							regularLayerOutputs[layer] =
							_regularLayers[layer].computeResponse(
									     regularLayerOutputs[layer-1],
									     regularLayerOutputs[layer],
									     regularLayerOutputs[layer+1],
									     _regularLayerParameters[layer]);
						}
						else { // for the last layer in the model there is no apical information
							regularLayerOutputs[layer] =
							_regularLayers[layer].computeResponse(
									     regularLayerOutputs[layer-1],
									     regularLayerOutputs[layer],
									     _regularLayerParameters[layer]);
						}
					}
				}
				//MPI_Barrier(MPI::COMM_WORLD);
			}
			// accumulates the encoder and regular layer outputs
			cumulativeEncoderLayerOutput.push_back(encoderLayerOutput.currentIndexes);
			for ( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ )
				cumulativeRegularLayerOutputs[layer].push_back(regularLayerOutputs[layer].currentIndexes);

			//MPI_Barrier(MPI::COMM_WORLD);
		}
		cumulativeEncoderLayerOutput.shrink_to_fit();
		for ( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ )
			cumulativeRegularLayerOutputs[layer].shrink_to_fit();

		Model::saveCumulativeEncoderLayerOutput(folderName, cumulativeEncoderLayerOutput);
		Model::saveCumulativeRegularLayerOutput(folderName, cumulativeRegularLayerOutputs);

		//MPI_Barrier(MPI::COMM_WORLD);
	}
	else {	// if there is no encoder layer
		// there must be at least one regular layer
		if ( !(_modelStructure.numberOfLayers > 0) ) {
			if ( world_rank == 0 ) {
				std::cout << "\nIn class Model, in function run:" << endl;
				std::cout << "_modelStructure.numberOfLayers > 0 was not satisfied." << endl;
			}
			//MPI_Barrier(MPI::COMM_WORLD);
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if ( world_rank == 0 )
			std::cout << "\nIn this model we have "
				  << _modelStructure.numberOfLayers
				  << " number of regular layers.\n";

		// loads the information to feed the first regular layer in the model
		if ( world_rank == 0 )
			std::cout << "\nLoading Regular Layer Input data.\n";

		auto	regularLayerInput = Model::loadRegularInputs("inputs");	

		if ( world_rank == 0 )
			std::cout << "\nRegular Layer Input data loaded.\n";

		auto	timeSteps = regularLayerInput.size();

		//MPI_Barrier(MPI::COMM_WORLD);

		// Holds output information that comes from
		// the regular layers
		fourdvector<std::size_t>	cumulativeRegularLayerOutputs;
		cumulativeRegularLayerOutputs.resize(_modelStructure.numberOfLayers);

		if ( world_rank == 0 )
			std::cout << "\nProcessing the Input data.\n";
		
		//MPI_Barrier(MPI::COMM_WORLD);
		// starts to process the input data one time step at a time
		for ( std::size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {

			// regular layers processes the data in turns
			// one regular layer at a time
			for ( std::size_t layer = 0;
					  layer < _modelStructure.numberOfLayers;
					  layer++ ) {

				if ( layer == 0 ) { // if layer is 0, information comes from regular layer input
					if ( timeStep == 0 ) { // in the first iteration there is no lateral information
						regularLayerOutputs[layer] =
						_regularLayers[layer].computeResponse(
								     regularLayerInput[timeStep],
								     _regularLayerParameters[layer]);
					}
					else {
						if ( layer < _modelStructure.numberOfLayers-1 ) {
							regularLayerOutputs[layer] =
							_regularLayers[layer].computeResponse(
									     regularLayerInput[timeStep],
									     regularLayerOutputs[layer],
									     regularLayerOutputs[layer+1],
									     _regularLayerParameters[layer]);
						}
						else { // for the last layer in the model there is no apical information
							regularLayerOutputs[layer] =
							_regularLayers[layer].computeResponse(
									     regularLayerInput[timeStep],
									     regularLayerOutputs[layer],
									     _regularLayerParameters[layer]);
						}
					}
				}
				else {
					if ( timeStep == 0 ) { // in the first iteration there is no lateral information
						regularLayerOutputs[layer] =
						_regularLayers[layer].computeResponse(
								     regularLayerOutputs[layer-1],
								     _regularLayerParameters[layer]);
					}
					else {
						if ( layer < _modelStructure.numberOfLayers-1 ) {
							regularLayerOutputs[layer] =
							_regularLayers[layer].computeResponse(
									     regularLayerOutputs[layer-1],
									     regularLayerOutputs[layer],
									     regularLayerOutputs[layer+1],
									     _regularLayerParameters[layer]);
						}
						else { // for the last layer in the model there is no apical information
							regularLayerOutputs[layer] =
							_regularLayers[layer].computeResponse(
									     regularLayerOutputs[layer-1],
									     regularLayerOutputs[layer],
									     _regularLayerParameters[layer]);
						}
					}
				}

				//MPI_Barrier(MPI::COMM_WORLD);
			}
			// accumulates the regular layer outputs
			if ( world_rank == 0 ) {
				for ( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ )
					cumulativeRegularLayerOutputs[layer].push_back(regularLayerOutputs[layer].currentIndexes);
			}
			//MPI_Barrier(MPI::COMM_WORLD);
		}
		if ( world_rank == 0 ) {
			for ( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ )
				cumulativeRegularLayerOutputs[layer].shrink_to_fit();

			Model::saveCumulativeRegularLayerOutput(folderName, cumulativeRegularLayerOutputs);
		}
		//MPI_Barrier(MPI::COMM_WORLD);
	}
	//MPI_Barrier(MPI::COMM_WORLD);
} // end function run 


// train the model
void	Model::train( const std::string& folderName,
		      const std::size_t iterations,
		      const std::size_t stages )
{
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

  	regularLayerResponse				encoderLayerOutput;	// output from the encoder layer
	std::vector<regularLayerResponse>		regularLayerOutputs;	// outputs from the regular layers
	regularLayerOutputs.resize(_modelStructure.numberOfLayers);

	//MPI_Barrier(MPI::COMM_WORLD);
	if ( _modelStructure.encoderIncorporation ) {	// if there is encoder
		// then, loads the input information to feed the encoder layer
		if ( world_rank == 0 )
			std::cout << "\nLoading Encoder Layer Input data.\n";

		auto	encoderLayerInput = Model::loadEncoderInputs("inputs");	

		if ( world_rank == 0 )
			std::cout << "\nEncoder Layer Input data loaded.\n";

		auto	timeSteps = encoderLayerInput.size();

		//MPI_Barrier(MPI::COMM_WORLD);
		// ask if the encoder layer is new
		if ( _modelStructure.newEncoder ) {
			// if the encoder is new, newLayerAt must be 0
			if ( !(_modelStructure.newLayerAt == 0) ) {
				if ( world_rank == 0 ) {
					std::cout << "\nIn Model class, in train function:" << endl;
					std::cout << "_modelStructure.newLayerAt == 0 is not satisfied" << endl;
				}

				//MPI_Barrier(MPI::COMM_WORLD);
				MPI_Abort(MPI::COMM_WORLD,1);
			}

			if ( world_rank == 0 )
				std::cout << "\nTraining process for the encoder layer.\n";

			// initializes auxiliary encoder layer parameters from the encoder layer parameters
			auto	auxiliaryEncoderParameters = _encoderLayerParameters;
			// enable proximal and distal learning mechanisms in the encoder layer
			auxiliaryEncoderParameters.enableLearning = true;
			auxiliaryEncoderParameters.learning.enableProximalLearning = true;
			auxiliaryEncoderParameters.learning.enableDistalLearning = true;
			// parameters gradually decreased for the learning process
			double	encoderInitialProximalLearningRate = 0.9;
			double	encoderInitialProximalNeighborhood = 5.0;
			double	encoderInitialDistalLearningRate = 1.0;

			//MPI_Barrier(MPI::COMM_WORLD);
			// these are the different stages in the training process for a new encoder layer
			for ( std::size_t stage = 0; stage <= stages+1; stage++ ) {
				if ( world_rank == 0 )
					std::cout << "\nEncoder Layer training stage number " << stage << ".\n";

				for ( std::size_t iteration = 0; iteration < iterations; iteration++ ) {
					for ( std::size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {

						if ( stage < stages ) {
							auxiliaryEncoderParameters.learning.proximalLearningRate = encoderInitialProximalLearningRate *
										std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
							auxiliaryEncoderParameters.learning.proximalNeighborhood = encoderInitialProximalNeighborhood *
										std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
							auxiliaryEncoderParameters.learning.distalLearningRate = encoderInitialDistalLearningRate *
										std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
						}
						else if ( iteration == 0 ) {
							auxiliaryEncoderParameters.learning.proximalLearningRate =
							_encoderLayerParameters.learning.proximalLearningRate;
							auxiliaryEncoderParameters.learning.proximalNeighborhood =
							_encoderLayerParameters.learning.proximalNeighborhood;
							auxiliaryEncoderParameters.learning.distalLearningRate =
							_encoderLayerParameters.learning.distalLearningRate;
						}

						if ( timeStep == 0 && iteration == 0 ) // for the first iteration there is no lateral information
							encoderLayerOutput = _encoderLayer.computeResponse(encoderLayerInput[timeStep], // afferent
								       					   auxiliaryEncoderParameters);
						else
							encoderLayerOutput = _encoderLayer.computeResponse(encoderLayerInput[timeStep], // afferent
								       					   encoderLayerOutput, 		// lateral
													   auxiliaryEncoderParameters);

						//MPI_Barrier(MPI::COMM_WORLD);
					}
				}
				encoderInitialProximalLearningRate = encoderInitialProximalLearningRate*0.2;
				encoderInitialProximalNeighborhood = encoderInitialProximalNeighborhood*0.2;
				encoderInitialDistalLearningRate = encoderInitialDistalLearningRate*0.2;
			}

			if ( world_rank == 0 )
				std::cout << "\nEncoder Layer training process complete.\n";

			// this is the end of the initial encoder layer training procedure
			//MPI_Barrier(MPI::COMM_WORLD);
		}

		//MPI_Barrier(MPI::COMM_WORLD);
		// If there are regular layers in the model, they have to be trained
		// only if newLayerAt is smaller than numberOfLayers
		// Asks if there is -at least- one regular layer in the model
		// and if newLayerAt < numberOfLayers
		if ( _modelStructure.numberOfLayers > 0 &&
		     _modelStructure.newLayerAt < _modelStructure.numberOfLayers ) {
			if ( world_rank == 0 )
				std::cout << "\nIn this model we have "
					  << _modelStructure.numberOfLayers
					  << " regular layers.\n";

			if ( world_rank == 0 )
				std::cout << "\nWe have to train from regular layer number "
					  << _modelStructure.newLayerAt
					  << " to regular layer number "
					  << _modelStructure.numberOfLayers-1
					  << ".\n";

			//MPI_Barrier(MPI::COMM_WORLD);
			// trainingLayer is the layer which is new and which is trained in this iteration
			for ( std::size_t trainingLayer = _modelStructure.newLayerAt;
					  trainingLayer < _modelStructure.numberOfLayers;
					  trainingLayer++ ) {
				if ( world_rank == 0 )
					std::cout << "\nTraining regular layer number " << trainingLayer << ".\n";

				// initializes auxiliary encoder layer parameters from encoder layer parameters
				auto	auxiliaryEncoderParameters = _encoderLayerParameters;
				// disables proximal learning but enables distal learning
				// for the encoder layer
				// Encoder layer is the unique in which proximal learning in disabled once it has been
				// initially trained
				auxiliaryEncoderParameters.enableLearning = true;
				auxiliaryEncoderParameters.learning.enableProximalLearning = false;
				auxiliaryEncoderParameters.learning.enableDistalLearning = true;

				// there must be one regular layer parameter set for every regular layer
				if ( !(_regularLayerParameters.size() == _modelStructure.numberOfLayers) ) {
					if ( world_rank == 0 ) {
						std::cout << "\nIn Model class, in function train:" << endl;
						std::cout << "the condition _regularLayerParameters.size() ";
						std::cout << "== _modelStructure.numberOfLayers is not satisfied" << endl;
					}

					//MPI_Barrier(MPI::COMM_WORLD);
					MPI_Abort(MPI::COMM_WORLD,1);
				}
				// initializes auxiliary regular layer parameters from regular layer parameters
				std::vector<regularLayerParameters>	auxiliaryRegularParameters;
				for (const auto& parameters : _regularLayerParameters)
					auxiliaryRegularParameters.push_back(parameters);

				auxiliaryRegularParameters.shrink_to_fit();
				// enable proximal and distal learning for every regular layer in the model
				for ( std::size_t layer = 0;
						  layer < _modelStructure.numberOfLayers;
						  layer++ ) {
					auxiliaryRegularParameters[layer].enableLearning = true;
					auxiliaryRegularParameters[layer].activationHomeostasis = true;
					auxiliaryRegularParameters[layer].learning.enableProximalLearning = true;
					auxiliaryRegularParameters[layer].learning.enableDistalLearning = true;
					auxiliaryRegularParameters[layer].learning.synapticHomeostasis = true;
				}

				// parameters gradually decreased for the learning process
				double	regularInitialProximalLearningRate = 0.9;
				double	regularInitialProximalNeighborhood = 5.0;
				double	regularInitialDistalLearningRate = 1.0;

				//MPI_Barrier(MPI::COMM_WORLD);
				for ( std::size_t stage = 0; stage <= stages+1; stage++ ) {
					if ( world_rank == 0 )
						std::cout << "\nRegular Layer training stage number " << stage << ".\n";

					for ( std::size_t iteration = 0; iteration < iterations; iteration++ ) {
						for ( std::size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {

							// the data is processed by the encoder layer
							if ( timeStep == 0 && iteration == 0 ) // in the first iteration there is no lateral information
								encoderLayerOutput = _encoderLayer.computeResponse(encoderLayerInput[timeStep],	// afferent
														   auxiliaryEncoderParameters);
							else
								encoderLayerOutput = _encoderLayer.computeResponse(encoderLayerInput[timeStep],	// afferent
														   encoderLayerOutput,		// lateral
														   regularLayerOutputs[0],	// apical
														   auxiliaryEncoderParameters);

							if ( stage < stages ) {
								auxiliaryRegularParameters[trainingLayer].learning.proximalLearningRate =
											regularInitialProximalLearningRate *
											std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
								auxiliaryRegularParameters[trainingLayer].learning.proximalNeighborhood =
											regularInitialProximalNeighborhood *
											std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
								auxiliaryRegularParameters[trainingLayer].learning.distalLearningRate =
											regularInitialDistalLearningRate *
											std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
							}
							else if ( timeStep == 0 ) {
								auxiliaryRegularParameters[trainingLayer].learning.proximalLearningRate =
								_regularLayerParameters[trainingLayer].learning.proximalLearningRate;
								auxiliaryRegularParameters[trainingLayer].learning.proximalNeighborhood =
								_regularLayerParameters[trainingLayer].learning.proximalNeighborhood;
								auxiliaryRegularParameters[trainingLayer].learning.distalLearningRate =
								_regularLayerParameters[trainingLayer].learning.distalLearningRate;
							}

							//MPI_Barrier(MPI::COMM_WORLD);
							// regular layers processes the data in turns
							// until the training layer is reached
							for ( std::size_t layer = 0;
									  layer <= trainingLayer;
									  layer++ ) {

								if ( layer == 0 ) { // if layer is 0, information comes from encoder layer
									if ( timeStep == 0 && iteration == 0 ) { // in the first iteration there is no
									       					 // lateral information neither
														 // apical
										regularLayerOutputs[layer] =
										_regularLayers[layer].computeResponse(
												     encoderLayerOutput, // afferent
												     auxiliaryRegularParameters[layer]);
									}
									else {
										if ( layer < trainingLayer ) {
											regularLayerOutputs[layer] =
											_regularLayers[layer].computeResponse(
													     encoderLayerOutput, // afferent
													     regularLayerOutputs[layer], // lateral
													     regularLayerOutputs[layer+1], // apical
													     auxiliaryRegularParameters[layer]);
										}
										else { // for the training layer there is no apical information
											regularLayerOutputs[layer] =
											_regularLayers[layer].computeResponse(
													     encoderLayerOutput, // afferent
													     regularLayerOutputs[layer], // lateral
													     auxiliaryRegularParameters[layer]);
										}
									}
								}
								else {
									if ( timeStep == 0 && iteration == 0 ) { // in the first iteration there is no
									       					 // lateral information
														 // neither apical
										regularLayerOutputs[layer] =
										_regularLayers[layer].computeResponse(
												     regularLayerOutputs[layer-1], // afferent
												     auxiliaryRegularParameters[layer]);
									}
									else {
										if ( layer < trainingLayer ) {
											regularLayerOutputs[layer] =
											_regularLayers[layer].computeResponse(
													     regularLayerOutputs[layer-1], // afferent
													     regularLayerOutputs[layer], // lateral
													     regularLayerOutputs[layer+1], // apical
													     auxiliaryRegularParameters[layer]);
										}
										else { // for the training layer there is no apical information
											regularLayerOutputs[layer] =
											_regularLayers[layer].computeResponse(
													     regularLayerOutputs[layer-1], // afferent
													     regularLayerOutputs[layer], // lateral
													     auxiliaryRegularParameters[layer]);
										}
									}
								}
								//MPI_Barrier(MPI::COMM_WORLD);
							}
							//MPI_Barrier(MPI::COMM_WORLD);
						}
					}
					regularInitialProximalLearningRate = regularInitialProximalLearningRate*0.2;
					regularInitialProximalNeighborhood = regularInitialProximalNeighborhood*0.2;
					regularInitialDistalLearningRate = regularInitialDistalLearningRate*0.2;
				}
				if ( world_rank == 0 )
					std::cout << "\nRegular Layer training process complete.\n";

				//MPI_Barrier(MPI::COMM_WORLD);
			}
			//MPI_Barrier(MPI::COMM_WORLD);
		}
		//MPI_Barrier(MPI::COMM_WORLD);
	}
	else {	// if there is no encoder
		// there must be at least one regular layer
		if ( !(_modelStructure.numberOfLayers > 0) ) {
			if ( world_rank == 0 ) {
				std::cout << "\nIn Model class, in function train:" << endl;
				std::cout << "Condition _modelStructure.numberOfLayers > 0 was not satisfied" << endl;
			}

			//MPI_Barrier(MPI::COMM_WORLD);
			MPI_Abort(MPI::COMM_WORLD,1);
		}
		// for this function to have sense
		// newLayerAt has to be smaller than
		// numbeOfLayers
		if ( !(_modelStructure.newLayerAt <
		       _modelStructure.numberOfLayers) ) {
			if ( world_rank == 0 ) {
				std::cout << "\nIn Model class, in function train:" << endl;
				std::cout << "Condition _modelStructure.newLayerAt <";
		       		std::cout << " _modelStructure.numberOfLayers was not satisfied" << endl;
			}

			//MPI_Barrier(MPI::COMM_WORLD);
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		// loads the information to feed the first regular layer in the model
		if ( world_rank == 0 )
			std::cout << "\nLoading Regular Layer Input data.\n";

		auto	regularLayerInput = Model::loadRegularInputs("inputs");	

		if ( world_rank == 0 )
			std::cout << "\nRegular Layer Input data loaded.\n";

		auto	timeSteps = regularLayerInput.size();

		// Regular Layers in the model have to be trained
		if ( world_rank == 0 )
			std::cout << "\nIn this model we have "
				  << _modelStructure.numberOfLayers
				  << " number of regular layers.\n";

		if ( world_rank == 0 )
			std::cout << "\nWe have to train from regular layer number "
				  << _modelStructure.newLayerAt
				  << " to regular layer "
				  << _modelStructure.numberOfLayers-1
				  << ".\n";

		//MPI_Barrier(MPI::COMM_WORLD);
		// trainingLayer is the layer which is new and which is trained in this iteration
		for ( std::size_t trainingLayer = _modelStructure.newLayerAt;
				  trainingLayer < _modelStructure.numberOfLayers;
				  trainingLayer++ ) {
			if ( world_rank == 0 )
				std::cout << "\nTraining regular layer number " << trainingLayer << ".\n";

			// there must be one regular layer parameter set for every regular layer
			if ( !(_regularLayerParameters.size() == _modelStructure.numberOfLayers) ) {
				if ( world_rank == 0 ) {
					std::cout << "\nIn Model class, in function train:" << endl;
					std::cout << "Condition _regularLayerParameters.size() == _modelStructure.numberOfLayers";
				        std::cout << " was not satisfied" << endl;
				}

				//MPI_Barrier(MPI::COMM_WORLD);
				MPI_Abort(MPI::COMM_WORLD,1);
			}

			// initializes auxiliary regular layer parameters from regular layer parameters
			std::vector<regularLayerParameters>	auxiliaryRegularParameters;
			for (const auto& parameters : _regularLayerParameters)
				auxiliaryRegularParameters.push_back(parameters);

			auxiliaryRegularParameters.shrink_to_fit();
			// enable proximal and distal learning for every regular layer in the model
			for ( std::size_t layer = 0;
					  layer < _modelStructure.numberOfLayers;
					  layer++ ) {
				auxiliaryRegularParameters[layer].enableLearning = true;
				auxiliaryRegularParameters[layer].activationHomeostasis = true;
				auxiliaryRegularParameters[layer].learning.enableProximalLearning = true;
				auxiliaryRegularParameters[layer].learning.enableDistalLearning = true;
				auxiliaryRegularParameters[layer].learning.synapticHomeostasis = true;
			}

			// parameters gradually decreased for the learning process
			double	regularInitialProximalLearningRate = 0.9;
			double	regularInitialProximalNeighborhood = 5.0;
			double	regularInitialDistalLearningRate = 1.0;

			//MPI_Barrier(MPI::COMM_WORLD);
			for ( std::size_t stage = 0; stage <= stages+1; stage++ ) {
				if ( world_rank == 0 )
					std::cout << "\nRegular Layer training stage number " << stage << ".\n";

				for ( std::size_t iteration = 0; iteration < iterations; iteration++ ) {
					for ( std::size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {

						if ( stage < stages ) {
							auxiliaryRegularParameters[trainingLayer].learning.proximalLearningRate =
										regularInitialProximalLearningRate *
										std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
							auxiliaryRegularParameters[trainingLayer].learning.proximalNeighborhood =
										regularInitialProximalNeighborhood *
										std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
							auxiliaryRegularParameters[trainingLayer].learning.distalLearningRate =
										regularInitialDistalLearningRate *
										std::pow(0.01,(timeStep+iteration*timeSteps)/(timeSteps*iterations));
						}
						else if ( timeStep == 0 ) {
							auxiliaryRegularParameters[trainingLayer].learning.proximalLearningRate =
							_regularLayerParameters[trainingLayer].learning.proximalLearningRate;
							auxiliaryRegularParameters[trainingLayer].learning.proximalNeighborhood =
							_regularLayerParameters[trainingLayer].learning.proximalNeighborhood;
							auxiliaryRegularParameters[trainingLayer].learning.distalLearningRate =
							_regularLayerParameters[trainingLayer].learning.distalLearningRate;
						}

						// every regular layer processes the data by turns
						// until the training layer is reached
						for ( std::size_t layer = 0;
								  layer <= trainingLayer;
								  layer++ ) {

							if ( layer == 0 ) { // if layer is 0, information comes from regular layer input
								if ( timeStep == 0 && iteration == 0 ) { // in the first iteration there is no
								       					 // lateral information
													 // neither apical
									regularLayerOutputs[layer] =
									_regularLayers[layer].computeResponse(
											     regularLayerInput[timeStep], // afferent
											     auxiliaryRegularParameters[layer]);
								}
								else {
									if ( layer < trainingLayer ) {
										regularLayerOutputs[layer] =
										_regularLayers[layer].computeResponse(
												     regularLayerInput[timeStep], // afferent
												     regularLayerOutputs[layer], // lateral
												     regularLayerOutputs[layer+1], // apical
												     auxiliaryRegularParameters[layer]);
									}
									else { // for the training layer there is no apical information
										regularLayerOutputs[layer] =
										_regularLayers[layer].computeResponse(
												     regularLayerInput[timeStep], // afferent
												     regularLayerOutputs[layer], // lateral
												     auxiliaryRegularParameters[layer]);
									}
								}
							}
							else {	// if layer is not 0, information comes from the previous layer
								if ( timeStep == 0 && iteration == 0 ) { // in the first iteration there is no
								       					 // lateral information 
													 // neither apical
									regularLayerOutputs[layer] =
									_regularLayers[layer].computeResponse(
											     regularLayerOutputs[layer-1], // afferent
											     auxiliaryRegularParameters[layer]);
								}
								else {
									if ( layer < trainingLayer ) {
										regularLayerOutputs[layer] =
										_regularLayers[layer].computeResponse(
												     regularLayerOutputs[layer-1], // afferent
												     regularLayerOutputs[layer], // lateral
												     regularLayerOutputs[layer+1], // apical
												     auxiliaryRegularParameters[layer]);
									}
									else { // for the training layer there is no apical information
										regularLayerOutputs[layer] =
										_regularLayers[layer].computeResponse(
												     regularLayerOutputs[layer-1], // afferent
												     regularLayerOutputs[layer], // lateral
												     auxiliaryRegularParameters[layer]);
									}
								}
							}
							//MPI_Barrier(MPI::COMM_WORLD);
						}
						//MPI_Barrier(MPI::COMM_WORLD);
					}
				}
				regularInitialProximalLearningRate = regularInitialProximalLearningRate*0.2;
				regularInitialProximalNeighborhood = regularInitialProximalNeighborhood*0.2;
				regularInitialDistalLearningRate = regularInitialDistalLearningRate*0.2;
			}
			if ( world_rank == 0 )
				std::cout << "\nRegular Layer training process complete.\n";

			//MPI_Barrier(MPI::COMM_WORLD);
		}
	}

	//MPI_Barrier(MPI::COMM_WORLD);
	Model::saveModelStatus(folderName);
	//MPI_Barrier(MPI::COMM_WORLD);
} // end function train 


// loads input information
std::vector<encoderLayerInput>	Model::loadEncoderInputs( const std::string& fileName )
{
	std::stringstream	inputStream;

       // open a file in read mode.
	MPI::File infile = MPI::File::Open(MPI::COMM_WORLD, ("../../Octave/" + fileName + ".mat").c_str(),
					   MPI::MODE_RDONLY,
					   MPI::INFO_NULL);

	// gets the file size
	MPI::Offset filesize = infile.Get_size(); // in bytes
	int	bufsize = filesize / sizeof(char); /* in number of char */
	std::string	aux;
	aux.resize(bufsize);

	infile.Set_view(0 * bufsize * sizeof(char),
			MPI_UNSIGNED_CHAR,
			MPI_UNSIGNED_CHAR,
			"native", MPI::INFO_NULL);

	// read the complete file
	infile.Read(&aux[0], bufsize,
		    MPI_UNSIGNED_CHAR);

	inputStream.str(aux);

	// close the opened file.
	infile.Close();

	bool	check_inputs = false;

	std::string	str;
	std::string	STR;
	twodvector<double>	input;

	while ( std::getline(inputStream, str) ) {
		STR = "# name: inputs";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector_of_vectors(input, inputStream);
			check_inputs = true;
		}
	}

	assert(check_inputs == true);
	assert(is_rectangular(input));

	auto	timeSteps = input.size();
	auto	indexes = input[0].size();
	std::vector<encoderLayerInput>	inputModel(timeSteps);
	for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {
		inputModel[timeStep].inputs.resize(indexes);
		inputModel[timeStep].synchronization.resize(indexes);
		inputModel[timeStep].information.resize(indexes);
		for ( size_t index = 0; index < indexes; index++ ) {
			inputModel[timeStep].inputs[index] = input[timeStep][index];
			inputModel[timeStep].synchronization[index] = true;
			if ( std::abs(input[timeStep][index]) >= INPUT_THRESHOLD )
				inputModel[timeStep].information[index] = true;
			else
				inputModel[timeStep].information[index] = false;
		}
	}

	return	inputModel;
} // end function loadEncoderInputs


// loads input information
std::vector<regularLayerResponse>	Model::loadRegularInputs( const std::string& fileName )
{
	std::stringstream	inputStream;

       // open a file in read mode.
	MPI::File infile = MPI::File::Open(MPI::COMM_WORLD, ("../../Octave/" + fileName + ".mat").c_str(),
					   MPI::MODE_RDONLY,
					   MPI::INFO_NULL);

	// gets the file size
	MPI::Offset filesize = infile.Get_size(); // in bytes
	int	bufsize = filesize / sizeof(char); /* in number of char */
	std::string	aux;
	aux.resize(bufsize);

	infile.Set_view(0 * bufsize * sizeof(char),
			MPI_UNSIGNED_CHAR,
			MPI_UNSIGNED_CHAR,
			"native", MPI::INFO_NULL);

	// read the complete file
	infile.Read(&aux[0], bufsize,
		    MPI_UNSIGNED_CHAR);

	inputStream.str(aux);

	// close the opened file.
	infile.Close();

	bool	check_inputs = false;

	std::string	str;
	std::string	STR;
	threedvector<int>	input;

	while ( std::getline(inputStream, str) ) {
		STR = "# name: inputs";
		if ( str.compare(STR) == 0 ) {
			load_cell_to_multidimensional_vector(input, inputStream);
			check_inputs = true;
		}
	}
	assert(check_inputs == true);

	auto	timeSteps = input.size();
	std::vector<regularLayerResponse>	inputModel(timeSteps);
	for ( size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {
		auto	columns = input[timeStep].size();
		inputModel[timeStep].currentIndexes.resize(columns);
		inputModel[timeStep].synchronization.resize(columns);
		inputModel[timeStep].information.resize(columns);
		for ( size_t column = 0; column < columns; column++ ) {
			auto	indexes = input[timeStep][column].size();
			for ( std::size_t index = 0; index < indexes; index++ ) {
				if ( input[timeStep][column][index] > -1 ) { // just load the indexes that bring information
					inputModel[timeStep].currentIndexes[column].push_back(
					input[timeStep][column][index]);
				}
				else {
					assert(input[timeStep][column].size() == 1);
				}
			}

			if ( inputModel[timeStep].currentIndexes[column].size() == 0 ) {
				inputModel[timeStep].information[column] = false;
			}
			else {
				inputModel[timeStep].information[column] = true;
			}	

			inputModel[timeStep].synchronization[column] = true;
		}
	}

	return	inputModel;
} // end function loadRegularInputs


// function that saves the Model's status in a file
void	Model::saveModelStatus( const std::string& folderName )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	// if there is encoder layer and there is at least
	// one regular layer, then save them
	if ( _modelStructure.encoderIncorporation &&
	     (_modelStructure.numberOfLayers > 0) ) {

		MPI_Barrier(MPI::COMM_WORLD);

		if ( world_rank == 0 )
			std::cout << "\nSaving encoder layer\n";

		_encoderLayer.saveEncoderLayerStatus(folderName);
		
		if ( world_rank == 0 )
			std::cout << "\nEncoder layer saved.\n";

		MPI_Barrier(MPI::COMM_WORLD);

		if ( world_rank == 0 ) {
			std::cout << "\nSaving regular layer number 0\n";
			std::cout << "to regular layer number " << _modelStructure.numberOfLayers-1 << ".\n";
		}

		//#pragma omp parallel for default(none) shared(folderName)
		for ( std::size_t layerNumber = 0;
				  layerNumber < _modelStructure.numberOfLayers;
				  layerNumber++ )
			_regularLayers[layerNumber].saveRegularLayerStatus(folderName, layerNumber);

		if ( world_rank == 0 )
			std::cout << "\nRegular layers saved.\n";

		MPI_Barrier(MPI::COMM_WORLD);
	}
	else if ( _modelStructure.encoderIncorporation ) {
		if ( world_rank == 0 )
			std::cout << "\nSaving encoder layer.\n";

		_encoderLayer.saveEncoderLayerStatus(folderName);
		if ( world_rank == 0 )
			std::cout << "\nEncoder layer saved.\n";

	}
	else {
		assert(_modelStructure.numberOfLayers > 0);

		MPI_Barrier(MPI::COMM_WORLD);

		if ( world_rank == 0 ) {
			std::cout << "\nSaving regular layer number 0\n";
			std::cout << "to regular layer number " << _modelStructure.numberOfLayers-1 << ".\n";
		}
		//#pragma omp parallel for default(none) shared(folderName)
		for ( std::size_t layerNumber = 0;
				  layerNumber < _modelStructure.numberOfLayers;
				  layerNumber++ )
			_regularLayers[layerNumber].saveRegularLayerStatus(folderName, layerNumber);

		if ( world_rank == 0 )
			std::cout << "\nRegular layers saved.\n";

		MPI_Barrier(MPI::COMM_WORLD);
	}

	//MPI_Barrier(MPI::COMM_WORLD);
} // end function saveModelStatus


// function that loads the Model's status from a file
void	Model::loadModelStatus( const std::string& folderName )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	//MPI_Barrier(MPI::COMM_WORLD);
	if ( world_rank == 0 )
		std::cout << "\nLoading model structure.\n";

	Model::loadModelStructure(folderName);
	if ( world_rank == 0 )
		std::cout << "\nmodel structure loaded.\n";

	//MPI_Barrier(MPI::COMM_WORLD);
	Model::validateModelStructure();
	if ( world_rank == 0 )
		std::cout << "\nmodel structure validated.\n";

	//MPI_Barrier(MPI::COMM_WORLD);
	_regularLayers.resize(_modelStructure.numberOfLayers);
	if ( _modelStructure.encoderIncorporation ) {	// if there is encoder
		if ( world_rank == 0 )
			std::cout << "\nThis model has encoder layer.\n";

		//MPI_Barrier(MPI::COMM_WORLD);
		if ( _modelStructure.newEncoder ) {	// if the encoder is new
			if ( world_rank == 0 )
				std::cout << "\nThe encoder layer in this model is new.\n";
			
			if (_modelStructure.newLayerAt != 0) {
				if ( world_rank == 0 ) {
					std::cout << "\nIn class Model. In function loadModelStatus.\n";
					std::cout << "\n_modelStructure.newLayerAt != 0\n";
				}
				//MPI_Barrier(MPI::COMM_WORLD);
				MPI_Abort(MPI::COMM_WORLD,1);
			}

			//MPI_Barrier(MPI::COMM_WORLD);
			Model::loadEncoderLayerStructure(folderName);
			if ( world_rank == 0 )
				std::cout << "\nCreating new encoder layer.\n";

			//MPI_Barrier(MPI::COMM_WORLD);
			_encoderLayer.encoderInitializer(_encoderLayerStructure);
			if ( world_rank == 0 ) {
				std::cout << "\nNew Encoder layer created.\n";
				std::cout << "\nFor this model we have "
					  << _modelStructure.numberOfLayers << " regular layers.\n";
			}

			//MPI_Barrier(MPI::COMM_WORLD);
			if ( _modelStructure.numberOfLayers > 0 ) {
				Model::loadRegularLayerStructures(folderName);
				if ( world_rank == 0 )
					std::cout << "\nCreating new regular layers.\n";

				#pragma omp parallel for default(none)
				for ( std::size_t layerNumber = 0;
						  layerNumber < _modelStructure.numberOfLayers;
						  layerNumber++ )
					_regularLayers[layerNumber].layerInitializer(_regularLayerStructures[layerNumber]);
				
				if ( world_rank == 0 )
					std::cout << "\nNew regular layers created.\n";
			}
			//MPI_Barrier(MPI::COMM_WORLD);
		}
		else {					// if the encoder is not new
			if ( world_rank == 0 ) {
				std::cout << "\nThe encoder layer in this model is not new.\n";
				std::cout << "\nFor this model we have "
					  << _modelStructure.numberOfLayers << " regular layers.\n";
			}

			//MPI_Barrier(MPI::COMM_WORLD);
			if ( _modelStructure.newLayerAt > 0 &&
			     _modelStructure.numberOfLayers > 0 ) {

				if ( world_rank == 0 )
					std::cout << "\nLoading encoder layer.\n";

				_encoderLayer.loadEncoderLayerStatus(folderName);

				if ( world_rank == 0 )
					std::cout << "\nEncoder layer loaded.\n";
				
				MPI_Barrier(MPI::COMM_WORLD);

				if ( world_rank == 0 )
					std::cout << "\nLoading regular layers from 0 to "
						  << _modelStructure.newLayerAt-1 << ".\n";

				//#pragma omp parallel for default(none) shared(folderName)
				for ( std::size_t layerNumber = 0;
						  layerNumber < std::min(_modelStructure.numberOfLayers,
									 _modelStructure.newLayerAt);
						  layerNumber++ )
					_regularLayers[layerNumber].loadRegularLayerStatus(folderName, layerNumber);


				if ( world_rank == 0 )
					std::cout << "\nRegular layers loaded.\n";

				MPI_Barrier(MPI::COMM_WORLD);
			}
			else {
				if ( world_rank == 0 )
					std::cout << "\nLoading encoder layer.\n";

				_encoderLayer.loadEncoderLayerStatus(folderName);

				if ( world_rank == 0 )
					std::cout << "\nEncoder layer loaded.\n";
			}
			//MPI_Barrier(MPI::COMM_WORLD);

			if ( _modelStructure.newLayerAt < _modelStructure.numberOfLayers ) {
				Model::loadRegularLayerStructures(folderName);
				if ( world_rank == 0 ) {
					std::cout << "\nCreating new regular layers from "
						  << _modelStructure.newLayerAt
						  << " to " << _modelStructure.numberOfLayers-1 << ".\n";
				}
				#pragma omp parallel for default(none)
				for ( std::size_t layerNumber = _modelStructure.newLayerAt;
						  layerNumber < _modelStructure.numberOfLayers;
						  layerNumber++ )
					_regularLayers[layerNumber].layerInitializer(_regularLayerStructures[layerNumber]);

				if ( world_rank == 0 )
					std::cout << "\nNew regular layers created.\n";
			}
			//MPI_Barrier(MPI::COMM_WORLD);

			Model::loadEncoderLayerStructure(folderName);
			if ( _modelStructure.numberOfLayers > 0 &&
			     !(_modelStructure.newLayerAt < _modelStructure.numberOfLayers) )
				Model::loadRegularLayerStructures(folderName);
		}
		Model::loadEncoderLayerParameters(folderName);
		// checks the encoder structure coherence
		_encoderLayer.checkEncoderLayerStructure(_encoderLayerStructure);
	}
	else {						// if there is no encoder
		if ( _modelStructure.numberOfLayers == 0 )
			MPI_Abort(MPI::COMM_WORLD,1);

		if ( world_rank == 0 )
			std::cout << "\nThis model does not have encoder layer.\n";

		//MPI_Barrier(MPI::COMM_WORLD);
		if ( _modelStructure.newLayerAt > 0 ) {

			MPI_Barrier(MPI::COMM_WORLD);

			if ( world_rank == 0 )
				std::cout << "\nLoading regular layers from 0 to "
					  << _modelStructure.newLayerAt-1 << ".\n";

			//#pragma omp parallel for default(none) shared(folderName)
			for ( std::size_t layerNumber = 0;
					  layerNumber < std::min(_modelStructure.numberOfLayers,
								 _modelStructure.newLayerAt);
					  layerNumber++ )
				_regularLayers[layerNumber].loadRegularLayerStatus(folderName, layerNumber);

			if ( world_rank == 0 )
				std::cout << "\nRegular layers loaded.\n";

			MPI_Barrier(MPI::COMM_WORLD);
		}
		//MPI_Barrier(MPI::COMM_WORLD);

		Model::loadRegularLayerStructures(folderName);

		//MPI_Barrier(MPI::COMM_WORLD);
		if ( _modelStructure.newLayerAt < _modelStructure.numberOfLayers ) {
			if ( world_rank == 0 )
				std::cout << "\nCreating regular layers from "
					  << _modelStructure.newLayerAt << " to "
					  << _modelStructure.numberOfLayers-1 << ".\n";

			#pragma omp parallel for default(none)
			for ( std::size_t layerNumber = _modelStructure.newLayerAt;
					  layerNumber < _modelStructure.numberOfLayers;
					  layerNumber++ )
				_regularLayers[layerNumber].layerInitializer(_regularLayerStructures[layerNumber]);

			if ( world_rank == 0 )
				std::cout << "\nRegular layers created.\n";
		}
		//MPI_Barrier(MPI::COMM_WORLD);
	}
	//MPI_Barrier(MPI::COMM_WORLD);
	if ( _modelStructure.numberOfLayers > 0 )
		Model::loadRegularLayerParameters(folderName);

	for( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ )
		_regularLayers[layer].checkRegularLayerStructure(_regularLayerStructures[layer]);

	Model::validateLayersInterconnection();
	//MPI_Barrier(MPI::COMM_WORLD);
} // end function loadModelStatus


// function that loads the Model's structure from a file
void	Model::loadModelStructure( const std::string& folderName )
{
	std::string	str;
	std::string	STR;
	
	bool	check_encoderIncorporation = false;
	bool	check_newEncoder = false;
	bool	check_numberOfLayers = false;
	bool	check_newLayerAt = false;

	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + folderName + "/ModelStructure.mat", ios::in | std::ifstream::binary);

	while ( std::getline(infile, str) ) {

		STR = "# name: encoderIncorporation";
		if ( str.compare(STR) == 0 ) {
			load_bool(_modelStructure.encoderIncorporation, infile);
			check_encoderIncorporation = true;
		}

		STR = "# name: newEncoder";
		if ( str.compare(STR) == 0 ) {
			load_bool(_modelStructure.newEncoder, infile);
			check_newEncoder = true;
		}

		STR = "# name: numberOfLayers";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_modelStructure.numberOfLayers, infile);
			check_numberOfLayers = true;
		}

		STR = "# name: newLayerAt";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_modelStructure.newLayerAt, infile);
			check_newLayerAt = true;
		}

	}
	// close the opened file.
	infile.close();

	assert(check_encoderIncorporation == true);
	assert(check_newEncoder == true);
	assert(check_numberOfLayers == true);
	assert(check_newLayerAt == true);

} // end function loadModelStructure


// function that validates the model structure
void	Model::validateModelStructure()
{
	if ( _modelStructure.encoderIncorporation &&
	     _modelStructure.newEncoder && 
	     _modelStructure.numberOfLayers > 0 ) {
		if ( _modelStructure.newLayerAt != 0 )
			MPI_Abort(MPI::COMM_WORLD,1);
	}
} // end function validateModelStructure


// function that validates the interconnection between the layers of the model structure
void	Model::validateLayersInterconnection()
{
	for ( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ ) {
		if ( layer == 0 && _modelStructure.encoderIncorporation ) { // if this is the first regular layer
			assert(_encoderLayerStructure.columnsArrayDimensionality ==
			       _regularLayerStructures[layer].afferentArrayDimensionality);

			assert(_encoderLayerStructure.populationsArrayDimensionality ==
			       _regularLayerStructures[layer].afferentPopulationsArrayDimensionality);

			assert(_encoderLayerStructure.apicalArrayDimensionality ==
			       _regularLayerStructures[layer].columnsArrayDimensionality);

			assert(_encoderLayerStructure.apicalPopulationsArrayDimensionality ==
			       _regularLayerStructures[layer].populationsArrayDimensionality);
		}
		else if ( layer > 0 ) {
			assert(_regularLayerStructures[layer-1].columnsArrayDimensionality ==
			       _regularLayerStructures[layer].afferentArrayDimensionality);

			assert(_regularLayerStructures[layer-1].populationsArrayDimensionality ==
			       _regularLayerStructures[layer].afferentPopulationsArrayDimensionality);

			assert(_regularLayerStructures[layer-1].apicalArrayDimensionality ==
			       _regularLayerStructures[layer].columnsArrayDimensionality);

			assert(_regularLayerStructures[layer-1].apicalPopulationsArrayDimensionality ==
			       _regularLayerStructures[layer].populationsArrayDimensionality);
		}
	}
} // end function validateLayersInterconnection


// function that loads the encoder layer structure
void	Model::loadEncoderLayerStructure( const std::string& folderName )
{
	bool	check_afferentArrayDimensionality = false;
	bool	check_apicalArrayDimensionality = false;
	bool	check_columnsArrayDimensionality = false;
	bool	check_afferentReceptiveField = false;
	bool	check_afferentPercentage = false;
	bool	check_afferentWrapAround = false;
	bool	check_lateralDistalReceptiveField = false;
	bool	check_lateralDistalPercentage = false;
	bool	check_lateralDistalWrapAround = false;
	bool	check_apicalReceptiveField = false;
	bool	check_apicalPercentage = false;
	bool	check_apicalWrapAround = false;
	bool	check_iterationNum = false;
	bool	check_populationsArrayDimensionality = false;
	bool	check_apicalPopulationsArrayDimensionality = false;
	bool	check_potentialPercentage = false;

	std::string	str;
	std::string	STR;

	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + folderName + "/EncoderLayerStructure.mat", ios::in | std::ifstream::binary);

	while ( std::getline(infile, str) ) {

		STR = "# name: afferentArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_encoderLayerStructure.afferentArrayDimensionality, infile);
			check_afferentArrayDimensionality = true;
		}
		
		STR = "# name: apicalArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_encoderLayerStructure.apicalArrayDimensionality, infile);
			check_apicalArrayDimensionality = true;
		}

		STR = "# name: columnsArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_encoderLayerStructure.columnsArrayDimensionality, infile);
			check_columnsArrayDimensionality = true;
		}




		STR = "# name: afferentReceptiveField";
		if ( str.compare(STR) == 0 ) {
			_encoderLayerStructure.afferentReceptiveField.clear();
			std::vector<int>	receptiveField;
			load_matrix_to_vector(receptiveField, infile);
			for(const auto& s : receptiveField) {
				if ( s > -1 )
					_encoderLayerStructure.afferentReceptiveField.push_back(s);
			}
			check_afferentReceptiveField = true;
		}

		STR = "# name: afferentPercentage";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerStructure.afferentPercentage, infile);
			check_afferentPercentage = true;
		}

		STR = "# name: afferentWrapAround";
		if ( str.compare(STR) == 0 ) {
			load_bool(_encoderLayerStructure.afferentWrapAround, infile);
			check_afferentWrapAround = true;
		}




		STR = "# name: lateralDistalReceptiveField";
		if ( str.compare(STR) == 0 ) {
			_encoderLayerStructure.lateralDistalReceptiveField.clear();
			std::vector<int>	receptiveField;
			load_matrix_to_vector(receptiveField, infile);
			for(const auto& s : receptiveField) {
				if ( s > -1 )
					_encoderLayerStructure.lateralDistalReceptiveField.push_back(s);
			}
			check_lateralDistalReceptiveField = true;
		}

		STR = "# name: lateralDistalPercentage";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerStructure.lateralDistalPercentage, infile);
			check_lateralDistalPercentage = true;
		}

		STR = "# name: lateralDistalWrapAround";
		if ( str.compare(STR) == 0 ) {
			load_bool(_encoderLayerStructure.lateralDistalWrapAround, infile);
			check_lateralDistalWrapAround = true;
		}




		STR = "# name: apicalReceptiveField";
		if ( str.compare(STR) == 0 ) {
			_encoderLayerStructure.apicalReceptiveField.clear();
			std::vector<int>	receptiveField;
			load_matrix_to_vector(receptiveField, infile);
			for(const auto& s : receptiveField) {
				if ( s > -1 )
					_encoderLayerStructure.apicalReceptiveField.push_back(s);
			}
			check_apicalReceptiveField = true;
		}

		STR = "# name: apicalPercentage";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerStructure.apicalPercentage, infile);
			check_apicalPercentage = true;
		}

		STR = "# name: apicalWrapAround";
		if ( str.compare(STR) == 0 ) {
			load_bool(_encoderLayerStructure.apicalWrapAround, infile);
			check_apicalWrapAround = true;
		}




		STR = "# name: iterationNum";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerStructure.iterationNum, infile);
			check_iterationNum = true;
		}




		STR = "# name: populationsArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_encoderLayerStructure.populationsArrayDimensionality, infile);
			check_populationsArrayDimensionality = true;
		}

		STR = "# name: apicalPopulationsArrayDimensionality";
		if ( str.compare(STR) == 0 ) {
			load_matrix_to_vector(_encoderLayerStructure.apicalPopulationsArrayDimensionality, infile);
			check_apicalPopulationsArrayDimensionality = true;
		}



		STR = "# name: potentialPercentage";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerStructure.potentialPercentage, infile);
			check_potentialPercentage = true;
		}

	}
	// close the opened file.
	infile.close();

	assert(check_afferentArrayDimensionality == true);
	assert(check_apicalArrayDimensionality == true);
	assert(check_columnsArrayDimensionality == true);
	assert(check_afferentReceptiveField == true);
	assert(check_afferentPercentage == true);
	assert(check_afferentWrapAround == true);
	assert(check_lateralDistalReceptiveField == true);
	assert(check_lateralDistalPercentage == true);
	assert(check_lateralDistalWrapAround == true);
	assert(check_apicalReceptiveField == true);
	assert(check_apicalPercentage == true);
	assert(check_apicalWrapAround == true);
	assert(check_iterationNum == true);
	assert(check_populationsArrayDimensionality == true);
	assert(check_apicalPopulationsArrayDimensionality == true);
	assert(check_potentialPercentage == true);
} // end function loadEncoderLayerStructure


// function that loads a regular layer structure
void	Model::loadRegularLayerStructures( const std::string& folderName )
{
	std::string	str;
	std::string	STR;

	assert(_modelStructure.numberOfLayers > 0);
	_regularLayerStructures.clear();
	_regularLayerStructures.resize(_modelStructure.numberOfLayers);

	for ( std::size_t layerNumber = 0;
			  layerNumber < _modelStructure.numberOfLayers;
			  layerNumber++ ) {


		bool	check_afferentArrayDimensionality = false;
		bool	check_apicalArrayDimensionality = false;
		bool	check_columnsArrayDimensionality = false;
		bool	check_afferentReceptiveField = false;
		bool	check_afferentPercentage = false;
		bool	check_afferentWrapAround = false;
		bool	check_lateralProximalReceptiveField = false;
		bool	check_lateralProximalPercentage = false;
		bool	check_lateralProximalWrapAround = false;
		bool	check_lateralDistalReceptiveField = false;
		bool	check_lateralDistalPercentage = false;
		bool	check_lateralDistalWrapAround = false;
		bool	check_apicalReceptiveField = false;
		bool	check_apicalPercentage = false;
		bool	check_apicalWrapAround = false;
		bool	check_iterationnum = false;
		bool	check_populationsArrayDimensionality = false;
		bool	check_afferentPopulationsArrayDimensionality = false;
		bool	check_apicalPopulationsArrayDimensionality = false;
		bool	check_temporalGatheringAfferentValue = false;
		bool	check_potentialPercentage = false;

		// open a file in read mode.
		ifstream infile;
		infile.open("../../Octave/" + folderName + "/RegularLayerStructure_"
					    + std::to_string(layerNumber)
					    + ".mat", ios::in | std::ifstream::binary);

		while ( std::getline(infile, str) ) {


			STR = "# name: afferentArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_regularLayerStructures[layerNumber].afferentArrayDimensionality, infile);
				check_afferentArrayDimensionality = true;
			}
			
			STR = "# name: apicalArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_regularLayerStructures[layerNumber].apicalArrayDimensionality, infile);
				check_apicalArrayDimensionality = true;
			}

			STR = "# name: columnsArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_regularLayerStructures[layerNumber].columnsArrayDimensionality, infile);
				check_columnsArrayDimensionality = true;
			}




			STR = "# name: afferentReceptiveField";
			if ( str.compare(STR) == 0 ) {
				_regularLayerStructures[layerNumber].afferentReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_matrix_to_vector(receptiveField, infile);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_regularLayerStructures[layerNumber].afferentReceptiveField.push_back(s);
				}
				check_afferentReceptiveField = true;
			}

			STR = "# name: afferentPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerStructures[layerNumber].afferentPercentage, infile);
				check_afferentPercentage = true;
			}

			STR = "# name: afferentWrapAround";
			if ( str.compare(STR) == 0 ) {
				load_bool(_regularLayerStructures[layerNumber].afferentWrapAround, infile);
				check_afferentWrapAround = true;
			}




			STR = "# name: lateralProximalReceptiveField";
			if ( str.compare(STR) == 0 ) {
				_regularLayerStructures[layerNumber].lateralProximalReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_matrix_to_vector(receptiveField, infile);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_regularLayerStructures[layerNumber].lateralProximalReceptiveField.push_back(s);
				}
				check_lateralProximalReceptiveField = true;
			}

			STR = "# name: lateralProximalPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerStructures[layerNumber].lateralProximalPercentage, infile);
				check_lateralProximalPercentage = true;
			}

			STR = "# name: lateralProximalWrapAround";
			if ( str.compare(STR) == 0 ) {
				load_bool(_regularLayerStructures[layerNumber].lateralProximalWrapAround, infile);
				check_lateralProximalWrapAround = true;
			}




			STR = "# name: lateralDistalReceptiveField";
			if ( str.compare(STR) == 0 ) {
				_regularLayerStructures[layerNumber].lateralDistalReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_matrix_to_vector(receptiveField, infile);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_regularLayerStructures[layerNumber].lateralDistalReceptiveField.push_back(s);
				}
				check_lateralDistalReceptiveField = true;
			}

			STR = "# name: lateralDistalPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerStructures[layerNumber].lateralDistalPercentage, infile);
				check_lateralDistalPercentage = true;
			}

			STR = "# name: lateralDistalWrapAround";
			if ( str.compare(STR) == 0 ) {
				load_bool(_regularLayerStructures[layerNumber].lateralDistalWrapAround, infile);
				check_lateralDistalWrapAround = true;
			}




			STR = "# name: apicalReceptiveField";
			if ( str.compare(STR) == 0 ) {
				_regularLayerStructures[layerNumber].apicalReceptiveField.clear();
				std::vector<int>	receptiveField;
				load_matrix_to_vector(receptiveField, infile);
				for(const auto& s : receptiveField) {
					if ( s > -1 )
						_regularLayerStructures[layerNumber].apicalReceptiveField.push_back(s);
				}
				check_apicalReceptiveField = true;
			}

			STR = "# name: apicalPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerStructures[layerNumber].apicalPercentage, infile);
				check_apicalPercentage = true;
			}

			STR = "# name: apicalWrapAround";
			if ( str.compare(STR) == 0 ) {
				load_bool(_regularLayerStructures[layerNumber].apicalWrapAround, infile);
				check_apicalWrapAround = true;
			}




			STR = "# name: iterationNum";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerStructures[layerNumber].iterationNum, infile);
				check_iterationnum = true;
			}




			STR = "# name: populationsArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_regularLayerStructures[layerNumber].populationsArrayDimensionality, infile);
				check_populationsArrayDimensionality = true;
			}

			STR = "# name: afferentPopulationsArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_regularLayerStructures[layerNumber].afferentPopulationsArrayDimensionality, infile);
				check_afferentPopulationsArrayDimensionality = true;
			}

			STR = "# name: apicalPopulationsArrayDimensionality";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(_regularLayerStructures[layerNumber].apicalPopulationsArrayDimensionality, infile);
				check_apicalPopulationsArrayDimensionality = true;
			}



			STR = "# name: temporalGatheringAfferentValue";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerStructures[layerNumber].temporalGatheringAfferentValue, infile);
				check_temporalGatheringAfferentValue = true;
			}




			STR = "# name: potentialPercentage";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerStructures[layerNumber].potentialPercentage, infile);
				check_potentialPercentage = true;
			}

		}
		// close the opened file.
		infile.close();

		assert(check_afferentArrayDimensionality == true);
		assert(check_apicalArrayDimensionality == true);
		assert(check_columnsArrayDimensionality == true);
		assert(check_afferentReceptiveField == true);
		assert(check_afferentPercentage == true);
		assert(check_afferentWrapAround == true);
		assert(check_lateralProximalReceptiveField == true);
		assert(check_lateralProximalPercentage == true);
		assert(check_lateralProximalWrapAround == true);
		assert(check_lateralDistalReceptiveField == true);
		assert(check_lateralDistalPercentage == true);
		assert(check_lateralDistalWrapAround == true);
		assert(check_apicalReceptiveField == true);
		assert(check_apicalPercentage == true);
		assert(check_apicalWrapAround == true);
		assert(check_iterationnum == true);
		assert(check_populationsArrayDimensionality == true);
		assert(check_afferentPopulationsArrayDimensionality == true);
		assert(check_apicalPopulationsArrayDimensionality == true);
		assert(check_temporalGatheringAfferentValue == true);
		assert(check_potentialPercentage == true);

	}
} // end function loadRegularLayerStructures


// function that loads the encoder layer parameters
void	Model::loadEncoderLayerParameters( const std::string& folderName )
{
	bool	check_enableLearning = false;
	bool	check_distalSensitivity = false;
	bool	check_proximalInformationThreshold = false;
	bool	check_distalInformationThreshold = false;
	bool	check_activationRadius = false;
	bool	check_sparsity = false;
	bool	check_enableProximalLearning = false;
	bool	check_enableDistalLearning = false;
	bool	check_proximalLearningRate = false;
	bool	check_proximalNeighborhood = false;
	bool	check_spikeTimeDependentSynapticPlasticity = false;
	bool	check_distalLearningRate = false;
	bool	check_limitsLearningRate = false;

	std::string	str;
	std::string	STR;

	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + folderName + "/EncoderLayerParameters.mat", ios::in | std::ifstream::binary);

	while ( std::getline(infile, str) ) {

		STR = "# name: enableLearning";
		if ( str.compare(STR) == 0 ) {
			load_bool(_encoderLayerParameters.enableLearning, infile);
			check_enableLearning = true;
		}

		STR = "# name: distalSensitivity";
		if ( str.compare(STR) == 0 ) {
			load_bool(_encoderLayerParameters.distalSensitivity, infile);
			check_distalSensitivity = true;
		}

		STR = "# name: proximalInformationThreshold";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerParameters.proximalInformationThreshold, infile);
			check_proximalInformationThreshold = true;
		}

		STR = "# name: distalInformationThreshold";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerParameters.distalInformationThreshold, infile);
			check_distalInformationThreshold = true;
		}

		STR = "# name: activationRadius";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerParameters.activationRadius, infile);
			check_activationRadius = true;
		}

		STR = "# name: sparsity";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerParameters.sparsity, infile);
			check_sparsity = true;
		}




		STR = "# name: enableProximalLearning";
		if ( str.compare(STR) == 0 ) {
			load_bool(_encoderLayerParameters.learning.enableProximalLearning, infile);
			check_enableProximalLearning = true;
		}

		STR = "# name: enableDistalLearning";
		if ( str.compare(STR) == 0 ) {
			load_bool(_encoderLayerParameters.learning.enableDistalLearning, infile);
			check_enableDistalLearning = true;
		}

		STR = "# name: proximalLearningRate";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerParameters.learning.proximalLearningRate, infile);
			check_proximalLearningRate = true;
		}

		STR = "# name: proximalNeighborhood";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerParameters.learning.proximalNeighborhood, infile);
			check_proximalNeighborhood = true;
		}

		STR = "# name: spikeTimeDependentSynapticPlasticity";
		if ( str.compare(STR) == 0 ) {
			load_bool(_encoderLayerParameters.learning.spikeTimeDependentSynapticPlasticity, infile);
			check_spikeTimeDependentSynapticPlasticity = true;
		}

		STR = "# name: distalLearningRate";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerParameters.learning.distalLearningRate, infile);
			check_distalLearningRate = true;
		}

		STR = "# name: limitsLearningRate";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_encoderLayerParameters.learning.limitsLearningRate, infile);
			check_limitsLearningRate = true;
		}

	}
	// close the opened file.
	infile.close();

	assert(check_enableLearning == true);
	assert(check_distalSensitivity == true);
	assert(check_proximalInformationThreshold == true);
	assert(check_distalInformationThreshold == true);
	assert(check_activationRadius == true);
	assert(check_sparsity == true);
	assert(check_enableProximalLearning == true);
	assert(check_enableDistalLearning == true);
	assert(check_proximalLearningRate == true);
	assert(check_proximalNeighborhood == true);
	assert(check_spikeTimeDependentSynapticPlasticity == true);
	assert(check_distalLearningRate == true);
	assert(check_limitsLearningRate == true);

} // end function loadEncoderLayerParameters


// function that loads the regular layer parameters
void	Model::loadRegularLayerParameters( const std::string& folderName )
{
	std::string	str;
	std::string	STR;

	assert(_modelStructure.numberOfLayers > 0);
	_regularLayerParameters.clear();
	_regularLayerParameters.resize(_modelStructure.numberOfLayers);

	for ( std::size_t layerNumber = 0;
			  layerNumber < _modelStructure.numberOfLayers;
			  layerNumber++ ) {

		bool	check_enableLearning = false;
		bool	check_distalSensitivity = false;
		bool	check_activationHomeostasis = false;
		bool	check_proximalInformationThreshold = false;
		bool	check_distalInformationThreshold = false;
		bool	check_sparsity = false;
		bool	check_enableProximalLearning = false;
		bool	check_enableDistalLearning = false;
		bool	check_synapticHomeostasis = false;
		bool	check_proximalLearningRate = false;
		bool	check_proximalNeighborhood = false;
		bool	check_plasticity = false;
		bool	check_spikeTimeDependentSynapticPlasticity = false;
		bool	check_distalLearningRate = false;

		// open a file in read mode.
		ifstream infile;
		infile.open("../../Octave/" + folderName + "/RegularLayerParameters_"
					    + std::to_string(layerNumber)
					    + ".mat", ios::in | std::ifstream::binary);

		while ( std::getline(infile, str) ) {

			STR = "# name: enableLearning";
			if ( str.compare(STR) == 0 ) {
				load_bool(_regularLayerParameters[layerNumber].enableLearning, infile);
				check_enableLearning = true;
			}

			STR = "# name: distalSensitivity";
			if ( str.compare(STR) == 0 ) {
				load_bool(_regularLayerParameters[layerNumber].distalSensitivity, infile);
				check_distalSensitivity = true;
			}

			STR = "# name: activationHomeostasis";
			if ( str.compare(STR) == 0 ) {
				load_bool(_regularLayerParameters[layerNumber].activationHomeostasis, infile);
				check_activationHomeostasis = true;
			}

			STR = "# name: proximalInformationThreshold";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerParameters[layerNumber].proximalInformationThreshold, infile);
				check_proximalInformationThreshold = true;
			}

			STR = "# name: distalInformationThreshold";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerParameters[layerNumber].distalInformationThreshold, infile);
				check_distalInformationThreshold = true;
			}

			STR = "# name: sparsity";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerParameters[layerNumber].sparsity, infile);
				check_sparsity = true;
			}




			STR = "# name: enableProximalLearning";
			if ( str.compare(STR) == 0 ) {
				load_bool(_regularLayerParameters[layerNumber].learning.enableProximalLearning, infile);
				check_enableProximalLearning = true;
			}

			STR = "# name: enableDistalLearning";
			if ( str.compare(STR) == 0 ) {
				load_bool(_regularLayerParameters[layerNumber].learning.enableDistalLearning, infile);
				check_enableDistalLearning = true;
			}

			STR = "# name: synapticHomeostasis";
			if ( str.compare(STR) == 0 ) {
				load_bool(_regularLayerParameters[layerNumber].learning.synapticHomeostasis, infile);
				check_synapticHomeostasis = true;
			}

			STR = "# name: proximalLearningRate";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerParameters[layerNumber].learning.proximalLearningRate, infile);
				check_proximalLearningRate = true;
			}

			STR = "# name: proximalNeighborhood";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerParameters[layerNumber].learning.proximalNeighborhood, infile);
				check_proximalNeighborhood = true;
			}

			STR = "# name: plasticity";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerParameters[layerNumber].learning.plasticity, infile);
				check_plasticity = true;
			}

			STR = "# name: spikeTimeDependentSynapticPlasticity";
			if ( str.compare(STR) == 0 ) {
				load_bool(_regularLayerParameters[layerNumber].learning.spikeTimeDependentSynapticPlasticity, infile);
				check_spikeTimeDependentSynapticPlasticity = true;
			}

			STR = "# name: distalLearningRate";
			if ( str.compare(STR) == 0 ) {
				load_scalar(_regularLayerParameters[layerNumber].learning.distalLearningRate, infile);
				check_distalLearningRate = true;
			}

		}
		// close the opened file.
		infile.close();

		assert(check_enableLearning == true);
		assert(check_distalSensitivity == true);
		assert(check_activationHomeostasis == true);
		assert(check_proximalInformationThreshold == true);
		assert(check_distalInformationThreshold == true);
		assert(check_sparsity == true);
		assert(check_enableProximalLearning == true);
		assert(check_enableDistalLearning == true);
		assert(check_synapticHomeostasis == true);
		assert(check_proximalLearningRate == true);
		assert(check_proximalNeighborhood == true);
		assert(check_plasticity == true);
		assert(check_spikeTimeDependentSynapticPlasticity == true);
		assert(check_distalLearningRate == true);
	}

} // end function loadRegularLayerParameters


// this function saves the cumulative encoder layer outputs in a file
void	Model::saveCumulativeEncoderLayerOutput( const std::string& folderName,
						 const threedvector<std::size_t>& outputFromEncoderLayer )
{
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	// first of all, suits the output data in order to save it for Octave
	threedvector<int>	output;
	output.resize(outputFromEncoderLayer.size());
	for(std::size_t timeStep = 0; timeStep < outputFromEncoderLayer.size(); timeStep++) {
		output[timeStep].resize(outputFromEncoderLayer[timeStep].size());
		for(std::size_t column = 0; column < outputFromEncoderLayer[timeStep].size(); column++) {
			if(outputFromEncoderLayer[timeStep][column].size() == 0) { // if there are no indices,
			       							   // inserts one index with a value of -1
				output[timeStep][column].push_back(-1);
			}
			else {
				for (const auto& index : outputFromEncoderLayer[timeStep][column])
					output[timeStep][column].push_back(index);

				output[timeStep][column].shrink_to_fit();
			}
		}
	}

	// stringstream has to be filled with the file contents before written it into the file itself
	std::stringstream	outputStream;

	// file preamble.
	outputStream << "# This is a file created by saveCumulativeEncoderLayerOutput function from," << endl;
	outputStream << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
	outputStream << "# Author: Dematties Dario Jesus." << endl;

	outputStream << "\n\n" << endl;

	std::string	str;
	std::string	STR;

	// open a file in read mode.
	ifstream infile;
	infile.open("../../Octave/" + folderName + "/EncoderLayer.mat", ios::in | std::ifstream::binary);
	std::vector<std::size_t>	columnsArrayDimensionality;
	std::vector<std::size_t>	populationsArrayDimensionality;
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
	save_vector_as_matrix("columnsArrayDimensionality", columnsArrayDimensionality, outputStream);

	// saves populationsArrayDimensionality
	save_vector_as_matrix("populationsArrayDimensionality", populationsArrayDimensionality, outputStream);

	// saves outputFromEncoderLayer
	if ( world_rank == 0 )
		std::cout << "\nsaving cumulative output from Encoder Layer\n";

	save_multidimensional_vector_as_cell("encoderLayerOutput", output, outputStream);

	if ( world_rank == 0 )
		std::cout << "\ncumulative output from Encoder Layer saved\n";

	// File view comunication among ranks
	std::size_t	fileView = 0, fileViewNew;
	std::size_t	lengthToBeSent = outputStream.str().size()/world_size; // sets the amount of characters to be send by this rank
	if ( world_size > 1 && world_rank == world_size-1 )
		lengthToBeSent += outputStream.str().size()%world_size; // accumulates the rest of the characters in the last rank

	if ( world_rank == 0 ) {
		fileViewNew = fileView+lengthToBeSent;
		if ( world_size > 1 )
			MPI_Send(&fileViewNew, 1, my_MPI_SIZE_T,
				 (int)(world_rank+1), 3, MPI::COMM_WORLD);
	}
	else if ( world_rank == world_size-1 ) {
		MPI_Recv(&fileView, 1, my_MPI_SIZE_T,
			 (int)(world_rank-1), 3, MPI::COMM_WORLD,
			 MPI_STATUS_IGNORE);
	}
	else {
		MPI_Recv(&fileView, 1, my_MPI_SIZE_T,
			 (int)(world_rank-1), 3, MPI::COMM_WORLD,
			 MPI_STATUS_IGNORE);
		
		fileViewNew = fileView+lengthToBeSent;
		MPI_Send(&fileViewNew, 1, my_MPI_SIZE_T,
			 (int)(world_rank+1), 3, MPI::COMM_WORLD);
	}

	//MPI_Barrier(MPI::COMM_WORLD);

	// open a file in write mode
	std::string	outputFileName = "EncoderLayerOutput";
	MPI::File	outfile = MPI::File::Open(MPI::COMM_WORLD, ("../../Octave/" + outputFileName + ".mat").c_str(),
						  MPI::MODE_CREATE | MPI::MODE_WRONLY,
						  MPI::INFO_NULL);

	// sets the file view for this rank
	outfile.Set_view(fileView*sizeof(char),
			 MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR,
			 "native", MPI::INFO_NULL);

	// write stream in file
	outfile.Write(&outputStream.str()[fileView],
		     lengthToBeSent,
		     MPI_UNSIGNED_CHAR);

	// close the opened file.
	outfile.Close();
} // end function saveCumulativeEncoderLayerOutput


// this function saves the cumulative regular layer outputs in a file
void	Model::saveCumulativeRegularLayerOutput( const std::string& folderName,
						 const fourdvector<std::size_t>& outputFromRegularLayers )
{
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();
	std::size_t	world_size = MPI::COMM_WORLD.Get_size();

	fourdvector<int>	output;
	assert(outputFromRegularLayers.size() == _modelStructure.numberOfLayers);
	output.resize(_modelStructure.numberOfLayers);
	for ( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ ) {
		// first of all, suits the output data in order to save it for Octave
		output[layer].resize(outputFromRegularLayers[layer].size());
		for(std::size_t timeStep = 0; timeStep < outputFromRegularLayers[layer].size(); timeStep++) {
			output[layer][timeStep].resize(outputFromRegularLayers[layer][timeStep].size());
			for(std::size_t column = 0; column < outputFromRegularLayers[layer][timeStep].size(); column++) {
				if(outputFromRegularLayers[layer][timeStep][column].size() == 0) { // if there are no indices,
											   // inserts one index with a value of -1
					output[layer][timeStep][column].push_back(-1);
				}
				else {
					for (const auto& index : outputFromRegularLayers[layer][timeStep][column])
						output[layer][timeStep][column].push_back(index);

					output[layer][timeStep][column].shrink_to_fit();
				}
			}
		}

		// stringstream has to be filled with the file contents before written it into the file itself
		std::stringstream	outputStream;

		// file preamble.
		outputStream << "# This is a file created by saveCumulativeRegularLayerOutput function from," << endl;
		outputStream << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
		outputStream << "# Author: Dematties Dario Jesus." << endl;

		outputStream << "\n\n" << endl;

		std::string	str;
		std::string	STR;

		// open a file in read mode.
		ifstream infile;
		infile.open("../../Octave/" + folderName + "/RegularLayer_" + std::to_string(layer) + ".mat", ios::in | std::ifstream::binary);
		std::vector<std::size_t>	columnsArrayDimensionality;
		std::vector<std::size_t>	populationsArrayDimensionality;
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
		save_vector_as_matrix("columnsArrayDimensionality", columnsArrayDimensionality, outputStream);

		// saves populationsArrayDimensionality
		save_vector_as_matrix("populationsArrayDimensionality", populationsArrayDimensionality, outputStream);


		// saves outputFromRegularLayers
		if ( world_rank == 0 )
			std::cout << "\nsaving cumulative output from Regular Layer number " << layer << "\n";

		save_multidimensional_vector_as_cell("regularLayerOutput", output[layer], outputStream);

		if ( world_rank == 0 )
			std::cout << "\ncumulative output from Regular Layer " << layer << " saved\n";

		// File view comunication among ranks
		std::size_t	fileView = 0, fileViewNew;
		std::size_t	lengthToBeSent = outputStream.str().size()/world_size; // sets the amount of characters to be send by this rank
		if ( world_size > 1 && world_rank == world_size-1 )
			lengthToBeSent += outputStream.str().size()%world_size; // accumulates the rest of the characters in the last rank

		if ( world_rank == 0 ) {
			fileViewNew = fileView+lengthToBeSent;
			if ( world_size > 1 )
				MPI_Send(&fileViewNew, 1, my_MPI_SIZE_T,
					 (int)(world_rank+1), 3, MPI::COMM_WORLD);
		}
		else if ( world_rank == world_size-1 ) {
			MPI_Recv(&fileView, 1, my_MPI_SIZE_T,
				 (int)(world_rank-1), 3, MPI::COMM_WORLD,
				 MPI_STATUS_IGNORE);
		}
		else {
			MPI_Recv(&fileView, 1, my_MPI_SIZE_T,
				 (int)(world_rank-1), 3, MPI::COMM_WORLD,
				 MPI_STATUS_IGNORE);
			
			fileViewNew = fileView+lengthToBeSent;
			MPI_Send(&fileViewNew, 1, my_MPI_SIZE_T,
				 (int)(world_rank+1), 3, MPI::COMM_WORLD);
		}

		//MPI_Barrier(MPI::COMM_WORLD);

		// open a file in write mode
		std::string	outputFileName = "RegularLayerOutput_";
		MPI::File	outfile = MPI::File::Open(MPI::COMM_WORLD, ("../../Octave/" + outputFileName + std::to_string(layer) + ".mat").c_str(),
							  MPI::MODE_CREATE | MPI::MODE_WRONLY,
							  MPI::INFO_NULL);

		// sets the file view for this rank
		outfile.Set_view(fileView*sizeof(char),
				 MPI_UNSIGNED_CHAR, MPI_UNSIGNED_CHAR,
				 "native", MPI::INFO_NULL);

		// write stream in file
		outfile.Write(&outputStream.str()[fileView],
			     lengthToBeSent,
			     MPI_UNSIGNED_CHAR);

		// close the opened file.
		outfile.Close();
	}
} // end function saveCumulativeRegularLayerOutput

