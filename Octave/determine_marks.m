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

# File Name:		determine_marks.m
# Language:		GNU Octave high-level interpreted language.

# Determines the marks at the pauses between words in the corpus
function [start_marks, end_marks] = determine_marks(inputs, wordsSequence)

	activity = any(inputs');	# holds the activity in every time step

	gap = 25;			# this is the necessary gap as to consider the end of the last word
	end_marks = 1;			# this is the vector which holds the marks at the end of words
	start_marks = 1;		# this is the vector which holds the marks at the beginning of words
	timeStep = 1;			#
	timeSteps = rows(inputs);	# the total number of time steps in the corpus
	condition = true;
	beginning = true;
	while	condition
		# if there is activity in the next gap of time steps
		if ((timeStep+gap) < timeSteps && any(activity(1,timeStep:timeStep+gap)))
			# if it is the beginning of the corpus
			if (beginning)
				# puts a mark at the beginning of the first word
				start_marks = [start_marks, 1];
				beginning = false;
			endif
			# goes to the end of the word
			# through a fine step
			timeStep += 1;
			while ((timeStep+gap) < timeSteps && any(activity(1,timeStep:timeStep+gap)))
				timeStep += 1;
			endwhile
			end_marks = [end_marks, timeStep]; # produces a new end mark
		# if there is no activity in the next gap of time steps
		else
			# if it is the beginning of the corpus
			if (beginning)
				# goes to the beginning of the next word
				beginning = false;
				timeStep += gap;
				while ((timeStep+gap) < timeSteps && !any(activity(1,timeStep:timeStep+gap)))
					timeStep += gap;
				endwhile
			# if it is not the beginning of the corpus
			else
				# goes to the beginning of the next word
				# first, through a coarse step
				timeStep += gap;
				while ((timeStep+gap) < timeSteps && !any(activity(1,timeStep:timeStep+gap)))
					timeStep += gap;
				endwhile
				# then, through a fine step
				while ((timeStep+1) < timeSteps && !any(activity(1,timeStep:timeStep+1)))
					timeStep += 1;
				endwhile
				if ( (timeStep+1) < timeSteps )
					start_marks = [start_marks, timeStep]; # produces a new start mark
				endif
			endif
		endif
		# check if it is at the end of the corpus
		if ((timeStep + gap) >= timeSteps)
			condition = false;
		endif
	endwhile

	start_marks = start_marks(2:end);
	end_marks = end_marks(2:end);

	# check the integrity of the marks.
	# there must be the same number of marks than words in the corpus
	if ( (columns(start_marks) != columns(wordsSequence)) || (columns(end_marks) != columns(wordsSequence)) )
		string = ["We have " num2str(columns(start_marks)) " start_marks\n"];
		disp(string);
		string = ["We have " num2str(columns(end_marks)) " end_marks\n"];
		disp(string);
		string = ["On the other hand, we have " num2str(columns(wordsSequence)) " words in the corpus.\n"];
		disp(string);
		error("This is an error produced in determine_marks function.\n");
	endif
endfunction
