# Cartesian product, equivalent to a nested for-loop.
function coordinates = cartesianProduct(intervals)

	if length(size(intervals)) != 2
		error("Bad format in input data.\n")
	endif

	if any(any(intervals < 0))
		error("At least one of the interval members is less than 0.\n")
	endif

	coordinates = [];
	Step = 1;
	numberOfrows = (columns(intervals)^rows(intervals));
	for i = rows(intervals):-1:1
		if Step == 1;
			coordinates = [coordinates,intervals(i,:)];
			Step++;
		else
			aux = [];
			for j = 1:length(intervals(i,:))
				temp = coordinates;
				for k = 1:Step:length(temp)+ceil(length(temp)/(Step-1))
					if k == 1
						temp = [intervals(i,j), temp];
					else
						temp = [temp(1:k-1), intervals(i,j), temp(k:end)];
					endif
				endfor
				aux = [aux, temp];
			endfor
			Step++;
			coordinates = aux;
		endif
	endfor
	coordinates = reshape(coordinates,rows(intervals),numberOfrows)';
endfunction
