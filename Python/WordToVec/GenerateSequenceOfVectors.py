import os
import numpy as np
import GenerateVectorsFromWords as w2v

outputPath = '/projects/neurophon/TestsData/wiki-split/test/'
inputPath = '/projects/neurophon/TestsData/wiki-split/test/test.txt'
numberOfLines = 15000
gapSize = 6
exists = os.path.isfile(outputPath + 'inputs.mat')
if not exists:
    w2v.parallelWrapper(w2v.getWordsSequence(inputPath, numberOfLines, gapSize), outputPath)

print("Finished the first stage")
outputPath = '/projects/neurophon/TestsData/wiki-split/tune/'
inputPath = '/projects/neurophon/TestsData/wiki-split/tune/tune.txt'
numberOfLines = 15000
gapSize = 6
exists = os.path.isfile(outputPath + 'inputs.mat')
if not exists:
    w2v.parallelWrapper(w2v.getWordsSequence(inputPath, numberOfLines, gapSize), outputPath)

print("Finished the second stage")
outputPath = '/projects/neurophon/TestsData/wiki-split/train/'
inputPath = '/projects/neurophon/TestsData/wiki-split/train/train.txt'
numberOfLines = 3000000
gapSize = 6
exists = os.path.isfile(outputPath + 'inputs.mat')
if not exists:
    w2v.parallelWrapper(w2v.getWordsSequence(inputPath, numberOfLines, gapSize), outputPath)

print("Finished the third stage")
# outputPath = '/projects/neurophon/TestsData/wiki-split/minitest/'
# inputPath = '/projects/neurophon/TestsData/wiki-split/minitest/minitest.txt'
# numberOfLines = 15
# gapSize = 6
# w2v.parallelWrapper(w2v.getWordsSequence(inputPath, numberOfLines, gapSize), outputPath)
