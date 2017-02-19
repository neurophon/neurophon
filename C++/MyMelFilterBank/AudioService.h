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

// File Name:		AudioService.h
// File Description: This file contains a series of functions and definitions used by programs of audio treatment.

#include <stdio.h>
#include <fftw3.h>

struct audioVector				// This structure will contain a pointer to the audio vector X and the sample frequency Fs
{
	int	Fs;				// sample frequency
	int	rows;				// the number of rows in X
	int	columns;			// the number of columns in X
	double	*X;				// audio vector
};

struct audioArray				// This structure will contain a pointer -for every channel- to the windowed audio array
{
	int	channels;			// this is the number of channels
	int	chunks;				// this is the number of chunks of length equal to fourierSamplingPeriodLength in every channel of X
	int	fourierSamplingPeriodLength;	// this is the vector length of the Fourier sampling period
	int	fourierWindowLength;		// this is the vector length of the Fourier window
	int	sampleFrequency;		// this is the sample frequency of the audio file
	double	fourierSamplingPeriod;		// this is the sampling period for Fourier transformation
	double	fourierWindow;			// this is the temporal window for Fourier transformation
	double	samplingPeriod;			// this is the sampling period of the audio file
	double	*channel[4];			// audio array pointers for channels 1 to 4
};

struct spectrumArray				// This structure will contain a pointer -for every channel- to the windowed spectrum array
{
	int	channels;			// this is the number of channels
	int	chunks;				// this is the number of chunks of length equal to fourierSamplingPeriodLength in every channel of X
	int	fourierSamplingPeriodLength;	// this is the vector length of the Fourier sampling period
	int	fourierWindowLength;		// this is the vector length of the Fourier window
	int	sampleFrequency;		// this is the sample frequency of the audio file
	double	fourierSamplingPeriod;		// this is the sampling period for Fourier transformation
	double	fourierWindow;			// this is the temporal window for Fourier transformation
	double	samplingPeriod;			// this is the sampling period of the audio file
	fftw_complex	*channel[4];		// spectrum array pointers for channels 1 to 4
};

struct melArray					// This structure will contain a pointer -for every channel- to the windowed MFCC array
{
	int	filters;			// this is the number of filters			
	int	channels;			// this is the number of channels
	int	chunks;				// this is the number of chunks of length equal to fourierSamplingPeriodLength in every channel of X
	int	fourierSamplingPeriodLength;	// this is the vector length of the Fourier sampling period
	int	fourierWindowLength;		// this is the vector length of the Fourier window
	int	sampleFrequency;		// this is the sample frequency of the audio file
	double	fourierSamplingPeriod;		// this is the sampling period for Fourier transformation
	double	fourierWindow;			// this is the temporal window for Fourier transformation
	double	samplingPeriod;			// this is the sampling period of the audio file
	double	*channel[4];			// audio array pointers for channels 1 to 4
	double	*filter;			// this contains the filter configuration
};

struct deltaArray				// This structure will contain a pointer -for every channel- to the windowed deltas and delta-deltas of the MFCC array
{
	int	filters;			// this is the number of filters			
	int	channels;			// this is the number of channels
	int	chunks;				// this is the number of chunks of length equal to fourierSamplingPeriodLength in every channel of X
	int	fourierSamplingPeriodLength;	// this is the vector length of the Fourier sampling period
	int	fourierWindowLength;		// this is the vector length of the Fourier window
	int	sampleFrequency;		// this is the sample frequency of the audio file
	double	fourierSamplingPeriod;		// this is the sampling period for Fourier transformation
	double	fourierWindow;			// this is the temporal window for Fourier transformation
	double	samplingPeriod;			// this is the sampling period of the audio file
	double	*channelDeltas[4];		// audio array pointers for channels 1 to 4
	double	*channelDeltaDeltas[4];		// audio array pointers for channels 1 to 4
};

// function to read "AudioVector.mat" file
audioVector	loadAudioVectorFile();

// function to reshape audio vector X for multidimensional Fourier transformation isues
audioArray	reshapeAudioVector( audioVector, double, double );

// function to save the windowed audio vector in Octave format for Octave visualization issues
void	saveWindowed( audioArray );

// function to extract the spectrum from audioArray through fftw function
spectrumArray	extractSpectrum( audioArray );

// function to process the spectrum computed by extractSpectrum
audioArray	processSpectrum( spectrumArray );

// function to process the power spectrum computed by extractSpectrum
audioArray	processPowerSpectrum( spectrumArray );

// function to save the processed spectrum in Octave format for Octave visualization issues
void	saveSpectrum( audioArray );

// function to compute the Mel filterbank
melArray	melFilterbank( audioArray );

// function to compute the log of the Mel filterbank
void	logMelFilterbank( melArray );

// function to compute the Discrete Cosine Transform (DCT) of the log Mel filterbank
melArray	dctLogMelFilterbank( melArray );

// function to compute the Deltas and Delta-Deltas of the MFCC
deltaArray	deltasDeltaDeltas( melArray, int );

// function to save the MFCC in a Octave format file
void	saveMFCC( melArray );

// function to save the MFCC in a Octave format file
void	saveDeltas( deltaArray );

// function to transform from Hz to Mel frequency
double	hertz2Mel( double );

// function to transform from Mel to Hz frequency
double	mel2Hertz( double );

// function to adjust the gain of the coefficients
void	adjustGain( melArray, double );

// function to adjust the gain of the deltas and delta-deltas coefficients
void	adjustDeltaGain( deltaArray, double );

// function to pass to the new line from the file reading process
void	passNewLine(FILE *);

// function to add persistence to the coefficients. This function modifies the input
void	persistence( melArray, int );

