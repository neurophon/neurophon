##################################################################################################################
##				Author:		Dematties Dario Jesus						##
##				Contact:	dariodematties@hotmail.com.ar					##
##						dariodematties@yahoo.com.ar					##
##						dario.dematties@frm.utn.edu.ar					##
##				Project:	Engineering PhD Project						##
##				Institution:	Universidad de Buenos Aires					##
##						Facultad de Ingeniería (FIUBA)					##
##				Workplace:	Instituto de Ingeniería						##
##						Biomédica FIUBA	&						##
##						CCT CONICET Mendoza INCIHUSA					##
##################################################################################################################

# File Name:		processes_data_for_train_supervision.m
# Language:		GNU Octave high-level interpreted language.

# This program processes the input to the model and outputs from every layer of the same
# in order to make it readable from the Support Vector Machine libsvm software

function processes_data_for_train_supervision(finishProcessingAt, start_marks, end_marks)

if ( (nargin != 1) && (nargin != 3) ) 
	error("processes_data_for_train_supervision: wrong number of arguments\n"); 
endif

# Loads the inputs that received the model
load inputs.mat

# Loads the sequence of words of the corpus that received the model as input
cd GenerateTextCorpora
load wordsSequence.mat
cd ..

if ( finishProcessingAt >= 1 )
	# Loads the output form hierarchy 1 layer 4 of the model
	load outputLayer_H1_L4.mat
	columnsArrayDimensionality_H1_L4 = columnsArrayDimensionality;
	populationsArrayDimensionality_H1_L4 = populationsArrayDimensionality;
	layerOutput_H1_L4 = layerOutput;
endif

if ( finishProcessingAt >= 2 )
	# Loads the output form hierarchy 1 layer 23 of the model
	load outputLayer_H1_L23.mat
	columnsArrayDimensionality_H1_L23 = columnsArrayDimensionality;
	populationsArrayDimensionality_H1_L23 = populationsArrayDimensionality;
	layerOutput_H1_L23 = layerOutput;
endif

if ( finishProcessingAt >= 3 )
	# Loads the output form hierarchy 2 layer 4 of the model
	load outputLayer_H2_L4.mat
	columnsArrayDimensionality_H2_L4 = columnsArrayDimensionality;
	populationsArrayDimensionality_H2_L4 = populationsArrayDimensionality;
	layerOutput_H2_L4 = layerOutput;
endif

if ( finishProcessingAt == 1 )
	# Checks if all the arrays have the same number of time steps
	if ( rows(inputs) != rows(layerOutput_H1_L4) )
		string = "Input data inconsistence in script processes_data_for_supervision: different time steps\n";
		string = [string, "input and outputs from the model have to have the same time steps"];
		error(string);
	endif
endif

if ( finishProcessingAt == 2 )
	# Checks if all the arrays have the same number of time steps
	if ( rows(inputs) != rows(layerOutput_H1_L4) || ...
	     rows(inputs) != rows(layerOutput_H1_L23) )
		string = "Input data inconsistence in script processes_data_for_supervision: different time steps\n";
		string = [string, "input and outputs from the model have to have the same time steps"];
		error(string);
	endif
endif

if ( finishProcessingAt == 3 )
	# Checks if all the arrays have the same number of time steps
	if ( rows(inputs) != rows(layerOutput_H1_L4) || ...
	     rows(inputs) != rows(layerOutput_H1_L23) || ...
	     rows(inputs) != rows(layerOutput_H2_L4) )
		string = "Input data inconsistence in script processes_data_for_supervision: different time steps\n";
		string = [string, "input and outputs from the model have to have the same time steps"];
		error(string);
	endif
endif

if ( nargin == 1 )
	# Computes the marks corresponding to the input
	# word's pauses
	[start_marks, end_marks] = determine_marks(inputs,wordsSequence);
	save marks.mat start_marks end_marks
endif

# Processes the data and save the files
inputs_libsvm = inputs_to_libsvm(inputs,wordsSequence,start_marks,end_marks);
inputs_libsvm = scale_for_libsvm(inputs_libsvm,-1,1);
scaled = inputs_libsvm.scaled;
save inputs_libsvm.mat	scaled wordsSequence
slopes = inputs_libsvm.slopes;
intercept = inputs_libsvm.intercept;
save inputs_scale_parameters_libsvm.mat	slopes intercept


if ( finishProcessingAt >= 1 )
	outputs_libsvm = outputs_to_libsvm(layerOutput_H1_L4,wordsSequence,start_marks,end_marks, ...
				   populationsArrayDimensionality_H1_L4, ...
				   columnsArrayDimensionality_H1_L4 );
	outputs_libsvm = scale_for_libsvm(outputs_libsvm,-1,1);
	scaled = outputs_libsvm.scaled;
	save outputs_H1_L4_libsvm.mat	scaled wordsSequence
	slopes = outputs_libsvm.slopes;
	intercept = outputs_libsvm.intercept;
	save outputs_H1_L4_scale_parameters_libsvm.mat	slopes intercept
endif

if ( finishProcessingAt >= 2 )
	outputs_libsvm = outputs_to_libsvm(layerOutput_H1_L23,wordsSequence,start_marks,end_marks, ...
					   populationsArrayDimensionality_H1_L23, ...
					   columnsArrayDimensionality_H1_L23 );
	outputs_libsvm = scale_for_libsvm(outputs_libsvm,-1,1);
	scaled = outputs_libsvm.scaled;
	save outputs_H1_L23_libsvm.mat	scaled wordsSequence
	slopes = outputs_libsvm.slopes;
	intercept = outputs_libsvm.intercept;
	save outputs_H1_L23_scale_parameters_libsvm.mat	slopes intercept
endif

if ( finishProcessingAt >= 3 )
	outputs_libsvm = outputs_to_libsvm(layerOutput_H2_L4,wordsSequence,start_marks,end_marks, ...
					   populationsArrayDimensionality_H2_L4, ...
					   columnsArrayDimensionality_H2_L4 );
	outputs_libsvm = scale_for_libsvm(outputs_libsvm,-1,1);
	scaled = outputs_libsvm.scaled;
	save outputs_H2_L4_libsvm.mat	scaled wordsSequence
	slopes = outputs_libsvm.slopes;
	intercept = outputs_libsvm.intercept;
	save outputs_H2_L4_scale_parameters_libsvm.mat	slopes intercept
endif

##{
average = mean(inputs');
start_auxiliary = zeros(1,1:rows(inputs));
start_auxiliary(start_marks) = 0.2;
end_auxiliary = zeros(1,1:rows(inputs));
end_auxiliary(end_marks) = 0.2;
plot(1:columns(average),average(1,1:columns(average)));
hold on
plot(1:columns(start_auxiliary),start_auxiliary);
plot(1:columns(end_auxiliary),end_auxiliary);
hold off
#}

endfunction
