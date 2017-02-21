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
double		delay = 0;

std::string	fileName = "MFB008";
double		sampleWindow = 0.008;
double		leakyCoefficient = 0.1;
filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
		   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
		   delay, fileName );

fileName = "MFB016";
sampleWindow = 0.016;
leakyCoefficient = 0,3;
filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
		   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
		   delay, fileName );

fileName = "MFB032";
sampleWindow = 0.032;
leakyCoefficient = 0,5;
filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
		   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
		   delay, fileName );

fileName = "MFB064";
sampleWindow = 0.064;
leakyCoefficient = 0,7;
filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
		   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
		   delay, fileName );

fileName = "MFB128";
sampleWindow = 0.128;
leakyCoefficient = 0,9;
filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
		   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
		   delay, fileName );

} // end main

