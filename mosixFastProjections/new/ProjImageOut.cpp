/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 01:09:01 $
 *
 * \version 0.1
 * 
 * \file ProjImageOut.h 
 * 
 * \brief The ProjImageOut object is meant to be a representation 
 * of a image for an image projection and all of its 
 * implicit characteristics.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include <MiscUtils/cmp_nocase.h>
#include <PNGImageOFile.h>
#include <JPEGImageOFile.h>
#include "ProjImageScale.h"
#include "ProjImageOut.h"
#include "GeneralException.h"

namespace USGSMosix 
{
    
    
/****************************************************************************/

ProjImageOut::ProjImageOut( const ProjImageParams & params, 
                            ProjIOLib::ProjectionWriter & writer, 
                            std::string filename, 
                            std::pair<long int, long int> heightThenWidth,
                            const ProjImageScale & newScale,
                            int photometric,
                            int bps,           ///< bits per sample
                            int spp,           ///< samples per pixel 
                            DRect bounds,      ///< bounding box for image
                            USGSImageLib::RGBPalette * pal )
    : ProjImageData(bounds, NULL, params.getProjection()),
      m_params(params),
      m_writer(writer),
      m_pal(pal)
{
   this->setBounds(bounds);
   m_scale = newScale;
   setupImage(filename, heightThenWidth, newScale, photometric, bps, spp ); 
}
    
/****************************************************************************/

ProjImageOut::~ProjImageOut()
{
}

/****************************************************************************/

void ProjImageOut::setupImage( std::string filename, 
                               std::pair<long int, long int> heightThenWidth,
                               const ProjImageScale & scale,
                               int photometric,
                               int bps,
                               int spp
                             )
{
  using MiscUtils::cmp_nocase;
  using std::nothrow;
  using USGSImageLib::JPEGImageOFile;
  using USGSImageLib::PNGImageOFile;
  
  double tp[6] = {0};
  double res[3] = {0};
  ProjLib::Projection * proj = NULL;  
  std::string strExtension;
  
  try
  {
    //perform some checks
    if( heightThenWidth.first  == 0  || heightThenWidth.second == 0 ) 
        throw GeneralException("Height and width problem.");

    strExtension = getFileExtension(filename);
    
    if ( !cmp_nocase( strExtension, "TIFF" ) ||
         !cmp_nocase( strExtension, "TIF" ) ) 
    {
        // for incorrect const in projlib.
        if( m_proj != NULL )  proj = m_proj->clone();
        else throw GeneralException("No Projection.");
        
        //Create the output file
        tp[3] = this->getLeftBound(); 
        tp[4] = this->getTopBound(); 
        res[0] = scale.x;
        res[1] = scale.y;
        res[2] = 0;
        m_file = m_writer.create( proj, 
                                  filename, 
                                  heightThenWidth.second, 
                                  heightThenWidth.first, 
                                  photometric, 
                                  tp,      // bounding box
                                  res );   // scaling
        delete proj;
        
    } else if ( !cmp_nocase( strExtension, "JPEG" ) || 
                !cmp_nocase( strExtension, "JPG" ) ) 
    {
       if(!( m_file = new(nothrow)JPEGImageOFile( filename,
                       heightThenWidth.first,
                       heightThenWidth.second,
                       photometric,
                       80 ))) // JPEG quality
           throw std::bad_alloc();
        
    } else if ( !cmp_nocase( strExtension, "PNG" ) ) 
    {  
       if (!( m_file = new(nothrow)PNGImageOFile( filename,
                        heightThenWidth.first,
                        heightThenWidth.second,
                        photometric,
                        bps,
                        PNG_INTERLACE_NONE )))
           throw std::bad_alloc();
    }
    
    if ( m_file != NULL ) 
    {
        setHeight(heightThenWidth.first);
        setWidth(heightThenWidth.second);
        
        //check for palette
        if( ( m_pal != NULL ) && (photometric == PHOTO_PALETTE ) )
        {
          dynamic_cast<USGSImageLib::ImageOFile*>(m_file)->setPalette(m_pal);
        }
        
        m_file->setSamplesPerPixel(spp);
        m_file->setBitsPerSample(bps);

    } else 
        throw GeneralException("No ImageFile created.");
        
  }
  catch( GeneralException & ge )
  {
    std::cout << ge.toString() << std::endl;
  }
}
      
/****************************************************************************/

void ProjImageOut::putScanline( scanline_t scanline,
                                const unsigned int& width)
{
    using USGSImageLib::ImageOFile;
    
    try {
        
        ImageOFile* file = dynamic_cast<ImageOFile*>(m_file);
        if ( file != NULL ) 
            file->putRawScanline(width, scanline);
        else 
            throw std::bad_cast();
        
    } catch ( std::bad_cast & e ) 
    {
        std::cout << "Dynamic cast failure in ProjImageOut." << std::endl;
    }

    return;
}

/****************************************************************************/

void ProjImageOut::putScanlines( scanlines_t scanlines,
                                 const unsigned int& height )
{
    using USGSImageLib::ImageOFile;
    
    try {
        
        ImageOFile* file = dynamic_cast<ImageOFile*>(m_file);
        
        if ( file != NULL ) 
        {
	        for( unsigned long int h = 0; h < height; ++h ) 
	        {
	            file->putRawScanline(h, scanlines[h]);
	        }            
            
        } else 
            throw std::bad_cast();        
        
    } catch ( std::bad_cast & e ) 
    {
        std::cout << "Dynamic cast failure in ProjImageOut." << std::endl;
    }

    return;
}

/****************************************************************************/

ProjImageOut ProjImageOut::createFromSocket( ClientSocket & socket )
{
    ProjImageParams params = ProjImageParams::createFromSocket(socket);
    ProjImageScale newScale = ProjImageScale::createFromSocket(socket);
    static ProjIOLib::ProjectionWriter writer;
    unsigned int filenameLength = 0;
    char * filename = NULL; 
    std::pair<long int, long int> heightThenWidth; 
    int photometric(0);
    int bps(0);           ///< bits per sample
    int spp(0);           ///< samples per pixel 
    DRect bounds;      ///< bounding box for image

    socket.receive(&filenameLength, sizeof(filenameLength) );
    if ( filenameLength <= 0 ) 
        throw GeneralException("Error: cannot create array of size <= 0");
    else
        filename = new char[filenameLength];

    socket.receive( filename, filenameLength); 
    socket.receive( &heightThenWidth.first, sizeof(heightThenWidth.first) );
    socket.receive( &heightThenWidth.second, sizeof(heightThenWidth.second) );
    socket.receive( &photometric, sizeof(photometric) );
    socket.receive( &spp, sizeof(spp) );
    socket.receive( &bounds, sizeof(bounds) );
    
    
    return ProjImageOut( params,writer,filename,heightThenWidth,newScale,
                         photometric, bps, spp, bounds ) ;
}

/****************************************************************************/

void ProjImageOut::exportToSocket( ClientSocket & socket )const
{
    std::string filename = getFilename();
    const unsigned int length = filename.length();
    const long unsigned int height(getHeight()), width(getWidth());
    const int photometric(getPhotometric()), spp(getSPP());
    const DRect bounds(getOuterBounds());
    
    m_params.exportToSocket(socket);
    m_scale.exportToSocket(socket); 
    socket.appendToBuffer(&length,sizeof(length));
    socket.appendToBuffer(filename.c_str(), filename.length());
    socket.appendToBuffer(&height, sizeof(height));
    socket.appendToBuffer(&width, sizeof(width));
    socket.appendToBuffer(&photometric, sizeof(photometric));
    socket.appendToBuffer(&spp, sizeof(getSPP()));
    socket.appendToBuffer(&bounds, sizeof(bounds));
    socket.sendFromBuffer();
    
    return;
}

/****************************************************************************/

}
