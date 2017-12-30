% Cartesian product, equivalent to a nested for-loop.
function coordinates = cartesianProduct_M(intervals)

	if length(size(intervals)) ~= 2
		error('Bad format in input data.\n')
	end

	if any(any(intervals < 0))
		error('At least one of the interval members is less than 0.\n')
	end

	coordinates = [];
	Step = 1;
	numberOfrows = (size(intervals,2)^size(intervals,1));
	for i = size(intervals,1):-1:1
		if Step == 1;
			coordinates = [coordinates,intervals(i,:)];
			Step = Step + 1;
		else
			aux = [];
			for j = 1:length(intervals(i,:))
				temp = coordinates;
				for k = 1:Step:length(temp)+ceil(length(temp)/(Step-1))
					if k == 1
						temp = [intervals(i,j), temp];
					else
						temp = [temp(1:k-1), intervals(i,j), temp(k:end)];
					end
				end
				aux = [aux, temp];
			end
			Step = Step + 1;
			coordinates = aux;
		end
	end
	coordinates = reshape(coordinates,size(intervals,1),numberOfrows)';
end
