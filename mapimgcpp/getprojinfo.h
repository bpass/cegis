// $Id: getprojinfo.h,v 1.1 2005/11/23 00:22:37 mswilliams Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

#ifndef GETPROJINFO_H
#define GETPROJINFO_H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header files for updated get_projInfo, mapimg, mapframeit, geo2eqr, and random  functions
// Created by Stephan Posch -- 8/02
// Modified by Jason Trent to allow for templating  -- 7/03
// Modified by Jason Trent and Bob Buehler to add resampling -- 2/05
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <qwidget.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qdir.h>
#include <qcolor.h>
#include <qmap.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "window_flags.h"
#include "getprojinfo.h"
#include "imgio.h"
#include "mapimg.h"
#include "mapimgform.h"

#include "mapimgdatatypes.h"
#include "mapimgpalette.h"
#include "mapimgprogressdialog.h"

#include "logform.h"
#include "jt_time.h"
#include "rasterinfo.h"
#include "resampleinfo.h"

#include "gctpcpp/transformer.h"

static char*  logFile = strdup( QDir::currentDirPath().append("/mapimg.log").ascii() );
static long   errorMode = 3;    //print both errors and parameters to a precreated
static long   paramMode = 3;    //FILE* to logFile

// Bring in the buffers
extern  void * mapimginbuf;         // Ptr to the input image (all in memory)
extern  void * mapimgoutbuf;         // Ptr to one line of output image data

/*
The heart of mapimg. Here is where all the re-projecting and resampling takes
place. Sorry for the 500+ line function. Enjoy!
*/
template <typename type>
bool mapimg_resample( const RasterInfo input, const RasterInfo output, const ResampleInfo resample, type useType, QWidget * mapimgdial)
{
   // Loop counters and loop variables
   unsigned long outRow, inRow, rowCount = output.rows();
   unsigned long outCol, inCol, colCount = output.cols();
   Coordinate outCoord, tmp1Coord, inCoord;
   outCoord.units = METER;
   double coordDelta = output.pixelSize()/2;
   bool isFill = false;

   // File I/O front end
   IMGINFO inIMG, outIMG;
   IMGIO<type> imgIO( resample.cacheLineCount() );
   imgIO.parse_input(input.imgFileName().ascii(), output.imgFileName().ascii());
   imgIO.init_io(input, output, &inIMG, &outIMG, useType );

   // File I/O Buffers
   void * mapimginbuf = imgIO.mapimginbuf;
   void * mapimgoutbuf = imgIO.mapimgoutbuf;

   // Transformer to go from output projection to input projection
   Transformer mainTransformer;
   mainTransformer.setInput( (ProjCode)output.projectionNumber(), output.allGctpParams(),
      (ProjUnit)output.unitNumber(), (ProjDatum)output.datumNumber() );
   mainTransformer.setOutput( (ProjCode)input.projectionNumber(), input.allGctpParams(),
      (ProjUnit)input.unitNumber(), (ProjDatum)input.datumNumber() );

   Transformer wrapTransformer;
   wrapTransformer.setOutput( (ProjCode)output.projectionNumber(), output.allGctpParams(),
      (ProjUnit)output.unitNumber(), (ProjDatum)output.datumNumber() );

   // Progress Bar Dialog
   MapimgProgressDialog progress( "Performing Transformation", "Abort", rowCount, &INPUT_COLOR, &OUTPUT_COLOR,
      mapimgdial, "progress", TRUE, WINDOW_FLAGS );
   progress.setCaption( "Processing..." );
   progress.setMinimumDuration(1);

   // Timer used in completion message box
   jt_time debugTimer;
   debugTimer.start();

   // Perform inverse mapping--Loop through output image and get appropriate input values
   for(outRow = 0; outRow < rowCount; outRow++)
   {
      outCoord.y = outIMG.ul_y - (outRow * outIMG.pixsize);

      progress.setProgress(outRow);
      if(progress.wasCancelled())
         break;

      for (outCol = 0; outCol < colCount; outCol++)
      {
         if( outRow == 182 && outCol == 381 )
         {
            isFill = true;
         }

         outCoord.x = outIMG.ul_x + (outCol * outIMG.pixsize);

         inCoord.copy( outCoord );
         mainTransformer.transformInverse( &inCoord );

         tmp1Coord.copy( inCoord );
         wrapTransformer.transformForward( &tmp1Coord );

         if( comparable(tmp1Coord, outCoord, coordDelta) )
         {
            mainTransformer.transformForward( &inCoord );

            inRow = mapimg::round((inIMG.ul_y - inCoord.y) / inIMG.pixsize);
            inCol = mapimg::round((inCoord.x - inIMG.ul_x) / inIMG.pixsize);

            imgIO.get_line(  mapimginbuf, (Q_ULLONG)inRow, inIMG.ns, useType );

            isFill = (mapimginbuf == 0 || mainTransformer.errored());
         }
         else
         {
            isFill = true;
         }

         if( isFill )
            (*( (type*)mapimgoutbuf + outCol)) = resample.fillValue();
         else
            (*( (type*)mapimgoutbuf + outCol)) = (*(((type*)mapimginbuf + inCol)));

      }

      imgIO.put_line(mapimgoutbuf, useType);
   }

   progress.setProgress( rowCount );

   // Close down processing & Exit... Processing is complete.
   // -------------------------------------------------------
   imgIO.cleanup_input();
   imgIO.cleanup_output();
   // END of mapimg!!

   bool cancelled = false;
   // if process was Aborted, output aborted message

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
      QString str = QString::number(rowCount, 10);
      out += str;
      out += "\n\nOutput Columns: ";
      str = QString::number(colCount, 10);
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


      //exec() will be an integer, 0 if OK is clicked, 1 if Log is clicked, or -1 if the X is clicked
      QMessageBox informationDisplay( "Completed", out,
         QMessageBox::Information,
         QMessageBox::Ok | QMessageBox::Default | QMessageBox::Escape,
         QMessageBox::Yes,
         QMessageBox::NoButton,
         mapimgdial, false );
      informationDisplay.setButtonText( QMessageBox::Yes, "View Log" );
      informationDisplay.setPalette( ABOUTFORM_COLOR );

      if( informationDisplay.exec() == QMessageBox::Yes )      //corresponding to the "View Log" Button
      {
         logForm *logform;
         logform = new logForm(0,0,true,WINDOW_FLAGS);
         logform->setPalette( AUTHORFORM_COLOR );
         logform->exec();
         delete logform;
      }

      cancelled = false;
   }

   return cancelled;
}

/*
The mapimg_downsample() function is used to create a lower resolution version
of the input file.
*/
template <typename type>
bool mapimg_downsample( const RasterInfo &input, const RasterInfo &output, type useType, QWidget *mapimgdial )
{
   double pixRatio = output.pixelSize() / input.pixelSize();
   if( pixRatio < 1 ) pixRatio = 0;

   IMGIO<type> imgIO;
   imgIO.parse_input(input.imgFileName().ascii(), output.imgFileName().ascii());
   IMGINFO inIMG, outIMG;
   imgIO.init_io(input, output, &inIMG, &outIMG, useType );

   void * mapimginbuf = imgIO.mapimginbuf;
   void * mapimgoutbuf = imgIO.mapimgoutbuf;

   MapimgProgressDialog progress( "Down Sample Input", "Abort", outIMG.nl,
      &INPUT_COLOR, &ABOUTFORM_COLOR, mapimgdial, "progress", TRUE, WINDOW_FLAGS );
   progress.setCaption( "Sampling..." );
   progress.setMinimumDuration(1);

   long outX, outY;
   for(outY = 0; outY < outIMG.nl; outY++)       // For each output image line
   {
      // Set progress of Dialog box and cancel if process was cancelled
      progress.setProgress( outY );

      if(progress.wasCancelled())
         break;

      imgIO.get_line( mapimginbuf, (Q_ULLONG)(outY*pixRatio), inIMG.ns, useType );

      if( mapimginbuf == NULL )
         break;

      for(outX = 0; outX < outIMG.ns; outX++)   // For each output image sample
      {
         (*( (type*)mapimgoutbuf + outX)) = (*(((type*)mapimginbuf + (int)(outX*pixRatio))));
      }

      imgIO.put_line( mapimgoutbuf, useType );
   }

   progress.setProgress( progress.totalSteps() );

   return true;
}

#endif
