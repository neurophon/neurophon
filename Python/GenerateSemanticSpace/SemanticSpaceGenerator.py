import bs4 as bs
import urllib.request
import numpy as np
import re
from nltk.corpus import stopwords
from collections import Counter
import scipy.io as sio
from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

# Get snippets from web url
def extractSnippetsFromWebPage(url, removeStopWords):
    sauce = urllib.request.urlopen(url).read()
    cachedStopWords = stopwords.words("english")
    soup = bs.BeautifulSoup(sauce)

    body = soup.body
    snippets = []
    for paragraph in body.find_all('p'):
        auxiliary = paragraph.text
        auxiliary = re.sub(r"[^a-zA-Z0-9\s]+", '', auxiliary)
        auxiliary = ''.join([i for i in auxiliary if not i.isdigit()])
        auxiliary = re.sub(' +', ' ', auxiliary)
        if removeStopWords:
            auxiliary = ' '.join([word for word in auxiliary.split() if word not in cachedStopWords])

        snippets.append(auxiliary.rstrip().lower())

    return snippets



# Generate Semantic Space from snippets
def generateWordSemanticSpaces(snippets):
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    numberOfSnippets = len(snippets)
    seq_of_words = ' '.join(snippets).split()
    uniqueWords = list(np.unique(np.array(seq_of_words)))
    numberOfWords = len(uniqueWords)
    semanticSpace = np.zeros([numberOfSnippets,numberOfWords])
    for j in range(len(snippets)):
        if (j%size==rank):
            dist = Counter(snippets[j].split())
            for word in dist:
                semanticSpace[j,uniqueWords.index(word)] += dist[word]

    if (size>1):
        sendbuf = semanticSpace
        recvbuf = None
        if rank == 0:
            recvbuf = np.empty([size, semanticSpace.size], dtype='d')
    
        comm.Gather(sendbuf, recvbuf, root=0)

        if (rank==0):
            auxiliary = np.sum(recvbuf,axis=0)
            semanticSpace = np.reshape(auxiliary, (numberOfSnippets, numberOfWords))

    if (rank==0):
        sequence = []
        for word in seq_of_words:
            sequence.append(uniqueWords.index(word))

        sio.savemat("semanticSpace.mat", {'semanticSpace': semanticSpace, 'seq_of_words': sequence})







# Bring text from web urls and get formated snippets
snippets = []
url = 'https://en.wikipedia.org/wiki/'
with open('./SomeTitles.txt') as fp:
    for i, l in enumerate(fp):
        if (i%size==rank):
                snippets += extractSnippetsFromWebPage(url + l, False)

snippets = list(filter(str.strip, snippets))

snippetsSize = len(snippets)

snippetsSize = comm.gather(snippetsSize, root=0)





# If more than one rank, gather all snippets in one list on rank 0
if (size>1):
        if (rank==0):
                for i in range(1, len(snippetsSize)):
                        for j in range(snippetsSize[i]):
                                sizeOfSnippet = comm.recv(source=i)
                                data = bytearray(sizeOfSnippet)
                                comm.Recv(data, source=i)
                                snippets.append(data.decode('utf-8'))
        else:
                snippetsSize = len(snippets)
                for k in range(snippetsSize):
                        data = snippets[k].encode('utf-8')
                        sizeOfSnippet=len(data)
                        comm.send(sizeOfSnippet, dest=0)
                        comm.Send(data, dest=0)





# If more than one rank, broadcast snippets list in rank 0 to the other ranks in the mpi environment
if (size>0):
        if rank == 0:
                snippets = np.array(snippets)
                snippetsSize = snippets.size 
                snippetsType = snippets.dtype 
        else:
                snippetsSize = None
                snippetsType = None

        snippetsSize = comm.bcast(snippetsSize, root=0)
        snippetsType = comm.bcast(snippetsType, root=0)

        if rank != 0:
                snippets = np.empty(snippetsSize, dtype=snippetsType)

        for i in range(snippetsSize):
                if rank == 0:
                        data = snippets[i].encode('utf-8')
                        Size = len(data)
                else:
                        Size = None

                Size = comm.bcast(Size, root=0)

                if rank != 0:
                        data = bytearray(Size)

                comm.Bcast(data, root=0)

                if rank != 0:
                        snippets[i] = data.decode('utf-8')


        snippets = snippets.tolist()

if (rank==0):
        np.savetxt("snippets.txt", snippets, fmt="%s")


generateWordSemanticSpaces(snippets)






