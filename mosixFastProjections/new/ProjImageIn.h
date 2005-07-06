#ifndef _USGSMOSIX_PROJIMAGEIN_H_
#define _USGSMOSIX_PROJIMAEGIN_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/07/06 23:16:45 $
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

#include "Globals.h"
#include "ProjImageData.h"
#include "ProjectorInterface.h"
#include "ProjImageParams.h"
#include <ImageLib/CacheManager.h>

namespace USGSMosix 
{

/*! The ProjImageIn object contains information useful in 
 *  reprojecting a specific image (i.e., its scale, samples
 *  per pixel, bits per sample, photometric, filename, etc.)
 */
class ProjImageIn : private ProjImageData,
                    public virtual ProjImageInInterface
                    
{
    public:
  
        ProjImageIn( const ProjImageParams & params,  
                     ProjIOLib::ProjectionReader& projReader );
        
        virtual ~ProjImageIn() {}
        virtual DRect getNewBounds(const PmeshLib::ProjectionMesh & mesh)const; 
        const_scanline_t getCachedLine( unsigned int line ) const; 
        virtual const unsigned char *  
        getPixel( const unsigned int& x, const unsigned int& y ) const;

    private:
        double getMax( const std::list<double>& l )const;
        double getMin( const std::list<double>& l )const;
        ProjImageScale calculateScale(const DRect& bounds);
        bool openImageWithParamFile(const std::string& filename); 
        
        ProjImageParams m_parameters;
        ProjIOLib::ProjectionReader& m_projReader;
        USGSImageLib::CacheManager* m_cache;
        
};

} // namespace 


#endif // _USGSMOSIX_PROJIMAGEIN_H_
