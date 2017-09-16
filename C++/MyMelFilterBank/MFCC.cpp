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
#include <omp.h>

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
bool		leakyIntegration = false;
bool		kernelConvolution = true;
double		delay = 0;

system("clear");					// clears the screen


#pragma omp parallel sections
{

	#pragma omp section
	{
		std::string	fileName = "MFB008";
		std::string	kernelName = "Kernel008";
		double		sampleWindow = 0.008;
		double		leakyCoefficient = 0.01;
		filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
				   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
				   kernelConvolution, delay, fileName, kernelName );
	}

	#pragma omp section
	{
		std::string	fileName = "MFB016";
		std::string	kernelName = "Kernel016";
		double		sampleWindow = 0.016;
		double		leakyCoefficient = 0.03;
		filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
				   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
				   kernelConvolution, delay, fileName, kernelName );
	}

	#pragma omp section
	{
		std::string	fileName = "MFB032";
		std::string	kernelName = "Kernel032";
		double		sampleWindow = 0.032;
		double		leakyCoefficient = 0.05;
		filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
				   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
				   kernelConvolution, delay, fileName, kernelName );
	}

	#pragma omp section
	{
		std::string	fileName = "MFB064";
		std::string	kernelName = "Kernel064";
		double		sampleWindow = 0.064;
		double		leakyCoefficient = 0.07;
		filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
				   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
				   kernelConvolution, delay, fileName, kernelName );
	}

	#pragma omp section
	{
		std::string	fileName = "MFB128";
		std::string	kernelName = "Kernel128";
		double		sampleWindow = 0.128;
		double		leakyCoefficient = 0.09;
		filterAudioVector( numberOfFilters, sampleRate, sampleWindow, persistenceValue, leakyCoefficient,
				   computeCepstrum, gainControl, computeDeltas, applyPersistence, leakyIntegration,
				   kernelConvolution, delay, fileName, kernelName );
	}
}

} // end main

