// $Id: getprojinfo.cpp,v 1.3 2005/02/01 16:08:13 jtrent Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

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
	strncpy(projFile, name, 254);
	projFile[254] = '\0';
	strncat(projFile,".proj", 254 - strlen(projFile));
	projFile[254] = '\0';

	inptr = fopen(projFile, "r");
	if (!inptr)
	{
	    fclose(inptr);
	    remove(projFile);
	    QMessageBox::critical( 0, "mapimg",
                 QString("An internal error occurred while trying to open the designated project file\n\nmapimg will not execute."));
	    return 0;
	}

	if(fscanf(inptr, "%ld %ld %ld %ld ", sys, zone, unit, datum) != 4)
	{
	    fclose(inptr);
	    remove(projFile);
	    QMessageBox::critical( 0, "mapimg",
                 QString("An internal error occurred while trying to read the designated project file\n\nmapimg will not execute." ));
	    return 0;
	}

	for(i = 0; i < 15; ++i)
	if(fscanf(inptr, "%lf", &pparm[i]) != 1)
	{
	    fclose(inptr);
	    remove(projFile);
	    QMessageBox::critical( 0, "mapimg",
                 QString("An internal error occurred while trying to read the designated project file\n\nmapimg will not execute." ));
	    return 0;
	}

	fclose(inptr);
	return 1;
}

