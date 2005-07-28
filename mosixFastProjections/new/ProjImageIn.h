#ifndef _USGSMOSIX_PROJIMAGEIN_H_
#define _USGSMOSIX_PROJIMAEGIN_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/07/28 00:30:09 $
 *
 * \version 0.1
 * 
 * \file ProjImageIn.h 
 * 
 * \brief The ProjImage object is meant to be a representation 
 * of a image and an associated projection. 
 * 
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

/*! The ProjImageIn object contains information useful in 
 *  reprojecting a specific image (i.e., its scale, samples
 *  per pixel, bits per sample, photometric, filename, etc.)
 */
class ProjImageIn : private virtual ProjImageData,
                    public virtual ProjImageInInterface
{
    public:
        
        ProjImageIn( const ProjImageParams & params,  
                     ProjIOLib::ProjectionReader& projReader );
        
        virtual ~ProjImageIn() {}
        
        virtual DRect getNewBounds(const PmeshLib::ProjectionMesh & mesh) const;
        
        virtual DRect getGeographicBounds()const;
        
        const_scanline_t getCachedLine( unsigned int line ) const; 
       
        virtual const PixelInterface<sample_t> * 
        getPixel( const unsigned int& x, const unsigned int& y ) const;

        virtual const PixelInterface<sample_t> *
        getPixel( const double& latitude, const double& longitude ) const;
        
    private:
        
        double getMax( const std::list<double>& l )const;
        double getMin( const std::list<double>& l )const;
        ProjImageScale calculateScale(const DRect& bounds);
        bool openImageWithParamFile(const std::string& filename); 
        
        ProjImageParams m_parameters;
        ProjIOLib::ProjectionReader& m_projReader;
        USGSImageLib::CacheManager* m_cache;
        static const ProjLib::GeographicProjection m_geoProjection; 

        int m_spp;
        mutable std::pair<long int, const unsigned char *> m_lastLine;
        mutable const PmeshLib::ProjectionMesh * m_geoMesh;
        mutable DRect m_geoBounds; 
        mutable bool m_haveGeoBounds;
};

} // namespace 


#endif // _USGSMOSIX_PROJIMAGEIN_H_
