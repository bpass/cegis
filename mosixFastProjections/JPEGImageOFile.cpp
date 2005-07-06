
/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file JPEGImageOFile.cpp
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

#include "JPEGImageOFile.h"

/*****************************************************************************/

namespace USGSImageLib 
{

JPEGImageOFile::JPEGImageOFile( std::string tfilename, 
                                long height, 
                                long width,
                                int  color_type,
                                int quality
                              )
    
throw (ImageException, std::bad_alloc)
  : ImageOFile(tfilename, width, height),
    m_currentLine(0)
{
    
    std::cout << " constructing JPEGImageOFile " << std::endl;
    try { 
        
        // set the error mode
        m_jInfo.err = jpeg_std_error(&m_jError);

        // create the compression object
        jpeg_create_compress(&m_jInfo);
        
        
        // check the quality parameter's validity
        if ( quality > 100 || quality < 0 ) 
            throw ImageException();
        
        // open the output file 
        if ( ( m_jFile = fopen( tfilename.c_str(), "wb" ) ) == NULL ) 
        {
            throw ImageException();
        }
        
        // associate the output file with jpeglib's structures 
        jpeg_stdio_dest(&m_jInfo, m_jFile);

        // set image width and height
        m_jInfo.image_width = Width = width;
        m_jInfo.image_height = Height = height; 

        switch ( color_type ) 
        {
            case PHOTO_GRAY:
                m_jInfo.in_color_space = JCS_GRAYSCALE;
                m_jInfo.input_components = 1;
                break;
            case PHOTO_RGB:
                m_jInfo.in_color_space = JCS_RGB;
                m_jInfo.input_components = 3;
                break;
            default:
                throw ImageException(IMAGE_INVALID_OP_ERR);
                break;
        }
        
        jpeg_set_defaults(&m_jInfo);
        
        // .......... redundant?
        // This parameter should be true if it is desired that 
        // the resulting image always uses greyscale as the 
        // output colorspace
        // if ( bGreySpace ) 
        jpeg_set_colorspace( &m_jInfo, JCS_GRAYSCALE ); 

        // Since jpeg is a "lossy" format, one must select the 
        // quality of the resulting image, denoting how much 
        // data "loss" will take place in the saving of the 
        // image.  100 denotes, no image quality loss, 
        // 0 denotes... a lot of loss.
        jpeg_set_quality( &m_jInfo, quality, true );
       
        jpeg_start_compress(&m_jInfo, true ); 
        
    } catch ( ... ) 
    {

        std::cerr << "JPEGImageOFile: Caught exception on construction."
                  << std::endl;
        
        fclose( m_jFile );
        jpeg_destroy_compress(&m_jInfo);
    }
}

/*****************************************************************************/

JPEGImageOFile::~JPEGImageOFile()
{
    std::cout << "proper destructor being called." << std::endl;
    jpeg_finish_compress( &m_jInfo );
    fclose( m_jFile );
    jpeg_destroy_compress( &m_jInfo );
}

/*****************************************************************************/

void JPEGImageOFile::setPalette(Palette* p) 
    throw (ImageException, std::bad_alloc)
{
    (void)p;
    throw ImageException(IMAGE_INVALID_OP_ERR);
}

/*****************************************************************************/

// get/put functions use column major order, ie (x,y)==(column,row)
void JPEGImageOFile::putPixel(long x, long y, AbstractPixel* outpixel)
    throw (ImageException, std::bad_alloc)
{
    (void)x;
    (void)y;
    (void)outpixel;
    throw ImageException(IMAGE_INVALID_OP_ERR);
}


/*****************************************************************************/

void JPEGImageOFile::putScanline(long row, RGBPixel* outarray)
    throw (ImageException, std::bad_alloc)
{
    (void)row;
    (void)outarray;

    throw ImageException(IMAGE_INVALID_OP_ERR);
}
/*****************************************************************************/

void JPEGImageOFile::putScanline(long row, GreyPixel* outarray)
    throw (ImageException, std::bad_alloc)
{
    (void)row;
    (void)outarray;

    throw ImageException(IMAGE_INVALID_OP_ERR);
}

/*****************************************************************************/

void JPEGImageOFile::putRawPixel(long int x,long int y, void* outpixel)
    throw (ImageException, std::bad_alloc)
{
    (void)x;
    (void)y;
    (void)outpixel;
    
    throw ImageException(IMAGE_INVALID_OP_ERR);
}

/*****************************************************************************/

void JPEGImageOFile::putRawScanline(long row, void* outarray)
    throw (ImageException, std::bad_alloc)
{
    if( !validateRow(row))
        throw ImageException(IMAGE_BOUNDING_ERR);
   

    // does not support non-sequential writing of 
    // scanlines.
    if ( m_currentLine != row ) 
        throw ImageException(IMAGE_INVALID_OP_ERR);
   
    m_currentLine++;
    
    m_row[0] = static_cast<JSAMPLE*>( outarray );
    
    try {
        
       jpeg_write_scanlines( &m_jInfo, m_row, 1 );
       
    } catch ( ... ) 
    {
        setFailWBit();
        throw ImageException( IMAGE_FILE_WRITE_ERR);
    }
    
    return;
}

/*****************************************************************************/

/// \note Not supported by libpng.
void JPEGImageOFile::putRawRectangle( long int x1, long int y1,
                                      long int x2, long int y2,
                                      void* outarray )
    throw (ImageException, std::bad_alloc)
{
    (void)x1;
    (void)y1;
    (void)x2;
    (void)y2;
    (void)outarray;
    
    throw ImageException(IMAGE_INVALID_OP_ERR);
}

/*****************************************************************************/


} // namespace


