// $Id: imgio.h,v 1.3 2005/01/11 17:11:27 jtrent Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

// Modified by Jason Trent to allow for templating -- 7/03

#ifndef IMGIO_H
#define IMGIO_H

#define OK			0
#define NOT_OK	            -1
#define INFILE_NAME		1
#define OUTFILE_NAME	2


#include <qmessagebox.h>

struct IMGINFO
{
	double pparm[15];				// 15 GCTP projection parameters
	double ul_y;					// Upper left projection corner y
	double ul_x;					// Upper left projection corner x

	float pixsize;					// Image pixel size in meters

	long nl;						// Number of lines in image
	long ns;						// Number of samples in image
	long sys;					    // Projection system code
	long zone;						// Projection zone code
	long unit;
	long datum;						// Datum code (spheroid code for now)
};

void cleanup_input();				// Function to close input file and deallocate buffers
void cleanup_output();				// Function to close output file and deallocate buffers
void parse_input(const char * infile, const char * outfile);
void send_imgio_par(long inout, const char * name);
void early_error_cleanup();


// Function to initialize input image, buffers & projection
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------------
extern char infile_name[500];		// Name of input file   from imgio.cpp
extern char outfile_name[500];		// Name of output file
extern char infile_info[500];		// Name of input info file
extern char outfile_info[500];		// Name of output info file
extern FILE * ininfoptr;				// Input .info file pointer
extern FILE * outinfoptr;				// Output .info file pointer


template <class type>
int init_io(IMGINFO * inimg, IMGINFO * outimg, type typeToUse)
{
	void * bufptr;		// Pointer to input buffer
	long i;				// Loop counter

	// Get .info of input file
	strncpy(infile_info, infile_name, 500);
	strncat(infile_info, ".info", 500);

	// Get .info out output file
	strncpy(outfile_info, outfile_name, 500);
	strncat(outfile_info, ".info", 500);

	// Open input file and check for any errors
	inptr = fopen(infile_name, "rb");
	if(!inptr)
	{
	    early_error_cleanup();
	    QMessageBox::critical( 0, "mapimg",
	    QString("An internal error occurred while trying to open the designated input file\n\nmapimg will not execute."));
	    return 0;
	}

	// Open output file and check for any errors
	outptr = fopen(outfile_name, "wb");
	if(!outptr)
	{
	    early_error_cleanup();
	    QMessageBox::critical( 0, "mapimg",
	    QString("An internal error occurred while trying to open the designated output file\n\nmapimg will not execute."));
	    return 0;
	}


	// Open input .info file and get image geometry information
	// -------------------------------------------------
	ininfoptr = fopen(infile_info, "rb");		// Open info file
	if(!ininfoptr)
	{
	    early_error_cleanup();
	    QMessageBox::critical( 0, "mapimg",
	    QString("An internal error occurred while trying to open the designated input info file\n\nmapimg will not execute."));
	    return 0;
	}

	// Open output .info file and get image geometry information
	// -------------------------------------------------
	outinfoptr = fopen(outfile_info, "rb");	// Open info file
	if(!outinfoptr)
	{
	    early_error_cleanup();
	    QMessageBox::critical( 0, "mapimg",
	    QString("An internal error occurred while trying to open the designated output info file\n\nmapimg will not execute."));
	    return 0;
	}

	// Scan in variables from input .info file
	fscanf(ininfoptr, "%d %d", &inimg->nl, &inimg->ns);				// Get number of lines, samples
	fscanf(ininfoptr, "%d", &inimg->sys);					// Projection system code
	fscanf(ininfoptr, "%d", &inimg->zone);					// Projection zone code
	fscanf(ininfoptr, "%d", &inimg->unit);					// Projection unit code
	fscanf(ininfoptr, "%d", &inimg->datum);					// Projection datum code
	fscanf(ininfoptr, "%f", &inimg->pixsize);					// Image pixel size in meters
	fscanf(ininfoptr, "%lf %lf", &inimg->ul_x, &inimg->ul_y);			// Upper left projection coords
	for(i = 0; i < 15; i++)
	{
		fscanf(ininfoptr, "%lf", &inimg->pparm[i]);				// 15 gctp projection parameters
	}

	// Scan in variables from output .info file
	fscanf(outinfoptr, "%d %d", &outimg->nl, &outimg->ns);				// Get number of lines, samples
	fscanf(outinfoptr, "%d", &outimg->sys);						// Projection system code
	fscanf(outinfoptr, "%d", &outimg->zone);						// Projection zone code
	fscanf(outinfoptr, "%d", &outimg->unit);						// Projection unit code
	fscanf(outinfoptr, "%d", &outimg->datum);						// Projection datum code
	fscanf(outinfoptr, "%f", &outimg->pixsize);						// Image pixel size in meters
	fscanf(outinfoptr, "%lf %lf", &outimg->ul_x, &outimg->ul_y);				// Upper left projection coords


	for(i = 0; i < 15; i++)
	{
		fscanf(outinfoptr, "%lf", &outimg->pparm[i]);				// 15 gctp projection parameters
	}

	// Set input file size and try to allocate that amount of memory

   /******* Change here for allocating lines in input *******/

	insize = inimg->nl * inimg->ns;


        insize = inimg->ns + 2;

        bufptr = (type *) malloc(insize*sizeof(type));
        Q_CHECK_PTR( bufptr );

	if(!bufptr)
	{
	    early_error_cleanup();
	    QMessageBox::critical(0, "mapimg",
	    QString("An internal error occurred while trying to allocate the input image buffer!  Image is too large!\n\nmapimg will not execute."));
	    return 0;
	}

	// Set output file size

	outsize = outimg->ns;

	// close all files
	fclose(ininfoptr);
	fclose(outinfoptr);

	mapimginbuf = bufptr;
 	mapimgoutbuf = (void *) malloc(outsize*sizeof(type));

        return 1;
}

// Read partial input image and find max value
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------

template <class type>
type get_max_value(const char* inputFilename, int inputSize, type typeToUse)
{
     FILE * inputPtr = fopen(inputFilename, "rb");
     if( inputPtr == NULL ) return (type)0.0;

     if( inputSize > 10000 )
     {
     	 inputSize = 10000;
     }

     void* bufptrMax = (type *) malloc(inputSize*sizeof(typeToUse));
     if( bufptrMax == NULL ) return (type)0.0;

     fread(bufptrMax, sizeof(typeToUse), inputSize, inputPtr);

     inputSize = inputSize / sizeof( typeToUse );

     type max_value = (type)0.0;

     for( int index = 0; index < (inputSize-1)/2; index++ )
     {
           if( *((type*)bufptrMax + index*sizeof(typeToUse)) > max_value )
              max_value = *((type*)bufptrMax + index*sizeof(typeToUse));
     }

     delete bufptrMax;
     delete inputPtr;

     return max_value;
}


// Read the entire input image (no longer used)
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------
extern FILE * inptr;				// Input file pointer  from imgio.cpp
extern long insize;				// Number of bytes in input image

template <class type>
void get_image(void * buf, type typeToUse)
{
     fread(buf, sizeof(type), insize, inptr);
     return;
}

// Read input image line by line number
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------
extern FILE * inptr;				// Input file pointer  from imgio.cpp
extern long insize;				// Number of bytes in input image
static long get_line_loadedData;

template <class type>
void get_line(void * buf, long offset, int lineLength, type typeToUse)
{
     // check and see if line requested is already in memory
     if( get_line_loadedData != offset )
     {
        //if not, first, seek to that point in the file
     	get_line_loadedData = offset;

        if( fseek( inptr, offset * sizeof(type), 0 ) != 0 )
        {
            // end of file or corrupt file found
            p_error( "seek_image: error!", "[image read]");
            get_line_loadedData = -1;
        }

        //then load the line into memory
        fread(buf, sizeof(type), insize, inptr);

     }
     return;
}


// Read input image line
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------
extern FILE * inptr;				// Input file pointer  from imgio.cpp
extern long insize;				// Number of bytes in input image

template <class type>
void get_line(void * buf, int lineLength, type typeToUse)
{
     fread(buf, sizeof(type), insize, inptr);
     return;
}




// Write a line of output image data
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------------
extern FILE * outptr;				// Output file pointer  from imgio.cpp
extern long outsize;				// Number of bytes in output image

template <class type>
void put_line(void * buf, type typeToUse)
{
     fwrite(buf, sizeof(type), outsize, outptr);

     return;
}

#endif

