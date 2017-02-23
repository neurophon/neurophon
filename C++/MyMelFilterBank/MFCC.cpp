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

// File Name:		MFCC.cpp
// File Description:	This file includes a program whose main objective is to generate Mel Frequency Cepstral Coefficients from AudioVector.mat.

#include <stdlib.h>
#include <iostream>

#include "AudioService.h"

// starts the program GenerateCorticalNode
int main()
{
int		numberOfFilters = 128;
double		sampleRate = 0.008;
int		persistenceValue = 4;
bool		computeCepstrum = false;
bool		gainControl = true;
bool		computeDeltas = false;
bool		applyPersistence = false;
bool		leakyIntegration = true;
bool		kernelConvolution = true;
double		delay = 0;

system("clear");					// clears the screen

std::string	fileName = "MFB008";
std::string	kernelName = "Kernel008";
double		sampleWindow = 0.008;
double		leakyCoefficient = 0.1;
filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
		   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
		   kernelConvolution, delay, fileName, kernelName );

fileName = "MFB016";
kernelName = "Kernel016";
sampleWindow = 0.016;
leakyCoefficient = 0,3;
filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
		   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
		   kernelConvolution, delay, fileName, kernelName );

fileName = "MFB032";
kernelName = "Kernel032";
sampleWindow = 0.032;
leakyCoefficient = 0,5;
filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
		   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
		   kernelConvolution, delay, fileName, kernelName );

fileName = "MFB064";
kernelName = "Kernel064";
sampleWindow = 0.064;
leakyCoefficient = 0,7;
filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
		   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
		   kernelConvolution, delay, fileName, kernelName );

fileName = "MFB128";
kernelName = "Kernel128";
sampleWindow = 0.128;
leakyCoefficient = 0,9;
filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
		   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
		   kernelConvolution, delay, fileName, kernelName );

} // end main

