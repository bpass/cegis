// $Id: imgio.h,v 1.8 2005/02/14 17:29:05 jtrent Exp $


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

#include <qstring.h>
#include <qfile.h>
#include <qmessagebox.h>
#include "rasterinfo.h"
#include <qcache.h>



#define DEFAULT_Max_Data_Element_Count 100;

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


template <class type>
class IMGIO
{
 private:
      int Max_Data_Element_Count;
      QCache<type>* inputDataMap;

      QString infile_name;		        // Name of input file
      QString outfile_name;		        // Name of output file

      QFile inptr;				// Input file pointer
      QFile outptr;				// Output file pointer
      QFile ininfoptr;				// Input .info file pointer
      QFile outinfoptr;				// Output .info file pointer


 public:
      long insize;				// Number of bytes in input image
      long outsize;				// Number of bytes in output image

      void* mapimginbuf;			// Ptr to the input image (all in memory)
      void* mapimgoutbuf;			// Ptr to one line of output image data


      IMGIO()
      {
      	Max_Data_Element_Count = DEFAULT_Max_Data_Element_Count;
        inputDataMap = NULL;
      }
      
      ~IMGIO()
      {
      	if( inputDataMap )
      	{
      	    inputDataMap->clear();
      	    delete inputDataMap;
      	}
      }

      void clearCache()
      {
      	  if( inputDataMap )
      	      inputDataMap->clear();
      	  return;
      }

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
	 {
             img->pparm[i] = ras.gctpParam(i);
         }

         return;
      }

      // Image I/O functions
      void send_imgio_par(long inout, QString name)
      {
	 if(inout == INFILE_NAME)
	 {
		infile_name = name;		// Init infile name
	 }

	 if(inout == OUTFILE_NAME)
	 {
		outfile_name = name;		// Init outfile name
         }

  	 return;
      }

      // Cleanup
      void cleanup_input()
      {
	inptr.close();
	return;
      }

      void cleanup_output()
      {
	outptr.close();
	return;
      }

      // Parse the input arguments and initialize processing
      void parse_input( QString infile, QString outfile)
      {
	send_imgio_par(INFILE_NAME, infile);
	send_imgio_par(OUTFILE_NAME, outfile);
	return;
      }


      // Early Error cleanup
      void early_error_cleanup()
      {
          if(inptr.isOpen())
          {
              inptr.close();
          }

          if(outptr.isOpen())
          {
	      outptr.close();
          }

          if(ininfoptr.isOpen())
          {
	      ininfoptr.close();
          }

          if(outinfoptr.isOpen())
          {
	      outinfoptr.close();
          }

          remove(outfile_name);
          return;
      }

      // Function to initialize input image, buffers & projection
      // Definition must be in header for
      // Solaris compiler compatability
      template <class type>
      int init_io(RasterInfo &input, RasterInfo &output, IMGINFO * inimg, IMGINFO * outimg, type )
      {
      	void * bufptr;		// Pointer to input buffer

	// Open input file and check for any errors
	if( inptr.isOpen() )
	    inptr.close();

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
	if( outptr.isOpen() )
	    outptr.close();

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
     static type get_max_value( RasterInfo &input, type )
     {
        QString inputFilename = input.imgFileName().ascii();

        int inputSize = input.rows() * input.cols();

        FILE * inputPtr = fopen(inputFilename.ascii(), "rb");
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
     template <class type>
     void get_line(void* &buf, Q_ULLONG  offset, int lineLength, type)
     {
         // check and see if line requested is already in memory
         QString offsetString = "";
         offsetString.setNum( offset );

         if( inputDataMap == NULL )
         {
      	     inputDataMap = new QCache<type>( Max_Data_Element_Count, 1.6*Max_Data_Element_Count );
             inputDataMap->setAutoDelete( true );
         }

         if( inputDataMap->find( offsetString ) ==  0 )
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
        	if( inputDataMap->insert( offsetString, (type*)newBuffer ) != true )
        	{
	           printf( "Error deleting least recently used item.\n" );
        	   fflush( stdout );
	        }
	    }
        }


        if( inputDataMap->find( offsetString ) !=  0 )
        {
           buf = inputDataMap->find( offsetString );
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
};

#ifndef RESAMPLE
#define RESAMPLE

/******************************************************************
  New 10/15/2004 for resample
  New 02/02/2005 fpr mapimg 2.0
******************************************************************/

// Read input image sample by line and sample number
// Definition must be in header for
// Solaris compiler compatability
// ---------------------------
template <class type>
void* get_raster_value(void * buf, long offset, long sample, int lineLength, type typeToUse)
{
     // check and see if line requested is already in memory
     //if not load it
     get_line( buf, offset, lineLength, typeToUse );
     return ((type*)buf + sample);
}



//--------------------------------------------------------
// get_coords
//--------------------------------------------------------
// use output pixel coordinate to find the coordinates of
// the corners of the corresponding input pixel
//--------------------------------------------------------
static int find2corners=0;	// true if previous pixel's corners can be reused

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



#endif //RESAMPLE

#endif //IMGIO_H

