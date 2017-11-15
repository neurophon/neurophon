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

// File Name:		Model.h
// File Description:	Model class definition. This file presents Model's public
//			interface without revealing the implementations of Model's member functions,
//			which are defined in Model.cpp.


// Class Description: ...

#ifndef MODEL_H
#define MODEL_H

#include "EncoderLayer.h"
#include "RegularLayer.h"

class Model
{
public:

	Model( std::string&, const bool );		// Model constructor with data from file

	void	run( const std::string& );		// run the model

	void	train( const std::string& );		// train the model

	std::vector<encoderLayerInput>	loadEncoderInputs( const std::string& );	// loads input information for the encoder.
											// puts the inputs in a vector of encoderLayerInput structures
											// every encoderLayerInput structure corresponds to a time step
											// in the run

	std::vector<regularLayerResponse>	loadRegularInputs( const std::string& );		// loads input information for the first regular layer.
											// puts the inputs in a vector of regularLayerResponse structures
											// every regularLayerResponse structure corresponds to a time step
											// in the run

	void	saveModelStatus( const std::string& );			// function that saves the Model's status in a file
									// this has to be called every time a stage is completed
									// when the encoder or a layer training is completes
									// newEncoder becomes false or newLayerAt is increades
									// respectively and initialStageAt is reseted to 0

	void	loadModelStatus( const std::string&,
		       		 const bool );				// function that loads the Model's status from a file

	void	saveModelStructure( const std::string& );		// function that saves the Model structure in a file

	void	loadModelStructure( const std::string& );		// function that loads the Model's structure from a file

	void	validateModelStructure( const bool );			// function that validates the model structure

	void	validateLayersInterconnection();			// function that validates the interconnection between
       									// the layers of the model structure
	void	loadEncoderLayerStructure( const std::string& );	// function that loads the encoder layer structure

	void	loadRegularLayerStructures( const std::string& );	// function that loads a regular layer structures

	void	loadEncoderLayerParameters( const std::string& );	// function that loads the encoder layer parameters

	void	loadRegularLayerParameters( const std::string& );	// function that loads the regular layer parameters

	void	saveCumulativeEncoderLayerOutput( const std::string&,
						  const threedvector<std::size_t>& );	// this function saves the cumulative encoder layer outputs in a file

	void	saveCumulativeRegularLayerOutput( const std::string&,
						  const fourdvector<std::size_t>& );	// this function saves the cumulative regular layer outputs in a file

private:
	modelStructure					_modelStructure;	// the model structure

	encoderLayerParameters				_encoderLayerParameters;// encoder layer parameters
	encoderLayerStructure				_encoderLayerStructure;	// encoder layer structure
	EncoderLayer					_encoderLayer;		// encoder layer

	std::vector<regularLayerParameters>		_regularLayerParameters;// regular layer parameters
	std::vector<regularLayerStructure>		_regularLayerStructures;// regular layer structure
	std::vector<RegularLayer>			_regularLayers;		// vector of regular layers
}; // end class Model

#endif

