#ifndef __USGSMOSIX_PROJIMAGEININTERFACE_H_
#define __USGSMOSIX_PROJIMAGEININTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file ProjImageInInterface.h 
 * 
 * \brief Header file for the ProjImageInInterface class. 
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
    /// An interface for dealing with Projected Image input in a re-projection
    /// environment 
    class ProjImageInInterface : public virtual ProjImageDataInterface,
                                 public virtual SerializableInterface
    {
        public:
            
        ProjImageInInterface() {} 
        virtual ~ProjImageInInterface() {} 
       
        /// \param latitude The latitude of the pixel you are requesting.
        /// \param longitude The longitude of the pixel you are requesting.
        ///
        /// \brief Returns a pixel based off of latitude and longitude 
        /// coordinates in the image. 
        virtual const PixelInterface<sample_t> *
        getPixel( const double & latitude, const double & longitude )const = 0;
        
        /// \brief Returns the new bounds for the current image based off 
        /// of a forward projecting mesh which is forwarded to the function.
        virtual DRect 
        getNewBounds(const PmeshLib::ProjectionMesh & m)const = 0; 

        /// \brief Returns a rectangle representing the latitude and longitude
        /// bounds for the image.
        virtual DRect getGeographicBounds()const = 0;
   };
    
} //namespace

#endif // _USGSMOSIX_PROJIMAGEININTERFACE_H_

