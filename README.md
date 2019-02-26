Cortical Spectro-Temporal Model for phonetic abstraction.
=========================================================

A new approach for deep phonetic acquisition
--------------------------------------------

The human brain is the most complex object created by evolution in the known universe. Yet, how much of this complexity is devoted to exclusively carrying out its algorithmic capabilities and how much of it has been inherited from biological paths of evolution in order to work properly in its physical environment? What if the information processing properties of the brain could be reduced to a few simple columnar rules replicated throughout the neocortex?  In our research project we seek for those principles by means of the elaboration of computational models of the neocortex. On the one hand, the simpler the rules, the more repetitions of such rules are needed in order to get interesting--human like--behaviours from our models--such as perception invariance. The mammalian brain--with its possible combinatorial states going beyond the number of atoms in the known universe--leaves us categorical clues in favor of such a position. On the other hand, the more repetitions of those simple rules, the more computational resources we need in order to implement our models.

We approached this endeavour trying to replicate human early language acquisition capabilities. We have started with incidental human early acquisition of phonotactic rules, specific to a particular language. We have implemented a neurocomputational model whose biological plausibility--at the cortical level--allows us to test neurophysiological hypotheses incorporated in the algorithms. With the desired number of layers, our model abstracts phonological features in a completely unsupervised fashion.

The input is composed of a series of words which are pre-processed by the [multiresolution spectro-temporal analysis of complex sounds](http://asa.scitation.org/doi/abs/10.1121/1.1945807?journalCode=jas) (MRSTSA) algorithm, which simulates subcortical nuclei in the auditory pathway. We implemented our own version of such algorithm in C. Such implementation is parallelized by means of MPI and OpenMP.

The outputs from the MRSTSA are received by the Encoder Layer (EL), which is the main stage in our model. The EL has shown promising phonetic classification capabilities which make us foresee it as a future alternative to current deep learning architectures which do not pay much attention to neurophysilogical features in the cortex discovered in the last years. Such features are incorporated in our model since we think they are key for information processing in cortical tissue. Unlike current deep learning architectures, our model uses [Sparse Distributed Representations (SDRs)](https://arxiv.org/pdf/1503.07469.pdf) in order to process information. SDRs have surprising mathematical properties which give them high noise rejection and fault tolerance. Those are typical characteristics in cortical tissue where individual cells are far from 100 percent reliable and they die and regenerate continuously. According to recent findings in neuroscience, the brain uses SDRs to process information. This is true for all mammals, from mice to humans.

The model developed in this project incorporates columnar organization, proximal and distal dendritic arborization, afferent, apical and lateral inter-columnar interaction, proximal lateral intra-columnar inhibition, long term potentiation, long term depression, spike time dependent plasticity and stochastic patterns of activation; to name a few of the neurophysiological phenomena modeled by these networks. Our model also exhibits massive firing events (lack of inhibition) as a response to prediction faults (unexpected sequential stimuli in the stream of data) and it responds with normal firing events (inhibition) when the sequential stimulus is known.

The phonetic features extracted by the model improve the performance of standardized supervised pattern classification techniques whose main objective is to assess the level of invariance achieved by the model layers. We use [libsvm](https://www.csie.ntu.edu.tw/~cjlin/libsvm/) library under Matlab and GNU Octave in order to test phonetic invariance in the classification performance measured on the MRSTSA outputs as well as on the EL output abstract features.

The algorithms in our Cortical Spectro-Temporal Model (CSTM) are C++14 compliant, they are composed of a set of classes interrelated by inheritance and composition. The classes in the algorithms are parallelized by means of an hybrid paradigm using MPI and OpenMP. This implementation has the capacity of storing its outputs in Matlab and Octave file formats and it is suitable for the use of parallel I/O file systems.

## Getting Started

In order to get a copy of this project on your local machine run:

Use SSH
```
git clone git@github.com:neurophon/neurophon.git
```

Use HTTPS
```
git clone https://github.com/neurophon/neurophon.git
```

### Prerequisites

This project has been tested in the following environment (what does not necessarily mean that it could not work in others)

On [Cooley](https://www.alcf.anl.gov/user-guides/cooley) at [Argonne National Laboratory](https://www.anl.gov/):
```
[demattie@cooleylogin2 ~]$ cat /etc/redhat-release 
Red Hat Enterprise Linux Server release 7.6 (Maipo)
[demattie@cooleylogin2 ~]$ gcc --version
gcc (GCC) 7.1.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

[demattie@cooleylogin2 ~]$ mpiexec --version
HYDRA build details:
    Version:                                 3.1.4
    Release Date:                            Wed Sep  7 14:33:43 EDT 2016
    CC:                              gcc    
    CXX:                             g++    
    F77:                             gfortran   
    F90:                             gfortran   
    Configure options:                       '--disable-option-checking' '--prefix=/soft/libraries/mpi/mvapich2-2.2/gcc' '--cache-file=/dev/null' '--srcdir=.' 'CC=gcc' 'CFLAGS= -DNDEBUG -DNVALGRIND -O2' 'LDFLAGS=-L/lib -L/lib -L/lib -Wl,-rpath,/lib -L/lib -Wl,-rpath,/lib -L/lib -L/lib' 'LIBS=-libmad -lrdmacm -libumad -libverbs -ldl -lrt -lm -lpthread ' 'CPPFLAGS= -I/home/acherry/src/mvapich2-2.2/src/mpl/include -I/home/acherry/src/mvapich2-2.2/src/mpl/include -I/home/acherry/src/mvapich2-2.2/src/openpa/src -I/home/acherry/src/mvapich2-2.2/src/openpa/src -D_REENTRANT -I/home/acherry/src/mvapich2-2.2/src/mpi/romio/include -I/include -I/include -I/include -I/include'
    Process Manager:                         pmi
    Launchers available:                     ssh rsh fork slurm ll lsf sge manual persist
    Topology libraries available:            hwloc
    Resource management kernels available:   user slurm ll lsf sge pbs cobalt
    Checkpointing libraries available:       
    Demux engines available:                 poll select
[demattie@cooleylogin2 ~]$ python3 --version
Python 3.5.1 :: Anaconda 4.0.0 (64-bit)
[demattie@cooleylogin2 ~]$ fftw-wisdom
(fftw-3.3.3 fftw_wisdom #x91a0d0f0 #xe3fff824 #xbb042095 #x2f9c8270
)
[demattie@cooleylogin2 ~]$ 
```
Additionally, you will need several packages for python (such as `mpi4py`) which are appropriately referenced in the corresponding code.

You will need [Festival Text-to-Speech](http://www.cstr.ed.ac.uk/projects/festival/) soft too, which is a general multi-lingual speech synthesis system originally developed by Alan W. Black at Centre for Speech Technology Research (CSTR) at the University of Edinburgh.

```
[demattie@cooleylogin2 bin]$ ./festival

Festival Speech Synthesis System 2.4:release December 2014
Copyright (C) University of Edinburgh, 1996-2010. All rights reserved.

clunits: Copyright (C) University of Edinburgh and CMU 1997-2010
clustergen_engine: Copyright (C) Carnegie Mellon University 2005-2014
hts_engine: 
The HMM-Based Speech Synthesis Engine "hts_engine API"
hts_engine API version 1.07 (http://hts-engine.sourceforge.net/)
Copyright (C) The HMM-Based Speech Synthesis Engine "hts_engine API"
Version 1.07 (http://hts-engine.sourceforge.net/)
Copyright (C) 2001-2012 Nagoya Institute of Technology
              2001-2008 Tokyo Institute of Technology
All rights reserved.

All rights reserved.
For details type `(festival_warranty)'
festival>       
[demattie@cooleylogin2 bin]$ 
```

You will also need software featuring a high-level programming languages, primarily intended for numerical computations, such as GNU Octave or Matlab.

```
[demattie@cooleylogin2 ~]$ matlab 
MATLAB is selecting SOFTWARE OPENGL rendering.

                                                                            < M A T L A B (R) >
                                                                  Copyright 1984-2015 The MathWorks, Inc.
                                                                   R2015b (8.6.0.267246) 64-bit (glnxa64)
                                                                              August 20, 2015

 
To get started, type one of these: helpwin, helpdesk, or demo.
For product information, visit www.mathworks.com.
 
>> exit
[demattie@cooleylogin2 ~]$ octave
warning: docstring file '/usr/share/octave/3.8.2/etc/built-in-docstrings' not found
GNU Octave, version 3.8.2
Copyright (C) 2014 John W. Eaton and others.
This is free software; see the source code for copying conditions.
There is ABSOLUTELY NO WARRANTY; not even for MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  For details, type 'warranty'.

Octave was configured for "x86_64-redhat-linux-gnu".

Additional information about Octave is available at http://www.octave.org.

Please contribute if you find this software useful.
For more information, visit http://www.octave.org/get-involved.html

Read http://www.octave.org/bugs.html to learn how to submit bug reports.
For information about changes from previous versions, type 'news'.

octave:1> exit

[demattie@cooleylogin2 ~]$ 
```
Finally, you will need [libsvm](https://www.csie.ntu.edu.tw/~cjlin/libsvm/) package, which we use under Matlab on Cooley.
You can use it under [GNU Octave](https://www.gnu.org/software/octave/).

### Building

In order to get the development environment running you have to execute the following steps.

Assuming you cloned this repository in your home directory you have to do the following actions.

To build the Multiresolution Spectro-Temporal Sound Analysis (MRSTSA) you have to run the following commands

```
[demattie@cooleylogin2 ~]$ cd ~/neurophon/C++/MyMelFilterBank/
[demattie@cooleylogin2 MyMelFilterBank]$ make
```
To build the Encoder Layer you have to run the following commands

```
[demattie@cooleylogin2 ~]$ cd ~/neurophon/C++/Model/
[demattie@cooleylogin2 Model]$ make
```

## Running the tests

Before you can run tests on the CSTM, the datasets must be available.
Then you have to generate the MRSTSA outputs which will feed the EL inputs.
Once the MRSTSA outputs are available you will be able to run EL training and inference stages.
After this point, you have to train the Support Vector Machine (SVM) algorithms with the data returned by the MRSTSA and the EL in inference mode.
Finally you will be able to use the SVM algorithms to test the models trained in the last stage.

Below, is a detailed (step-by-step) explanation to run the tests.

### Getting the datasets to train and test the CSTM

#### Getting the datasets from Zenodo

First of all, download the [two zip files](https://zenodo.org/record/2576130#.XHQwOIV7lNA).
In our case we have a folder in the following path: `/projects/neurophon/TestsData/5_Way_Corpora`.
We extract the files in such folder and such path is configured in our tests (source code).
If you have a different path, you will have to change the path in the source code in different parts of the repository in order to make it coherent with yours.

#### Generating the datasets from Python Scripts

In order to submit jobs on Cooley we use `qsub`, which is an IEEE Std 1003.1-2008 Unix command for submitting jobs to a job scheduler, usually in cluster or grid computing.

The following command, `qsub -n 1 -t 720 -A neurophon ./generate_Corpora.sh` submits a job using a script called `generate_Corpora.sh` asking for 1 node during 12 hours (720 minutes) in the queue.

The script will run a python script to generate the corpora.

If the run cannot generate all the corpora asked, the python script can be called again in order to continue its work from the point where it left the previous job since such script has checkpoint and restart capacity.

```
#!/bin/sh
cd ~/neurophon/Python/GenerateTextCorpora
mpiexec -n 10 -f $COBALT_NODEFILE -env MV2_ENABLE_AFFINITY=0 python3 GenerateTextCorpora.py
```
In its current state, the python script will generate 2 corpora of 500 words for each condition which is determined by 2 set of voices, three syllabic conditions and ten different vocabularies.

This configuration ends up producing 120 corpora.

Once the corpora has been generated, you have to create a new set of audio files from each file `Corpus.wav` with different acoustic variants.
We use [Audacity](https://www.audacityteam.org/) audio editor to incorporate different acoustic variants to the original corpora generated using Festival.

For example, having the following directory tree in the folder `5_Way_Corpora`,

```
.
├── 5_Way_Corpora
│   ├── Voices_1
│   │   ├── Syllables_1
│   │   │   ├── Corpus_0
│   │   │   │   ├── Vocabulary_0
│   │   │   │   │   ├── AudioVector.mat
│   │   │   │   │   ├── CorpusMetadata.mat
│   │   │   │   │   ├── Corpus.sable
│   │   │   │   │   └── Corpus.wav
│   │   │   │   ├── Vocabulary_1
│   │   │   │   │   ├── AudioVector.mat
│   │   │   │   │   ├── CorpusMetadata.mat
│   │   │   │   │   ├── Corpus.sable
│   │   │   │   │   └── Corpus.wav
│   │   │   │   ├── Vocabulary_2
│   │   │   │   │   ├── AudioVector.mat
│   │   │   │   │   ├── CorpusMetadata.mat
│   │   │   │   │   ├── Corpus.sable
│   │   │   │   │   └── Corpus.wav
│   │   │   │   ├── Vocabulary_3
│   │   │   │   │   ├── AudioVector.mat
│   │   │   │   │   ├── CorpusMetadata.mat
│   │   │   │   │   ├── Corpus.sable
│   │   │   │   │   └── Corpus.wav
```

you have to get into `Vocabulary_0` in such path and create 7 new folders called 'original', `whitenoise1`, `whitenoise2`, `reverberation30`, `reverberation60`, `pitchdown` and `pitchup`.
Inside such folders we save the corpora that match the file `Corpus.wav` in `Vocabulary_0` affected by the corresponding acoustic variant.
The `original` folder will hold the original `Corpus.wav` file.
White Noise 1 determines a SNR average RMS power rate of 19.8 dB while White Noise 2 13.8 dB. Reverberation 30% determines a RT-60 value of 0.61 seconds while
Reverberation 60% determines a RT-60 value of 1.78 seconds.  Pitch Up determines a pitch move from E to G, while Pitch Down determines a pitch move from E to C.

Taking into account the original corpus you will end up with 840 corpora in the folder. 

If you want to add, remove or modify such acoustic variants and its corresponding folder names, you will have to modify the source code that references them.



### Preparing the datasets for the MRSTSA algorithm

In order to prepare the datasets to feed the MRSTSA algorithm you have to go into the following path, `~/neurophon/Octave/GenerateAudioCorpora`.
Then, from Matlab run:

```
>> GenerateAudioVector_M(number of voices, syllabic configurations, number of corpora, number of vocabularies, acoustic variants)
```

In this particular case it would be:

```
>> GenerateAudioVector_M(2,3,2,10,true)
```

This sentence will generate an audio for each `Corpus.wav` file from a scheme with 2 voices, three syllabic conditions, two corpora and ten vocabularies in 7 acoustic variants (including the original corpus). 




### Running the MRSTSA algorithm

Once the preprocessing of the datasets has been conducted you can run the following command in order to reserve 20 nodes for 1 hour.

```
qsub -n 20 -t 60 -A neurophon ./generate_Inputs.sh
```
 
And the script is:

```
#!/bin/sh
cd ~/neurophon/C++/MyMelFilterBank
mpiexec -n 20 -f $COBALT_NODEFILE -env MV2_ENABLE_AFFINITY=0 -ppn 1 ./Test 2 3 2 10 variance
```

This script passes the same parameters to the MRSTSA algorithm in order to process the 840 corpora (i.e. 2 voices, 3 syllabic conditions, 2 corpora and 10 vocabularies) in the dataset.

Once the MRSTSA algorithms have run properly, you have to go to `cd ~/neurophon/Octave/`, launch Matlab and run the following program: `>> ComposeInput_M(2,3,2,10,true)`.
This program will provide you with a file ready to be processed by the EL (such file is called `inputs.mat`).


### Running the EL algorithm

Once the outputs from the MRSTSA are properly formated you will use those files as inputs to feed the EL, but first of all you will have to generate a folder containing all the model specifications. After generating such folder, an EL run will read and test the files inside it and generate and EL object guided by such specifications.

You will go to `~/neurophon/Octave` and launch Matlab. Then you will run the following program in Matlab:

```
>> GenerateModelFiles_M('Model')
```

Such program generates a folder called `Model` in the path `~/neurophon/Octave`.

The Matlab code in `GenerateModelFiles_M()` is:

```
% File Name:		GenerateModelFiles_M.m
% Language:		GNU Octave high-level interpreted language.

% This program generates a set of files which will be used by Model object class 
% in order togenerate its structure and configuration.

function GenerateModelFiles_M(folderName)

keepVariables = [folderName];
clear -except keepVariables

if ~(ischar(folderName))
	error('folder name must be of type string');
end

[status, msg, msgid] = mkdir (folderName);
if ( status ~= 1 )
	disp(msg);
	disp(msgid);
	error('in function GenerateModelFiles_M');
end

clear -except keepVariables

my_residue = 1*10^(-10);
% This is the model structure
encoderIncorporation = 1 + my_residue;
newEncoder = 1 + my_residue;
numberOfLayers = 0 + my_residue;
newLayerAt = 0 + my_residue;
initialStageAt = 0 + my_residue;
iterations = 4 + my_residue;
stages = 4 + my_residue;

% Saves the model structure
name = ['./'];
name = [name folderName];
name = [name '/ModelStructure.mat'];

save (name, 'encoderIncorporation', ...
	    'newEncoder', ...
	    'numberOfLayers', ...
	    'newLayerAt', ...
	    'initialStageAt', ...
	    'iterations', ...
	    'stages', ...
	    '-v6'); 

clear -except keepVariables

% This is the encoder layer structure
afferentArrayDimensionality = [5 + my_residue, 128 + my_residue];
apicalArrayDimensionality = [1 + my_residue, 1 + my_residue];
columnsArrayDimensionality = [15 + my_residue, 15 + my_residue];

afferentReceptiveField = [2 + my_residue, 63 + my_residue];
afferentPercentage = 0.05;
afferentWrapAround = 1 + my_residue;

lateralDistalReceptiveField = [4 + my_residue, 4 + my_residue];
lateralDistalPercentage = 0.9;
lateralDistalWrapAround = 1 + my_residue;

apicalReceptiveField = [-2 + my_residue, -2 + my_residue];
apicalPercentage = 0.9;
apicalWrapAround = 1 + my_residue;

iterationNum = 0 + my_residue;

populationsArrayDimensionality = [15 + my_residue, 15 + my_residue];
apicalPopulationsArrayDimensionality = [15 + my_residue,15 + my_residue];
potentialPercentage = 0.03;

% Saves the encoder layer structure
name = ['./'];
name = [name folderName];
name = [name '/EncoderLayerStructure.mat'];

save (name, 'afferentArrayDimensionality', ...
	    'apicalArrayDimensionality', ...
	    'columnsArrayDimensionality', ...
	    'afferentReceptiveField', ...
	    'afferentPercentage', ...
	    'afferentWrapAround', ...
	    'lateralDistalReceptiveField', ...
	    'lateralDistalPercentage', ...
	    'lateralDistalWrapAround', ...
	    'apicalReceptiveField', ...
	    'apicalPercentage', ...
	    'apicalWrapAround', ...
	    'iterationNum', ...
	    'populationsArrayDimensionality', ...
	    'apicalPopulationsArrayDimensionality', ...
	    'potentialPercentage', ...
	    '-v6');

clear -except keepVariables

% These are the encoder layer parameters 
enableLearning = 0 + my_residue;
distalSensitivity = 0 + my_residue;
proximalInformationThreshold = 0.1;
distalInformationThreshold = 0.5;
activationRadius = 0.8;
sparsity = 0.99;
enableProximalLearning = 0 + my_residue;
enableDistalLearning = 0 + my_residue;
proximalLearningRate = 0.001;
proximalNeighborhood = 0.005;
spikeTimeDependentSynapticPlasticity = 1 + my_residue;
distalLearningRate = 0.001;
limitsLearningRate = 0.1;

% Saves the encoder layer parameters
name = ['./'];
name = [name folderName];
name = [name '/EncoderLayerParameters.mat'];

save (name, 'enableLearning', ...
	    'distalSensitivity', ...
	    'proximalInformationThreshold', ...
	    'distalInformationThreshold', ...
	    'activationRadius', ...
	    'sparsity', ...
	    'enableProximalLearning', ...
	    'enableDistalLearning', ...
	    'proximalLearningRate', ...
	    'proximalNeighborhood', ...
	    'spikeTimeDependentSynapticPlasticity', ...
	    'distalLearningRate', ...
	    'limitsLearningRate', ...
	    '-v6');

clear -except keepVariables

end

```

This program generates the specification for an EL with 15 for 15 cortical columns (CCs), with afferent receptive fields of 5 for 127 inputs, lateral receptive fields of 9 for 9 CCs, etc. This program specifies a CSTM instance in three files `ModelStructure.mat`, `EncoderLayerStructure.mat` and `EncoderLayerParameters.mat` in which it gives details of the whole CSTM structure, the EL structure and specific EL parameters.

In our experimental setup we move the folder `Model` to the path `/projects/neurophon/TestsData`. If you choose another path, you will have to change that path in the source code of the project.

Once you have the `Model` folder in the right place you will run the following `qsub` command: `qsub -n 25 -t 240 -A neurophon ./run_Model.sh`, whose script `run_Model.sh` has the following instructions:

```
#!/bin/sh

# Common path
my_path='/projects/neurophon/TestsData/'

# Processes the original corpus
# Prepares the input file for the original corpus
cd $my_path
cp 5_Way_Corpora/Voices_2/Syllables_1/Corpus_0/Vocabulary_0/original/inputs.mat Model/inputs.mat
mkdir -p Model/original/ && cp 5_Way_Corpora/Voices_2/Syllables_1/Corpus_0/Vocabulary_0/original/inputs.mat Model/original/inputs.mat
mkdir -p Model/whitenoise1/ && cp 5_Way_Corpora/Voices_2/Syllables_1/Corpus_1/Vocabulary_0/whitenoise1/inputs.mat Model/whitenoise1/inputs.mat
mkdir -p Model/whitenoise2/ && cp 5_Way_Corpora/Voices_2/Syllables_1/Corpus_1/Vocabulary_0/whitenoise2/inputs.mat Model/whitenoise2/inputs.mat
mkdir -p Model/reverberation30/ && cp 5_Way_Corpora/Voices_2/Syllables_1/Corpus_1/Vocabulary_0/reverberation30/inputs.mat Model/reverberation30/inputs.mat
mkdir -p Model/reverberation60/ && cp 5_Way_Corpora/Voices_2/Syllables_1/Corpus_1/Vocabulary_0/reverberation60/inputs.mat Model/reverberation60/inputs.mat
mkdir -p Model/pitchdown/ && cp 5_Way_Corpora/Voices_2/Syllables_1/Corpus_1/Vocabulary_0/pitchdown/inputs.mat Model/pitchdown/inputs.mat
mkdir -p Model/pitchup/ && cp 5_Way_Corpora/Voices_2/Syllables_1/Corpus_1/Vocabulary_0/pitchup/inputs.mat Model/pitchup/inputs.mat
mkdir -p Model/changedvoices/ && cp 5_Way_Corpora/Voices_1/Syllables_1/Corpus_1/Vocabulary_0/original/inputs.mat Model/changedvoices/inputs.mat


cd ~/hstm/C++/Model/
# Trains the model
mpiexec -n 25 -f $COBALT_NODEFILE -env MV2_ENABLE_AFFINITY=0 -ppn 1 ./Test Model training

# Run the model in inference mode
mpiexec -n 25 -f $COBALT_NODEFILE -env MV2_ENABLE_AFFINITY=0 -ppn 1 ./Test Model inference

```

This script copies all the necessary corpora and then runs the model in training and inference mode.
Afterwars, inside the `Model` folder you will end up with a set of sub-folders: `original`, `whitenoise1`, `whitenoise2`, `reverberation30`, `reverberation60`, `pitchdown`, `pitchup` and `changedvoices` which will contain the MRSTSA an EL outputs to train and test the SVM algorithms.






### Preparing the data to Train and Test the SVM algorithm

After running the EL you are in good shape to train and test the SVM algorithm.

First of all, you have to prepare the data to train the SVM algorithm.

Copy the files in `original` folder to `Octave` folder: `cp /projects/neurophon/TestsData/Model/original/*.mat ~/neurophon/Octave`, then launch Matlab and run the following script: `>> processes_data_for_train_supervision_M(1)`. This script will prepare the data to train both SVM models, one using the MRSTSA outputs and the other using the EL outputs.

To train the SVM models see [libsvm](https://www.csie.ntu.edu.tw/~cjlin/libsvm/) documentation.

Once the SVM models are trained, you will be able to test them using some of the features returned by the MRSTSA and the EL in response to the corpora affected by the acoustic variants. In order to prepare the data to test the SVM models you will have to run the following command `cp /projects/neurophon/TestsData/Model/whitenoise1/*.mat ~/neurophon/Octave` and then run the following script from Matlab `processes_data_for_test_supervision_M(1)`. Such script will prepare the MRSTSA outputs and the EL outputs to test the SVM already trained models.

To test the SVM models see [libsvm](https://www.csie.ntu.edu.tw/~cjlin/libsvm/) documentation.



