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

// File Name:		RegionGenerationService.cpp
// File Description:	RegionGenerationService member-function definitions. This file contains implementations of the
//			member functions prototyped in RegionGenerationService.h.

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <string>

#include "RegionGenerationServiceNew.h"

// function to generate a two dimensional random matrix of weights
void	generateWeights( double offset, double modulation, int inputDim, int unitsDim, double *weights )
{
	int	i, j;

	srand ( time(NULL) );
	for (i = 0; i < unitsDim; i++)
		for (j = 0; j < inputDim; j++)
			*((weights + i*inputDim) + j) = modulation*(((double)(rand() % 1000)/(double)1000) - (0.5 - offset));
} // end function generateWeights


// function to print the title screen
void	titleScreen()
{
	system("clear");			// clears the screen
	printf("\n//////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
	printf("//				Author:		Dematties Dario Jesus						//\n");
	printf("//				Contact:	dariodematties@hotmail.com.ar					//\n");
	printf("//						dariodematties@yahoo.com.ar					//\n");
	printf("//						dario.dematties@frm.utn.edu.ar					//\n");
	printf("//				Project:	Engineering PhD Project						//\n");
	printf("//				Institution:	Universidad de Buenos Aires					//\n");
	printf("//						Facultad de Ingeniería (FIUBA)					//\n");
	printf("//				Workplace:	Instituto de Ingeniería						//\n");
	printf("//						Biomédica FIUBA	&						//\n");
	printf("//						CCT CONICET Mendoza INCIHUSA					//\n");
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n\n");
} // end function title screen

// function to save the connectome of the cortical node
void	saveConnectome( connectome connectomeNode, int hierarchicalLayer, int number )
{
	int	i, j, c;
	int	inputDim, unitsDim, clusters;
	char	pathFile[100];
	double	*weightsPointer, *predictionWeightsPointer, *strongUnitsWeights;


	FILE *filePointer; 								// ConnectomeNode.conn file pointer

	switch ( hierarchicalLayer )
	{
		case 1:
			switch ( number )
			{
				case 1:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H1_ConnectomeNode_1.conn");
				break;

				case 2:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H1_ConnectomeNode_2.conn");
				break;

				case 3:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H1_ConnectomeNode_3.conn");
				break;

				case 4:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H1_ConnectomeNode_4.conn");
				break;

				default:
					printf( "Incorrect option in saveConnectome function. In hierarchy number %d, we have %d node\n", hierarchicalLayer, number );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		case 2:
			switch ( number )
			{
				case 1:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H2_ConnectomeNode_1.conn");
				break;

				case 2:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H2_ConnectomeNode_2.conn");
				break;

				default:
					printf( "Incorrect option in saveConnectome function. In hierarchy number %d, we have %d node\n", hierarchicalLayer, number );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		case 3:
			switch ( number )
			{
				case 1:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H3_ConnectomeNode_1.conn");
				break;

				default:
					printf( "Incorrect option in saveConnectome function. In hierarchy number %d, we have %d node\n", hierarchicalLayer, number );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		default:
			printf( "Incorrect option in saveConnectome function. Incorrect hierarchy number %d.\n", hierarchicalLayer );
			exit( EXIT_FAILURE );
		break;
	}

	// fopen opens file. Exit program if unable to create file
	if ( ( filePointer = fopen( pathFile, "w" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		fprintf( filePointer, "# This is a file created by saveConnectome function.\n" );

		/////////////////////////// writes layer 4 cluster weights into file with fprintf

		fprintf( filePointer, "# Layer 4 cluster weights:\n" );
		inputDim = connectomeNode.l4I;						// gets the number of input components in layer 4 cluster
		unitsDim = connectomeNode.l4U;						// gets the number of units components in layer 4 cluster
		fprintf( filePointer, "# inputDim:\n%d\n", inputDim );
		fprintf( filePointer, "# unitsDim:\n%d\n", unitsDim );
		weightsPointer = connectomeNode.l4W;					// gets the pointer to layer 4 cluster weights
		for (i = 0; i < unitsDim; i++)
		{
				for (j = 0; j < inputDim; j++)
				{
					fprintf( filePointer, " %lf", *((weightsPointer + i*inputDim) + j) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		fprintf( filePointer, "# Layer 4 cluster prediction weights:\n" );
		unitsDim = connectomeNode.l4U;						// gets the number of units components in layer 4 cluster
		fprintf( filePointer, "# lastWinner:\n%d\n", unitsDim+1 );
		fprintf( filePointer, "# winner:\n%d\n", unitsDim );
		predictionWeightsPointer = connectomeNode.l4P;				// gets the pointer to layer 4 cluster prediction weights
		for (i = 0; i < unitsDim+1; i++)
		{
				for (j = 0; j < unitsDim; j++)
				{
					fprintf( filePointer, " %lf", *((predictionWeightsPointer + i*unitsDim) + j) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		fprintf( filePointer, "# Layer 4 cluster strong units weights:\n" );
		unitsDim = connectomeNode.l4U;						// gets the number of units components in layer 4 cluster
		fprintf( filePointer, "# unitsDim:\n%d\n", unitsDim );
		strongUnitsWeights = connectomeNode.l4S;				// gets the pointer to layer 4 cluster strong units weights
		for (i = 0; i < unitsDim; i++)
		{
			fprintf( filePointer, " %lf", *(strongUnitsWeights + i) );
			//fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		/////////////////////////// writes layer 5 cluster weights into file with fprintf

		fprintf( filePointer, "# Layer 5 cluster weights:\n" );
		inputDim = connectomeNode.l5I;						// gets the number of input components in layer 5 cluster
		unitsDim = connectomeNode.l5U;						// gets the number of units components in layer 5 cluster
		fprintf( filePointer, "# inputDim:\n%d\n", inputDim );
		fprintf( filePointer, "# unitsDim:\n%d\n", unitsDim );
		weightsPointer = connectomeNode.l5W;					// gets the pointer to layer 5 cluster weights
		for (i = 0; i < unitsDim; i++)
		{
				for (j = 0; j < inputDim; j++)
				{
					fprintf( filePointer, " %lf", *((weightsPointer + i*inputDim) + j) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		fprintf( filePointer, "# Layer 5 cluster strong units weights:\n" );
		unitsDim = connectomeNode.l5U;						// gets the number of units components in layer 5 cluster
		fprintf( filePointer, "# unitsDim:\n%d\n", unitsDim );
		strongUnitsWeights = connectomeNode.l5S;				// gets the pointer to layer 5 cluster strong units weights
		for (i = 0; i < unitsDim; i++)
		{
			fprintf( filePointer, " %lf", *(strongUnitsWeights + i) );
			//fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		/////////////////////////// writes layer 6 cluster weights into file with fprintf

		fprintf( filePointer, "# Layer 6 cluster weights:\n" );
		inputDim = connectomeNode.l6I;						// gets the number of input components in layer 6 cluster
		unitsDim = connectomeNode.l6U;						// gets the number of units components in layer 6 cluster
		fprintf( filePointer, "# inputDim:\n%d\n", inputDim );
		fprintf( filePointer, "# unitsDim:\n%d\n", unitsDim );
		weightsPointer = connectomeNode.l6W;					// gets the pointer to layer 6 cluster weights
		for (i = 0; i < unitsDim; i++)
		{
				for (j = 0; j < inputDim; j++)
				{
					fprintf( filePointer, " %lf", *((weightsPointer + i*inputDim) + j) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		fprintf( filePointer, "# Layer 6 cluster strong units weights:\n" );
		unitsDim = connectomeNode.l6U;						// gets the number of units components in layer 6 cluster
		fprintf( filePointer, "# unitsDim:\n%d\n", unitsDim );
		strongUnitsWeights = connectomeNode.l6S;				// gets the pointer to layer 6 cluster strong units weights
		for (i = 0; i < unitsDim; i++)
		{
			fprintf( filePointer, " %lf", *(strongUnitsWeights + i) );
			//fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		/////////////////////////// writes layer 23 forward array clusters weights into file with fprintf

		fprintf( filePointer, "# Layer 23 forward array clusters weights:\n" );
		inputDim = connectomeNode.l23FGAI;					// gets the number of input components in layer 23 forward array clusters
		unitsDim = connectomeNode.l23FGAU;					// gets the number of units components in layer 23 forward array clusters
		clusters = connectomeNode.l23FGC;					// gets the number of clusters in layer 23 forward group

		fprintf( filePointer, "# inputDim:\n%d\n", inputDim );
		fprintf( filePointer, "# unitsDim:\n%d\n", unitsDim );
		fprintf( filePointer, "# clusters:\n%d\n", clusters );

		for ( c = 0; c < clusters; c++ )
		{
			fprintf( filePointer, "# Forward group cluster number:\n%d\n", c+1 );
			weightsPointer = connectomeNode.l23FGAW[c];			// gets the pointer to layer 23 forward first cluster weights
			for (i = 0; i < unitsDim; i++)
			{
					for (j = 0; j < inputDim; j++)
					{
						fprintf( filePointer, " %lf", *((weightsPointer + i*inputDim) + j) );
					}
					fprintf( filePointer, "\n" );
			}
			fprintf( filePointer, "\n\n" );
		}

		fprintf( filePointer, "# Layer 23 forward array clusters strong units weights:\n" );
		unitsDim = connectomeNode.l23FGAU;					// gets the number of units components in layer 23 forward array clusters
		clusters = connectomeNode.l23FGC;					// gets the number of clusters in layer 23 forward group

		fprintf( filePointer, "# unitsDim:\n%d\n", unitsDim );
		fprintf( filePointer, "# clusters:\n%d\n", clusters );

		for ( c = 0; c < clusters; c++ )
		{
			fprintf( filePointer, "# Forward group cluster number:\n%d\n", c+1 );
			strongUnitsWeights = connectomeNode.l23FGAS[c];			// gets the pointer to layer 23 forward first cluster weights
			for (i = 0; i < unitsDim; i++)
			{
				fprintf( filePointer, " %lf", *(strongUnitsWeights + i) );
				//fprintf( filePointer, "\n" );
			}
			fprintf( filePointer, "\n\n" );
		}

		/////////////////////////// writes layer 23 backward array clusters weights into file with fprintf

		fprintf( filePointer, "# Layer 23 backward array clusters weights:\n" );
		inputDim = connectomeNode.l23BGAI;					// gets the number of input components in layer 23 backward array clusters
		unitsDim = connectomeNode.l23BGAU;					// gets the number of units components in layer 23 backward array clusters
		clusters = connectomeNode.l23BGC;					// gets the number of clusters in layer 23 backward group

		fprintf( filePointer, "# inputDim:\n%d\n", inputDim );
		fprintf( filePointer, "# unitsDim:\n%d\n", unitsDim );
		fprintf( filePointer, "# clusters:\n%d\n", clusters );

		for ( c = 0; c < clusters; c++ )
		{
			fprintf( filePointer, "# Backward group cluster number:\n%d\n", c+1 );
			weightsPointer = connectomeNode.l23BGAW[c];			// gets the pointer to layer 23 backward first cluster weights
			for (i = 0; i < unitsDim; i++)
			{
					for (j = 0; j < inputDim; j++)
					{
						fprintf( filePointer, " %lf", *((weightsPointer + i*inputDim) + j) );
					}
					fprintf( filePointer, "\n" );
			}
			fprintf( filePointer, "\n\n" );
		}

		fprintf( filePointer, "# Layer 23 backward array clusters strong units weights:\n" );
		unitsDim = connectomeNode.l23BGAU;					// gets the number of units components in layer 23 backward array clusters
		clusters = connectomeNode.l23BGC;					// gets the number of clusters in layer 23 backward group

		fprintf( filePointer, "# unitsDim:\n%d\n", unitsDim );
		fprintf( filePointer, "# clusters:\n%d\n", clusters );

		for ( c = 0; c < clusters; c++ )
		{
			fprintf( filePointer, "# Backward group cluster number:\n%d\n", c+1 );
			strongUnitsWeights = connectomeNode.l23BGAS[c];			// gets the pointer to layer 23 backward first cluster weights
			for (i = 0; i < unitsDim; i++)
			{
				fprintf( filePointer, " %lf", *(strongUnitsWeights + i) );
				//fprintf( filePointer, "\n" );
			}
			fprintf( filePointer, "\n\n" );
		}

		fclose( filePointer );							// fclose closes file
	}										// end else
} // end function saveConnectome


// function to load the connectome of the cortical node
connectome	loadConnectome( int hierarchicalLayer, int number )
{
	int	i, j, c;
	int	inputDim, unitsDim, Aux, auxiliaryVariable;
	char	pathFile[100];
	double	*weightsPointer;

	connectome connectomeNode;

	char	firstLine[54];
	char	l4Line[28];
	char	l4LineP[39];
	char	l4LineS[41];
	char	l5Line[28];
	char	l5LineS[41];
	char	l6Line[28];
	char	l6LineS[41];
	char	l23FGALine[44];
	char	l23FGALineS[57];
	char	l23BGALine[45];
	char	l23BGALineS[58];
	char	l23FGACLine[33];
	char	l23BGACLine[34];
	char	l23Line[29];
	char	inputLine[13];
	char	unitsLine[14];
	char	clustersLine[13];
	char	lastWinnerLine[15];
	char	winnerLine[11];

	FILE *filePointer; 								// ConnectomeNode.conn file pointer

	switch ( hierarchicalLayer )
	{
		case 1:
			switch ( number )
			{
				case 1:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H1_ConnectomeNode_1.conn");
				break;

				case 2:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H1_ConnectomeNode_2.conn");
				break;

				case 3:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H1_ConnectomeNode_3.conn");
				break;

				case 4:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H1_ConnectomeNode_4.conn");
				break;

				default:
					printf( "Incorrect option in loadConnectome function. In hierarchy number %d, we have %d node\n", hierarchicalLayer, number );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		case 2:
			switch ( number )
			{
				case 1:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H2_ConnectomeNode_1.conn");
				break;

				case 2:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H2_ConnectomeNode_2.conn");
				break;

				default:
					printf( "Incorrect option in loadConnectome function. In hierarchy number %d, we have %d node\n", hierarchicalLayer, number );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		case 3:
			switch ( number )
			{
				case 1:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/C++/Mono-processing/Connectome/H3_ConnectomeNode_1.conn");
				break;

				default:
					printf( "Incorrect option in loadConnectome function. In hierarchy number %d, we have %d node\n", hierarchicalLayer, number );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		default:
			printf( "Incorrect option in loadConnectome function. Incorrect hierarchy number %d.\n", hierarchicalLayer );
			exit( EXIT_FAILURE );
		break;
	}

	// fopen opens file. Exit program if unable to create file
	if ( ( filePointer = fopen( pathFile, "r" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		fgets(firstLine, 54, filePointer);
		if ( strcmp(firstLine, "# This is a file created by saveConnectome function.\n") != 0)
		{
			printf("File corrupted; read: %s\n", firstLine);
			exit( EXIT_FAILURE );
		}

		/////////////////////////// reads layer 4 cluster weights from file with fscanf

		fgets(l4Line, 28, filePointer);
		if ( strcmp(l4Line, "# Layer 4 cluster weights:\n") != 0)
		{
			printf("File corrupted; read: %s\n", l4Line);
			exit( EXIT_FAILURE );
		}

		fgets(inputLine, 13, filePointer);
		if ( strcmp(inputLine, "# inputDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", inputLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l4I);
		passNewLine(filePointer);

		fgets(unitsLine, 14, filePointer);
		if ( strcmp(unitsLine, "# unitsDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", unitsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l4U);
		passNewLine(filePointer);

		connectomeNode.l4W = (double*)calloc(connectomeNode.l4U*connectomeNode.l4I, sizeof(double));	// reserves space for connectomeNode.l4W
		for (i = 0; i < connectomeNode.l4U; i++)
		{
				for (j = 0; j < connectomeNode.l4I; j++)
				{
					fscanf(filePointer, " %lf", ((connectomeNode.l4W + i*connectomeNode.l4I) + j) );
				}
				passNewLine(filePointer);
		}
		passNewLine(filePointer);
		passNewLine(filePointer);

		//////////////		//////////////		//////////////		//////////////		//////////////

		fgets(l4LineP, 39, filePointer);
		if ( strcmp(l4LineP, "# Layer 4 cluster prediction weights:\n") != 0)
		{
			printf("File corrupted; read: %s\n", l4LineP);
			exit( EXIT_FAILURE );
		}

		fgets(lastWinnerLine, 15, filePointer);
		if ( strcmp(lastWinnerLine, "# lastWinner:\n") != 0)
		{
			printf("File corrupted; read: %s\n", lastWinnerLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &auxiliaryVariable);
		if ( auxiliaryVariable-1 != connectomeNode.l4U )
		{
			printf("File corrupted; lastWinner: %d,\n when it must be: %d.\n", auxiliaryVariable, connectomeNode.l4U+1);
			exit( EXIT_FAILURE );
		}
		passNewLine(filePointer);

		fgets(winnerLine, 11, filePointer);
		if ( strcmp(winnerLine, "# winner:\n") != 0)
		{
			printf("File corrupted; read: %s\n", winnerLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &auxiliaryVariable);
		if ( auxiliaryVariable != connectomeNode.l4U )
		{
			printf("File corrupted; winner: %d,\n when it must be: %d.\n", auxiliaryVariable, connectomeNode.l4U);
			exit( EXIT_FAILURE );
		}
		passNewLine(filePointer);

		connectomeNode.l4P = (double*)calloc((connectomeNode.l4U+1)*connectomeNode.l4U, sizeof(double));	// reserves space for connectomeNode.l4P
		for (i = 0; i < connectomeNode.l4U+1; i++)
		{
				for (j = 0; j < connectomeNode.l4U; j++)
				{
					fscanf(filePointer, " %lf", ((connectomeNode.l4P + i*connectomeNode.l4U) + j) );
				}
				passNewLine(filePointer);
		}
		passNewLine(filePointer);
		passNewLine(filePointer);

		//////////////		//////////////		//////////////		//////////////		//////////////

		fgets(l4LineS, 41, filePointer);
		if ( strcmp(l4LineS, "# Layer 4 cluster strong units weights:\n") != 0)
		{
			printf("File corrupted; read: %s\n", l4LineS);
			exit( EXIT_FAILURE );
		}

		fgets(unitsLine, 14, filePointer);
		if ( strcmp(unitsLine, "# unitsDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", unitsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l4U);
		passNewLine(filePointer);

		connectomeNode.l4S = (double*)calloc(connectomeNode.l4U, sizeof(double));	// reserves space for connectomeNode.l4S
		for (i = 0; i < connectomeNode.l4U; i++)
		{
			fscanf(filePointer, " %lf", (connectomeNode.l4S + i) );
			//passNewLine(filePointer);
		}
		passNewLine(filePointer);
		passNewLine(filePointer);

		/////////////////////////// reads layer 5 cluster weights from file with fscanf

		fgets(l5Line, 28, filePointer);
		if ( strcmp(l5Line, "# Layer 5 cluster weights:\n") != 0)
		{
			printf("File corrupted; read: %s\n", l5Line);
			exit( EXIT_FAILURE );
		}

		fgets(inputLine, 13, filePointer);
		if ( strcmp(inputLine, "# inputDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", inputLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l5I);
		passNewLine(filePointer);

		fgets(unitsLine, 14, filePointer);
		if ( strcmp(unitsLine, "# unitsDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", unitsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l5U);
		passNewLine(filePointer);

		connectomeNode.l5W = (double*)calloc(connectomeNode.l5U*connectomeNode.l5I, sizeof(double));	// reserves space for connectomeNode.l5W
		for (i = 0; i < connectomeNode.l5U; i++)
		{
				for (j = 0; j < connectomeNode.l5I; j++)
				{
					fscanf(filePointer, " %lf", ((connectomeNode.l5W + i*connectomeNode.l5I) + j) );
				}
				passNewLine(filePointer);
		}
		passNewLine(filePointer);
		passNewLine(filePointer);

		//////////////		//////////////		//////////////		//////////////		//////////////

		fgets(l5LineS, 41, filePointer);
		if ( strcmp(l5LineS, "# Layer 5 cluster strong units weights:\n") != 0)
		{
			printf("File corrupted; read: %s\n", l5LineS);
			exit( EXIT_FAILURE );
		}

		fgets(unitsLine, 14, filePointer);
		if ( strcmp(unitsLine, "# unitsDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", unitsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l5U);
		passNewLine(filePointer);

		connectomeNode.l5S = (double*)calloc(connectomeNode.l5U, sizeof(double));	// reserves space for connectomeNode.l5S
		for (i = 0; i < connectomeNode.l5U; i++)
		{
			fscanf(filePointer, " %lf", (connectomeNode.l5S + i) );
			//passNewLine(filePointer);
		}
		passNewLine(filePointer);
		passNewLine(filePointer);

		/////////////////////////// reads layer 6 cluster weights from file with fscanf

		fgets(l6Line, 28, filePointer);
		if ( strcmp(l6Line, "# Layer 6 cluster weights:\n") != 0)
		{
			printf("File corrupted; read: %s\n", l6Line);
			exit( EXIT_FAILURE );
		}

		fgets(inputLine, 13, filePointer);
		if ( strcmp(inputLine, "# inputDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", inputLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l6I);
		passNewLine(filePointer);

		fgets(unitsLine, 14, filePointer);
		if ( strcmp(unitsLine, "# unitsDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", unitsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l6U);
		passNewLine(filePointer);

		connectomeNode.l6W = (double*)calloc(connectomeNode.l6U*connectomeNode.l6I, sizeof(double));	// reserves space for connectomeNode.l6W
		for (i = 0; i < connectomeNode.l6U; i++)
		{
				for (j = 0; j < connectomeNode.l6I; j++)
				{
					fscanf(filePointer, " %lf", ((connectomeNode.l6W + i*connectomeNode.l6I) + j) );
				}
				passNewLine(filePointer);
		}
		passNewLine(filePointer);
		passNewLine(filePointer);

		//////////////		//////////////		//////////////		//////////////		//////////////

		fgets(l6LineS, 41, filePointer);
		if ( strcmp(l6LineS, "# Layer 6 cluster strong units weights:\n") != 0)
		{
			printf("File corrupted; read: %s\n", l6LineS);
			exit( EXIT_FAILURE );
		}

		fgets(unitsLine, 14, filePointer);
		if ( strcmp(unitsLine, "# unitsDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", unitsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l6U);
		passNewLine(filePointer);

		connectomeNode.l6S = (double*)calloc(connectomeNode.l6U, sizeof(double));	// reserves space for connectomeNode.l6S
		for (i = 0; i < connectomeNode.l6U; i++)
		{
			fscanf(filePointer, " %lf", (connectomeNode.l6S + i) );
			//passNewLine(filePointer);
		}
		passNewLine(filePointer);
		passNewLine(filePointer);

		/////////////////////////// reads layer 23 forward array clusters weights from file with fscanf

		fgets(l23FGALine, 44, filePointer);
		if ( strcmp(l23FGALine, "# Layer 23 forward array clusters weights:\n") != 0)
		{
			printf("File corrupted; read: %s\n", l23FGALine);
			exit( EXIT_FAILURE );
		}

		fgets(inputLine, 13, filePointer);
		if ( strcmp(inputLine, "# inputDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", inputLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l23FGAI);
		passNewLine(filePointer);

		fgets(unitsLine, 14, filePointer);
		if ( strcmp(unitsLine, "# unitsDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", unitsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l23FGAU);
		passNewLine(filePointer);

		fgets(clustersLine, 13, filePointer);
		if ( strcmp(clustersLine, "# clusters:\n") != 0)
		{
			printf("File corrupted; read: %s\n", clustersLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l23FGC);
		passNewLine(filePointer);

		for ( c = 0; c < connectomeNode.l23FGC; c++ )
		{
			fgets(l23FGACLine, 33, filePointer);
			if ( strcmp(l23FGACLine, "# Forward group cluster number:\n") != 0 )
			{
				printf("File corrupted; read: %s\n", l23FGACLine);
				exit( EXIT_FAILURE );
			}

			fscanf(filePointer, "%d", &Aux);
			if ( Aux != c+1 )
			{
				printf("File corrupted; read: %d when it must have been: %d\n", Aux, c+1);
				exit( EXIT_FAILURE );
			}
			passNewLine(filePointer);

			connectomeNode.l23FGAW[c] = (double*)calloc(connectomeNode.l23FGAU*connectomeNode.l23FGAI, sizeof(double));	// reserves space for connectomeNode.l23FGAW
			for (i = 0; i < connectomeNode.l23FGAU; i++)
			{
				for (j = 0; j < connectomeNode.l23FGAI; j++)
				{
					fscanf(filePointer, " %lf", ((connectomeNode.l23FGAW[c] + i*connectomeNode.l23FGAI) + j) );
				}
				passNewLine(filePointer);
			}
			passNewLine(filePointer);
			passNewLine(filePointer);
		}

		//////////////		//////////////		//////////////		//////////////		//////////////

		fgets(l23FGALineS, 57, filePointer);
		if ( strcmp(l23FGALineS, "# Layer 23 forward array clusters strong units weights:\n") != 0)
		{
			printf("File corrupted; read: %s\n", l23FGALineS);
			exit( EXIT_FAILURE );
		}

		fgets(unitsLine, 14, filePointer);
		if ( strcmp(unitsLine, "# unitsDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", unitsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l23FGAU);
		passNewLine(filePointer);

		fgets(clustersLine, 13, filePointer);
		if ( strcmp(clustersLine, "# clusters:\n") != 0)
		{
			printf("File corrupted; read: %s\n", clustersLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l23FGC);
		passNewLine(filePointer);

		for ( c = 0; c < connectomeNode.l23FGC; c++ )
		{
			fgets(l23FGACLine, 33, filePointer);
			if ( strcmp(l23FGACLine, "# Forward group cluster number:\n") != 0 )
			{
				printf("File corrupted; read: %s\n", l23FGACLine);
				exit( EXIT_FAILURE );
			}

			fscanf(filePointer, "%d", &Aux);
			if ( Aux != c+1 )
			{
				printf("File corrupted; read: %d when it must have been: %d\n", Aux, c+1);
				exit( EXIT_FAILURE );
			}
			passNewLine(filePointer);

			connectomeNode.l23FGAS[c] = (double*)calloc(connectomeNode.l23FGAU, sizeof(double));	// reserves space for connectomeNode.l23FGAS
			for (i = 0; i < connectomeNode.l23FGAU; i++)
			{
				fscanf(filePointer, " %lf", (connectomeNode.l23FGAS[c] + i) );
				//passNewLine(filePointer);
			}
			passNewLine(filePointer);
			passNewLine(filePointer);
		}


		/////////////////////////// reads layer 23 backward array clusters weights from file with fscanf

		fgets(l23BGALine, 45, filePointer);
		if ( strcmp(l23BGALine, "# Layer 23 backward array clusters weights:\n") != 0)
		{
			printf("File corrupted; read: %s\n", l23BGALine);
			exit( EXIT_FAILURE );
		}

		fgets(inputLine, 13, filePointer);
		if ( strcmp(inputLine, "# inputDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", inputLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l23BGAI);
		passNewLine(filePointer);

		fgets(unitsLine, 14, filePointer);
		if ( strcmp(unitsLine, "# unitsDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", unitsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l23BGAU);
		passNewLine(filePointer);

		fgets(clustersLine, 13, filePointer);
		if ( strcmp(clustersLine, "# clusters:\n") != 0)
		{
			printf("File corrupted; read: %s\n", clustersLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l23BGC);
		passNewLine(filePointer);

		for ( c = 0; c < connectomeNode.l23BGC; c++ )
		{
			fgets(l23BGACLine, 34, filePointer);
			if ( strcmp(l23BGACLine, "# Backward group cluster number:\n") != 0 )
			{
				printf("File corrupted; read: %s\n", l23BGACLine);
				exit( EXIT_FAILURE );
			}

			fscanf(filePointer, "%d", &Aux);
			if ( Aux != c+1 )
			{
				printf("File corrupted; read: %d when it must have been: %d\n", Aux, c+1);
				exit( EXIT_FAILURE );
			}
			passNewLine(filePointer);

			connectomeNode.l23BGAW[c] = (double*)calloc(connectomeNode.l23BGAU*connectomeNode.l23BGAI, sizeof(double));	// reserves space for connectomeNode.l23BGAW
			for (i = 0; i < connectomeNode.l23BGAU; i++)
			{
				for (j = 0; j < connectomeNode.l23BGAI; j++)
				{
					fscanf(filePointer, " %lf", ((connectomeNode.l23BGAW[c] + i*connectomeNode.l23BGAI) + j) );
				}
				passNewLine(filePointer);
			}
			passNewLine(filePointer);
			passNewLine(filePointer);
		}

		//////////////		//////////////		//////////////		//////////////		//////////////

		fgets(l23BGALineS, 58, filePointer);
		if ( strcmp(l23BGALineS, "# Layer 23 backward array clusters strong units weights:\n") != 0)
		{
			printf("File corrupted; read: %s\n", l23BGALineS);
			exit( EXIT_FAILURE );
		}

		fgets(unitsLine, 14, filePointer);
		if ( strcmp(unitsLine, "# unitsDim:\n") != 0)
		{
			printf("File corrupted; read: %s\n", unitsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l23BGAU);
		passNewLine(filePointer);

		fgets(clustersLine, 13, filePointer);
		if ( strcmp(clustersLine, "# clusters:\n") != 0)
		{
			printf("File corrupted; read: %s\n", clustersLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &connectomeNode.l23BGC);
		passNewLine(filePointer);

		for ( c = 0; c < connectomeNode.l23BGC; c++ )
		{
			fgets(l23BGACLine, 34, filePointer);
			if ( strcmp(l23BGACLine, "# Backward group cluster number:\n") != 0 )
			{
				printf("File corrupted; read: %s\nlalala", l23BGACLine);
				exit( EXIT_FAILURE );
			}

			fscanf(filePointer, "%d", &Aux);
			if ( Aux != c+1 )
			{
				printf("File corrupted; read: %d when it must have been: %d\n", Aux, c+1);
				exit( EXIT_FAILURE );
			}
			passNewLine(filePointer);

			connectomeNode.l23BGAS[c] = (double*)calloc(connectomeNode.l23BGAU, sizeof(double));	// reserves space for connectomeNode.l23BGAS
			for (i = 0; i < connectomeNode.l23BGAU; i++)
			{
				fscanf(filePointer, " %lf", (connectomeNode.l23BGAS[c] + i) );
				//passNewLine(filePointer);
			}
			passNewLine(filePointer);
			passNewLine(filePointer);
		}

/////////////////////////////////////////////////////////////////// Shows the connectome arrays on screen

		char	option;

		//printf("\nDo you want to show the connectome arrays on screen? (Y) for yes and any key for no\n\n");
		//printf("\nThis corresponds to hierarchical layer %d and node number %d.\n\n", hierarchicalLayer, number);
		//scanf(" %c", &option);

		option = 'n';

		if ( option == 'Y')
		{
			/////////////////////////// Shows layer 4 cluster connectome on screen

			printf("File read input: %d\n", connectomeNode.l4I);
			printf("File read units: %d\n", connectomeNode.l4U);
			printf("%s", l4Line);
			for (i = 0; i < connectomeNode.l4U; i++)
			{
					for (j = 0; j < connectomeNode.l4I; j++)
					{
						printf(" %lf", *((connectomeNode.l4W + i*connectomeNode.l4I) + j) );
					}
					printf("\n");
			}
			printf("\n\n");

			auxiliaryVariable = connectomeNode.l4U + 1;
			printf("File read lastWinner: %d\n", auxiliaryVariable);
			printf("File read winner: %d\n", connectomeNode.l4U);
			printf("%s", l4LineP);
			for (i = 0; i < connectomeNode.l4U+1; i++)
			{
					for (j = 0; j < connectomeNode.l4U; j++)
					{
						printf(" %lf", *((connectomeNode.l4P + i*connectomeNode.l4U) + j) );
					}
					printf("\n");
			}
			printf("\n\n");

			/////////////////////////// Shows layer 5 cluster connectome on screen

			printf("File read input: %d\n", connectomeNode.l5I);
			printf("File read units: %d\n", connectomeNode.l5U);
			printf("%s", l5Line);
			for (i = 0; i < connectomeNode.l5U; i++)
			{
					for (j = 0; j < connectomeNode.l5I; j++)
					{
						printf(" %lf", *((connectomeNode.l5W + i*connectomeNode.l5I) + j) );
					}
					printf("\n");
			}
			printf("\n\n");

			/////////////////////////// Shows layer 6 cluster connectome on screen

			printf("File read input: %d\n", connectomeNode.l6I);
			printf("File read units: %d\n", connectomeNode.l6U);
			printf("%s", l6Line);
			for (i = 0; i < connectomeNode.l6U; i++)
			{
					for (j = 0; j < connectomeNode.l6I; j++)
					{
						printf(" %lf", *((connectomeNode.l6W + i*connectomeNode.l6I) + j) );
					}
					printf("\n");
			}
			printf("\n\n");

			/////////////////////////// Shows layer 23 forward group array of clusters connectome on screen

			printf("File read input: %d\n", connectomeNode.l23FGAI);
			printf("File read units: %d\n", connectomeNode.l23FGAU);
			printf("File read clusters: %d\n", connectomeNode.l23FGC);
			printf("%s", l23FGALine);
			for (c = 0; c < connectomeNode.l23FGC; c++)
			{
				printf("%s", l23FGACLine);
				printf("%d\n", c+1);
				for (i = 0; i < connectomeNode.l23FGAU; i++)
				{
						for (j = 0; j < connectomeNode.l23FGAI; j++)
						{
							printf(" %lf", *((connectomeNode.l23FGAW[c] + i*connectomeNode.l23FGAI) + j) );
						}
						printf("\n");
				}
				printf("\n\n");
			}

			/////////////////////////// Shows layer 23 backward group array of clusters connectome on screen

			printf("File read input: %d\n", connectomeNode.l23BGAI);
			printf("File read units: %d\n", connectomeNode.l23BGAU);
			printf("File read clusters: %d\n", connectomeNode.l23BGC);
			printf("%s", l23BGALine);
			for (c = 0; c < connectomeNode.l23BGC; c++)
			{
				printf("%s", l23BGACLine);
				printf("%d\n", c+1);
				for (i = 0; i < connectomeNode.l23BGAU; i++)
				{
						for (j = 0; j < connectomeNode.l23BGAI; j++)
						{
							printf(" %lf", *((connectomeNode.l23BGAW[c] + i*connectomeNode.l23BGAI) + j) );
						}
						printf("\n");
				}
				printf("\n\n");
			}
		}

	fclose( filePointer );							// fclose closes file
	}										// end else

	return	connectomeNode;
} // end function loadConnectome


// function to pass to the new line from the file reading process
void	passNewLine(FILE *filePointer)
{
	char	newLine;

	newLine = getc(filePointer);
	while ( newLine != '\n' )
		newLine = getc(filePointer);
} // end function passNewLine


// function to save the connectome of the cortical node for visualization issues on Octave format
void	saveConnectomeForOctave( connectome connectomeNode, int hierarchicalLayer, int number )
{
	int	i, j, c;
	int	inputDim, unitsDim, clusters;
	char	pathFile[100];
	double	*weightsPointer, *predictionWeightsPointer;

	FILE *filePointer; 								// ConnectomeNode.conn file pointer

	switch ( hierarchicalLayer )
	{
		case 1:
			switch ( number )
			{
				case 1:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/Octave/H1_ConnectomeNode_1.mat");
				break;

				case 2:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/Octave/H1_ConnectomeNode_2.mat");
				break;

				case 3:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/Octave/H1_ConnectomeNode_3.mat");
				break;

				case 4:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/Octave/H1_ConnectomeNode_4.mat");
				break;

				default:
					printf( "Incorrect option in saveConnectomeForOctave function. In hierarchy number %d, we have %d node\n", hierarchicalLayer, number );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		case 2:
			switch ( number )
			{
				case 1:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/Octave/H2_ConnectomeNode_1.mat");
				break;

				case 2:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/Octave/H2_ConnectomeNode_2.mat");
				break;

				default:
					printf( "Incorrect option in saveConnectomeForOctave function. In hierarchy number %d, we have %d node\n", hierarchicalLayer, number );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		case 3:
			switch ( number )
			{
				case 1:
					strcpy(pathFile, "/home/dario/MEGA/Doctorado/Contenidos/Model/Octave/H3_ConnectomeNode_1.mat");
				break;

				default:
					printf( "Incorrect option in saveConnectomeForOctave function. In hierarchy number %d, we have %d node\n", hierarchicalLayer, number );
					exit( EXIT_FAILURE );
				break;
			}
		break;

		default:
			printf( "Incorrect option in saveConnectomeForOctave function. Incorrect hierarchy number %d.\n", hierarchicalLayer );
			exit( EXIT_FAILURE );
		break;
	}

	// fopen opens file. Exit program if unable to create file
	if ( ( filePointer = fopen( pathFile, "w" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		fprintf( filePointer, "# This is a file created by saveConnectomeForOctave function.\n" );

		/////////////////////////// writes layer 4 cluster weights into file with fprintf

		fprintf( filePointer, "# name: Layer4Cluster\n" );
		fprintf( filePointer, "# type: matrix\n" );
		inputDim = connectomeNode.l4I;						// gets the number of input components in layer 4 cluster
		unitsDim = connectomeNode.l4U;						// gets the number of units components in layer 4 cluster
		fprintf( filePointer, "# rows: %d\n", unitsDim );
		fprintf( filePointer, "# columns: %d\n", inputDim );
		weightsPointer = connectomeNode.l4W;					// gets the pointer to layer 4 cluster weights
		for (i = 0; i < unitsDim; i++)
		{
				for (j = 0; j < inputDim; j++)
				{
					fprintf( filePointer, " %lf", *((weightsPointer + i*inputDim) + j) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		fprintf( filePointer, "# name: Layer4PredictiveCluster\n" );
		fprintf( filePointer, "# type: matrix\n" );
		unitsDim = connectomeNode.l4U;						// gets the number of units components in layer 4 cluster
		fprintf( filePointer, "# rows: %d\n", unitsDim+1 );
		fprintf( filePointer, "# columns: %d\n", unitsDim );
		predictionWeightsPointer = connectomeNode.l4P;				// gets the pointer to layer 4 cluster prediction weights
		for (i = 0; i < unitsDim+1; i++)
		{
				for (j = 0; j < unitsDim; j++)
				{
					fprintf( filePointer, " %lf", *((predictionWeightsPointer + i*unitsDim) + j) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		/////////////////////////// writes layer 5 cluster weights into file with fprintf

		fprintf( filePointer, "# name: Layer5Cluster\n" );
		fprintf( filePointer, "# type: matrix\n" );
		inputDim = connectomeNode.l5I;						// gets the number of input components in layer 5 cluster
		unitsDim = connectomeNode.l5U;						// gets the number of units components in layer 5 cluster
		fprintf( filePointer, "# rows: %d\n", unitsDim );
		fprintf( filePointer, "# columns: %d\n", inputDim );
		weightsPointer = connectomeNode.l5W;					// gets the pointer to layer 5 cluster weights
		for (i = 0; i < unitsDim; i++)
		{
				for (j = 0; j < inputDim; j++)
				{
					fprintf( filePointer, " %lf", *((weightsPointer + i*inputDim) + j) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		/////////////////////////// writes layer 6 cluster weights into file with fprintf

		fprintf( filePointer, "# name: Layer6Cluster\n" );
		fprintf( filePointer, "# type: matrix\n" );
		inputDim = connectomeNode.l6I;						// gets the number of input components in layer 6 cluster
		unitsDim = connectomeNode.l6U;						// gets the number of units components in layer 6 cluster
		fprintf( filePointer, "# rows: %d\n", unitsDim );
		fprintf( filePointer, "# columns: %d\n", inputDim );
		weightsPointer = connectomeNode.l6W;					// gets the pointer to layer 6 cluster weights
		for (i = 0; i < unitsDim; i++)
		{
				for (j = 0; j < inputDim; j++)
				{
					fprintf( filePointer, " %lf", *((weightsPointer + i*inputDim) + j) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		/////////////////////////// writes layer 23 forward array clusters weights into file with fprintf

		char	*groupClusterDimensionality = NULL;
		char	*aux = NULL;
		fprintf( filePointer, "# name: Layer23ForwardArrayCluster\n" );
		fprintf( filePointer, "# type: matrix\n" );
		fprintf( filePointer, "# ndims: 3\n" );
		inputDim = connectomeNode.l23FGAI;					// gets the number of input components in layer 23 forward array clusters
		unitsDim = connectomeNode.l23FGAU;					// gets the number of units components in layer 23 forward array clusters
		clusters = connectomeNode.l23FGC;					// gets the number of clusters in layer 23 forward group

		aux = digitsString(unitsDim);
		groupClusterDimensionality = (char *)calloc(strlen(aux),sizeof(char));
		groupClusterDimensionality = strcat(groupClusterDimensionality, aux);
		groupClusterDimensionality = (char *)realloc(groupClusterDimensionality,1);
		groupClusterDimensionality = strcat(groupClusterDimensionality, " ");
		free(aux);
		aux = NULL;

		aux = digitsString(inputDim);
		groupClusterDimensionality = (char *)realloc(groupClusterDimensionality,strlen(aux));
		groupClusterDimensionality = strcat(groupClusterDimensionality, aux);
		groupClusterDimensionality = (char *)realloc(groupClusterDimensionality,1);
		groupClusterDimensionality = strcat(groupClusterDimensionality, " ");
		free(aux);
		aux = NULL;

		aux = digitsString(clusters);
		groupClusterDimensionality = (char *)realloc(groupClusterDimensionality,strlen(aux));
		groupClusterDimensionality = strcat(groupClusterDimensionality, aux);
		groupClusterDimensionality = (char *)realloc(groupClusterDimensionality,1);
		groupClusterDimensionality = strcat(groupClusterDimensionality, " ");
		free(aux);
		aux = NULL;

		fputs(groupClusterDimensionality,filePointer);
		free(groupClusterDimensionality);
		groupClusterDimensionality = NULL;

		for ( c = 0; c < clusters; c++ )
		{
			weightsPointer = connectomeNode.l23FGAW[c];			// gets the pointer to layer 23 forward first cluster weights
			for (j = 0; j < inputDim; j++)
			{
				for (i = 0; i < unitsDim; i++)
				{
					fprintf( filePointer, "\n" );
					fprintf( filePointer, " %lf", *((weightsPointer + i*inputDim) + j) );
				}

			}
		}
		fprintf( filePointer, "\n\n\n" );

		/////////////////////////// writes layer 23 backward array clusters weights into file with fprintf

		fprintf( filePointer, "# name: Layer23BackwardArrayCluster\n" );
		fprintf( filePointer, "# type: matrix\n" );
		fprintf( filePointer, "# ndims: 3\n" );
		inputDim = connectomeNode.l23BGAI;					// gets the number of input components in layer 23 backward array clusters
		unitsDim = connectomeNode.l23BGAU;					// gets the number of units components in layer 23 backward array clusters
		clusters = connectomeNode.l23BGC;					// gets the number of clusters in layer 23 backward group

		aux = digitsString(unitsDim);
		groupClusterDimensionality = (char *)calloc(strlen(aux),sizeof(char));
		groupClusterDimensionality = strcat(groupClusterDimensionality, aux);
		groupClusterDimensionality = (char *)realloc(groupClusterDimensionality,1);
		groupClusterDimensionality = strcat(groupClusterDimensionality, " ");
		free(aux);
		aux = NULL;

		aux = digitsString(inputDim);
		groupClusterDimensionality = (char *)realloc(groupClusterDimensionality,strlen(aux));
		groupClusterDimensionality = strcat(groupClusterDimensionality, aux);
		groupClusterDimensionality = (char *)realloc(groupClusterDimensionality,1);
		groupClusterDimensionality = strcat(groupClusterDimensionality, " ");
		free(aux);
		aux = NULL;

		aux = digitsString(clusters);
		groupClusterDimensionality = (char *)realloc(groupClusterDimensionality,strlen(aux));
		groupClusterDimensionality = strcat(groupClusterDimensionality, aux);
		groupClusterDimensionality = (char *)realloc(groupClusterDimensionality,1);
		groupClusterDimensionality = strcat(groupClusterDimensionality, " ");
		free(aux);
		aux = NULL;

		fputs(groupClusterDimensionality,filePointer);
		free(groupClusterDimensionality);
		groupClusterDimensionality = NULL;

		for ( c = 0; c < clusters; c++ )
		{
			weightsPointer = connectomeNode.l23BGAW[c];			// gets the pointer to layer 23 forward first cluster weights
			for (j = 0; j < inputDim; j++)
			{
				for (i = 0; i < unitsDim; i++)
				{
					fprintf( filePointer, "\n" );
					fprintf( filePointer, " %lf", *((weightsPointer + i*inputDim) + j) );
				}
			}
		}
		fprintf( filePointer, "\n\n\n" );

		/////////////////////////// writes the forward group number of cluster as a variable into file with fprintf

		fprintf( filePointer, "# name: forwardGroupClusters\n" );
		fprintf( filePointer, "# type: scalar\n" );
		clusters = connectomeNode.l23FGC;					// gets the number of clusters in layer 23 forward group
		fprintf( filePointer, "%d\n\n", clusters );

		/////////////////////////// writes the backward group number of cluster as a variable into file with fprintf

		fprintf( filePointer, "# name: backwardGroupClusters\n" );
		fprintf( filePointer, "# type: scalar\n" );
		clusters = connectomeNode.l23BGC;					// gets the number of clusters in layer 23 backward group
		fprintf( filePointer, "%d\n\n", clusters );

		fclose( filePointer );							// fclose closes file
	}										// end else
} // end function saveConnectomeForOctave


// function to decompose an integer number in its individual digits
char	*digitsString( int number )
{
	char	*digits = NULL;
	char	*dig = NULL;
	int	i = 0, digit;
	while(number > 0)
	{
		digits = (char *)realloc(digits,1);
		digit = (int)(number % 10);
	   	digits[i] = digit + '0';				// to get the right most digit
		number /= 10;						// reduce the number by one digit
		++i;
	}
	digits[i]='\0';							// terminate the string

	dig = (char *)calloc(i+1, sizeof(char));			// creates a new string
	dig[i] = digits[i];
	int	j;
	int	top = i;
	for ( j = 0; j < top; j++ )					// this loop is to put the digits in the right order inside of dig array
	{
		dig[j] = digits[i-1];
		--i;
	}
	free(digits);							// frees the space used by digits
	return	dig;							// returns a pointer to dif array
} // end function digit


// function to load the input data matrix
inputData	loadInputData()
{
	int i, j;

	inputData	data;

	char	firstLine[25];
	char	nameLine[15];
	char	typeLine[16];
	char	rowsLine[8];
	char	columnsLine[11];

	FILE *filePointer; 								// ConnectomeNode.conn file pointer

	// fopen opens file. Exit program if unable to create file
	if ( ( filePointer = fopen( "/home/dario/MEGA/Doctorado/Contenidos/Model/Octave/MFCC.mat", "r" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		/////////////////////////// reads check the preamble (first line) from the file MFCC.mat 
		fgets(firstLine, 25, filePointer);
		if ( strcmp(firstLine, "# This is a file created") != 0)
		{
			printf("File corrupted; read: %s when it was expected to read: # This is a file created\n", firstLine);
			exit( EXIT_FAILURE );
		}
		passNewLine(filePointer);

		/////////////////////////// reads the name (name line) of the input variable

		fgets(nameLine, 15, filePointer);
		if ( strcmp(nameLine, "# name: MFCC1\n") != 0)
		{
			printf("File corrupted; read: %s when it was expected to read: # name: Coef\n", nameLine);
			exit( EXIT_FAILURE );
		}

		/////////////////////////// reads the type (type line) of the input variable

		fgets(typeLine, 16, filePointer);
		if ( strcmp(typeLine, "# type: matrix\n") != 0)
		{
			printf("File corrupted; read: %s when it was expected to read: # type: matrix\n", typeLine);
			exit( EXIT_FAILURE );
		}

		/////////////////////////// reads the rows (rows line) of the input variable

		fgets(rowsLine, 8, filePointer);
		if ( strcmp(rowsLine, "# rows:") != 0)
		{
			printf("File corrupted; read: %s when it was expected to read: # rows:\n", rowsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &data.chunks);
		passNewLine(filePointer);

		/////////////////////////// reads the columns (columns line) of the input variable

		fgets(columnsLine, 11, filePointer);
		if ( strcmp(columnsLine, "# columns:") != 0)
		{
			printf("File corrupted; read: %s when it was expected to read: # columns: \n", columnsLine);
			exit( EXIT_FAILURE );
		}
		fscanf(filePointer, "%d", &data.filters);
		passNewLine(filePointer);

		/////////////////////////// reads the data to feed the model

		data.inputs = (double*)calloc(data.chunks*data.filters, sizeof(double));	// reserves space for data
		for (i = 0; i < data.chunks; i++)
		{
				for (j = 0; j < data.filters; j++)
				{
					fscanf(filePointer, " %lf", ((data.inputs + i*data.filters) + j) );
				}
				passNewLine(filePointer);
		}
		passNewLine(filePointer);
		passNewLine(filePointer);

/////////////////////////////////////////////////////////////////// Shows the input data arrays on screen

		char	option;

		printf("\nDo you want to show the input data arrays on screen? (Y) for yes and any key for no\n\n");
		scanf(" %c", &option);

		if ( option == 'Y')
		{
			printf("Number of chunks of the input: %d\n", data.chunks);
			printf("Number of filters of the input: %d\n", data.filters);
			printf("%s", nameLine);
			for (i = 0; i < data.chunks; i++)
			{
					for (j = 0; j < data.filters; j++)
					{
						printf(" %lf", *((data.inputs + i*data.filters) + j) );
					}
					printf("\n");
			}
			printf("\n\n");

		}

		fclose( filePointer );							// fclose closes file

	return	data;
	}

} // end function loadInputData


// function to compute the energy variation between two matrices
double	deltaEnergy( double *array1, double *array2, int rows, int columns )
{
	int i, j;

	double averageEnergy, accumulation, difference;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < columns; j++)
		{
			difference = *((array1 + i*columns) + j) - *((array2 + i*columns) + j);
			if (difference < 0)
				difference = -difference;

			accumulation = accumulation + difference;
		}
	}

	averageEnergy = accumulation/(rows*columns);
	return averageEnergy;
} // end function deltaEnergy


// function to save the energy evolution of the cortical node for visualization issues on Octave format
void	saveEnergyForOctave( double *energy, int rows, int columns )
{
	int	i, j, c;

	double	*weightsPointer;

	FILE *filePointer; 								// ConnectomeNode.conn file pointer

	// fopen opens file. Exit program if unable to create file
	if ( ( filePointer = fopen( "/home/dario/MEGA/Doctorado/Contenidos/Model/Octave/Energy.mat", "w" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		fprintf( filePointer, "# This is a file created by saveEnergyForOctave function.\n" );

		/////////////////////////// writes the energy evolution on layer 5 cluster weights into file with fprintf

		fprintf( filePointer, "# name: Energy\n" );
		fprintf( filePointer, "# type: matrix\n" );
		fprintf( filePointer, "# rows: %d\n", rows );
		fprintf( filePointer, "# columns: %d\n", columns );
		for (i = 0; i < rows; i++)
		{
				for (j = 0; j < columns; j++)
				{
					fprintf( filePointer, " %lf", *((energy + i*columns) + j) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );


		fclose( filePointer );							// fclose closes file
	}										// end else
} // end function saveEnergyForOctave


// function to save the firing of a cluster for Octave visualization issues
void	saveFiringForOctave( double *fireUnits, int rows, int columns, double *fireOutputs, int rows1, int columns1, std::string fileName )
{
	int	i, j;
	std::string	filePath = "/home/dario/MEGA/Doctorado/Contenidos/Model/Octave/HSTM_Output/";
	std::string	completePath;

	completePath = filePath + fileName;

	FILE *filePointer; 								// ConnectomeNode.conn file pointer

	const char *cstr = completePath.c_str();
	// fopen opens file. Exit program if unable to create file
	if ( ( filePointer = fopen( cstr, "w" ) ) == NULL ) {
		printf( "File could not be opened\n" );
		exit( EXIT_FAILURE );
	} // end if
	else {
		fprintf( filePointer, "# This is a file created by saveFiringForOctave function.\n" );

		/////////////////////////// writes the firing units cluster data into file with fprintf

		fprintf( filePointer, "# name: FiringsUnits\n" );
		fprintf( filePointer, "# type: matrix\n" );
		fprintf( filePointer, "# rows: %d\n", rows );
		fprintf( filePointer, "# columns: %d\n", columns );
		for (i = 0; i < rows; i++)
		{
				for (j = 0; j < columns; j++)
				{
					fprintf( filePointer, " %lf", *((fireUnits + i*columns) + j) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		/////////////////////////// writes the firing output cluster data into file with fprintf

		fprintf( filePointer, "# name: FiringsOutput\n" );
		fprintf( filePointer, "# type: matrix\n" );
		fprintf( filePointer, "# rows: %d\n", rows1 );
		fprintf( filePointer, "# columns: %d\n", columns1 );
		for (i = 0; i < rows1; i++)
		{
				for (j = 0; j < columns1; j++)
				{
					fprintf( filePointer, " %lf", *((fireOutputs + i*columns1) + j) );
				}
				fprintf( filePointer, "\n" );
		}
		fprintf( filePointer, "\n\n" );

		fclose( filePointer );							// fclose closes file
	}										// end else
} // end function saveFiringForOctave













