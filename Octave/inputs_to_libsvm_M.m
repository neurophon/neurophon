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

% File Name:		inputs_to_libsvm_M.m
% Language:		GNU Octave high-level interpreted language.

% Adapts the inputs to the model in order to feed
% libsvm software with them
function	inputs_libsvm = inputs_to_libsvm_M(inputs,wordsSequence,start_marks,end_marks)
	if ( size(start_marks,2) ~= size(end_marks,2) )
		string = ['Different number of start and end marks.\n'];
		disp(string);
		error('This is an error produced in inputs_to_libsvm_M function.\n');
	end
	% computes the mean between marks over time steps on inputs
	% this process ends up with a mean corresponding to every word in wordsSequence
	inputs_libsvm = zeros(1,size(inputs,2));
	for idx = 1:size(end_marks,2)
		inputs_libsvm = [inputs_libsvm; sum(inputs(start_marks(1,idx):end_marks(1,idx),:))];
	end
	inputs_libsvm = inputs_libsvm(2:end,:);
	% checks the integrity of the data
	if ( size(inputs_libsvm,1) ~= size(start_marks,2) || ...
	     size(inputs_libsvm,1) ~= size(end_marks,2) || ...
	     size(inputs_libsvm,1) ~= size(wordsSequence,2) )
		string = ['We have ' num2str(size(start_marks,2)) ' start_marks\n'];
		disp(string);
		string = ['We have ' num2str(size(end_marks,2)) ' end_marks\n'];
		disp(string);
		string = ['Also, we have ' num2str(size(wordsSequence,2)) ' words in wordsSequence.\n'];
		disp(string);
		string = ['On the other hand, we have ' num2str(size(inputs_libsvm,1)) ' averages in inputs_libsvm.\n'];
		disp(string);
		error('This is an error produced in inputs_to_libsvm_M function.\n');
	end
end
