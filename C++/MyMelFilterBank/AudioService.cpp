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

// File Name:		AudioService.cpp
// File Description:	AudioService member-function definitions. This file contains implementations of the
//			member functions prototyped in AudioService.h.

#include <iostream>
#include <fstream>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <stddef.h>

#include "AudioService.h"

using namespace std;

#include "../Libraries/Model/GlobalVariables.h"
#include "../Libraries/Model/Constants.h"
#include "../Libraries/Model/DataTypes.h"
#include "../Libraries/Model/Utilities.h"
#include "../Libraries/Model/OctaveInterface.h"
#include "../Libraries/Model/MatlabInterface.h"

// function to filter AudioVector.mat through a Mel Filter Bank.
void	filterAudioVector( int numberOfFilters, double sampleRate, double sampleWindow, int persistenceValue, double leakyCoefficient,
			   bool computeCepstrum, bool gainControl, bool computeDeltas, bool applyPersistence, bool leakyIntegration,
			   bool kernelConvolution, double delay, std::string fileName, std::string kernelName )
{
	////////////////////////////////////////// loads audio vector X and sample frequency Fs from "AudioVector.mat" file

	audioVector audio;

	audio = loadAudioVectorFile();

	////////////////////////////////////////// audio vector X and sample frequency Fs loaded from "AudioVector.mat" file






	////////////////////////////////////////// reshapes audio vector X conform to Fourier sample period and Fourier window

	audioArray array;

	array = reshapeAudioVector(audio, sampleRate, sampleWindow);
	//array = reshapeAudioVector(audio, 0.01, 0.025); // original training
	//array = reshapeAudioVector(audio, 0.5, 1);

	////////////////////////////////////////// audio vector X reshaped conform to Fourier sample period and Fourier window






	////////////////////////////////////////// saves the spectrum from the different channels in spectrum.mat file

	//saveWindowed(array);

	////////////////////////////////////////// spectrum saved from the different channels in spectrum.mat file






	////////////////////////////////////////// extract the spectrum from the different channels in array

	spectrumArray arrayS;

	#pragma omp critical
	{
		arrayS = extractSpectrum(array);
	}

	////////////////////////////////////////// spectrum extracted from the different channels in array






	////////////////////////////////////////// process the spectrum from the different channels in arrayS

	audioArray processedSpectrum;

	if ( computeCepstrum )
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

	MF = melFilterbank(processedSpectrum, numberOfFilters);

	////////////////////////////////////////// Mel filter banks computed






	////////////////////////////////////////// computes the log and DCT of the Mel filter banks

	melArray	MFCC;

	if ( computeCepstrum )
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

	if ( gainControl )
	{
		adjustGain(MFCC, threshold);
		//adjustDeltaGain(deltas, threshold);
	}

	////////////////////////////////////////// Mel filter banks gain adjusted





	////////////////////////////////////////// computes the Deltas and Delta-Deltas of MFCC

	deltaArray	deltas;

	if ( computeDeltas )
	{
		deltas = deltasDeltaDeltas(MFCC,2);
	}

	////////////////////////////////////////// Deltas and Delta-Deltas of MFCC computed





	////////////////////////////////////////// adds paersistence to the MFCC

	if ( applyPersistence )
		persistence(MFCC,persistenceValue);

	////////////////////////////////////////// paersistence added to the MFCC





	////////////////////////////////////////// applies leakyIntegrator to the MFCC

	if ( leakyIntegration )
		leakyIntegrator(MFCC,leakyCoefficient);

	////////////////////////////////////////// leakyIntegrator applied to the MFCC




	////////////////////////////////////////// applies Kernel Convolution to the MFCC

	melArray	RealArray, LeftImagArray, RightImagArray;

	std::string	RealKernelName = "Real" + kernelName;
	std::string	ImagKernelName = "Imag" + kernelName;
	if ( kernelConvolution )
	{
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				RealArray = applyKernelConvolution( MFCC, RealKernelName, false, false );
			}
			#pragma omp section
			{
				LeftImagArray = applyKernelConvolution( MFCC, ImagKernelName, false, false );
			}
			#pragma omp section
			{
				RightImagArray = applyKernelConvolution( MFCC, ImagKernelName, false, true );
			}
		}
		
		for (int i = 0; i < MFCC.channels; i++)
			free(MFCC.channel[i]);
	}

	////////////////////////////////////////// Kernel Convolution applied to the MFCC





	////////////////////////////////////////// saves the Mel Frequency baks from the different channels

	#pragma omp parallel sections
	{
		#pragma omp section
		{
			saveMFCC(RealArray, ("Real" + fileName));
		}
		#pragma omp section
		{
			saveMFCC(LeftImagArray, ("LeftImag" + fileName));
		}
		#pragma omp section
		{
			saveMFCC(RightImagArray, ("RightImag" + fileName));
		}
	}

	////////////////////////////////////////// Mel Frequency baks saved from the different channels






	////////////////////////////////////////// saves the Deltas and Delta-Deltas of the MFCC in two files

	if ( computeDeltas )
	{
		saveDeltas(deltas, fileName);
	}

	////////////////////////////////////////// Deltas and Delta-Deltas of the MFCC saved in two files






	////////////////////////////////////////// computes the norm
	
	//auto	output = computeNorm(RealArray, LeftImagArray);

	////////////////////////////////////////// 

	//return	output;
} // end function filterAudioVector


// function to read "AudioVector.mat" file
audioVector	loadAudioVectorFile()
{
	int	Fs;
	twodvector<double>	X;

	audioVector audio;

	std::ifstream	inputFile( "../../Octave/AudioVector.mat", ios::in | ios::binary );
	// fopen opens inputFile. Exit program if unable to create inputFile
	if ( !inputFile ) {
		printf( "inputFile could not be opened\n" );
		exit( EXIT_FAILURE );
	}
	else {

		std::stringstream buffer;

		buffer << inputFile.rdbuf();

		inputFile.close();

		std::string	str, auxiliary;

		bool	check_X = false;
		bool	check_Fs = false;

		if (ENABLE_MATLAB_COMPATIBILITY) {
			big_endianness = load_the_header(buffer);
			auto	array_structure = check_next_data_structure(buffer, big_endianness);
			while ( array_structure.more_data ) {

				auxiliary = "Fs";
				if ( array_structure.name.compare(auxiliary) == 0 ) {
					load_numeric_array_to_scalar(array_structure, Fs, buffer, big_endianness);
					check_Fs = true;
				}

				auxiliary = "X";
				if ( array_structure.name.compare(auxiliary) == 0 ) {
					load_numeric_array_to_vector_of_vectors(array_structure, X, buffer, big_endianness);
					check_X = true;
				}

				array_structure = check_next_data_structure(buffer,big_endianness);
			}	


		}
		else {
			while ( std::getline(buffer, str) ) {

				auxiliary = "# name: Fs";
				if ( str.compare(auxiliary) == 0 ) {
					load_scalar(Fs, buffer);
					check_Fs = true;
				}

				auxiliary = "# name: X";
				if ( str.compare(auxiliary) == 0 ) {
					load_matrix_to_vector_of_vectors(X, buffer);
					check_X = true;
				}

			}
		}

		assert(check_X == true);
		assert(check_Fs == true);

		auto	dimensions = get_dimensionality(X);
		assert(dimensions.size() == 2);

		audio.rows = dimensions[0];
		audio.columns = dimensions[1];
		audio.X = (double*)calloc(dimensions[0]*dimensions[1], sizeof(double));					// reserves space for vector

		for(std::size_t i = 0; i < dimensions[0]; i++) {
			for(std::size_t j = 0; j < dimensions[1]; j++) {
				*((audio.X + i*dimensions[1]) + j) = X[i][j];
			}
		}
		audio.Fs = Fs;
	}										// end else

	return audio;
} // end function readAudioVectorFile


// function to reshape audio vector X for multidimensional Fourier transformation issues. The variables fourierSamplingPeriod and fourierWindow are specified in seconds
audioArray	reshapeAudioVector( audioVector audio, double fourierSamplingPeriod, double fourierWindow )
{
	int	i, j, k;
	int	fourierSamplingPeriodLength, fourierWindowLength, chunks;

	char	option;

	double	samplingPeriod;
	double	*array;
	audioArray	output;

	array = NULL;

	for (i = 0; i < 4; i++)								// initializes the array pointers to NULL
	{
		output.channel[i] = NULL;
	}

	samplingPeriod = (double)1/audio.Fs;						// sampling period (in seconds) is the inverse of sampling frequency (in Hz)
	fourierSamplingPeriodLength = (int)ceil(fourierSamplingPeriod/samplingPeriod);	// this is the vector length of the Fourier sampling period
	fourierWindowLength = (int)ceil(fourierWindow/samplingPeriod);			// this is the vector length of the Fourier window

	chunks = (int)(audio.rows/fourierSamplingPeriodLength) - (int)ceil(fourierWindow/fourierSamplingPeriod);	// this is the number of chunks of length equal to fourierSamplingPeriodLength in every channel of X

	output.channels = audio.columns;
	output.chunks = chunks;
	output.fourierSamplingPeriodLength = fourierSamplingPeriodLength;
	output.fourierWindowLength = fourierWindowLength;
	output.sampleFrequency = audio.Fs;
	output.fourierSamplingPeriod = fourierSamplingPeriod;
	output.fourierWindow = fourierWindow;
	output.samplingPeriod = samplingPeriod;

	//printf("\nDo you want to show the windowning parameters on screen? (Y) for yes and any key for no\n\n");
	//scanf(" %c", &option);

	option = 'n';

	if ( option == 'Y')
	{
		/////////////////////////// Shows windowning parameters on screen

		printf("\nchunks = %d\n", chunks);
		printf("fourierSamplingPeriodLength = %d\n", fourierSamplingPeriodLength);
		printf("fourierWindowLength = %d\n", fourierWindowLength);
		printf("fourierSamplingPeriod = %lf\n", fourierSamplingPeriod);
		printf("fourierWindow = %lf\n", fourierWindow);
		printf("samplingPeriod = %lf\n", samplingPeriod);
	}


	for (i = 0; i < audio.columns; i++)
	{
		array = (double*)fftw_malloc(sizeof(double) * chunks*fourierWindowLength);	// reserves space for array
		for (j = 0; j < chunks; j++)
		{
			for (k = 0; k < fourierWindowLength; k++)
			{
				*((array + fourierWindowLength*j) + k) = *((audio.X + fourierSamplingPeriodLength*j) + k);
			}
		}
		output.channel[i] = array;
	}

	//printf("\nDo you want to show the reshaped audio vector channels on screen? (Y) for yes and any key for no\n\n");
	//scanf(" %c", &option);

	option = 'n';

	if ( option == 'Y')
	{
		/////////////////////////// Shows the audio vector channels on screen

		for (i = 0; i < audio.columns; i++)
		{
			printf("\nThis is the channel number %d.\n\n", i+1);
			for (j = 0; j < chunks; j++)
			{
				for (k = 0; k < fourierWindowLength; k++)
				{
					printf(" %lf",*((output.channel[i] + fourierWindowLength*j) + k));
				}
				printf("\n");
			}
			printf("\n\n");
		}
	}
	fftw_free(audio.X);					// frees the memory use by the vector audio.X

	return	output;
} // end function reshapeAudioVector


// function to save the windowed audio vector in Octave format for Octave visualization issues
void	saveWindowed( audioArray windows )
{
	int	i, j, k;

	FILE *filePointer; 								// spectrum.mat file pointer

	// fopen opens file. Exit program if unable to create file
	if ( ( filePointer = fopen( "../../Octave/windowed.mat", "w" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		fprintf( filePointer, "# This is a file created by saveSpectrum function.\n" );

		/////////////////////////// writes the processed spectrum data into file with fprintf

		for (i = 0; i < windows.channels; i++)
		{
			switch ( i+1 )
			{
				case 1:
					fprintf( filePointer, "# name: W1\n" );
				break;

				case 2:
					fprintf( filePointer, "# name: W2\n" );
				break;

				case 3:
					fprintf( filePointer, "# name: W3\n" );
				break;

				case 4:
					fprintf( filePointer, "# name: W4\n" );
				break;

				default:
					printf( "Incorrect option in saveWindowed function. Incorrect channel number %d.\n", i+1 );
					exit( EXIT_FAILURE );
				break;
			}

			fprintf( filePointer, "# type: matrix\n" );
			fprintf( filePointer, "# rows: %d\n", windows.chunks );
			fprintf( filePointer, "# columns: %d\n", windows.fourierWindowLength );
			for (j = 0; j < windows.chunks; j++)
			{
					for (k = 0; k < windows.fourierWindowLength; k++)
					{
						fprintf( filePointer, " %lf", *((windows.channel[i] + j*(windows.fourierWindowLength)) + k) );
					}
					fprintf( filePointer, "\n" );
			}
			fprintf( filePointer, "\n\n" );
		}

		fclose( filePointer );							// fclose closes file
	}										// end else
} // end function saveWindowed


// function to extract the spectrum from audioArray through fftw function
spectrumArray	extractSpectrum( audioArray array )
{
	int	i, j, k;
	double	*input;
	fftw_plan	my_plan;
	fftw_complex	*output;
	spectrumArray spectrum;

	for (i = 0; i < array.channels; i++)
	{
		spectrum.channel[i] = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (array.chunks*(array.fourierWindowLength/2 + 1)));
		input = (double*) fftw_malloc(sizeof(double) * array.fourierWindowLength);
		output = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (array.fourierWindowLength/2 + 1));

		my_plan = fftw_plan_dft_r2c_1d(array.fourierWindowLength, input, output, FFTW_MEASURE);

		for (j = 0; j < array.chunks; j++)
		{
			for (k = 0; k < array.fourierWindowLength; k++)
			{
				*(input + k) = *((array.channel[i] + j*array.fourierWindowLength) + k);
			}

			fftw_execute(my_plan);

			for (k = 0; k < array.fourierWindowLength/2-1; k++)
			{
				memcpy( ((spectrum.channel[i] + j*(spectrum.fourierWindowLength/2 + 1)) + k), (output + k), sizeof( double _Complex ) );
			}
		}
		fftw_free(input);
		fftw_free(output);
		fftw_free(array.channel[i]);
		fftw_destroy_plan(my_plan);
	}

	spectrum.channels = array.channels;
	spectrum.chunks = array.chunks;
	spectrum.fourierSamplingPeriodLength = array.fourierSamplingPeriodLength;
	spectrum.fourierWindowLength = array.fourierWindowLength;
	spectrum.sampleFrequency = array.sampleFrequency;
	spectrum.fourierSamplingPeriod = array.fourierSamplingPeriod;
	spectrum.fourierWindow = array.fourierWindow;
	spectrum.samplingPeriod = array.samplingPeriod;

	return spectrum;
} // end function extractSpectrum


// function to process the spectrum computed by extractSpectrum
audioArray	processSpectrum( spectrumArray spectrum )
{
	int	i, j, k;
	double	*vector;
	double _Complex Z;
	audioArray	output;

	vector = (double*)calloc(spectrum.fourierWindowLength/2 + 1, sizeof(double));					// reserves space for vector
	for (i = 0; i < spectrum.channels; i++)
	{
		output.channel[i] = (double*)calloc(spectrum.chunks*(spectrum.fourierWindowLength/2 + 1), sizeof(double));	// reserves space for output.channel[i]
		for (j = 0; j < spectrum.chunks; j++)
		{
			for (k = 0; k < spectrum.fourierWindowLength/2 + 1; k++)
			{
				memcpy( &Z, ((spectrum.channel[i] + j*(spectrum.fourierWindowLength/2 + 1)) + k), sizeof( double _Complex ) );

				*(vector + k) = 2*pow(cabs(Z), 2);

				*((output.channel[i] + j*(spectrum.fourierWindowLength/2 + 1)) + k) = *(vector + k);
			}
			*((output.channel[i] + j*(spectrum.fourierWindowLength/2 + 1))) = *(vector)/2;
		}
		free(spectrum.channel[i]);
	}

	output.channels = spectrum.channels;
	output.chunks = spectrum.chunks;
	output.fourierSamplingPeriodLength = spectrum.fourierSamplingPeriodLength;
	output.fourierWindowLength = spectrum.fourierWindowLength;
	output.sampleFrequency = spectrum.sampleFrequency;
	output.fourierSamplingPeriod = spectrum.fourierSamplingPeriod;
	output.fourierWindow = spectrum.fourierWindow;
	output.samplingPeriod = spectrum.samplingPeriod;

	return output;
} // end function processSpectrum


// function to process the power spectrum computed by extractSpectrum
audioArray	processPowerSpectrum( spectrumArray spectrum )
{
	int	i, j, k;
	double	*vector;
	double _Complex Z;
	audioArray	output;

	vector = (double*)calloc(spectrum.fourierWindowLength/2 + 1, sizeof(double));					// reserves space for vector
	for (i = 0; i < spectrum.channels; i++)
	{
		output.channel[i] = (double*)calloc(spectrum.chunks*(spectrum.fourierWindowLength/2 + 1), sizeof(double));	// reserves space for output.channel[i]
		for (j = 0; j < spectrum.chunks; j++)
		{
			for (k = 0; k < spectrum.fourierWindowLength/2 + 1; k++)
			{
				memcpy( &Z, ((spectrum.channel[i] + j*(spectrum.fourierWindowLength/2)) + k), sizeof( double _Complex ) );

				*(vector + k) = 2*pow(cabs(Z), 2)/spectrum.fourierWindowLength;

				*((output.channel[i] + j*(spectrum.fourierWindowLength/2)) + k) = *(vector + k);
			}
			*((output.channel[i] + j*(spectrum.fourierWindowLength/2 + 1))) = *(vector)/2;
		}
		free(spectrum.channel[i]);
	}

	output.channels = spectrum.channels;
	output.chunks = spectrum.chunks;
	output.fourierSamplingPeriodLength = spectrum.fourierSamplingPeriodLength;
	output.fourierWindowLength = spectrum.fourierWindowLength;
	output.sampleFrequency = spectrum.sampleFrequency;
	output.fourierSamplingPeriod = spectrum.fourierSamplingPeriod;
	output.fourierWindow = spectrum.fourierWindow;
	output.samplingPeriod = spectrum.samplingPeriod;

	return output;
} // end function processPowerSpectrum


// function to save the processed spectrum in Octave format for Octave visualization issues
void	saveSpectrum( audioArray processedSpect )
{
	int	i, j, k;

	FILE *filePointer; 								// spectrum.mat file pointer

	// fopen opens file. Exit program if unable to create file
	if ( ( filePointer = fopen( "../../Octave/spectrum.mat", "w" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		fprintf( filePointer, "# This is a file created by saveSpectrum function.\n" );

		/////////////////////////// writes the processed spectrum data into file with fprintf

		for (i = 0; i < processedSpect.channels; i++)
		{
			switch ( i+1 )
			{
				case 1:
					fprintf( filePointer, "# name: P1\n" );
				break;

				case 2:
					fprintf( filePointer, "# name: P2\n" );
				break;

				case 3:
					fprintf( filePointer, "# name: P3\n" );
				break;

				case 4:
					fprintf( filePointer, "# name: P4\n" );
				break;

				default:
					printf( "Incorrect option in saveSpectrum function. Incorrect channel number %d.\n", i+1 );
					exit( EXIT_FAILURE );
				break;
			}

			fprintf( filePointer, "# type: matrix\n" );
			fprintf( filePointer, "# rows: %d\n", processedSpect.chunks );
			fprintf( filePointer, "# columns: %d\n", processedSpect.fourierWindowLength/2+1 );
			for (j = 0; j < processedSpect.chunks; j++)
			{
					for (k = 0; k < processedSpect.fourierWindowLength/2+1; k++)
					{
						fprintf( filePointer, " %lf", *((processedSpect.channel[i] + j*(processedSpect.fourierWindowLength/2+1)) + k) );
					}
					fprintf( filePointer, "\n" );
			}
			fprintf( filePointer, "\n\n" );
		}

		fclose( filePointer );							// fclose closes file
	}										// end else
} // end function saveSpectrum


// function to compute the Mel filterbank
melArray	melFilterbank( audioArray spectrum, int filters )
{
	int	c, i, j, k;
	int	nfft;
	int	*dicreteHertzPositions;
	double	auxiliary, lowerFrequency, upperFrequency, melLowerFrequency, melUpperFrequency, melBandWidth, melBandWidthFilter, samplerate;
	double	*melPositions, *hertzPositions;
	melArray	output;

	lowerFrequency = 300;								// this is the lower frequency in Hz
	upperFrequency = 8000;								// this is the upper frequency in Hz

	melLowerFrequency = hertz2Mel(lowerFrequency);					// this is the lower frequency in Mel
	melUpperFrequency = hertz2Mel(upperFrequency);					// this is the upper frequency in Mel

	//printf("\nmelLowerFrequency : %lf\n", melLowerFrequency);
	//printf("\nmelUpperFrequency : %lf\n", melUpperFrequency);

	melBandWidth = melUpperFrequency - melLowerFrequency;

	melBandWidthFilter = melBandWidth/(filters+1);

	melPositions = (double*)calloc(filters+2, sizeof(double));			// reserves space for melPositions
	hertzPositions = (double*)calloc(filters+2, sizeof(double));			// reserves space for hertzPositions
	dicreteHertzPositions = (int*)calloc(filters+2, sizeof(int));			// reserves space for dicreteHertzPositions

	/* We don't have the frequency resolution required to put filters at the exact points calculated above,
	so we need to round those frequencies to the nearest FFT bin. This process does not affect the
	accuracy of the features. To convert the frequncies to fft bin numbers we need to know the FFT size
	and the sample rate, f(i) = floor((nfft+1)*h(i)/samplerate) */

	nfft = spectrum.fourierWindowLength;
	samplerate = spectrum.sampleFrequency;

	//nfft = 512;
	//samplerate = 16000;

	auxiliary = melLowerFrequency;
	for(i = 0; i < filters+2; i++)
	{
		*(melPositions + i) = auxiliary;
		*(hertzPositions + i) = mel2Hertz(*(melPositions + i));
		auxiliary = auxiliary + melBandWidthFilter;
		//printf("\nmelPositions: %lf, hertzPositions: %lf\n", *(melPositions + i), *(hertzPositions + i));
		*(dicreteHertzPositions + i) = floor((nfft+1)*(*(hertzPositions + i))/samplerate);
		//printf("\ndicreteHertzPositions: %d\n", *(dicreteHertzPositions + i));
	}

	for (c = 0; c < spectrum.channels; c++)
	{
		output.channel[c] = (double*)calloc(spectrum.chunks*filters, sizeof(double));	// reserves space for coefficients
		output.filter = (double*)calloc(filters*(spectrum.fourierWindowLength/2 + 1), sizeof(double));	// reserves space for coefficients
		for (i = 0; i < spectrum.chunks; i++)
		{
			for (j = 0; j < filters; j++)
			{
				for (k = 0; k < spectrum.fourierWindowLength/2 + 1; k++)
				{

					if ( k+1 >= *(dicreteHertzPositions + j) && k+1 < *(dicreteHertzPositions + j+1) )
					{
						*((output.filter + j*(spectrum.fourierWindowLength/2+1)) + k) = ( (k+1) - (double)*(dicreteHertzPositions + j) )/( *(dicreteHertzPositions + j+1) - *(dicreteHertzPositions + j) );
						*((output.channel[c] + i*filters) + j) = *((output.channel[c] + i*filters) + j) + (( (k+1) - (double)*(dicreteHertzPositions + j) )/( *(dicreteHertzPositions + j+1) - *(dicreteHertzPositions + j) ))*( *((spectrum.channel[c] + i*(spectrum.fourierWindowLength/2+1) + k)) );
					}

					if ( k+1 >= *(dicreteHertzPositions + j+1) && k+1 < *(dicreteHertzPositions + j+2) )
					{
						*((output.filter + j*(spectrum.fourierWindowLength/2+1)) + k) = ( (double)*(dicreteHertzPositions + j+2) - (k+1) )/( *(dicreteHertzPositions + j+2) - *(dicreteHertzPositions + j+1) );
						*((output.channel[c] + i*filters) + j) = *((output.channel[c] + i*filters) + j) + (( (double)*(dicreteHertzPositions + j+2) - (k+1) )/( *(dicreteHertzPositions + j+2) - *(dicreteHertzPositions + j+1) ))*( *((spectrum.channel[c] + i*(spectrum.fourierWindowLength/2+1) + k)) );
					}
				}
			}
		}
		free(spectrum.channel[c]);
	}

	output.filters = filters;
	output.channels = spectrum.channels;
	output.chunks = spectrum.chunks;
	output.fourierSamplingPeriodLength = spectrum.fourierSamplingPeriodLength;
	output.fourierWindowLength = spectrum.fourierWindowLength;
	output.sampleFrequency = spectrum.sampleFrequency;
	output.fourierSamplingPeriod = spectrum.fourierSamplingPeriod;
	output.fourierWindow = spectrum.fourierWindow;
	output.samplingPeriod = spectrum.samplingPeriod;

	free(melPositions);
	free(hertzPositions);
	free(dicreteHertzPositions);

	return output;
} // end function melFilterbank


// function to compute the log of the Mel filterbank. This function modifies the input
void	logMelFilterbank( melArray Array )
{
	int	i, j, k;

	for (i = 0; i < Array.channels; i++)
	{
		for (j = 0; j < Array.chunks; j++)
		{
			for (k = 0; k < Array.filters; k++)
			{
				*((Array.channel[i] + j*Array.filters) + k) = log( *((Array.channel[i] + j*Array.filters) + k) );
			}
		}
	}
} // end function logMelFilterbank


// function to compute the Discrete Cosine Transform (DCT) of the log Mel filterbank
melArray	dctLogMelFilterbank( melArray Array )
{
	int	i, j, k, n;
	double	auxiliary;

	melArray	output;

	for (i = 0; i < Array.channels; i++)
	{
		output.channel[i] = (double*)calloc(Array.chunks*Array.filters, sizeof(double));	// reserves space for the output channel
		for (j = 0; j < Array.chunks; j++)
		{
			auxiliary = 0;
			for (n = 0; n < Array.filters; n++)
			{
				 //auxiliary = auxiliary + (1/sqrt(2)) * log( *((Array.channel[i] + j*Array.filters) + n) ) * cos((2*n+1)*0*M_PI/(2*Array.filters));
				 auxiliary = auxiliary + sqrt((double)1/Array.filters) * log( *((Array.channel[i] + j*Array.filters) + n) );
			}
			//*((Array.channel[i] + j*Array.filters) + 0) = auxiliary;
			*(output.channel[i] + j*Array.filters) = auxiliary;

			for (k = 1; k < Array.filters; k++)
			{
				auxiliary = 0;
				for (n = 0; n < Array.filters; n++)
				{
					 auxiliary = auxiliary + sqrt((double)2/Array.filters) * log( *((Array.channel[i] + j*Array.filters) + n) ) * cos((2*n+1)*k*M_PI/((double)(2*Array.filters)));
				}
				*((output.channel[i] + j*Array.filters) + k) = auxiliary;
			}
		}
		free(Array.channel[i]);
	}

	output.filters = Array.filters;
	output.channels = Array.channels;
	output.chunks = Array.chunks;
	output.fourierSamplingPeriodLength = Array.fourierSamplingPeriodLength;
	output.fourierWindowLength = Array.fourierWindowLength;
	output.sampleFrequency = Array.sampleFrequency;
	output.fourierSamplingPeriod = Array.fourierSamplingPeriod;
	output.fourierWindow = Array.fourierWindow;
	output.samplingPeriod = Array.samplingPeriod;
	output.filter = Array.filter;

	return	output;
} // end function dctLogMelFilterbank


// function to compute the Deltas and Delta-Deltas of the MFCC
deltaArray	deltasDeltaDeltas( melArray Array, int N )
{
	int	i, j, t, n;
	double	numeartor, denominator;
	deltaArray	output;


	// This is for the Deltas computation
	for (i = 0; i < Array.channels; i++)
	{
		output.channelDeltas[i] = (double*)calloc(Array.chunks*Array.filters, sizeof(double));	// reserves space for the Deltas
		for (j = 0; j < Array.chunks; j++)
		{
			for (t = 0; t < Array.filters; t++)
			{
				numeartor = 0;
				denominator = 0;
				for (n = 1; n <= N; n++)
				{
					if ( j-n < 0 )
					{
						numeartor = numeartor + n*( *((Array.channel[i] + (j+n)*Array.filters) + t) );
						denominator = denominator + pow(n,2);
					}
					else if ( j+n > Array.chunks-1 )
					{
						numeartor = numeartor + n*( - *((Array.channel[i] + (j-n)*Array.filters) + t) );
						denominator = denominator + pow(n,2);
					}
					else
					{
						numeartor = numeartor + n*( *((Array.channel[i] + (j+n)*Array.filters) + t) - *((Array.channel[i] + (j-n)*Array.filters) + t) );
						denominator = denominator + pow(n,2);
					}
				}
				*((output.channelDeltas[i] + j*Array.filters) + t) = numeartor/(2*denominator);
			}
		}
		//free(Array.channel[i]);
	}

	// This is for the Delta-Deltas computation
	for (i = 0; i < Array.channels; i++)
	{
		output.channelDeltaDeltas[i] = (double*)calloc(Array.chunks*Array.filters, sizeof(double));	// reserves space for the Delta-Deltas
		for (j = 0; j < Array.chunks; j++)
		{
			for (t = 0; t < Array.filters; t++)
			{
				numeartor = 0;
				denominator = 0;
				for (n = 1; n <= N; n++)
				{
					if ( j-n < 0 )
					{
						numeartor = numeartor + n*( *((output.channelDeltas[i] + (j+n)*Array.filters) + t) + 0 );
						denominator = denominator + pow(n,2);
					}
					else if ( j+n > Array.chunks-1 )
					{
						numeartor = numeartor + n*( 0 - *((output.channelDeltas[i] + (j-n)*Array.filters) + t) );
						denominator = denominator + pow(n,2);
					}
					else
					{
						numeartor = numeartor + n*( *((output.channelDeltas[i] + (j+n)*Array.filters) + t) - *((output.channelDeltas[i] + (j-n)*Array.filters) + t) );
						denominator = denominator + pow(n,2);
					}
				}
				*((output.channelDeltaDeltas[i] + j*Array.filters) + t) = numeartor/(2*denominator);
			}
		}
	}

	output.filters = Array.filters;
	output.channels = Array.channels;
	output.chunks = Array.chunks;
	output.fourierSamplingPeriodLength = Array.fourierSamplingPeriodLength;
	output.fourierWindowLength = Array.fourierWindowLength;
	output.sampleFrequency = Array.sampleFrequency;
	output.fourierSamplingPeriod = Array.fourierSamplingPeriod;
	output.fourierWindow = Array.fourierWindow;
	output.samplingPeriod = Array.samplingPeriod;

	return output;
} // end function deltasDeltaDeltas


// function to save the MFCC in a Octave format file
void	saveMFCC( melArray mel, std::string name )
{
	std::stringstream	outputStream;

	// file preamble.
	if (ENABLE_MATLAB_COMPATIBILITY) {
		save_the_header(outputStream);
	}
	else {
		outputStream << "# This is a file created by saveMFCC member function in AudioService from," << endl;
		outputStream << "# C++ implementation code of Hierarchical Spectro-Temporal Model (HSTM)." << endl;
		outputStream << "# Author: Dematties Dario Jesus." << endl;
		outputStream << "\n\n" << endl;
	}


	for (int i = 0; i < mel.channels; i++) {
		std::string	variableName = "MFCC";
		if ( i+1 > 4 || i+1 < 1 ) {
			printf( "Incorrect option in saveMFCC function. Incorrect channel number %d.\n", i+1 );
			exit( EXIT_FAILURE );
		}
		else {
			variableName += std::to_string(i+1);
		}

		twodvector<double>	MFCC;
		MFCC.resize(mel.chunks);
		for (int j = 0; j < mel.chunks; j++) {
			MFCC[j].resize(mel.filters);
			for (int k = 0; k < mel.filters; k++)
				MFCC[j][k] = *((mel.channel[i] + j*(mel.filters)) + k);
		}

		// saves MFCC channel
		if (ENABLE_MATLAB_COMPATIBILITY)
			save_vector_of_vectors_as_numeric_array(variableName, MFCC, outputStream);
		else
			save_vector_of_vectors_as_matrix(variableName, MFCC, outputStream);

	}

	std::string	variableName = "Filters";
	twodvector<double>	filter;
	filter.resize(mel.filters);
	for (int j = 0; j < mel.filters; j++) {
		filter[j].resize(mel.fourierWindowLength/2+1);
		for (int k = 0; k < mel.fourierWindowLength/2+1; k++)
			filter[j][k] = *((mel.filter + j*(mel.fourierWindowLength/2+1)) + k);
	}

	// saves the Filters
	if (ENABLE_MATLAB_COMPATIBILITY)
		save_vector_of_vectors_as_numeric_array(variableName, filter, outputStream);
	else
		save_vector_of_vectors_as_matrix(variableName, filter, outputStream);

	std::string	path = "../../Octave/";
	std::string	extension = ".mat";
	std::string	fileName = path + name + extension;
	ofstream	outputFile;
	outputFile.open(fileName, ios::out | ios::binary | ios::trunc);
	if (!outputFile) {
		printf( "outputFile could not be opened\n" );
		exit( EXIT_FAILURE );
	}
	else {
		outputFile << outputStream.rdbuf();
		outputFile.close();
	}
} // end function saveMFCC


// function to save the Deltas and Delta-Deltas in a Octave format file
void	saveDeltas( deltaArray delta, std::string name )
{
	int	i, j, k;

	FILE *filePointer; 								// Deltas.mat file pointer
	std::string	path = "../../Octave/";
	std::string	extension = "_Deltas.mat";
	std::string	newName = path + name + extension;

	/////////////////////// This file is to save the Deltas	///////////////////////

	// fopen opens file. Exit program if unable to create file
	const char * c_newName = newName.c_str();
	if ( ( filePointer = fopen( c_newName, "w" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		fprintf( filePointer, "# This is a file created by saveDeltas function.\n" );

		/////////////////////////// writes the MFCC data into file with fprintf

		for (i = 0; i < delta.channels; i++)
		{
			switch ( i+1 )
			{
				case 1:
					fprintf( filePointer, "# name: Deltas1\n" );
				break;

				case 2:
					fprintf( filePointer, "# name: Deltas2\n" );
				break;

				case 3:
					fprintf( filePointer, "# name: Deltas3\n" );
				break;

				case 4:
					fprintf( filePointer, "# name: Deltas4\n" );
				break;

				default:
					printf( "Incorrect option in saveDeltas function. Incorrect channel number %d.\n", i+1 );
					exit( EXIT_FAILURE );
				break;
			}

			fprintf( filePointer, "# type: matrix\n" );
			fprintf( filePointer, "# rows: %d\n", delta.chunks );
			fprintf( filePointer, "# columns: %d\n", delta.filters );
			for (j = 0; j < delta.chunks; j++)
			{
					for (k = 0; k < delta.filters; k++)
					{
						fprintf( filePointer, " %lf", *((delta.channelDeltas[i] + j*(delta.filters)) + k) );
					}
					fprintf( filePointer, "\n" );
			}
			fprintf( filePointer, "\n\n" );
		}

		fclose( filePointer );							// fclose closes file
	}										// end else



	newName.clear();
	extension.clear();
	/////////////////////// This file is to save the Delta-Deltas	///////////////////////

	extension = "_DeltasDeltas.mat";
	newName = path + name + extension;
	// fopen opens file. Exit program if unable to create file
	const char * c_newName1 = newName.c_str();
	if ( ( filePointer = fopen( c_newName1, "w" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		fprintf( filePointer, "# This is a file created by saveDeltas function.\n" );

		/////////////////////////// writes the MFCC data into file with fprintf

		for (i = 0; i < delta.channels; i++)
		{
			switch ( i+1 )
			{
				case 1:
					fprintf( filePointer, "# name: DeltaDeltas1\n" );
				break;

				case 2:
					fprintf( filePointer, "# name: DeltaDeltas2\n" );
				break;

				case 3:
					fprintf( filePointer, "# name: DeltaDeltas3\n" );
				break;

				case 4:
					fprintf( filePointer, "# name: DeltaDeltas4\n" );
				break;

				default:
					printf( "Incorrect option in saveDeltas function. Incorrect channel number %d.\n", i+1 );
					exit( EXIT_FAILURE );
				break;
			}

			fprintf( filePointer, "# type: matrix\n" );
			fprintf( filePointer, "# rows: %d\n", delta.chunks );
			fprintf( filePointer, "# columns: %d\n", delta.filters );
			for (j = 0; j < delta.chunks; j++)
			{
					for (k = 0; k < delta.filters; k++)
					{
						fprintf( filePointer, " %lf", *((delta.channelDeltaDeltas[i] + j*(delta.filters)) + k) );
					}
					fprintf( filePointer, "\n" );
			}
			fprintf( filePointer, "\n\n" );
		}

		fclose( filePointer );							// fclose closes file
	}										// end else
} // end function saveDeltas


// function to transform from Hz to Mel frequency
double	hertz2Mel( double hertzFrequency )
{
	return (1125 * log(1 + hertzFrequency/700));
} // end function hertz2Mel


// function to transform from Mel to Hz frequency
double	mel2Hertz( double melFrequency )
{
	return (700 * (exp(melFrequency/1125) - 1));
} // end function mel2Hertz


// function to adjust the gain of the coefficients. This function modifies the input
void	adjustGain( melArray Array, double threshold )
{
	int	i, j, k;
	double	auxiliary[2];

	for (i = 0; i < Array.channels; i++)
	{
		auxiliary[0] = 0;
		for (j = 0; j <= Array.chunks; j++)
		{
			auxiliary[1] = auxiliary[0];
			auxiliary[0] = 0;
			for (k = 0; k < Array.filters; k++)
			{
				if ( (j < Array.chunks) && (*((Array.channel[i] + j*Array.filters) + k) > auxiliary[0]) )
					auxiliary[0] = *((Array.channel[i] + j*Array.filters) + k);

				if ( (j != 0) && (auxiliary[1] >= threshold) )
					*((Array.channel[i] + (j-1)*Array.filters) + k) = *((Array.channel[i] + (j-1)*Array.filters) + k)/auxiliary[1];
			}
		}
	}
} // end function adjustGain


// function to adjust the gain of the deltas and delta-deltas coefficients. This function modifies the input
void	adjustDeltaGain( deltaArray Array, double threshold )
{
	int	i, j, k;
	double	auxiliary[2];

	for (i = 0; i < Array.channels; i++)
	{
		auxiliary[0] = 0;
		for (j = 0; j <= Array.chunks; j++)
		{
			auxiliary[1] = auxiliary[0];
			auxiliary[0] = 0;
			for (k = 0; k < Array.filters; k++)
			{
				if ( j < Array.chunks )
					auxiliary[0] = auxiliary[0] + abs(*((Array.channelDeltas[i] + j*Array.filters) + k));

				if ( (j != 0) && (auxiliary[1] >= threshold) )
					*((Array.channelDeltas[i] + (j-1)*Array.filters) + k) = *((Array.channelDeltas[i] + (j-1)*Array.filters) + k)/auxiliary[1];
			}
		}
	}

	for (i = 0; i < Array.channels; i++)
	{
		auxiliary[0] = 0;
		for (j = 0; j <= Array.chunks; j++)
		{
			auxiliary[1] = auxiliary[0];
			auxiliary[0] = 0;
			for (k = 0; k < Array.filters; k++)
			{
				if ( j < Array.chunks )
					auxiliary[0] = auxiliary[0] + abs(*((Array.channelDeltaDeltas[i] + j*Array.filters) + k));

				if ( (j != 0) && (auxiliary[1] >= threshold) )
					*((Array.channelDeltaDeltas[i] + (j-1)*Array.filters) + k) = *((Array.channelDeltaDeltas[i] + (j-1)*Array.filters) + k)/auxiliary[1];
			}
		}
	}
} // end function adjustDeltaGain


// function to pass to the new line from the file reading process
void	passNewLine(FILE *filePointer)
{
	char	newLine;

	newLine = getc(filePointer);
	while ( newLine != '\n' )
		newLine = getc(filePointer);
} // end function passNewLine


// function to add persistence to the coefficients. This function modifies the input
void	persistence( melArray Array, int persistent )
{
	int	i, j, k, count = 0;
	bool	silence[2] = {true,true};

	for (i = 0; i < Array.channels; i++)
	{
		for (j = 0; j < Array.chunks; j++)
		{
			for (k = 0; k < Array.filters; k++)
			{
				if ( *((Array.channel[i] + j*Array.filters) + k) > 0.000001 )
				{
					silence[1] = false;
					continue;
				}
			}

			if ( !silence[0] && silence[1] && count < persistent )
			{
				for (k = 0; k < Array.filters; k++)
					*((Array.channel[i] + j*Array.filters) + k) = *((Array.channel[i] + (j-1)*Array.filters) + k);

				silence[1] = false;
				count++;
			}
			else
			{
				count = 0;
			}

			silence[0] = silence[1];
			silence[1] = true;
		}
	}
} // end function persistence


// function to apply leaky integration to the input. This function modifies the input
void	leakyIntegrator( melArray Array, double coefficient )
{
	int	i, j, k;

	for (i = 0; i < Array.channels; i++)
	{
		for (j = 0; j < Array.chunks; j++)
		{
			if ( j == 0 ) {
				for (k = 0; k < Array.filters; k++)
					*((Array.channel[i] + j*Array.filters) + k) = (1 - coefficient) * *((Array.channel[i] + j*Array.filters) + k) + 0;
			}
			else {
				for (k = 0; k < Array.filters; k++)
					*((Array.channel[i] + j*Array.filters) + k) = (1 - coefficient) * *((Array.channel[i] + j*Array.filters) + k) + coefficient * *((Array.channel[i] + (j-1)*Array.filters) + k);
			}
		}
	}
} // end function leakyIntegrator


void	convolve(const std::vector<double> Signal/* SignalLen */, size_t SignalLen,
		 const std::vector<double> Kernel/* KernelLen */, size_t KernelLen,
		 std::vector<double>& Result/* SignalLen + KernelLen - 1 */)
{
	size_t n;

	for (n = 0; n < SignalLen + KernelLen - 1; n++)
	{
		size_t kmin, kmax, k;

		Result[n] = 0;

		kmin = (n >= KernelLen - 1) ? n - (KernelLen - 1) : 0;
		kmax = (n < SignalLen - 1) ? n : SignalLen - 1;

		for (k = kmin; k <= kmax; k++)
		{
			Result[n] += Signal[k] * Kernel[n - k];
		}
	}
} // end function convolve


// function to apply kernel convolution to the Mel Filter-Bank.
// Though this function does not modify Array, it can free such structure depending on freeArray boolean variable.
melArray	applyKernelConvolution( melArray Array, std::string kernelName, bool freeArray, bool negativeKernel )
{
	int i, j, k;

	std::vector<double>	signal(Array.filters);
	std::vector<double>	kernel(Array.filters);
	std::vector<double>	result(2*Array.filters - 1);

	std::vector<double>	AuxiliaryKernel;

	AuxiliaryKernel.resize(Array.filters);

	melArray	output;

	std::string	str;
	std::string	STR;

	bool	check_Aux = false;

	kernelName = "../../Octave/" + kernelName + ".mat";
	// open a file in read mode.
	ifstream infile; 
	infile.open(kernelName, ios::in | std::ifstream::binary);

	if (ENABLE_MATLAB_COMPATIBILITY) {
		big_endianness = load_the_header(infile);
		auto	array_structure = check_next_data_structure(infile, big_endianness);
		while ( array_structure.more_data ) {
			STR = "Aux";
			if ( array_structure.name.compare(STR) == 0 ) {
				load_numeric_array_to_vector(array_structure, AuxiliaryKernel, infile, big_endianness);
				check_Aux = true;
			}

			array_structure = check_next_data_structure(infile,big_endianness);
		}	



	}
	else {
		while ( std::getline(infile, str) )
		{
			STR = "# name: Aux";
			if ( str.compare(STR) == 0 ) {
				load_matrix_to_vector(AuxiliaryKernel, infile);
				check_Aux = true;
			}
		}
	}

	infile.close();
	assert(check_Aux == true);

	for (k = 0; k < Array.filters; k++)
	{
		if ( negativeKernel ) {
			kernel[k] = -AuxiliaryKernel[k];
		}
		else {
			kernel[k] = AuxiliaryKernel[k];
		}
	}

	for (i = 0; i < Array.channels; i++)
	{
		output.channel[i] = (double*)calloc(Array.chunks*Array.filters, sizeof(double));	// reserves space for the output channel
		for (j = 0; j < Array.chunks; j++)
		{
			for (k = 0; k < Array.filters; k++)
				signal[k] = *((Array.channel[i] + j*Array.filters) + k);

			convolve(signal, Array.filters,
				 kernel, Array.filters,
				 result);

			for (k = 0; k < Array.filters; k++)
				*((output.channel[i] + j*Array.filters) + k) = result[k + (int)ceil(double(Array.filters)/2)];
		}
		if ( freeArray )
			free(Array.channel[i]);
	}

	output.filters = Array.filters;
	output.channels = Array.channels;
	output.chunks = Array.chunks;
	output.fourierSamplingPeriodLength = Array.fourierSamplingPeriodLength;
	output.fourierWindowLength = Array.fourierWindowLength;
	output.sampleFrequency = Array.sampleFrequency;
	output.fourierSamplingPeriod = Array.fourierSamplingPeriod;
	output.fourierWindow = Array.fourierWindow;
	output.samplingPeriod = Array.samplingPeriod;
	output.filter = Array.filter;

	return	output;
} // end function applyKernelConvolution


/*// computes norm of the components*/
//twodvector<double>	computeNorm( const melArray& component1, const melArray& component2 )
//{
	//assert(component1.filters == component2.filters);
	//assert(component1.channels == component2.channels);
	//assert(component1.chunks == component2.chunks);
	//assert(component1.fourierWindowLength == component2.fourierWindowLength);
	//assert(component1.fourierWindowLength == component2.fourierWindowLength);
	//assert(component1.sampleFrequency == component2.sampleFrequency);
	//assert(component1.fourierSamplingPeriod == component2.fourierSamplingPeriod);
	//assert(component1.fourierWindow == component2.fourierWindow);
	//assert(component1.samplingPeriod == component2.samplingPeriod);
	//std::vector<std::vector<double>>	output;

	//output.resize(component1.channels);
	//for (int i = 0; i < component1.channels; i++)
	//{
		//output[i].resize(component1.chunks*component1.filters);	// reserves space for the output channel
		//#pragma omp parallel for default(none) shared(i, component1, component2, output)
		//for (int j = 0; j < component1.chunks; j++)
		//{
			//for (int k = 0; k < component1.filters; k++) {
				//auto	first = *((component1.channel[i] + j*component1.filters) + k);
				//auto	second = *((component2.channel[i] + j*component2.filters) + k);
				//auto	auxiliary = std::sqrt(std::pow(first,2)+std::pow(second,2));
				
				//#pragma omp critical
				//{
					//output[i][j*component1.filters + k] = auxiliary;
				//}
			//}

		//}
	//}

	//return	output;
//} // end function computeNorm



