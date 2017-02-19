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

int	i;
char	option, option1, option2, option3;

system("clear");					// clears the screen

//printf("\nDo you want to compute the cepstrum from the Mel Filterbank? (Y) for yes and any key for no\n\n");
//scanf(" %c", &option);

option = 'n';

//printf("\nDo you want to apply the adjust of the gain to the coefficients? (Y) for yes and any key for no\n\n");
//scanf(" %c", &option1);

option1 = 'Y';

//printf("\nDo you want to compute the Deltas from the MFCCs? (Y) for yes and any key for no\n\n");
//scanf(" %c", &option2);

option2 = 'n';

// Persistence option
option3 = 'n';

////////////////////////////////////////// loads audio vector X and sample frequency Fs from "AudioVector.mat" file

audioVector audio;

audio = loadAudioVectorFile();

////////////////////////////////////////// audio vector X and sample frequency Fs loaded from "AudioVector.mat" file






////////////////////////////////////////// reshapes audio vector X conform to Fourier sample period and Fourier window

double	delayValue = 1;
audioArray array;

array = reshapeAudioVector(audio, delayValue*0.01, 0.025);
//array = reshapeAudioVector(audio, 0.01, 0.025); // original training
//array = reshapeAudioVector(audio, 0.5, 1);

////////////////////////////////////////// audio vector X reshaped conform to Fourier sample period and Fourier window






////////////////////////////////////////// saves the spectrum from the different channels in spectrum.mat file

//saveWindowed(array);

////////////////////////////////////////// spectrum saved from the different channels in spectrum.mat file






////////////////////////////////////////// extract the spectrum from the different channels in array

spectrumArray arrayS;

arrayS = extractSpectrum(array);

////////////////////////////////////////// spectrum extracted from the different channels in array






////////////////////////////////////////// process the spectrum from the different channels in arrayS

audioArray processedSpectrum;

if ( option == 'Y' )
{
	processedSpectrum = processSpectrum(arrayS);
}
else
{
	processedSpectrum = processPowerSpectrum(arrayS);
}

////////////////////////////////////////// spectrum processed from the different channels in arrayS






////////////////////////////////////////// saves the spectrum from the different channels in spectrum.mat file

//saveSpectrum(processedSpectrum);

////////////////////////////////////////// spectrum saved from the different channels in spectrum.mat file






////////////////////////////////////////// computes the Mel filter banks

melArray MF;

MF = melFilterbank(processedSpectrum);

////////////////////////////////////////// Mel filter banks computed






////////////////////////////////////////// computes the log and DCT of the Mel filter banks

melArray	MFCC;

if ( option == 'Y' )
{
	MFCC = dctLogMelFilterbank(MF);
}
else
{
	MFCC = MF;
}


////////////////////////////////////////// log and DCT of the Mel filter banks computed






////////////////////////////////////////// adjusts the gain in Mel filter banks

double	threshold = 0.001;

if ( option1 == 'Y' )
{
	adjustGain(MFCC, threshold);
	//adjustDeltaGain(deltas, threshold);
}

////////////////////////////////////////// Mel filter banks gain adjusted





////////////////////////////////////////// computes the Deltas and Delta-Deltas of MFCC

deltaArray	deltas;

if ( option2 == 'Y' )
{
	deltas = deltasDeltaDeltas(MFCC,2);
}

////////////////////////////////////////// Deltas and Delta-Deltas of MFCC computed




////////////////////////////////////////// adds paersistence to the MFCC

int pers = 3;
//int pers = 6;

if ( option3 == 'Y' )
	persistence(MFCC,pers);

////////////////////////////////////////// paersistence added to the MFCC





////////////////////////////////////////// saves the Mel Frequency baks from the different channels

saveMFCC(MFCC);

////////////////////////////////////////// Mel Frequency baks saved from the different channels






////////////////////////////////////////// saves the Deltas and Delta-Deltas of the MFCC in two files

if ( option2 == 'Y' )
{
	saveDeltas(deltas);
}

////////////////////////////////////////// Deltas and Delta-Deltas of the MFCC saved in two files




} // end main

