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

# File Name:		inputs_to_libsvm.m
# Language:		GNU Octave high-level interpreted language.

# Adapts the inputs to the model in order to feed
# libsvm software with them
function	inputs_libsvm = inputs_to_libsvm(inputs,wordsSequence,start_marks,end_marks)
	if ( columns(start_marks) != columns(end_marks) )
		string = ["Different number of start and end marks.\n"];
		disp(string);
		error("This is an error produced in inputs_to_libsvm function.\n");
	endif
	# computes the mean between marks over time steps on inputs
	# this process ends up with a mean corresponding to every word in wordsSequence
	inputs_libsvm = zeros(1,columns(inputs));
	for idx = 1:columns(end_marks)
		inputs_libsvm = [inputs_libsvm; sum(inputs(start_marks(1,idx):end_marks(1,idx),:))];
	endfor
	inputs_libsvm = inputs_libsvm(2:end,:);
	# checks the integrity of the data
	if ( rows(inputs_libsvm) != columns(start_marks) || ...
	     rows(inputs_libsvm) != columns(end_marks) || ...
	     rows(inputs_libsvm) != columns(wordsSequence) )
		string = ["We have " num2str(columns(start_marks)) " start_marks\n"];
		disp(string);
		string = ["We have " num2str(columns(end_marks)) " end_marks\n"];
		disp(string);
		string = ["Also, we have " num2str(columns(wordsSequence)) " words in wordsSequence.\n"];
		disp(string);
		string = ["On the other hand, we have " num2str(rows(inputs_libsvm)) " averages in inputs_libsvm.\n"];
		disp(string);
		error("This is an error produced in inputs_to_libsvm function.\n");
	endif
endfunction
