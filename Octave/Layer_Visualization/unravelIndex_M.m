function coordinates = unravelIndex_M(index, dimensionsOfArray)

	numberOfCoordinates = length(dimensionsOfArray);

	if index >= prod(dimensionsOfArray)
		error('index is bigger than the number of elements in the array\n')
	end

	if numberOfCoordinates == 1
		coordinates = index;
	else
		aux = index;
		for i = numberOfCoordinates:-1:1
			if dimensionsOfArray(i) <= 0
				error('at least one array coordinate is <= 0\n')
			end
			coordinates(i) = mod(aux,dimensionsOfArray(i));
			aux = floor(aux/dimensionsOfArray(i));
		end
	end
end
