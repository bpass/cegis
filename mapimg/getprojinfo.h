// $Id: getprojinfo.h,v 1.5 2005/02/05 00:16:55 rbuehler Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

#ifndef GETPROJINFO_H
#define GETPROJINFO_H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header files for updated get_projInfo, mapimg, mapframeit, geo2eqr, and random  functions
// Created by Stephan Posch -- 8/02
// Modified by Jason Trent to allow for templating  -- 7/03
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <qwidget.h>
#include <qmessagebox.h>
#include <qprogressdialog.h>
#include <qtextstream.h>
#include <qdir.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "window_flags.h"
#include "getprojinfo.h"
#include "imgio.h"

#include "logform.h"
#include "jt_time.h"
#include "rasterinfo.h"
#include "resampleinfo.h"

extern "C"
{
       #include "proj.h"
}

/*jtrent   I have turned off all logginf for right now, then should be enabled after teh merge */
static char*  logFile = strdup( QDir::currentDirPath().append("/mapimg.log").ascii() );
static long   errorMode = 0;    //print both errors and parameters to a precreated
static long   paramMode = 0;    //FILE* to logFile


int get_projInfo(char *name, long *sys, long *unit, long *zone, long *datum, double pparm[]);

int mapframeit(char * filename,double pixsiz,double ul_lat,double ul_lon,
		double lr_lat,double lr_lon, const char *outfilename, long * outputRows, long * outputCols);

int geo2eqr(long numLines, long numSamps, const char * infilename, int projnum);


// Bring in the buffers
extern  void * mapimginbuf;			// Ptr to the input image (all in memory)
extern  void * mapimgoutbuf;			// Ptr to one line of output image data

template <class type>
bool mapimg_resample( RasterInfo input, RasterInfo output, ResampleInfo resample, type useType, QWidget * mapimgdial)
{
    // mapimg STARTS HERE!!!!
    // mapimg to do the reprojection (no longer called as function in order to provide progress dialog)
    // mapimg written by D. Steinwand and updated by S. Posch

    switch( resample.resampleCode() )
    {
    	case ResampleInfo::NearestNeighbor:
    	     printf( "Nearest neighbor" );
    	     break;
    	case ResampleInfo::Add:
    	     printf( "Add" );
    	     break;
    	case ResampleInfo::Mean:
    	     printf( "Mean" );
    	     break;
    	case ResampleInfo::Mode:
    	     printf( "Mode" );
    	     break;
    	case ResampleInfo::Min:
    	     printf( "Min" );
    	     break;
    	case ResampleInfo::Max:
    	     printf( "Max" );
    	     break;
    	case ResampleInfo::NullResample:
    	     printf( "NullResample" );
    	     break;
    	default:
    	     printf( "Bad resample code!!!" );
    	     break;
    }

    printf( "\tFill value = %f\tNo data value = %f\n",
            resample.fillValue(),
            resample.noDataValue() );
    fflush( stdout );

    printf( "Ignore List(%i):\n", resample.ignoreList().size() );

    IgnoreList::iterator ignoreVal;

    for( ignoreVal = resample.ignoreList().begin(); ignoreVal != resample.ignoreList().end(); ignoreVal++ )
    {
    	printf( "\t%f\n", *ignoreVal );
    }

    fflush( stdout );

    int outputRows = output.rows();
    int outputCols = output.cols();

    type fill = (type)output.fillValue();			// Fill value for mapimg

    parse_input(input.imgFileName().ascii(), output.imgFileName().ascii());

    IMGINFO inimg;				// Image information--input
    IMGINFO outimg;			// Image information--output

    int ioreturnval;
    long out_line, out_samp;	// Output image coordinates of a point

    double in[2];				// Input projection coordinates of a point
    double out[2];				// Output projection coordinates of a point
    double t1[2];				// Temp coords for comparing
    double t2[2];				// Temp coords for comparing
    double temp1, temp2;			// Temp vars
    double pparm[15] = {0};			// 15 GCTP projection parameters

    long zero = 0;
    long four = 4;


    long status;				// Return status flag for gctp() call
    long in_line, in_samp;			// Input image coordinates of a point


    jt_time debugTimer;
    debugTimer.start();



    // Initialize input & output space image & projections
    ioreturnval = init_io(input, output, &inimg, &outimg, useType );

    /*if(!ioreturnval)	// jtrent    is this needed anymore?? 
    {                   // rbuehler  my guess would be no. 
	remove("abctempxyz31199.proj");
	remove("abctempxyz311992.proj");
	mapimgdial->setEnabled(true);
	return true;
    }*/

    // Progress Dialog added in QT
    QProgressDialog progress( "Performing Transformation", "Abort", outimg.nl,
			      mapimgdial, "progress", TRUE, WINDOW_FLAGS );
    progress.setCaption( "Processing..." );

    // Set min Duration required for Dialog at 1 second (try to make it always show)
    progress.setMinimumDuration(1);

    // Perform inverse mapping--loop thru output image and get appropriate input
    // -----------------------------------------------------------------------------------------


    /******* Change bounds here for doing per line or per line section *******/
    FILE *paramfile = fopen( logFile, "wa");

    for(out_line = 0; out_line < outimg.nl; out_line++) 		// For each output image line
    {
	// Set progress of Dialog box and cancel if process was cancelled

	progress.setProgress(out_line);

	if(progress.wasCancelled())
	{
 	   break;
	}

	out[1] = outimg.ul_y - (out_line * outimg.pixsize);		// Calc out-img proj_Y coord

	for (out_samp = 0; out_samp < outimg.ns; out_samp++)	// For each output image sample
	{
		out[0] = outimg.ul_x + (out_samp * outimg.pixsize);	// Calc out-img proj_X coord

		// Goode's already does error checking; skip the wrap-around check
		// ---------------------------------------------------------------
		if(outimg.sys == 24)
		{
//			gctp(out,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
//				&zero,"",&zero,"",in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
//				&inimg.datum,"","",&status);
			gctp(out,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
				&errorMode,logFile, &paramMode,logFile,paramfile,in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
				&inimg.datum,"","",&status);

		}

		else
		{

			// Check for a wrap-around condition by mapping Proj --> Lat/Long and
			// then back again.  Is it the same coordinate?  If not, consider the
			// pixel in the wrap-around area...
			// --------------------------------
/*			gctp(out,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
				&zero,"",&zero,"",t1,&zero,&outimg.zone,pparm,&four,
				&outimg.datum,"","",&status);

			gctp(t1,&zero,&outimg.zone,pparm,&four,&outimg.datum,
				&zero,"",&zero,"",t2,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,
				&outimg.datum,"","",&status);

*/
			gctp(out,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
				&errorMode,logFile,&paramMode,logFile,paramfile,t1,&zero,&outimg.zone,pparm,&four,
				&outimg.datum,"","",&status);

			gctp(t1,&zero,&outimg.zone,pparm,&four,&outimg.datum,
				&errorMode,logFile,&paramMode,logFile,paramfile,t2,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,
				&outimg.datum,"","",&status);

			temp1=fabs(t2[0]-out[0]);
			temp2=fabs(t2[1]-out[1]);

			if(temp1 > (outimg.pixsize/2) || temp2 > (outimg.pixsize/2))
			{
				status = NOT_OK;
			}

			else
			{
				// Now, if still OK, do the projection transformation
				// --------------------------------------------------
//				gctp(out,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
//					&zero,"",&zero,"",in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
//							&inimg.datum,"","",&status);
				gctp(out,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
				     &errorMode,logFile,&paramMode,logFile,paramfile,in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
							&inimg.datum,"","",&status);
			}
		}

		if(status != OK)
		{
                   *( (type*)mapimgoutbuf+out_samp) = fill;	// Fill if not valid...
		}

		// Calculate the input image coordinate (line, sample) from the projection
		// coordinate that resulted from the output-to-input projection transformation
		// ---------------------------------------------------------------------------
		else
		{
			in_line = (long int)(((inimg.ul_y - in[1]) / inimg.pixsize) + 0.5);
			in_samp = (long int)(((in[0] - inimg.ul_x) / inimg.pixsize) + 0.5);

			// Still inside the input image??
  			// ------------------------------
			if (in_line<0||in_samp<0||in_line>=inimg.nl||in_samp>=inimg.ns)
			{
			    *( (type*)mapimgoutbuf + out_samp) = fill;
			}

			// Assign the appropriate input image pixel to the output image using
			// nearest-neighbor resampling. (...the half-pixel rounding is done in
			// the in_line, in_samp calculate above...)
			// ---------------------------------------------
			else
			{

                            //Loads into memory the current line of input needed
                            get_line(  mapimginbuf, in_line, inimg.ns, useType );

                            //references specific element of input
			    *( (type*)mapimgoutbuf + out_samp) = *((type*)mapimginbuf + in_samp);

			}
		}
	}


	// Spit out a processing message & write the output image line to disk
	// -------------------------------------------------------------------
	put_line(mapimgoutbuf, useType);
    }

    progress.setProgress( outimg.nl );

    // Close down processing & Exit... Processing is complete.
    // -------------------------------------------------------
    cleanup_input();
    cleanup_output();
    // END of mapimg!!

    bool cancelled = false;
    // if process was Aborted, output aborted message
   fclose( paramfile );

   if( mapimgdial == 0 )
      return progress.wasCancelled();

    if(progress.wasCancelled())
    {
	QMessageBox::information (mapimgdial, "Aborted!","mapimg has been aborted",
					  QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	cancelled = true;
    }

    // if process Completed output completion message
    else
    {
   	QString out = "mapimg has completed\n\nOutput Rows: " ;
	QString str = QString::number(outputRows, 10);
	out += str;
	out += "\n\nOutput Columns: ";
	str = QString::number(outputCols, 10);
	out += str;
	out += "\n";

	//debug timing purposes
	out += "\nDuration: ";
	out += debugTimer.stop();
	out += "\n";

        if( QFile::exists( logFile ) )
        {
            QFile appendLog( logFile );
            appendLog.open( IO_WriteOnly | IO_Append );
            QTextStream appendStream( &appendLog );
            appendStream << '\n' << out;
            appendLog.close();
        }


        //retVal will be an integer, 0 if OK is clicked, 1 if Log is clicked, or -1 if the X is clicked
	int retVal = QMessageBox::information (mapimgdial, "Completed",out, "OK",
					  "View Log", NULL, 0);

	if( retVal == 1 )
	{
	    logForm *logform;
	    logform = new logForm(0,0,true,WINDOW_FLAGS);
	    logform->exec();
            delete logform;
	}

	cancelled = false;
    }

    return cancelled;
}

#endif
