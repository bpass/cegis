
#ifndef _JPEGIMAGEOFILE_H_
#define _JPEGIMAGEOFILE_H_

/*! 
 *
 * \author Mark Schisler, USGS, MCMC, mschisler@usgs.gov
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file JPEGImageOFile.h
 * 
 * \brief A class wrapper implemented around UNIX's jpeglib,
 * a library of C system calls which make manipulation of JPEG 
 * images "easier."  This particular wrapper will be able to 
 * output JPEG images from raw data.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 * 
 */

#include <iostream>
#include <stdio.h>
#include <fstream>

extern "C" { 

   #include "jpeglib.h"
   #include "jerror.h"
    
}
    
#include <ImageLib/RGBPalette.h>
#include <ImageLib/ImageOFile.h>
#include <ImageLib/ImageException.h>

namespace USGSImageLib
{
    /*! JPEGImageOFile is an object whose purpose it is to output JPEG
     *  image files. 
     */
    class JPEGImageOFile : public ImageOFile  
    {
        public:
            
            JPEGImageOFile( std::string tfilename, 
                            long height, 
                            long width,
                            int  color_type,
                            int quality
                          )
                throw (ImageException, std::bad_alloc);
          
            virtual ~JPEGImageOFile();

            // Virtual functions to outline behaviour 
            virtual void setPalette(Palette* p)
                throw (ImageException, std::bad_alloc);

            // get/put functions use column major order, ie (x,y)==(column,row)
            virtual void putPixel(long x, long y, AbstractPixel* outpixel)
                throw (ImageException, std::bad_alloc);
            virtual void putScanline(long row, RGBPixel* outarray)
                throw (ImageException, std::bad_alloc);
            virtual void putScanline(long row, GreyPixel* outarray)
                throw (ImageException, std::bad_alloc);
          
            // These are set to voids as there can be many different data 
            // types output
            virtual void putRawPixel(long int x,long int y, void* outpixel)
                throw (ImageException, std::bad_alloc);
            virtual void putRawScanline(long row, void* outarray)
                throw (ImageException, std::bad_alloc);
            virtual void putRawRectangle(long int x1, long int y1,
                                         long int x2, long int y2,
                                         void* outarray)
                throw (ImageException, std::bad_alloc);
            
        private:
            long int m_currentLine;
            
            jpeg_compress_struct m_jInfo;
            jpeg_error_mgr m_jError;

            JSAMPROW m_row[1];
            
            FILE * m_jFile;
    };

} // USGSImageLib

#endif // _JPEGIMAGEOFILE_H_

