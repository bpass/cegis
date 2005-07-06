#ifndef _USGSMOSIX_PROJIMAGESCALE_H_
#define _USGSMOSIX_PROJIMAGESCALE_H_

/*! 
 * \author Mark Schisler 
 *
 * \date   $Date $
 *
 * \version 0.1
 * 
 * \file ImageScale.h
 *
 * \brief The ImageScale object is meant to encapsulate the comparison 
 * of different types of image scales. 
 *
 * \note This library is free software and is distributed under the 
 * MIT open source license.  For more information, consult the file 
 * COPYING.
 * 
 */

#include "Globals.h"
#include "Math.h"
#include <ProjectionLib/ProjectionTypes.h>
#include <utility>

namespace USGSMosix 
{
    using ProjLib::PROJSYS;
    using ProjLib::UNKNOWN_PROJSYS;
    
    class ProjImageScale
    {
        public:
            ProjImageScale()
                : x(0.0f), y(0.0f),
                  m_imgType(OTHER), m_projSys(UNKNOWN_PROJSYS) {} 
           
            ProjImageScale(std::pair<scale_t, scale_t> xy)
                : x(xy.first), y(xy.second) {} 
            
            ProjImageScale( IMAGETYPE im, 
                            PROJSYS prosys, 
                            scale_t _x, 
                            scale_t _y )
                : x(_x), y(_y), m_imgType(im), m_projSys(prosys) {}  
            
            scale_t x;
            scale_t y;
            
            bool operator==( const ProjImageScale& RHS )
            {
                // if parameters are bogus
                if ( m_imgType == OTHER || m_projSys == UNKNOWN_PROJSYS ) 
                    return false;
                
                // make sure the projection systems are equal
                if ( this->m_projSys != RHS.m_projSys )  
                    return false;
              
                // make sure that the scales are within a certain 
                // amount of eachother.  (Because of machine error, 
                // we can't compare on binary equality.)
                if ( Math<scale_t>::abs(this->x - RHS.x) >= kgEqTol ) 
                    return false;
                
                // make sure that the scales that have been 
                // taken are compatible with one another.  TODO: 
                // come up with a conversion between the scales.
                if ( this->m_imgType == DOQ && RHS.m_imgType != DOQ ) 
                    return false;
               
                if ( this->m_imgType == GEOTIFF && RHS.m_imgType != GEOTIFF )
                    return false;

                if ( ( this->m_imgType == PNG || this->m_imgType == JPEG ||
                       this->m_imgType == TIFF ) && 
                     ( RHS.m_imgType != PNG && RHS.m_imgType != JPEG &&
                       RHS.m_imgType != TIFF ) ) 
                    return false;
                
                if ( this->m_imgType == OTHER || RHS.m_imgType == OTHER ) 
                    return false;

                return true;

            }
           
            void setImgType( IMAGETYPE it ) { m_imgType = it; } 
            void setProjSys( PROJSYS ps ) { m_projSys = ps; } 
            
        private:
            IMAGETYPE m_imgType;
            PROJSYS m_projSys;      
    };

} // namespace 

#endif // _USGSMOSIX_PROJIMAGESCALE_H_
