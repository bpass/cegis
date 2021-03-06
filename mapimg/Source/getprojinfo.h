// $Id: getprojinfo.h,v 1.9 2006/05/19 18:55:18 lwoodard Exp $


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
#include <QWidget>
#include <QMessageBox>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QColor>
#include <QMap>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <CString>

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

extern "C"
{
#include "proj.h"
}

static char*  logFile = strdup( QDir::currentPath().append("/mapimg.log").toAscii() );
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
	// mapimg STARTS HERE!!!!
	// mapimg to do the reprojection (no longer called as function in order to provide progress dialog)
	// mapimg written by D. Steinwand and updated by S. Posch

	int outputRows = output.rows();
	int outputCols = output.cols();

	type fill = (type)output.fillValue();         // Fill value for mapimg

	IMGIO<type> imgIO( resample.cacheLineCount() );

	imgIO.parse_input(input.imgFileName(), output.imgFileName());

	IMGINFO inimg;            // Image information--input
	IMGINFO outimg;         // Image information--output

	int ioreturnval;
	long out_line, out_samp;   // Output image coordinates of a point

	double in[2]={0,0};       // Input projection coordinates of a point
	double out[2];            // Output projection coordinates of a point

	long in_line, in_samp;    // Input image coordinates of a point


	typedef QMap< type, unsigned int> StatisticMap;

	jt_time debugTimer;
	debugTimer.start();



	// Initialize input & output space image & projections
	ioreturnval = imgIO.init_io(input, output, &inimg, &outimg, useType );
	if( ioreturnval != 1 )
		return false;

	void * mapimginbuf = imgIO.mapimginbuf;
	void * mapimgoutbuf = imgIO.mapimgoutbuf;

	// Progress Dialog added in QT
	//INPUT_COLOR and OUTPUT_COLOR are defined in mapimgpalette.h
	MapimgProgressDialog progress( "Performing Transformation", "Abort", outimg.nl, &INPUT_COLOR, &OUTPUT_COLOR,
		mapimgdial, "progress", TRUE, WINDOW_FLAGS );
	progress.setWindowTitle( "Processing..." );

	// Set min Duration required for Dialog at 1 second (try to make it always show)
	progress.setMinimumDuration(1);

	// Perform inverse mapping--loop thru output image and get appropriate input
	// -----------------------------------------------------------------------------------------

	/******* Change bounds here for doing per line or per line section *******/
	FILE *paramfile = fopen( logFile, "wa");

	for(out_line = 0; out_line < outimg.nl; out_line++)       // For each output image line
	{
		// Set progress of Dialog box and cancel if process was cancelled

		progress.setProgress(out_line);

		if(progress.wasCanceled())
		{
			break;
		}

		out[1] = outimg.ul_y - (out_line * outimg.pixsize);    // Calc out-img proj_Y coord

		for (out_samp = 0; out_samp < outimg.ns; out_samp++)   // For each output image sample
		{
			out[0] = outimg.ul_x + (out_samp * outimg.pixsize); // Calc out-img proj_X coord


			double inbox[5][2];

			for( int inbox_i = 0; inbox_i < 5; inbox_i++ )
				for( int inbox_j = 0; inbox_j < 2; inbox_j++ )
					inbox[inbox_i][inbox_j] = 0;

			find2corners = 0;  // must get all 4 corners at beg. of line

			if( get_coords( outimg, inimg, out, inbox, out_line, out_samp, paramfile,
				(resample.resampleCode() == ResampleInfo::NearestNeighbor) ) )
			{
				in_line = (long int)(((inimg.ul_y - in[1]) / inimg.pixsize) + 0.5);
				in_samp = (long int)(((in[0] - inimg.ul_x) / inimg.pixsize) + 0.5);

				// Still inside the input image??
				// ------------------------------
				if (in_line<0||in_samp<0||in_line>=inimg.nl||in_samp>=inimg.ns)
				{
					*( (type*)mapimgoutbuf + out_samp) = fill; // Barely ever happens, if at all
				}

				// Assign the appropriate input image pixel to the output image using
				// nearest-neighbor resampling. (...the half-pixel rounding is done in
				// the in_line, in_samp calculate above...)
				// ---------------------------------------------
				else
				{
					if( resample.resampleCode() == ResampleInfo::NearestNeighbor ) //NN same for categorical and continuous
					{
						imgIO.get_line(  mapimginbuf, (qulonglong)inbox[4][1], inimg.ns, useType );

						if( mapimginbuf != NULL )
						{
							if( !resample.shouldIgnore( (*(((type*)mapimginbuf + (int)(inbox[4][0]))))) )
							{
								(*( (type*)mapimgoutbuf + out_samp)) = (*(((type*)mapimginbuf + (int)(inbox[4][0]))));
							}
							else
							{
								(*( (type*)mapimgoutbuf + out_samp)) = (type)resample.noDataValue();
							}
						}
						else
						{
							(*( (type*)mapimgoutbuf + out_samp)) = (type)resample.noDataValue();
						}
					}
					else   //Analysis
					{
						//----- compute minbox -----//
						int maxx = 0,
							maxy = 0,
							minx = inimg.ns,
							miny = inimg.ns;

						for(int inbox_index = 0; inbox_index < 4; ++inbox_index)
				  {
					  if(maxx < inbox[inbox_index][0])
						  maxx = mapimg::round(inbox[inbox_index][0]);
					  else
					  {
						  if(minx > inbox[inbox_index][0])
							  minx = mapimg::round(inbox[inbox_index][0]);
					  }

					  if(maxy < inbox[inbox_index][1])
						  maxy = mapimg::round(inbox[inbox_index][1]);
					  // else
					  // {
					  if(miny > inbox[inbox_index][1])
						  miny = mapimg::round(inbox[inbox_index][1]);
					  //}
				  } //for inbox_index

				  //----- check each pixel in minbox & compile stats -----//
				  //----- Calculate coverage size ----//
				  int boxError = 1;
				  int yCoverageArea = mapimg::round( maxy ) - mapimg::round( miny ) + 1;
				  int xCoverageArea = mapimg::round( maxx ) - mapimg::round( minx ) + 1;

				  if( yCoverageArea < 1 ) yCoverageArea = 1;
				  if( yCoverageArea == 1 && maxy != miny ) yCoverageArea++;

				  if( xCoverageArea < 1 ) xCoverageArea = 1;
				  if( xCoverageArea == 1 && maxx != minx ) xCoverageArea++;

				  int coverageSize =  (int)( yCoverageArea * xCoverageArea );

				  //---create memory for coverage ----//
				  void* inputCoverage = (void*)malloc( sizeof(type) * coverageSize+1 );
				  int offset = 0;
				  double coord[2] = { 0 };


				  offset = 0;

				  //------  Load data into coverage memory  ----//
				  for(int currentY = mapimg::round(miny); currentY <= mapimg::round(maxy); currentY++)
				  {
					  coord[1] = currentY;

					  //Loads into memory the current line of input needed
					  imgIO.get_line(  mapimginbuf, (qulonglong)coord[1], inimg.ns, useType );
					  if( mapimginbuf == NULL )
						  break;

					  for(int currentX = mapimg::round(minx); currentX <= mapimg::round(maxx); currentX++)
					  {
						  coord[0] = currentX;
						  if( inBox( inbox, coord ) )
						  {
							  boxError = 0;

							  type coordValue = (*(((type*)mapimginbuf + (int)coord[0])));

							  (*( (type*)inputCoverage + offset)) = coordValue;

							  if( resample.noDoubleCounting() )
								  (*(((type*)mapimginbuf + (int)coord[0]))) = (type)resample.noDataValue();

						  }//if inBox
						  else
						  {
							  *( (type*)inputCoverage + offset) = (type)resample.noDataValue();
						  }

						  offset++;
					  }// for curx
				  }//for cury

				  //----- finish statistical analysis -----//
				  if(boxError)   //no pixels from rectangle in the minbox, get NN.
				  {
					  //Loads into memory the current line of input needed
					  imgIO.get_line(  mapimginbuf, (qulonglong)inbox[4][1], inimg.ns, useType );

					  if( mapimginbuf != NULL )
					  {
						  if( !resample.shouldIgnore( (*(((type*)mapimginbuf + (int)(inbox[4][0]))))) )
						  {
							  (*( (type*)mapimgoutbuf + out_samp)) = (*(((type*)mapimginbuf + (int)(inbox[4][0]))));
						  }
						  else
						  {
							  (*( (type*)mapimgoutbuf + out_samp)) = (type)resample.noDataValue();
						  }
					  }
					  else
					  {
						  (*( (type*)mapimgoutbuf + out_samp)) = (type)resample.noDataValue();
					  }

					  if( resample.noDoubleCounting() )
						  (*(((type*)mapimginbuf + (int)(inbox[4][0])))) = (type)resample.noDataValue();
				  }
				  else   //!boxError
				  {
					  int coverageIndex = 0;
					  type dataValue = (type)0;
					  bool allIgnored = true;
					  StatisticMap coverageMap;
					  QString coverageString;
					  int bob;

					  int firstPosition = -1;
					  int lastPosition = -1;

					  int effectiveSize = -1;

					  switch( resample.resampleCode() )
					  {
					  case ResampleInfo::Sum:   //Sum
						  if( resample.isCategorical() )
						  {
							  QMessageBox::critical( &progress, "Error", "Sum resampling is not supported for categorical data." );
							  progress.cancel();
						  }
						  else
						  {
							  for( coverageIndex = 0;  coverageIndex < coverageSize; coverageIndex++ )
							  {
								  if( !resample.shouldIgnore( *( (type*)inputCoverage + coverageIndex )) )
								  {
									  dataValue += *( (type*)inputCoverage + coverageIndex );
									  allIgnored = false;
								  }
							  }
						  }
						  break;
					  case ResampleInfo::Mean:   //Avg
						  if( resample.isCategorical() )
						  {
							  QMessageBox::critical( &progress, "Error", "Mean resampling is not supported for categorical data." );
							  progress.cancel();
						  }
						  else
						  {
							  for( coverageIndex = 0; coverageIndex < coverageSize; coverageIndex++ )
							  {
								  if( !resample.shouldIgnore(*( (type*)inputCoverage + coverageIndex )) )
								  {
									  dataValue += *( (type*)inputCoverage + coverageIndex );
									  allIgnored = false;
								  }
							  }
							  dataValue /= coverageSize;
						  }
						  break;
					  case ResampleInfo::Median:  //Median
						  dataValue = (type)resample.noDataValue();

						  firstPosition = lastPosition = mapimg::quickSortAndSearch<type>( inputCoverage, (type)resample.noDataValue(), coverageSize  );

						  for( bob = firstPosition; bob >= 0 && bob < coverageSize; bob-- )
						  {
							  if( !resample.shouldIgnore(*((type*)inputCoverage + bob)) )
							  {
								  firstPosition = bob+1;
								  bob = -1;
								  break;
							  }
							  else
								  firstPosition = bob;
						  }

						  for( bob = lastPosition; bob >= 0 && bob < coverageSize; bob++ )
						  {
							  if( !resample.shouldIgnore(*((type*)inputCoverage + bob)) )
							  {
								  lastPosition = bob;
								  bob = -1;
								  break;
							  }
							  lastPosition = bob+1;
						  }

						  effectiveSize = coverageSize - ( lastPosition - firstPosition );


						  if( effectiveSize > 0 )
							  allIgnored = false;

						  if( effectiveSize%2 == 0 )   //even number of elements
						  {
							  int index_point1 = (effectiveSize-2)/2;
							  int index_point2 = effectiveSize/2;

							  if( index_point1 >= 0 && index_point1 >= firstPosition && firstPosition > -1 )
							  {
								  if( firstPosition == 0 )
								  {
									  index_point1 = lastPosition + index_point1;
								  }
								  else
								  {
									  index_point1 = lastPosition + (index_point1%firstPosition);
								  }
							  }
							  if( index_point2 >= 0 && index_point2 >= firstPosition && firstPosition > -1 )
							  {
								  if( firstPosition == 0 )
								  {
									  index_point2 = lastPosition + index_point2;
								  }
								  else
								  {
									  index_point2 = lastPosition + (index_point2%firstPosition);
								  }
							  }

							  if( resample.isCategorical() )
							  {
								  dataValue = *((type*)inputCoverage + index_point1);
							  }
							  else
							  {
								  dataValue = *((type*)inputCoverage + index_point1);
								  dataValue += *((type*)inputCoverage + index_point2);
								  dataValue /= 2;
							  }
						  }
						  else                        //odd number of elements
						  {
							  int index_point = (effectiveSize-1)/2;

							  //if( index_point >= 0 && index_point < firstPosition ) //don't do anything these numbers are fine
							  if( index_point >= 0 && index_point >= firstPosition && firstPosition > -1 )
							  {
								  if( firstPosition == 0 )
								  {
									  index_point = lastPosition + index_point;
								  }
								  else
								  {
									  index_point = lastPosition + (index_point%firstPosition);
								  }
							  }

							  //This case is the same for categorical and continuous
							  dataValue = *((type*)inputCoverage + index_point);
						  }
						  break;
					  case ResampleInfo::Mode:   //Mode same for categorical and continuous
						  coverageMap.clear();
						  dataValue = (type)resample.noDataValue();
						  for( coverageIndex = 0;  coverageIndex < coverageSize; coverageIndex++ )
						  {
							  if( !resample.shouldIgnore( *( (type*)inputCoverage + coverageIndex )) )
							  {
								  coverageMap[ *( (type*)inputCoverage + coverageIndex ) ]++;
								  allIgnored = false;
							  }
						  }
						  if( !coverageMap.empty() )
						  {
							  unsigned int maxCount = 0;
							  type modeValue = (type)resample.noDataValue();

							  for( typename QMap<type,unsigned int>::Iterator it = coverageMap.begin();
								  it != coverageMap.end();
								  ++it )
							  {
								  if( it.value() > maxCount )
								  {
									  maxCount = it.value();
									  modeValue = it.key();
								  }
							  }
							  dataValue = modeValue;
						  }

						  break;
					  case ResampleInfo::Min:      //Min same for categorical and continuous
						  dataValue = (type)Q_UINT64_MAX;

						  for( coverageIndex = 0; coverageIndex < coverageSize; coverageIndex++ )
						  {
							  if( ( *( (type*)inputCoverage + coverageIndex ) < dataValue ) &&
								  ( !resample.shouldIgnore(*( (type*)inputCoverage + coverageIndex ))) )
							  {
								  dataValue = *( (type*)inputCoverage + coverageIndex );
								  allIgnored = false;
							  }
						  }
						  break;
					  case ResampleInfo::Max:      //Max same for categorical and continuous
						  dataValue = (type)Q_INT64_MIN;
						  for( coverageIndex = 0; coverageIndex < coverageSize; coverageIndex++ )
						  {
							  if( ( *( (type*)inputCoverage + coverageIndex ) > dataValue ) &&
								  ( resample.shouldIgnore(*( (type*)inputCoverage + coverageIndex ))) )
							  {
								  dataValue = *( (type*)inputCoverage + coverageIndex );
								  allIgnored = false;
							  }
						  }
						  break;
					  case ResampleInfo::Bilinear: //Only used for continuous data.
						  if( resample.isCategorical() )
						  {
							  QMessageBox::critical( &progress, "Error", "Mean resampling is not supported for categorical data." );
							  progress.cancel();
						  }
						  else
						  {
							  float f00, f10, f01, f11;
							  dataValue = 0;
							  dataValue = *((type*)inputCoverage - coverageIndex);

							  if( (coord[0] < inimg.ns-1) && (coord[1] < inimg.nl-1) )
							  {   //Assumes the point falls in the top left box.
								  imgIO.get_line( mapimginbuf, (qulonglong)(coord[1]), inimg.ns, useType );
								  f00 = (*(((type*)mapimginbuf + (int)(coord[0]))));
								  f10 = (*(((type*)mapimginbuf + (int)(coord[0]+1))));
								  imgIO.get_line( mapimginbuf, (qulonglong)(coord[1]+1), inimg.ns, useType );
								  f01 = (*(((type*)mapimginbuf + (int)(coord[0]))));
								  f11 = (*(((type*)mapimginbuf + (int)(coord[0]+1))));

								  dataValue = (f10 - f00)*.5 + (f01 - f00)*.5 + (f11 + f00 - f01 - f10)*.5*.5 + f00;
							  }
							  allIgnored = false;
						  }
						  break;
					  case ResampleInfo::CubicConvolution:
						  if( resample.isCategorical() )
						  {
							  QMessageBox::critical( &progress, "Error", "Mean resampling is not supported for categorical data." );
							  progress.cancel();
						  }
						  else
						  {
							  dataValue = 0;
							  dataValue = *((type*)inputCoverage - coverageIndex);
							  if( ( coord[1] > 1 ) && ( coord[1] < inimg.nl-1 ) && 
								  ( coord[0] > 1 ) && ( coord[0] < inimg.ns-1 ) )
							  {
								  // | f00 | f01 | f02 | f03 |
								  // | f10 | f11 | f12 | f13 |
								  // | f20 | f21 | f22 | f23 |
								  // | f30 | f31 | f32 | f33 |

								  float f00,f01,f02,f03 = 0; //each represents a value of the pixels in the row
								  //closest to the point of interest.
								  float vertical[4] = {0};
								  double x,y=0; //the position of our point inside of the 2x2 grid.

								  //Automatically assume point is center of f22
								  for( int i = 0; i < 4; i++ )
								  {
									  imgIO.get_line( mapimginbuf, (qulonglong)(coord[1] + (i-2)), inimg.ns, useType );
									  f00 = (*(((type*)mapimginbuf + (int)(coord[0]-2))));
									  f01 = (*(((type*)mapimginbuf + (int)(coord[0]-1))));
									  f02 = (*(((type*)mapimginbuf + (int)(coord[0]))));
									  f03 = (*(((type*)mapimginbuf + (int)(coord[0]+1))));

									  x = (1/8.0);

									  vertical[i] = f00*( 4 - 8*(1+x) + 5*(1+x)*(1+x) - (1+x)*(1+x)*(1+x) ) +
												    f01*( 1 - 2*x*x + x*x*x ) +
													f02*( 1 - 2*(1-x)*(1-x) + (1-x)*(1-x)*(1-x) ) +
													f03*( 4 - 8*(2-x) + 5*(2-x)*(2-x) - (2-x)*(2-x)*(2-x) ) + 
													f00;
								  }

								  y = (1/8.0); 

								  dataValue = vertical[0]*( 4 - 8*(1+y) + 5*(1+y)*(1+y) - (1+y)*(1+y)*(1+y) ) +
											  vertical[1]*( 1 - 2*y*y + y*y*y ) +
											  vertical[2]*( 1 - 2*(1-y)*(1-y) + (1-y)*(1-y)*(1-y) ) +
									          vertical[3]*( 4 - 8*(2-y) + 5*(2-y)*(2-y) - (2-y)*(2-y)*(2-y) ) + 
											  vertical[0];
							  }
							  allIgnored = false;

						  }
						  break;
					  default:
						  dataValue = (type)resample.noDataValue();
						  break;
					  }

					  if( !allIgnored )
					  {
						  *( (type*)mapimgoutbuf + out_samp) = dataValue;
					  }
					  else
					  {
						  *( (type*)mapimgoutbuf + out_samp) = (type)resample.noDataValue();
					  }
				  }

				  free( inputCoverage );
					}
				}
			}
			else
			{
				*( (type*)mapimgoutbuf + out_samp) = fill;
			}

		}


		// Spit out a processing message & write the output image line to disk
		// -------------------------------------------------------------------
		imgIO.put_line(mapimgoutbuf, useType);
	}

	progress.setProgress( outimg.nl );

	// Close down processing & Exit... Processing is complete.
	// -------------------------------------------------------
	imgIO.cleanup_input();
	imgIO.cleanup_output();
	// END of mapimg!!

	bool cancelled = false;
	// if process was Aborted, output aborted message
	fclose( paramfile );

	// If mapimgdial is false then don't display the completion message
	// instead return true if the process wasn't canceled;
	if( mapimgdial == 0 )
		return (!progress.wasCanceled());

	if(progress.wasCanceled())
	{
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

		//debug timing purposes
		out += "\nDuration: ";
		out += debugTimer.stop();
		out += "\n";

		if( QFile::exists( logFile ) )
		{
			QFile appendLog( logFile );
			appendLog.open( QIODevice::WriteOnly | QIODevice::Append );
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
			logform = new logForm(0,true,WINDOW_FLAGS);
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
	imgIO.parse_input(input.imgFileName().toAscii(), output.imgFileName().toAscii());
	IMGINFO inimg, outimg;
	imgIO.init_io(input, output, &inimg, &outimg, useType );

	void * mapimginbuf = imgIO.mapimginbuf;
	void * mapimgoutbuf = imgIO.mapimgoutbuf;

	MapimgProgressDialog progress( "Down Sample Input", "Abort", outimg.nl,
		&INPUT_COLOR, &ABOUTFORM_COLOR, mapimgdial, "progress", TRUE, WINDOW_FLAGS );
	progress.setWindowTitle( "Sampling..." );
	progress.setMinimumDuration(1);

	long outX, outY;
	for(outY = 0; outY < outimg.nl; outY++)       // For each output image line
	{
		// Set progress of Dialog box and cancel if process was cancelled
		progress.setProgress( outY );

		if(progress.wasCanceled())
			break;

		imgIO.get_line( mapimginbuf, (qulonglong)(outY*pixRatio), inimg.ns, useType );

		if( mapimginbuf == NULL )
			break;

		for(outX = 0; outX < outimg.ns; outX++)   // For each output image sample
		{
			(*( (type*)mapimgoutbuf + outX)) = (*(((type*)mapimginbuf + (int)(outX*pixRatio))));
		}

		imgIO.put_line( mapimgoutbuf, useType );
	}

	progress.setProgress( progress.maximum() );

	return true;
}

#endif
