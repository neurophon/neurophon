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

# File Name:		VisualizeMelFilterBank.m
# Language:		GNU Octave high-level interpreted language.

# This program visualizes the Mel Filter Bank temporal evolution.

# Load the Mel Filter Bank output.
load MFB008.mat
A = MFCC1;

load MFB016.mat
B = MFCC1;

load MFB032.mat
C = MFCC1;

load MFB064.mat
D = MFCC1;

load MFB128.mat
E = MFCC1;

Columns = columns(MFCC1');

for i = 1:Columns
	#imagesc(MFCC1'(:,i)');
	#imagesc([A'(:,i) B'(:,i)]');
	imagesc([A'(:,i) B'(:,i) C'(:,i) D'(:,i) E'(:,i)]');
	pause(0.1);
endfor



