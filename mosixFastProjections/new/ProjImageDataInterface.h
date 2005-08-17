
#ifndef _USGSMOSIX_PROJIMAGEDATAINTERFACE_H_
#define _USGSMOSIX_PROJIMAGEDATAINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 01:09:01 $
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
        
        // virtual get methods
        virtual double getLeftBound()                        const = 0;
        virtual double getRightBound()                       const = 0;
        virtual double getTopBound()                         const = 0;
        virtual double getBottomBound()                      const = 0;
        virtual DRect  getOuterBounds()                      const = 0;
        virtual long int getHeight()                         const = 0;
        virtual long int getWidth()                          const = 0; 
        virtual const ProjLib::Projection * getProjection()  const = 0;
        virtual ProjImageScale getPixelScale()               const = 0;
        virtual int getPhotometric()                         const = 0;
        virtual int getBPS()                                 const = 0;
        virtual int getSPP()                                 const = 0;
        
        // virtual set methods
        virtual void setPixelScale( const ProjImageScale & s )  = 0;

        // other methods
        virtual const PmeshLib::ProjectionMesh & setupMesh(
                const ProjLib::Projection & secondProjection,
                unsigned int divisions = kgMeshDivisions , 
                MathLib::InterpolatorType interp = kgInterpolator )const = 0;

        virtual const PmeshLib::ProjectionMesh & setupReverseMesh(
                const ProjLib::Projection & secondProjection,
                const DRect & boundaries,
                unsigned int divisions = kgMeshDivisions , 
                MathLib::InterpolatorType interp = kgInterpolator )const = 0;
 };

} // namespace 

#endif // _USGSMOSIX_PROJIMAGEDATAINTERFACE_H_
