
#ifndef _USGSMOSIX_FROMMULTIGEOPROJECTOR_H_
#define _USGSMOSIX_FROMMULTIGEOPROJECTOR_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file FromMultiProjector.h 
 * 
 * \brief The FromMultiGeoProjector class is an object which takes 
 * input from several images in a Geographic projection, and projects 
 * this input into a single image of a desired projection.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include <stdexcept>
#include <exception>
#include <ProjectorException.h>
#include <ProjectionIO/ProjectionReader.h>
#include <ProjectionIO/ProjectionWriter.h>
#include <MathLib/InterpolatorTypes.h>
#include "ProjImageParams.h"
#include "ProjectorInterface.h"
#include "ProjImageOut.h"
#include "ProjImageInList.h"

namespace USGSMosix 
{

/*! The FromMultiGeoProjector takes input from a list of images 
 *  (ImageList) and crafts this list of images and their cooresponding 
 *  bounding boxes into a single resulting image in a specified projection 
 */
class FromMultiGeoProjector : public ProjectorInterface 
{
   public:
       
       FromMultiGeoProjector( ProjIOLib::ProjectionWriter & projWriter,
                              ProjIOLib::ProjectionReader & projReader,
                              ProjImageParams & params,
                              ProjImageInInterface & source
                            ); 
       
       virtual ~FromMultiGeoProjector(); 
       
       // set functions
       void setPmeshInterpolator( const MathLib::InterpolatorType& t );
       void setPmeshDivisions( const int & inpmeshsize );
       void setInputSources( ProjImageInInterface & pi );  
                  
       // get functions
       MathLib::InterpolatorType getPmeshInterpolator() const;
       int getPmeshDivisions() const;
       DRect getoutRect() const;
       virtual const ProjLib::Projection* getOutputProjection() const; 

       // Projection Functions 
       virtual void getNewExtents();
       
       /// \pre To use this function you must have called getNewExtents()
       /// first.
       virtual scanlines_t project( long unsigned int beginLine, 
                                    long unsigned int endLine ); 
       virtual void project();    
       
    private:
       PmeshLib::ProjectionMesh * setupPmesh( PmeshLib::ProjectionMesh *& mesh,
                                              DRect outerbounds,
                                              bool forward  );
       
       void getImageBounds( const PmeshLib::ProjectionMesh & translatingMesh );

       /// \pre  m_fromProj is not NULL. m_toProj is not NULL.
       /// \post x and y are projected to the projection specified
       /// by the m_from and m_to pointers.
       void projectPoint(double& x, double& y);
       void reverseProjectPoint(double& x, double& y);

       ProjImageInInterface & m_imgIn;
       ProjImageOutInterface * m_imgOut;
       ProjIOLib::ProjectionWriter & m_projWriter;
       ProjIOLib::ProjectionReader & m_projReader;
       ProjImageParams & m_params;
       
       int m_pmeshDivisions; ///< Used for the number of divisions vertically 
                             ///< and horizontally made for the image when 
                             ///< calculating a mesh
                             
       MathLib::InterpolatorType m_pmeshInterp; ///< An enum type which can be 
                                                ///< found in 
                                                ///< InterpolatorTypes.h 
                                                ///< in the Mathlib.  It 
                                                ///< specifies the type of 
                                                ///< interpolation that is 
                                                ///< used with the mesh. 
                                                
       const PmeshLib::ProjectionMesh * m_forwardMesh; 
       const PmeshLib::ProjectionMesh * m_reverseMesh;
       
       const ProjLib::Projection * m_fromProj;
       const ProjLib::Projection * m_toProj;

};

/******************************************************************************/

inline void FromMultiGeoProjector::projectPoint(double& x, double& y)
{
    if( m_forwardMesh != NULL )
        m_forwardMesh->projectPoint(x, y);
    else
    {
        // y and x become actual latitude and longitude 
        // after executing this line
        m_fromProj->projectToGeo(x,y,y,x);
        
        // after execution of this line of code:
        // y and x are now to "true scale" with regard 
        // to the original map, however they are now 
        // reprojected to a different location on the 
        // old map's grid.  Note that these new x and y 
        // are not latitude and longitude or pixel 
        // values.  They are to the arbitary scale of the
        // source map.
        m_toProj->projectFromGeo(y,x,x,y);
    }
}

/******************************************************************************/

inline void FromMultiGeoProjector::reverseProjectPoint(double& x, double& y)
{
    if( m_reverseMesh != NULL )
        m_reverseMesh->projectPoint(x, y);
    else
    {
        // y and x become actual latitude and longitude 
        // after executing this line
        m_toProj->projectToGeo(x,y,y,x);

        // after execution of this line of code:
        // y and x are now to "true scale" with regard 
        // to the original map, however they are now 
        // reprojected to a different location on the 
        // old map's grid.  Note that these new x and y 
        // are not latitude and longitude or pixel 
        // values.  They are to the arbitary scale of the
        // source map.
        m_fromProj->projectFromGeo(y,x,x,y);
    }
}

} // namespace

#endif // _USGSMOSIX_FROMMULTIGEOPROJECTOR_H_

