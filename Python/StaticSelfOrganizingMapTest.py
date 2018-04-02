import StaticSelfOrganizingMap
import numpy as np
import scipy.io as sio

inputDimensionality = 3
dim = 10
unitsArrayDimensionality = np.array([dim,dim,dim])

print("Loading input")
a = sio.loadmat('../Octave/inputs.mat')
inputs = a['inputs']
print("Input loaded")
numberOfInputs = inputs.shape[0];

print("Creatin SOM object")
SOM = StaticSelfOrganizingMap.StaticSelfOrganizingMap(unitsArrayDimensionality,inputDimensionality)
print("SOM object created")


print("Processing data")
for row in range(numberOfInputs) :
    learningRate = np.float64(0.9*(0.01/0.9))**(np.float64(row)/np.float64(numberOfInputs))
    neighborParameter = np.float64(5*0.01)**(np.float64(row)/np.float64(numberOfInputs))
    unitsWinnerPosition = SOM.getResponse(inputs[row])[1][0];
    SOM.learningRule(learningRate, neighborParameter, unitsWinnerPosition, inputs[row]);

for row in range(numberOfInputs) :
    learningRate = np.float64(0.1*(0.01/0.9))**(np.float64(row)/np.float64(numberOfInputs))
    neighborParameter = np.float64(0.5*0.01)**(np.float64(row)/np.float64(numberOfInputs))
    unitsWinnerPosition = SOM.getResponse(inputs[row])[1][0];
    SOM.learningRule(learningRate, neighborParameter, unitsWinnerPosition, inputs[row]);

learningRate = np.float64(0.1*(0.01/0.9))
neighborParameter = np.float64(0.5*0.01)
for row in range(numberOfInputs) :
    unitsWinnerPosition = SOM.getResponse(inputs[row])[1][0];
    SOM.learningRule(learningRate, neighborParameter, unitsWinnerPosition, inputs[row]);
print("Data processed")

print("Saving object status")
SelfOrganizingMapIdentifier="1"
fileName = "SOM_Status"
status=SOM.saveStaticSelfOrganizingMapStatus(SelfOrganizingMapIdentifier)
sio.savemat('../Octave/' + fileName, {'status': status})
print("Object status saved")

