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

# File Name:		Generate_Training_SABLE.m
# Language:		GNU Octave high-level interpreted language.

# This program generates a corpus in SABLE markup format and save it in a file called "".sable in this folder.
# SABLE is collaboration between Sun, AT&T, Bell Labs, Edinburgh and CMU to devise a standard cross synthesizer standard mark up language.
# The language is XML-based and allows users to add addition controlling commands in text to affect the output.
# An implementation exists in Festival as well as Bell Labs synthesis system. 

function Generate_Training_SABLE( nameOfFile, voices, vocabulary, turns, wordsPerTurn )

if (nargin != 5)
	temporalString = ["Generate_Training_SABLE function must receive 5 arguments\n"];
	disp(temporalString);
	return;
endif

# Holds the sequence of words in the corpus
# Such variable is specified by a sequence of numbers corresponding to the location
# of the words in the vocabulary
global	wordsSequence;
global	speakersSequence;

filename = [nameOfFile, ".sable"];
fid = fopen (filename, "w");

InsertHeader(fid);

numberOfVoices = rows (voices);			# the number of voices
numberOfWords = rows (vocabulary);		# the number of words in the vocabulary

count = 0;
usedVoices = zeros(1,numberOfVoices);		# array whose members different from zero indicates the voices that have been used

lastVoice = "none";				# the last voice used in the corpus
generalLastWord = "none";			# the last word used in the corpus

# this is a structure with two members:
# "lastWord" is the last word used by a determined speaker (voice)
# "usedWords" is an array whose members different from zero indicates the words that have been used for a determined speaker (voice)
activityPerVoice = struct("lastWord","none","usedWords", zeros(1,numberOfWords));
voicesActivity(1,1:numberOfVoices) = activityPerVoice;	# this is an array with as many "activityPerVoice" as voices (one structure per voice).

# "turns" is the number of turns distributed for the speakers (voices).
# The turns are distributed in run sections in which every speaker has to participate in random order.
# In every run section, every speaker cannot participate more than once.
# After a run section, the first speaker in the new run section cannot be the same that participated in the last turn of the last run section.
for i=1:turns

	fputs (fid, "\n");

	if (count == numberOfVoices)
		count = 0;
		usedVoices = zeros(1,numberOfVoices);
	endif

	condition = true;
	while ( condition )
		r = randi(numberOfVoices);
		if (numberOfVoices > 1 && usedVoices(1,r) == 0 && !strcmp(lastVoice, voices(r,:)))
			fputs (fid, "<SPEAKER NAME=\"reset\"></SPEAKER>\n");
			temporalString = ["<SPEAKER NAME=\"", voices(r,:), "\">\n"]; 
			fputs (fid, temporalString);
			voicesActivity(1,r) = InsertTextWords(fid, voicesActivity(1,r), vocabulary, wordsPerTurn, generalLastWord);
			generalLastWord = voicesActivity(1,r).lastWord;
			fputs (fid, "</SPEAKER>\n");
			usedVoices(1,r) = 1;
			for i=1:wordsPerTurn
				speakersSequence = [speakersSequence, r];
			endfor
			count++;
			lastVoice = voices(r,:);
			condition = false;
		elseif (numberOfVoices == 1 )
			if (i == 1)
				fputs (fid, "<SPEAKER NAME=\"reset\"></SPEAKER>\n");
				temporalString = ["<SPEAKER NAME=\"", voices(r,:), "\">\n"]; 
				fputs (fid, temporalString);
			endif
			voicesActivity(1,r) = InsertTextWords(fid, voicesActivity(1,r), vocabulary, wordsPerTurn, generalLastWord);
			generalLastWord = voicesActivity(1,r).lastWord;
			if (i == turns)
				fputs (fid, "</SPEAKER>\n");
			endif
			usedVoices(1,r) = 1;
			for i=1:wordsPerTurn
				speakersSequence = [speakersSequence, r];
			endfor
			count++;
			lastVoice = voices(r,:);
			condition = false;
		endif
	endwhile

endfor

fputs (fid, "\n\n");
CloseHeader(fid);

save	wordsSequence.mat wordsSequence -v6
save	speakersSequence.mat speakersSequence -v6

endfunction
