# Layer_Visualization. This function allows the Layer connections' visualization for a cortical column.
function Layer_Visualization(column,fileName)

	cd ..
	load(fileName);
	cd Layer_Visualization

	if (size(afferentArrayDimensionality) > 2 || size(apicalArrayDimensionality) > 2 || size(columnsArrayDimensionality) > 2 )
		error("This function only acepts fields of two or one dimensions.\n")
	endif

	AfferentInputs = zeros(afferentArrayDimensionality);
	Columns = zeros(columnsArrayDimensionality);
	ApicalInputs = zeros(apicalArrayDimensionality);
	LateralProximalInputs = zeros(columnsArrayDimensionality);
	LateralDistalInputs = zeros(columnsArrayDimensionality);





	coordinates = unravelIndex(column, columnsArrayDimensionality).+1;
	Columns(coordinates(1),coordinates(2)) = 1;

	figure
	imagesc(Columns); % plot the matrix
	title("Cortical Columns")

	if ( exist("afferentConnections") )
		indexes = find(full(afferentConnections)(column+1,:));

		for i = 1:length(indexes)
			coordinates = unravelIndex(indexes(i)-1,afferentArrayDimensionality).+1;
			AfferentInputs(coordinates(1),coordinates(2)) = 1;
		endfor

		figure
		imagesc(AfferentInputs); % plot the matrix
		title("Afferent Inputs")
	endif

	if ( exist("apicalConnections") )
		indexes = find(full(apicalConnections)(column+1,:));

		for i = 1:length(indexes)
			coordinates = unravelIndex(indexes(i)-1,apicalArrayDimensionality).+1;
			ApicalInputs(coordinates(1),coordinates(2)) = 1;
		endfor

		figure
		imagesc(ApicalInputs); % plot the matrix
		title("Apical Inputs")
	endif

	if ( exist("lateralDistalConnections") )
		indexes = find(full(lateralDistalConnections)(column+1,:));

		for i = 1:length(indexes)
			coordinates = unravelIndex(indexes(i)-1,columnsArrayDimensionality).+1;
			LateralDistalInputs(coordinates(1),coordinates(2)) = 1;
		endfor

		figure
		imagesc(LateralDistalInputs); % plot the matrix
		title("Lateral Distal Inputs")
	endif

	if ( exist("lateralProximalConnections") )
		indexes = find(full(lateralProximalConnections)(column+1,:));

		for i = 1:length(indexes)
			coordinates = unravelIndex(indexes(i)-1,columnsArrayDimensionality).+1;
			LateralProximalInputs(coordinates(1),coordinates(2)) = 1;
		endfor

		figure
		imagesc(LateralProximalInputs); % plot the matrix
		title("Lateral Proximal Inputs")
	endif

endfunction
