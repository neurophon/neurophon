% Layer_Visualization_M. This function allows the Layer connections' visualization for a cortical column.
function Layer_Visualization_M(column,fileName)

	cd ..
	load(fileName);
	cd Layer_Visualization

	if (length(size(afferentArrayDimensionality)) > 2 || ...
	    length(size(apicalArrayDimensionality)) > 2 || ...
	    length(size(columnsArrayDimensionality)) > 2 )
		error('This function only acepts fields of two or one dimensions.\n')
	end

	AfferentInputs = zeros(afferentArrayDimensionality);
	Columns = zeros(columnsArrayDimensionality);
	ApicalInputs = zeros(apicalArrayDimensionality);
	LateralProximalInputs = zeros(columnsArrayDimensionality);
	LateralDistalInputs = zeros(columnsArrayDimensionality);





	coordinates = unravelIndex_M(column, columnsArrayDimensionality)+1;
	Columns(coordinates(1),coordinates(2)) = 1;

	figure
	imagesc(Columns); % plot the matrix
	title('Cortical Columns')

	if ( exist('afferentConnections') )
		temp = full(afferentConnections);
		indexes = find(temp(column+1,:));

		for i = 1:length(indexes)
			coordinates = unravelIndex_M(indexes(i)-1,afferentArrayDimensionality)+1;
			AfferentInputs(coordinates(1),coordinates(2)) = 1;
		end

		figure
		imagesc(AfferentInputs); % plot the matrix
		title('Afferent Inputs')
	end

	if ( exist('apicalConnections') )
		temp = full(apicalConnections);
		indexes = find(temp(column+1,:));

		for i = 1:length(indexes)
			coordinates = unravelIndex_M(indexes(i)-1,apicalArrayDimensionality)+1;
			ApicalInputs(coordinates(1),coordinates(2)) = 1;
		end

		figure
		imagesc(ApicalInputs); % plot the matrix
		title('Apical Inputs')
	end

	if ( exist('lateralDistalConnections') )
		temp = full(lateralDistalConnections);
		indexes = find(temp(column+1,:));

		for i = 1:length(indexes)
			coordinates = unravelIndex_M(indexes(i)-1,columnsArrayDimensionality)+1;
			LateralDistalInputs(coordinates(1),coordinates(2)) = 1;
		end

		figure
		imagesc(LateralDistalInputs); % plot the matrix
		title('Lateral Distal Inputs')
	end

	if ( exist('lateralProximalConnections') )
		temp = full(lateralProximalConnections);
		indexes = find(temp(column+1,:));

		for i = 1:length(indexes)
			coordinates = unravelIndex_M(indexes(i)-1,columnsArrayDimensionality)+1;
			LateralProximalInputs(coordinates(1),coordinates(2)) = 1;
		end

		figure
		imagesc(LateralProximalInputs); % plot the matrix
		title('Lateral Proximal Inputs')
	end

end
