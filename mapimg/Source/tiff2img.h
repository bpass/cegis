// $Id: tiff2img.h,v 1.3 2005/08/17 19:44:32 lwoodard Exp $


#ifndef TIFF2IMG_H
#define TIFF2IMG_H

#include <qmap.h>
#include <QMessageBox>
#include <QFile>

#include <tiff.h>
#include <tiffio.h>

//tiff2img
//pre: takes 2 filenames
//post: converts the tiffFile to a generic binary file file saved at
//    imgFile.
bool tiff2img( QString tiffFile, QString imgFile )
{
   bool retVal = false;
   unsigned int y = 0, x = 0, index = 0;

   QMap<int,int> qConvertMap;

   if( tiffFile == "" || imgFile == "" )
      return retVal;

   if( QFile::exists( tiffFile ) && tiffFile.right(3)=="tif" )
   {
      TIFF* tif = TIFFOpen(tiffFile.ascii(), "r");

      if (tif)
      {
         uint32 w, h;
         size_t npixels;
         uint32* raster;

         TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
         TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
         npixels = w * h;
         raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));

         if (raster != NULL)
         {
            if (TIFFReadRGBAImageOriented(tif, w, h, raster, ORIENTATION_TOPLEFT, 0))
            {
               uint32  value = 0xffffffff;

               QMap<int,unsigned char> qConvertMap;
               QMap<int,unsigned char>::Iterator it;
               unsigned char **outMap;
               outMap = new unsigned char*[w];

               unsigned char nextEntry = 0;

               for( index = 0; index < w; index++ )
                  outMap[index] = new unsigned char[h];

               for( y = 0; y < h; y++ )
               {
                  for( x = 0; x < w; x++ )
              	   {
                     value = raster[y*w+x];
                     if( value != 1 )
                     {
                        it = qConvertMap.find( value );
                        if( it == qConvertMap.end() )
                        {
                           qConvertMap[value] = nextEntry;
                           nextEntry++;
                           outMap[x][y] = qConvertMap[value];
                        }
                        else
                           outMap[x][y] = qConvertMap[value];
                     }
                  }
               }
               _TIFFfree(raster);

               QFile newImg( imgFile );
               newImg.open( QIODevice::WriteOnly );

               for( y = 0; y < h; y++ )
               {
                  for( unsigned int x = 0; x < w; x++ )
                  {
                     newImg.putch( outMap[x][y] );
                  }
               }
               newImg.close();
            }
            TIFFClose(tif);
         }
         retVal = true;
      }
   }

   return retVal;
}

#endif
