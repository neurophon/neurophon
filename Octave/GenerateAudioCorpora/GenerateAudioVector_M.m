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

% File Name:		GenerateAudioVector_M.m
% Language:		GNU Octave high-level interpreted language.

% This program reads Corpus.wav file in this folder and generates a new file called 'AudioVector.mat' with the vector X and the sample frequency Fs inside of it.

function GenerateAudioVector (numberOfVoices, numberOfSyllableConfigurations, numbeOfCorpora, numbeOfVocabularies, variances)
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
						disp('Reading audio corpora with applied variances')
						% Load the audio file 'audio.wav' from this folther.
						% The data is saved in X and Fs is the sampling frequency with which the audio wave has been sampled.
						current_path = [iterativePath 'original/Corpus.wav'];
						[X,Fs] = audioread(current_path);
						X = X + Residue;
						Fs = Fs + Residue;
						% Saves vector X and scalar Fs in 'AudioVector.mat' file
						current_path = [iterativePath 'original/AudioVector.mat'];
						save('-v6', current_path, 'X', 'Fs');

						% The data is saved in X and Fs is the sampling frequency with which the audio wave has been sampled.
						current_path = [iterativePath 'whitenoise1/Corpus.wav'];
						[X,Fs] = audioread(current_path);
						X = X + Residue;
						Fs = Fs + Residue;
						% Saves vector X and scalar Fs in 'AudioVector.mat' file
						current_path = [iterativePath 'whitenoise1/AudioVector.mat'];
						save('-v6', current_path, 'X', 'Fs');

						% The data is saved in X and Fs is the sampling frequency with which the audio wave has been sampled.
						current_path = [iterativePath 'whitenoise2/Corpus.wav'];
						[X,Fs] = audioread(current_path);
						X = X + Residue;
						Fs = Fs + Residue;
						% Saves vector X and scalar Fs in 'AudioVector.mat' file
						current_path = [iterativePath 'whitenoise2/AudioVector.mat'];
						save('-v6', current_path, 'X', 'Fs');

						% The data is saved in X and Fs is the sampling frequency with which the audio wave has been sampled.
						current_path = [iterativePath 'reverberation30/Corpus.wav'];
						[X,Fs] = audioread(current_path);
						X = X + Residue;
						Fs = Fs + Residue;
						% Saves vector X and scalar Fs in 'AudioVector.mat' file
						current_path = [iterativePath 'reverberation30/AudioVector.mat'];
						save('-v6', current_path, 'X', 'Fs');

						% The data is saved in X and Fs is the sampling frequency with which the audio wave has been sampled.
						current_path = [iterativePath 'reverberation60/Corpus.wav'];
						[X,Fs] = audioread(current_path);
						X = X + Residue;
						Fs = Fs + Residue;
						% Saves vector X and scalar Fs in 'AudioVector.mat' file
						current_path = [iterativePath 'reverberation60/AudioVector.mat'];
						save('-v6', current_path, 'X', 'Fs');

						% The data is saved in X and Fs is the sampling frequency with which the audio wave has been sampled.
						current_path = [iterativePath 'pitchdown/Corpus.wav'];
						[X,Fs] = audioread(current_path);
						X = X + Residue;
						Fs = Fs + Residue;
						% Saves vector X and scalar Fs in 'AudioVector.mat' file
						current_path = [iterativePath 'pitchdown/AudioVector.mat'];
						save('-v6', current_path, 'X', 'Fs');

						% The data is saved in X and Fs is the sampling frequency with which the audio wave has been sampled.
						current_path = [iterativePath 'pitchup/Corpus.wav'];
						[X,Fs] = audioread(current_path);
						X = X + Residue;
						Fs = Fs + Residue;
						% Saves vector X and scalar Fs in 'AudioVector.mat' file
						current_path = [iterativePath 'pitchup/AudioVector.mat'];
						save('-v6', current_path, 'X', 'Fs');
					else
						disp('Reading only original audio corpora without applied variance')
						% The data is saved in X and Fs is the sampling frequency with which the audio wave has been sampled.
						current_path = [iterativePath 'Corpus.wav'];
						[X,Fs] = audioread(current_path);
						X = X + Residue;
						Fs = Fs + Residue;
						% Saves vector X and scalar Fs in 'AudioVector.mat' file
						current_path = [iterativePath 'AudioVector.mat'];
						save('-v6', current_path, 'X', 'Fs');
					end




				end
			end
		end
	end
end

%% Load the audio file 'audio.wav' from this folther. The data is saved in X and Fs is the sampling frequency with which the audio wave has been sampled.
%[X,Fs] = audioread('../../Audio/Corpus.wav');

%Residue = 1*10^(-10);
%X = X + Residue;
%Fs = Fs + Residue;
%% Saves vector X and scalar Fs in 'AudioVector.mat' file
%save ../AudioVector.mat X Fs -v6;











