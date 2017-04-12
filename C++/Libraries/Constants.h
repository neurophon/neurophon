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

// File Name:		Constants.h
// File Description:	Constant definitions. This file contains the constant definitions to be used in the code

#ifndef CONTANTS_H
#define CONTANTS_H

// This is the synaptic increment
#define	SYNAPTIC_INCREMENT	0.001

// This is the synaptic decrement
#define	SYNAPTIC_DECREMENT	0.001

// This is the period of updates
#define	UPDATE_PERIOD	10000

// This is threshold imposed to the input information
#define	INPUT_THRESHOLD		0.000001

// This is the threshold from which a massive activation
// is considered as a prediction fault
#define	PREDICTION_FAULT_THRESHOLD	10

// This is the busting taken for distal synapses
// growing when there is a prediction fault
#define	BUSTING	10

#endif
