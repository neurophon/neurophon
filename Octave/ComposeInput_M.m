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

% File Name:		ComposeInput_M.m
% Language:		GNU Octave high-level interpreted language.

% This program compose the input as a multidimensional array
% from the Mel Filter Bank.

function ComposeInput_M()

% Load the Mel Filter Bank output.
load RealMFB008.mat;
RA = MFCC1;
load LeftImagMFB008.mat;
LIA = MFCC1;
%load RightImagMFB008.mat;
%RIA = MFCC1;

load RealMFB016.mat;
RB = MFCC1;
load LeftImagMFB016.mat;
LIB = MFCC1;
%load RightImagMFB016.mat;
%RIB = MFCC1;

load RealMFB032.mat;
RC = MFCC1;
load LeftImagMFB032.mat;
LIC = MFCC1;
%load RightImagMFB032.mat;
%RIC = MFCC1;

load RealMFB064.mat;
RD = MFCC1;
load LeftImagMFB064.mat;
LID = MFCC1;
%load RightImagMFB064.mat;
%RID = MFCC1;

load RealMFB128.mat;
RE = MFCC1;
load LeftImagMFB128.mat;
LIE = MFCC1;
%load RightImagMFB128.mat;
%RIE = MFCC1;

Columns = size(MFCC1',2);

L_A = abs(RA+i*LIA);
L_B = abs(RB+i*LIB);
L_C = abs(RC+i*LIC);
L_D = abs(RD+i*LID);
L_E = abs(RE+i*LIE);

%R_A = abs(RA+i*RIA);
%R_B = abs(RB+i*RIB);
%R_C = abs(RC+i*RIC);
%R_D = abs(RD+i*RID);
%R_E = abs(RE+i*RIE);

inputs =  [L_A(1:Columns,:)'; L_B(1:Columns,:)'; L_C(1:Columns,:)'; L_D(1:Columns,:)'; L_E(1:Columns,:)']';
%inputs =  [R_A(1:Columns,:)'; R_B(1:Columns,:)'; R_C(1:Columns,:)'; R_D(1:Columns,:)'; R_E(1:Columns,:)']';

save inputs.mat inputs;

end
