import bs4 as bs
import urllib.request
import numpy as np
import re
from nltk.corpus import stopwords
from collections import Counter
import scipy.io as sio


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




def generateWordSemanticSpaces(snippets):
    numberOfSnippets = len(snippets)
    seq_of_words = ' '.join(snippets).split()
    uniqueWords = list(set(seq_of_words))
    numberOfWords = len(uniqueWords)
    semanticSpace = np.zeros([numberOfSnippets,numberOfWords])
    for j in range(len(snippets)):
        dist = Counter(snippets[j].split())
        for word in dist:
            semanticSpace[j,uniqueWords.index(word)] += dist[word]

    sequence = []
    for word in seq_of_words:
        sequence.append(uniqueWords.index(word))

    return semanticSpace, sequence








snippets = []
length = 7
url = 'https://en.wikipedia.org/wiki/'
with open('./SomeTitles.txt') as fp:
    line = fp.readline()
    cnt = 1
    while line:
        print(line)
        snippets += extractSnippetsFromWebPage(url + line, False)
        line = fp.readline()
        cnt += 1


snippets = list(filter(str.strip, snippets))
semanticSpace, seq_of_words = generateWordSemanticSpaces(snippets)
np.savetxt("snippets.txt", snippets, fmt="%s")
sio.savemat("semanticSpace.mat", {'semanticSpace': semanticSpace, 'seq_of_words': seq_of_words})

