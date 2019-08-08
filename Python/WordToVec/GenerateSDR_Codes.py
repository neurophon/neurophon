import os
import numpy as np
import GenerateVectorsFromWords as w2v


# inputPath =  '/projects/neurophon/TestsData/wiki-split/test/output'
# outputPath =  '/projects/neurophon/TestsData/wiki-split/test/'
# w2v.generateCategorySequence(inputPath, outputPath)

# path =  '/projects/neurophon/TestsData/SDR_Codes/'
# columnsArrayDimensionality = np.array([15, 15])
# populationsArrayDimensionality = np.array([15, 15])
# numberOfCategories = 1000
# w2v.generateSDRCodesFromCatalog(path, columnsArrayDimensionality, populationsArrayDimensionality, numberOfCategories)

path =  '/projects/neurophon/TestsData/wiki-split/test/'
inputPath = '/projects/neurophon/TestsData/wiki-split/test/test.txt'
numberOfLines = 15000
gapSize = 6
w2v.generateSDRCodesFromWords(w2v.getWordsSequence(inputPath, numberOfLines, gapSize), path, path)

# path =  '/projects/neurophon/TestsData/wiki-split/tune/'
# inputPath = '/projects/neurophon/TestsData/wiki-split/tune/tune.txt'
# numberOfLines = 15000
# gapSize = 6
# w2v.generateSDRCodesFromWords(w2v.getWordsSequence(inputPath, numberOfLines, gapSize), path, path)
