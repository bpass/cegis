// $Id: mapimg.h,v 1.14 2005/03/02 16:26:25 jtrent Exp $


#ifndef MAPIMG_H
#define MAPIMG_H

class RasterInfo;
class ResampleInfo;
class QWidget;
class QString;


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

   int round(double value, unsigned int decimals = 0);

   template <typename type>
      int quickSortAndSearch( void* values, type searchValue, int size, int left = 0, int right = -1 )
   {
      int returnValue = -1;

      if( size <= 0 )
          return returnValue;

      if( right == -1 )
         right = size -1;

      type *castValues = (type*)values;
      int left_hold_point = left;
      int right_hold_point = right;
      type pivot_value = castValues[left]; //(rand()%right)+left));

      //qDebug( QString("Here pivot = %1").arg( pivot_value ).ascii() );

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
      //        qDebug( "Here 2" );

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
      //qDebug( QString("Here 3, %1 %2 %3").arg( size ).arg( pivot_point+1 ).arg( right ).ascii() );

      if( right > pivot_point )
      {
         //qDebug( "Here 3a" );
         int rightReturn = mapimg::quickSortAndSearch<type>( values, searchValue, size, pivot_point+1, right );

         if( rightReturn != -1 )
             returnValue = rightReturn;

         //qDebug( "Here 3b" );
      }

      //qDebug( "Here 4" );


      return returnValue;
   }
};



#endif//MAPIMG_H
