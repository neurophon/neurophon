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
Model::Model( std::string& folderName, const bool training )
{
	Model::loadModelStatus(folderName, training);
} // end Model constructor


// run the model
void	Model::run( const std::string& folderName )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	if ( world_rank == 0 ) {
		std::cout << "\n\n-----------------------------------------------------" << std::endl;
		std::cout << "\n              STARTING INFERENCE PROCESS " << std::endl;
		std::cout << "\n-----------------------------------------------------" << std::endl;
	}

	regularLayerResponse				encoderLayerOutput;	// output from the encoder layer
	std::vector<regularLayerResponse>		regularLayerOutputs;	// outputs from the regular layers
	regularLayerOutputs.resize(_modelStructure.numberOfLayers);

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


		auto	timeSteps = encoderLayerInput.size();

		if ( world_rank == 0 )
			std::cout << "\nProcessing the Input data.\n";

		// starts to process the input data one time step at a time
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
			}
			// accumulates the encoder and regular layer outputs
			cumulativeEncoderLayerOutput.push_back(encoderLayerOutput.currentIndexes);
			for ( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ )
				cumulativeRegularLayerOutputs[layer].push_back(regularLayerOutputs[layer].currentIndexes);

		}
		cumulativeEncoderLayerOutput.shrink_to_fit();
		for ( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ )
			cumulativeRegularLayerOutputs[layer].shrink_to_fit();
		
		if ( world_rank == 0 ) {
			std::cout << "\n\n-----------------------------------------------------" << std::endl;
			std::cout << "\n              SAVING CUMULATIVE OUTPUTS  " << std::endl;
			std::cout << "\n-----------------------------------------------------" << std::endl;
		}
		Model::saveCumulativeEncoderLayerOutput(folderName, cumulativeEncoderLayerOutput);
		Model::saveCumulativeRegularLayerOutput(folderName, cumulativeRegularLayerOutputs);
		if ( world_rank == 0 ) {
			std::cout << "\n\n-----------------------------------------------------" << std::endl;
			std::cout << "\n              CUMULATIVE OUTPUTS SAVED  " << std::endl;
			std::cout << "\n-----------------------------------------------------" << std::endl;
		}
	}
	else {	// if there is no encoder layer
		// there must be at least one regular layer
		if ( !(_modelStructure.numberOfLayers > 0) ) {
			if ( world_rank == 0 ) {
				std::cout << "\nIn class Model, in function run:" << endl;
				std::cout << "there is no encoder layer, then there must be -at least- one regular layer, but" << endl;
				std::cout << "_modelStructure.numberOfLayers > 0 was not satisfied." << endl;
			}
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


		// Holds output information that comes from
		// the regular layers
		fourdvector<std::size_t>	cumulativeRegularLayerOutputs;
		cumulativeRegularLayerOutputs.resize(_modelStructure.numberOfLayers);

		if ( world_rank == 0 )
			std::cout << "\nProcessing the Input data.\n";
		
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

			}
			// accumulates the regular layer outputs
			for ( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ )
				cumulativeRegularLayerOutputs[layer].push_back(regularLayerOutputs[layer].currentIndexes);
		}
		for ( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ )
			cumulativeRegularLayerOutputs[layer].shrink_to_fit();

		if ( world_rank == 0 ) {
			std::cout << "\n\n-----------------------------------------------------" << std::endl;
			std::cout << "\n              SAVING CUMULATIVE OUTPUTS  " << std::endl;
			std::cout << "\n-----------------------------------------------------" << std::endl;
		}
		Model::saveCumulativeRegularLayerOutput(folderName, cumulativeRegularLayerOutputs);
		if ( world_rank == 0 ) {
			std::cout << "\n\n-----------------------------------------------------" << std::endl;
			std::cout << "\n              CUMULATIVE OUTPUTS SAVED  " << std::endl;
			std::cout << "\n-----------------------------------------------------" << std::endl;
		}
	}
	if ( world_rank == 0 ) {
		std::cout << "\n\n-----------------------------------------------------" << std::endl;
		std::cout << "\n              INFERENCE PROCESS FINISHED  " << std::endl;
		std::cout << "\n-----------------------------------------------------" << std::endl;
	}
} // end function run 


// train the model
void	Model::train( const std::string& folderName )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	if ( world_rank == 0 ) {
		std::cout << "\n\n-----------------------------------------------------" << std::endl;
		std::cout << "\n              STARTING TRAINING PROCESS " << std::endl;
		std::cout << "\n-----------------------------------------------------" << std::endl;
	}

  	regularLayerResponse				encoderLayerOutput;	// output from the encoder layer
	std::vector<regularLayerResponse>		regularLayerOutputs;	// outputs from the regular layers
	regularLayerOutputs.resize(_modelStructure.numberOfLayers);

	if ( _modelStructure.encoderIncorporation ) {	// if there is encoder
		// then, loads the input information to feed the encoder layer
		if ( world_rank == 0 )
			std::cout << "\nLoading Encoder Layer Input data.\n";

		auto	encoderLayerInput = Model::loadEncoderInputs("inputs");	

		if ( world_rank == 0 )
			std::cout << "\nEncoder Layer Input data loaded.\n";

		auto	timeSteps = encoderLayerInput.size();

		// ask if the encoder layer is new
		if ( _modelStructure.newEncoder ) {
			// if the encoder is new, newLayerAt must be 0
			if ( !(_modelStructure.newLayerAt == 0) ) {
				if ( world_rank == 0 ) {
					std::cout << "\nIn Model class, in train function:" << endl;
					std::cout << "the encoder layer is new but" << endl;
					std::cout << "_modelStructure.newLayerAt == 0 is not satisfied" << endl;
				}

				MPI_Abort(MPI::COMM_WORLD,1);
			}

			if ( world_rank == 0 )
				std::cout << "\nTraining process for the encoder layer." << std::endl;

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

			// these are the different stages in the training process for a new encoder layer
			std::size_t	initialStageAt = _modelStructure.initialStageAt;
			for ( std::size_t stage = initialStageAt; stage <= _modelStructure.stages+1; stage++ ) {
				if ( world_rank == 0 ) {
					std::cout << "\nTraining process for the encoder layer." << std::endl;
					std::cout << "Encoder Layer training stage number " << stage << ".\n";
				}

				for ( std::size_t iteration = 0; iteration < _modelStructure.iterations; iteration++ ) {
					for ( std::size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {

						if ( stage < _modelStructure.stages ) {
							auxiliaryEncoderParameters.learning.proximalLearningRate = std::pow(0.2,stage) *
										encoderInitialProximalLearningRate *
										std::pow(0.01,(timeStep+iteration*timeSteps)/
											(timeSteps*_modelStructure.iterations));
							auxiliaryEncoderParameters.learning.proximalNeighborhood = std::pow(0.2,stage) *
										encoderInitialProximalNeighborhood *
										std::pow(0.01,(timeStep+iteration*timeSteps)/
											(timeSteps*_modelStructure.iterations));
							auxiliaryEncoderParameters.learning.distalLearningRate = std::pow(0.2,stage) *
										encoderInitialDistalLearningRate *
										std::pow(0.01,(timeStep+iteration*timeSteps)/
											(timeSteps*_modelStructure.iterations));
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

					}
				}
				// increments _modelStructure.initialStageAt and
				// saves the model status for every iteration
				_modelStructure.initialStageAt = (_modelStructure.initialStageAt+1) % (_modelStructure.stages+2);
				// if the encoder training process is in the last stage
				// the encoder is not new any more
				if ( stage == _modelStructure.stages+1 )
					_modelStructure.newEncoder = false;

				if ( world_rank == 0 ) {
					std::cout << "\n\n-----------------------------------------------------" << std::endl;
					std::cout << "\n              CHECKPOINT    SAVING STATE" << std::endl;
					std::cout << "\n-----------------------------------------------------" << std::endl;
				}
				Model::saveModelStatus(folderName);
				if ( world_rank == 0 ) {
					std::cout << "\n\n-----------------------------------------------------" << std::endl;
					std::cout << "\n              CHECKPOINT    STATE SAVED" << std::endl;
					std::cout << "\n-----------------------------------------------------" << std::endl;
				}
			}

			if ( world_rank == 0 )
				std::cout << "\nEncoder Layer training process completed.\n";

			// this is the end of the initial encoder layer training procedure
		}

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

			// trainingLayer is the layer which is new and which is trained in this iteration
			std::size_t	newLayerAt = _modelStructure.newLayerAt;
			for ( std::size_t trainingLayer = newLayerAt;
					  trainingLayer < _modelStructure.numberOfLayers;
					  trainingLayer++ ) {
				if ( world_rank == 0 )
					std::cout << "\nTraining regular layer number " << trainingLayer << "." << std::endl;

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

				std::size_t	initialStageAt = _modelStructure.initialStageAt;
				for ( std::size_t stage = initialStageAt; stage <= _modelStructure.stages+1; stage++ ) {
					if ( world_rank == 0 ) {
						std::cout << "\nTraining regular layer number " << trainingLayer << "." << std::endl;
						std::cout << "Regular Layer training stage number " << stage << ".\n";
					}

					for ( std::size_t iteration = 0; iteration < _modelStructure.iterations; iteration++ ) {
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

							if ( stage < _modelStructure.stages ) {
								auxiliaryRegularParameters[trainingLayer].learning.proximalLearningRate =
											std::pow(0.2,stage) *
											regularInitialProximalLearningRate *
											std::pow(0.01,(timeStep+iteration*timeSteps)/
												(timeSteps*_modelStructure.iterations));
								auxiliaryRegularParameters[trainingLayer].learning.proximalNeighborhood =
											std::pow(0.2,stage) *
											regularInitialProximalNeighborhood *
											std::pow(0.01,(timeStep+iteration*timeSteps)/
												(timeSteps*_modelStructure.iterations));
								auxiliaryRegularParameters[trainingLayer].learning.distalLearningRate =
											std::pow(0.2,stage) *
											regularInitialDistalLearningRate *
											std::pow(0.01,(timeStep+iteration*timeSteps)/
												(timeSteps*_modelStructure.iterations));
							}
							else if ( timeStep == 0 ) {
								auxiliaryRegularParameters[trainingLayer].learning.proximalLearningRate =
								_regularLayerParameters[trainingLayer].learning.proximalLearningRate;
								auxiliaryRegularParameters[trainingLayer].learning.proximalNeighborhood =
								_regularLayerParameters[trainingLayer].learning.proximalNeighborhood;
								auxiliaryRegularParameters[trainingLayer].learning.distalLearningRate =
								_regularLayerParameters[trainingLayer].learning.distalLearningRate;
							}

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
							}
						}
					}
					// increments _modelStructure.initialStageAt and
					// saves the model status for every iteration
					_modelStructure.initialStageAt = (_modelStructure.initialStageAt+1) % (_modelStructure.stages+2);
					// if this regular layer training process is in the last stage
					// this regular layer is not new any more
					if ( stage == _modelStructure.stages+1 )
						_modelStructure.newLayerAt++;

					if ( world_rank == 0 ) {
						std::cout << "\n\n-----------------------------------------------------" << std::endl;
						std::cout << "\n              CHECKPOINT    SAVING STATE" << std::endl;
						std::cout << "\n-----------------------------------------------------" << std::endl;
					}
					Model::saveModelStatus(folderName);
					if ( world_rank == 0 ) {
						std::cout << "\n\n-----------------------------------------------------" << std::endl;
						std::cout << "\n              CHECKPOINT    STATE SAVED" << std::endl;
						std::cout << "\n-----------------------------------------------------" << std::endl;
					}
				}
				if ( world_rank == 0 )
					std::cout << "\nRegular Layer training process completed.\n";

			}
		}
	}
	else {	// if there is no encoder
		// there must be at least one regular layer
		if ( !(_modelStructure.numberOfLayers > 0) ) {
			if ( world_rank == 0 ) {
				std::cout << "\nIn Model class, in function train:" << endl;
				std::cout << "Condition _modelStructure.numberOfLayers > 0 was not satisfied" << endl;
			}

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

		// trainingLayer is the layer which is new and which is trained in this iteration
		std::size_t	newLayerAt = _modelStructure.newLayerAt;
		for ( std::size_t trainingLayer = newLayerAt;
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

			std::size_t	initialStageAt = _modelStructure.initialStageAt;
			for ( std::size_t stage = initialStageAt; stage <= _modelStructure.stages+1; stage++ ) {
				if ( world_rank == 0 ) {
					std::cout << "\nTraining regular layer number " << trainingLayer << "." << std::endl;
					std::cout << "Regular Layer training stage number " << stage << ".\n";
				}

				for ( std::size_t iteration = 0; iteration < _modelStructure.iterations; iteration++ ) {
					for ( std::size_t timeStep = 0; timeStep < timeSteps; timeStep++ ) {

						if ( stage < _modelStructure.stages ) {
							auxiliaryRegularParameters[trainingLayer].learning.proximalLearningRate =
										std::pow(0.2,stage) *
										regularInitialProximalLearningRate *
										std::pow(0.01,(timeStep+iteration*timeSteps)/
											(timeSteps*_modelStructure.iterations));
							auxiliaryRegularParameters[trainingLayer].learning.proximalNeighborhood =
										std::pow(0.2,stage) *
										regularInitialProximalNeighborhood *
										std::pow(0.01,(timeStep+iteration*timeSteps)/
											(timeSteps*_modelStructure.iterations));
							auxiliaryRegularParameters[trainingLayer].learning.distalLearningRate =
										std::pow(0.2,stage) *
										regularInitialDistalLearningRate *
										std::pow(0.01,(timeStep+iteration*timeSteps)/
											(timeSteps*_modelStructure.iterations));
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
						}
					}
				}
				// increments _modelStructure.initialStageAt and
				// saves the model status for every iteration
				_modelStructure.initialStageAt = (_modelStructure.initialStageAt+1) % (_modelStructure.stages+2);
				// if this regular layer training process is in the last stage
				// this regular layer is not new any more
				if ( stage == _modelStructure.stages+1 )
					_modelStructure.newLayerAt++;

				if ( world_rank == 0 ) {
					std::cout << "\n\n-----------------------------------------------------" << std::endl;
					std::cout << "\n              CHECKPOINT    SAVING STATE" << std::endl;
					std::cout << "\n-----------------------------------------------------" << std::endl;
				}
				Model::saveModelStatus(folderName);
				if ( world_rank == 0 ) {
					std::cout << "\n\n-----------------------------------------------------" << std::endl;
					std::cout << "\n              CHECKPOINT    STATE SAVED" << std::endl;
					std::cout << "\n-----------------------------------------------------" << std::endl;
				}
			}
			if ( world_rank == 0 )
				std::cout << "\nRegular Layer training process complete.\n";
		}
	}
	if ( world_rank == 0 ) {
		std::cout << "\n\n\n\n-----------------------------------------------------" << std::endl;
		std::cout << "\n              TRAINING PROCESS FINISHED  " << std::endl;
		std::cout << "\n-----------------------------------------------------" << std::endl;
	}
} // end function train 


// loads input information
std::vector<encoderLayerInput>	Model::loadEncoderInputs( const std::string& fileName )
{
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();
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

	if (!(check_inputs == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_inputs == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}


	if (!(is_rectangular(input))) {
		if ( world_rank == 0 ) {
			std::cout << "(is_rectangular(input)) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}



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
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();
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
	if (!(check_inputs == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_inputs == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

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
					if (!(input[timeStep][column].size() == 1)) {
						if ( world_rank == 0 ) {
							std::cout << "(input[timeStep][column].size() == 1) no satisfied" << std::endl;
						}
						MPI_Abort(MPI::COMM_WORLD,1);
					}
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

	// if there is encoder layer
	if ( _modelStructure.encoderIncorporation ) {
		// if the encoder is new
		if ( _modelStructure.newEncoder ) {
			// if the encoder layer is partially trained
			if ( _modelStructure.initialStageAt > 0 ) {
				if ( world_rank == 0 )
					std::cout << "\nSaving encoder layer." << std::endl;

				_encoderLayer.saveEncoderLayerStatus(folderName);
				if ( world_rank == 0 )
					std::cout << "\nEncoder layer saved (this is partially trained)." << std::endl;
			}
			// if the encoder layer is not partially trained
			else {
				if ( world_rank == 0 )
					std::cout << "\nEncoder layer won't be saved "
						  << "since its training has not passed the threshold of -at least- "
						  << "one stage.\n"
						  << "No regular layer has been saved." << std::endl;
			}
		}
		// if the encoder is not new
		else {
			// then save the already trained encoder layer
			if ( world_rank == 0 )
				std::cout << "\nSaving encoder layer." << std::endl;

			_encoderLayer.saveEncoderLayerStatus(folderName);
			if ( world_rank == 0 )
				std::cout << "\nAlready trained encoder layer saved." << std::endl;

			// if there are regular layers
			if ( _modelStructure.numberOfLayers > 0 ) {
				// if there are already trained regular layers
				if ( _modelStructure.newLayerAt > 0 ) {
					if ( world_rank == 0 ) {
						std::cout << "\nSaving regular layer number 0\n";
						std::cout << "to regular layer number " << _modelStructure.newLayerAt-1 << ".\n";
					}

					//#pragma omp parallel for default(none) shared(folderName)
					for ( std::size_t layerNumber = 0;
							  layerNumber < _modelStructure.newLayerAt;
							  layerNumber++ )
						_regularLayers[layerNumber].saveRegularLayerStatus(folderName, layerNumber);

					if ( world_rank == 0 )
						std::cout << "\nAlready trained regular layers saved.\n";
				}

				// if _modelStructure.initialStageAt > 0 is satisfied, that means
				// the new layer has been partially trained and has to be saved
				if ( _modelStructure.initialStageAt > 0 ) {
					if ( world_rank == 0 )
						std::cout << "\nSaving regular layer "
							  << _modelStructure.newLayerAt
							  << " since it has been trained partially."
							  << std::endl; 
					_regularLayers[_modelStructure.newLayerAt].saveRegularLayerStatus(folderName, _modelStructure.newLayerAt);
					if ( world_rank == 0 )
						std::cout << "\nRegular layer "
							  << _modelStructure.newLayerAt
							  << " saved (this has been trained partially)."
							  << std::endl; 
				}
			}
		}
	}
	else { // if there is not encoder layer there must be at least one regular layer
		if (!(_modelStructure.numberOfLayers > 0)) {
			if ( world_rank == 0 ) {
				std::cout << "\nIn this context, the model does not have encoder layer, but" << endl;
				std::cout << "(_modelStructure.numberOfLayers > 0) is not satisfied." << endl;
				std::cout << "Then, the model is empty." << endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		// if there are already trained regular layers
		if ( _modelStructure.newLayerAt > 0 ) {
			if ( world_rank == 0 ) {
				std::cout << "\nSaving regular layer number 0\n";
				std::cout << "to regular layer number " << _modelStructure.newLayerAt-1 << ".\n";
			}

			//#pragma omp parallel for default(none) shared(folderName)
			for ( std::size_t layerNumber = 0;
					  layerNumber < _modelStructure.newLayerAt;
					  layerNumber++ )
				_regularLayers[layerNumber].saveRegularLayerStatus(folderName, layerNumber);

			if ( world_rank == 0 )
				std::cout << "\nAlready trained regular layers saved.\n";
		}

		// if _modelStructure.initialStageAt > 0 is satisfied, that means
		// the new layer has been partially trained and has to be saved
		if ( _modelStructure.initialStageAt > 0 ) {
			if ( world_rank == 0 )
				std::cout << "\nSaving regular layer "
					  << _modelStructure.newLayerAt
					  << " since it has been trained partially."
					  << std::endl; 
			_regularLayers[_modelStructure.newLayerAt].saveRegularLayerStatus(folderName, _modelStructure.newLayerAt);
				std::cout << "\nRegular layer "
					  << _modelStructure.newLayerAt
					  << " saved (this has been trained partially)."
					  << std::endl; 
		}
	}
	// save the new structure of the model
	Model::saveModelStructure(folderName);
} // end function saveModelStatus


// function that loads the Model's status from a file
void	Model::loadModelStatus( const std::string& folderName, const bool training )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	if ( world_rank == 0 ) {
		std::cout << "\n\n-----------------------------------------------------" << std::endl;
		std::cout << "\n              LOADING MODEL STATUS " << std::endl;
		std::cout << "\n-----------------------------------------------------" << std::endl;
	}

	if ( training ) { // if this model is in the training mode

		////////////////////////////////////////////////////////////////////////
		// Loads model structure
		if ( world_rank == 0 )
			std::cout << "\nLoading model structure." << std::endl;

		// loads the model structure
		Model::loadModelStructure(folderName);
		if ( world_rank == 0 )
			std::cout << "\nmodel structure loaded." << std::endl;

		// validates the model structure
		Model::validateModelStructure(true);
		if ( world_rank == 0 )
			std::cout << "\nmodel structure validated." << std::endl;
		////////////////////////////////////////////////////////////////////////
		// Model structure loaded

		if ( _modelStructure.encoderIncorporation ) {	// if there is encoder
			// even if the encoder layer is not new, loads the encoder layer structure
			// in order to check its coherence with the structure of the -already trained-
			// encoder layer that is laoded from file
			// or in order to create a new encoder layer
			Model::loadEncoderLayerParameters(folderName);
			Model::loadEncoderLayerStructure(folderName);

			if ( world_rank == 0 )
				std::cout << "\nThis model has encoder layer." << std::endl;

			if ( _modelStructure.newEncoder ) { // if the encoder is new
				if ( world_rank == 0 )
					std::cout << "\nThe encoder layer in this model is new." << std::endl;

				// if the new encoder has been partially trained
				if ( _modelStructure.initialStageAt > 0 ) {
					if ( world_rank == 0 ) {
						std::cout << "\nThis encoder has been partialy trained" << std::endl;
						std::cout << "Then, loads it from file." << std::endl;
					}

					// loads the partially trained encoder layer from a file
					_encoderLayer.loadEncoderLayerStatus(folderName);
					if ( world_rank == 0 )
						std::cout << "\nPartially trained Encoder layer loaded." << std::endl;
				}
				else { // if the new encoder has not been trained at all
					if ( world_rank == 0 )
						std::cout << "\nCreating new encoder layer." << std::endl;

					// creates (initializes) the new encoder layer
					// using the encoder layer structure just loaded
					_encoderLayer.encoderInitializer(_encoderLayerStructure);
					if ( world_rank == 0 )
						std::cout << "\nNew Encoder layer created." << std::endl;
				}
			}
			else { // if the encoder is not new
				if ( world_rank == 0 )
					std::cout << "\nThe encoder layer in this model is not new." << std::endl;

				if ( world_rank == 0 )
					std::cout << "\nLoading encoder layer." << std::endl;

				// first of all, loads the already trained encoder layer from a file
				_encoderLayer.loadEncoderLayerStatus(folderName);

				if ( world_rank == 0 )
					std::cout << "\nEncoder layer loaded." << std::endl;
			}
			// checks the encoder structure coherence
			_encoderLayer.checkEncoderLayerStructure(_encoderLayerStructure);
		}

		if ( _modelStructure.numberOfLayers > 0 ) { // if there are regular layers
			// if there is -at least- one regular layer, loads the regular layers structures
			// even if all of them are already trained (not new)
			// in order to check its coherence
			// with the structure of the already trained regular layers
			// or in order to create new regular layers
			Model::loadRegularLayerParameters(folderName);
			Model::loadRegularLayerStructures(folderName);

			if ( world_rank == 0 ) {
				std::cout << "\nFor this model we have "
					  << _modelStructure.numberOfLayers << " regular layers." << std::endl;
			}

			// allocates space for the regular layers
			_regularLayers.resize(_modelStructure.numberOfLayers);

			// loads the already trained regular layers
			for ( std::size_t layerNumber = 0;
					  layerNumber < _modelStructure.newLayerAt;
					  layerNumber++ ) {
				if ( world_rank == 0 && layerNumber == 0 )
					std::cout << "\nLoading regular layers from 0 to "
						  << _modelStructure.newLayerAt-1 << "." << std::endl;

				_regularLayers[layerNumber].loadRegularLayerStatus(folderName, layerNumber);

				if ( world_rank == 0 && layerNumber == _modelStructure.newLayerAt-1 )
					std::cout << "\nRegular layers loaded." << std::endl;
			}

			// if the new regular layer has been partially trained
			if ( _modelStructure.initialStageAt > 0 &&
			   (!_modelStructure.encoderIncorporation || !_modelStructure.newEncoder) ) {
				if ( world_rank == 0 )
					std::cout << "\nLoading new regular layer, since it has been partially trained." << std::endl;

				_regularLayers[_modelStructure.newLayerAt].loadRegularLayerStatus(folderName, _modelStructure.newLayerAt);

				if ( world_rank == 0 )
					std::cout << "\nNew regular layer loaded (this has been partially trained)." << std::endl;
			}
			else { // if the new regular layer has not been trained at all
				if ( world_rank == 0 ) {
					std::cout << "\nCreating new regular layer number "
						  << _modelStructure.newLayerAt << "." << std::endl;
				}
				_regularLayers[_modelStructure.newLayerAt].layerInitializer(_regularLayerStructures[_modelStructure.newLayerAt]);

				if ( world_rank == 0 )
					std::cout << "\nNew regular layer number " << _modelStructure.newLayerAt << " created." << std::endl;
			}

			// creates (initializes) the new regular layers
			for ( std::size_t layerNumber = _modelStructure.newLayerAt+1;
					  layerNumber < _modelStructure.numberOfLayers;
					  layerNumber++ ) {
				if ( world_rank == 0 ) {
					std::cout << "\nCreating new regular layers "
					          << "from regular layer number "
						  << _modelStructure.newLayerAt+1
						  << " to regular layer number "
						  << _modelStructure.numberOfLayers-1
						  << " ." << std::endl;
				}
				_regularLayers[layerNumber].layerInitializer(_regularLayerStructures[layerNumber]);

				if ( world_rank == 0 )
					std::cout << "\nNew regular layers created." << std::endl;

			}
			// if there is -at least- one regular layer, then
			// checks its coherence
			for( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ )
				_regularLayers[layer].checkRegularLayerStructure(_regularLayerStructures[layer]);
		}

		// validates the interconnections of the layers in the model
		Model::validateLayersInterconnection();
	}
	else { // if this model is not in the training mode
		// this must be in inference mode
		// then, the soft has to load all the files

		////////////////////////////////////////////////////////////////////////
		// Loads model structure
		if ( world_rank == 0 )
			std::cout << "\nLoading model structure." << std::endl;

		// loads the model structure
		Model::loadModelStructure(folderName);
		if ( world_rank == 0 )
			std::cout << "\nmodel structure loaded." << std::endl;

		// validates the model structure
		Model::validateModelStructure(false);
		if ( world_rank == 0 )
			std::cout << "\nmodel structure validated." << std::endl;
		////////////////////////////////////////////////////////////////////////
		// Model structure loaded


		if ( _modelStructure.encoderIncorporation ) { // if there is encoder
			if ( world_rank == 0 )
				std::cout << "\nThis model has encoder layer." << std::endl;

			if ( world_rank == 0 )
				std::cout << "\nLoading encoder layer." << std::endl;

			Model::loadEncoderLayerParameters(folderName);
			Model::loadEncoderLayerStructure(folderName);
			_encoderLayer.loadEncoderLayerStatus(folderName);

			if ( world_rank == 0 )
				std::cout << "\nEncoder layer loaded." << std::endl;

			// checks the encoder structure coherence
			_encoderLayer.checkEncoderLayerStructure(_encoderLayerStructure);
		}

		if ( _modelStructure.numberOfLayers > 0 ) { // if there are regular layers
			// allocates space for the regular layers
			_regularLayers.resize(_modelStructure.numberOfLayers);

			if ( world_rank == 0 )
				std::cout << "\nLoading regular layers from 0 to "
					  << _modelStructure.numberOfLayers-1 << "." << std::endl;

			Model::loadRegularLayerParameters(folderName);
			Model::loadRegularLayerStructures(folderName);
			//#pragma omp parallel for default(none) shared(folderName)
			for ( std::size_t layerNumber = 0;
					  layerNumber < _modelStructure.numberOfLayers;
					  layerNumber++ )
				_regularLayers[layerNumber].loadRegularLayerStatus(folderName, layerNumber);


			if ( world_rank == 0 )
				std::cout << "\nRegular layers loaded." << std::endl;

			// checks the regular layers structure coherence
			for( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ )
				_regularLayers[layer].checkRegularLayerStructure(_regularLayerStructures[layer]);
		}
		Model::validateLayersInterconnection();
	}
} // end function loadModelStatus


// function that saves the Model structure in a file
void	Model::saveModelStructure( const std::string& folderName )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	if ( world_rank == 0 ) {
		std::ofstream outfile;
		outfile.open ("../../Octave/" + folderName + "/ModelStructure.mat", ios::out | ios::trunc);

		// file preamble.
		outfile << "# This is a file created by saveModelStructure member function in Model class from," << endl;
		outfile << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
		outfile << "# Author: Dematties Dario Jesus." << endl;

		outfile << "\n\n" << endl;
		
		// saves encoderIncorporation
		save_as_bool("encoderIncorporation", _modelStructure.encoderIncorporation, outfile);

		// saves newEncoder
		save_as_bool("newEncoder", _modelStructure.newEncoder, outfile);

		// saves numberOfLayers
		save_as_scalar("numberOfLayers", _modelStructure.numberOfLayers, outfile);

		// saves newLayerAt
		save_as_scalar("newLayerAt", _modelStructure.newLayerAt, outfile);

		// saves initialStageAt
		save_as_scalar("initialStageAt", _modelStructure.initialStageAt, outfile);

		// saves iterations
		save_as_scalar("iterations", _modelStructure.iterations, outfile);

		// saves stages
		save_as_scalar("stages", _modelStructure.stages, outfile);

		outfile.close();
	}
} // end function saveModelStructure


// function that loads the Model's structure from a file
void	Model::loadModelStructure( const std::string& folderName )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	std::string	str;
	std::string	STR;
	
	bool	check_encoderIncorporation = false;
	bool	check_newEncoder = false;
	bool	check_numberOfLayers = false;
	bool	check_newLayerAt = false;
	bool	check_initialStageAt = false;
	bool	check_iterations = false;
	bool	check_stages = false;

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

		STR = "# name: initialStageAt";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_modelStructure.initialStageAt, infile);
			check_initialStageAt = true;
		}

		STR = "# name: iterations";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_modelStructure.iterations, infile);
			check_iterations = true;
		}

		STR = "# name: stages";
		if ( str.compare(STR) == 0 ) {
			load_scalar(_modelStructure.stages, infile);
			check_stages = true;
		}

	}
	// close the opened file.
	infile.close();

	if (!(check_encoderIncorporation == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_encoderIncorporation == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_newEncoder == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_newEncoder == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_numberOfLayers == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_numberOfLayers == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_newLayerAt == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_newLayerAt == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_initialStageAt == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_initialStageAt == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_iterations == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_iterations == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_stages == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_stages == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

} // end function loadModelStructure


// function that validates the model structure
void	Model::validateModelStructure( const bool training )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	// _modelStructure.newLayerAt cannot be grater than
	// _modelStructure.numberOfLayers
	if ( _modelStructure.newLayerAt > _modelStructure.numberOfLayers ) {
		if ( world_rank == 0 ) {
			std::cout << "_modelStructure.newLayerAt cannot be grater than ";
		        std::cout << "_modelStructure.numberOfLayers" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	// if thre is not encoder in this model, there must be -at laest-
	// one regular layer
	if ( !_modelStructure.encoderIncorporation &&
	     !(_modelStructure.numberOfLayers > 0) ) {
		if ( world_rank == 0 ) {
			std::cout << "\nIn this model there is no encoder, but" << std::endl;
			std::cout << "_modelStructure.numberOfLayers > 0 is not satisfied" << std::endl;
			std::cout << "that is, the model is empty" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	// if the encoder is new, the first regular layer (0)
	// has to be new too
	if ( _modelStructure.encoderIncorporation &&
	     _modelStructure.newEncoder && 
	     _modelStructure.numberOfLayers > 0 ) {
		if ( _modelStructure.newLayerAt != 0 ) {
			if ( world_rank == 0 ) {
				std::cout << "\nThe encoder is new but" << std::endl;
				std::cout << "_modelStructure.newLayerAt != 0" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}
	}

	// the initial stage can never be grater than the total number of stages in the training plus one
	if ( !(_modelStructure.initialStageAt <= _modelStructure.stages+1) ) {
		if ( world_rank == 0 ) {
			std::cout << "(_modelStructure.initialStageAt <= _modelStructure.stages+1) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	// if the model is in inference mode
	// it has to be completely trained
	if ( !training ) {
		if (!(_modelStructure.newLayerAt == _modelStructure.numberOfLayers) ||
		      _modelStructure.newEncoder ) {
			if ( world_rank == 0 )
				std::cout << "\nThis model is in inference mode, "
					  << "but this is not completely trained.\n"
					  << "In order to be in inference mode, "
					  << "the model has to be completely trained." << std::endl;
			
			MPI_Abort(MPI::COMM_WORLD,1);
		}
	}

        // if the model is in training mode
	// it has to have something to train
	// then if it is completely trained, it throws an error
	if ( training ) {
		if ( !(_modelStructure.newLayerAt < _modelStructure.numberOfLayers) ) {
			if ( world_rank == 0 )
				std::cout << "\nThis model is in training mode, "
					  << "but this is completely trained.\n"
					  << "In order to be in training mode, "
					  << "the model must not be completely trained." << std::endl;

			MPI_Abort(MPI::COMM_WORLD,1);
		}
       }
} // end function validateModelStructure


// function that validates the interconnection between the layers of the model structure
void	Model::validateLayersInterconnection()
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	for ( std::size_t layer = 0; layer < _modelStructure.numberOfLayers; layer++ ) {
		if ( layer == 0 && _modelStructure.encoderIncorporation ) { // if this is the first regular layer
			if (!(_encoderLayerStructure.columnsArrayDimensionality ==
			       _regularLayerStructures[layer].afferentArrayDimensionality)) {
				if ( world_rank == 0 ) {
					std::cout << "(_encoderLayerStructure.columnsArrayDimensionality ==";
					std::cout << " _regularLayerStructures[layer].afferentArrayDimensionality) no satisfied" << std::endl;
				}
				MPI_Abort(MPI::COMM_WORLD,1);
			}
			if (!(_encoderLayerStructure.populationsArrayDimensionality ==
			       _regularLayerStructures[layer].afferentPopulationsArrayDimensionality)) {
				if ( world_rank == 0 ) {
					std::cout << "(_encoderLayerStructure.populationsArrayDimensionality ==";
					std::cout << " _regularLayerStructures[layer].afferentPopulationsArrayDimensionality) no satisfied" << std::endl;
				}
				MPI_Abort(MPI::COMM_WORLD,1);
			}
			if (!(_encoderLayerStructure.apicalArrayDimensionality ==
			       _regularLayerStructures[layer].columnsArrayDimensionality)) {
				if ( world_rank == 0 ) {
					std::cout << "(_encoderLayerStructure.apicalArrayDimensionality ==";
					std::cout << " _regularLayerStructures[layer].columnsArrayDimensionality) no satisfied" << std::endl;
				}
				MPI_Abort(MPI::COMM_WORLD,1);
			}
			if (!(_encoderLayerStructure.apicalPopulationsArrayDimensionality ==
			       _regularLayerStructures[layer].populationsArrayDimensionality)) {
				if ( world_rank == 0 ) {
					std::cout << "(_encoderLayerStructure.apicalPopulationsArrayDimensionality ==";
					std::cout << " _regularLayerStructures[layer].populationsArrayDimensionality) no satisfied" << std::endl;
				}
				MPI_Abort(MPI::COMM_WORLD,1);
			}
		}
		else if ( layer > 0 ) {
			if (!(_regularLayerStructures[layer-1].columnsArrayDimensionality ==
			       _regularLayerStructures[layer].afferentArrayDimensionality)) {
				if ( world_rank == 0 ) {
					std::cout << "(_regularLayerStructures[layer-1].columnsArrayDimensionality ==";
					std::cout << " _regularLayerStructures[layer].afferentArrayDimensionality) no satisfied" << std::endl;
				}
				MPI_Abort(MPI::COMM_WORLD,1);
			}
			if (!(_regularLayerStructures[layer-1].populationsArrayDimensionality ==
			       _regularLayerStructures[layer].afferentPopulationsArrayDimensionality)) {
				if ( world_rank == 0 ) {
					std::cout << "(_regularLayerStructures[layer-1].populationsArrayDimensionality ==";
					std::cout << " _regularLayerStructures[layer].afferentPopulationsArrayDimensionality) no satisfied" << std::endl;
				}
				MPI_Abort(MPI::COMM_WORLD,1);
			}
			if (!(_regularLayerStructures[layer-1].apicalArrayDimensionality ==
			       _regularLayerStructures[layer].columnsArrayDimensionality)) {
				if ( world_rank == 0 ) {
					std::cout << "(_regularLayerStructures[layer-1].apicalArrayDimensionality ==";
					std::cout << " _regularLayerStructures[layer].columnsArrayDimensionality) no satisfied" << std::endl;
				}
				MPI_Abort(MPI::COMM_WORLD,1);
			}
			if (!(_regularLayerStructures[layer-1].apicalPopulationsArrayDimensionality ==
			       _regularLayerStructures[layer].populationsArrayDimensionality)) {
				if ( world_rank == 0 ) {
					std::cout << "(_regularLayerStructures[layer-1].apicalPopulationsArrayDimensionality ==";
					std::cout << " _regularLayerStructures[layer].populationsArrayDimensionality) no satisfied" << std::endl;
				}
				MPI_Abort(MPI::COMM_WORLD,1);
			}
		}
	}
} // end function validateLayersInterconnection


// function that loads the encoder layer structure
void	Model::loadEncoderLayerStructure( const std::string& folderName )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

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

	if (!(check_afferentArrayDimensionality == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_afferentArrayDimensionality == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_apicalArrayDimensionality == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_apicalArrayDimensionality == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_columnsArrayDimensionality == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_columnsArrayDimensionality == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_afferentReceptiveField == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_afferentReceptiveField == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_afferentPercentage == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_afferentPercentage == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_afferentWrapAround == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_afferentWrapAround == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_lateralDistalReceptiveField == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_lateralDistalReceptiveField == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_lateralDistalPercentage == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_lateralDistalPercentage == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_lateralDistalWrapAround == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_lateralDistalWrapAround == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_apicalReceptiveField == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_apicalReceptiveField == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_apicalPercentage == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_apicalPercentage == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_apicalWrapAround == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_apicalWrapAround == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_iterationNum == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_iterationNum == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_populationsArrayDimensionality == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_populationsArrayDimensionality == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_apicalPopulationsArrayDimensionality == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_apicalPopulationsArrayDimensionality == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_potentialPercentage == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_potentialPercentage == true) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}
} // end function loadEncoderLayerStructure


// function that loads a regular layer structure
void	Model::loadRegularLayerStructures( const std::string& folderName )
{
	// Get the rank of the process
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	std::string	str;
	std::string	STR;

	if (!(_modelStructure.numberOfLayers > 0)) {
		if ( world_rank == 0 ) {
			std::cout << "(_modelStructure.numberOfLayers > 0) is not satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

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

		if (!(check_afferentArrayDimensionality == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_afferentArrayDimensionality == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_apicalArrayDimensionality == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_apicalArrayDimensionality == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_columnsArrayDimensionality == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_columnsArrayDimensionality == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_afferentReceptiveField == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_afferentReceptiveField == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_afferentPercentage == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_afferentPercentage == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_afferentWrapAround == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_afferentWrapAround == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_lateralProximalReceptiveField == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_lateralProximalReceptiveField == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_lateralProximalPercentage == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_lateralProximalPercentage == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_lateralProximalWrapAround == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_lateralProximalWrapAround == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_lateralDistalReceptiveField == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_lateralDistalReceptiveField == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_lateralDistalPercentage == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_lateralDistalPercentage == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_lateralDistalWrapAround == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_lateralDistalWrapAround == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_apicalReceptiveField == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_apicalReceptiveField == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_apicalPercentage == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_apicalPercentage == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_apicalWrapAround == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_apicalWrapAround == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_iterationnum == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_iterationnum == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_populationsArrayDimensionality == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_populationsArrayDimensionality == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_afferentPopulationsArrayDimensionality == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_afferentPopulationsArrayDimensionality == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_apicalPopulationsArrayDimensionality == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_apicalPopulationsArrayDimensionality == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_temporalGatheringAfferentValue == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_temporalGatheringAfferentValue == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_potentialPercentage == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_potentialPercentage == true) is not satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}


	}
} // end function loadRegularLayerStructures


// function that loads the encoder layer parameters
void	Model::loadEncoderLayerParameters( const std::string& folderName )
{
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

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

	if (!(check_enableLearning == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_enableLearning == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_distalSensitivity == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_distalSensitivity == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_proximalInformationThreshold == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_proximalInformationThreshold == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_distalInformationThreshold == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_distalInformationThreshold == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_activationRadius == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_activationRadius == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_sparsity == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_sparsity == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_enableProximalLearning == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_enableProximalLearning == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_enableDistalLearning == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_enableDistalLearning == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_proximalLearningRate == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_proximalLearningRate == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_proximalNeighborhood == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_proximalNeighborhood == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_spikeTimeDependentSynapticPlasticity == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_spikeTimeDependentSynapticPlasticity == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_distalLearningRate == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_distalLearningRate == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

	if (!(check_limitsLearningRate == true)) {
		if ( world_rank == 0 ) {
			std::cout << "(check_limitsLearningRate == true) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

} // end function loadEncoderLayerParameters


// function that loads the regular layer parameters
void	Model::loadRegularLayerParameters( const std::string& folderName )
{
	std::size_t	world_rank = MPI::COMM_WORLD.Get_rank();

	std::string	str;
	std::string	STR;

	if (!(_modelStructure.numberOfLayers > 0)) {
		if ( world_rank == 0 ) {
			std::cout << "(_modelStructure.numberOfLayers > 0) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

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

		if (!(check_enableLearning == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_enableLearning == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_distalSensitivity == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_distalSensitivity == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_activationHomeostasis == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_activationHomeostasis == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_proximalInformationThreshold == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_proximalInformationThreshold == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_distalInformationThreshold == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_distalInformationThreshold == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_sparsity == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_sparsity == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_enableProximalLearning == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_enableProximalLearning == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_enableDistalLearning == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_enableDistalLearning == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_synapticHomeostasis == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_synapticHomeostasis == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_proximalLearningRate == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_proximalLearningRate == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_proximalNeighborhood == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_proximalNeighborhood == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_plasticity == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_plasticity == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_spikeTimeDependentSynapticPlasticity == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_spikeTimeDependentSynapticPlasticity == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

		if (!(check_distalLearningRate == true)) {
			if ( world_rank == 0 ) {
				std::cout << "(check_distalLearningRate == true) no satisfied" << std::endl;
			}
			MPI_Abort(MPI::COMM_WORLD,1);
		}

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
	if (!(outputFromRegularLayers.size() == _modelStructure.numberOfLayers)) {
		if ( world_rank == 0 ) {
			std::cout << "(outputFromRegularLayers.size() == _modelStructure.numberOfLayers) no satisfied" << std::endl;
		}
		MPI_Abort(MPI::COMM_WORLD,1);
	}

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

