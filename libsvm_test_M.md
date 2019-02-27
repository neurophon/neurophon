```
% File Name:		libsvm_test_M.m
% Language:		GNU Octave high-level interpreted language.

function libsvm_test_M(finishProcessing)

% load the files needed to feed libsvm
cd ~/neurophon/Octave
load inputs_libsvm.mat
inputs_scaled = scaled;
if ( finishProcessing >= 1 )
	load outputs_EL_libsvm.mat
	outputs_scaled_EL = scaled;
end
if ( finishProcessing >= 2 )
	load outputs_RL_0_libsvm.mat
	outputs_scaled_RL_0 = scaled;
end
if ( finishProcessing >= 3 )
	load outputs_RL_1_libsvm.mat
	outputs_scaled_RL_1 = scaled;
end
if ( finishProcessing >= 4 )
	load outputs_RL_2_libsvm.mat
	outputs_scaled_RL_2 = scaled;
end
clear scaled
cd ~/libsvm/matlab

testing_label_vector = wordsSequence';
%libsvm_options = '-b 1'; % probability stimates
libsvm_options = '';

% test libsvm for inputs_libsvm
cd MyInterface
load inputs_model.mat
cd ..
model = best_model;
testing_instance_matrix = inputs_scaled;
[predicted_label, accuracy, prob_estimates] = svmpredict(testing_label_vector, testing_instance_matrix, model, [libsvm_options]);
disp('For the inputs to the model we have:');
disp('accuracy');
disp(accuracy);
cd MyInterface
save inputs_test.mat predicted_label accuracy prob_estimates
cd ..

if ( finishProcessing >= 1 )
	% test libsvm for outputs_EL_libsvm
	cd MyInterface
	load outputs_EL_model.mat
	cd ..
	model = best_model;
	testing_instance_matrix = outputs_scaled_EL;
	[predicted_label, accuracy, prob_estimates] = svmpredict(testing_label_vector, testing_instance_matrix, model, [libsvm_options]);
	disp('For the outputs from EL of the model we have:');
	disp('accuracy');
	disp(accuracy);
	cd MyInterface
	save outputs_EL_test.mat predicted_label accuracy prob_estimates
	cd ..
end

if ( finishProcessing >= 2 )
	% test libsvm for outputs_RL_0_libsvm
	cd MyInterface
	load outputs_RL_0_model.mat
	cd ..
	model = best_model;
	testing_instance_matrix = outputs_scaled_RL_0;
	[predicted_label, accuracy, prob_estimates] = svmpredict(testing_label_vector, testing_instance_matrix, model, [libsvm_options]);
	disp('For the outputs from RL_0 of the model we have:');
	disp('accuracy');
	disp(accuracy);
	cd MyInterface
	save outputs_RL_0_test.mat predicted_label accuracy prob_estimates
	cd ..
end

if ( finishProcessing >= 3 )
	% test libsvm for outputs_RL_1_libsvm
	cd MyInterface
	load outputs_RL_1_model.mat
	cd ..
	model = best_model;
	testing_instance_matrix = outputs_scaled_RL_1;
	[predicted_label, accuracy, prob_estimates] = svmpredict(testing_label_vector, testing_instance_matrix, model, [libsvm_options]);
	disp('For the outputs from RL_1 of the model we have:');
	disp('accuracy');
	disp(accuracy);
	cd MyInterface
	save outputs_RL_1_test.mat predicted_label accuracy prob_estimates
	cd ..
end

if ( finishProcessing >= 4 )
	% test libsvm for outputs_RL_2_libsvm
	cd MyInterface
	load outputs_RL_2_model.mat
	cd ..
	model = best_model;
	testing_instance_matrix = outputs_scaled_RL_2;
	[predicted_label, accuracy, prob_estimates] = svmpredict(testing_label_vector, testing_instance_matrix, model, [libsvm_options]);
	disp('For the outputs from RL_2 of the model we have:');
	disp('accuracy');
	disp(accuracy);
	cd MyInterface
	save outputs_RL_2_test.mat predicted_label accuracy prob_estimates
	cd ..
end



cd MyInterface

end
```
