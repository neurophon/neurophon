##################################################################################################################
##				Author:		Dematties Dario Jesus						##
##				Contact:	dariodematties@hotmail.com.ar					##
##						dariodematties@yahoo.com.ar					##
##						dario.dematties@frm.utn.edu.ar					##
##				Project:	Engineering PhD Project						##
##				Institution:	Universidad de Buenos Aires					##
##						Facultad de Ingeniería (FIUBA)					##
##				Workplace:	Instituto de Ingeniería						##
##						Biomédica FIUBA	&						##
##						CCT CONICET Mendoza INCIHUSA					##
##################################################################################################################

# File Name:		TestSelfOrganizingMap.m
# Language:		GNU Octave high-level interpreted language.

#This program tests the Self Organizing Map plotting the units' lattice.

cd ..
load SOM_Status.mat
cd SOM_Visulization

if (inputDimensionality == 1)
	plot(1:length(weights),weights,'-o');
elseif (inputDimensionality == 2)
	scatter(weights(:,1),weights(:,2));
	DrawLattice(weights, unitsArrayDimensionality);
elseif (inputDimensionality == 3)
	scatter3(weights(:,1),weights(:,2),weights(:,3));
	DrawLattice(weights, unitsArrayDimensionality);
else
	error("inputDimensionality exceeds the plots' possibilities.")
endif
