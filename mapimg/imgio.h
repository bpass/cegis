// $Id: imgio.h,v 1.4 2005/02/01 16:08:13 jtrent Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

// Modified by Jason Trent to allow for templating -- 7/03

#ifndef IMGIO_H
#define IMGIO_H

#define OK			0
#define NOT_OK	            -1
#define INFILE_NAME		1
#define OUTFILE_NAME	2


//#define _LARGEFILE_SOURCE
//#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include <qfile.h>
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

extern QFile inptr;
extern QFile outptr;			// Output file pointer  from imgio.cpp

template <class type>
int init_io(RasterInfo &input, RasterInfo &output, IMGINFO * inimg, IMGINFO * outimg, type )
{
	void * bufptr;		// Pointer to input buffer

	// Open input file and check for any errors
	inptr.setName( infile_name );
	inptr.open( IO_ReadOnly | IO_Raw );

	if( !inptr.isOpen() || !inptr.isReadable() )
	{
	    early_error_cleanup();
	    QMessageBox::critical( 0, "mapimg",
	    QString("An internal error occurred while trying to open the designated input file\n\nmapimg will not execute."));
	    return 0;
	}

	// Open output file and check for any errors
	outptr.setName( outfile_name );
        outptr.open( IO_WriteOnly | IO_Raw );

	if(!outptr.isOpen() || !outptr.isWritable() )
	{
	    early_error_cleanup();
	    QMessageBox::critical( 0, "MapIMG",
	    QString("An internal error occurred while trying to open the designated output file\n\nMapIMG will not execute."));
	    return 0;
	}

   raster2IMG( input, inimg );
   raster2IMG( output, outimg );
	
	// Set input file size and try to allocate that amount of memory
	
   /******* Change here for allocating lines in input *******/

	insize = inimg->nl * inimg->ns;


        insize = inimg->ns;

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

        if(!mapimgoutbuf)
	{
	    early_error_cleanup();
	    QMessageBox::critical(0, "MapIMG",
	    QString("An internal error occurred while trying to allocate the input image buffer!  Image is too large!\n\nMapIMG will not execute."));
	    return 0;
	}

	for( int index = 0; index < outsize; index++ )
        {
        	*((type*)mapimgoutbuf + index) = (type)0;
        }


        return 1;
}

// Read partial input image and find max value
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------
template <class type>
type get_max_value( RasterInfo &input, type )
{
   char* inputFilename = new char[500];
   strncpy( inputFilename, input.imgFileName().ascii(), 499 );
   inputFilename[499] = '\0';

   int inputSize = input.rows() * input.cols();

     FILE * inputPtr = fopen(inputFilename, "rb");
   if( inputPtr == NULL )
      return (type)0.0;

     if( inputSize > 10000 )
     	 inputSize = 10000;

     void* bufptrMax = (type *) malloc(inputSize*sizeof(type));

     if( bufptrMax == NULL )
      return (type)0.0;

     fread( bufptrMax, sizeof(type), inputSize, inputPtr );
     
     fclose( inputPtr );

     inputSize = inputSize / sizeof( type );

     type max_value = (type)0.0;

     for( int index = 0; index < (inputSize-1)/2; index++ )
     {
           if( *((type*)bufptrMax + index) > max_value )
              max_value = *((type*)bufptrMax + index);
     }

     free(bufptrMax);
   //delete inputPtr;

     return max_value;
}

// Read input image line by line number
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------
extern QFile inptr;				// Input file pointer  from imgio.cpp
extern long insize;				// Number of bytes in input image
//static off64_t get_line_loadedData;

#include <qcache.h>
static int MAX_DATA_ELEMENT_COUNT = 800;		//20, 23
static int FRIST_PRIME_AFTER_MAX = 801;

template <class type>
void get_line(void* &buf, Q_ULLONG  offset, int lineLength, type)
{
  static QCache<type> inputDataMap( MAX_DATA_ELEMENT_COUNT, FRIST_PRIME_AFTER_MAX );
  inputDataMap.setAutoDelete( true );

     // check and see if line requested is already in memory
     QString offsetString = "";
     offsetString.setNum( offset );

     if( inputDataMap.find( offsetString ) ==  0 )
     {
        if( !inptr.at( (Q_ULLONG)(offset) * lineLength * sizeof(type)) )
        {
            // end of file or corrupt file found
            printf( "error seeking!!!\n" );
	    fflush( stdout );
        }
	
        //then load the line into memory
        type *newBuffer = new type[lineLength];
	long amountRead = inptr.readBlock( (char*&)newBuffer, sizeof(type) * lineLength);

	if( amountRead != (long)(sizeof(type) * lineLength) )
	{
		printf( "Read %li requested %i\n", amountRead, lineLength );
		fflush( stdout );

		if( inptr.atEnd() )
		{
			printf( "End-of-File reached\n" );
			fflush( stdout );
		}
		else if( amountRead = -1 )
		{
			printf( "Error on read.!!!\n" );
			fflush(stdout);

			/*  Add more descriptive messages here*/
		}
	}
	else
	{
        	if( inputDataMap.insert( offsetString, (type*)newBuffer ) != true )
        	{
	           printf( "Error deleting least recently used item.\n" );
        	   fflush( stdout );
	        }
	}
     }


     if( inputDataMap.find( offsetString ) !=  0 )
     {
       buf = inputDataMap.find( offsetString );

     }
     else
     {
       printf( "Error inserting into and retreiving from least recently used cache.\n" );
       fflush( stdout );
       buf = NULL;
     }
     
     return;
}


// Write a line of output image data
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------------
extern QFile outptr;				// Output file pointer  from imgio.cpp
extern long outsize;				// Number of bytes in output image

template <class type>
void put_line(void * buf, type )
{
     if( outptr.isOpen() && outptr.isWritable() )
     {
         outptr.writeBlock( (char*&)buf, outsize*sizeof(type) );
         outptr.flush();
     }
     else
     {
         printf( "Outptr not open or writable\n" );
	 fflush( stdout );
     }

     return;
}


// #ifndef RESAMPLE
// #define RESAMPLE
// 
// /******************************************************************
//   New 10/15/2004 for resample
// 
// ******************************************************************/
// 
// 
// // Read input image sample by line and sample number
// // Definition must be in header for
// // Solaris compiler compatability
// // ---------------------------
// template <class type>
// void* get_raster_value(void * buf, long offset, long sample, int lineLength, type typeToUse)
// {
//      // check and see if line requested is already in memory
//      //if not load it
//      get_line( buf, offset, linLength, typeToUse );
// 
//      return ((type*)buf + sample);
// }
// 
// 
// 
// 
// //--------------------------------------------------------
// // get_coords                                             
// //--------------------------------------------------------
// // use output pixel coordinate to find the coordinates of 
// // the corners of the corresponding input pixel           
// //--------------------------------------------------------
// 
// 
// 
// //static double in[2];			/* Input projection coordinates of a point */
// //static double out[2];			/* Output projection coordinates of a point */
// //static double corner[2];		// for computing values at pixel corners
// //static double coord[2];		// random coordinates
// //static double in_line, in_samp;	/* Input image coordinates of a point */
// //static long status;			/* Return status flag for gctp() call */
// //static long zero=0;			/* Constant of 0 */
// //static long num_classes=255;		// number of classes used...
// //static int preferred;		// most preferred class
// //static int unpreferred;	// least preferred class
// 
// 
// //---------- flags ----------//
// //static int dodump=0;		// whether or not to dump minbox to screen
// //static int domax=0;		// image of most common value per pixel
// //static int domin=0;		// image of least common value per pixel
// //static int donn=0;		// create traditional mapimg image
// //static int dobands=0;		// images of how many hits per pixel per band
// //static int dochoice=0;		// image of how many classes to choose from per pixel
// //static int doout=0;		// write "the" output file
// //static int dopreferred=0;	// most preferred class
// //static int dounpreferred=0;	// least preferred class
// //static int boxerr=0;		// true if no minbox pixels are in polygon
// static int find2corners=0;	// true if previous pixel's corners can be reused
// //static int classcount[256][2]={0};// flags which classes are used per output pixel
// 	// classcount[x][0] is true if class x is used
// 	// classcount[][1] is a list of the classes used in current pixel
// //static int class2data[256][2]={0};// flags which classes are used in input image
// 	// class2data[x][0] converts data value x to its internal class number
// 	// class2data[x][1] converts internal class number x to its data value
// 
// 
// 
// #define DELTA_LS 0.00005
// #define DELTA_METERS outimg.pixsize/2
// 
// extern "C"
// {
//        #include "proj.h"
// }
// 
// #include <math.h>
// 
// int get_coords( IMGINFO outimg, IMGINFO inimg, double out[2], double inbox[5][2], long out_line, long out_samp, FILE* paramfile );
// int onLine(double p1[2], double p2[2], double test[2]);
// int inBox(double box[4][2], double test[2]);
// 
// 
// /*
// // Write a line of output image data
// // Definition must be in header for
// // Solaris compiler compatability
// // ---------------------------------
// extern FILE * outptr;				// Output file pointer  from imgio.cpp
// 
// template <class type>
// void put_line(void * buf, FILE* file, type)
// {
//      fwrite(buf, sizeof(type), outsize, file);
//      return;
// }
// 
// */
// 
// 
// 
// 
// #endif //RESAMPLE


#endif

