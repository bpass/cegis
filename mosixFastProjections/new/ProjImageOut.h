#ifndef __USGSMOSIX_PROJIAMGEOUT_H_
#define __USGSMOSIX_PROJIMAGEOUT_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 01:09:01 $
 *
 * \version 0.1
 * 
 * \file ProjOutImageOut.h 
 * 
 * \brief The ProjImage object is meant to be a representation 
 * of a image for an image projection and all of its 
 * implicit characteristics.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include <ImageLib/ImageOFile.h>
#include <ImageLib/RGBPalette.h>
#include <string>
#include <utility>
#include "ProjImageData.h"
#include "ProjImageParams.h"
#include "ProjUtil.h"
#include "ClientSocket.h"

namespace USGSMosix
{

class ProjImageOut : private virtual ProjImageData,
                     public virtual ProjImageOutInterface
{
    public:
        
        ProjImageOut( const ProjImageParams & params, 
                      ProjIOLib::ProjectionWriter & writer, 
                      std::string filename, 
                      std::pair<long int, long int> heightThenWidth,
                      const ProjImageScale & newScale,
                      int photometric,
                      int bps,           ///< bits per sample
                      int spp,           ///< samples per pixel 
                      DRect bounds,      ///< bounding box for image
                      USGSImageLib::RGBPalette * pal = NULL );
        
        virtual ~ProjImageOut(); 

        static ProjImageOut createFromSocket( ClientSocket & socket );
        
        virtual void exportToSocket( ClientSocket & socket)const;
        
        virtual void putScanline( scanline_t scanline,
                                  const unsigned int& lineNo );
        
        virtual void putScanlines( scanlines_t scanlines,
                                   const unsigned int& height ); 
    private:
        
        void setupImage( std::string filename,
                         std::pair<long int, long int> heightThenWidth,
                         const ProjImageScale & newscale,
                         int photometric,
                         int bps,
                         int spp );
        
        const ProjImageParams & m_params; 
        ProjIOLib::ProjectionWriter& m_writer;
        USGSImageLib::RGBPalette * m_pal;
};

} // namespace 

#endif // __USGSMOSIX_PROJIMAGEOUT_H_
