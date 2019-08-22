%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%				Author:		Dematties Dario Jesus						%%
%%				Contact:	dariodematties@hotmail.com.ar					%%
%%						dariodematties@yahoo.com.ar					%%
%%						dario.dematties@frm.utn.edu.ar					%%
%%				Project:	Engineering PhD Project						%%
%%				Institution:	Universidad de Buenos Aires					%%
%%						Facultad de Ingeniería (FIUBA)					%%
%%				Workplace:	Instituto de Ingeniería						%%
%%						Biomédica FIUBA	&						%%
%%						CCT CONICET Mendoza INCIHUSA					%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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
numberOfLayers = 2 + my_residue;
%numberOfLayers = 0 + my_residue;
newLayerAt = 0 + my_residue;
initialStageAt = 0 + my_residue;
iterations = 2 + my_residue;
stages = 2 + my_residue;
%iterations = 4 + my_residue;
%stages = 4 + my_residue;

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
afferentArrayDimensionality = [10 + my_residue, 30 + my_residue];
%afferentArrayDimensionality = [5 + my_residue, 128 + my_residue];
%apicalArrayDimensionality = [1 + my_residue, 1 + my_residue];
apicalArrayDimensionality = [15 + my_residue, 15 + my_residue];
columnsArrayDimensionality = [15 + my_residue, 15 + my_residue];

afferentReceptiveField = [4 + my_residue, 14 + my_residue];
%afferentReceptiveField = [2 + my_residue, 63 + my_residue];
afferentPercentage = 0.12;
%afferentPercentage = 0.05;
afferentWrapAround = 1 + my_residue;

lateralDistalReceptiveField = [4 + my_residue, 4 + my_residue];
lateralDistalPercentage = 0.9/3;
%lateralDistalPercentage = 0.9;
lateralDistalWrapAround = 1 + my_residue;

apicalReceptiveField = [4 + my_residue, 4 + my_residue];
%apicalReceptiveField = [-2 + my_residue, -2 + my_residue];
apicalPercentage = 0.9;
apicalWrapAround = 1 + my_residue;

iterationNum = 0 + my_residue;

populationsArrayDimensionality = [15 + my_residue, 15 + my_residue];
apicalPopulationsArrayDimensionality = [15 + my_residue,15 + my_residue];
%apicalPopulationsArrayDimensionality = [15 + my_residue, 15 + my_residue];
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














% This is the regular layer structure number 0
afferentArrayDimensionality = [15 + my_residue, 15 + my_residue];
%apicalArrayDimensionality = [1 + my_residue, 1 + my_residue];
apicalArrayDimensionality = [15 + my_residue, 15 + my_residue];
columnsArrayDimensionality = [15 + my_residue, 15 + my_residue];

afferentReceptiveField = [4 + my_residue, 4 + my_residue];
afferentPercentage = 0.025*2;
%afferentPercentage = 0.9;
afferentWrapAround = 1 + my_residue;

%lateralProximalReceptiveField = [4 + my_residue, 4 + my_residue];
lateralProximalReceptiveField = [-2 + my_residue, -2 + my_residue];
lateralProximalPercentage = 0.8;
%lateralProximalPercentage = 0.8;
lateralProximalWrapAround = 1 + my_residue;

lateralDistalReceptiveField = [4 + my_residue, 4 + my_residue];
lateralDistalPercentage = 0.9/3;
%lateralDistalPercentage = 0.9;
lateralDistalWrapAround = 1 + my_residue;

apicalReceptiveField = [4 + my_residue, 4 + my_residue];
%apicalReceptiveField = [-2 + my_residue, -2 + my_residue];
apicalPercentage = 0.9;
apicalWrapAround = 1 + my_residue;

iterationNum = 0 + my_residue;

populationsArrayDimensionality = [15 + my_residue,15 + my_residue];
%populationsArrayDimensionality = [15 + my_residue, 15 + my_residue];
afferentPopulationsArrayDimensionality = [15 + my_residue, 15 + my_residue];
apicalPopulationsArrayDimensionality = [15 + my_residue, 15 + my_residue];

temporalGatheringAfferentValue = 2 + my_residue;
proximalPotentialPercentage = 3*0.0045;
distalPotentialPercentage = 0.03;

% Saves the regular layer structure number 0
name = ['./'];
name = [name folderName];
name = [name '/RegularLayerStructure_0.mat'];

save (name, 'afferentArrayDimensionality', ...
	    'apicalArrayDimensionality', ...
	    'columnsArrayDimensionality', ...
	    'afferentReceptiveField', ...
	    'afferentPercentage', ...
	    'afferentWrapAround', ...
	    'lateralProximalReceptiveField', ...
	    'lateralProximalPercentage', ...
	    'lateralProximalWrapAround', ...
	    'lateralDistalReceptiveField', ...
	    'lateralDistalPercentage', ...
	    'lateralDistalWrapAround', ...
	    'apicalReceptiveField', ...
	    'apicalPercentage', ...
	    'apicalWrapAround', ...
	    'iterationNum', ...
	    'populationsArrayDimensionality', ...
	    'afferentPopulationsArrayDimensionality', ...
	    'apicalPopulationsArrayDimensionality', ...
	    'temporalGatheringAfferentValue', ...
	    'proximalPotentialPercentage', ...
	    'distalPotentialPercentage', ...
	    '-v6');

clear -except keepVariables

% These are the regular layer parameters number 0
enableLearning = 0 + my_residue;
distalSensitivity = 0 + my_residue;
activationHomeostasis = 0 + my_residue;
proximalInformationThreshold = 0.5;
distalInformationThreshold = 0.5;
sparsity = 0.99;
enableProximalLearning = 0 + my_residue;
enableDistalLearning = 0 + my_residue;
synapticHomeostasis = 0 + my_residue;
proximalLearningRate = 0.001;
proximalNeighborhood = 0.005;
plasticity = 0.1; %0.01;
spikeTimeDependentSynapticPlasticity = 1 + my_residue;
distalLearningRate = 0.001;

% Saves the regular layer parameters number 0
name = ['./'];
name = [name folderName];
name = [name '/RegularLayerParameters_0.mat'];

save (name, 'enableLearning', ...
	    'distalSensitivity', ...
	    'activationHomeostasis', ...
	    'proximalInformationThreshold', ...
	    'distalInformationThreshold', ...
	    'sparsity', ...
	    'enableProximalLearning', ...
	    'enableDistalLearning', ...
	    'synapticHomeostasis', ...
	    'proximalLearningRate', ...
	    'proximalNeighborhood', ...
	    'plasticity', ...
	    'spikeTimeDependentSynapticPlasticity', ...
	    'distalLearningRate', ...
	    '-v6');

clear -except keepVariables

















% This is the regular layer structure number 1
afferentArrayDimensionality = [15 + my_residue, 15 + my_residue];
%apicalArrayDimensionality = [15 + my_residue, 15 + my_residue];
apicalArrayDimensionality = [1 + my_residue, 1 + my_residue];
columnsArrayDimensionality = [15 + my_residue, 15 + my_residue];

afferentReceptiveField = [4 + my_residue, 4 + my_residue];
afferentPercentage = 0.025*2;
%afferentPercentage = 0.9;
afferentWrapAround = 1 + my_residue;

lateralProximalReceptiveField = [-2 + my_residue, -2 + my_residue];
%lateralProximalReceptiveField = [4 + my_residue, 4 + my_residue];
lateralProximalPercentage = 0.5;
lateralProximalWrapAround = 1 + my_residue;

lateralDistalReceptiveField = [4 + my_residue, 4 + my_residue];
lateralDistalPercentage = 0.9;
lateralDistalWrapAround = 1 + my_residue;

%apicalReceptiveField = [4 + my_residue, 4 + my_residue];
apicalReceptiveField = [-2 + my_residue, -2 + my_residue];
apicalPercentage = 0.9;
apicalWrapAround = 1 + my_residue;

iterationNum = 0 + my_residue;

populationsArrayDimensionality = [15 + my_residue, 15 + my_residue];
afferentPopulationsArrayDimensionality = [15 + my_residue, 15 + my_residue];
apicalPopulationsArrayDimensionality = [15 + my_residue, 15 + my_residue];

temporalGatheringAfferentValue = 2 + my_residue;
proximalPotentialPercentage = 3*0.0045;
distalPotentialPercentage = 0.03;

% Saves the regular layer structure number 1
name = ['./'];
name = [name folderName];
name = [name '/RegularLayerStructure_1.mat'];

save (name, 'afferentArrayDimensionality', ...
	    'apicalArrayDimensionality', ...
	    'columnsArrayDimensionality', ...
	    'afferentReceptiveField', ...
	    'afferentPercentage', ...
	    'afferentWrapAround', ...
	    'lateralProximalReceptiveField', ...
	    'lateralProximalPercentage', ...
	    'lateralProximalWrapAround', ...
	    'lateralDistalReceptiveField', ...
	    'lateralDistalPercentage', ...
	    'lateralDistalWrapAround', ...
	    'apicalReceptiveField', ...
	    'apicalPercentage', ...
	    'apicalWrapAround', ...
	    'iterationNum', ...
	    'populationsArrayDimensionality', ...
	    'afferentPopulationsArrayDimensionality', ...
	    'apicalPopulationsArrayDimensionality', ...
	    'temporalGatheringAfferentValue', ...
	    'proximalPotentialPercentage', ...
	    'distalPotentialPercentage', ...
	    '-v6');

clear -except keepVariables

% These are the regular layer parameters number 1
enableLearning = 0 + my_residue;
distalSensitivity = 0 + my_residue;
activationHomeostasis = 0 + my_residue;
proximalInformationThreshold = 0.5;
distalInformationThreshold = 0.5;
sparsity = 0.99;
enableProximalLearning = 0 + my_residue;
enableDistalLearning = 0 + my_residue;
synapticHomeostasis = 0 + my_residue;
proximalLearningRate = 0.001;
proximalNeighborhood = 0.005;
plasticity = 0.1; %0.01;
spikeTimeDependentSynapticPlasticity = 1 + my_residue;
distalLearningRate = 0.001;

% Saves the encoder layer parameters
name = ['./'];
name = [name folderName];
name = [name '/RegularLayerParameters_1.mat'];

save (name, 'enableLearning', ...
	 'distalSensitivity', ...
	 'activationHomeostasis', ...
	 'proximalInformationThreshold', ...
	 'distalInformationThreshold', ...
	 'sparsity', ...
	 'enableProximalLearning', ...
	 'enableDistalLearning', ...
	 'synapticHomeostasis', ...
	 'proximalLearningRate', ...
	 'proximalNeighborhood', ...
	 'plasticity', ...
	 'spikeTimeDependentSynapticPlasticity', ...
	 'distalLearningRate', ...
	 '-v6');














%{
% This is the regular layer structure number 2
afferentArrayDimensionality = [3 + my_residue, 3 + my_residue];
apicalArrayDimensionality = [1 + my_residue, 1 + my_residue];
columnsArrayDimensionality = [3 + my_residue, 3 + my_residue];

afferentReceptiveField = [1 + my_residue, 1 + my_residue];
afferentPercentage = 0.9;
afferentWrapAround = 1 + my_residue;

lateralProximalReceptiveField = [1 + my_residue, 1 + my_residue];
lateralProximalPercentage = 0.5;
lateralProximalWrapAround = 1 + my_residue;

lateralDistalReceptiveField = [1 + my_residue, 1 + my_residue];
lateralDistalPercentage = 0.9;
lateralDistalWrapAround = 1 + my_residue;

apicalReceptiveField = [-2 + my_residue, -2 + my_residue];
apicalPercentage = 0.9;
apicalWrapAround = 1 + my_residue;

iterationNum = 0 + my_residue;

populationsArrayDimensionality = [15 + my_residue, 15 + my_residue];
afferentPopulationsArrayDimensionality = [15 + my_residue, 15 + my_residue];
apicalPopulationsArrayDimensionality = [15 + my_residue, 15 + my_residue];

temporalGatheringAfferentValue = 1 + my_residue;
proximalPotentialPercentage = 0.03;
distalPotentialPercentage = 0.03;

% Saves the regular layer structure number 2
name = ['./'];
name = [name folderName];
name = [name '/RegularLayerStructure_2.mat'];

save (name, 'afferentArrayDimensionality', ...
	    'apicalArrayDimensionality', ...
	    'columnsArrayDimensionality', ...
	    'afferentReceptiveField', ...
	    'afferentPercentage', ...
	    'afferentWrapAround', ...
	    'lateralProximalReceptiveField', ...
	    'lateralProximalPercentage', ...
	    'lateralProximalWrapAround', ...
	    'lateralDistalReceptiveField', ...
	    'lateralDistalPercentage', ...
	    'lateralDistalWrapAround', ...
	    'apicalReceptiveField', ...
	    'apicalPercentage', ...
	    'apicalWrapAround', ...
	    'iterationNum', ...
	    'populationsArrayDimensionality', ...
	    'afferentPopulationsArrayDimensionality', ...
	    'apicalPopulationsArrayDimensionality', ...
	    'temporalGatheringAfferentValue', ...
	    'proximalPotentialPercentage', ...
	    'distalPotentialPercentage', ...
	    '-v6');

clear -except keepVariables

% These are the regular layer parameters number 2
enableLearning = 0 + my_residue;
distalSensitivity = 0 + my_residue;
activationHomeostasis = 0 + my_residue;
proximalInformationThreshold = 0.5;
distalInformationThreshold = 0.5;
sparsity = 0.99;
enableProximalLearning = 0 + my_residue;
enableDistalLearning = 0 + my_residue;
synapticHomeostasis = 0 + my_residue;
proximalLearningRate = 0.001;
proximalNeighborhood = 0.005;
plasticity = 0.1; %0.01;
spikeTimeDependentSynapticPlasticity = 1 + my_residue;
distalLearningRate = 0.001;

% Saves the encoder layer parameters
name = ['./'];
name = [name folderName];
name = [name '/RegularLayerParameters_2.mat'];

save (name, 'enableLearning', ...
	 'distalSensitivity', ...
	 'activationHomeostasis', ...
	 'proximalInformationThreshold', ...
	 'distalInformationThreshold', ...
	 'sparsity', ...
	 'enableProximalLearning', ...
	 'enableDistalLearning', ...
	 'synapticHomeostasis', ...
	 'proximalLearningRate', ...
	 'proximalNeighborhood', ...
	 'plasticity', ...
	 'spikeTimeDependentSynapticPlasticity', ...
	 'distalLearningRate', ...
	 '-v6');


%}

end

