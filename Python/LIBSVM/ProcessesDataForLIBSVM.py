# File Name:       ProcessesDataForLIBSVM.py
# Language:        Python 3 high-level interpreted language.

# The soft in this file contains methods that process the input to the model and outputs from every layer of the same
# in order to make it readable from the Support Vector Machine libsvm software



import os
import sys
import numpy as np
import scipy.io as sio
sys.path.append('/home/demattie/libsvm/python')
from svmutil import *
from scipy.sparse import csr_matrix
import threading as th












def getArrayFromSDRs(SDRs, columnsArrayDimensionality, populationsArrayDimensionality, numberOfThreads, threadID, lock):

        global auxiliary

        numberOfSteps = int(SDRs.shape[0])
        numberOfColumns = int(np.prod(columnsArrayDimensionality))
        numberOfUnits = int(np.prod(populationsArrayDimensionality))

        output = np.zeros((numberOfSteps, numberOfColumns*numberOfUnits))

        for step in range(numberOfSteps):
                if step%numberOfThreads==threadID:
                        for column in range(numberOfColumns):
                                if not SDRs[step][0][column][0].shape[1]==1 or not SDRs[step][0][column][0][0][0]<0:
                                        inds = numberOfColumns*column+SDRs[step][0][column][0][0].astype(int)
                                        np.put(output[step,:],inds, 1.0)

        lock.acquire()
        auxiliary += output
        lock.release()











def data_to_libsvm(modelData):

        number_of_threads = 12

        outputs = {}
        # Get the indices whose rows are not null
        indices = np.where(~modelData['inputs'].any(axis=1))[0]
        outputs['inputs'] = np.delete(modelData['inputs'], indices, axis=0)

        # This is the global variable to be modified by all threads
        global auxiliary
        auxiliary = np.zeros((int(modelData['layerOutput_EL'].shape[0]),\
                              int(np.prod(modelData['columnsArrayDimensionality_EL']))*\
                              int(np.prod(modelData['populationsArrayDimensionality_EL']))))

        # creating a lock
        lock = th.Lock()

        # creating threads
        threads = []
        for thread in range(number_of_threads):
                 threads.append(th.Thread(target=getArrayFromSDRs, args=\
                                (modelData['layerOutput_EL'],\
                                 modelData['columnsArrayDimensionality_EL'],\
                                 modelData['populationsArrayDimensionality_EL'],\
                                 number_of_threads,\
                                 thread,\
                                 lock)))

        # starting threads
        for thread in threads:
                thread.start()
        
        # wait until threads are completely executed
        for thread in threads:
                thread.join()

        outputs['encoderLayer'] = np.delete(auxiliary, indices, axis=0)

        counter = 0
        while 'layerOutput_RL_' + str(counter) in modelData:

                # This is the global variable to be modified by all threads
                auxiliary = np.zeros((int(modelData['layerOutput_RL_' + str(counter)].shape[0]),\
                                      int(np.prod(modelData['columnsArrayDimensionality_RL_' + str(counter)]))*\
                                      int(np.prod(modelData['populationsArrayDimensionality_RL_' + str(counter)]))))

                # creating a lock
                lock = th.Lock()

                # creating threads
                threads = []
                for thread in range(number_of_threads):
                         threads.append(th.Thread(target=getArrayFromSDRs, args=\
                                        (modelData['layerOutput_RL_' + str(counter)],\
                                         modelData['columnsArrayDimensionality_RL_' + str(counter)],\
                                         modelData['populationsArrayDimensionality_RL_' + str(counter)],\
                                         number_of_threads,\
                                         thread,\
                                         lock)))

                # starting threads
                for thread in threads:
                        thread.start()
                
                # wait until threads are completely executed
                for thread in threads:
                        thread.join()

                outputs['regularLayer_' + str(counter)] = np.delete(auxiliary, indices, axis=0)
                counter += 1

        outputs['categorySequence'] = modelData['categorySequence']



        # Checks data integrity
        if outputs['inputs'].shape[0] != outputs['categorySequence'].shape[1]:
                raise Exception('in data_to_libsvm, different number of steps in inputs and in categorySequence.')

        if outputs['inputs'].shape[0] != outputs['encoderLayer'].shape[0]:
                raise Exception('in data_to_libsvm, different number of steps in inputs and in encoderLayer.')

        counter = 0
        while 'regularLayer_' + str(counter) in outputs:
                if outputs['inputs'].shape[0] != outputs['regularLayer_' + str(counter)].shape[0]:
                        raise Exception('in data_to_libsvm, different number of steps in inputs and in regularLayer_{}.'\
                                        .format(counter))

                counter += 1



        return outputs



def scale_modelData(modelData, arguments):

        output = {}

        if type(arguments) is np.ndarray:
                if arguments.size != 2:
                        raise Exception('in scale_modelData, numpy array arguments size must be 2.')

                parameters = {}
                parameters['inputs'] = csr_find_scale_param(csr_matrix(modelData['inputs']), lower=arguments[0], upper=arguments[1])
                output['inputs'] = csr_scale(csr_matrix(modelData['inputs']), parameters['inputs'])

                parameters['encoderLayer'] = csr_find_scale_param(csr_matrix(modelData['encoderLayer']), lower=arguments[0], upper=arguments[1])
                output['encoderLayer'] = csr_scale(csr_matrix(modelData['encoderLayer']), parameters['encoderLayer'])

                counter = 0
                while 'regularLayer_' + str(counter) in modelData:
                        parameters['regularLayer_' + str(counter)] = csr_find_scale_param(csr_matrix(modelData['regularLayer_' + str(counter)]),\
                                                                                          lower=arguments[0], upper=arguments[1])
                        output['regularLayer_' + str(counter)] = csr_scale(csr_matrix(modelData['regularLayer_' + str(counter)]),\
                                                                           parameters['regularLayer_' + str(counter)])

                        counter += 1

                output['categorySequence'] = modelData['categorySequence']
                
                return output, parameters

        elif type(arguments) is str:
                parameters = load_parameters(arguments)

                output['inputs'] = csr_scale(csr_matrix(modelData['inputs']), parameters['inputs'])

                output['encoderLayer'] = csr_scale(csr_matrix(modelData['encoderLayer']), parameters['encoderLayer'])

                counter = 0
                while 'regularLayer_' + str(counter) in modelData:
                        output['regularLayer_' + str(counter)] = csr_scale(csr_matrix(modelData['regularLayer_' + str(counter)]),\
                                                                                      parameters['regularLayer_' + str(counter)])
                        counter += 1
                
                output['categorySequence'] = modelData['categorySequence']

                return output

        else:
                raise Exception('in scale_modelData, arguments must be a string or a numpy array with two elements.')





def save_parameters(parameters, filePath):

        sio.savemat(filePath + 'inputs_param.mat', {'offset': parameters['inputs']['offset'], 'coef': parameters['inputs']['coef']})
        sio.savemat(filePath + 'encoderLayer_param.mat', {'offset': parameters['encoderLayer']['offset'], 'coef': parameters['encoderLayer']['coef']})
                
        counter = 0
        while 'regularLayer_' + str(counter) in parameters:
                sio.savemat(filePath + 'regularLayer_' + str(counter) + '_param.mat',\
                           {'offset': parameters['regularLayer_' + str(counter)]['offset'],\
                            'coef': parameters['regularLayer_' + str(counter)]['coef']})

                counter += 1
        










def load_parameters(filePath):
        parameters = {}
        parameters['inputs'] = sio.loadmat(filePath + 'inputs_param.mat')
        parameters['encoderLayer'] = sio.loadmat(filePath + 'encoderLayer_param.mat')

        counter = 0
        while os.path.isfile('regularLayer_' + str(counter) + '_param.mat'):
                parameters['regularLayer_' + str(counter)] = sio.loadmat(filePath + 'regularLayer_' + str(counter) + '_param.mat')
                counter += 1
        
        return parameters
        







def save_modelData(modelData, filePath):
        # Open a file for the inputs
        fo = open(filePath + 'inputs.txt', "w")
        for category, row in zip(modelData['categorySequence'][0], modelData['inputs']):
                fo.write(str(int(category)) + ' ')
                for index in range(row.size):
                        if row[index] != 0:
                                fo.write(str(index+1) + ':' + str(row[index]) + ' ')

                fo.write('\n')

        # Close opend file for the inputs
        fo.close()


        # Open a file for the encoderLayer
        fo = open(filePath + 'encoderLayer.txt', "w")
        for category, row in zip(modelData['categorySequence'][0], modelData['encoderLayer']):
                fo.write(str(int(category)) + ' ')
                for index in range(row.size):
                        if row[index] != 0:
                                fo.write(str(index+1) + ':' + str(row[index]) + ' ')

                fo.write('\n')

        # Close opend file for the encoderLayer
        fo.close()



        counter = 0
        while 'regularLayer_' + str(counter) in modelData:
                # Open a file for the encoderLayer
                fo = open(filePath + 'regularLayer_' + str(counter) + '.txt', "w")
                for category, row in zip(modelData['categorySequence'][0], modelData['regularLayer_' + str(counter)]):
                        fo.write(str(int(category)) + ' ')
                        for index in range(row.size):
                                if row[index] != 0:
                                        fo.write(str(index+1) + ':' + str(row[index]) + ' ')

                        fo.write('\n')

                # Close opend file for the encoderLayer
                fo.close()

                counter += 1
        








def load_modelData(filePath):

        modelData = {}
        # Loads the inputs that received the model
        if os.path.isfile(filePath + 'inputs.mat'):
                contents = sio.loadmat(filePath + 'inputs.mat')
                modelData['inputs'] = contents['inputs']
        else:
                raise Exception('There is no inputs.mat file in the path {}.'\
                                .format(filePath))

        # Loads the output from the encoder layer of the model
        if os.path.isfile(filePath + 'EncoderLayerOutput.mat'):
                contents = sio.loadmat(filePath + 'EncoderLayerOutput.mat')
                modelData['columnsArrayDimensionality_EL'] = contents['columnsArrayDimensionality']
                modelData['populationsArrayDimensionality_EL'] = contents['populationsArrayDimensionality']
                modelData['layerOutput_EL'] = contents['encoderLayerOutput']
        else:
                raise Exception('There is no EncoderLayerOutput.mat file in the path {}.'\
                                .format(filePath))

        # Loads the output form the Regular Layers of the model
        counter = 0
        exists = os.path.isfile(filePath + 'RegularLayerOutput_' + str(counter) + '.mat')
        while exists:
                contents = sio.loadmat(filePath + 'RegularLayerOutput_' + str(counter) + '.mat')
                modelData['columnsArrayDimensionality_RL_' + str(counter)] = contents['columnsArrayDimensionality']
                modelData['populationsArrayDimensionality_RL_' + str(counter)] = contents['populationsArrayDimensionality']
                modelData['layerOutput_RL_' + str(counter)] = contents['regularLayerOutput']
                counter+=1
                exists = os.path.isfile(filePath + 'RegularLayerOutput_' + str(counter) + '.mat')

        # Checks if all the arrays have the same number of time steps
        if modelData['inputs'].shape[0] != modelData['layerOutput_EL'].shape[0]:
                raise Exception('Input data inconsistence in script load_modelData: different time steps'\
                                'input and outputs from the encoder layer model have to have the same time steps')

        for count in range(counter):
                if modelData['inputs'].shape[0] != modelData['layerOutput_RL_' + str(count)].shape[0]:
                        raise Exception('Input data inconsistence in script load_modelData: different time steps'\
                                        'input and outputs from the regular layer model number {} have to have the same time steps'\
                                        .format(count))

        # Now loads the sequence of tags of the corpus
        if os.path.isfile(filePath + 'categorySequence.mat'):
                contents = sio.loadmat(filePath + 'categorySequence.mat')
                modelData['categorySequence'] = contents['categories']
        else:
                raise Exception('There is no categorySequence.mat file in the path {}.'\
                                .format(filePath))

        return modelData

















def processes_data_for_train_libsvm(filePath):
        # limits = np.array([0.0,1.0])
        output = data_to_libsvm(load_modelData(filePath))
        save_modelData(output, filePath)









def processes_data_for_test_libsvm(filePath):
        output = data_to_libsvm(load_modelData(filePath))
        save_modelData(output, filePath)
