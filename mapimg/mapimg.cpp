// $Id: mapimg.cpp,v 1.9 2005/02/05 00:16:55 rbuehler Exp $


#include "mapimg.h"

#include <qwidget.h>
#include <qdir.h>
#include <qstring.h>

#include "rasterinfo.h"
#include "resampleinfo.h"

#include "getprojinfo.h"
extern "C"
{
#include "proj.h"
}

#include <math.h>
double mapimg::round(double value, unsigned int decimals)
{
  double factor = pow(10,decimals);
  return floor((value * factor) + 0.5) / factor;
}

bool mapimg::readytoFrameIt( RasterInfo &input, QWidget * parent )
{
   QString msg = QString::null;

   if( input.datumNumber() != 19 || input.unitNumber() != 2 )
   {
      msg += "The current spheroid code and unit code are unsupported.\n"
         "\t-Only Spheroid of Radius and Meters are supported at this time.\n";
   }

   if( input.pixelSize() <= 0 )
   {
      msg += "Pixel Size must be greater than 0.\n"
         "\t-Please select an appropriate Pixel Size.\n";
   }

   if( !msg.isEmpty() )
   {
      QMessageBox::critical( parent, "Unable to frame it", msg );
      return false;
   }
   else
   {
      msg = projectionErrors( input );
      if( !msg.isEmpty() )
      {
         if( QMessageBox::warning( parent, "Bad projection parameters", msg,
            QMessageBox::Cancel, QMessageBox::Ignore ) == QMessageBox::Cancel )
            return false;
      }
   }

   return true;
}

bool mapimg::readytoReproject( RasterInfo &input, QWidget *parent )
{
   QString msg = QString::null;

   if( input.datumNumber() != 19 || input.unitNumber() != 2 )
   {
      msg += "The current spheroid code and unit code are unsupported.\n"
         "\t-Only Spheroid of Radius and Meters are supported at this time.\n";
   }

   if( input.pixelSize() <= 0 )
   {
      msg += "Pixel Size must be greater than 0.\n"
         "\t-Please select an appropriate Pixel Size.\n";
   }

   if( input.bitCount() == 0 || input.dataType().isNull() )
      msg += "Please select a valid data type for each pixel.";

   if( !msg.isEmpty() )
   {
      QMessageBox::critical( parent, "Input Error", msg );
      return false;
   }
   else
   {
      msg = projectionErrors( input );
      if( !msg.isEmpty() )
      {
         if( QMessageBox::warning( parent, "Input Error", msg,
            QMessageBox::Cancel, QMessageBox::Ignore ) == QMessageBox::Cancel )
            return false;
      }
   }

   return true;
}

QString mapimg::projectionErrors( RasterInfo &input )
{
   QString msg = QString::null;

   if( input.projectionNumber() < 0 || input.projectionNumber() > 30 )
   {
      msg += "Please select and define the projection.\n";
   }
   else
   {
      switch( input.projectionNumber() )
      {
      case 0:
         msg += "Geographic is an unsupported projection at this time.\n"
            "\t-The framing generates zeros for rows and cols.\n";
         break;
      case 1:
         msg += "UTM is an unsupported projection at this time.\n"
            "\t-The reprojection created will be uselessly distorted out of zone.";
         if( input.zoneNumber() == 0 )
            msg += "0 is not a standard UTM Zone code\n"
            "\t-Please select a number for -60 to 60 not including 0\n";
         break;
      case 2:
         msg += "State Plane Coordinates is an unsupported projection at this time.\n"
            "\t-mapimg will crash if you choose to use this projection.\n";
         break;
      case 4:
         msg += "Lambert Conformal Conic is an unsupported projection at this time.\n"
            "\t-The framing generates zeros for rows and cols.\n";
      case 3:
         if( input.gctpParam(3) == input.gctpParam(4) ||
            input.gctpParam(3) == -1*input.gctpParam(4) )
            msg += "Standard parallel values may produce invalid data.\n"
            "\t-Make sure their absolute values are not equal\n";
         break;
      case 6:
         msg += "Polar Stereographic is an unsupported projection at this time.\n"
            "\t-The framing generates huge values for rows and cols.\n";
         break;
      case 10:
         msg += "Stereographic is an unsupported projection at this time.\n"
            "\t-The framing generates huge values for rows and cols.\n";
         break;
      case 13:
         msg += "Gnomic is an unsupported projection at this time.\n"
            "\t-The framing generates zeros for rows and cols.\n";
         break;
      case 23:
         msg += "Modified Stereographic Conformal--Alaska is an unsupported projection at this time.\n"
            "\t-The framing generates zeros for rows and cols.\n";
         break;
      }
   }

   return msg;
}

void mapimg::frameIt( RasterInfo &input )
{
   double pxmin = 1000000000.0, pxmax = -1000000000.0;//Projection domain in X
   double pymin = 1000000000.0, pymax = -1000000000.0;//Projection domain in Y

   long status;
   long stepSize = 1000;
   double delta_north, delta_east;

   long inProj = 0; //Geographic
   long inUnit = 4; //Degress of Arc
   long inZone = 62; //Null Zone code
   long inDatum = 0; //Sphere of Radius 6370997
   double inParms[15] = {0.0};

   long outProj = input.projectionNumber();
   long outUnit = input.unitNumber();
   long outZone = input.zoneNumber();
   long outDatum = input.datumNumber();
   double outParms[15];
   for( int a = 0; a < 15; ++a )
      outParms[a] = input.gctpParam( a );

   double pixsize = input.pixelSize();

   FILE *paramfile = fopen( logFile, "wa");

   double inCoords[2];
   double outCoords[2];
   double ul_lon = -180.0, ul_lat = 90.0, lr_lon = 180.0, lr_lat = -90;

   // Calc projection coordinates (initially) for the four corners
   // ------------------------------------------------------------
   inCoords[0] = ul_lon;
   inCoords[1] = ul_lat;

   gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode, 
      logFile, &paramMode, logFile, paramfile, outCoords, &outProj, &outZone, 
      outParms, &outUnit, &outDatum, "", "", &status);

   if (outCoords[0] < pxmin) pxmin = outCoords[0];
   if (outCoords[0] > pxmax) pxmax = outCoords[0];
   if (outCoords[1] < pymin) pymin = outCoords[1];
   if (outCoords[1] > pymax) pymax = outCoords[1];

   inCoords[0] = lr_lon;
   inCoords[1] = lr_lat;

   gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode, 
      logFile, &paramMode, logFile, paramfile, outCoords, &outProj, &outZone, 
      outParms, &outUnit, &outDatum, "", "", &status);

   if (outCoords[0] < pxmin) pxmin = outCoords[0];
   if (outCoords[0] > pxmax) pxmax = outCoords[0];
   if (outCoords[1] < pymin) pymin = outCoords[1];
   if (outCoords[1] > pymax) pymax = outCoords[1];

	inCoords[0] = ul_lon;
	inCoords[1] = lr_lat;

   gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode, 
      logFile, &paramMode, logFile, paramfile, outCoords, &outProj, &outZone, 
      outParms, &outUnit, &outDatum, "", "", &status);

	if (outCoords[0] < pxmin) pxmin = outCoords[0];
	if (outCoords[0] > pxmax) pxmax = outCoords[0];
	if (outCoords[1] < pymin) pymin = outCoords[1];
	if (outCoords[1] > pymax) pymax = outCoords[1];

	inCoords[0] = lr_lon;
	inCoords[1] = ul_lat;

   gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode, 
      logFile, &paramMode, logFile, paramfile, outCoords, &outProj, &outZone, 
      outParms, &outUnit, &outDatum, "", "", &status);

	if (outCoords[0] < pxmin) pxmin = outCoords[0];
	if (outCoords[0] > pxmax) pxmax = outCoords[0];
	if (outCoords[1] < pymin) pymin = outCoords[1];
	if (outCoords[1] > pymax) pymax = outCoords[1];

	// Now step along the sides -- this is somewhat brute force, and it's accuracy
	//  is affected by the step size.  This method should be replaced (!!)
	// ---------------------------------------------------------------------
	delta_east = fabs(ul_lon - lr_lon) / stepSize;
	delta_north = fabs(ul_lat - lr_lat) / (stepSize/2.0);

	/* Calculate the minimum and maximum coordinates
	 ---------------------------------------------*/
	for (long i = 0; i <= stepSize; i++)
	{
		inCoords[0] = ul_lon + (delta_east * i);
		for (long j = 0; j<= (stepSize/2); j++)
		{
			inCoords[1] = lr_lat + (delta_north * j);

         gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode, 
            logFile, &paramMode, logFile, paramfile, outCoords, &outProj, &outZone, 
            outParms, &outUnit, &outDatum, "", "", &status);

	      if (outCoords[0] < pxmin) pxmin = outCoords[0];
	      if (outCoords[0] > pxmax) pxmax = outCoords[0];
	      if (outCoords[1] < pymin) pymin = outCoords[1];
	      if (outCoords[1] > pymax) pymax = outCoords[1];
		}
	}
   fclose(paramfile);

	// Set output raster area
	// ---------------------------
   input.setArea(
      pxmin + (pixsize/2),
      pymax - (pixsize/2), 
      (int)(mapimg::round( ((pymax - pymin) / pixsize) )),
      (int)(mapimg::round( ((pxmax - pxmin) / pixsize) )) );
}

bool mapimg::downSampleImg( RasterInfo &input, RasterInfo &output, int maxDimension )
{
   output.copy(input);

   output.setFileName( QDir::currentDirPath().append("/temp.img") );
   mapimg::downSizeProjection(output, maxDimension);

   ResampleInfo resample;
   resample.setResampleCode( ResampleInfo::NearestNeighbor );
   resample.setFillValue( input.fillValue() );
   resample.setNoDataValue( input.noDataValue() );

   mapimg::reproject(input, output, resample);
   output.save();


   return true;
}

bool mapimg::downSizeProjection( RasterInfo &input, int maxDimension )
{
   int r(input.rows()), c(input.cols());

   if( r == 0 || c == 0 )
      return false;

   if( r < maxDimension && c < maxDimension )
      return true;

   double pixratio;
   if( r < c )
      pixratio = static_cast<double>(c) / static_cast<double>(maxDimension);
   else
      pixratio = static_cast<double>(r) / static_cast<double>(maxDimension);

   input.setPixelSize( input.pixelSize() * pixratio );
   mapimg::frameIt( input );

   return true;
}

double mapimg::calcFillValue( RasterInfo &input )
{
   QString dtype(input.isSigned()?"Signed ":"Unsigned ");
   dtype += QString::number(input.bitCount());
   dtype += " Bit ";
   dtype += input.dataType();

   if( dtype == "Signed 64 Bit IEEE Float" )
   {	double data = 0;
      return get_max_value( input, data );   }
   else if( dtype == "Signed 32 Bit IEEE Float" )
   {	float data = 0;
      return get_max_value( input, data );   }
   else if( dtype == "Signed 32 Bit Integer" )
   {	Q_INT32 data = 0;
      return get_max_value( input, data );   }
   else if( dtype == "Unsigned 32 Bit Integer" )
   {	Q_UINT32 data = 0;
      return get_max_value( input, data );   }
   else if( dtype == "Signed 16 Bit Integer" )
   {	Q_INT16 data = 0;
      return get_max_value( input, data );   }
   else if( dtype == "Unsigned 16 Bit Integer" )
   {	Q_UINT16 data = 0;
      return get_max_value( input, data );   }
   else if( dtype == "Signed 8 Bit Integer" )
   { 	Q_INT8 data = 0;
      return get_max_value( input, data );   }
   else //( dtype == "Unsigned 8 Bit Integer" )
   {  Q_UINT8 data = 0;
      return get_max_value( input, data );   }
}

bool mapimg::reproject( RasterInfo &input, RasterInfo &output, ResampleInfo resample, QWidget *parent )
{
   QString dtype(output.isSigned()?"Signed ":"Unsigned ");
   dtype += QString::number(output.bitCount());
   dtype += " Bit ";
   dtype += output.dataType();

   if( dtype == "Signed 64 Bit IEEE Float" )
   {	double data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Signed 32 Bit IEEE Float" )
   {	float data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Signed 32 Bit Integer" )
   {	Q_INT32 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Unsigned 32 Bit Integer" )
   {	Q_UINT32 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Signed 16 Bit Integer" )
   {	Q_INT16 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Unsigned 16 Bit Integer" )
   {	Q_UINT16 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Signed 8 Bit Integer" )
   { 	Q_INT8 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else //( dtype == "Unsigned 8 Bit Integer" )
   {  Q_UINT8 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
}

