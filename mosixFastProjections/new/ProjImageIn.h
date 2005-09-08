#ifndef _USGSMOSIX_PROJIMAGEIN_H_
#define _USGSMOSIX_PROJIMAEGIN_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file ProjImageIn.h 
 * 
 * \brief Header file for ProjImageIn class. 
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */
#include <ImageLib/CacheManager.h>
#include <ProjectionLib/GeographicProjection.h>
#include "Globals.h"
#include "ProjImageData.h"
#include "ProjectorInterface.h"
#include "ProjImageParams.h"
#include "PixelRGB.h"
#include "PixelGrey.h"


namespace USGSMosix 
{

/// The ProjImageIn object contains information useful in 
/// reprojecting a specific image  -- its scale, samples
/// per pixel, bits per sample, photometric, filename, etc.
class ProjImageIn : private virtual ProjImageData,
                    public virtual ProjImageInInterface
{
    public:
        
        ProjImageIn( const ProjImageParams & params );

        ProjImageIn( const ProjImageIn & copyOf );
        
        virtual ~ProjImageIn(); 
       
        /// \brief Returns the new bounds for the current image based off 
        /// of a forward projecting mesh which is forwarded to the function.
        /// \brief  
        virtual DRect getNewBounds(const PmeshLib::ProjectionMesh & mesh) const;
        
        /// \brief Returns a rectangle representing the latitude and longitude
        /// bounds for the image.
        virtual DRect getGeographicBounds()const;
       
        /// \param line The line requested.
        /// \brief Returns a specified scanline from a LRU cache. 
        const_scanline_t getCachedLine( unsigned int line ) const; 
        
        /// \param x The x pixel coordinate of the pixel being fetched. 
        /// \param y The y pixel coordinate of the pixel being fetched.
        ///
        /// \brief Returns a pixel based off of the pixel coordinates in 
        /// the image. 
        const PixelInterface<sample_t> * 
        getPixel( const unsigned int& x, const unsigned int& y ) const;

        /// \param latitude The latitude of the pixel you are requesting.
        /// \param longitude The longitude of the pixel you are requesting.
        ///
        /// \brief Returns a pixel based off of latitude and longitude 
        /// coordinates in the image. 
        virtual const PixelInterface<sample_t> *
        getPixel( const double& latitude, const double& longitude ) const;
       
        /// \param socket The socket which has data waiting on it 
        /// describing the construction of the ProjImageIn.
        ///
        /// \brief Returns a newly constructed ProjImageIn based off of
        /// data waiting on the socket.
        static ProjImageIn createFromSocket( ClientSocket & socket );

        /// \param socket The socket to export the data describing the
        /// current object. 
        ///
        /// \brief Exports data to the socket so that a copy may be 
        /// constructed on the other end and have the same state.
        virtual void exportToSocket( ClientSocket & socket )const;
        
    private:
        /// \param bounds The bounding box which should be used to 
        /// calculate the width/height in degrees.
        /// \brief degrees to pixel scale based off forwarded bounding box.
        ProjImageScale calculateScale(const DRect& bounds);
        
        /// \param filename The image filename 
        /// \brief Opens an image file with filename.  Returns true
        /// on success.  Sets up m_file pointer to a valid ImageLib file.
        bool openImageWithParamFile(const std::string& filename); 
       
        /// image input parameters.
        const ProjImageParams & m_params;
        
        /// for opening TIFF files
        static ProjIOLib::ProjectionReader m_projReader;
        
        /// to manage scanlines. 
        USGSImageLib::CacheManager* m_cache;

        /// for fetching geographic boundaries.
        static const ProjLib::GeographicProjection m_geoProjection; 

        /// samples per pixel for image. (e.g., RGB (3), Greyscale (1), etc.)
        int m_spp;
       
        /// the last scanline fetched and its line number
        mutable std::pair<long int, const unsigned char *> m_lastLine;

        /// a geographic mesh for calculating geographic boundaries.
        mutable const PmeshLib::ProjectionMesh * m_geoMesh;

        /// geographic boundaries
        mutable DRect m_geoBounds; 

        /// have the geographic boundaries been generated yet?
        mutable bool m_haveGeoBounds;
};

} // namespace 


#endif // _USGSMOSIX_PROJIMAGEIN_H_
