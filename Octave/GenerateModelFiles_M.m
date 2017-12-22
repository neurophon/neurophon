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

% File Name:		GenerateModelFiles.m
% Language:		GNU Octave high-level interpreted language.

% This program generates a set of files which will be used by Model object class 
% in order togenerate its structure and configuration.

function GenerateModelFiles_M(folderName)

if ( !is_dq_string(folderName) && !is_sq_string(folderName) )
	error("folder name must be of type string");
endif

[status, msg, msgid] = mkdir (folderName);
if ( status != 1 )
	disp(msg);
	disp(msgid);
	error("in function GenerateModelFiles");
endif

clear -x folderName

% This is the model structure
encoderIncorporation = 1;
newEncoder = 1;
numberOfLayers = 2;
newLayerAt = 0;
initialStageAt = 0;
iterations = 2;
stages = 3;

% Saves the model structure
save ("-mat", ["./" folderName "/ModelStructure.mat"], "encoderIncorporation", ...
					       "newEncoder", ...
					       "numberOfLayers", ...
					       "newLayerAt", ...
					       "initialStageAt", ...
					       "iterations", ...
					       "stages"); 

clear -x folderName

% This is the encoder layer structure
afferentArrayDimensionality = [5, 128];
apicalArrayDimensionality = [5, 5];
columnsArrayDimensionality = [5, 5];

afferentReceptiveField = [2, 63];
afferentPercentage = 0.05;
afferentWrapAround = 1;

lateralDistalReceptiveField = [2, 2];
lateralDistalPercentage = 0.6*0.5;
lateralDistalWrapAround = 1;

apicalReceptiveField = [2, 2];
apicalPercentage = 0.6*0.5;
apicalWrapAround = 1;

iterationNum = 0;

populationsArrayDimensionality = [5, 5];
apicalPopulationsArrayDimensionality = [5, 5];
potentialPercentage = 0.1;

% Saves the encoder layer structure
save ("-mat", ["./" folderName "/EncoderLayerStructure.mat"], "afferentArrayDimensionality", ...
						      "apicalArrayDimensionality", ...
						      "columnsArrayDimensionality", ...
						      "afferentReceptiveField", ...
						      "afferentPercentage", ...
						      "afferentWrapAround", ...
						      "lateralDistalReceptiveField", ...
						      "lateralDistalPercentage", ...
						      "lateralDistalWrapAround", ...
						      "apicalReceptiveField", ...
						      "apicalPercentage", ...
						      "apicalWrapAround", ...
						      "iterationNum", ...
						      "populationsArrayDimensionality", ...
						      "apicalPopulationsArrayDimensionality", ...
						      "potentialPercentage");

clear -x folderName

% These are the encoder layer parameters 
enableLearning = 0;
distalSensitivity = 0;
proximalInformationThreshold = 0.1;
distalInformationThreshold = 0.5;
activationRadius = 0.8;
sparsity = 0.99;
enableProximalLearning = 0;
enableDistalLearning = 0;
proximalLearningRate = 0.001;
proximalNeighborhood = 0.005;
spikeTimeDependentSynapticPlasticity = 1;
distalLearningRate = 0.001;
limitsLearningRate = 0.1;

% Saves the encoder layer parameters
save ("-mat", ["./" folderName "/EncoderLayerParameters.mat"], "enableLearning", ...
						       "distalSensitivity", ...
						       "proximalInformationThreshold", ...
						       "distalInformationThreshold", ...
						       "activationRadius", ...
						       "sparsity", ...
						       "enableProximalLearning", ...
						       "enableDistalLearning", ...
						       "proximalLearningRate", ...
						       "proximalNeighborhood", ...
						       "spikeTimeDependentSynapticPlasticity", ...
						       "distalLearningRate", ...
						       "limitsLearningRate");

clear -x folderName

%%{
% This is the regular layer structure number 0
afferentArrayDimensionality = [5, 5];
apicalArrayDimensionality = [5, 5];
columnsArrayDimensionality = [5, 5];

afferentReceptiveField = [2, 2];
afferentPercentage = 0.6*0.5;
afferentWrapAround = 1;

lateralProximalReceptiveField = [2, 2];
lateralProximalPercentage = 0.5*0.5;
lateralProximalWrapAround = 1;

lateralDistalReceptiveField = [2, 2];
lateralDistalPercentage = 0.5*0.5;
lateralDistalWrapAround = 1;

apicalReceptiveField = [2, 2];
apicalPercentage = 0.5*0.5;
apicalWrapAround = 1;

iterationNum = 0;

populationsArrayDimensionality = [5, 5];
afferentPopulationsArrayDimensionality = [5, 5];
apicalPopulationsArrayDimensionality = [5, 5];

temporalGatheringAfferentValue = 1;
potentialPercentage = 0.1;

% Saves the regular layer structure number 0
save ("-mat", ["./" folderName "/RegularLayerStructure_0.mat"], "afferentArrayDimensionality", ...
						      "apicalArrayDimensionality", ...
						      "columnsArrayDimensionality", ...
						      "afferentReceptiveField", ...
						      "afferentPercentage", ...
						      "afferentWrapAround", ...
						      "lateralProximalReceptiveField", ...
						      "lateralProximalPercentage", ...
						      "lateralProximalWrapAround", ...
						      "lateralDistalReceptiveField", ...
						      "lateralDistalPercentage", ...
						      "lateralDistalWrapAround", ...
						      "apicalReceptiveField", ...
						      "apicalPercentage", ...
						      "apicalWrapAround", ...
						      "iterationNum", ...
						      "populationsArrayDimensionality", ...
						      "afferentPopulationsArrayDimensionality", ...
						      "apicalPopulationsArrayDimensionality", ...
						      "temporalGatheringAfferentValue", ...
						      "potentialPercentage");

clear -x folderName

% These are the regular layer parameters number 0
enableLearning = 0;
distalSensitivity = 0;
activationHomeostasis = 0;
proximalInformationThreshold = 0.5;
distalInformationThreshold = 0.5;
sparsity = 0.95; % 0.99;
enableProximalLearning = 0;
enableDistalLearning = 0;
synapticHomeostasis = 0;
proximalLearningRate = 0.001;
proximalNeighborhood = 0.005;
plasticity = 0.1; %0.01;
spikeTimeDependentSynapticPlasticity = 1;
distalLearningRate = 0.001;

% Saves the encoder layer parameters
save ("-mat", ["./" folderName "/RegularLayerParameters_0.mat"], "enableLearning", ...
						         "distalSensitivity", ...
						         "activationHomeostasis", ...
						         "proximalInformationThreshold", ...
						         "distalInformationThreshold", ...
						         "sparsity", ...
						         "enableProximalLearning", ...
						         "enableDistalLearning", ...
						         "synapticHomeostasis", ...
						         "proximalLearningRate", ...
						         "proximalNeighborhood", ...
						         "plasticity", ...
						         "spikeTimeDependentSynapticPlasticity", ...
						         "distalLearningRate");

clear -x folderName

%}
%%{
% This is the regular layer structure number 1
afferentArrayDimensionality = [5, 5];
apicalArrayDimensionality = [5, 5];
columnsArrayDimensionality = [5, 5];

afferentReceptiveField = [2, 2];
afferentPercentage = 0.6*0.5;
afferentWrapAround = 1;

lateralProximalReceptiveField = [2, 2];
lateralProximalPercentage = 0.5*0.5;
lateralProximalWrapAround = 1;

lateralDistalReceptiveField = [2, 2];
lateralDistalPercentage = 0.5*0.5;
lateralDistalWrapAround = 1;

apicalReceptiveField = [2, 2];
apicalPercentage = 0.5*0.5;
apicalWrapAround = 1;

iterationNum = 0;

populationsArrayDimensionality = [5, 5];
afferentPopulationsArrayDimensionality = [5, 5];
apicalPopulationsArrayDimensionality = [5, 5];

temporalGatheringAfferentValue = 1;
potentialPercentage = 0.1;

% Saves the regular layer structure number 1
save ("-mat", ["./" folderName "/RegularLayerStructure_1.mat"], "afferentArrayDimensionality", ...
						      "apicalArrayDimensionality", ...
						      "columnsArrayDimensionality", ...
						      "afferentReceptiveField", ...
						      "afferentPercentage", ...
						      "afferentWrapAround", ...
						      "lateralProximalReceptiveField", ...
						      "lateralProximalPercentage", ...
						      "lateralProximalWrapAround", ...
						      "lateralDistalReceptiveField", ...
						      "lateralDistalPercentage", ...
						      "lateralDistalWrapAround", ...
						      "apicalReceptiveField", ...
						      "apicalPercentage", ...
						      "apicalWrapAround", ...
						      "iterationNum", ...
						      "populationsArrayDimensionality", ...
						      "afferentPopulationsArrayDimensionality", ...
						      "apicalPopulationsArrayDimensionality", ...
						      "temporalGatheringAfferentValue", ...
						      "potentialPercentage");

clear -x folderName

% These are the regular layer parameters number 1
enableLearning = 0;
distalSensitivity = 0;
activationHomeostasis = 0;
proximalInformationThreshold = 0.5;
distalInformationThreshold = 0.5;
sparsity = 0.95; % 0.99;
enableProximalLearning = 0;
enableDistalLearning = 0;
synapticHomeostasis = 0;
proximalLearningRate = 0.001;
proximalNeighborhood = 0.005;
plasticity = 0.1; %0.01;
spikeTimeDependentSynapticPlasticity = 1;
distalLearningRate = 0.001;

% Saves the encoder layer parameters
save ("-mat", ["./" folderName "/RegularLayerParameters_1.mat"], "enableLearning", ...
						         "distalSensitivity", ...
						         "activationHomeostasis", ...
						         "proximalInformationThreshold", ...
						         "distalInformationThreshold", ...
						         "sparsity", ...
						         "enableProximalLearning", ...
						         "enableDistalLearning", ...
						         "synapticHomeostasis", ...
						         "proximalLearningRate", ...
						         "proximalNeighborhood", ...
						         "plasticity", ...
						         "spikeTimeDependentSynapticPlasticity", ...
						         "distalLearningRate");
%}

endfunction
