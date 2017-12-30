% Cartesian product, equivalent to a nested for-loop.
% This function only receives inferior and superior limits of the intervals (not the complete intervals).
% This feature allows the function to receive irregular intervals for each dimension
% (not all the intervals have to have the same number of elements).
function coordinates = cartesianProductI_M(intervals)

	if size(intervals,2) ~= 2 || length(size(intervals)) ~= 2
		error('Bad format in input data.\n')
	end

	if any(any(intervals < 0))
		error('At least one of the interval margins is less than 0.\n')
	end

	coordinates = [];
	Step = 1;
	numberOfrows = 1;
	for i = size(intervals,1):-1:1
		if Step == 1;
			if intervals(i,1) >= intervals(i,2)
				error('Corrupt intervals.\n')
			end

			for j = intervals(i,1):intervals(i,2)
				coordinates = [coordinates,j];
			end
			Step = Step+1;
		else
			aux = [];
			if intervals(i,1) >= intervals(i,2)
				error('Corrupt intervals.\n')
			end

			for j = intervals(i,1):intervals(i,2)
				temp = coordinates;
				for k = 1:Step:length(temp)+ceil(length(temp)/(Step-1))
					if k == 1
						temp = [j, temp];
					else
						temp = [temp(1:k-1), j, temp(k:end)];
					end
				end
				aux = [aux, temp];
			end
			Step = Step+1;
			coordinates = aux;
		end
		numberOfrows = numberOfrows * (intervals(i,2)-intervals(i,1)+1);
	end
	coordinates = reshape(coordinates,size(intervals,1),numberOfrows)';
end
