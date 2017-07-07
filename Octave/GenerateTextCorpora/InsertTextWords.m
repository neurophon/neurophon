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

# File Name:		InsertTextWords.m
# Language:		GNU Octave high-level interpreted language.

# This program inserts the vocabulary words in a SABLE markup format file.
# This file will be interpreted by The Festival Speech Synthesis System.
# SABLE is collaboration between Sun, AT&T, Bell Labs, Edinburgh and CMU to devise a standard cross synthesizer standard mark up language.
# The language is XML-based and allows users to add addition controlling commands in text to affect the output.
# An implementation exists in Festival as well as Bell Labs synthesis system. 

function newActivity = InsertTextWords( identifier, activity, vocabulary, numberOfWords, lastWord )

if (nargin != 5)
	temporalString = ["InsertTextWords function must receive 5 arguments.\n"];
	disp(temporalString);
	return;
endif

totalNumberOfWords = rows (vocabulary);
if ( totalNumberOfWords < numberOfWords )
	temporalString = ["The number of words per turn (numberOfWords), must be less than or equal to the total number of words in the vocabulary.\n"];
	disp(temporalString);
	return;
endif

# Holds the sequence of words in the corpus
# Such variable is specified by a sequence of numbers corresponding to the location
# of the words in the vocabulary
global	wordsSequence;

# Every speaker has to pronounce "numberOfWords" words without using the words already used by him and without repeating the last word pronounced
# by the last participant speaker
count = 0;
while (count < numberOfWords)
	remainingWords = find(~activity.usedWords);
	numberOfRemainingWords = length(remainingWords);
	if ( numberOfRemainingWords == 0 )
		activity.usedWords = zeros(1,totalNumberOfWords);
	endif
	r = randi(totalNumberOfWords);
	if ( count == 0 && activity.usedWords(1,r) == 0 && !strcmp(activity.lastWord,vocabulary(r,:)) && !strcmp(lastWord,vocabulary(r,:)) || ...
	     count  > 0 && activity.usedWords(1,r) == 0 && !strcmp(activity.lastWord,vocabulary(r,:)) )
		temporalString = [" ", vocabulary(r,:), "<VOLUME LEVEL=\"-100%\">Cat.</VOLUME>\n"];
		fputs (identifier, temporalString);
		activity.lastWord = vocabulary(r,:);
		activity.usedWords(1,r) = 1;
		wordsSequence = [wordsSequence, r];
		count++;
	elseif ( count == 0 && numberOfRemainingWords == 1 && strcmp(lastWord,vocabulary(remainingWords,:)) )
		temporalString = [" ", vocabulary(remainingWords,:), "<VOLUME LEVEL=\"-100%\">Cat.</VOLUME>\n"];
		fputs (identifier, temporalString);
		activity.lastWord = vocabulary(remainingWords,:);
		activity.usedWords(1,remainingWords) = 1;
		wordsSequence = [wordsSequence, r];
		count++;
	endif
endwhile

newActivity = activity;

endfunction
