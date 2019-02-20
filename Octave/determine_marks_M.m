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

% File Name:		determine_marks_M.m
% Language:		GNU Octave high-level interpreted language.

% Determines the marks at the pauses between words in the corpus
function [start_marks, end_marks] = determine_marks_M(inputs, wordsSequence)

	%inputs(inputs<0.00000133)=0;
	activity = any(inputs');	% holds the activity in every time step

	gap = 25;			% this is the necessary gap as to consider the end of the last word
	end_marks = 1;			% this is the vector which holds the marks at the end of words
	start_marks = 1;		% this is the vector which holds the marks at the beginning of words
	timeStep = 1;			%
	timeSteps = size(inputs,1);	% the total number of time steps in the corpus
	condition = true;
	beginning = true;
	while	condition
		% if there is activity in the next gap of time steps
		if ((timeStep+gap) < timeSteps && any(activity(1,timeStep:timeStep+gap)))
			% if it is the beginning of the corpus
			if (beginning)
				% puts a mark at the beginning of the first word
				start_marks = [start_marks, 1];
				beginning = false;
			end
			% goes to the end of the word
			% through a fine step
			timeStep = timeStep + 1;
			while ((timeStep+gap) < timeSteps && any(activity(1,timeStep:timeStep+gap)))
				timeStep = timeStep + 1;
			end
			end_marks = [end_marks, timeStep]; % produces a new end mark
		% if there is no activity in the next gap of time steps
		else
			% if it is the beginning of the corpus
			if (beginning)
				% goes to the beginning of the next word
				beginning = false;
				timeStep = timeStep + gap;
				while ((timeStep+gap) < timeSteps && ~any(activity(1,timeStep:timeStep+gap)))
					timeStep = timeStep + gap;
				end
			% if it is not the beginning of the corpus
			else
				% goes to the beginning of the next word
				% first, through a coarse step
				timeStep = timeStep + gap;
				while ((timeStep+gap) < timeSteps && ~any(activity(1,timeStep:timeStep+gap)))
					timeStep = timeStep + gap;
				end
				% then, through a fine step
				while ((timeStep+1) < timeSteps && ~any(activity(1,timeStep:timeStep+1)))
					timeStep = timeStep + 1;
				end
				if ( (timeStep+1) < timeSteps )
					start_marks = [start_marks, timeStep]; % produces a new start mark
				end
			end
		end
		% check if it is at the end of the corpus
		if ((timeStep + gap) >= timeSteps)
			condition = false;
		end
	end

	start_marks = start_marks(2:end);
	end_marks = end_marks(2:end);

	% check the integrity of the marks.
	% there must be the same number of marks than words in the corpus
	if ( (size(start_marks,2) ~= size(wordsSequence,2)) || (size(end_marks,2) ~= size(wordsSequence,2)) )
		string = ['We have ' num2str(size(start_marks,2)) ' start_marks\n'];
		disp(string);
		string = ['We have ' num2str(size(end_marks,2)) ' end_marks\n'];
		disp(string);
		string = ['On the other hand, we have ' num2str(size(wordsSequence,2)) ' words in the corpus.\n'];
		disp(string);
		error('This is an error produced in determine_marks_M function.\n');
	end
end
