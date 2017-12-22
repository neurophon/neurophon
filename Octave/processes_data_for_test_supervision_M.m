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

% File Name:		processes_data_for_test_supervision_M.m
% Language:		GNU Octave high-level interpreted language.

% This program processes the input to the model and outputs from every layer of the same
% in order to make it readable from the Support Vector Machine libsvm software

function processes_data_for_test_supervision_M(finishProcessingAt, start_marks, end_marks)

if ( (nargin ~= 1) && (nargin ~= 3) ) 
	error('processes_data_for_test_supervision_M: wrong number of arguments\n'); 
end


% Loads the inputs that received the model
load inputs.mat
load inputs_scale_parameters_libsvm.mat
inputs_slopes = slopes;
inputs_intercept = intercept;

% Loads the sequence of words of the corpus that received the model as input
cd GenerateTextCorpora
load wordsSequence.mat
cd ..

if ( finishProcessingAt >= 1 )
	% Loads the output form hierarchy 1 layer 4 of the model
	load EncoderLayerOutput.mat
	columnsArrayDimensionality_EL = columnsArrayDimensionality;
	populationsArrayDimensionality_EL = populationsArrayDimensionality;
	layerOutput_EL = encoderLayerOutput;
	load outputs_EL_scale_parameters_libsvm.mat
	outputs_EL_slopes = slopes;
	outputs_EL_intercept = intercept;
end

if ( finishProcessingAt >= 2 )
	% Loads the output form hierarchy 1 layer 23 of the model
	load RegularLayerOutput_0.mat
	columnsArrayDimensionality_RL_0 = columnsArrayDimensionality;
	populationsArrayDimensionality_RL_0 = populationsArrayDimensionality;
	layerOutput_RL_0 = regularLayerOutput;
	load outputs_RL_0_scale_parameters_libsvm.mat
	outputs_RL_0_slopes = slopes;
	outputs_RL_0_intercept = intercept;
end

if ( finishProcessingAt >= 3 )
	% Loads the output form hierarchy 2 layer 4 of the model
	load RegularLayerOutput_1.mat
	columnsArrayDimensionality_RL_1 = columnsArrayDimensionality;
	populationsArrayDimensionality_RL_1 = populationsArrayDimensionality;
	layerOutput_RL_1 = regularLayerOutput;
	load outputs_RL_1_scale_parameters_libsvm.mat
	outputs_RL_1_slopes = slopes;
	outputs_RL_1_intercept = intercept;
end

if ( finishProcessingAt == 1 )
	% Checks if all the arrays have the same number of time steps
	if ( size(inputs,1) ~= size(layerOutput_EL,1) )
		string = ['Input data inconsistence in script processes_data_for_supervision: different time steps\n'];
		string = [string 'input and outputs from the model have to have the same time steps'];
		error(string);
	end
end

if ( finishProcessingAt == 2 )
	% Checks if all the arrays have the same number of time steps
	if ( size(inputs,1) ~= size(layerOutput_EL,1) || ...
	     size(inputs,1) ~= size(layerOutput_RL_0,1) )
		string = ['Input data inconsistence in script processes_data_for_supervision: different time steps\n'];
		string = [string 'input and outputs from the model have to have the same time steps'];
		error(string);
	end
end

if ( finishProcessingAt == 3 )
	% Checks if all the arrays have the same number of time steps
	if ( size(inputs,1) ~= size(layerOutput_EL,1) || ...
	     size(inputs,1) ~= size(layerOutput_RL_0,1) || ...
	     size(inputs,1) ~= size(layerOutput_RL_1,1) )
		string = 'Input data inconsistence in script processes_data_for_supervision: different time steps\n';
		string = string + 'input and outputs from the model have to have the same time steps';
		error(string);
	end
end

if ( nargin ~= 3 ) 
	% Computes the marks corresponding to the input
	% word's pauses
	[start_marks, end_marks] = determine_marks_M(inputs,wordsSequence);
	save marks.mat start_marks end_marks
end

% Processes the data and save the files

% this is for the inputs to the model
inputs_libsvm = inputs_to_libsvm_M(inputs,wordsSequence,start_marks,end_marks);
inputs_scale_parameters = struct('slopes', inputs_slopes, 'intercept', inputs_intercept);
scaled = scale_for_libsvm_M(inputs_libsvm,inputs_scale_parameters);
save inputs_libsvm.mat	scaled wordsSequence

if ( finishProcessingAt >= 1 )
	% this is for the outputs from layer 4 in hierarchy 1 of the model
	outputs_libsvm = outputs_to_libsvm_M(layerOutput_EL,wordsSequence,start_marks,end_marks, ...
					   populationsArrayDimensionality_EL, ...
					   columnsArrayDimensionality_EL );
	outputs_EL_scale_parameters = struct('slopes', outputs_EL_slopes, 'intercept', outputs_EL_intercept);
	scaled = scale_for_libsvm_M(outputs_libsvm,outputs_EL_scale_parameters);
	save outputs_EL_libsvm.mat scaled wordsSequence
end

if ( finishProcessingAt >= 2 )
	% this is for the outputs from layer 23 in hierarchy 1 of the model
	outputs_libsvm = outputs_to_libsvm_M(layerOutput_RL_0,wordsSequence,start_marks,end_marks, ...
					   populationsArrayDimensionality_RL_0, ...
					   columnsArrayDimensionality_RL_0 );
	outputs_RL_0_scale_parameters = struct('slopes', outputs_RL_0_slopes, 'intercept', outputs_RL_0_intercept);
	scaled = scale_for_libsvm_M(outputs_libsvm,outputs_RL_0_scale_parameters);
	save outputs_RL_0_libsvm.mat scaled wordsSequence
end

if ( finishProcessingAt >= 3 )
	% this is for the outputs from layer 4 in hierarchy 2 of the model
	outputs_libsvm = outputs_to_libsvm_M(layerOutput_RL_1,wordsSequence,start_marks,end_marks, ...
					   populationsArrayDimensionality_RL_1, ...
					   columnsArrayDimensionality_RL_1 );
	outputs_RL_1_scale_parameters = struct('slopes', outputs_RL_1_slopes, 'intercept', outputs_RL_1_intercept);
	scaled = scale_for_libsvm_M(outputs_libsvm,outputs_RL_1_scale_parameters);
	save outputs_RL_1_libsvm.mat scaled wordsSequence
end

%{
average = mean(inputs');
start_auxiliary = zeros(1,1:size(inputs,1));
start_auxiliary(start_marks) = 0.2;
end_auxiliary = zeros(1,1:size(inputs,1));
end_auxiliary(end_marks) = 0.2;
plot(1:size(average,2),average(1,1:size(average,2)));
hold on
plot(1:size(start_auxiliary,2),start_auxiliary);
plot(1:size(end_auxiliary,2),end_auxiliary);
hold off
%}

end
