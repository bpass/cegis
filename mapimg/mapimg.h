// $Id: mapimg.h,v 1.13 2005/02/27 05:30:01 rbuehler Exp $


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
      void quicksort( void* values, int size, int left = 0, int right = -1 )
   {
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
            right--;
         if( left != right )
        	{
            castValues[left] = castValues[right];
            left++;
        	}

         while( (castValues[left] <= pivot_value) && (left < right) )
            left++;
         if( left != right )
        	{
            castValues[right] = castValues[left];
            right--;
        	}
      }
      //        qDebug( "Here 2" );

      castValues[left] = pivot_value;
      int pivot_point = left;
      left = left_hold_point;
      right = right_hold_point;

      if( left < pivot_point )
         mapimg::quicksort<type>( values, size, left, pivot_point-1 );

      //qDebug( QString("Here 3, %1 %2 %3").arg( size ).arg( pivot_point+1 ).arg( right ).ascii() );

      if( right > pivot_point )
      {
         //qDebug( "Here 3a" );
         mapimg::quicksort<type>( values, size, pivot_point+1, right );
         //qDebug( "Here 3b" );
      }

      //qDebug( "Here 4" );


      return;
   }

};



#endif//MAPIMG_H
