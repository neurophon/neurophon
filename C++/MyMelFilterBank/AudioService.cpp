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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cmath>
#include <complex.h>

#include <iostream>

#include "AudioService.h"

// function to filter AudioVector.mat through a Mel Filter Bank.
void	filterAudioVector( int numberOfFilters, double sampleRate, double sampleWindow, int persistenceValue, double leakyCoefficient,
			   bool computeCepstrum, bool gainControl, bool computeDeltas, bool applyPersistence, bool leakyIntegration,
			   double delay, std::string fileName )
{
	system("clear");					// clears the screen

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

	arrayS = extractSpectrum(array);

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





	////////////////////////////////////////// saves the Mel Frequency baks from the different channels

	saveMFCC(MFCC, fileName);

	////////////////////////////////////////// Mel Frequency baks saved from the different channels






	////////////////////////////////////////// saves the Deltas and Delta-Deltas of the MFCC in two files

	if ( computeDeltas )
	{
		saveDeltas(deltas, fileName);
	}

	////////////////////////////////////////// Deltas and Delta-Deltas of the MFCC saved in two files

} // end function filterAudioVector


// function to read "AudioVector.mat" file
audioVector	loadAudioVectorFile()
{
	int	i, j;
	int	rows, columns;

	char	pathFile[100];

	char	vectorName[11];
	char	vectorType[16];
	char	vectorRows[8];
	char	vectorColumns[11];

	char	scalarName[12];
	char	scalarType[16];

	int	Fs;
	double	*X;

	audioVector audio;

	FILE *filePointer; 								// ConnectomeNode.conn file pointer

	strcpy(pathFile, "../../Octave/AudioVector.mat");

	// fopen opens file. Exit program if unable to create file
	if ( ( filePointer = fopen( pathFile, "r" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		passNewLine(filePointer);

		/////////////////////////// reads the vector name
		fgets(vectorName, 11, filePointer);
		if ( strcmp(vectorName, "# name: X\n") != 0)
		{
			printf("File corrupted; read: %s\nwhen it must have been: # name: X\n", vectorName);
			exit( EXIT_FAILURE );
		}

		/////////////////////////// reads the vector type
		fgets(vectorType, 16, filePointer);
		if ( strcmp(vectorType, "# type: matrix\n") != 0)
		{
			printf("File corrupted; read: %s\nwhen it must have been: # type: matrix\n", vectorType);
			exit( EXIT_FAILURE );
		}

		/////////////////////////// reads the vector rows
		fgets(vectorRows, 8, filePointer);
		if ( strcmp(vectorRows, "# rows:") != 0)
		{
			printf("File corrupted; read: %s\nwhen it must have been: # rows: \n", vectorRows);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &rows);
		passNewLine(filePointer);

		/////////////////////////// reads the vector columns
		fgets(vectorColumns, 11, filePointer);
		if ( strcmp(vectorColumns, "# columns:") != 0)
		{
			printf("File corrupted; read: %s\nwhen it must have been: # columns: \n", vectorColumns);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &columns);
		passNewLine(filePointer);

		X = (double*)calloc(rows*columns, sizeof(double));	// reserves space for vector
		for (i = 0; i < rows; i++)
		{
				for (j = 0; j < columns; j++)
				{
					fscanf(filePointer, " %lf", ((X + i*columns) + j) );
				}
				passNewLine(filePointer);
		}
		passNewLine(filePointer);
		passNewLine(filePointer);

		/////////////////////////// reads the scalar name
		fgets(scalarName, 12, filePointer);
		if ( strcmp(scalarName, "# name: Fs\n") != 0)
		{
			printf("File corrupted; read: %s\nwhen it must have been: # name: Fs\n", scalarName);
			exit( EXIT_FAILURE );
		}

		/////////////////////////// reads the vector type
		fgets(scalarType, 16, filePointer);
		if ( strcmp(scalarType, "# type: scalar\n") != 0)
		{
			printf("File corrupted; read: %s\nwhen it must have been: # type: scalar\n", scalarType);
			exit( EXIT_FAILURE );
		}

		fscanf(filePointer, "%d", &Fs);

		audio.X = X;
		audio.rows = rows;
		audio.columns = columns;
		audio.Fs = Fs;

/////////////////////////////////////////////////////////////////// Shows the connectome arrays on screen

		char	option;

		//printf("\nDo you want to show the audio vector X and the sample frequency Fs on screen? (Y) for yes and any key for no\n\n");
		//scanf(" %c", &option);

		option = 'n';

		if ( option == 'Y')
		{
			/////////////////////////// Shows audio vector X on screen

			printf("\n\n%s\n\n", vectorName);
			printf("\n\n%s\n\n", vectorType);
			printf("rows: %d\n\n", rows);
			printf("columns: %d\n\n", columns);
			for (i = 0; i < rows; i++)
			{
					for (j = 0; j < columns; j++)
					{
						printf(" %lf", *((X + i*columns) + j) );
					}
					printf("\n");
			}
			printf("\n\n");

			/////////////////////////// Shows audio scalar sample frequency Fs on screen

			printf("\n\n%s\n\n", scalarName);
			printf("%s\n\n", scalarType);

			printf(" %d", Fs);
			printf("\n\n");
		}
	fclose( filePointer );								// fclose closes file
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
	int	i, j, k;

	FILE *filePointer; 								// "name".mat file pointer
	std::string	path = "../../Octave/";
	std::string	extension = ".mat";
	name = path + name + extension;

	// fopen opens file. Exit program if unable to create file
	const char * c_name = name.c_str();
	if ( ( filePointer = fopen( c_name, "w" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		fprintf( filePointer, "# This is a file created by saveMFCC function.\n" );

		/////////////////////////// writes the MFCC data into file with fprintf

		for (i = 0; i < mel.channels; i++)
		{
			switch ( i+1 )
			{
				case 1:
					fprintf( filePointer, "# name: MFCC1\n" );
				break;

				case 2:
					fprintf( filePointer, "# name: MFCC2\n" );
				break;

				case 3:
					fprintf( filePointer, "# name: MFCC3\n" );
				break;

				case 4:
					fprintf( filePointer, "# name: MFCC4\n" );
				break;

				default:
					printf( "Incorrect option in saveMFCC function. Incorrect channel number %d.\n", i+1 );
					exit( EXIT_FAILURE );
				break;
			}

			fprintf( filePointer, "# type: matrix\n" );
			fprintf( filePointer, "# rows: %d\n", mel.chunks );
			fprintf( filePointer, "# columns: %d\n", mel.filters );
			for (j = 0; j < mel.chunks; j++)
			{
					for (k = 0; k < mel.filters; k++)
					{
						fprintf( filePointer, " %lf", *((mel.channel[i] + j*(mel.filters)) + k) );
					}
					fprintf( filePointer, "\n" );
			}
			fprintf( filePointer, "\n\n" );
		}

		/////////////////////////// writes the MFCC filters configuration into file with fprintf

		fprintf( filePointer, "# name: Filters\n" );
		fprintf( filePointer, "# type: matrix\n" );
		fprintf( filePointer, "# rows: %d\n", mel.filters );
		fprintf( filePointer, "# columns: %d\n", mel.fourierWindowLength/2+1 );
		for (j = 0; j < mel.filters; j++)
		{
				for (k = 0; k < mel.fourierWindowLength/2+1; k++)
				{
					fprintf( filePointer, " %lf", *((mel.filter + j*(mel.fourierWindowLength/2+1)) + k) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		fclose( filePointer );							// fclose closes file
	}										// end else
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
} // end function persistence









