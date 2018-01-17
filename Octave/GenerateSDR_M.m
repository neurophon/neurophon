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

% File Name:		GenerateSDR_M.m
% Language:		GNU Octave high-level interpreted language.

%This program generates a sequence of Sparse Distributed Representations (SDRs) in order to train models.

function GenerateSDR_M(numberOfSDRs, dimensionality, sparsity)
	my_residue = 1*10^(-10);
	inputVector = {};
	for row = 1:numberOfSDRs
		SDR = [];
		while (size(SDR,2) < dimensionality*sparsity)
			randomLimit = randi(5);
			for i = 1:randomLimit
				SDR = [SDR, randi(dimensionality)-1+my_residue];
			end
		end

		inputVector{row,1} = unique(SDR);
	end

	save inputs.mat inputVector -v6
end
