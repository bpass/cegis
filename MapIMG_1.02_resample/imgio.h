//Copyright 2002 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

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
#include <unistd.h>
#include <stdlib.h>

#include <qfile.h>
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
extern FILE * ininfoptr;		// Input .info file pointer
extern FILE * outinfoptr;		// Output .info file pointer

extern QFile inptr;
extern FILE * outptr;			// Output file pointer  from imgio.cpp

template <class type>
int init_io(IMGINFO * inimg, IMGINFO * outimg, type)
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
	inptr.setName( infile_name );
	inptr.open( IO_ReadOnly | IO_Raw );
/*	inptr = fopen64(infile_name, "rb");
	if(!inptr)
	{
		int dangErrCode = errno;//ferror( inptr );
		
		if( dangErrCode == EINVAL )
		{
			printf ("Input mode invalid\n" );
			fflush( stdout );
		}
		else if( dangErrCode == EACCES || dangErrCode == EAGAIN )
		{
			printf ("Operation is prohibited by locks held by other  processes. Or, by another process.\n" );
			fflush( stdout );
		}
		else if( dangErrCode == EBADF )
		{
			printf ("fd is not an open file descriptor, or the command was F_SETLK or F_SETLKW  and  the  file descriptor open mode doesn't match with the type of lock requested.\n" );
			fflush( stdout );
		}
		
		else if( dangErrCode == EDEADLK )
		{
			printf ("It was detected that the specified F_SETLKW command would  cause  a deadlock.\n" );
			fflush( stdout );
		}
		else if( dangErrCode == EFAULT )
		{
			printf ("lock is outside your accessible address space.\n" );
			fflush( stdout );
		}
		else if( dangErrCode == EINTR )
		{
			printf ("For  F_SETLKW,  the  command  was  interrupted by a signal. For F_GETLK and F_SETLK, the command was  interrupted  by  a signal before the lock was checked or acquired.  Most likely when lock-ing a remote file (e.g. locking over  NFS),  but  can  sometimes happen locally.\n" );
			fflush( stdout );
		}
		else if( dangErrCode == EMFILE )
		{
			printf ("For  F_DUPFD, the process already has the maximum number of file descriptors open.\n" );
			fflush( stdout );
		}
		else if( dangErrCode == ENOLCK )
		{
			printf ("Too many segment locks open, lock table is  full,  or  a  remote locking protocol failed (e.g. locking over NFS).\n" );
			fflush( stdout );
		}
		else if( dangErrCode == EPERM )
		{
			printf ("Attempted  to  clear  the  O_APPEND  flag on a file that has the  append-only attribute set.\n" );
			fflush( stdout );
		}
		else 
		{
			printf ("Undefined = %i opening %s\n", errno, infile_name );
			fflush( stdout );
		}
*/
	if( !inptr.isOpen() || !inptr.isReadable() )
	{
	    early_error_cleanup();
	    QMessageBox::critical( 0, "MapIMG",
	    QString("An internal error occurred while trying to open the designated input file\n\nMapIMG will not execute."));
	    return 0;
	}

	// Open output file and check for any errors
	outptr = fopen64(outfile_name, "wb");
	if(!outptr)
	{
	    early_error_cleanup();
	    QMessageBox::critical( 0, "MapIMG",
	    QString("An internal error occurred while trying to open the designated output file\n\nMapIMG will not execute."));
	    return 0;
	}


	// Open input .info file and get image geometry information
	// -------------------------------------------------
	ininfoptr = fopen(infile_info, "rb");		// Open info file
	if(!ininfoptr)
	{
	    early_error_cleanup();
	    QMessageBox::critical( 0, "MapIMG",
	    QString("An internal error occurred while trying to open the designated input info file\n\nMapIMG will not execute."));
	    return 0;
	}

	// Open output .info file and get image geometry information
	// -------------------------------------------------
	outinfoptr = fopen(outfile_info, "rb");	// Open info file
	if(!outinfoptr)
	{
	    early_error_cleanup();
	    QMessageBox::critical( 0, "MapIMG",
	    QString("An internal error occurred while trying to open the designated output info file\n\nMapIMG will not execute."));
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


        insize = inimg->ns;

        bufptr = (type *) malloc(insize*sizeof(type));
        Q_CHECK_PTR( bufptr );

	if(!bufptr)
	{
	    early_error_cleanup();
	    QMessageBox::critical(0, "MapIMG",
	    QString("An internal error occurred while trying to allocate the input image buffer!  Image is too large!\n\nMapIMG will not execute."));
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
type get_max_value(const char* inputFilename, int inputSize, type)
{
     FILE * inputPtr = fopen(inputFilename, "rb");
     if( inputPtr == NULL ) return (type)0.0;

     if( inputSize > 10000 )
     {
     	 inputSize = 10000;
     }

     void* bufptrMax = (type *) malloc(inputSize*sizeof(type));
     if( bufptrMax == NULL ) return (type)0.0;

     fread(bufptrMax, sizeof(type), inputSize, inputPtr);

     inputSize = inputSize / sizeof( type );

     type max_value = (type)0.0;

     for( int index = 0; index < (inputSize-1)/2; index++ )
     {
           if( *((type*)bufptrMax + index*sizeof(type)) > max_value )
              max_value = *((type*)bufptrMax + index*sizeof(type));
     }

     free(bufptrMax);
     free(inputPtr);
     
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

        if( !inptr.at( (Q_ULLONG)(offset * sizeof(type)) ) )
        {
            // end of file or corrupt file found
            printf( "error seeking!!!\n" );
	    fflush(stdout );
        }

        //then load the line into memory
        type *newBuffer = new type[insize];
//  	long amountRead = fread(newBuffer, sizeof(type), insize, inptr);
	long amountRead = inptr.readBlock( (char*&)newBuffer, sizeof(type) * insize);
	
	if( amountRead != sizeof(type) * insize )
	{
		printf( "Read %i requested %i\n", amountRead, insize );
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
/*
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
  }
 */
     return;
}



// Write a line of output image data
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------------
extern FILE * outptr;				// Output file pointer  from imgio.cpp
extern long outsize;				// Number of bytes in output image

template <class type>
void put_line(void * buf, type)
{
     fwrite(buf, sizeof(type), outsize, outptr);
     fflush( outptr );
     return;
}

#ifndef RESAMPLE
#define RESAMPLE

/******************************************************************
  New 10/15/2004 for resample

******************************************************************/


// Read input image sample by line and sample number
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------
extern long insize;				// Number of bytes in input image

template <class type>
void* get_raster_value(void * buf, long offset, long sample, int lineLength, type typeToUse)
{
     // check and see if line requested is already in memory
     //if not load it
     get_line( buf, offset, linLength, typeToUse );

     return ((type*)buf + sample);
}




/*--------------------------------------------------------*\
\* get_coords                                             */
/*--------------------------------------------------------*\
\* use output pixel coordinate to find the coordinates of */
/* the corners of the corresponding input pixel           *\
\*--------------------------------------------------------*/



//static double in[2];			/* Input projection coordinates of a point */
//static double out[2];			/* Output projection coordinates of a point */
//static double corner[2];		// for computing values at pixel corners
//static double coord[2];		// random coordinates
//static double in_line, in_samp;	/* Input image coordinates of a point */
//static long status;			/* Return status flag for gctp() call */
//static long zero=0;			/* Constant of 0 */
//static long num_classes=255;		// number of classes used...
//static int preferred;		// most preferred class
//static int unpreferred;	// least preferred class


//---------- flags ----------//
//static int dodump=0;		// whether or not to dump minbox to screen
//static int domax=0;		// image of most common value per pixel
//static int domin=0;		// image of least common value per pixel
//static int donn=0;		// create traditional mapimg image
//static int dobands=0;		// images of how many hits per pixel per band
//static int dochoice=0;		// image of how many classes to choose from per pixel
//static int doout=0;		// write "the" output file
//static int dopreferred=0;	// most preferred class
//static int dounpreferred=0;	// least preferred class
//static int boxerr=0;		// true if no minbox pixels are in polygon
static int find2corners=0;	// true if previous pixel's corners can be reused
//static int classcount[256][2]={0};// flags which classes are used per output pixel
	// classcount[x][0] is true if class x is used
	// classcount[][1] is a list of the classes used in current pixel
//static int class2data[256][2]={0};// flags which classes are used in input image
	// class2data[x][0] converts data value x to its internal class number
	// class2data[x][1] converts internal class number x to its data value



#define DELTA_LS 0.00005
#define DELTA_METERS outimg.pixsize/2

extern "C"
{
       #include "proj.h"
}

#include <math.h>

int get_coords( IMGINFO outimg, IMGINFO inimg, double out[2], double inbox[5][2], long out_line, long out_samp, FILE* paramfile );
int onLine(double p1[2], double p2[2], double test[2]);
int inBox(double box[4][2], double test[2]);


/*
// Write a line of output image data
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------------
extern FILE * outptr;				// Output file pointer  from imgio.cpp

template <class type>
void put_line(void * buf, FILE* file, type)
{
     fwrite(buf, sizeof(type), outsize, file);
     return;
}

*/




#endif //RESAMPLE


#endif  //IMGIO_H


