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

% File Name:		scale_for_libsvm_M.m
% Language:		GNU Octave high-level interpreted language.

% scales A matrix from the model in order to feed
% libsvm software with it
% If the arguments are A as the matrix to be scaled, arg_one as minimum and arg_two
% as maximum, the function returns a struct with the scaling data
% and the scaled matrix
% If the arguments are A as the matrix to be scaled and
% arg_one as the scaling data from past scaling activity,
% the function returns just the scaled matrix
function C = scale_for_libsvm_M(A,arg_one,arg_two)

	% check argument's errors
	if (length(size(A)) ~= 2)
		string = 'wrong A argument in scale_for_libsvm_M \n';
		string = string + 'A s dimensionality must be 2.\n';
		error(string);
	end
	if (~isa(A,'double'))
		string = 'wrong A argument in scale_for_libsvm_M \n';
		string = string + 'A s type must be double.\n';
		error(string);
	end

	if (nargin == 3)
		% checks arguments' integrity
		if (length(arg_one) ~= 1 || length(arg_two) ~= 1)
			string = 'wrong arg_one/two in scale_for_libsvm_M \n';
			string = [string, 'argument s length must be 1.\n'];
			error(string);
		end
		if (~isa(arg_one,'double') || ~isa(arg_two,'double'))
			string = 'wrong nim/max arguments in scale_for_libsvm_M \n';
			string = [string, 'new_min and new_max must be of type double.\n'];
			error(string);
		end

		new_min = arg_one;
		new_max = arg_two;
		if (new_min >= new_max)
			string = 'wrong nim/max arguments in scale_for_libsvm_M \n';
			string = [string, 'the relationship must be new_min < new_max.\n'];
			error(string);
		end

		% vector with a current maximum for every feature
		current_max = max(A);
		% vector with a current minimum for every feature
		current_min = min(A);

		% vector with a slope value for every feature
		slopes = (new_max-new_min)./(current_max-current_min);
		% vector with an intercept value for every feature
		intercept = -(new_max-new_min)./(current_max-current_min).*current_min+new_min;
		% this is the scaled version of matrix A
		scaled = bsxfun(@plus,bsxfun(@times,slopes,A),intercept);
		scaled(isnan(scaled))=0;
		C = struct('scaled', scaled, 'slopes', slopes, 'intercept', intercept);

	elseif (nargin == 2)
		% checks arg_one's integrity
		if (~isa(arg_one,'struct'))
			string = 'wrong arg_one in scale_for_libsvm_M \n';
			string = [string, 'argument s type must be struct.\n'];
			error(string);
		end
		if (~isa(arg_one.slopes,'double') || ...
		    length(size(arg_one.slopes)) ~= 2 || ...
		    size(arg_one.slopes,1) ~= 1 || ...
		    size(arg_one.slopes,2) ~= size(A,2))
			string = 'wrong arg_one.slopes in scale_for_libsvm_M \n';
			error(string);
		end
		if (~isa(arg_one.intercept,'double') || ...
		    length(size(arg_one.intercept)) ~= 2 || ...
		    size(arg_one.intercept,1) ~= 1 || ...
		    size(arg_one.intercept,2) ~= size(A,2))
			string = 'wrong arg_one.intercept in scale_for_libsvm_M \n';
			error(string);
		end
		C = bsxfun(@plus,bsxfun(@times,arg_one.slopes,A),arg_one.intercept);
		C(isnan(C))=0;
	else
		string = 'number of arguments in scale_for_libsvm_M \n';
		string = [string, 'must be two or three.\n'];
		error(string);
	end

end
