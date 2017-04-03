##################################################################################################################
##				Author:		Dematties Dario Jesus						##
##				Contact:	dariodematties@hotmail.com.ar					##
##						dariodematties@yahoo.com.ar					##
##						dario.dematties@frm.utn.edu.ar					##
##				Project:	Engineering PhD Project						##
##				Institution:	Universidad de Buenos Aires					##
##						Facultad de Ingeniería (FIUBA)					##
##				Workplace:	Instituto de Ingeniería						##
##						Biomédica FIUBA	&						##
##						CCT CONICET Mendoza INCIHUSA					##
##################################################################################################################

# File Name:		VisualizeModelEvolution.m
# Language:		GNU Octave high-level interpreted language.

# This program visualizes the Mel Filter Bank temporal evolution.

# Load inputs.mat.
load inputs.mat

# Load OutputLayer.mat.
load outputLayer.mat


figure
inputPlot = gcf;

figure
outputLayer = gcf;

cd Layer_Visualization
timeSteps = rows(inputs);

for t = 1:timeSteps
	set(0, 'CurrentFigure', inputPlot)
	array = reshape(inputs(t,:),128,5);
	imagesc(array');
	cortex = zeros(1,prod(columnsArrayDimensionality)*prod(populationsArrayDimensionality));
	numberOfColumns = prod(columnsArrayDimensionality);
	for ( column = 1:numberOfColumns)
		columnsCoordinates = unravelIndex(column-1,columnsArrayDimensionality);
		reference = columnsCoordinates.*populationsArrayDimensionality;
		if ( any(layerOutput(:,column,t) != -1) )
			alternatives = rows(layerOutput);
			for alt = 1:alternatives
				if ( layerOutput(alt,column,t) >= 0 )
					populationCoordinates = unravelIndex(layerOutput(alt,column,t),populationsArrayDimensionality);
					layerCoordinates = reference.+populationCoordinates;
					cortexIndex = ravelMultiIndex(layerCoordinates,columnsArrayDimensionality.*populationsArrayDimensionality);
					cortex(1,cortexIndex+1) = 1;
				endif
			endfor
		endif
	endfor
	set(0, 'CurrentFigure', outputLayer)
	cortex = reshape(cortex,flipud(columnsArrayDimensionality')'.*populationsArrayDimensionality);
	imagesc(cortex');
	pause(0.01);
endfor

cd ..
