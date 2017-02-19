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

# File Name:		InsertHeader.m
# Language:		GNU Octave high-level interpreted language.

# This program inserts the header in a SABLE markup format file.
# This file will be interpreted by The Festival Speech Synthesis System.
# SABLE is collaboration between Sun, AT&T, Bell Labs, Edinburgh and CMU to devise a standard cross synthesizer standard mark up language.
# The language is XML-based and allows users to add addition controlling commands in text to affect the output.
# An implementation exists in Festival as well as Bell Labs synthesis system. 

function InsertHeader( identifier )

fputs (identifier, "<?xml version=\"1.0\"?>\n");
fputs (identifier, "<!DOCTYPE SABLE PUBLIC \"-//SABLE//DTD SABLE speech mark up//EN\" \n");
fputs (identifier, "      \"Sable.v0_2.dtd\"");
fputs (identifier, "[]>\n");
fputs (identifier, "<SABLE>\n\n");

endfunction
