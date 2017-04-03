# Cartesian product, equivalent to a nested for-loop.
# This function only receives inferior and superior limits of the intervals (not the complete intervals).
# This feature allows the function to receive irregular intervals for each dimension
# (not all the intervals have to have the same number of elements).
function coordinates = cartesianProductI(intervals)

	if columns(intervals) != 2 || length(size(intervals)) != 2
		error("Bad format in input data.\n")
	endif

	if any(any(intervals < 0))
		error("At least one of the interval margins is less than 0.\n")
	endif

	coordinates = [];
	Step = 1;
	numberOfrows = 1;
	for i = rows(intervals):-1:1
		if Step == 1;
			if intervals(i,1) >= intervals(i,2)
				error("Corrupt intervals.\n")
			endif

			for j = intervals(i,1):intervals(i,2)
				coordinates = [coordinates,j];
			endfor
			Step++;
		else
			aux = [];
			if intervals(i,1) >= intervals(i,2)
				error("Corrupt intervals.\n")
			endif

			for j = intervals(i,1):intervals(i,2)
				temp = coordinates;
				for k = 1:Step:length(temp)+ceil(length(temp)/(Step-1))
					if k == 1
						temp = [j, temp];
					else
						temp = [temp(1:k-1), j, temp(k:end)];
					endif
				endfor
				aux = [aux, temp];
			endfor
			Step++;
			coordinates = aux;
		endif
		numberOfrows *= (intervals(i,2)-intervals(i,1)+1);
	endfor
	coordinates = reshape(coordinates,rows(intervals),numberOfrows)';
endfunction
