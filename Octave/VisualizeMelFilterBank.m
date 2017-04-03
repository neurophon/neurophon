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
load RealMFB008.mat;
RA = MFCC1;
load LeftImagMFB008.mat;
LIA = MFCC1;
load RightImagMFB008.mat;
RIA = MFCC1;

load RealMFB016.mat;
RB = MFCC1;
load LeftImagMFB016.mat;
LIB = MFCC1;
load RightImagMFB016.mat;
RIB = MFCC1;

load RealMFB032.mat;
RC = MFCC1;
load LeftImagMFB032.mat;
LIC = MFCC1;
load RightImagMFB032.mat;
RIC = MFCC1;

load RealMFB064.mat;
RD = MFCC1;
load LeftImagMFB064.mat;
LID = MFCC1;
load RightImagMFB064.mat;
RID = MFCC1;

load RealMFB128.mat;
RE = MFCC1;
load LeftImagMFB128.mat;
LIE = MFCC1;
load RightImagMFB128.mat;
RIE = MFCC1;

Columns = columns(MFCC1');

L_A = abs(RA+i*LIA);
L_B = abs(RB+i*LIB);
L_C = abs(RC+i*LIC);
L_D = abs(RD+i*LID);
L_E = abs(RE+i*LIE);

R_A = abs(RA+i*RIA);
R_B = abs(RB+i*RIB);
R_C = abs(RC+i*RIC);
R_D = abs(RD+i*RID);
R_E = abs(RE+i*RIE);

figure
fig1 = gcf;
#figure
#fig2 = gcf;

#cd GenerateKernels
for i = 1:Columns
	#[fA,x] = mexicanhat(-64,64,129,10);
	#A1 = abs(conv(A'(:,i),fA,"same"));
	#[fB,x] = mexicanhat(-64,64,129,8);
	#B1 = abs(conv(B'(:,i),fB,"same"));
	#[fC,x] = mexicanhat(-64,64,129,6);
	#C1 = abs(conv(C'(:,i),fC,"same"));
	#[fD,x] = mexicanhat(-64,64,129,4);
	#D1 = abs(conv(D'(:,i),fD,"same"));
	#[fE,x] = mexicanhat(-64,64,129,2);
	#E1 = abs(conv(E'(:,i),fE,"same"));


	#imagesc(MFCC1'(:,i)');
	#imagesc([A'(:,i) B'(:,i) C'(:,i) D'(:,i) E'(:,i)]');
	#imagesc([A1' B1' C1' D1' E1']');

	#imagesc([A'(:,i) B'(:,i) C'(:,i) D'(:,i) E'(:,i)]');

	#set(0, 'CurrentFigure', fig1)
	imagesc([L_A'(:,i) L_B'(:,i) L_C'(:,i) L_D'(:,i) L_E'(:,i)]');
	#set(0, 'CurrentFigure', fig2)
	#imagesc([R_A'(:,i) R_B'(:,i) R_C'(:,i) R_D'(:,i) R_E'(:,i)]');

	#imagesc([R_A'(:,i)-L_A'(:,i) R_B'(:,i)-L_B'(:,i) R_C'(:,i)-L_C'(:,i) R_D'(:,i)-L_D'(:,i) R_E'(:,i)-L_E'(:,i)]');

	pause(0.1);
endfor
#cd ..


