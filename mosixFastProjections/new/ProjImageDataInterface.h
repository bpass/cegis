
#ifndef __USGSMOSIX_PROJIMAGEDATAINTERFACE_H_
#define __USGSMOSIX_PROJIMAGEDATAINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/25 21:07:29 $
 *
 * \version 0.1
 * 
 * \file ProjImageDataInterface.h 
 * 
 * \brief A ProjImage object is meant to be a representation 
 * of a image for an image projection and all of its 
 * implicit characteristics.  The ProjImageDataInterface defines
 * the common set and get functions for the ProjImage set of 
 * objects.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "DRect.h"
#include "ProjImageScale.h"

namespace USGSMosix 
{

class ProjImageDataInterface
{
    public:
        ProjImageDataInterface() {}
        virtual ~ProjImageDataInterface() {}
        
        /* virtual get methods */

        /// \brief Returns the left bound in degrees of the current image.
        virtual double getLeftBound()                        const = 0;

        /// \brief Returns the right bound in degrees of the current image.
        virtual double getRightBound()                       const = 0;

        /// \brief Returns the top bound in degress of the current image.
        virtual double getTopBound()                         const = 0;

        /// \brief Returns the bottom bound in degrees of the current image.
        virtual double getBottomBound()                      const = 0;

        /// \brief Returns the outerbounds in degrees of the current image.
        virtual DRect  getOuterBounds()                      const = 0;

        /// \brief Returns the height in pixels of the current image.
        virtual long int getHeight()                         const = 0;

        /// \brief Returns the width in pixels of the current image. 
        virtual long int getWidth()                          const = 0; 

        /// \brief Returns the projection of the current image.
        virtual const ProjLib::Projection * getProjection()  const = 0;

        /// \brief Returns the units/pixel measurement of the image.
        virtual ProjImageScale getPixelScale()               const = 0;

        /// \brief Returns the image's photometric (e.g., RGB, greyscale)
        virtual int getPhotometric()                         const = 0;

        /// \brief Returns the image's bits per sample (or channel.)
        virtual int getBPS()                                 const = 0;

        /// \brief Returns the samples per pixel. 
        virtual int getSPP()                                 const = 0;
        
        /*  virtual set methods */

        /// \brief Sets the units/pixel scale for the current image.
        virtual void setPixelScale( const ProjImageScale & s )  = 0;

        /// \param toProjection The destination projection to be 
        /// projected to from this image's current projection.  In other
        /// words, the mesh will be mapping from the current projimage's
        /// projection to the destination.
        /// 
        /// \param divisions The number of vertical/horizontal divisions
        /// made in the mesh.  Defaults to a globally defined constant.
        ///
        /// \param interp.  The type of interpolation to be used in 
        /// mesh pixel getting.  Defaults to a globally defined constant.
        /// 
        /// \brief Sets up a forward mesh using this ProjImage's projection
        /// and a second Projection.  
        virtual const PmeshLib::ProjectionMesh & setupMesh(
                const ProjLib::Projection & toProjection,
                unsigned int divisions = kgMeshDivisions , 
                MathLib::InterpolatorType interp = kgInterpolator )const = 0;

        /// \param fromProjection The source projection.  In other words,
        /// the mesh will be a mapping from fromProjection to the 
        /// current projection.
        /// 
        /// \param divisions The number of vertical/horizontal divisions
        /// made in the mesh.  Defaults to a globally defined constant.
        ///
        /// \param interp.  The type of interpolation to be used in 
        /// mesh pixel getting.  Defaults to a globally defined constant.
        /// 
        /// \brief Sets up a reverse mesh using this ProjImage's projection
        /// and a second Projection.  
        virtual const PmeshLib::ProjectionMesh & setupReverseMesh(
                const ProjLib::Projection & fromProjection,
                const DRect & boundaries,
                unsigned int divisions = kgMeshDivisions , 
                MathLib::InterpolatorType interp = kgInterpolator )const = 0;
 };

} // namespace 

#endif // _USGSMOSIX_PROJIMAGEDATAINTERFACE_H_
