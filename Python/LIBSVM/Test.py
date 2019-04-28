import sys
sys.path.append('/home/demattie/hstm/Python/LIBSVM')
import ProcessesDataForLIBSVM as svm
import LIBSVM

filePath = '/projects/neurophon/TestsData/auxiliary/'
# svm.processes_data_for_train_libsvm(filePath)
LIBSVM.libsvm_train(filePath, 2)
