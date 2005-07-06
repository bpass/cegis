/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
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

ProjImageOut::ProjImageOut ( const ProjLib::Projection & proj,
                             ProjIOLib::ProjectionWriter& writer,
                             std::string filename,
                             std::pair<long int ,long int> heightThenWidth,
                       	     const ProjImageScale & newScale,
                             int photometric,
                             int bps,
                             int spp,
                             DRect bounds,
                             USGSImageLib::RGBPalette * pal )
    : ProjImageData(bounds, NULL, &proj), 
      m_writer(writer),
      m_pal(pal)
{
   this->setBounds(bounds);
    
    m_scale = newScale;
    m_scale.setProjSys( proj.getProjectionSystem() );
    setupImage(filename, heightThenWidth, newScale, photometric, bps, spp ); 
}

/****************************************************************************/

ProjImageOut::~ProjImageOut()
{
    std::cout << "ProjImageOut destructor called." << std::endl;
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
    std::cout << strExtension << std::endl;
    
    if ( !cmp_nocase( strExtension, "TIFF" ) ||
         !cmp_nocase( strExtension, "TIF" ) ) 
    {
        std::cout << " outputing  tiff " << std::endl;
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
       std::cout << " outputing  jpeg " << std::endl;
       if(!( m_file = new(nothrow)JPEGImageOFile( filename,
                       heightThenWidth.first,
                       heightThenWidth.second,
                       photometric,
                       70 )))
           throw std::bad_alloc();
        
    } else if ( !cmp_nocase( strExtension, "PNG" ) ) 
    {  
       std::cout << " outputing png " << std::endl;
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

}
