// $Id: mapframeit.cpp,v 1.5 2005/01/11 17:11:27 jtrent Exp $

//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

// MapFrame calculates an output space mapping frame based on given projection
// parameters, the pixel stepSize of the output image, and a bounding box given
// in decimal degrees.
//
// Written by D.Steinwand 7/5/02
//
// Updated by S. Posch 9/27/02 -- converted to C++ for use in QT
//                             -- redirected output to file for GUI
//
// ---------------------------------------------------------------------------

#include <qmessagebox.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "getprojinfo.h"

extern "C"
{
#include "proj.h"
}

/* I hope this doesn't break anything!!
 * VC++ math.h does not have the c/c++standard round function!!!
 * One day the evil shall FALL
 */
#include <qglobal.h>
#ifdef Q_OS_WIN32
 	double round(double value, unsigned int decimals = 0)
	{
		double factor = pow(10,decimals);
		return floor((value * factor) + 0.5) / factor;
	}
#endif //Q_OS_WIN32


int mapframeit(char * filename, double pixsiz, double ul_lat, double ul_lon,
			   double lr_lat, double lr_lon, const char * outfilename,
			   long * outputRows, long * outputCols)
{
	FILE * file2;
	int projreturnval;

	long numLines;
	long numSamps;
	double pixX, pixY;
	double pxmin;			// Projection minimum in X
	double pxmax;			// Projection maximum in X
	double pymin;			// Projection minimum in Y
	double pymax;			// Projection maximum in Y

	long status;
	long i,j;
	long stepSize = 1000;
	double delta_north, delta_east;

	long outProj;
	long outUnit;
	long outZone;
	long outDatum;
	double outParms[15];

	long inProj = 0;
	long inUnit = 4;
	long inZone = 62;
	long inDatum = 0;
	long zero = 0;
	double inParms[15];

	double inCoords[2];
	double outCoords[2];

	for (i = 0; i < 15; inParms[i++] = 0.0);

	pxmin = 1000000000.0;
	pymin = 1000000000.0;
	pxmax = -1000000000.0;
	pymax = -1000000000.0;


	// Read the projection parameters from the .proj file
	// --------------------------------------------------
        projreturnval = get_projInfo(filename, &outProj, &outUnit, &outZone, &outDatum, outParms);
	if(!projreturnval)
	{
		return 0;
	}

        FILE *paramfile = fopen( logFile, "wa");

	// Calc projection coordinates (initially) for the four corners
	// ------------------------------------------------------------
	inCoords[0] = ul_lon;
	inCoords[1] = ul_lat;


	gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
   	        paramfile,outCoords, &outProj,&outZone,outParms,&outUnit,&outDatum,"","",&status);

	if (outCoords[0] < pxmin) pxmin = outCoords[0];
	if (outCoords[0] > pxmax) pxmax = outCoords[0];
	if (outCoords[1] < pymin) pymin = outCoords[1];
	if (outCoords[1] > pymax) pymax = outCoords[1];

	inCoords[0] = lr_lon;
	inCoords[1] = lr_lat;


        gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
		paramfile,outCoords, &outProj,&outZone,outParms,&outUnit,&outDatum,"","",&status);
	if (outCoords[0] < pxmin) pxmin = outCoords[0];
	if (outCoords[0] > pxmax) pxmax = outCoords[0];
	if (outCoords[1] < pymin) pymin = outCoords[1];
	if (outCoords[1] > pymax) pymax = outCoords[1];

	inCoords[0] = ul_lon;
	inCoords[1] = lr_lat;


	gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
		paramfile,outCoords, &outProj,&outZone,outParms,&outUnit,&outDatum,"","",&status);
	if (outCoords[0] < pxmin) pxmin = outCoords[0];
	if (outCoords[0] > pxmax) pxmax = outCoords[0];
	if (outCoords[1] < pymin) pymin = outCoords[1];
	if (outCoords[1] > pymax) pymax = outCoords[1];

	inCoords[0] = lr_lon;
	inCoords[1] = ul_lat;


        gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
		paramfile,outCoords, &outProj,&outZone,outParms,&outUnit,&outDatum,"","",&status);
	if (outCoords[0] < pxmin) pxmin = outCoords[0];
	if (outCoords[0] > pxmax) pxmax = outCoords[0];
	if (outCoords[1] < pymin) pymin = outCoords[1];
	if (outCoords[1] > pymax) pymax = outCoords[1];

	// Now step along the sides -- this is somewhat brute force, and it's accuracy
	//  is affected by the step size.  This method should be replaced (!!)
	// ---------------------------------------------------------------------
	delta_east = fabs(ul_lon - lr_lon) / stepSize;
	delta_north = fabs(ul_lat - lr_lat) / (stepSize/2.0);

	/* Calculate the minimum and maximum coordinates
	 ---------------------------------------------*/
	for (i = 0; i <= stepSize; i++)
	{
		inCoords[0] = ul_lon + (delta_east * i);
		for (j = 0; j<= (stepSize/2); j++)
		{
			inCoords[1] = lr_lat + (delta_north * j);

			gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
				 paramfile,outCoords, &outProj,&outZone,outParms,&outUnit,&outDatum,"","",&status);
			if (outCoords[0] < pxmin) pxmin = outCoords[0];
			if (outCoords[0] > pxmax) pxmax = outCoords[0];
			if (outCoords[1] < pymin) pymin = outCoords[1];
			if (outCoords[1] > pymax) pymax = outCoords[1];

		}
	}

	// Calc output image size, etc.
	// ---------------------------
	numLines = (long) round( ((pymax - pymin) / pixsiz) );
	numSamps = (long) round( ((pxmax - pxmin) / pixsiz) );


	pixX = pxmin + (pixsiz/2);
	pixY = pymax - (pixsiz/2);

	// Report Results
	// --------------
	file2 = fopen(outfilename, "w");
	if(!file2)
	{
		fclose(file2);
		remove(outfilename);
		QMessageBox::critical(0, "mapimg", QString("An internal error occurred while trying to open the designated output file\n\nmapimg will not execute."));
		return 0;
	}
	fprintf(file2, "%d %d\n", numLines, numSamps);
	fprintf(file2, "%d\n", outProj);
	fprintf(file2, "%d\n", outZone);
	fprintf(file2, "%d\n", outUnit);
	fprintf(file2, "%d\n", outDatum);
	fprintf(file2, "%lf\n", pixsiz);
	fprintf(file2, "%lf %lf\n", pixX, pixY);
	for(i = 0; i < 15; i++) fprintf(file2, "%lf ", outParms[i]);
	fclose(file2);
	*outputRows = numLines;
	*outputCols = numSamps;

        fclose( paramfile );
	return 1;
}

