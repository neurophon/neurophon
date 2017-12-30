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

% File Name:		mexicanhat.m
% Language:		GNU Octave high-level interpreted language.

% This function computes the mexican hat wavelet kernel.
% A complete Kernel contains:
% A complex vector with the mexican hat in its real part and the mexican hat's Hilbert transform in its imaginary part.
function [psi,x] = mexicanhat(lb,ub,n,coefficient)
	if (nargin < 3); print_usage; end

	if (n <= 0)
		error('n must be strictly positive');
	end
	x = linspace(lb,ub,n);
	psi = hilbert( (1-x.^2./coefficient.^2).*(2/(sqrt(3*coefficient)*pi^0.25)) .* exp(-x.^2./(2.*coefficient.^2)) );
end

