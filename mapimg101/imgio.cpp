//Copyright 2002 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MapIMG I/O functions:  These functions read and write files associated with MapIMG
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
char infile_info[500];		// Name of input info file
char outfile_info[500];		// Name of output info file

FILE * inptr;				// Input file pointer
FILE * outptr;				// Output file pointer
FILE * ininfoptr;				// Input .info file pointer
FILE * outinfoptr;				// Output .info file pointer

long insize;				// Number of bytes in input image
long outsize;				// Number of bytes in output image

void * mapimginbuf;			// Ptr to the input image (all in memory)
void * mapimgoutbuf;			// Ptr to one line of output image data


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
    {
	fclose(inptr);
    }

    if(outptr)
    {
	fclose(outptr);
    }

    if(ininfoptr)
    {
	fclose(ininfoptr);
    }

    if(outinfoptr)
    {
	fclose(outinfoptr);
    }

    remove(outfile_name);
    remove(infile_info);
    remove(outfile_info);
    return;
}
