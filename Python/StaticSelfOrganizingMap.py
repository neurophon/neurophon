import numpy as np
import Constants as const
from   scipy   import  spatial as sp
import math

class   StaticSelfOrganizingMap:
    def __init__(self,  unitsArrayDimensionality,
                        inputDimensionality,
                        weightLimits=None):

        self.__updateStep = 0
        self.__inputDimensionality = inputDimensionality
        self._unitsDimensionality = np.prod(np.array(unitsArrayDimensionality)) 
        self._unitsArrayDimensionality = unitsArrayDimensionality

        self.validateObject()

        if const.ENABLE_RANDOM_BEHAVIOUR :
            if weightLimits == None :
                self.__weights = np.random.rand(self._unitsDimensionality,inputDimensionality)
            else :
                self.__weights = np.random.uniform(weightLimits[0],weightLimits[1],(self._unitsDimensionality,inputDimensionality))
        else :
            self.__weights = np.tile(0.5,(self._unitsDimensionality,inputDimensionality))






    def validateObject(self):
        if (type(self.__inputDimensionality) is not int) or \
           (type(self._unitsDimensionality) is not np.int64) or \
           (type(self._unitsArrayDimensionality) is not np.ndarray) :
            print("StaticSelfOrganizingMap object inconsistence: __inputDimensionality is not int or "\
                                                                "_unitsDimensionality is not int or "\
                                                                "_unitsArrayDimensionality is not numpy array")
            raise ValueError('StaticSelfOrganizingMap error')
            
        if (len(self._unitsArrayDimensionality.shape) is not 1) :
            print("StaticSelfOrganizingMap object inconsistence: len(unitsArrayDimensionality.shape) must be one")
            raise ValueError('StaticSelfOrganizingMap error in member function validateObject')
    
        for dim in range(0,self._unitsArrayDimensionality.size): 
            if  type(self._unitsArrayDimensionality[dim]) is not np.int64 :
                print("StaticSelfOrganizingMap object inconsistence: unitsArrayDimensionality in its dim ", dim)
                print("has a wrong value: ", self._unitsArrayDimensionality[dim])
                raise ValueError('StaticSelfOrganizingMap error in member function validateObject')

        if (self.__inputDimensionality == 0) or (self._unitsDimensionality == 0) or (self._unitsArrayDimensionality.size == 0) :
            print("StaticSelfOrganizingMap object inconsistence: inputDimensionality = ", self.__inputDimensionality)
            print("StaticSelfOrganizingMap object inconsistence: unitsArrayDimensionality.size = ", self._unitsArrayDimensionality.size)
            print("StaticSelfOrganizingMap object inconsistence: unitsDimensionality = ", self._unitsDimensionality)
            raise ValueError('StaticSelfOrganizingMap error in member function validateObject')

        for dim in range(0,self._unitsArrayDimensionality.size): 
            if  self._unitsArrayDimensionality[dim] <= 0 :
                print("StaticSelfOrganizingMap object inconsistence: unitsArrayDimensionality in its dim ", dim)
                print("has a wrong value: ", self._unitsArrayDimensionality[dim])
                raise ValueError('StaticSelfOrganizingMap error in member function validateObject')

        if self.__inputDimensionality < self._unitsArrayDimensionality.size :
            print("StaticSelfOrganizingMap object inconsistence: inputDimensionality = {} \
                   must be greater than or -at least- equal to unitsArrayDimensionality.size = {}".format(self.__inputDimensionality,
                                                                                                          self._unitsArrayDimensionality.size))
            raise ValueError('StaticSelfOrganizingMap error in member function validateObject')

        product = np.prod(np.array(self._unitsArrayDimensionality))

        if self._unitsDimensionality != product :
                print("StaticSelfOrganizingMap object inconsistence: unitsDimensionality = {} \
                       must be equal to the product of the elements in unitsArrayDimensionality which is = {}".format(self._unitsDimensionality,
                                                                                                                      product))
                raise ValueError('StaticSelfOrganizingMap error in member function validateObject')








    def learningRule(self,learningRate,neighborParameter,unitsWinnerPosition,inputVector):
        if (type(learningRate) is not np.float64) or \
           (type(neighborParameter) is not np.float64) or \
           (type(unitsWinnerPosition) is not np.int64) or \
           (type(inputVector) is not np.ndarray) :
            print("StaticSelfOrganizingMap object inconsistence: learningRate is not np.float64 or "\
                                                                "neighborParameter is not np.float64 or "\
                                                                "unitsWinnerPosition is not np.float64 or "\
                                                                "inputVector is not numpy array")
            raise ValueError('StaticSelfOrganizingMap error in member function learningRule')

        if not (inputVector.size == self.__inputDimensionality) :
            print("StaticSelfOrganizingMap object inconsistence: inputVector.size != __inputDimensionality")
            raise ValueError('StaticSelfOrganizingMap error in member function learningRule')

        for inputElement in inputVector : 
            if  type(inputElement) is not np.float64 :
                print("StaticSelfOrganizingMap object inconsistence: inputElement nside of inputVector is not np.float64")
                raise ValueError('StaticSelfOrganizingMap error in member function learningRule')

        if not (unitsWinnerPosition < self._unitsDimensionality) :
            print("StaticSelfOrganizingMap object inconsistence: unitsWinnerPosition >= _unitsDimensionality")
            raise ValueError('StaticSelfOrganizingMap error in member function learningRule')

        lateralInteractionFunction = 'gaussian'
        neighborhoodValue = self.learningNeighborhood(neighborParameter, unitsWinnerPosition, lateralInteractionFunction)
        self.__weights+=learningRate*np.transpose(np.tile(neighborhoodValue,(self.__inputDimensionality,1)))*\
                                                 (np.tile(inputVector,(self._unitsDimensionality,1))-self.__weights)








    def learningNeighborhood(self, widthParameter, winnerPosition, string) :
        if not (winnerPosition < self._unitsDimensionality) :
            print("StaticSelfOrganizingMap object inconsistence: !(winnerPosition < _unitsDimensionality) is true")
            raise ValueError('StaticSelfOrganizingMap error in member function learningNeighborhood')

        a = np.tile(np.array(np.unravel_index(winnerPosition, self._unitsArrayDimensionality)),(self._unitsDimensionality,1))
        b = np.transpose(np.array(np.unravel_index(range(0,self._unitsDimensionality), self._unitsArrayDimensionality)))
        distances = np.float64(np.sum(np.abs(a-b),axis=1))

        if string == "gaussian" :
            return  np.exp(-(distances**2)/(2*widthParameter))
        else :
            print("StaticSelfOrganizingMap object inconsistence: wrong string")
            raise ValueError('StaticSelfOrganizingMap error in member function learningNeighborhood')






    def getResponse(self, inputVector) :
        if not (inputVector.size == self.__inputDimensionality) :
                print("StaticSelfOrganizingMap object inconsistence: inputVector.size != __inputDimensionality")
                raise ValueError('StaticSelfOrganizingMap error in member function getResponse')
    
        distances = np.sum(np.abs(self.__weights - np.tile(inputVector,(self._unitsDimensionality,1)))**2,axis=-1)**(1/2)
        return distances, np.argsort(distances)






    def saveStaticSelfOrganizingMapStatus(self, SelfOrganizingMapIdentifier) :
        if not (type(SelfOrganizingMapIdentifier) == str) :
                print("StaticSelfOrganizingMap object inconsistence: SelfOrganizingMapIdentifier is not a string")
                raise ValueError('StaticSelfOrganizingMap error in member function saveStaticSelfOrganizingMapStatus')
    
        string = "SSOM"
        string += SelfOrganizingMapIdentifier

        status = {string + '_inputDimensionality': self.__inputDimensionality,\
                  string + '_unitsDimensionality': self._unitsDimensionality,\
                  string + '_unitsArrayDimensionality': self._unitsArrayDimensionality,\
                  string + '_weights': self.__weights,
                  string + '_updateStep': self.__updateStep}

        return status
