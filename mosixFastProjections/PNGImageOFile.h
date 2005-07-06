
#ifndef _PNGIMAGEOFILE_H_
#define _PNGIMAGEOFILE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file PNGImageOFile.h
 * 
 * \brief A class wrapper implemented around UNIX's libpng,
 * a library of C system calls which make manipulation of PNG 
 * images "easier."  This particular wrapper will be able to 
 * output PNG images from raw data.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 
 */

#include <png.h>
#include <ImageLib/RGBPalette.h>
#include <ImageLib/ImageOFile.h>
#include <ImageLib/ImageException.h>

namespace USGSImageLib
{
    /*! PNGImageOFile is an object whose purpose it is to output PNG
     *  image files. 
     */
    class PNGImageOFile : public ImageOFile  
    {
        public:
            
            PNGImageOFile( std::string tfilename, 
                           long height, 
                           long width,
                           int photometric,
                           int bit_depth,
                           int interlace_type,
                           bool alpha_channel = false )
                throw (ImageException, std::bad_alloc);
          
            virtual ~PNGImageOFile();

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
            RGBPalette * m_colorPalette;
                
            png_structp m_pngPtr;
            png_infop m_infoPtr;
            
            FILE * m_pngFile;

            long int m_currentLine;
            
            int m_numberPasses;
            long int m_bytesPerLine;
    };

} // USGSImageLib

#endif // _PNGIMAGEOFILE_H_

