// $Id: imgio.h,v 1.1 2005/01/14 16:18:49 rbuehler Exp $


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
#include "rasterinfo.h"

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

void raster2IMG( RasterInfo &ras, IMGINFO *img );
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


template <class type>
int init_io(RasterInfo &input, RasterInfo &output, IMGINFO * inimg, IMGINFO * outimg, type )
{
	void * bufptr;		// Pointer to input buffer

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

   raster2IMG( input, inimg );
   raster2IMG( output, outimg );

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

	mapimginbuf = bufptr;
 	mapimgoutbuf = (void *) malloc(outsize*sizeof(type));

        return 1;
}

// Read partial input image and find max value
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------
template <class type>
//type get_max_value(const char* inputFilename, int inputSize, type typeToUse)
type get_max_value( RasterInfo &input, type typeToUse )
{
   char* inputFilename = new char[500];
   strcpy( inputFilename, input.imgFileName().ascii() );
   int inputSize = input.rows() * input.cols();

   FILE * inputPtr = fopen(inputFilename, "rb");
   if( inputPtr == NULL ) 
      return (type)0.0;

   if( inputSize > 10000 )
     	inputSize = 10000;

   void* bufptrMax = (type *) malloc(inputSize*sizeof(typeToUse));

   if( bufptrMax == NULL )
      return (type)0.0;

   fread(bufptrMax, sizeof(typeToUse), inputSize, inputPtr);

   fclose(inputPtr);

   inputSize = inputSize / sizeof( typeToUse );

   type max_value = (type)0.0;

   for( int index = 0; index < (inputSize-1)/2; index++ )
   {
         if( *((type*)bufptrMax + index*sizeof(typeToUse)) > max_value )
            max_value = *((type*)bufptrMax + index*sizeof(typeToUse));
   }

   free(bufptrMax);
   //delete inputPtr;

   return max_value;
}

// Read input image line by line number
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------
extern FILE * inptr;				// Input file pointer  from imgio.cpp
extern long insize;				// Number of bytes in input image
static long get_line_loadedData;

template <class type>
void get_line(void * buf, long offset, int , type )
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
void get_line(void * buf, int , type )
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
void put_line(void * buf, type )
{
     fwrite(buf, sizeof(type), outsize, outptr);

     return;
}

#endif

