#ifndef _USGSMOSIX_PROJIMAGEININTERFACE_H_
#define _USGSMOSIX_PROJIMAGEININTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 01:09:01 $
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

#include "Globals.h"
#include <ProjectionMesh/ProjectionMesh.h>
#include <ProjectionLib/Projection.h>
#include <MathLib/InterpolatorTypes.h>
#include "PixelInterface.h"
#include "DRect.h"
#include "ProjImageDataInterface.h" 
#include "ProjImageScale.h"

namespace USGSMosix 
{
    class ProjImageInInterface : public virtual ProjImageDataInterface,
                                 public virtual SerializableInterface
    {
        public:
            
        ProjImageInInterface() {} 
        virtual ~ProjImageInInterface() {} 
        
        // virtual get
        virtual const PixelInterface<sample_t> *
        getPixel( const unsigned int& x, const unsigned int& y )const = 0;
        
        virtual const PixelInterface<sample_t> *
        getPixel( const double & latitude, const double & longitude )const = 0;
        
        virtual DRect 
        getNewBounds(const PmeshLib::ProjectionMesh & m)const = 0; 

        virtual DRect getGeographicBounds()const = 0;
   };
    
} //namespace

#endif // _USGSMOSIX_PROJIMAGEININTERFACE_H_

