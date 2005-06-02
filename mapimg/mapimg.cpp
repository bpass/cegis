// $Id: mapimg.cpp,v 1.29 2005/06/02 20:11:44 rbuehler Exp $


#include "mapimg.h"

#include <qwidget.h>
#include <qdir.h>
#include <qstring.h>

#include "rasterinfo.h"
#include "resampleinfo.h"

#include "getprojinfo.h"
#include "imgio.h"
extern "C"
{
#include "proj.h"
}

#include <math.h>
int mapimg::round(double value, unsigned int decimals)
{
   double factor = pow(10,decimals);
   return (int)(floor((value * factor) + 0.5) / factor);
}

#ifndef EPSILON
#define EPSILON   1.0e-10
#endif

#ifndef DMSHALF_PI
#define DMSHALF_PI   90000000.000000
#endif//DMSHALF_PI


/*
readytoFrameIt() returns true if there are no errors or if the user chooses to
ignore any projection errors. If any map parameters required for frameit() are
incorrectly set then it will tell the user without an ignore option. If any
projection parameters are incorrect then it will tell the user, but also give
them the option to proceed anyway. This can result in crashes or infinite,
but I decided to let the user have that power if they want.
*/
bool mapimg::readytoFrameIt( const RasterInfo &input, QWidget * parent, bool checkProjection )
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
      msg.prepend( "Unable to calculate the frame.\n\n" );
      QMessageBox::critical( parent, "Calculate Frame", msg );
      return false;
   }
   else
   {
      msg = projectionErrors( input );
      if( !msg.isEmpty() && checkProjection )
      {
         if( QMessageBox::warning( parent, "Bad projection parameters", msg,
            QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ok )
            return false;
         // User has decided to ignore errors...
         // Don't say I didn't warn you!
      }
   }

   return true;
}

/*
Pretty much the same as readytoFrameIt except it also checks datatype and size
*/
bool mapimg::readytoReproject( const RasterInfo &input, QWidget *parent, bool checkProjection )
{
   QString msg = QString::null;

   if( input.rows() == 0 || input.cols() == 0 )
   {
      msg += "Rows and Columns must be greater than 0.\n"
         "\t-Use calculator button to automate values.\n";
   }
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
   {
      msg += "Please select a valid data type for each pixel.";
   }

   if( !msg.isEmpty() )
   {
      msg.prepend( "Cannot perform reprojection.\n\n" );
      QMessageBox::critical( parent, "Parameter Error", msg );
      return false;
   }
   else
   {
      msg = mapimg::projectionErrors( input );
      if( !msg.isEmpty() && checkProjection )
      {
         if( QMessageBox::warning( parent, "Projection Warning", msg,
            QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ok )
            return false;
         // User has decided to ignore errors...
         // Don't say I didn't warn you!
      }
   }

   return true;
}

/*
projectionErrors() will return a description of any errors it finds in the
projection parameters of the input.
*/
QString mapimg::projectionErrors( const RasterInfo &input )
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
      case 0:  // Geographic
         msg += "Geographic is an unsupported projection at this time.\n"
            "\t-The framing generates zeros for rows and cols.\n";
         break;
      case 1:  // UTM
         msg += "UTM is an unsupported projection at this time.\n"
            "\t-The reprojection created will be uselessly distorted out of zone.";
         if( input.zoneNumber() == 0 )
            msg += "0 is not a standard UTM Zone code\n"
            "\t-Please select a number for -60 to 60 not including 0\n";
         break;
      case 2:  // State Plane Coordinates
         msg += "State Plane Coordinates is an unsupported projection at this time.\n"
            "\t-mapimg will crash if you choose to use this projection.\n";
         break;
      case 3:  // Alber's Equal Area
         if( fabs(input.gctpParam(2) + input.gctpParam(3)) < EPSILON )
            msg += "Standard parallel values may produce invalid data.\n"
            "\t-Make sure they don't add up to zero\n";
         break;
      case 8:  // Equidistant Conic
         if( input.gctpParam( 8 ) == 0.0 )
         {  // Version A
            if( fabs(input.gctpParam(2)) < EPSILON )
               msg += "Standard parallel value may produce invalid data.\n"
               "\t-Make sure it isn't equal to zero\n";
            break;
         }
         else
         {  // Version B
            if( fabs(input.gctpParam(2) - input.gctpParam(3)) < EPSILON )
               msg += "Standard parallel values may produce invalid data.\n"
               "\t-Make sure they don't add up to zero\n";
            break;
         }
      case 9:  // Transverse Mercator
         if( input.gctpParam(2) < 0 || input.gctpParam(2) > 2 )
            msg += "Unexpected Scale Factor value.\n"
            "\t-Most usage of Scale Factor is closer to a value of 1\n";
         break;
      case 6:  // Polar Stereographic
         msg += "Polar Stereographic is an unsupported projection at this time.\n"
            "\t-The framing generates huge values for rows and cols.\n";
         break;
      case 10: // Stereographic
         msg += "Stereographic is an unsupported projection at this time.\n"
            "\t-The framing generates huge values for rows and cols.\n";
         break;
      case 13: // Gnomonic
         msg += "Gnomonic is an unsupported projection at this time.\n"
            "\t-The framing generates zeros for rows and cols.\n";
         break;
      case 15: // General Vertical Near-Side Perspective
         if( fabs(input.gctpParam(2)) < EPSILON )
            msg += "Height of 0 may produce invalid data.\n"
            "\t-Height is measured in meters above sea level.\n"
            "\t  Common values are greater than 1km (1000m).\n";
         break;
      case 20: // Hotine Oblique Mercators
         if( fabs(input.gctpParam(2)) < EPSILON || input.gctpParam(2) > 2 )
            msg += "Unexpected Scale Factor value.\n"
            "\t-Most usage of Scale Factor is closer to a value of 1\n";
         if( input.gctpParam( 12 ) == 0.0 )  // Version A
         {
            if( fabs(input.gctpParam(9) - input.gctpParam(11)) <= EPSILON )
               msg += "Specified latitudes of points 1 and 2 may produce invalid data.\n"
               "\t-Make sure they are not equal to one another\n";
            if( fabs(fabs(input.gctpParam(9)) - DMSHALF_PI) <= EPSILON || fabs(input.gctpParam(9)) <= EPSILON )
               msg += "Latitude of point 1 may produce invalid data.\n"
               "\t-Make sure it is not equal to +/-90 or 0 degrees\n";
            if( fabs(input.gctpParam(5)) == DMSHALF_PI )
               msg += "Latitude of Projection Origin may produce invalid data.\n"
               "\t-Make sure it is not equal to +/-90 degrees\n";
         }
         else  // Version B
         {
            if( fabs(fabs(input.gctpParam(5)) - DMSHALF_PI) <= EPSILON || fabs(input.gctpParam(5)) <= EPSILON )
               msg += "Latitude of Projection Origin may produce invalid data.\n"
               "\t-Make sure it is not equal to +/-90 or 0 degrees\n";
         }
         break;
      case 23: // Modified Stereographic Conformal--Alaska
         msg += "Modified Stereographic Conformal--Alaska is an unsupported projection at this time.\n"
            "\t-The framing generates zeros for rows and cols.\n";
         break;
      case 26: // Interrupted Mollweide
         msg += "Interrupted Mollweide is an unoptimized projection at this time.\n"
            "\t-It takes large amounts of time to calculate and re-project.\n";
         break;
      case 30: // Oblated Equal Area
         if( input.gctpParam(2) < 0 || input.gctpParam(2) > 2 )
            msg += "Oval Shape Parameter m may produce invalid data.\n"
            "\t-Value must be between 0 and 2.\n";
         if( input.gctpParam(3) < 2 )
            msg += "Oval Shape Parameter n may produce invalid data.\n"
            "\t-Value must be greater than 2.\n";
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
   long inDatum = 19; //Sphere of Radius 6370997
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
   double outCoords[2] = {0,0};
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

void mapimg::geo2eqr( RasterInfo &input )
{
   double pixsiz;				// Pixel Size in output image
   double pixX, pixY;
   double pxmin;				// Projection minimum in X
   double pxmax;				// Projection maximum in X
   double pymin;				// Projection minimum in Y
   double pymax;				// Projection maximum in Y

   long status;
   long i;

   long outProj = 17;
   long outUnit = 2;
   long outZone = 62;
   long outDatum = 0;
   double outParms[15];

   long inProj = 0;
   long inUnit = 4;
   long inZone = 62;
   long inDatum = 0;
   double R = 6370997.0;
   double inParms[15];

   double inCoords[2];
   double outCoords[2];

   for(i = 0; i < 15; inParms[i++] = 0.0);
   for(i = 0; i < 15; outParms[i++] = 0.0);

   pxmin = 1000000000.0;
   pymin = 1000000000.0;
   pxmax = -1000000000.0;
   pymax = -1000000000.0;

   FILE *paramfile = fopen( logFile, "wa");

   // Parse Parameters
   // ----------------
   outParms[0] = R;

   // Calc projection coordinates for the four corners.  OK, OK, for an Equirectangular
   // space & square pixels this really isn't necessary 'cuz it's easy to figure out with 2PiR
   // etc., but it's here for a check.  It should check EXACTLY...
   // ---------------------------------------------------------------------------------------
   inCoords[0] = -180.0;
   inCoords[1] = 90.0;

   gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
      paramfile, outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);

   if(outCoords[0] < pxmin) pxmin = outCoords[0];
   if(outCoords[0] > pxmax) pxmax = outCoords[0];
   if(outCoords[1] < pymin) pymin = outCoords[1];
   if(outCoords[1] > pymax) pymax = outCoords[1];


   inCoords[0] = 180.0;
   inCoords[1] = -90.0;

   gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
      paramfile,outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);

   if(outCoords[0] < pxmin) pxmin = outCoords[0];
   if(outCoords[0] > pxmax) pxmax = outCoords[0];
   if(outCoords[1] < pymin) pymin = outCoords[1];
   if(outCoords[1] > pymax) pymax = outCoords[1];


   inCoords[0] = -180.0;
   inCoords[1] = -90.0;

   gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
      paramfile,outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);

   if(outCoords[0] < pxmin) pxmin = outCoords[0];
   if(outCoords[0] > pxmax) pxmax = outCoords[0];
   if(outCoords[1] < pymin) pymin = outCoords[1];
   if(outCoords[1] > pymax) pymax = outCoords[1];


   inCoords[0] = 180.0;
   inCoords[1] = 90.0;

   gctp(inCoords, &inProj, &inZone, inParms, &inUnit, &inDatum, &errorMode,logFile,&paramMode,logFile,
      paramfile,outCoords, &outProj, &outZone, outParms, &outUnit, &outDatum, "", "", &status);

   if(outCoords[0] < pxmin) pxmin = outCoords[0];
   if(outCoords[0] > pxmax) pxmax = outCoords[0];
   if(outCoords[1] < pymin) pymin = outCoords[1];
   if(outCoords[1] > pymax) pymax = outCoords[1];


   // Calc output imagee pixel size
   // -----------------------------
   pixsiz = (2.0 * 3.141593653589793238 * R) / (double)input.cols();
   pixX = pxmin + (pixsiz / 2);
   pixY = pymax + (pixsiz / 2);

   //Save results to input
   input.setProjection( outProj, outZone, outDatum, outUnit );
   input.setPixelSize( pixsiz );
   input.setUL( pixX, pixY );
   for( i = 0; i < 15; ++i )
      input.setGctpParam(i++, outParms[i]);
}

bool mapimg::downSampleImg( const RasterInfo &input, RasterInfo &output, int maxDimension, QWidget *parent )
{
   QString tmp( output.imgFileName() );
   output.copy(input);
   if( tmp != QString::null )
      output.setFileName( tmp );
   else
      output.setFileName( QDir::currentDirPath().append("/temp_small.img") );

   mapimg::downSizeProjection(output, maxDimension);

   QString dtype(input.isSigned()?"Signed ":"Unsigned ");
   dtype += QString::number(input.bitCount());
   dtype += " Bit ";
   dtype += input.dataType();

   if( dtype == "Signed 64 Bit IEEE Float" )
   {   double data = 0;
      return mapimg_downsample( input, output, data, parent ); }
   else if( dtype == "Signed 32 Bit IEEE Float" )
   {   float data = 0;
      return mapimg_downsample( input, output, data, parent ); }
   else if( dtype == "Signed 32 Bit Integer" )
   {   Q_INT32 data = 0;
      return mapimg_downsample( input, output, data, parent ); }
   else if( dtype == "Unsigned 32 Bit Integer" )
   {   Q_UINT32 data = 0;
      return mapimg_downsample( input, output, data, parent ); }
   else if( dtype == "Signed 16 Bit Integer" )
   {   Q_INT16 data = 0;
      return mapimg_downsample( input, output, data, parent ); }
   else if( dtype == "Unsigned 16 Bit Integer" )
   {   Q_UINT16 data = 0;
      return mapimg_downsample( input, output, data, parent ); }
   else if( dtype == "Signed 8 Bit Integer" )
   {    Q_INT8 data = 0;
      return mapimg_downsample( input, output, data, parent ); }
   else //( dtype == "Unsigned 8 Bit Integer" )
   {  Q_UINT8 data = 0;
      return mapimg_downsample( input, output, data, parent ); }

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

double mapimg::calcFillValue( const RasterInfo &input )
{
   QString dtype(input.isSigned()?"Signed ":"Unsigned ");
   dtype += QString::number(input.bitCount());
   dtype += " Bit ";
   dtype += input.dataType();

   if( dtype == "Signed 64 Bit IEEE Float" )
   {   double data = 0;
      return IMGIO<double>::get_max_value( input, data );   }
   else if( dtype == "Signed 32 Bit IEEE Float" )
   {   float data = 0;
      return IMGIO<float>::get_max_value( input, data );   }
   else if( dtype == "Signed 32 Bit Integer" )
   {   Q_INT32 data = 0;
      return IMGIO<Q_INT32>::get_max_value( input, data );   }
   else if( dtype == "Unsigned 32 Bit Integer" )
   {   Q_UINT32 data = 0;
      return IMGIO<Q_UINT32>::get_max_value( input, data );   }
   else if( dtype == "Signed 16 Bit Integer" )
   {   Q_INT16 data = 0;
      return IMGIO<Q_INT16>::get_max_value( input, data );   }
   else if( dtype == "Unsigned 16 Bit Integer" )
   {   Q_UINT16 data = 0;
      return IMGIO<Q_UINT16>::get_max_value( input, data );   }
   else if( dtype == "Signed 8 Bit Integer" )
   {    Q_INT8 data = 0;
      return IMGIO<Q_INT8>::get_max_value( input, data );   }
   else //( dtype == "Unsigned 8 Bit Integer" )
   {  Q_UINT8 data = 0;
      return IMGIO<Q_UINT8>::get_max_value( input, data );   }
}

bool mapimg::reproject( const RasterInfo &input, const RasterInfo &output, const ResampleInfo &resample, QWidget *parent )
{
   QString dtype(output.isSigned()?"Signed ":"Unsigned ");
   dtype += QString::number(output.bitCount());
   dtype += " Bit ";
   dtype += output.dataType();

   if( dtype == "Signed 64 Bit IEEE Float" )
   {   double data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Signed 32 Bit IEEE Float" )
   {   float data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Signed 32 Bit Integer" )
   {   Q_INT32 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Unsigned 32 Bit Integer" )
   {   Q_UINT32 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Signed 16 Bit Integer" )
   {   Q_INT16 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Unsigned 16 Bit Integer" )
   {   Q_UINT16 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else if( dtype == "Signed 8 Bit Integer" )
   {    Q_INT8 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
   else //( dtype == "Unsigned 8 Bit Integer" )
   {  Q_UINT8 data = 0;
      return mapimg_resample( input, output, resample, data, parent);   }
}


