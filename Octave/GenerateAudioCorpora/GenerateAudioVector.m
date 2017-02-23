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

# File Name:		AudioVector.m
# Language:		GNU Octave high-level interpreted language.

# This program reads Corpus.wav file in this folder and generates a new file called "AudioVector.mat" with the vector X and the sample frequency Fs inside it.

# Load the audio file "audio.wav" from this folther. The data is saved in X and Fs is the sampling frequency with which the audio wave has been sampled.
[X,Fs] = audioread(file_in_loadpath("../../Audio/Corpus.wav"));


# Saves vector X and scalar Fs in "AudioVector.mat" file
save ../AudioVector.mat X Fs;











