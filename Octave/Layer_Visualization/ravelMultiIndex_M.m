function index = ravelMultiIndex_M(coordinatesOfArray, dimensionsOfArray)

	if length(coordinatesOfArray) ~= length(dimensionsOfArray)
		error('the two input arrays have different number of elements\n')
	end

	numberOfCoordinates = length(dimensionsOfArray);

	if numberOfCoordinates == 1
		index = coordinatesOfArray;
	else
		index = 0;
		for i = 1:numberOfCoordinates
			if dimensionsOfArray(i) <= 0 || coordinatesOfArray(i) < 0
				error('at least one array dimension or array coordinate is <= 0\n')
			end

			product = 1;
			if i < numberOfCoordinates
				for j = 2+(i-1):numberOfCoordinates
					product = product * dimensionsOfArray(j);
				end
			end

			index = index + product*coordinatesOfArray(i);
		end
	end
end
