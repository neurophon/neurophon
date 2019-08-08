import os
import math
import sys
import gensim
import warnings
from collections import Counter
import numpy as np
# from gensim.models.keyedvectors import KeyedVectors
from mpi4py import MPI
import scipy.io as sio



warnings.filterwarnings(action='ignore', category=UserWarning, module='gensim')

path_to_model = '/projects/neurophon/TestsData/GoogleNews-vectors-negative300.bin'

# this line loads the GoogleNews-vectors-negative300.bin.gz model
# This is a set of Pre-trained word and phrase vectors
# model = KeyedVectors.load_word2vec_format(path_to_model, binary=True)










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
def parallelWrapper(words, outputPath):
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
        sio.savemat(outputPath + 'inputs.mat', {'inputs': output['vectors']})
        absentWords = np.array([])
        frequences = np.array([])
        for key,val in absentWordsHistograms.items():
                absentWords = np.append(absentWords,key)
                frequences = np.append(frequences,val)

        sio.savemat(outputPath + 'absentWords.mat', {'absentWords': absentWords, 'frequences': frequences})







# This method get a sequence of words from text lines and separates
# the lines using a gap with empty strings
# The sequence of words is returned in a numpy array
def getWordsSequence(filePath, numberOfLines, gapSize):
    output = np.array([])
    gap = np.full((gapSize),'')
    with open(filePath) as fp:
        for i, l in enumerate(fp):
            output = np.append(output, gap)
            output = np.append(output, l.split())

            if (i+1>=numberOfLines):
                break

    return output











# Gets the part of speech sequence from file
def getPartOfSpeechSequence(filePath):
    output = np.array([])
    with open(filePath) as fp:
        for i, l in enumerate(fp):
            output = np.append(output, l.split())

    return output











# Generates a catalog with all the parts of speech in
# partOfSpeechSequence
def generateCatalog(partOfSpeechSequence):
    aux = wordsHistogram(partOfSpeechSequence)
    part_of_speech = np.array([])
    for key,val in aux.items():
        part_of_speech = np.append(part_of_speech,key)

    sio.savemat("catalog.mat", {'part_of_speech': part_of_speech})









# Generates a category sequence from a file containing
# a part of speech sequence corresponding to a certain corpus
def generateCategorySequence(inputFilePath, outputFilePath):
    sequence = getPartOfSpeechSequence(inputFilePath)
    # catalog = np.array(['V_VBP', 'ADV_DT', 'P_VBN', 'ADV_VBZ', 'ADV_JJ', 'ADV_RBS', 'ADV_NNS', 'ADV_RB', 'ADJ_NN',\
                          # 'N_VBZ', 'CONJ_CC', 'P_CC', 'D_PRP$', 'N_WDT', 'PRT_RP', 'N_VBP', 'N_LS', 'SC_RB', 'ADJ_NNS',\
                          # 'P_DT', 'V_POS', 'V_VBN', 'PRT_IN', 'ADV_CD', 'P_RB', 'D_VBZ', 'ADJ_DT', 'N_NN', 'ADJ_JJR', 'ADV_NN',\
                          # 'D_WP$', 'ADJ_RBR', 'ADJ_CD', 'N_JJR', 'N_NNS', 'V_VBD', 'ADJ_VB', 'N_VBN', 'C_TO', 'N_WP', 'SC_WRB',\
                          # 'C_WDT', 'ADV_JJS', 'ADV_IN', 'V_VB', 'V_NN', 'N_FW', 'N_VBG', 'D_DT', 'N_RB', 'N_RBS', 'ADV_RBR', 'ADV_VBD',\
                          # 'N_PRP', 'N_SYM', 'ADJ_FW', 'N_VB', 'N_VBD', 'V_VBZ', 'ADV_VB', 'N_PRP$', 'ADV_FW', 'ADV_JJR', 'N_IN', 'P_FW',\
                          # 'P_NN', 'SC_IN', 'P_JJ', 'N_JJ', 'N_JJS', 'ADJ_RB', 'CONJ_RB', 'ADV_LS', 'V_VBG', 'ADV_CC', 'N_RBR', 'P_VB',\
                          # 'CONJ_IN', 'N_PDT', 'P_VBG', 'N_EX', 'V_NNS', 'ADJ_IN', 'P_RP', 'ADV_WRB', 'V_MD', 'N_DT', 'C_IN', 'ADJ_JJ',\
                          # 'ADJ_JJS', 'ADJ_VBD', 'V_JJ', 'V_IN', 'ADJ_PDT', 'ADV_RP', 'ADJ_VBG', 'V_PRP$', 'C_DT', 'PRT_RB', 'P_TO', 'N_CD',\
                          # 'ADJ_RP', 'P_POS', 'P_IN', 'ADJ_VBN', 'D_WDT', 'N_NNP', 'N_NNPS', 'V_NNP', 'PRT_JJ','ADJ_NNP','ADV_NNP','ADJ_RBS'])

    catalog = np.array(['CONT', 'FUNC', 'VERB'])

    output = np.array([])
    for part_of_speech in sequence:
        index = np.where(catalog == part_of_speech)
        if len(index[0]) == 0:
            raise Exception('The part of speech {} is not in the catalog. You should add it to the code in generateCategorySequence function'\
                            .format(part_of_speech))
        elif len(index[0]) > 1:
            raise Exception('More than one part of speech {} in catalog. You should delete duplicated elements in the code in generateCategorySequence function'\
                            .format(part_of_speech))
        else:
            output = np.append(output, index)

    sio.savemat(outputFilePath + 'categorySequence.mat', {'categories': output})








# Generates a random SDR
def generateRandomSDR(columnsArrayDimensionality, populationsArrayDimensionality, sparsity):
        if sparsity > 1 or sparsity < 0:
            raise Exception('Wrong sparsity value; sparsity must be a real number between 0 and 1, function received {}.'.format(sparsity))
        numberOfColumns=int(columnsArrayDimensionality.prod())
        numberOfUnits=int(populationsArrayDimensionality.prod())
        SDR=np.zeros((numberOfColumns,1), dtype=object)
        Size=int((1.0-sparsity)*numberOfUnits)
        for column in range(numberOfColumns):
                if Size is 0:
                        auxiliary=np.array([np.repeat(-1,1)],dtype=float)
                else:
                        auxiliary=np.array([np.random.randint(numberOfUnits, size=Size)],dtype=float)

                columnSDR=np.array([0],dtype=object)
                columnSDR[0]=auxiliary
                SDR[column]=columnSDR
        
        output = np.array([0],dtype=object)
        output[0]=SDR
        return output







# This code generates a SDR code for each category in a catalog
def generateSDRCodesFromCatalog(outputFilePath, columnsArrayDimensionality, populationsArrayDimensionality, numberOfCategories):

        codes=np.zeros((numberOfCategories+1,1), dtype=object)
        for category in range(numberOfCategories):
                codes[category] = generateRandomSDR(columnsArrayDimensionality, populationsArrayDimensionality, 0.99)
        
        codes[numberOfCategories] = generateRandomSDR(columnsArrayDimensionality, populationsArrayDimensionality, 1.0)

        sio.savemat(outputFilePath + 'SDR_Codes.mat', {'SDR_Codes': codes,\
                                                       'columnsArrayDimensionality': columnsArrayDimensionality,\
                                                       'populationsArrayDimensionality': populationsArrayDimensionality})












# This method generates a sequence of SDR_Codes (using a catalog) from the corresponding sequence of categories in the corpus
def generateSDRCodesFromWords(words, inputFilePath, outputFilePath):

        sequence_of_tags=np.zeros((words.size), dtype=int)
        codes = sio.loadmat(inputFilePath + 'SDR_Codes.mat')
        category = sio.loadmat(inputFilePath + 'categorySequence.mat')
        
        auxiliary = category['categories'][0]

        counter = 0
        for word in words:
                if word.rstrip() is '':
                        sequence_of_tags[counter] = float(1000)
                else:
                        sequence_of_tags[counter] = float(auxiliary[0])
                        auxiliary = np.delete(auxiliary, 0)

                counter += 1

        if auxiliary.size != 0:
                raise Exception('generateSDRCodesFromWords inconsistence: category.size must be 0 at the end of the run but it is actualy {}.'\
                                .format(auxiliary.size))


        sio.savemat(outputFilePath + 'supervision.mat', {'SDR_Codes': codes['SDR_Codes'],\
                                                         'columnsArrayDimensionality': codes['columnsArrayDimensionality'],\
                                                         'populationsArrayDimensionality': codes['populationsArrayDimensionality'],\
							 'sequence_of_tags': sequence_of_tags})

