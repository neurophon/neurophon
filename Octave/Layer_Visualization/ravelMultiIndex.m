function index = ravelMultiIndex(coordinatesOfArray, dimensionsOfArray)

	if length(coordinatesOfArray) != length(dimensionsOfArray)
		error("the two input arrays have different number of elements\n")
	endif

	numberOfCoordinates = length(dimensionsOfArray);

	if numberOfCoordinates == 1
		index = coordinatesOfArray;
	else
		index = 0;
		for i = 1:numberOfCoordinates
			if dimensionsOfArray(i) <= 0 || coordinatesOfArray(i) < 0
				error("at least one array dimension or array coordinate is <= 0\n")
			endif

			product = 1;
			if i < numberOfCoordinates
				for j = 2+(i-1):numberOfCoordinates
					product *= dimensionsOfArray(j);
				endfor
			endif

			index += product*coordinatesOfArray(i);
		endfor
	endif
endfunction
