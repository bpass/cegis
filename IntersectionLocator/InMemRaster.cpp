/*
 * File: InMemRaster.h
 * Purpose: This file contains the declaration of the InMemRaster 
 *          class.  This class is intended to hold a Raster file
 *          in memory for processing.  Currently it only supports
 *          8 bit word RGB format files.
 * Programmer: Ryan Stelzleni
 * Date: 7-9-04
 */


#include "InMemRaster.h"

InMemRaster::InMemRaster( GDALDataset *pRaster )
{
   int nXBlockSize, nYBlockSize, nXBlocks, nYBlocks;
   unsigned char *pucBuffer;
   GDALRasterBand *pBand;

   pBand = pRaster->GetRasterBand( 1 );


   // Make sure this file uses byte value types.
   if( pBand->GetRasterDataType() != GDT_Byte )
   {
      Error_Print( "Raster file doesn't have byte sized values.\n"
                   "The InMemRaster wasn't initialized.\n" );
      return;
   }

   // Store the spatial reference information for later.
   mpszProjectionWkt = new char[ 1 + strlen( pRaster->GetProjectionRef() ) ];
   strcpy( mpszProjectionWkt, pRaster->GetProjectionRef() );
 
   pRaster->GetGeoTransform( madAffine );

   // Start reading in the Dataset and storing it internally.
   pBand->GetBlockSize( &nXBlockSize, &nYBlockSize );
   
   nXBlocks = (pBand->GetXSize() + nXBlockSize - 1)/nXBlockSize;
   nYBlocks = (pBand->GetYSize() + nYBlockSize - 1)/nYBlockSize;

   mnXSize = pBand->GetXSize();
   mnYSize = pBand->GetYSize();

   // Allocate some huge amounts of memory (hopefully)
   pucBuffer = new unsigned char[ nXBlockSize * nYBlockSize ];
   mpadHue   = new int[ mnXSize * mnYSize ];
   mpadSat   = new int[ mnXSize * mnYSize ];
   mpadVal   = new int[ mnXSize * mnYSize ];

   if( !(mpadHue && mpadSat && mpadVal && pucBuffer) )
   {
      Error_Print( "Failure allocating internal buffers for raster.\n" );
      return;
   }

   // Read data for band 1 (red)
   readBand( pBand, mpadHue, pucBuffer, 
             nYBlocks, nXBlocks, nXBlockSize, nYBlockSize );

   // Read for band 2 (green)
   pBand = pRaster->GetRasterBand( 2 );
   readBand( pBand, mpadSat, pucBuffer, 
             nYBlocks, nXBlocks, nXBlockSize, nYBlockSize );

   // Read for band 3 (blue)
   pBand = pRaster->GetRasterBand( 3 );
   readBand( pBand, mpadVal, pucBuffer, 
             nYBlocks, nXBlocks, nXBlockSize, nYBlockSize );

   delete [] pucBuffer;
} // End constructor


InMemRaster::~InMemRaster()
{
   delete [] mpadHue;
   delete [] mpadSat;
   delete [] mpadVal;
   delete [] mpszProjectionWkt;
}


void InMemRaster::readBand( GDALRasterBand *pBand, int *pDestBuff,
                            unsigned char *pBuff, 
                            int nYBlocks, int nXBlocks,
                            int nXBlockSize, int nYBlockSize )
{
   int iYBlock, iXBlock, nXValid, nYValid;

   for( iYBlock = 0; iYBlock < nYBlocks; iYBlock++ )
   {
      for( iXBlock = 0; iXBlock < nXBlocks; iXBlock++ )
      {
         pBand->ReadBlock( iXBlock, iYBlock, pBuff );

         // Compute the portion of the block that is valid
         // for partial edge blocks.
         if( iXBlock * nXBlockSize > mnXSize )
            nXValid = mnXSize - iXBlock * nXBlockSize;
         else if( nXBlockSize > mnXSize )
            nXValid = mnXSize;
         else
            nXValid = nXBlockSize;

         if( iYBlock * nYBlockSize > mnYSize )
            nYValid = mnYSize - iYBlock * nYBlockSize;
         else if( nYBlockSize > mnYSize )
            nYValid = mnYSize;
         else
            nYValid = nYBlockSize;

         // Assign values into the destination array.
         for( int iY = 0; iY < nYValid; iY++ )
         {
            for( int iX = 0; iX < nXValid; iX++ )
            {
               pDestBuff[ /* x location + y offset */
                          (iXBlock*nXBlockSize+iX) + 
                          (iYBlock*nYBlockSize+iY)*mnXSize
                        ] = static_cast<int>( pBuff[ iX + iY*nXBlockSize ] );
            } // for iX
         } // for iY
      } // for iXBlock
   } // for iYBlock
   return;
} // End function readBand


void InMemRaster::classify( const Classifier &C )
{
   for( int y=0; y<mnYSize; ++y )
   {
      for( int x=0; x<mnXSize; ++x )
      {
         if( C.isRoad( mpadHue[ x + y*mnXSize ],
                       mpadSat[ x + y*mnXSize ],
                       mpadVal[ x + y*mnXSize ] ) )
         {
            mpadHue[ x + y*mnXSize ] = -1;
            mpadSat[ x + y*mnXSize ] = -1;
            mpadVal[ x + y*mnXSize ] = -1;
         }
      } // End for x
   } // End for y
   
   return;
} // End Function classify


void InMemRaster::smooth()
{
   int temp;

   for( int y=0; y<mnYSize; ++y )
   {
      for( int x=0; x<mnXSize; ++x )
      {
         temp = 0;
         if( (x > 0 && x < mnXSize-1) && (y > 0 && y < mnYSize-1) )
         {
            if( mpadHue[ (x-1) + (y-1)*mnXSize ] == -1 )
               ++temp;
            if( mpadHue[ (x+1) + (y-1)*mnXSize ] == -1 )
               ++temp;
            if( mpadHue[ (x-1) + (y+1)*mnXSize ] == -1 )
               ++temp;
            if( mpadHue[ (x+1) + (y+1)*mnXSize ] == -1 )
               ++temp;

            if( temp > 3 )
            {
               mpadHue[ x + y*mnXSize ] = -1;
               mpadSat[ x + y*mnXSize ] = -1;
               mpadVal[ x + y*mnXSize ] = -1;
            }
            else if( temp < 1 )
            {
               mpadHue[ x + y*mnXSize ] = 0;
               mpadSat[ x + y*mnXSize ] = 0;
               mpadVal[ x + y*mnXSize ] = 0;
            }
         }
      } // End for x
   } // End for y
   
   return;
} // End Function smooth

void InMemRaster::smoothFive()
{
   int temp;

   for( int y=2; y<mnYSize-3; ++y )
   {
      for( int x=2; x<mnXSize-3; ++x )
      {
         temp = 0;
         for( int i=y-2; i<y+3; ++i )
         {
            for( int j=x-2; j<x+3; ++j )
            {
               if( mpadHue[ j + i*mnXSize ] == -1 )
                  ++temp;
            }
         }

         if( temp > 12 )
         {
            mpadHue[ x + y*mnXSize ] = -1;
            mpadSat[ x + y*mnXSize ] = -1;
            mpadVal[ x + y*mnXSize ] = -1;
         }
         else
         {
            mpadHue[ x + y*mnXSize ] = 0;
            mpadSat[ x + y*mnXSize ] = 0;
            mpadVal[ x + y*mnXSize ] = 0;
         }
      } // End for x
   } // End for y
   
   return;
} // End Function smoothFive


void InMemRaster::convertToHSV()
{
   for( int y=0; y<mnYSize; ++y )
   {
      for( int x=0; x<mnXSize; ++x )
      {
         RGB2HSV( mpadHue[ x + y*mnXSize ],
                  mpadSat[ x + y*mnXSize ],
                  mpadVal[ x + y*mnXSize ] );
      }
   }

   return;
} // End function convertToHSV


void InMemRaster::RGB2HSV( int &red, int &green, int &blue )
{
   double mn, mx; 
   double h, s, v;
   double dred, dgreen, dblue;

   // the rgb values should be between 0 and 1
   dred   = static_cast<double>(red)   / 255;
   dgreen = static_cast<double>(green) / 255;
   dblue  = static_cast<double>(blue)  / 255;

   // find the min and max
   mx = dred;
   mn = dred;

   if ( dgreen > mx ) mx = dgreen;
   if ( dblue > mx )  mx = dblue; 
   if ( dgreen < mn ) mn = dgreen; 
   if ( dblue < mn )  mn = dblue;  

   double delta = mx - mn; 

   // Val = the max
   v = mx;  

   if( delta == 0 )
   {
      h = 0;
      s = 0;
   }
   else
   {
      s = delta / mx;

      if( dred == mx )
         h = 60 * ( (dgreen - dblue) / delta );
      else if( dgreen == mx )
         h = 60 * ( 2 + (dblue - dred) / delta );
      else // blue is the max
         h = 60 * ( 4 + (dred - dgreen) / delta );

      // Make sure h is within the range [0, 360]
      while( h < 0 )
         h += 360;

      while( h > 360 )
         h -= 360;
   }

   // Set the values of the original ints
   // s and v will be between 0 and 1 at this point, 
   // and h will be between 0 and 360.  After this is
   // done green and blue will be in the range 0 to 255.
   red   = static_cast<int>( h );
   green = static_cast<int>( s * 255 );
   blue  = static_cast<int>( v * 255 );
   return;
} // End function RGB2HSV



void InMemRaster::raster2Vector( double &x, double &y ) const
{
   x = madAffine[0] + x*madAffine[1] + y*madAffine[2];
   y = madAffine[3] + x*madAffine[4] + y*madAffine[5];
}


bool InMemRaster::vector2Raster( double &x, double &y ) const
{
   // Determinant of this matrix is easy to calculate because the
   // affine transformation always has zeros at row3, column 1 and 2.
   double dDet = madAffine[1]*madAffine[5] - madAffine[2]*madAffine[4];

   // If the determinant is zero then the matrix is not invertable.
   if( dDet == 0 )
      return false;

   // These general inverse equations work.  See math sheet for
   // explanation
   x = madAffine[5]*(x - madAffine[0]) + madAffine[2]*(madAffine[3] - y);
   x /= dDet;

   y = madAffine[4]*(madAffine[0] - x) + madAffine[1]*(y - madAffine[3]);
   y /= dDet;

   return true;
}


int InMemRaster::getHue( int x, int y ) const
{
   return mpadHue[ x + y*mnXSize ];
}

int InMemRaster::getSat( int x, int y ) const
{
   return mpadSat[ x + y*mnXSize ];
}

int InMemRaster::getVal( int x, int y ) const
{
   return mpadVal[ x + y*mnXSize ];
}

int InMemRaster::getHue( double x, double y ) const
{
   if( !vector2Raster( x, y ) )
      return -100;

   return mpadHue[ static_cast<int>(x + y*mnXSize) ];
}

int InMemRaster::getSat( double x, double y ) const
{
   if( !vector2Raster( x, y ) )
      return -100;

   return mpadSat[ static_cast<int>(x + y*mnXSize) ];
}

int InMemRaster::getVal( double x, double y ) const
{
   if( !vector2Raster( x, y ) )
      return -100;

   return mpadVal[ static_cast<int>(x + y*mnXSize) ];
}


int InMemRaster::getXSize() const
{
   return mnXSize;
}

int InMemRaster::getYSize() const
{
   return mnYSize;
}

bool InMemRaster::isRoad( int x, int y ) const
{
   return( mpadHue[x + y*mnXSize] == -1 && 
           mpadSat[x + y*mnXSize] == -1 && 
           mpadVal[x + y*mnXSize] == -1 );
}

bool InMemRaster::isRoad( double x, double y ) const
{
   int iX, iY;
   if( vector2Raster( x, y ) )
   {
      iX = static_cast<int>( x );
      iY = static_cast<int>( y );
      return( mpadHue[iX + iY*mnXSize] == -1 && 
              mpadSat[iX + iY*mnXSize] == -1 && 
              mpadVal[iX + iY*mnXSize] == -1 );
   }
   
   // If vector2Raster failed
   return false;
}


void InMemRaster::dumpToFile( const char *filename )
{
   FILE *outfile;

   outfile = fopen( filename, "wb" );
   if( !outfile )
   {
      Error_Print( "outfile failed to open\n" );
      return;
   }

   for( int y=0; y<mnYSize; ++y )
   {
      for( int x=0; x<mnXSize; ++x )
      {
         fwrite( (mpadHue + (x + y*mnXSize)), 1, sizeof(int), outfile );
         fwrite( (mpadSat + (x + y*mnXSize)), 1, sizeof(int), outfile );
         fwrite( (mpadVal + (x + y*mnXSize)), 1, sizeof(int), outfile );

      }
   }

   fclose( outfile );
   return;
} // End function dumpToFile



void InMemRaster::dumpToBinary( const char *szFilename )
{
   FILE *outfile = fopen( szFilename, "wb" );
   unsigned char c;

   if( !outfile )
   {
      Error_Print( "outfile failed to open\n" );
      return;
   }

   for( int y=0; y<mnYSize; ++y )
   {
      for( int x=0; x<mnXSize; ++x )
      {
         if( mpadHue[x + y*mnXSize] == -1 && 
             mpadSat[x + y*mnXSize] == -1 && 
             mpadVal[x + y*mnXSize] == -1 )
         {
            c = 1;
            fwrite( &c, 1, sizeof( char ), outfile );
         }
         else
         {
            c = 0;
            fwrite( &c, 1, sizeof( char ), outfile );
         }
      }

   }

   fclose( outfile );
   return;
}
