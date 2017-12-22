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

% File Name:		outputs_to_libsvm_M.m
% Language:		GNU Octave high-level interpreted language.

% Adapts layerOutput from the model in order to feed
% libsvm software with it
function	outputs_libsvm = outputs_to_libsvm_M(layerOutput,wordsSequence,start_marks,end_marks, ...
						   populationsArrayDimensionality, ...
						   columnsArrayDimensionality )
	if ( size(start_marks,2) ~= size(end_marks,2) )
		string = ['Different number of start and end marks.\n'];
		disp(string);
		error('This is an error produced in outputs_to_libsvm_M function.\n');
	end
	% this is the number of units in the population (cortical column)
	numberOfUnits = prod(populationsArrayDimensionality);
	% this is the number of cortical columns in the layer
	numberOfColumns = prod(columnsArrayDimensionality);
	% computes the histograms between marks over time steps on outputs
	% this process ends up with a histogram corresponding to every word in wordsSequence

	% this is the function's output's first column
	% every element in this vector corresponds to
	% the activations count of a determine unit
	% inside a determined population (cortical column) in the layer
	% every column in this vector corresponds to certain
	% word in the corpus
	outputs_libsvm = [];
	histograms = [];
	auxiliary = [];
	for idx = 1:size(end_marks,2)
		for column = 1:numberOfColumns
			for i = start_marks(1,idx):end_marks(1,idx)
				% puts together all the model's outputs corresponding
				% to the time steps in the current word
				% in this column
				auxiliary = [auxiliary, layerOutput{i,1}{column,1}];
			end
			% computes the histogram corresponding with the current word
			histograms = [histograms, hist(auxiliary,-1:numberOfUnits-1)'];
			% reinitializes auxiliary so it does not accumulate
			% data from the previous column
			% nor previous word
			auxiliary = [];
		end
		histograms = histograms(2:end,:);
		% checks data integrity
		if ( size(histograms,1) ~= numberOfUnits || ...
		     size(histograms,2) ~= numberOfColumns )
			string = ['We have ' num2str(numberOfUnits) ' numberOfUnits.\n'];
			disp(string);
			string = ['Also, we have ' num2str(numberOfColumns) ' numberOfColumns.\n'];
			disp(string);
			string = ['On the other hand, we have ' num2str(size(histograms,1)) ' rows in our histograms.\n'];
			disp(string);
			string = ['And, we have ' num2str(size(histograms,2)) ' columns in our histograms.\n'];
			disp(string);
			error('This is an error produced in outputs_to_libsvm_M function.\n');
		end
		outputs_libsvm = [outputs_libsvm; reshape(histograms,1,numberOfUnits*numberOfColumns)];
		% reinitializes histograms so it does not accumulate
		% data from the previous word
		histograms = [];
	end

	% checks the integrity of the data
	if ( 	size(outputs_libsvm,1) ~= size(start_marks,2) || ...
		size(outputs_libsvm,1) ~= size(end_marks,2) || ...
		size(outputs_libsvm,1) ~= size(wordsSequence,2) )
		string = ['We have ' num2str(size(start_marks,2)) ' start_marks\n'];
		disp(string);
		string = ['We have ' num2str(size(end_marks,2)) ' end_marks\n'];
		disp(string);
		string = ['Also, we have ' num2str(size(wordsSequence,2)) ' words in wordsSequence.\n'];
		disp(string);
		string = ['On the other hand, we have ' num2str(size(outputs_libsvm,1)) ' averages in outputs_libsvm.\n'];
		disp(string);
		error('This is an error produced in outputs_to_libsvm_M function.\n');
	end
end
