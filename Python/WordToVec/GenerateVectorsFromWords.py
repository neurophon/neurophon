import math
import sys
import gensim
import warnings
from collections import Counter
import numpy as np
from gensim.models.keyedvectors import KeyedVectors
from mpi4py import MPI
import scipy.io as sio



warnings.filterwarnings(action='ignore', category=UserWarning, module='gensim')

path_to_model = '/projects/neurophon/TestsData/GoogleNews-vectors-negative300.bin'

# this line loads the GoogleNews-vectors-negative300.bin.gz model
# This is a set of Pre-trained word and phrase vectors
model = KeyedVectors.load_word2vec_format(path_to_model, binary=True)










# This method returns a histogram from a list of words
# by list of words we mean an array of strings
def wordsHistogram(words):
        return Counter(words)










# This method applies a special treatment to words that are not in the model vocabulary
def absentWordsTreatment(word):
        # some very frequent words are not in the vocabulary so we decided to treat them in a special way
        auxiliary ={
          "to": (model['directed']+model['toward']+model['into']+model['toward']+model['facing']+model['through']+model['via'])/7,
          "and": (model['along']+model['with']+model['furthermore']+model['moreover']+model['also']+model['as']+model['well']+\
                  model['as']+model['including']+model['together']+model['with'])/11,
          "a": model['an']
        }

        if word == '':
                output = np.full((300),0,dtype=float)
        elif word in auxiliary:
                output = auxiliary[word]
        else:
                output = np.random.rand(300)-0.5

        return output










# Generate a sequence of vectors from a sequence of words
# Each vector is returned in its respective row in a numpy array 
# and each vector has 300 real components
def generateVectorsFromWords(words):

        vectors = np.array([])
        absentWords = np.array([])
        First = True
        for word in words:

                if word in model:
                        newWord = model[word]
                else:
                        absentWords = np.append(absentWords,word)
                        newWord = absentWordsTreatment(word)

                if First:
                        vectors = np.hstack((vectors, newWord))
                        First = False
                else:
                        vectors = np.vstack((vectors, newWord))

        



        output = {'vectors': vectors, 'absentWords': absentWords}
        return output










# This method gathers twodimensinal numpy arrays with different number of rows
# in rank 0
def gatherBidimensionalArrays(inputs):
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    if (size>1):
        numberOfColumns = inputs.shape[1]
        inputs = np.reshape(inputs, (inputs.size))

        inputsSize = inputs.size
        inputsSize = comm.gather(inputsSize, root=0)

        inputsType = inputs.dtype
        inputsType = comm.gather(inputsType, root=0)

        if (rank==0):
               for i in range(1, len(inputsSize)):
                   data = np.empty(inputsSize[i], dtype=inputsType[i])
                   comm.Recv(data, source=i)
                   inputs = np.append(inputs, data)
 
               inputs = np.reshape(inputs, (int(inputs.size/numberOfColumns),numberOfColumns))
        else:
               inputsSize = inputs.size
               data = inputs
               comm.Send(data, dest=0)

    return inputs







# If more than one rank, gather all snippets in one list on rank 0
# snippets is a list of strings
def gatherSnippets(snippets):
       comm = MPI.COMM_WORLD
       rank = comm.Get_rank()
       size = comm.Get_size()

       if (size>1):
              snippetsSize = len(snippets)

              snippetsSize = comm.gather(snippetsSize, root=0)

              if (rank==0):
                     for i in range(1, len(snippetsSize)):
                            for j in range(snippetsSize[i]):
                                   sizeOfSnippet = comm.recv(source=i)
                                   data = bytearray(sizeOfSnippet)
                                   comm.Recv(data, source=i)
                                   snippets = np.append(snippets, data.decode('utf-8'))
              else:
                     snippetsSize = len(snippets)
                     for k in range(snippetsSize):
                            data = snippets[k].encode('utf-8')
                            sizeOfSnippet=len(data)
                            comm.send(sizeOfSnippet, dest=0)
                            comm.Send(data, dest=0)

       return snippets








# Gather all the information corresponding to generateVectorsFromWords
# function
def gatherInformation(inputs):
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    vectors = gatherBidimensionalArrays(inputs['vectors'])
    absentWords = gatherSnippets(inputs['absentWords'])

    output = {'vectors': vectors, 'absentWords': absentWords}
    return output





# This is a wrapper that parallelizes the generateVectorsFromWords method
def parallelWrapper(words):
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    firstIndex = rank*int(words.size/size)
    lastIndex = (rank+1)*int(words.size/size)
    if rank == size-1:
            output=generateVectorsFromWords(words[firstIndex:])
    else:
            output=generateVectorsFromWords(words[firstIndex:lastIndex])

    output = gatherInformation(output)

    if rank == 0:
        absentWordsHistograms = wordsHistogram(output['absentWords'])
        sio.savemat("inputs.mat", {'inputs': output['vectors']})
        absentWords = np.array([])
        frequences = np.array([])
        for key,val in absentWordsHistograms.items():
                absentWords = np.append(absentWords,key)
                frequences = np.append(frequences,val)

        sio.savemat("absentWords.mat", {'absentWords': absentWords, 'frequences': frequences})



