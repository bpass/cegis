// $Id: mapimg.h,v 1.16 2005/03/25 04:16:57 rbuehler Exp $


#ifndef MAPIMG_H
#define MAPIMG_H

class RasterInfo;
class ResampleInfo;
class QWidget;
class QString;

/*
This namespace provides functions for checking (re)projectoin parameters,
manipulating (re)projection parameters, manipulating mapimg .img files, and
the reprojection function.

Many of the functions are simply callers that parse the data type out of the
RasterInfo and calls the proper templated function found in "getprojinfo.h".
This is to make the higher level code easier to read. ( ie. mapimgForm.cpp )


--Error Checking Functions:

readytoFrameIt() will show a QMessageBox explaining any errors found that
migh make frameIt() fail.

readytoReproject() will show a QMessageBox explaining any errors found that
migh make reproject() fail.

projectionErros() returns a QString that explains errors that have to deal
the projection parameters as they stand by themselves. This QString is meant
to be show to the user so they know what to fix.


--Parameter Manipulation Functions

frameIt() will update the passed RasterInfo with the proper rows, columns, and
upper left coordinates to fully show the projection area.
NOTE: Many "bad" reprojections could be fixed by including special cases with
reduced Lat/Lon frames in this function. Examples are UTM and Polar
Stereographic.

downSizeProjection() will alter the rows, columns, upper left coordinate and
pixel size so that neither rows nor columns are larger than maxDimension.

calcFillValue() will suggest a fill value for a given input file.


--.img File Manipulation Functions

downSampleImg() is a caller for <>mapimg_downsample(). It will take the input
file and downsample it to the maxDimension and save it to where output points.
A progress bar is implemented to show the user why they are waiting.

reproject() is a caller for <>mapimg_resample. It will reproject the input
file into the output file using the designated resample method details. A
progress bar is implemented to show the user why they are waiting.


--Miscellaneous Helper Functions

<>quickSortAndSearch() does a quick sort on values and returns ... I dunno

round() is used for cross-platform compatability because not all platforms
round the same way.

*/
namespace mapimg
{
   bool readytoFrameIt( RasterInfo &input, QWidget *parent = 0 );
   bool readytoReproject( RasterInfo &input, QWidget *parent = 0 );
   QString projectionErrors( RasterInfo &input );

   void frameIt( RasterInfo &input);
   bool downSampleImg( RasterInfo &input, RasterInfo &output, int maxDimension, QWidget *parent );
   bool downSizeProjection( RasterInfo &input, int maxDimension );
   double calcFillValue( RasterInfo &input );
   bool reproject( RasterInfo &input, RasterInfo &output, ResampleInfo &resample, QWidget *parent = 0 );
   template <typename type>   // Inlined below;
      int quickSortAndSearch( void* values, type searchValue, int size, int left = 0, int right = -1 );

   int round(double value, unsigned int decimals = 0);


};

template <typename type>
inline int mapimg::quickSortAndSearch( void* values, type searchValue, int size, int left, int right )
{
   int returnValue = -1;

   if( size <= 0 )
      return returnValue;

   if( right == -1 )
      right = size -1;

   type *castValues = (type*)values;
   int left_hold_point = left;
   int right_hold_point = right;
   type pivot_value = castValues[left];

   while( left < right )
   {
      while( (castValues[right] >= pivot_value) && (left < right) )
      {

         right--;
      }
      if( left != right )
      {
         castValues[left] = castValues[right];
         left++;
      }

      while( (castValues[left] <= pivot_value) && (left < right) )
      {
         left++;
      }
      if( left != right )
      {
         castValues[right] = castValues[left];
         right--;
      }
   }

   castValues[left] = pivot_value;

   if( castValues[left] == searchValue )
      returnValue = left;

   int pivot_point = left;
   left = left_hold_point;
   right = right_hold_point;

   if( left < pivot_point )
   {
      int leftReturn = mapimg::quickSortAndSearch<type>( values, searchValue, size, left, pivot_point-1 );

      if( leftReturn != -1 )
         returnValue = leftReturn;
   }

   if( right > pivot_point )
   {
      int rightReturn = mapimg::quickSortAndSearch<type>( values, searchValue, size, pivot_point+1, right );

      if( rightReturn != -1 )
         returnValue = rightReturn;
   }

   return returnValue;
}

#endif//MAPIMG_H
