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

function GenerateModelFiles(folderName)

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

% This is the model structure
logical(encoderIncorporation = true);
logical(newEncoder = true);
int32(numberOfLayers = 0);
%int32(numberOfLayers = 0);
int32(newLayerAt = 0);
int32(initialStageAt = 0);
int32(iterations = 2);
int32(stages = 2);
%int32(iterations = 4);
%int32(stages = 4);

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
	    'stages'); 

clear -except keepVariables

% This is the encoder layer structure
int32(afferentArrayDimensionality = [10, 30]);
%int32(afferentArrayDimensionality = [5, 128]);
%int32(apicalArrayDimensionality = [1, 1]);
int32(apicalArrayDimensionality = [15, 15]);
int32(columnsArrayDimensionality = [15, 15]);

int32(afferentReceptiveField = [4, 14]);
%int32(afferentReceptiveField = [2, 63]);
afferentPercentage = 0.12;
%afferentPercentage = 0.05;
logical(afferentWrapAround = true);

int32(lateralDistalReceptiveField = [4, 4]);
lateralDistalPercentage = 0.9;
logical(lateralDistalWrapAround = true);

int32(apicalReceptiveField = [5, 5]);
%int32(apicalReceptiveField = [-1, -1]);
apicalPercentage = 0.9;
logical(apicalWrapAround = true);

int32(iterationNum = 0);

int32(populationsArrayDimensionality = [15, 15]);
int32(apicalPopulationsArrayDimensionality = [15,15]);
%int32(apicalPopulationsArrayDimensionality = [15, 15]);
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
	    'potentialPercentage');

clear -except keepVariables

% These are the encoder layer parameters 
logical(enableLearning = false);
logical(distalSensitivity = false);
proximalInformationThreshold = 0.1;
distalInformationThreshold = 0.5;
activationRadius = 0.8;
sparsity = 0.99;
logical(enableProximalLearning = false);
logical(enableDistalLearning = false);
proximalLearningRate = 0.001;
proximalNeighborhood = 0.005;
logical(spikeTimeDependentSynapticPlasticity = true);
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
	    'limitsLearningRate');

clear -except keepVariables














%{
% This is the regular layer structure number 0
int32(afferentArrayDimensionality = [15, 15]);
%int32(apicalArrayDimensionality = [1, 1]);
int32(apicalArrayDimensionality = [15, 15]);
int32(columnsArrayDimensionality = [15, 15]);

int32(afferentReceptiveField = [4, 4]);
afferentPercentage = 0.025*4*4;
%afferentPercentage = 0.9;
logical(afferentWrapAround = true);

%int32(lateralProximalReceptiveField = [4, 4]);
int32(lateralProximalReceptiveField = [-1, -1]);
lateralProximalPercentage = 0.8;
%lateralProximalPercentage = 0.8;
logical(lateralProximalWrapAround = true);

int32(lateralDistalReceptiveField = [4, 4]);
lateralDistalPercentage = 0.9;
logical(lateralDistalWrapAround = true);

int32(apicalReceptiveField = [5, 5]);
%int32(apicalReceptiveField = [-1, -1]);
apicalPercentage = 0.9;
logical(apicalWrapAround = true);

int32(iterationNum = 0);

int32(populationsArrayDimensionality = [15,15]);
%int32(populationsArrayDimensionality = [15, 15]);
int32(afferentPopulationsArrayDimensionality = [15, 15]);
int32(apicalPopulationsArrayDimensionality = [15, 15]);

int32(temporalGatheringAfferentValue = 1);
proximalPotentialPercentage = 0.0045;
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
	    'distalPotentialPercentage');

clear -except keepVariables

% These are the regular layer parameters number 0
logical(enableLearning = false);
logical(distalSensitivity = false);
logical(activationHomeostasis = false);
proximalInformationThreshold = 0.5;
distalInformationThreshold = 0.5;
sparsity = 0.99;
logical(enableProximalLearning = false);
logical(enableDistalLearning = false);
logical(synapticHomeostasis = false);
proximalLearningRate = 0.001;
proximalNeighborhood = 0.005;
plasticity = 0.1; %0.01;
logical(spikeTimeDependentSynapticPlasticity = true);
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
	    'distalLearningRate');

clear -except keepVariables

















% This is the regular layer structure number 1
int32(afferentArrayDimensionality = [15, 15]);
%int32(apicalArrayDimensionality = [15, 15]);
int32(apicalArrayDimensionality = [1, 1]);
int32(columnsArrayDimensionality = [15, 15]);

int32(afferentReceptiveField = [4, 4]);
afferentPercentage = 0.025*2;
%afferentPercentage = 0.9;
logical(afferentWrapAround = true);

int32(lateralProximalReceptiveField = [-1, -1]);
%int32(lateralProximalReceptiveField = [4, 4]);
lateralProximalPercentage = 0.5;
logical(lateralProximalWrapAround = true);

int32(lateralDistalReceptiveField = [4, 4]);
lateralDistalPercentage = 0.9;
logical(lateralDistalWrapAround = true);

%int32(apicalReceptiveField = [4, 4]);
int32(apicalReceptiveField = [-1, -1]);
apicalPercentage = 0.9;
logical(apicalWrapAround = true);

int32(iterationNum = 0);

int32(populationsArrayDimensionality = [15, 15]);
int32(afferentPopulationsArrayDimensionality = [15, 15]);
int32(apicalPopulationsArrayDimensionality = [15, 15]);

int32(temporalGatheringAfferentValue = 2);
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
	    'distalPotentialPercentage');

clear -except keepVariables

% These are the regular layer parameters number 1
logical(enableLearning = false);
logical(distalSensitivity = false);
logical(activationHomeostasis = false);
proximalInformationThreshold = 0.5;
distalInformationThreshold = 0.5;
sparsity = 0.99;
logical(enableProximalLearning = false);
logical(enableDistalLearning = false);
logical(synapticHomeostasis = false);
proximalLearningRate = 0.001;
proximalNeighborhood = 0.005;
plasticity = 0.1; %0.01;
logical(spikeTimeDependentSynapticPlasticity = true);
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
	 'distalLearningRate');














% This is the regular layer structure number 2
afferentArrayDimensionality = [3, 3];
apicalArrayDimensionality = [1, 1];
columnsArrayDimensionality = [3, 3];

afferentReceptiveField = [1, 1];
afferentPercentage = 0.9;
afferentWrapAround = 1;

lateralProximalReceptiveField = [1, 1];
lateralProximalPercentage = 0.5;
lateralProximalWrapAround = 1;

lateralDistalReceptiveField = [1, 1];
lateralDistalPercentage = 0.9;
lateralDistalWrapAround = 1;

apicalReceptiveField = [-1, -1];
apicalPercentage = 0.9;
apicalWrapAround = 1;

iterationNum = 0;

populationsArrayDimensionality = [15, 15];
afferentPopulationsArrayDimensionality = [15, 15];
apicalPopulationsArrayDimensionality = [15, 15];

temporalGatheringAfferentValue = 1;
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
	    'distalPotentialPercentage');

clear -except keepVariables

% These are the regular layer parameters number 2
enableLearning = 0;
distalSensitivity = 0;
activationHomeostasis = 0;
proximalInformationThreshold = 0.5;
distalInformationThreshold = 0.5;
sparsity = 0.99;
enableProximalLearning = 0;
enableDistalLearning = 0;
synapticHomeostasis = 0;
proximalLearningRate = 0.001;
proximalNeighborhood = 0.005;
plasticity = 0.1; %0.01;
spikeTimeDependentSynapticPlasticity = 1;
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
	 'distalLearningRate');


%}

end

