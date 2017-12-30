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

% File Name:		GenerateKernels.m
% Language:		GNU Octave high-level interpreted language.

% This program generates the Kernels for the cortical processing.

% Generates the Kernel for the MFB008
[f,x] = mexicanhat_M(-63,64,128,10);
% Saves real part of the vector f in "../RealKernel008.mat" file
Aux = real(f);
save ../RealKernel008.mat Aux -v6;
% Saves imaginary part of the vector f in "../ImagKernel008.mat" file
Aux = imag(f);
save ../ImagKernel008.mat Aux -v6;

% Generates the Kernel for the MFB016
[f,x] = mexicanhat_M(-63,64,128,8);
% Saves real part of the vector f in "../RealKernel016.mat" file
Aux = real(f);
save ../RealKernel016.mat Aux -v6;
% Saves imaginary part of the vector f in "../ImagKernel016.mat" file
Aux = imag(f);
save ../ImagKernel016.mat Aux -v6;

% Generates the Kernel for the MFB032
[f,x] = mexicanhat_M(-63,64,128,6);
% Saves real part of the vector f in "../RealKernel032.mat" file
Aux = real(f);
save ../RealKernel032.mat Aux -v6;
% Saves imaginary part of the vector f in "../ImagKernel032.mat" file
Aux = imag(f);
save ../ImagKernel032.mat Aux -v6;

% Generates the Kernel for the MFB064
[f,x] = mexicanhat_M(-63,64,128,4);
% Saves real part of the vector f in "../RealKernel064.mat" file
Aux = real(f);
save ../RealKernel064.mat Aux -v6;
% Saves imaginary part of the vector f in "../ImagKernel064.mat" file
Aux = imag(f);
save ../ImagKernel064.mat Aux -v6;

% Generates the Kernel for the MFB128
[f,x] = mexicanhat_M(-63,64,128,2);
% Saves real part of the vector f in "../RealKernel128.mat" file
Aux = real(f);
save ../RealKernel128.mat Aux -v6;
% Saves imaginary part of the vector f in "../ImagKernel128.mat" file
Aux = imag(f);
save ../ImagKernel128.mat Aux -v6;

