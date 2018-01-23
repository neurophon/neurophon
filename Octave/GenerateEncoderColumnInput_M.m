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

% File Name:		GenerateEncoderColumnInput_M.m
% Language:		GNU Octave high-level interpreted language.

% This program generates a sequence of inputs for a ComplexSelfOrganizingMap object.

function GenerateEncoderColumnInput_M(numberOfInputs, ...
				      proximalDimensionality, ...
				      distalDimensionality, ...
				      numberOfLinks, ...
				      sparsity)

	my_residue = 1*10^(-10);
	proximalInputs = rand(numberOfInputs,proximalDimensionality);

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
