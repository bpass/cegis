
#ifndef _USGSMOSIX_FROMMULTIGEOPROJECTOR_H_
#define _USGSMOSIX_FROMMULTIGEOPROJECTOR_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 01:09:01 $
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
#include "ProjImageInInterface.h"
#include "ProjImageOutInterface.h"
#include "ProjImageFactory.h"

namespace USGSMosix 
{

/*! The FromMultiGeoProjector takes input from a list of images 
 *  (ImageList) and crafts this list of images and their cooresponding 
 *  bounding boxes into a single resulting image in a specified projection 
 */
class FromMultiGeoProjector : public ProjectorInterface 
{
   public:
       
       FromMultiGeoProjector( ProjImageParams & params,
                              ProjImageInInterface & source ); 
      
       FromMultiGeoProjector( ProjImageInInterface & source,
                              ProjImageOutInterface & destination );
       
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

       virtual const ProjImageInInterface * getProjImageIn()const;
       virtual const ProjImageOutInterface * getProjImageOut()const;
       
       // Projection Functions 
       virtual bool setupOutput();
       
       /// \pre To use this function you must have called getNewExtents()
       /// first.
       virtual scanlines_t project( long unsigned int beginLine, 
                                    long unsigned int endLine ); 
       virtual void project();    
      
       static FromMultiGeoProjector createFromSocket( ClientSocket & socket);
       virtual void exportToSocket( ClientSocket & socket )const;
       
    private:
       PmeshLib::ProjectionMesh * setupPmesh( PmeshLib::ProjectionMesh *& mesh,
                                              DRect outerbounds,
                                              bool forward  );
       
       void getImageBounds( const PmeshLib::ProjectionMesh & translatingMesh );

       ProjImageInInterface & m_imgIn;
       ProjImageOutInterface * m_imgOut;
       static ProjIOLib::ProjectionReader m_projReader;
       static ProjIOLib::ProjectionWriter m_projWriter;
       static ProjImageFactory m_imgFactory;
       ProjImageParams * m_params;
       
       unsigned int m_pmeshDivisions; ///< Used for the number of divisions 
                                      ///<vertically  and horizontally made 
                                      ///< for the image when 
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

inline const ProjImageInInterface * 
FromMultiGeoProjector::getProjImageIn()const
{
    return &m_imgIn;
}

/******************************************************************************/

inline const ProjImageOutInterface * 
FromMultiGeoProjector::getProjImageOut()const
{
    return m_imgOut;
}

/******************************************************************************/

} // namespace

#endif // _USGSMOSIX_FROMMULTIGEOPROJECTOR_H_

