/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file ProjImageOut.h 
 * 
 * \brief Implementation file for the ProjImageOut class. 
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
      m_pal(pal),
      m_bSetupImage(false)
{
   WRITE_DEBUG ( "creating proj image out shell" << std::endl ); 
   this->setFilename(filename);
   this->setPhotometric(photometric);
   this->setHeight(heightThenWidth.first);
   this->setWidth(heightThenWidth.second);
   this->setSPP(spp);
   this->setBPS(bps);
   this->setBounds(bounds);
   m_scale = newScale;
   // setupImage();
}
    
/****************************************************************************/

ProjImageOut::~ProjImageOut()
{
}

/****************************************************************************/

void ProjImageOut::setupImage()
{
  using MiscUtils::cmp_nocase;
  using std::nothrow;
  using USGSImageLib::JPEGImageOFile;
  using USGSImageLib::PNGImageOFile;
  
  std::pair<long int, long int> heightThenWidth(getHeight(), getWidth());
  double tp[6] = {0};
  double res[3] = {0};
  ProjLib::Projection * proj = NULL;  
  std::string filename(getFilename()), strExtension;
  USGSImageLib::ImageOFile* file = NULL;  
  
  WRITE_DEBUG ( "Creating ProjImageOut" );
  
  try
  {
    //perform some checks
    if( heightThenWidth.first  == 0  || heightThenWidth.second == 0 ) 
        throw GeneralException("Height and width problem.");

    strExtension = getFileExtension(filename);
   
    // switch on file extension
    if ( !cmp_nocase( strExtension, "TIFF" ) ||
         !cmp_nocase( strExtension, "TIF" ) ) 
    {
        // for incorrect const in projlib.
        if( m_proj != NULL )  proj = m_proj->clone();
        else throw GeneralException("No Projection.");
        
        //Create the output file
        tp[3] = this->getLeftBound(); 
        tp[4] = this->getTopBound(); 
        res[0] = m_scale.x;
        res[1] = m_scale.y;
        res[2] = 0;
        file = m_writer.create( proj, 
                                filename, 
                                heightThenWidth.second, 
                                heightThenWidth.first, 
                                this->getPhotometric(), 
                                tp,      // bounding box
                                res );   // scaling
        delete proj;
        
    } else if ( !cmp_nocase( strExtension, "JPEG" ) || 
                !cmp_nocase( strExtension, "JPG" ) ) 
    {
       if(!( file = new(nothrow)JPEGImageOFile( filename,
                       heightThenWidth.first,
                       heightThenWidth.second,
                       this->getPhotometric(),
                       80 ))) // JPEG quality
           throw std::bad_alloc();
        
    } else if ( !cmp_nocase( strExtension, "PNG" ) ) 
    {  
       if (!( file = new(nothrow)PNGImageOFile( filename,
                        heightThenWidth.first,
                        heightThenWidth.second,
                        this->getPhotometric(),
                        this->getBPS(),
                        PNG_INTERLACE_NONE )))
           throw std::bad_alloc();
    }
    
    if ( file != NULL ) 
    {
        //check for palette
        if( ( m_pal != NULL ) && (this->getPhotometric() == PHOTO_PALETTE ) )
        {
          dynamic_cast<USGSImageLib::ImageOFile*>(file)->setPalette(m_pal);
        }
        
        this->setImageFile(file);
        
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

    if ( !m_bSetupImage ) 
    {
        setupImage();
        m_bSetupImage = true;
    }
    
    try {
        
        ImageOFile* file = dynamic_cast<ImageOFile*>(this->getImageFile());
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
                                 const unsigned int& beginLine,
                                 const unsigned int& endLine )
{
    using USGSImageLib::ImageOFile;
 
    if ( !m_bSetupImage ) 
    {
        setupImage();
        m_bSetupImage = true;
    }
   
    try {
        
        ImageOFile* file = dynamic_cast<ImageOFile*>(this->getImageFile());
        
        if ( file != NULL ) 
        {
	        for( unsigned long int h = beginLine; h <= endLine; ++h ) 
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

void ProjImageOut::putScanlines( const ProjImageOutPiece & piece )
{
    using USGSImageLib::ImageOFile;
        
    if ( !m_bSetupImage ) 
    {
        setupImage();
        m_bSetupImage = true;
    }
   
    try {
        
        scanlines_t scanlines = piece.getScanlines();
        const unsigned long beginLine = piece.getRange().first;
        const unsigned long endLine = piece.getRange().second;
        ImageOFile* file = dynamic_cast<ImageOFile*>(this->getImageFile());
        
        if ( file != NULL ) 
        {
	        for( unsigned long int h = beginLine; h < endLine; ++h ) 
	        {
	            file->putRawScanline(h, scanlines[h]);
	        }            
            std::cout << "rcv this many scanlines " << endLine << std::endl;
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
    DRect bounds;         ///< bounding box for image

    socket.receive(&filenameLength, sizeof(filenameLength) );
    if ( filenameLength <= 0 ) 
        throw GeneralException("Error: cannot create array of size <= 0");
    else
        filename = new char[filenameLength + 1];

    socket.receive( filename, filenameLength);
    filename[filenameLength] = '\0';
    socket.receive( &heightThenWidth.first, sizeof(heightThenWidth.first) );
    socket.receive( &heightThenWidth.second, sizeof(heightThenWidth.second) );
    socket.receive( &photometric, sizeof(photometric) );
    socket.receive( &spp, sizeof(spp) );
    socket.receive( &bounds, sizeof(bounds) );
 
    return ProjImageOut(params,writer,filename,heightThenWidth,newScale,
                        photometric, bps, spp, bounds );
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

} // namespace 

