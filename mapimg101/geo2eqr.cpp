// $Id: geo2eqr.cpp,v 1.3 2005/01/11 17:11:27 jtrent Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

// Mapframe calculates an output space mapping frame base on a given projection
// parameters, the pixel stepSize of the output image, and a bounding box given
// in decimal degrees.
//
// Written by D. Steinwand 7/5/02
//
// Updated by S. Posch 8/02 -- converted to C++ for implementatin in QT
//                                               -- redirected output to a file for GUI
//
// ---------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <qmessagebox.h>
#include <qstring.h>
#include "getprojinfo.h"

extern "C"
{
#include "proj.h"
}

int geo2eqr(long numLines, long numSamps, const char * infilename, int projnum)
{
	FILE *file;

	double pixsiz;				// Pixel Size in output image
	double pixX, pixY;
	double pxmin;				// Projection minimum in X
	double pxmax;				// Projection maximum in X
	double pymin;				// Projection minimum in Y
	double pymax;				// Projection maximum in Y

	long status;
	long i;

	long outProj = projnum;
	long outUnit = 2;
	long outZone = 62;
	long outDatum = 0;
	double outParms[15];

	long inProj = 0;
	long inUnit = 4;
	long inZone = 62;
	long inDatum = 0;
	long zero = 0;
	double R = 6370997.0;
	double inParms[15];

	double inCoords[2];
	double outCoords[2];

	for(i = 0; i < 15; inParms[i++] = 0.0);
	for(i = 0; i < 15; outParms[i++] = 0.0);

	pxmin = 1000000000.0;
	pymin = 1000000000.0;
	pxmax = -1000000000.0;
	pymax = -1000000000.0;

        FILE *paramfile = fopen( logFile, "wa");

	// Parse Parameters
	// ----------------
	outParms[0] = R;

	// Calc projection coordinates for the four corners.  OK, OK, for an Equirectangular
	// space & square pixels this really isn't necessary 'cuz it's easy to figure out with 2PiR
	// etc., but it's here for a check.  It should check EXACTLY...
	// ---------------------------------------------------------------------------------------
	inCoords[0] = -180.0;
	inCoords[1] = 90.0;

//	gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &zero, "", &zero, "",
//		outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);
	gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
		paramfile, outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);

	if(outCoords[0] < pxmin)
	{
	    pxmin = outCoords[0];
	}
	 if(outCoords[0] > pxmax)
	{
	    pxmax = outCoords[0];
	}
	if(outCoords[1] < pymin)
	 {
	    pymin = outCoords[1];
	}
	if(outCoords[1] > pymax)
	{
	    pymax = pymax = outCoords[1];
	}


	inCoords[0] = 180.0;
	inCoords[1] = -90.0;

//	gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &zero, "", &zero, "",
//		outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);
	gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
		paramfile,outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);

	if(outCoords[0] < pxmin)
	{
	    pxmin = outCoords[0];
	}
	if(outCoords[0] > pxmax)
	{
	    pxmax = outCoords[0];
	}
	if(outCoords[1] < pymin)
	{
	    pymin = outCoords[1];
	}
	if(outCoords[1] > pymax)
	{
	    pymax = pymax = outCoords[1];
	}


	inCoords[0] = -180.0;
	inCoords[1] = -90.0;

//	gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &zero, "", &zero, "",
//		outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);
	gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
		paramfile,outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);

	if(outCoords[0] < pxmin)
	{
	    pxmin = outCoords[0];
	}
	if(outCoords[0] > pxmax)
	{
	    pxmax = outCoords[0];
	}
	if(outCoords[1] < pymin)
	{
	    pymin = outCoords[1];
	}
	if(outCoords[1] > pymax)
	{
	    pymax = pymax = outCoords[1];
	}


	inCoords[0] = 180.0;
	inCoords[1] = 90.0;

//	gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &zero, "", &zero, "",
//		outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);
	gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
		paramfile,outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);

	if(outCoords[0] < pxmin)
	{
	    pxmin = outCoords[0];
	}
	if(outCoords[0] > pxmax)
	{
	    pxmax = outCoords[0];
	}
	if(outCoords[1] < pymin)
	{
	    pymin = outCoords[1];
	}
	if(outCoords[1] > pymax)
	{
	    pymax = pymax = outCoords[1];
	}


	// Calc output imagee pixel size
	// -----------------------------
	pixsiz = (2.0 * 3.141593653589793238 * R) / (double)numSamps;
	pixX = pxmin + (pixsiz / 2);
	pixY = pymax + (pixsiz / 2);

	// Output Results to File
	// ----------------------
	file = fopen(infilename, "w");
	if(!file)
	{
	    fclose(file);
	    remove(infilename);

	    /***
	       * Geographic to Eqirectangular projection (or variant)
               *
               * "An internal error occurred while trying to open the designated output file"
               * "mapimg will not execute."
               *
               * The Output file could not be opened for storing the parameter file.
               * Check for a write protected .info associated with the input .img
               ***/

	    QMessageBox::critical( 0, "mapimg",
		QString("An internal error occurred while trying to open the designated output file\n\nmapimg will not execute."));
	    return 0;
	}
	fprintf(file, "%d %d\n", numLines, numSamps);
	fprintf(file, "%d\n", outProj);
	fprintf(file, "%d\n", outZone);
	fprintf(file, "%d\n", outUnit);
	fprintf(file, "%d\n", outDatum);
	fprintf(file, "%lf\n", pixsiz);
	fprintf(file, "%lf %lf\n", pixX, pixY);
	for(i = 0; i < 15; i++)
		fprintf(file, "%lf ", outParms[i]);
	fclose(file);
	
        fclose( paramfile );
	return 1;
}


