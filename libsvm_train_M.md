```
% File Name:		libsvm_train_M.m
% Language:		GNU Octave high-level interpreted language.

function libsvm_train_M(finishProcessing,kernel_type)

if ( nargin ~= 1 && nargin ~= 2 )
	error('Bad number of input arguments')
end

if ( nargin == 1 )
	kernel_type = 0;
end

if ( kernel_type ~= 0 && kernel_type ~= 2 )
	error('Bad kernel type')
end

% load the files needed to feed libsvm
cd ~/neurophon/Octave
load inputs_libsvm.mat
inputs_scaled = scaled;
if ( finishProcessing >= 1 )
	load outputs_EL_libsvm.mat
	outputs_scaled_libsvm_EL = scaled;
end
if ( finishProcessing >= 2 )
	load outputs_RL_0_libsvm.mat
	outputs_scaled_libsvm_RL_0 = scaled;
end
if ( finishProcessing >= 3 )
	load outputs_RL_1_libsvm.mat
	outputs_scaled_libsvm_RL_1 = scaled;
end
if ( finishProcessing >= 4 )
	load outputs_RL_2_libsvm.mat
	outputs_scaled_libsvm_RL_2 = scaled;
end
clear scaled
cd ~/libsvm/matlab

training_label_vector = wordsSequence';
if ( kernel_type == 2 )
	options = '-t 2 -v 5 -q';
else
	options = '-t 0 -v 5 -q';
end

% train libsvm for inputs_libsvm
best_model = 0;
best_cost = 1;
training_instance_matrix = inputs_scaled;

if ( kernel_type == 2 )
	% coarse training
	for c = -5:15
		for g = -15:3
			libsvm_options = [options ' -c ' num2str(2^c) ' -g ' num2str(2^g)];
			model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
			if (model > best_model)
				best_model = model;
				best_cost = c;
				best_gamma = g;
			end
		end
	end
	% fine-grained training
	costs = [best_cost-1:0.1:best_cost+1];
	gammas = [best_gamma-1:0.1:best_gamma+1];
	for c = costs
		for g = gammas
			libsvm_options = [options ' -c ' num2str(2^c) ' -g ' num2str(2^g)];
			model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
			if (model > best_model)
				best_model = model;
				best_cost = c;
				best_gamma = g;
			end
		end
	end
	disp('The best model for inputs gives Accuracy = ');
	disp(best_model);
	training_options = ['-t 2 -q -c ' num2str(2^best_cost) ' -g ' num2str(2^best_gamma)];
	best_model = svmtrain(training_label_vector, training_instance_matrix, [training_options]);
else
	% coarse training
	for c = -5:15
		libsvm_options = [options ' -c ' num2str(2^c)];
		model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
		if (model > best_model)
			best_model = model;
			best_cost = c;
		end
	end
	% fine-grained training
	costs = [best_cost-1:0.1:best_cost+1];
	for c = costs
		libsvm_options = [options ' -c ' num2str(2^c)];
		model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
		if (model > best_model)
			best_model = model;
			best_cost = c;
		end
	end
	disp('The best model for inputs gives Accuracy = ');
	disp(best_model);
	training_options = ['-t 0 -q -c ' num2str(2^best_cost)];
	best_model = svmtrain(training_label_vector, training_instance_matrix, [training_options]);
end



cd MyInterface
save inputs_model.mat best_model best_cost
cd ..

if ( finishProcessing >= 1 )
	if ( kernel_type == 2 )
		% train libsvm for outputs_EL_libsvm
		best_model = 0;
		best_cost = 1;
		training_instance_matrix = outputs_scaled_libsvm_EL;
		% coarse training
		for c = -5:15
			for g = -15:3
				libsvm_options = [options ' -c ' num2str(2^c) ' -g ' num2str(2^g)];
				model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
				if (model > best_model)
					best_model = model;
					best_cost = c;
					best_gamma = g;
				end
			end
		end
		% fine-grained training
		costs = [best_cost-1:0.1:best_cost+1];
		gammas = [best_gamma-1:0.1:best_gamma-1];
		for c = costs
			for g = gammas
				libsvm_options = [options ' -c ' num2str(2^c) ' -g ' num2str(2^g)];
				model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
				if (model > best_model)
					best_model = model;
					best_cost = c;
					best_gamma = g;
				end
			end
		end
		disp('The best model for outputs from EL gives Accuracy = ');
		disp(best_model);
		training_options = ['-t 0 -q -c ' num2str(2^best_cost) ' -g ' num2str(2^best_gamma)];
		best_model = svmtrain(training_label_vector, training_instance_matrix, [training_options]);
	else
		% train libsvm for outputs_EL_libsvm
		best_model = 0;
		best_cost = 1;
		training_instance_matrix = outputs_scaled_libsvm_EL;
		% coarse training
		for c = -5:15
			libsvm_options = [options ' -c ' num2str(2^c)];
			model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
			if (model > best_model)
				best_model = model;
				best_cost = c;
			end
		end
		% fine-grained training
		costs = [best_cost-1:0.1:best_cost+1];
		for c = costs
			libsvm_options = [options ' -c ' num2str(2^c)];
			model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
			if (model > best_model)
				best_model = model;
				best_cost = c;
			end
		end
		disp('The best model for outputs from EL gives Accuracy = ');
		disp(best_model);
		training_options = ['-t 0 -q -c ' num2str(2^best_cost)];
		best_model = svmtrain(training_label_vector, training_instance_matrix, [training_options]);
	end

	cd MyInterface
	save outputs_EL_model.mat best_model best_cost
	cd ..
end

if ( finishProcessing >= 2 )
	if ( kernel_type == 2 )
		% train libsvm for outputs_RL_0_libsvm
		best_model = 0;
		best_cost = 1;
		training_instance_matrix = outputs_scaled_libsvm_RL_0;
		% coarse training
		for c = -5:15
			for g = -15:3
				libsvm_options = [options ' -c ' num2str(2^c) ' -g ' num2str(2^g)];
				model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
				if (model > best_model)
					best_model = model;
					best_cost = c;
					best_gamma = g;
				end
			end
		end
		% fine-grained training
		costs = [best_cost-1:0.1:best_cost+1];
		gammas = [best_gamma-1:0.1:best_gamma-1];
		for c = costs
			for g = gammas
				libsvm_options = [options ' -c ' num2str(2^c) ' -g ' num2str(2^g)];
				model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
				if (model > best_model)
					best_model = model;
					best_cost = c;
					best_gamma = g;
				end
			end
		end
		disp('The best model for outputs from RL_0 gives Accuracy = ');
		disp(best_model);
		training_options = ['-t 0 -q -c ' num2str(2^best_cost) ' -g ' num2str(2^best_gamma)];
		best_model = svmtrain(training_label_vector, training_instance_matrix, [training_options]);
	else
		% train libsvm for outputs_RL_0_libsvm
		best_model = 0;
		best_cost = 1;
		training_instance_matrix = outputs_scaled_libsvm_RL_0;
		% coarse training
		for c = -5:15
			libsvm_options = [options ' -c ' num2str(2^c)];
			model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
			if (model > best_model)
				best_model = model;
				best_cost = c;
			end
		end
		% fine-grained training
		costs = [best_cost-1:0.1:best_cost+1];
		for c = costs
			libsvm_options = [options ' -c ' num2str(2^c)];
			model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
			if (model > best_model)
				best_model = model;
				best_cost = c;
			end
		end
		disp('The best model for outputs from RL_0 gives Accuracy = ');
		disp(best_model);
		training_options = ['-t 0 -q -c ' num2str(2^best_cost)];
		best_model = svmtrain(training_label_vector, training_instance_matrix, [training_options]);
	end

	cd MyInterface
	save outputs_RL_0_model.mat best_model best_cost
	cd ..
end

if ( finishProcessing >= 3 )
	if ( kernel_type == 2 )
		% train libsvm for outputs_RL_1_libsvm
		best_model = 0;
		best_cost = 1;
		training_instance_matrix = outputs_scaled_libsvm_RL_1;
		% coarse training
		for c = -5:15
			for g = -15:3
				libsvm_options = [options ' -c ' num2str(2^c) ' -g ' num2str(2^g)];
				model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
				if (model > best_model)
					best_model = model;
					best_cost = c;
					best_gamma = g;
				end
			end
		end
		% fine-grained training
		costs = [best_cost-1:0.1:best_cost+1];
		gammas = [best_gamma-1:0.1:best_gamma-1];
		for c = costs
			for g = gammas
				libsvm_options = [options ' -c ' num2str(2^c) ' -g ' num2str(2^g)];
				model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
				if (model > best_model)
					best_model = model;
					best_cost = c;
					best_gamma = g;
				end
			end
		end
		disp('The best model for outputs from RL_1 gives Accuracy = ');
		disp(best_model);
		training_options = ['-t 0 -q -c ' num2str(2^best_cost) ' -g ' num2str(2^best_gamma)];
		best_model = svmtrain(training_label_vector, training_instance_matrix, [training_options]);
	else
		% train libsvm for outputs_RL_1_libsvm
		best_model = 0;
		best_cost = 1;
		training_instance_matrix = outputs_scaled_libsvm_RL_1;
		% coarse training
		for c = -5:15
			libsvm_options = [options ' -c ' num2str(2^c)];
			model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
			if (model > best_model)
				best_model = model;
				best_cost = c;
			end
		end
		% fine-grained training
		costs = [best_cost-1:0.1:best_cost+1];
		for c = costs
			libsvm_options = [options ' -c ' num2str(2^c)];
			model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
			if (model > best_model)
				best_model = model;
				best_cost = c;
			end
		end
		disp('The best model for outputs from RL_1 gives Accuracy = ');
		disp(best_model);
		training_options = ['-t 0 -q -c ' num2str(2^best_cost)];
		best_model = svmtrain(training_label_vector, training_instance_matrix, [training_options]);
	end

	cd MyInterface
	save outputs_RL_1_model.mat best_model best_cost
	cd ..
end

if ( finishProcessing >= 4 )
	if ( kernel_type == 2 )
		% train libsvm for outputs_RL_2_libsvm
		best_model = 0;
		best_cost = 1;
		training_instance_matrix = outputs_scaled_libsvm_RL_2;
		% coarse training
		for c = -5:15
			for g = -15:3
				libsvm_options = [options ' -c ' num2str(2^c) ' -g ' num2str(2^g)];
				model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
				if (model > best_model)
					best_model = model;
					best_cost = c;
					best_gamma = g;
				end
			end
		end
		% fine-grained training
		costs = [best_cost-1:0.1:best_cost+1];
		gammas = [best_gamma-1:0.1:best_gamma-1];
		for c = costs
			for g = gammas
				libsvm_options = [options ' -c ' num2str(2^c) ' -g ' num2str(2^g)];
				model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
				if (model > best_model)
					best_model = model;
					best_cost = c;
					best_gamma = g;
				end
			end
		end
		disp('The best model for outputs from RL_2 gives Accuracy = ');
		disp(best_model);
		training_options = ['-t 0 -q -c ' num2str(2^best_cost) ' -g ' num2str(2^best_gamma)];
		best_model = svmtrain(training_label_vector, training_instance_matrix, [training_options]);
	else
		% train libsvm for outputs_RL_2_libsvm
		best_model = 0;
		best_cost = 1;
		training_instance_matrix = outputs_scaled_libsvm_RL_2;
		% coarse training
		for c = -5:15
			libsvm_options = [options ' -c ' num2str(2^c)];
			model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
			if (model > best_model)
				best_model = model;
				best_cost = c;
			end
		end
		% fine-grained training
		costs = [best_cost-1:0.1:best_cost+1];
		for c = costs
			libsvm_options = [options ' -c ' num2str(2^c)];
			model = svmtrain(training_label_vector, training_instance_matrix, [libsvm_options]);
			if (model > best_model)
				best_model = model;
				best_cost = c;
			end
		end
		disp('The best model for outputs from RL_2 gives Accuracy = ');
		disp(best_model);
		training_options = ['-t 0 -q -c ' num2str(2^best_cost)];
		best_model = svmtrain(training_label_vector, training_instance_matrix, [training_options]);
	end

	cd MyInterface
	save outputs_RL_2_model.mat best_model best_cost
	cd ..
end

cd MyInterface

end
```
