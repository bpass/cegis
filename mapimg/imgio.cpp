// $Id: imgio.cpp,v 1.1 2005/01/14 16:18:49 rbuehler Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mapimg I/O functions:  These functions read and write files associated with mapimg
// Written by D. Steinwand
//
// Updated by Stephan Posch for GUI implementation -- 8/02
// Updated by Jason Trent for templating to allow multiple data types -- 7/03
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//#include <qmessagebox.h>
#include <qstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imgio.h"

char infile_name[500];		// Name of input file
char outfile_name[500];		// Name of output file

FILE * inptr;				// Input file pointer
FILE * outptr;				// Output file pointer
FILE * ininfoptr;				// Input .info file pointer
FILE * outinfoptr;				// Output .info file pointer

long insize;				// Number of bytes in input image
long outsize;				// Number of bytes in output image

void * mapimginbuf;			// Ptr to the input image (all in memory)
void * mapimgoutbuf;			// Ptr to one line of output image data

// Assigns all values in the IMGINFO to equal the RasterInfo
void raster2IMG( RasterInfo &ras, IMGINFO *img )
{
   int i;
   img->nl = ras.rows(); img->ns = ras.cols();
   img->sys = ras.projectionNumber();
   img->zone = ras.zoneNumber();
   img->unit = ras.unitNumber();
   img->datum = ras.datumNumber();
   img->pixsize = ras.pixelSize();
   img->ul_x = ras.ul_X(), img->ul_y = ras.ul_Y();
	for(i = 0; i < 15; i++)
      img->pparm[i] = ras.gctpParam(i);
}

// Image I/O functions
// -------------------
void send_imgio_par(long inout, const char * name)
{
	if(inout == INFILE_NAME)
	{
		strncpy(infile_name, name, 500);		// Init infile name
	}

   if(inout == OUTFILE_NAME)
   {
      strncpy(outfile_name, name, 500);		// Init outfile name
   }
	return;
}

// Cleanup
// -------
void cleanup_input()
{
	fclose(inptr);
	return;
}

void cleanup_output()
{
	fclose(outptr);
	return;
}

// Parse the input arguments and initialize processing
// ---------------------------------------------------
void parse_input(const char * infile, const char * outfile)
{
	send_imgio_par(INFILE_NAME, infile);
	send_imgio_par(OUTFILE_NAME, outfile);
	return;
}


// Early Error cleanup
void early_error_cleanup()
{
   if(inptr)
      fclose(inptr);

   if(outptr)
      fclose(outptr);

   if(ininfoptr)
      fclose(ininfoptr);

   if(outinfoptr)
      fclose(outinfoptr);

   remove(outfile_name);
   return;
}
