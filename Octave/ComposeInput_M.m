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

function ComposeInput_M(numberOfVoices, numberOfSyllableConfigurations, numbeOfCorpora, numbeOfVocabularies, variances)
	Residue = 1*10^(-10);
	my_path = '/projects/neurophon/TestsData/5_Way_Corpora/';
	for voice=1:numberOfVoices
		for syllables=1:numberOfSyllableConfigurations
			for corpus=1:numbeOfCorpora
				for vocabulary=1:numbeOfVocabularies
					iterativePath = [my_path 'Voices_' num2str(voice) '/' ...
								 'Syllables_' num2str(syllables) '/' ...
								 'Corpus_' num2str(corpus-1) '/' ...
								 'Vocabulary_' num2str(vocabulary-1) '/'];


					if (variances)
						disp('Reading audio files with applied variances')

						% Load the Mel Filter Bank output.
						current_path = [iterativePath 'original/RealMFB008.mat'];
						load(current_path);
						RA = MFCC1;
						current_path = [iterativePath 'original/LeftImagMFB008.mat'];
						load(current_path);
						LIA = MFCC1;
						%current_path = [iterativePath 'original/RightImagMFB008.mat'];
						%load(current_path);
						%RIA = MFCC1;

						current_path = [iterativePath 'original/RealMFB016.mat'];
						load(current_path);
						RB = MFCC1;
						current_path = [iterativePath 'original/LeftImagMFB016.mat'];
						load(current_path);
						LIB = MFCC1;
						%current_path = [iterativePath 'original/RightImagMFB016.mat'];
						%load(current_path);
						%RIB = MFCC1;

						current_path = [iterativePath 'original/RealMFB032.mat'];
						load(current_path);
						RC = MFCC1;
						current_path = [iterativePath 'original/LeftImagMFB032.mat'];
						load(current_path);
						LIC = MFCC1;
						%current_path = [iterativePath 'original/RightImagMFB032.mat'];
						%load(current_path);
						%RIC = MFCC1;

						current_path = [iterativePath 'original/RealMFB064.mat'];
						load(current_path);
						RD = MFCC1;
						current_path = [iterativePath 'original/LeftImagMFB064.mat'];
						load(current_path);
						LID = MFCC1;
						%current_path = [iterativePath 'original/RightImagMFB064.mat'];
						%load(current_path);
						%RID = MFCC1;

						current_path = [iterativePath 'original/RealMFB128.mat'];
						load(current_path);
						RE = MFCC1;
						current_path = [iterativePath 'original/LeftImagMFB128.mat'];
						load(current_path);
						LIE = MFCC1;
						%current_path = [iterativePath 'original/RightImagMFB128.mat'];
						%load(current_path);
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

						current_path = [iterativePath 'original/inputs.mat'];
						save('-v6', current_path, 'inputs');




						% Load the Mel Filter Bank output.
						current_path = [iterativePath 'whitenoise1/RealMFB008.mat'];
						load(current_path);
						RA = MFCC1;
						current_path = [iterativePath 'whitenoise1/LeftImagMFB008.mat'];
						load(current_path);
						LIA = MFCC1;
						%current_path = [iterativePath 'whitenoise1/RightImagMFB008.mat'];
						%load(current_path);
						%RIA = MFCC1;

						current_path = [iterativePath 'whitenoise1/RealMFB016.mat'];
						load(current_path);
						RB = MFCC1;
						current_path = [iterativePath 'whitenoise1/LeftImagMFB016.mat'];
						load(current_path);
						LIB = MFCC1;
						%current_path = [iterativePath 'whitenoise1/RightImagMFB016.mat'];
						%load(current_path);
						%RIB = MFCC1;

						current_path = [iterativePath 'whitenoise1/RealMFB032.mat'];
						load(current_path);
						RC = MFCC1;
						current_path = [iterativePath 'whitenoise1/LeftImagMFB032.mat'];
						load(current_path);
						LIC = MFCC1;
						%current_path = [iterativePath 'whitenoise1/RightImagMFB032.mat'];
						%load(current_path);
						%RIC = MFCC1;

						current_path = [iterativePath 'whitenoise1/RealMFB064.mat'];
						load(current_path);
						RD = MFCC1;
						current_path = [iterativePath 'whitenoise1/LeftImagMFB064.mat'];
						load(current_path);
						LID = MFCC1;
						%current_path = [iterativePath 'whitenoise1/RightImagMFB064.mat'];
						%load(current_path);
						%RID = MFCC1;

						current_path = [iterativePath 'whitenoise1/RealMFB128.mat'];
						load(current_path);
						RE = MFCC1;
						current_path = [iterativePath 'whitenoise1/LeftImagMFB128.mat'];
						load(current_path);
						LIE = MFCC1;
						%current_path = [iterativePath 'whitenoise1/RightImagMFB128.mat'];
						%load(current_path);
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

						current_path = [iterativePath 'whitenoise1/inputs.mat'];
						save('-v6', current_path, 'inputs');




						% Load the Mel Filter Bank output.
						current_path = [iterativePath 'whitenoise2/RealMFB008.mat'];
						load(current_path);
						RA = MFCC1;
						current_path = [iterativePath 'whitenoise2/LeftImagMFB008.mat'];
						load(current_path);
						LIA = MFCC1;
						%current_path = [iterativePath 'whitenoise2/RightImagMFB008.mat'];
						%load(current_path);
						%RIA = MFCC1;

						current_path = [iterativePath 'whitenoise2/RealMFB016.mat'];
						load(current_path);
						RB = MFCC1;
						current_path = [iterativePath 'whitenoise2/LeftImagMFB016.mat'];
						load(current_path);
						LIB = MFCC1;
						%current_path = [iterativePath 'whitenoise2/RightImagMFB016.mat'];
						%load(current_path);
						%RIB = MFCC1;

						current_path = [iterativePath 'whitenoise2/RealMFB032.mat'];
						load(current_path);
						RC = MFCC1;
						current_path = [iterativePath 'whitenoise2/LeftImagMFB032.mat'];
						load(current_path);
						LIC = MFCC1;
						%current_path = [iterativePath 'whitenoise2/RightImagMFB032.mat'];
						%load(current_path);
						%RIC = MFCC1;

						current_path = [iterativePath 'whitenoise2/RealMFB064.mat'];
						load(current_path);
						RD = MFCC1;
						current_path = [iterativePath 'whitenoise2/LeftImagMFB064.mat'];
						load(current_path);
						LID = MFCC1;
						%current_path = [iterativePath 'whitenoise2/RightImagMFB064.mat'];
						%load(current_path);
						%RID = MFCC1;

						current_path = [iterativePath 'whitenoise2/RealMFB128.mat'];
						load(current_path);
						RE = MFCC1;
						current_path = [iterativePath 'whitenoise2/LeftImagMFB128.mat'];
						load(current_path);
						LIE = MFCC1;
						%current_path = [iterativePath 'whitenoise2/RightImagMFB128.mat'];
						%load(current_path);
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

						current_path = [iterativePath 'whitenoise2/inputs.mat'];
						save('-v6', current_path, 'inputs');




						% Load the Mel Filter Bank output.
						current_path = [iterativePath 'reverberation30/RealMFB008.mat'];
						load(current_path);
						RA = MFCC1;
						current_path = [iterativePath 'reverberation30/LeftImagMFB008.mat'];
						load(current_path);
						LIA = MFCC1;
						%current_path = [iterativePath 'reverberation30/RightImagMFB008.mat'];
						%load(current_path);
						%RIA = MFCC1;

						current_path = [iterativePath 'reverberation30/RealMFB016.mat'];
						load(current_path);
						RB = MFCC1;
						current_path = [iterativePath 'reverberation30/LeftImagMFB016.mat'];
						load(current_path);
						LIB = MFCC1;
						%current_path = [iterativePath 'reverberation30/RightImagMFB016.mat'];
						%load(current_path);
						%RIB = MFCC1;

						current_path = [iterativePath 'reverberation30/RealMFB032.mat'];
						load(current_path);
						RC = MFCC1;
						current_path = [iterativePath 'reverberation30/LeftImagMFB032.mat'];
						load(current_path);
						LIC = MFCC1;
						%current_path = [iterativePath 'reverberation30/RightImagMFB032.mat'];
						%load(current_path);
						%RIC = MFCC1;

						current_path = [iterativePath 'reverberation30/RealMFB064.mat'];
						load(current_path);
						RD = MFCC1;
						current_path = [iterativePath 'reverberation30/LeftImagMFB064.mat'];
						load(current_path);
						LID = MFCC1;
						%current_path = [iterativePath 'reverberation30/RightImagMFB064.mat'];
						%load(current_path);
						%RID = MFCC1;

						current_path = [iterativePath 'reverberation30/RealMFB128.mat'];
						load(current_path);
						RE = MFCC1;
						current_path = [iterativePath 'reverberation30/LeftImagMFB128.mat'];
						load(current_path);
						LIE = MFCC1;
						%current_path = [iterativePath 'reverberation30/RightImagMFB128.mat'];
						%load(current_path);
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

						current_path = [iterativePath 'reverberation30/inputs.mat'];
						save('-v6', current_path, 'inputs');




						% Load the Mel Filter Bank output.
						current_path = [iterativePath 'reverberation60/RealMFB008.mat'];
						load(current_path);
						RA = MFCC1;
						current_path = [iterativePath 'reverberation60/LeftImagMFB008.mat'];
						load(current_path);
						LIA = MFCC1;
						%current_path = [iterativePath 'reverberation60/RightImagMFB008.mat'];
						%load(current_path);
						%RIA = MFCC1;

						current_path = [iterativePath 'reverberation60/RealMFB016.mat'];
						load(current_path);
						RB = MFCC1;
						current_path = [iterativePath 'reverberation60/LeftImagMFB016.mat'];
						load(current_path);
						LIB = MFCC1;
						%current_path = [iterativePath 'reverberation60/RightImagMFB016.mat'];
						%load(current_path);
						%RIB = MFCC1;

						current_path = [iterativePath 'reverberation60/RealMFB032.mat'];
						load(current_path);
						RC = MFCC1;
						current_path = [iterativePath 'reverberation60/LeftImagMFB032.mat'];
						load(current_path);
						LIC = MFCC1;
						%current_path = [iterativePath 'reverberation60/RightImagMFB032.mat'];
						%load(current_path);
						%RIC = MFCC1;

						current_path = [iterativePath 'reverberation60/RealMFB064.mat'];
						load(current_path);
						RD = MFCC1;
						current_path = [iterativePath 'reverberation60/LeftImagMFB064.mat'];
						load(current_path);
						LID = MFCC1;
						%current_path = [iterativePath 'reverberation60/RightImagMFB064.mat'];
						%load(current_path);
						%RID = MFCC1;

						current_path = [iterativePath 'reverberation60/RealMFB128.mat'];
						load(current_path);
						RE = MFCC1;
						current_path = [iterativePath 'reverberation60/LeftImagMFB128.mat'];
						load(current_path);
						LIE = MFCC1;
						%current_path = [iterativePath 'reverberation60/RightImagMFB128.mat'];
						%load(current_path);
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

						current_path = [iterativePath 'reverberation60/inputs.mat'];
						save('-v6', current_path, 'inputs');




						% Load the Mel Filter Bank output.
						current_path = [iterativePath 'pitchdown/RealMFB008.mat'];
						load(current_path);
						RA = MFCC1;
						current_path = [iterativePath 'pitchdown/LeftImagMFB008.mat'];
						load(current_path);
						LIA = MFCC1;
						%current_path = [iterativePath 'pitchdown/RightImagMFB008.mat'];
						%load(current_path);
						%RIA = MFCC1;

						current_path = [iterativePath 'pitchdown/RealMFB016.mat'];
						load(current_path);
						RB = MFCC1;
						current_path = [iterativePath 'pitchdown/LeftImagMFB016.mat'];
						load(current_path);
						LIB = MFCC1;
						%current_path = [iterativePath 'pitchdown/RightImagMFB016.mat'];
						%load(current_path);
						%RIB = MFCC1;

						current_path = [iterativePath 'pitchdown/RealMFB032.mat'];
						load(current_path);
						RC = MFCC1;
						current_path = [iterativePath 'pitchdown/LeftImagMFB032.mat'];
						load(current_path);
						LIC = MFCC1;
						%current_path = [iterativePath 'pitchdown/RightImagMFB032.mat'];
						%load(current_path);
						%RIC = MFCC1;

						current_path = [iterativePath 'pitchdown/RealMFB064.mat'];
						load(current_path);
						RD = MFCC1;
						current_path = [iterativePath 'pitchdown/LeftImagMFB064.mat'];
						load(current_path);
						LID = MFCC1;
						%current_path = [iterativePath 'pitchdown/RightImagMFB064.mat'];
						%load(current_path);
						%RID = MFCC1;

						current_path = [iterativePath 'pitchdown/RealMFB128.mat'];
						load(current_path);
						RE = MFCC1;
						current_path = [iterativePath 'pitchdown/LeftImagMFB128.mat'];
						load(current_path);
						LIE = MFCC1;
						%current_path = [iterativePath 'pitchdown/RightImagMFB128.mat'];
						%load(current_path);
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

						current_path = [iterativePath 'pitchdown/inputs.mat'];
						save('-v6', current_path, 'inputs');




						% Load the Mel Filter Bank output.
						current_path = [iterativePath 'pitchup/RealMFB008.mat'];
						load(current_path);
						RA = MFCC1;
						current_path = [iterativePath 'pitchup/LeftImagMFB008.mat'];
						load(current_path);
						LIA = MFCC1;
						%current_path = [iterativePath 'pitchup/RightImagMFB008.mat'];
						%load(current_path);
						%RIA = MFCC1;

						current_path = [iterativePath 'pitchup/RealMFB016.mat'];
						load(current_path);
						RB = MFCC1;
						current_path = [iterativePath 'pitchup/LeftImagMFB016.mat'];
						load(current_path);
						LIB = MFCC1;
						%current_path = [iterativePath 'pitchup/RightImagMFB016.mat'];
						%load(current_path);
						%RIB = MFCC1;

						current_path = [iterativePath 'pitchup/RealMFB032.mat'];
						load(current_path);
						RC = MFCC1;
						current_path = [iterativePath 'pitchup/LeftImagMFB032.mat'];
						load(current_path);
						LIC = MFCC1;
						%current_path = [iterativePath 'pitchup/RightImagMFB032.mat'];
						%load(current_path);
						%RIC = MFCC1;

						current_path = [iterativePath 'pitchup/RealMFB064.mat'];
						load(current_path);
						RD = MFCC1;
						current_path = [iterativePath 'pitchup/LeftImagMFB064.mat'];
						load(current_path);
						LID = MFCC1;
						%current_path = [iterativePath 'pitchup/RightImagMFB064.mat'];
						%load(current_path);
						%RID = MFCC1;

						current_path = [iterativePath 'pitchup/RealMFB128.mat'];
						load(current_path);
						RE = MFCC1;
						current_path = [iterativePath 'pitchup/LeftImagMFB128.mat'];
						load(current_path);
						LIE = MFCC1;
						%current_path = [iterativePath 'pitchup/RightImagMFB128.mat'];
						%load(current_path);
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

						current_path = [iterativePath 'pitchup/inputs.mat'];
						save('-v6', current_path, 'inputs');




					else
						disp('Reading only original audio files without applied variance')

						% Load the Mel Filter Bank output.
						current_path = [iterativePath 'RealMFB008.mat'];
						load(current_path);
						RA = MFCC1;
						current_path = [iterativePath 'LeftImagMFB008.mat'];
						load(current_path);
						LIA = MFCC1;
						%current_path = [iterativePath 'RightImagMFB008.mat'];
						%load(current_path);
						%RIA = MFCC1;

						current_path = [iterativePath 'RealMFB016.mat'];
						load(current_path);
						RB = MFCC1;
						current_path = [iterativePath 'LeftImagMFB016.mat'];
						load(current_path);
						LIB = MFCC1;
						%current_path = [iterativePath 'RightImagMFB016.mat'];
						%load(current_path);
						%RIB = MFCC1;

						current_path = [iterativePath 'RealMFB032.mat'];
						load(current_path);
						RC = MFCC1;
						current_path = [iterativePath 'LeftImagMFB032.mat'];
						load(current_path);
						LIC = MFCC1;
						%current_path = [iterativePath 'RightImagMFB032.mat'];
						%load(current_path);
						%RIC = MFCC1;

						current_path = [iterativePath 'RealMFB064.mat'];
						load(current_path);
						RD = MFCC1;
						current_path = [iterativePath 'LeftImagMFB064.mat'];
						load(current_path);
						LID = MFCC1;
						%current_path = [iterativePath 'RightImagMFB064.mat'];
						%load(current_path);
						%RID = MFCC1;

						current_path = [iterativePath 'RealMFB128.mat'];
						load(current_path);
						RE = MFCC1;
						current_path = [iterativePath 'LeftImagMFB128.mat'];
						load(current_path);
						LIE = MFCC1;
						%current_path = [iterativePath 'RightImagMFB128.mat'];
						%load(current_path);
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

						current_path = [iterativePath 'inputs.mat'];
						save('-v6', current_path, 'inputs');

					end

				end
			end
		end
	end
end




%% Load the Mel Filter Bank output.
%load RealMFB008.mat;
%RA = MFCC1;
%load LeftImagMFB008.mat;
%LIA = MFCC1;
%%load RightImagMFB008.mat;
%%RIA = MFCC1;

%load RealMFB016.mat;
%RB = MFCC1;
%load LeftImagMFB016.mat;
%LIB = MFCC1;
%%load RightImagMFB016.mat;
%%RIB = MFCC1;

%load RealMFB032.mat;
%RC = MFCC1;
%load LeftImagMFB032.mat;
%LIC = MFCC1;
%%load RightImagMFB032.mat;
%%RIC = MFCC1;

%load RealMFB064.mat;
%RD = MFCC1;
%load LeftImagMFB064.mat;
%LID = MFCC1;
%%load RightImagMFB064.mat;
%%RID = MFCC1;

%load RealMFB128.mat;
%RE = MFCC1;
%load LeftImagMFB128.mat;
%LIE = MFCC1;
%%load RightImagMFB128.mat;
%%RIE = MFCC1;

%Columns = size(MFCC1',2);

%L_A = abs(RA+i*LIA);
%L_B = abs(RB+i*LIB);
%L_C = abs(RC+i*LIC);
%L_D = abs(RD+i*LID);
%L_E = abs(RE+i*LIE);

%%R_A = abs(RA+i*RIA);
%%R_B = abs(RB+i*RIB);
%%R_C = abs(RC+i*RIC);
%%R_D = abs(RD+i*RID);
%%R_E = abs(RE+i*RIE);

%inputs =  [L_A(1:Columns,:)'; L_B(1:Columns,:)'; L_C(1:Columns,:)'; L_D(1:Columns,:)'; L_E(1:Columns,:)']';
%%inputs =  [R_A(1:Columns,:)'; R_B(1:Columns,:)'; R_C(1:Columns,:)'; R_D(1:Columns,:)'; R_E(1:Columns,:)']';

%save inputs.mat inputs -v6;

%end
