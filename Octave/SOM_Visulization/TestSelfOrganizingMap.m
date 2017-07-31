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

if (StaticSelfOrganizingMap_SOM_inputDimensionality == 1)
	plot(1:length(StaticSelfOrganizingMap_SOM_weights),StaticSelfOrganizingMap_SOM_weights,'-o');
elseif (StaticSelfOrganizingMap_SOM_inputDimensionality == 2)
	scatter(StaticSelfOrganizingMap_SOM_weights(:,1),StaticSelfOrganizingMap_SOM_weights(:,2));
	DrawLattice(StaticSelfOrganizingMap_SOM_weights, StaticSelfOrganizingMap_SOM_unitsArrayDimensionality);
elseif (StaticSelfOrganizingMap_SOM_inputDimensionality == 3)
	scatter3(StaticSelfOrganizingMap_SOM_weights(:,1),StaticSelfOrganizingMap_SOM_weights(:,2),StaticSelfOrganizingMap_SOM_weights(:,3));
	DrawLattice(StaticSelfOrganizingMap_SOM_weights, StaticSelfOrganizingMap_SOM_unitsArrayDimensionality);
else
	error("inputDimensionality exceeds the plots' possibilities.")
endif
