#ifndef _USGSMOSIX_PROJIMAGEININTERFACE_H_
#define _USGSMOSIX_PROJIMAGEININTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file ProjImageInInterface.h 
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

#include <ProjectionMesh/ProjectionMesh.h>
#include <ProjectionLib/Projection.h>
#include <MathLib/InterpolatorTypes.h>
#include "DRect.h"
#include "ProjImageScale.h"

namespace USGSMosix 
{
    class ProjImageInInterface 
    {
        public:
            
        ProjImageInInterface() {} 
        virtual ~ProjImageInInterface() {} 
         
        // virtual get
        virtual double getLeftBound()                       const = 0;
        virtual double getRightBound()                      const = 0;
        virtual double getTopBound()                        const = 0;
        virtual double getBottomBound()                     const = 0;
        virtual DRect  getOuterBounds()                     const = 0;
        virtual long int getHeight()                        const = 0;
        virtual long int getWidth()                         const = 0;
        virtual const ProjLib::Projection * getProjection() const = 0;
        virtual ProjImageScale getPixelScale()              const = 0;
        virtual int getPhotometric()                        const = 0;
        virtual int getBPS()                                const = 0;
        virtual int getSPP()                                const = 0;
        virtual const unsigned char * 
        getPixel( const unsigned int& x, 
                  const unsigned int& y )                   const = 0;
        virtual DRect 
        getNewBounds(const PmeshLib::ProjectionMesh & m)   const  = 0; 
        virtual const PmeshLib::ProjectionMesh & 
        setupMesh( const ProjLib::Projection & secondProjection,
                   unsigned int divisions = kgMeshDivisions,  
                   MathLib::InterpolatorType interp = kgInterpolator ) = 0;
    
        // virtual set 
        virtual void setPixelScale( const ProjImageScale&  p)     = 0;
        
   };
    
} //namespace

#endif // _USGSMOSIX_PROJIMAGEININTERFACE_H_

