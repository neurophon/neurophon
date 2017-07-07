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

function VisualizeModelEvolution(finishProcessingAt)
# Load inputs.mat.
load inputs.mat
##{
if ( finishProcessingAt >= 1 )
	# Load OutputLayer_H1_L4.mat.
	load outputLayer_H1_L4.mat
	columnsArrayDimensionality_H1_L4 = columnsArrayDimensionality;
	populationsArrayDimensionality_H1_L4 = populationsArrayDimensionality;
	layerOutput_H1_L4 = layerOutput;
endif
if ( finishProcessingAt >= 2 )
	# Load OutputLayer_H1_L23.mat.
	load outputLayer_H1_L23.mat
	columnsArrayDimensionality_H1_L23 = columnsArrayDimensionality;
	populationsArrayDimensionality_H1_L23 = populationsArrayDimensionality;
	layerOutput_H1_L23 = layerOutput;
endif
if ( finishProcessingAt >= 3 )
	# Load OutputLayer_H2_L4.mat.
	load outputLayer_H2_L4.mat
	columnsArrayDimensionality_H2_L4 = columnsArrayDimensionality;
	populationsArrayDimensionality_H2_L4 = populationsArrayDimensionality;
	layerOutput_H2_L4 = layerOutput;
endif
#}

figure
inputPlot = gcf;
title("Inputs");

if ( finishProcessingAt >= 1 )
	figure
	outputLayer_H1_L4 = gcf;
	title("H1_L4");
endif
if ( finishProcessingAt >= 2 )
	figure
	outputLayer_H1_L23 = gcf;
	title("H1_L23");
endif
if ( finishProcessingAt >= 3 )
	figure
	outputLayer_H2_L4 = gcf;
	title("H2_L4");
endif

cd Layer_Visualization
timeSteps = rows(inputs);

for t = 1:timeSteps
##{
	set(0, 'CurrentFigure', inputPlot)
	array = reshape(inputs(t,:),128,5);
	imagesc(array');
	title("Inputs");
#}

##{
if ( finishProcessingAt >= 1 )
	cortex = zeros(1,prod(columnsArrayDimensionality_H1_L4)*prod(populationsArrayDimensionality_H1_L4));
	numberOfColumns = prod(columnsArrayDimensionality_H1_L4);
	for ( column = 1:numberOfColumns)
		columnsCoordinates = unravelIndex(column-1,columnsArrayDimensionality_H1_L4);
		reference = columnsCoordinates.*populationsArrayDimensionality_H1_L4;
		if ( any(layerOutput_H1_L4{t,1}{column,1}( 1,: ) != -1) )
			alternatives = columns(layerOutput_H1_L4{t,1}{column,1}( 1,: ));
			for alt = 1:alternatives
				if ( layerOutput_H1_L4{t,1}{column,1}( 1,alt ) >= 0 )
					populationCoordinates = unravelIndex(layerOutput_H1_L4{t,1}{column,1}( 1,alt ),populationsArrayDimensionality_H1_L4);
					layerCoordinates = reference.+populationCoordinates;
					cortexIndex = ravelMultiIndex(layerCoordinates,columnsArrayDimensionality_H1_L4.*populationsArrayDimensionality_H1_L4);
					cortex(1,cortexIndex+1) = 1;
				endif
			endfor
		endif
	endfor
	set(0, 'CurrentFigure', outputLayer_H1_L4)
	cortex = reshape(cortex,flipud(columnsArrayDimensionality_H1_L4')'.*populationsArrayDimensionality_H1_L4);
	imagesc(cortex');
	title("H1_L4");
endif
#}
##{
if ( finishProcessingAt >= 2 )
	cortex = zeros(1,prod(columnsArrayDimensionality_H1_L23)*prod(populationsArrayDimensionality_H1_L23));
	numberOfColumns = prod(columnsArrayDimensionality_H1_L23);
	for ( column = 1:numberOfColumns)
		columnsCoordinates = unravelIndex(column-1,columnsArrayDimensionality_H1_L23);
		reference = columnsCoordinates.*populationsArrayDimensionality_H1_L23;
		if ( any(layerOutput_H1_L23{t,1}{column,1}( 1,: ) != -1) )
			alternatives = columns(layerOutput_H1_L23{t,1}{column,1}( 1,: ));
			for alt = 1:alternatives
				if ( layerOutput_H1_L23{t,1}{column,1}( 1,alt ) >= 0 )
					populationCoordinates = unravelIndex(layerOutput_H1_L23{t,1}{column,1}( 1,alt ),populationsArrayDimensionality_H1_L23);
					layerCoordinates = reference.+populationCoordinates;
					cortexIndex = ravelMultiIndex(layerCoordinates,columnsArrayDimensionality_H1_L23.*populationsArrayDimensionality_H1_L23);
					cortex(1,cortexIndex+1) = 1;
				endif
			endfor
		endif
	endfor
	set(0, 'CurrentFigure', outputLayer_H1_L23)
	cortex = reshape(cortex,flipud(columnsArrayDimensionality_H1_L23')'.*populationsArrayDimensionality_H1_L23);
	imagesc(cortex');
	title("H1_L23");
endif
#}
##{
if ( finishProcessingAt >= 3 )
	cortex = zeros(1,prod(columnsArrayDimensionality_H2_L4)*prod(populationsArrayDimensionality_H2_L4));
	numberOfColumns = prod(columnsArrayDimensionality_H2_L4);
	for ( column = 1:numberOfColumns)
		columnsCoordinates = unravelIndex(column-1,columnsArrayDimensionality_H2_L4);
		reference = columnsCoordinates.*populationsArrayDimensionality_H2_L4;
		if ( any(layerOutput_H2_L4{t,1}{column,1}( 1,: ) != -1) )
			alternatives = columns(layerOutput_H2_L4{t,1}{column,1}( 1,: ));
			for alt = 1:alternatives
				if ( layerOutput_H2_L4{t,1}{column,1}( 1,alt ) >= 0 )
					populationCoordinates = unravelIndex(layerOutput_H2_L4{t,1}{column,1}( 1,alt ),populationsArrayDimensionality_H2_L4);
					layerCoordinates = reference.+populationCoordinates;
					cortexIndex = ravelMultiIndex(layerCoordinates,columnsArrayDimensionality_H2_L4.*populationsArrayDimensionality_H2_L4);
					cortex(1,cortexIndex+1) = 1;
				endif
			endfor
		endif
	endfor
	set(0, 'CurrentFigure', outputLayer_H2_L4)
	cortex = reshape(cortex,flipud(columnsArrayDimensionality_H2_L4')'.*populationsArrayDimensionality_H2_L4);
	imagesc(cortex');
	title("H2_L4");
endif
#}
	pause(0.1);
endfor

cd ..

endfunction
