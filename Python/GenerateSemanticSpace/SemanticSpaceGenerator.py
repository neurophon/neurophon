import os
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
# snippets is a list of strings
def extractSnippetsFromWebPage(url, removeStopWords):
    try:
           sauce = urllib.request.urlopen(url).read()
    except Exception as error:
       print(error)
       print('Could not bring page: ' + url + '\n')
       raise

    cachedStopWords = stopwords.words("english")
    soup = bs.BeautifulSoup(sauce)

    body = soup.body
    snippets = []
    for paragraph in body.find_all('p'):
        auxiliary = paragraph.text
        auxiliary = re.sub(r"[^a-zA-Z0-9\s]+", '', auxiliary)
        auxiliary = ''.join([i for i in auxiliary if not i.isdigit()])
        auxiliary = re.sub("\n\s*\n*", "\n", auxiliary)
        auxiliary = auxiliary.replace('\n', ' ').replace('\r', '')
        auxiliary = re.sub(' +', ' ', auxiliary)
        if removeStopWords:
            auxiliary = ' '.join([word for word in auxiliary.split() if word not in cachedStopWords])

        snippets.append(auxiliary.rstrip().lower())

    return snippets








# Generate Semantic Space from snippets
# snippets is a list of strings
def generateWordSemanticSpaces(snippets, checkPointAt):
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    numberOfSnippets = len(snippets)
    seq_of_words = ' '.join(snippets).split()
    uniqueWords = list(np.unique(np.array(seq_of_words)))
    numberOfWords = len(uniqueWords)
    semanticSpace = np.zeros([numberOfSnippets,numberOfWords])

    if (os.path.isfile('./firstSnippetAt.mat')):
            contents = sio.loadmat('firstSnippetAt.mat')
            firstSnippetAt = contents['firstSnippetAt'][0][0]
    else:
            firstSnippetAt = 0

    while(firstSnippetAt<numberOfSnippets):
            broken = False
            for j in range(firstSnippetAt, numberOfSnippets):
                followingSnippet = j
                if (j%size==rank and (j%checkPointAt!=0 or j==firstSnippetAt)):
                    dist = Counter(snippets[j].split())
                    for word in dist:
                        semanticSpace[j,uniqueWords.index(word)] += dist[word]
                elif (j%checkPointAt==0 and j!=firstSnippetAt):
                        broken = True
                        break

            

            if (broken):
                    firstSnippetAt = followingSnippet
            else:
                    firstSnippetAt = followingSnippet+1



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

                if (os.path.isfile('./semanticSpace.mat')):
                    contents = sio.loadmat('semanticSpace.mat')
                    semanticSpace += contents['semanticSpace']

                sio.savemat("semanticSpace.mat", {'semanticSpace': semanticSpace, 'seq_of_words': sequence})

                sio.savemat("firstSnippetAt.mat", {'firstSnippetAt': firstSnippetAt})

            semanticSpace = np.zeros([numberOfSnippets,numberOfWords])
            comm.Barrier()








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
                                   snippets.append(data.decode('utf-8'))
              else:
                     snippetsSize = len(snippets)
                     for k in range(snippetsSize):
                            data = snippets[k].encode('utf-8')
                            sizeOfSnippet=len(data)
                            comm.send(sizeOfSnippet, dest=0)
                            comm.Send(data, dest=0)

       return snippets









# If more than one rank, broadcast snippets list in rank 0 to the other ranks in the mpi environment
# snippets is a list of strings
def broadcastSnippets(snippets):
       comm = MPI.COMM_WORLD
       rank = comm.Get_rank()
       size = comm.Get_size()

       if (size>1):
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
       
       return snippets










# Remove the n first lines from a file
def removeLinesFromFile(n, fileName):
        f = open(fileName, 'r')
        lines = f.readlines()
        f.close()

        f = open(fileName,'w')
        f.write(''.join(lines[n:]))
        f.close()









# This function delets the content of a file
def deleteContent(fileName):
    with open(fileName, "w"):
            pass







# This method saves snippets into a file, if the file already exists the
# function append the snippets to the files previous content
# snippets is a list of strings
def saveSnippets(fileName, snippets):
        if (os.path.isfile('./' + fileName)):
                aux = snippets
                snippets = []
                with open('./' + fileName) as fp:
                    line = fp.readline().rstrip()
                    while line:
                            snippets.append(line)
                            line = fp.readline().rstrip()

                snippets = list(filter(str.strip, snippets))
                snippets += aux

        np.savetxt(fileName, snippets, fmt="%s")














# This method loads snippets from a file if the file exists
# snippets is a list of strings
def loadSnippets(fileName):
        if (os.path.isfile('./' + fileName)):
                snippets = []
                with open('./' + fileName) as fp:
                    line = fp.readline().rstrip()
                    while line:
                            snippets.append(line)
                            line = fp.readline().rstrip()

                snippets = list(filter(str.strip, snippets))
                return snippets
        else:
                print('Not file called ' + fileName)
                raise









# Bring text from web urls and get formated snippets
check_point_at = 5;
url = 'https://en.wikipedia.org/wiki/'
while (os.stat('./SomeTitles.txt').st_size != 0):
        snippets = []
        accessed_pages = []
        non_accessed_pages = []
        broken = False
        with open('./SomeTitles.txt') as fp:
            for i, l in enumerate(fp):
                number_of_lines = i
                if (i%size==rank and i<check_point_at):
                      try:
                             snippets += extractSnippetsFromWebPage(url + l, False)
                             accessed_pages.append(l)
                      except:
                             print('Rank ' + str(rank) + ' coul not bring page ' + url + l + '\n')
                             non_accessed_pages.append(l)
                elif (i==check_point_at):
                      broken = True
                      break

        snippets = list(filter(str.strip, snippets))

        snippets = gatherSnippets(snippets)

        if (rank==0):
                saveSnippets('snippets.txt', snippets)

        non_accessed_pages = gatherSnippets(non_accessed_pages)

        if (rank==0):
                saveSnippets('non_accessed_pages.txt', non_accessed_pages)

        if (rank==0):
                if (broken):
                        removeLinesFromFile(number_of_lines, './SomeTitles.txt')
                else:
                        deleteContent('./SomeTitles.txt')

        comm.Barrier()




if (rank==0):
        snippets = loadSnippets('snippets.txt')
else:
        snippets = []

snippets = broadcastSnippets(snippets)

generateWordSemanticSpaces(snippets,300)






