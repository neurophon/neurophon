##################################################################################################################
##                                Author:                Dematties Dario Jesus                                  ##
##                                Contact:        dariodematties@hotmail.com.ar                                 ##
##                                                dariodematties@yahoo.com.ar                                   ##
##                                                dario.dematties@frm.utn.edu.ar                                ##
##                                Project:        Engineering PhD Project                                       ##
##                                Institution:        Universidad de Buenos Aires                               ##
##                                                Facultad de Ingeniería (FIUBA)                                ##
##                                Workplace:        Instituto de Ingeniería                                     ##
##                                                Biomédica FIUBA        &                                      ##
##                                                CCT CONICET Mendoza INCIHUSA                                  ##
##################################################################################################################

# File Name:                Generate_Training_SABLE.m
# Language:                 Python interpreted high-level programming language for general-purpose programming.

# This program generates a corpus in SABLE markup format and save it in a file called "".sable in this folder.
# SABLE is collaboration between Sun, AT&T, Bell Labs, Edinburgh and CMU to devise a standard cross synthesizer standard mark up language.
# The language is XML-based and allows users to add addition controlling commands in text to affect the output.
# An implementation exists in Festival as well as Bell Labs synthesis system. 
# Once a text corpus is generated, this function calls festival to generate the audio (.wav) file from the SABLE file.
# An audio vector is also saved with the sound wave from the audio file in matlab format

import os
import numpy as np
import scipy.io as sio
import subprocess
from scipy.io import wavfile

# Hold the sequence of words and the sequence of voices in the corpus
# Such variables are specified by a sequence of numbers corresponding to the location
# of the words in the vocabulary and the locations of voices
global wordsSequence
global speakersSequence
global path
global corpus

def Generate_SABLE ( voiceFolder, syllableFolder, corpusFolder, nameOfVocabulary, voices, vocabulary, turns, wordsPerTurn ):
        global wordsSequence
        global corpus

        # This is the path were festival is installed
        festival = "/projects/neurophon/MySoft/festival-2.4/festival/examples/text2wave"

        # This is the path were the corpora is saved
        path = "/projects/neurophon/TestsData/NewCorpora/"
        if not os.path.exists(path):
            os.makedirs(path)

        path = path + voiceFolder + "/"
        if not os.path.exists(path):
            os.makedirs(path)

        path = path + syllableFolder + "/"
        if not os.path.exists(path):
            os.makedirs(path)

        path = path + corpusFolder + "/"
        if not os.path.exists(path):
            os.makedirs(path)

        path = path + nameOfVocabulary + "/"
        if not os.path.exists(path):
            os.makedirs(path)


        filename = "Corpus.sable"
        fid = open (path + filename, "w");

        InsertHeader(fid);

        numberOfVoices = voices.size                           # the number of voices
        numberOfWords = vocabulary.size                        # the number of words in the vocabulary

        count = 0;
        usedVoices = np.zeros((numberOfVoices))                # array whose members different from zero indicates the voices that have been used
        lastVoice = "none"                                     # the last voice used in the corpus
        generalLastWord = "none"                               # the last word used in the corpus













        # this is a structure with two members:
        # "lastWord" is the last word used by a determined speaker (voice)
        # "usedWords" is an array whose members different from zero indicates the words that have been used for a determined speaker (voice)
        activityPerVoice = {"lastWord": "none", "usedWords": np.zeros((numberOfWords), dtype=int)}
        voicesActivity = np.full((numberOfVoices), activityPerVoice)        # this is an array with as many "activityPerVoice" as voices (one structure per voice).

        # "turns" is the number of turns distributed for the speakers (voices).
        # The turns are distributed in run sections in which every speaker has to participate in random order.
        # In every run section, every speaker cannot participate more than once.
        # After a run section, the first speaker in the new run section cannot be the same that participated in the last turn of the last run section.
        for i in range(1,turns+1):

                fid.write("\n")

                if count == numberOfVoices:
                        count = 0;
                        usedVoices = np.zeros((numberOfVoices))

                condition = True
                while ( condition ):

                        r = np.random.randint(numberOfVoices)

                        if numberOfVoices > 1 and usedVoices[r] == 0 and not(lastVoice is voices[r]):
                                fid.write("<SPEAKER NAME=\"reset\"></SPEAKER>\n")
                                temporalString = "<SPEAKER NAME=\"" + voices[r] + "\">\n"
                                fid.write(temporalString)
                                voicesActivity[r] = InsertTextWords(fid, voicesActivity[r], vocabulary, wordsPerTurn, generalLastWord)
                                generalLastWord = voicesActivity[r]["lastWord"]
                                fid.write("</SPEAKER>\n")
                                usedVoices[r] = 1
                                for j in range(0,wordsPerTurn):
                                    try:
                                            speakersSequence
                                    except NameError:
                                            speakersSequence = r
                                    else:
                                            speakersSequence = np.append(speakersSequence, r)

                                count+=1
                                lastVoice = voices[r]
                                condition = False
                        elif numberOfVoices == 1:
                                if i == 1:
                                        fid.write("<SPEAKER NAME=\"reset\"></SPEAKER>\n")
                                        temporalString = "<SPEAKER NAME=\"" + voices[r] + "\">\n"
                                        fid.write(temporalString)

                                voicesActivity[r] = InsertTextWords(fid, voicesActivity[r], vocabulary, wordsPerTurn, generalLastWord)
                                generalLastWord = voicesActivity[r]["lastWord"]
                                if i == turns:
                                        fid.write("</SPEAKER>\n")

                                usedVoices[r] = 1
                                for j in range(0,wordsPerTurn):
                                    try:
                                            speakersSequence
                                    except NameError:
                                            speakersSequence = r
                                    else:
                                            speakersSequence = np.append(speakersSequence, r)

                                count+=1
                                lastVoice = voices[r]
                                condition = False


        fid.write("\n\n")
        CloseHeader(fid)

        filename = "CorpusMetadata.mat"
        sio.savemat(path + filename, {'wordsSequence': wordsSequence,\
                                      'speakersSequence': speakersSequence,\
                                      'vocabulary': vocabulary,\
                                      'voices': voices,\
                                      'turns': turns,\
                                      'corpus': corpus,\
                                      'wordsPerTurn': wordsPerTurn})

        del wordsSequence
        del speakersSequence
        del corpus

        filename = "Corpus.wav"
        print(subprocess.check_output([festival, path + "Corpus.sable", "-o", path + filename]))

        Fs, X = wavfile.read(path + filename)

	# This abomination is in order to avoid matlab compression since my library does not manage matlab compression
        Residue = 1*10^(-10);
        X = X + Residue;
        Fs = Fs + Residue;

        # Saves vector X and scalar Fs in 'AudioVector.mat' file
        filename = "AudioVector.mat"
        sio.savemat(path + filename, {'X':X, 'Fs':Fs})
































# This function inserts the header in a SABLE markup format file.
# This file will be interpreted by The Festival Speech Synthesis System.
# SABLE is collaboration between Sun, AT&T, Bell Labs, Edinburgh and CMU to devise a standard cross synthesizer standard mark up language.
# The language is XML-based and allows users to add addition controlling commands in text to affect the output.
# An implementation exists in Festival as well as Bell Labs synthesis system. 
def InsertHeader ( identifier ):

        identifier.write("<?xml version=\"1.0\"?>\n")
        identifier.write("<!DOCTYPE SABLE PUBLIC \"-//SABLE//DTD SABLE speech mark up//EN\" \n");
        identifier.write("      \"Sable.v0_2.dtd\"")
        identifier.write("[]>\n")
        identifier.write("<SABLE>\n\n")




# This function inserts the vocabulary words in a SABLE markup format file.
# This file will be interpreted by The Festival Speech Synthesis System.
# SABLE is collaboration between Sun, AT&T, Bell Labs, Edinburgh and CMU to devise a standard cross synthesizer standard mark up language.
# The language is XML-based and allows users to add addition controlling commands in text to affect the output.
# An implementation exists in Festival as well as Bell Labs synthesis system. 
def InsertTextWords ( identifier, activity, vocabulary, numberOfWords, lastWord ):
        global wordsSequence
        global corpus

        totalNumberOfWords = vocabulary.size

        if totalNumberOfWords < numberOfWords:
                print("The number of words per turn (numberOfWords), must be less than or equal to the total number of words in the vocabulary.\n")
                return

        # Every speaker has to pronounce "numberOfWords" words without using the words already used by him and without repeating the last word pronounced
        # by the last participant speaker
        count = 0
        while (count < numberOfWords):
                remainingWords = np.nonzero(activity["usedWords"] == 0)[0]
                numberOfRemainingWords = remainingWords.size
                if numberOfRemainingWords == 0: 
                        activity["usedWords"] = np.zeros((totalNumberOfWords))

                r = np.random.randint(totalNumberOfWords)
                if count == 0 and activity["usedWords"][r] == 0 and (activity["lastWord"] is not vocabulary[r]) and (lastWord is not vocabulary[r]) or \
                   count > 0 and activity["usedWords"][r] == 0 and (activity["lastWord"] is not vocabulary[r]): 
                        temporalString = " " + vocabulary[r] + "<VOLUME LEVEL=\"-100%\">Cat.</VOLUME>\n"
                        identifier.write(temporalString)
                        activity["lastWord"] = vocabulary[r]
                        activity["usedWords"][r] = 1
                        try:
                                wordsSequence
                        except NameError:
                                wordsSequence = r
                        else:
                                wordsSequence = np.append(wordsSequence, r)

                        try:
                                corpus
                        except NameError:
                                corpus = vocabulary[r]
                        else:
                                corpus = np.append(corpus, vocabulary[r])

                        count+=1
                elif count == 0 and numberOfRemainingWords == 1 and lastWord is vocabulary[remainingWords]:
                        temporalString = " " + vocabulary[remainingWords] + "<VOLUME LEVEL=\"-100%\">Cat.</VOLUME>\n"
                        identifier.write(temporalString)
                        activity["lastWord"] = vocabulary[remainingWords]
                        activity["usedWords"][remainingWords] = 1
                        try:
                                wordsSequence
                        except NameError:
                                wordsSequence = r
                        else:
                                wordsSequence = np.append(wordsSequence, r)

                        try:
                                corpus
                        except NameError:
                                corpus = vocabulary[r]
                        else:
                                corpus = np.append(corpus, vocabulary[r])

                        count+=1

        return activity


# This program closes the header and the SABLE markup file.
# This file will be interpreted by The Festival Speech Synthesis System.
# SABLE is collaboration between Sun, AT&T, Bell Labs, Edinburgh and CMU to devise a standard cross synthesizer standard mark up language.
# The language is XML-based and allows users to add addition controlling commands in text to affect the output.
# An implementation exists in Festival as well as Bell Labs synthesis system. 
def CloseHeader ( identifier ):

        identifier.write("</SABLE>")
        identifier.close()




