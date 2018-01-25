%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%				Author:		Dematties Dario Jesus						%%
%%				Contact:	dariodematties@hotmail.com.ar					%%
%%						dariodematties@yahoo.com.ar					%%
%%						dario.dematties@frm.utn.edu.ar					%%
%%				Project:	Engineering PhD Project						%%
%%				Institution:	Universidad de Buenos Aires					%%
%%						Facultad de Ingeniería (FIUBA)					%%
%%				Workplace:	Instituto de Ingeniería						%%
%%						Biomédica FIUBA	&						%%
%%						CCT CONICET Mendoza INCIHUSA					%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% File Name:		GenerateRegularColumnInput.m
% Language:		GNU Octave high-level interpreted language.

%This program generates a sequence of inputs for a ComplexSelfOrganizingMap object.

function GenerateRegularColumnInput(numberOfInputs, ...
				    proximalDimensionality, ...
				    distalDimensionality, ...
				    numberOfLinks, ...
				    sparsity)

	my_residue = 1*10^(-10);
	proximalInputs = {};
	for row = 1:numberOfInputs
		SDR = [];
		while (size(SDR,2) < proximalDimensionality*sparsity)
			randomLimit = randi(5);
			for i = 1:randomLimit
				SDR = [SDR, randi(proximalDimensionality)-1+my_residue];
			end
		end

		proximalInputs{row,1} = unique(SDR);
	end

	distalInputs = {};
	for row = 1:numberOfInputs
		links = {};
		for link_index = 1:numberOfLinks
			SDR = [];
			while (size(SDR,2) < distalDimensionality*sparsity)
				randomLimit = randi(5);
				for i = 1:randomLimit
					SDR = [SDR, randi(distalDimensionality)-1+my_residue];
				end
			end

			links{link_index,1} = unique(SDR);
		end

		distalInputs{row,1} = links;
	end

	save inputs.mat proximalInputs distalInputs -v6
end
