// $Id: mapimg.h,v 1.12 2005/02/27 04:24:05 rbuehler Exp $


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

      int left_hold_point = left;
      int right_hold_point = right;
      type pivot_value = *((type*)values+((right-left)/2)); //(rand()%right)+left));

      qDebug( QString("Here pivot = %1").arg( pivot_value ).ascii() );

      while( left < right )
      {
         while( (*((type*)values+right) >= pivot_value) && (left < right) )
            right--;
         if( left != right )
        	{
            *((type*)values+left) = *((type*)values+right);
            left++;
        	}

         while( (*((type*)values+left) <= pivot_value) && (left < right) )
            left++;
         if( left != right )
        	{
            *((type*)values+right) = *((type*)values+left);
            right--;
        	}
      }
      //        qDebug( "Here 2" );

      *((type*)values+left) = pivot_value;
      int pivot_point = left;
      left = left_hold_point;
      right = right_hold_point;

      if( left < pivot_point )
         mapimg::quicksort<type>( values, size, left, pivot_point-1 );

      qDebug( QString("Here 3, %1 %2 %3").arg( size ).arg( pivot_point+1 ).arg( right ).ascii() );

      if( right > pivot_point )
      {
         qDebug( "Here 3a" );
         mapimg::quicksort<type>( values, size, pivot_point+1, right );
         qDebug( "Here 3b" );
      }

      qDebug( "Here 4" );


      return;
   }

};



#endif//MAPIMG_H
