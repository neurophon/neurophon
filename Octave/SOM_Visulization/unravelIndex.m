function coordinates = unravelIndex(index, dimensionsOfArray)

	numberOfCoordinates = length(dimensionsOfArray);

	if index >= prod(dimensionsOfArray)
		error("index is bigger than the number of elements in the array\n")
	endif

	if numberOfCoordinates == 1
		coordinates = index;
	else
		aux = index;
		for i = numberOfCoordinates:-1:1
			if dimensionsOfArray(i) <= 0
				error("at least one array coordinate is <= 0\n")
			endif
			coordinates(i) = mod(aux,dimensionsOfArray(i));
			aux = floor(aux/dimensionsOfArray(i));
		endfor
	endif
endfunction
