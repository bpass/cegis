#ifndef __USGSMOSIX_PROJIAMGEOUT_H_
#define __USGSMOSIX_PROJIMAGEOUT_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/25 21:07:29 $
 *
 * \version 0.1
 * 
 * \file ProjOutImageOut.h 
 * 
 * \brief The ProjImage object is meant to be a representation 
 * of a image for an image reprojection and all of its 
 * implicit characteristics.  ProjImageOut accepts misc. parameters
 * and opens a blank output image with an associated projection.
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

        /// \param socket The socket containing the exported information.
        /// \brief A function to create a duplicate object of another object
        /// which was exported to a socket through a call to exportToSocket().
        static ProjImageOut createFromSocket( ClientSocket & socket );
        
        /// \param socket The socket to which the pretinent member data should 
        /// be exported for the purposes of creating a duplicate object on 
        /// the receiving end.
        /// \brief This function is used for serializing the current object
        /// via socket communication.
        virtual void exportToSocket( ClientSocket & socket)const;
        
        /// \param scanline A pointer to a complete scanline's data.
        /// \param lineNo The zero-based line number cooresponding to the 
        /// scanline data that has been forwarded to the function.
        /// \brief Puts scanline data into USGSImageLib::ImageOFile.  
        /// \note Usually lines must be placed sequentially. Some types of TIFFs
        /// support non-sequential writing, but trying to get that working
        /// has been a source of much pain in this project.
        virtual void putScanline( scanline_t scanline,
                                  const unsigned int& lineNo );
        
        /// \param scanlines A pointer to a group of complete scanlines. 
        /// \param beginLine The zero-based line number cooresponding to the 
        /// first scanline which was forwarded to the Function.
        /// \param endLine The zero-based line number cooresponding to the 
        /// last scanline which was forwarded to the funciton.
        /// \brief Puts group of scanlines into USGSImageLib::ImageOFile for
        /// writing to output file.  
        /// \note See putScanline() note.
        virtual void putScanlines( scanlines_t scanlines,
                                   const unsigned int& beginLine,
                                   const unsigned int& endLine ); 
        /// \param scanlines A 'piece' of the output image (i.e., a contigous
        /// grouping of scanlines.)
        /// \brief Puts piece of output iamge into USGSImageLib::ImageOFile for
        /// writing to output file.  
        /// \note See putScanline() note.
        virtual void putScanlines( const ProjImageOutPiece & piece );

    private:

        /// \brief Sets up the output image, ImageOFile, for the file and 
        /// sets member data appropriately.
        void setupImage( std::string filename,
                         std::pair<long int, long int> heightThenWidth,
                         const ProjImageScale & newscale,
                         int photometric,
                         int bps,
                         int spp );
       
        /// The parameter file object for the output image.
        const ProjImageParams & m_params; 

        /// Used to open some TIFF files.
        ProjIOLib::ProjectionWriter& m_writer;

        /// The palette for ImageLib files, if used.  If not, NULL.
        USGSImageLib::RGBPalette * m_pal;
};

} // namespace 

#endif // __USGSMOSIX_PROJIMAGEOUT_H_
