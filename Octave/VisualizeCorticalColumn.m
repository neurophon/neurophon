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

# File Name:		VisualizeCorticalColumn.m
# Language:		GNU Octave high-level interpreted language.

# This program visualizes a sequence of Sparse Distributed Representations (SDRs).

function VisualizeCorticalColumn()
	load	CorticalColumnOutput.mat

	figure;
	plotname = gcf;
	title("SDR_plot");
	numberOfRows = rows(output);
	numberOfUnits = prod(dimensions);
	for row = 1:1:numberOfRows
		SDR = zeros(1,numberOfUnits);
		for activeUnit = output{row,1}
			SDR(1,activeUnit+1) = 1;
		endfor
		set(0, 'CurrentFigure', plotname)
		cortex = reshape(SDR,dimensions);
		imagesc(cortex');
		title("SDR_plot");
		pause(0.01);
	endfor
endfunction
