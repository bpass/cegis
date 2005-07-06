
/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file PNGImageOFile.cpp
 * 
 * \brief A class wrapper implemented around UNIX's libpng,
 * a library of C system calls which make manipulation of PNG 
 * images "easier."  This particular wrapper will be able to 
 * output PNG images from raw data.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "PNGImageOFile.h"

/*****************************************************************************/

namespace USGSImageLib 
{

PNGImageOFile::PNGImageOFile( std::string tfilename, 
                              long height, 
                              long width,
                              int photometric, 
                              int bit_depth, 
                              int interlace_type,
                              bool alpha_channel )
throw (ImageException, std::bad_alloc)
  : ImageOFile(tfilename, width, height),
    m_colorPalette(NULL),
    m_pngPtr(NULL),
    m_infoPtr(NULL),
    m_pngFile(NULL),
    m_currentLine(0),
    m_numberPasses(0),
    m_bytesPerLine(0)
{
    png_uint_32 theight = height, twidth = width;
    png_color_16 black;
    int color_type = 0;
    
    // set up the background color for the "alpha" 
    // channel.  Could add feature later to set this.
    black.index = 0;
    black.red = 0;
    black.green = 0;
    black.blue = 0;
    black.gray = 0;
        
    try {

        // set the Image Lib image type
        ImageType = IMAGE_PNG;

        // open the PNG image file
        m_pngFile = fopen( tfilename.c_str(), "wb" );
        
        if ( !m_pngFile )
            throw ImageException();

        
        // setup the PNG structures, and bail if there is a failure
        m_pngPtr = png_create_write_struct( PNG_LIBPNG_VER_STRING, 
                                            NULL,
                                            NULL,
                                            NULL );
        if ( !m_pngPtr ) 
            throw ImageException();

        
        m_infoPtr = png_create_info_struct(m_pngPtr);

        if ( !m_infoPtr ) 
            throw ImageException();
        
        
        // set a place to jump to in case we encounter an irrecoverable 
        // error in the writing of the PNG file.
        if ( setjmp(png_jmpbuf(m_pngPtr)) )
            throw ImageException();    
        
        // initialize the PNG file output.
        png_init_io( m_pngPtr, m_pngFile );
        
        // set the height and width for writing.
        Width = width;
        Height = height;
        
        // set up for best compression level with libz
        png_set_compression_level(m_pngPtr, Z_BEST_COMPRESSION);
        
        // NOTE: using default filter settings, possible
        // feature later to add different filter options.
       
        // set the interlace handling stuff
        m_numberPasses = png_set_interlace_handling(m_pngPtr);

        //branch on the color type
        switch(photometric)
        {
            case PHOTO_GRAY: 
                // gray scale case (bit depths 1, 2, 4, 8, 16)
                if ( !alpha_channel ) 
                {
                    color_type = PNG_COLOR_TYPE_GRAY;
                    Photometric = PHOTO_GRAY;
                    if (bit_depth <= 8)
                    {
                        bitsPerSample = 8;
                        png_set_expand(m_pngPtr);  //expand out to 8 bits
                    } else
                        bitsPerSample = 16;
                    samplesPerPixel = 1;
                
                // gray scale with alpha (bit depths 8, 16)
                } else  
                { 
                    color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
                    Photometric = PHOTO_GRAY;
                    bitsPerSample = bit_depth;
                    samplesPerPixel = 1;

                    png_set_background( m_pngPtr, &black, 
                                       PNG_BACKGROUND_GAMMA_SCREEN, 1, 1.0 );
                }
                break;
            // RGB data (bit_depths 8, 16)
            case PNG_COLOR_TYPE_RGB:
                if ( !alpha_channel ) 
                {
                    color_type = PNG_COLOR_TYPE_RGB;
                    Photometric = PHOTO_RGB;
                    bitsPerSample = bit_depth;
                    samplesPerPixel = 3;
                    
                // RGBA data (bit_depths 8, 16)
                } else
                {
                    color_type = PNG_COLOR_TYPE_RGB_ALPHA;
                    Photometric = PHOTO_RGB;
                    bitsPerSample = bit_depth;
                    samplesPerPixel = 3;
                    // default to a black background for the moment...
                    png_set_background( m_pngPtr, &black, 
                                    PNG_BACKGROUND_GAMMA_SCREEN, 1, 1.0 );
                }
                break;
            default:
                throw ImageException();
        }

        png_set_IHDR( m_pngPtr, m_infoPtr, twidth, theight, 
                      bit_depth, color_type, interlace_type, 0, 0);

        png_write_info(m_pngPtr, m_infoPtr);
        
        m_bytesPerLine = ( Width * bitsPerSample * samplesPerPixel ) / 8;
       
    } catch ( ... )
    {
        // set the failure bit in ImageData
        setFailWBit();
        
        // destory the PNG information and data structs.
        png_destroy_write_struct( &m_pngPtr, &m_infoPtr );
        m_pngPtr = NULL;
        m_infoPtr = NULL;

        // close the PNG file
        fclose(m_pngFile);
    }
}

/*****************************************************************************/

PNGImageOFile::~PNGImageOFile()
{
    png_write_end(m_pngPtr, m_infoPtr);
    png_destroy_write_struct( &m_pngPtr, &m_infoPtr );
    fclose(m_pngFile);
}

/*****************************************************************************/

void PNGImageOFile::setPalette(Palette* p) 
    throw (ImageException, std::bad_alloc)
{
    (void)p;

    throw ImageException(IMAGE_INVALID_OP_ERR);
}

/*****************************************************************************/

// get/put functions use column major order, ie (x,y)==(column,row)
void PNGImageOFile::putPixel(long x, long y, AbstractPixel* outpixel)
    throw (ImageException, std::bad_alloc)
{
    (void)x;
    (void)y;
    (void)outpixel;
    
    throw ImageException(IMAGE_INVALID_OP_ERR);
}


/*****************************************************************************/

void PNGImageOFile::putScanline(long row, RGBPixel* outarray)
    throw (ImageException, std::bad_alloc)
{
    (void)row;
    (void)outarray;

    throw ImageException(IMAGE_INVALID_OP_ERR);
}
/*****************************************************************************/

void PNGImageOFile::putScanline(long row, GreyPixel* outarray)
    throw (ImageException, std::bad_alloc)
{
    (void)row;
    (void)outarray;

    throw ImageException(IMAGE_INVALID_OP_ERR);
}

/*****************************************************************************/

/// \note Not supported by libpng.
void PNGImageOFile::putRawPixel(long int x,long int y, void* outpixel)
    throw (ImageException, std::bad_alloc)
{
    (void)x;
    (void)y;
    (void)outpixel;
    
    throw ImageException(IMAGE_INVALID_OP_ERR);
}

/*****************************************************************************/

/// \note This function, though it accepts a 'row' argument, does not
/// support the non-sequential writing of scanlines.  This is because
/// libpng does not support non-sequential writing.  
void PNGImageOFile::putRawScanline(long row, void* outarray)
    throw (ImageException, std::bad_alloc)
{
    if( !validateRow(row))
        throw ImageException(IMAGE_BOUNDING_ERR);
 
    // does not support non-sequential writing of 
    // scanlines.
    if ( m_currentLine != row ) 
        throw ImageException(IMAGE_INVALID_OP_ERR);
    
    m_currentLine++; 
   
    try {
        
        png_write_row(m_pngPtr, static_cast<png_byte*>(outarray) );
        
    } catch ( ... ) 
    {
        setFailWBit();
        throw ImageException( IMAGE_FILE_WRITE_ERR);
    }
    
    return;
}

/*****************************************************************************/

/// \note Not supported by libpng.
void PNGImageOFile::putRawRectangle( long int x1, long int y1,
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


