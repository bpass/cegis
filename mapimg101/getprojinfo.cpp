// $Id: getprojinfo.cpp,v 1.2 2005/01/06 19:55:34 jtrent Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

/* reads the .proj file that accompanies generic binary file */
#include <qmessagebox.h>
#include <qstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getprojinfo.h"

int get_projInfo(char *name, long *sys, long *unit, long *zone, long *datum, double pparm[])
{
	FILE *inptr;
	char projFile[255];
	int i;

	// Open .proj file
	// ---------------
	strcpy(projFile, name);
	strcat(projFile,".proj");
	inptr = fopen(projFile, "r");
	if (!inptr)
	{
	    fclose(inptr);
	    remove(projFile);
	    QMessageBox::critical( 0, "MapIMG",
                 QString("An internal error occurred while trying to open the designated project file\n\nMapIMG will not execute."));
	    return 0;
	}

	if(fscanf(inptr, "%d %d %d %d ", sys, zone, unit, datum) != 4)
	{
	    fclose(inptr);
	    remove(projFile);
	    QMessageBox::critical( 0, "MapIMG",
                 QString("An internal error occurred while trying to read the designated project file\n\nMapIMG will not execute." ));
	    return 0;
	}

	for(i = 0; i < 15; ++i)
	if(fscanf(inptr, "%lf", &pparm[i]) != 1)
	{
	    fclose(inptr);
	    remove(projFile);
	    QMessageBox::critical( 0, "MapIMG",
                 QString("An internal error occurred while trying to read the designated project file\n\nMapIMG will not execute." ));
	    return 0;
	}

	fclose(inptr);
	return 1;
}

