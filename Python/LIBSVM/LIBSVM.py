# File Name:       LIBSVM.py
# Language:        Python 3 high-level interpreted language.

# The soft in this file contains methods that use libsvm to train and test classification problems loaded from files.
# The libsvm library contains the necessary methods and classes to apply Support Vector Machine (SVM) algorithms



import os
import sys
import numpy as np
import scipy.io as sio
sys.path.append('/home/demattie/libsvm/python')
from svmutil import *
from scipy.sparse import csr_matrix
import threading as th

def libsvm_cross_validation(y, x, kernel_type, numberOfThreads, threadID, lock):

        global auxiliary_model
        global auxiliary_cost
        global auxiliary_gamma

        best_model = 0

        # train libsvm for inputs_libsvm
        if kernel_type is 0:

                options = '-t 0 -v 5 -q'
                # coarse training
                count = 0
                for c in range(-5,15):
                        if count%numberOfThreads==threadID:
                                libsvm_options = options + ' -c ' + str(2**c)
                                m = svm_train(y, x, libsvm_options)
                                if m > best_model:
                                        best_model = m
                                        best_cost = c

                        count += 1

                # fine-grained training
                count = 0
                costs = np.arange(best_cost-1, best_cost+1, 0.1)
                for c in costs:
                        if count%numberOfThreads==threadID:
                                libsvm_options = options + ' -c ' + str(2**c)
                                m = svm_train(y, x, libsvm_options)
                                if m > best_model:
                                        best_model = m
                                        best_cost = c

                        count += 1

                lock.acquire()
                auxiliary_model = np.append(auxiliary_model, best_model)
                auxiliary_cost = np.append(auxiliary_cost, best_cost)
                lock.release()

        elif kernel_type is 2:

                options = '-t 2 -v 5 -q'
                # coarse training
                count = 0
                for c in range(-5,15):
                        if count%numberOfThreads==threadID:
                                for g in range(-15,3):
                                        libsvm_options = options + ' -c ' + str(2**c) + ' -g ' + str(2**g)
                                        m = svm_train(y, x, libsvm_options)
                                        if m > best_model:
                                                best_model = m
                                                best_cost = c
                                                best_gamma = g

                        count += 1

                # fine-grained training
                count = 0
                costs = np.arange(best_cost-1, best_cost+1, 0.1)
                gammas = np.arange(best_gamma-1, best_gamma+1, 0.1)
                for c in costs:
                        if count%numberOfThreads==threadID:
                                for g in gammas:
                                        libsvm_options = options + ' -c ' + str(2**c) + ' -g ' + str(2**g)
                                        m = svm_train(y, x, libsvm_options)
                                        if m > best_model:
                                                best_model = m
                                                best_cost = c
                                                best_gamma = g

                        count += 1

                lock.acquire()
                auxiliary_model = np.append(auxiliary_model, best_model)
                auxiliary_cost = np.append(auxiliary_cost, best_cost)
                auxiliary_gamma = np.append(auxiliary_gamma, best_gamma)
                lock.release()

        else:
                raise Exception('in libsvm_cross_validation, kernel_type must be 0 or 2. The method received {}.'\
                                        .format(kernel_type))






def libsvm_train(filePath, kernel=0):

        number_of_threads = 12

        # These are the global variables to be modified by all threads
        global auxiliary_model
        global auxiliary_cost
        global auxiliary_gamma

        auxiliary_model = np.array([])
        auxiliary_cost = np.array([])
        auxiliary_gamma = np.array([])

        # creating a lock
        lock = th.Lock()

        if kernel is 0:
                if not os.path.isfile(filePath + 'inputs.model'):
                        if os.path.isfile(filePath + 'inputs.txt'):
                                labels, features = svm_read_problem(filePath + 'inputs.txt')
                                # creating threads
                                threads = []
                                for thread in range(number_of_threads):
                                         threads.append(th.Thread(target=libsvm_cross_validation, args=\
                                                        (labels, features, kernel,\
                                                         number_of_threads,\
                                                         thread,\
                                                         lock)))

                                # starting threads
                                for thread in threads:
                                        thread.start()
                                
                                # wait until threads are completely executed
                                for thread in threads:
                                        thread.join()

                                index = np.argmax(auxiliary_model)
                                model = auxiliary_model[index]
                                cost = auxiliary_cost[index]

                                auxiliary_model = np.array([])
                                auxiliary_cost = np.array([])

                                print('The best model for inputs gives Accuracy = {}'.format(model))
                                training_options = '-b 1 -t 0 -q -c ' + str(2**cost)
                                # training_options = '-t 0 -q -c ' + str(2**cost)
                                model = svm_train(labels, features, training_options)

                                svm_save_model(filePath + 'inputs.model', model)
                        else:
                                raise Exception('in libsvm_train, there is no file called inputs.txt in the path provided.')

                if not os.path.isfile(filePath + 'encoderLayer.model'):
                        if os.path.isfile(filePath + 'encoderLayer.txt'):
                                labels, features = svm_read_problem(filePath + 'encoderLayer.txt')
                                # creating threads
                                threads = []
                                for thread in range(number_of_threads):
                                         threads.append(th.Thread(target=libsvm_cross_validation, args=\
                                                        (labels, features, kernel,\
                                                         number_of_threads,\
                                                         thread,\
                                                         lock)))

                                # starting threads
                                for thread in threads:
                                        thread.start()
                                
                                # wait until threads are completely executed
                                for thread in threads:
                                        thread.join()

                                index = np.argmax(auxiliary_model)
                                model = auxiliary_model[index]
                                cost = auxiliary_cost[index]

                                auxiliary_model = np.array([])
                                auxiliary_cost = np.array([])

                                print('The best model for encoderLayer gives Accuracy = {}'.format(model))
                                training_options = '-b 1 -t 0 -q -c ' + str(2**cost)
                                # training_options = '-t 0 -q -c ' + str(2**cost)
                                model = svm_train(labels, features, training_options)

                                svm_save_model(filePath + 'encoderLayer.model', model)
                        else:
                                raise Exception('in libsvm_train, there is no file called encoderLayer.txt in the path provided.')

                counter = 0
                while os.path.isfile(filePath + 'regularLayer_' + str(counter) + '.txt') and not os.path.isfile(filePath + 'regularLayer_' + str(counter) + '.model'):
                        labels, features = svm_read_problem(filePath + 'regularLayer_' + str(counter) + '.txt')
                        # creating threads
                        threads = []
                        for thread in range(number_of_threads):
                                 threads.append(th.Thread(target=libsvm_cross_validation, args=\
                                                (labels, features, kernel,\
                                                 number_of_threads,\
                                                 thread,\
                                                 lock)))

                        # starting threads
                        for thread in threads:
                                thread.start()
                        
                        # wait until threads are completely executed
                        for thread in threads:
                                thread.join()

                        index = np.argmax(auxiliary_model)
                        model = auxiliary_model[index]
                        cost = auxiliary_cost[index]

                        auxiliary_model = np.array([])
                        auxiliary_cost = np.array([])

                        print('The best model for regularLayer_{} gives Accuracy = {}'.format(counter, model))
                        training_options = '-b 1 -t 0 -q -c ' + str(2**cost)
                        # training_options = '-t 0 -q -c ' + str(2**cost)
                        model = svm_train(labels, features, training_options)

                        svm_save_model(filePath + 'regularLayer_' + str(counter) + '.model', model)

                        counter += 1

        elif kernel is 2:
                if not os.path.isfile(filePath + 'inputs.model'):
                        if os.path.isfile(filePath + 'inputs.txt'):
                                labels, features = svm_read_problem(filePath + 'inputs.txt')
                                # creating threads
                                threads = []
                                for thread in range(number_of_threads):
                                         threads.append(th.Thread(target=libsvm_cross_validation, args=\
                                                        (labels, features, kernel,\
                                                         number_of_threads,\
                                                         thread,\
                                                         lock)))

                                # starting threads
                                for thread in threads:
                                        thread.start()
                                
                                # wait until threads are completely executed
                                for thread in threads:
                                        thread.join()

                                index = np.argmax(auxiliary_model)
                                model = auxiliary_model[index]
                                cost = auxiliary_cost[index]
                                gamma = auxiliary_gamma[index]

                                auxiliary_model = np.array([])
                                auxiliary_cost = np.array([])
                                auxiliary_gamma = np.array([])

                                print('The best model for inputs gives Accuracy = {}'.format(model))
                                training_options = '-b 1 -t 2 -q -c ' + str(2**cost) + ' -g ' + str(2**gamma)
                                # training_options = '-t 2 -q -c ' + str(2**cost) + ' -g ' + str(2**gamma)
                                model = svm_train(labels, features, training_options)

                                svm_save_model(filePath + 'inputs.model', model)
                        else:
                                raise Exception('in libsvm_train, there is no file called inputs.txt in the path provided.')

                if not os.path.isfile(filePath + 'encoderLayer.model'):
                        if os.path.isfile(filePath + 'encoderLayer.txt'):
                                labels, features = svm_read_problem(filePath + 'encoderLayer.txt')
                                # creating threads
                                threads = []
                                for thread in range(number_of_threads):
                                         threads.append(th.Thread(target=libsvm_cross_validation, args=\
                                                        (labels, features, kernel,\
                                                         number_of_threads,\
                                                         thread,\
                                                         lock)))

                                # starting threads
                                for thread in threads:
                                        thread.start()
                                
                                # wait until threads are completely executed
                                for thread in threads:
                                        thread.join()

                                index = np.argmax(auxiliary_model)
                                model = auxiliary_model[index]
                                cost = auxiliary_cost[index]
                                gamma = auxiliary_gamma[index]

                                auxiliary_model = np.array([])
                                auxiliary_cost = np.array([])
                                auxiliary_gamma = np.array([])

                                print('The best model for encoderLayer gives Accuracy = {}'.format(model))
                                training_options = '-b 1 -t 2 -q -c ' + str(2**cost) + ' -g ' + str(2**gamma)
                                # training_options = '-t 2 -q -c ' + str(2**cost) + ' -g ' + str(2**gamma)
                                model = svm_train(labels, features, training_options)

                                svm_save_model(filePath + 'encoderLayer.model', model)
                        else:
                                raise Exception('in libsvm_train, there is no file called encoderLayer.txt in the path provided.')

                counter = 0
                while os.path.isfile(filePath + 'regularLayer_' + str(counter) + '.txt') and not os.path.isfile(filePath + 'regularLayer_' + str(counter) + '.model'):
                        labels, features = svm_read_problem(filePath + 'regularLayer_' + str(counter) + '.txt')
                        # creating threads
                        threads = []
                        for thread in range(number_of_threads):
                                 threads.append(th.Thread(target=libsvm_cross_validation, args=\
                                                (labels, features, kernel,\
                                                 number_of_threads,\
                                                 thread,\
                                                 lock)))

                        # starting threads
                        for thread in threads:
                                thread.start()
                        
                        # wait until threads are completely executed
                        for thread in threads:
                                thread.join()

                        index = np.argmax(auxiliary_model)
                        model = auxiliary_model[index]
                        cost = auxiliary_cost[index]
                        gamma = auxiliary_gamma[index]

                        auxiliary_model = np.array([])
                        auxiliary_cost = np.array([])
                        auxiliary_gamma = np.array([])

                        print('The best model for regularLayer_{} gives Accuracy = {}'.format(counter, model))
                        training_options = '-b 1 -t 2 -q -c ' + str(2**cost) + ' -g ' + str(2**gamma)
                        # training_options = '-t 2 -q -c ' + str(2**cost) + ' -g ' + str(2**gamma)
                        model = svm_train(labels, features, training_options)

                        svm_save_model(filePath + 'regularLayer_' + str(counter) + '.model', model)

                        counter += 1

        else:
                raise Exception('in libsvm_train, kernel_type must be 0 or 2. The method received {}.'\
                                        .format(kernel_type))











def libsvm_test(filePath):

        if os.path.isfile(filePath + 'inputs.model'):
                if os.path.isfile(filePath + 'inputs.txt'):
                        labels, features = svm_read_problem(filePath + 'inputs.txt')

                        m = svm_load_model(filePath + 'inputs.model')
                        print('Classification results for inputs')
                        p_label, p_acc, p_val = svm_predict(labels, features, m, '-b 1')
                        # p_label, p_acc, p_val = svm_predict(labels, features, m)

                        sio.savemat(filePath + 'inputs_classification_accuracy.mat',\
                                    {'p_label': p_label, 'p_acc': p_acc, 'p_val': p_val})
                else:
                        raise Exception('in libsvm_test, there is no file called inputs.txt in the path provided.')

        if os.path.isfile(filePath + 'encoderLayer.model'):
                if os.path.isfile(filePath + 'encoderLayer.txt'):
                        labels, features = svm_read_problem(filePath + 'encoderLayer.txt')

                        m = svm_load_model(filePath + 'encoderLayer.model')
                        print('Classification results for encoderLayer')
                        p_label, p_acc, p_val = svm_predict(labels, features, m, '-b 1')
                        # p_label, p_acc, p_val = svm_predict(labels, features, m)

                        sio.savemat(filePath + 'encoderLayer_classification_accuracy.mat',\
                                    {'p_label': p_label, 'p_acc': p_acc, 'p_val': p_val})
                else:
                        raise Exception('in libsvm_test, there is no file called encoderLayer.txt in the path provided.')

        counter = 0
        while os.path.isfile(filePath + 'regularLayer_' + str(counter) + '.txt') and os.path.isfile(filePath + 'regularLayer_' + str(counter) + '.model'):
                labels, features = svm_read_problem(filePath + 'regularLayer_' + str(counter) + '.txt')

                m = svm_load_model(filePath + 'regularLayer_' + str(counter) + '.model')
                print('Classification results for regularLayer_{}'.format(counter))
                p_label, p_acc, p_val = svm_predict(labels, features, m, '-b 1')
                # p_label, p_acc, p_val = svm_predict(labels, features, m)

                sio.savemat(filePath + 'regularLayer_' + str(counter) + '_classification_accuracy.mat',\
                            {'p_label': p_label, 'p_acc': p_acc, 'p_val': p_val})
                counter += 1

