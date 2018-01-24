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

# File Name:		GenerateRegularColumnInput.m
# Language:		GNU Octave high-level interpreted language.

#This program generates a sequence of inputs for a ComplexSelfOrganizingMap object.

function GenerateRegularColumnInput(numberOfInputs,
				    proximalDimensionality,
				    distalDimensionality,
				    numberOfLinks,
				    sparsity)

	proximalInputs = {};
	for row = 1:numberOfInputs
		SDR = [];
		while (columns(SDR) < proximalDimensionality*sparsity)
			randomLimit = randi(5);
			for i = 1:randomLimit
				SDR = [SDR, randi(proximalDimensionality)-1];
			endfor
		endwhile

		proximalInputs{row,1} = unique(SDR);
	endfor

	distalInputs = {};
	for row = 1:numberOfInputs
		links = {};
		for link_index = 1:numberOfLinks
			SDR = [];
			while (columns(SDR) < distalDimensionality*sparsity)
				randomLimit = randi(5);
				for i = 1:randomLimit
					SDR = [SDR, randi(distalDimensionality)-1];
				endfor
			endwhile

			links{link_index,1} = unique(SDR);
		endfor

		distalInputs{row,1} = links;
	endfor

	save inputs.mat proximalInputs distalInputs -v6
endfunction
