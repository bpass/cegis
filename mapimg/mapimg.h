// $Id: mapimg.h,v 1.2 2005/01/27 18:15:13 jtrent Exp $


#ifndef MAPIMG_H
#define MAPIMG_H

class RasterInfo;
class QWidget;
class QString;

 #ifdef Q_OS_WIN32
double round(double value, unsigned int decimals = 0);
#endif

namespace mapimg
{
   bool readytoFrameIt( RasterInfo &input, QWidget *parent = 0 );
   bool readytoReproject( RasterInfo &input, QWidget *parent = 0 );
   QString projectionErrors( RasterInfo &input );

   void frameIt( RasterInfo &input);
   bool downSampleImg( RasterInfo &input, int maxDimension );
   bool downSizeProjection( RasterInfo &input, int maxDimension );
   double calcFillValue( RasterInfo &input );
   bool reproject( RasterInfo &input, RasterInfo &output, QWidget *parent = 0 );
};

#endif//MAPIMG_H

