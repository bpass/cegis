//Copyright 2002 United States Geological Survey
//Released under GPL with MapIMG copyright 2003 USGS

#ifndef GETPROJINFO_H
#define GETPROJINFO_H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header files for updated get_projInfo, mapimg, mapframeit, geo2eqr, and random  functions
// Created by Stephan Posch -- 8/02
// Modified by Jason Trent to allow for templating  -- 7/03
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <qdialog.h>
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

#include "logForm.h"

// Time Testing Library
#include "jt_time.h"


extern "C"
{
       #include "proj.h"
}

static char*  logFile = strdup( QDir::currentDirPath().append("/mapimg.log").ascii() );
static long   errorMode = 3;    //print both errors and parameters to a precreated
static long   paramMode = 3;    //FILE* to logFile


int get_projInfo(char *name, long *sys, long *unit, long *zone, long *datum, double pparm[]);

int mapframeit(char * filename,double pixsiz,double ul_lat,double ul_lon,
		double lr_lat,double lr_lon, const char *outfilename, long * outputRows, long * outputCols);

int geo2eqr(long numLines, long numSamps, const char * infilename, int projnum);


// Bring in the buffers
extern  void * mapimginbuf;			// Ptr to the input image (all in memory)
extern  void * mapimgoutbuf;			// Ptr to one line of output image data

template <class type>
bool mapimg(const char * mapimginfilename, const char * mapimgoutfilename,
	        int fillval, const char * outfilename, QDialog * mapimgdial,
	        int outputRows, int outputCols, type useType)
{
    // MapIMG STARTS HERE!!!!
    // mapimg to do the reprojection (no longer called as function in order to provide progress dialog)
    // mapimg written by D. Steinwand and updated by S. Posch

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

    unsigned long fill = fillval;			// Fill value for mapimg

    long status;				// Return status flag for gctp() call
    long in_line, in_samp;			// Input image coordinates of a point


    jt_time debugTimer;
    debugTimer.start();


    parse_input(mapimginfilename, mapimgoutfilename);

    // Initialize input & output space image & projections
    ioreturnval = init_io(&inimg, &outimg, useType );

    if(!ioreturnval)
    {
	remove("abctempxyz31199.proj");
	remove("abctempxyz311992.proj");
	mapimgdial->setEnabled(true);
	return true;
    }

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


    /* jtrent resample */
    static int call_count = 0;
    FILE* bobFile = fopen( "bob.img", "wr" );
    void* bobBuffer = malloc( sizeof( type )* outimg.ns );


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





/********** resample jtrent *******************/

                            find2corners = 0;	// must get all 4 corners at beg. of line

			    double inbox[5][2];

			    for( int inbox_i = 0; inbox_i < 5; inbox_i++ )
			         for( int inbox_j = 0; inbox_j < 2; inbox_j++ )
			              inbox[inbox_i][inbox_j] = 0;

                            call_count++;

                            unsigned char *fill_bob = new unsigned char;
                            *fill_bob = (unsigned char)(fillval);

                            if( get_coords( outimg, inimg, out, inbox, out_line, out_samp, paramfile ) )
                            {
                            	printf( "get_coords call %i returned these points:\n", call_count );

                            	for( int indexii = 0; indexii < 5; indexii++ )
                            	{
                            		printf( "\t ( %f, %f )\n", inbox[indexii][0], inbox[indexii][1] );
                            	}

//                                in_line = ((inimg.ul_y - in[1]) / inimg.pixsize) + 0.5;
//			        in_samp = ((in[0] - inimg.ul_x) / inimg.pixsize) + 0.5;

			        // Still inside the input image??
  			        // ------------------------------
             			if (in_line<0||in_samp<0||in_line>=inimg.nl||in_samp>=inimg.ns)
                		{
                         	   //fwrite( fill_bob, sizeof( type ), 1, bobFile );
			           *( (type*)bobBuffer + out_samp) = 33;
			        }

			        // Assign the appropriate input image pixel to the output image using
           			// nearest-neighbor resampling. (...the half-pixel rounding is done in
	            		// the in_line, in_samp calculate above...)
	              		// ---------------------------------------------
		        	else
			        {

                                  //Loads into memory the current line of input needed
                                  get_line(  mapimginbuf, inbox[4][1]*inimg.ns, inimg.ns+1, useType );


/*
                                  //----- compute minbox -----//
	                          maxx = maxy = 0; minx = inimg.ns; miny = inimg.ns;
	                          for(i = 0; i < 4; ++i)
	                          {
	                                if(maxx < inbox[i][0])
		                           maxx = inbox[i][0];
	                                else
	                                {
	                                   if(minx > inbox[i][0])
		                              minx = inbox[i][0];
	                                }

	                                if(maxy < inbox[i][1])
		                           maxy = inbox[i][1];
	                                else
	                                {
	                                    if(miny > inbox[i][1])
		                               miny = inbox[i][1];
	                                }
	                        } //for i

                                //----- check each pixel in minbox & compile stats -----//
	                        boxerr = 1;
	                        for(cury = (long)(miny+0.5); cury <= (long)(maxy+0.5); ++cury)
	                        {
	                            coord[1] = cury;
	                            for(curx = (long)(minx+0.5); curx <= (long)(maxx+0.5); ++curx)
	                            {
	                                coord[0] = curx;
	                                if(inBox(inbox, coord))
	                                {
                                 	      boxerr = 0;
                                              temp = inbuf[(long)(coord[1]*inimg.ns + coord[0])];
	                                }//if inBox
	                            }// for curx
	                        }//for cury

                                //----- finish statistical analysis -----//
                                if(boxerr)	//no pixels from rectangle in the minbox, get NN.
	                        {
		                    choicebuf[out_samp]=255;

                                    if(doout)
                                      outbuf[out_samp] = nnbuf[out_samp];
                                }
	                        else	//!boxerr
	                        {
	                            temp = t2 = 0;
	                            for(i = 0; i < num_classes; ++i)
	                                if(classcount[i][0])
	                                {
		                            classcount[t2][1] = i;	// make list of used classes
		                            t2++;			// increment current list subscript
		                            temp++;			// increment number of choices
	                                }

                                    choicebuf[out_samp] = temp;
                         }
*/
                                  //references specific element of input
                                  *( (type*)bobBuffer + out_samp) = *(((type*)mapimginbuf + (int)(inbox[4][0])));
			        }

                            	fflush( stdout );
                            }
                            else
                            {
                            	printf( "get_coords call %i returned zero\n", call_count );
                            	fflush( stdout );
                         	//fwrite( fill_bob, sizeof( type ), 1, bobFile );
   		                *( (type*)bobBuffer + out_samp) = 33;
                            }
                       	    fflush( stdout );

/********** resample jtrent *******************/

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
			in_line = ((inimg.ul_y - in[1]) / inimg.pixsize) + 0.5;
			in_samp = ((in[0] - inimg.ul_x) / inimg.pixsize) + 0.5;

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
                            get_line(  mapimginbuf, in_line*inimg.ns, inimg.ns+1, useType );

                            //references specific element of input
			    *( (type*)mapimgoutbuf + out_samp) = *((type*)mapimginbuf + in_samp);
			}
		}
	}


	// Spit out a processing message & write the output image line to disk
	// -------------------------------------------------------------------
	put_line(mapimgoutbuf, useType);
	fwrite( bobBuffer, sizeof( type ), outsize, bobFile );
    }

    progress.setProgress( outimg.nl );

    // Close down processing & Exit... Processing is complete.
    // -------------------------------------------------------
    cleanup_input();
    cleanup_output();
    // END of MapIMG!!

    bool cancelled = false;
    // if process was Aborted, output aborted message
   fclose( paramfile );

    if(progress.wasCancelled())
    {
	remove(mapimgoutfilename);
	remove(outfilename);
	QMessageBox::information (mapimgdial, "Aborted!","MapIMG has been aborted",
					  QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	cancelled = true;
    }

    // if process Completed output completion message
    else
    {
   	QString out = "MapIMG has completed\n\nOutput Rows: " ;
	QString str = QString::number(outputRows, 10);
	out += str;
	out += "\n\nOutput Columns: ";
	str = QString::number(outputCols, 10);
	out += str;
	out += "\n";
                   fclose( bobFile );
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
