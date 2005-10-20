#ifndef _USGSMOSIX_GENERALPROJECTOR_H_
#define _USGSMOSIX_GENERALPROJECTOR_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/10/20 23:21:15 $
 *
 * \version 0.1
 * 
 * \file GeneralProjector.h
 * 
 * \brief Header file for GeneralProjector class. 
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

/// The GeneralProjector takes input from an image interface 
/// and crafts this list of images and their cooresponding 
/// bounding boxes into a single resulting image in a specified projection 
class GeneralProjector : public ProjectorInterface 
{
   public:
       /// \param params The parameters for the output projection.
       /// \param source The ProjImageIn from which we are projecting.
       GeneralProjector( ProjImageParams & params,
                         ProjImageInInterface & source ); 
      
       /// \param source The ProjImageIn from which we are projecting.
       /// \param destination The ProjImageOut we are projecting to.
       GeneralProjector( ProjImageInInterface & source,
                         ProjImageOutInterface & destination );
       
       virtual ~GeneralProjector(); 
       
       /// \brief Sets which interpolation will be used in the 
       /// reprojection algorithm.
       void setPmeshInterpolator( const MathLib::InterpolatorType& t );

       /// \brief sets up the number of divisions both length wise and
       /// width wise that there will be in the mesh.  For instance, if
       /// inpmeshsize is 4, then there will be 16 distinct areas to
       /// the mesh.
       void setPmeshDivisions( const int & inpmeshsize );

       /// \brief Changes the input sources to be from a new interface.
       void setInputSources( ProjImageInInterface & pi );  
                  
       /// \brief Returns the type of interpolation used.
       MathLib::InterpolatorType getPmeshInterpolator() const;

       /// \brief Returns the number of divisions both length-wise and
       /// width-wise that there will be in the mesh.
       int getPmeshDivisions() const;

       /// \brief Returns the outerbounds in degrees of the output image.
       DRect getoutRect() const;

       /// \brief Returns the projection we are projecting to.
       virtual const ProjLib::Projection* getOutputProjection() const; 

       /// \brief Returns a const pointer to the input ProjImage.
       virtual const ProjImageInInterface * getProjImageIn()const;

       /// \brief Returns a const pointer to the output ProjImage.
       virtual ProjImageOutInterface * getProjImageOut();
       
       /// \brief Sets up the ProjImageOut and the interpolating mesh used  
       /// for the image re-projection.
       virtual bool setupOutput();
       
       /// \param beginLine Line which we should begin reprojecting at with the 
       /// current ProjImageIn and Out --zero based.
       ///
       /// \param endLine The line which we should end reprojecting at with the
       /// current ProjImageIn and Out --zero based.
       ///
       /// \note Unless this object was constructed with a output reference, 
       /// to use this function you must have called setupOutput.
       ///
       /// \brief Performs the image reprojection.
       virtual ProjImageOutPiece projectPiece( long unsigned int beginLine,
                                               long unsigned int endLine );

       /// \brief Performs the image reprojection and returns what was 
       /// reprojected as a image piece.
       virtual ProjImageOutPiece projectPiece();
       
       /// \note Unless this object was constructed with a output reference, 
       /// to use this function you must have called setupOutput.
       ///
       /// \brief Performs the image reprojection.
       virtual void project();    
       
       /// \param socket A socket which has the data which is neccessary to
       /// construct a GeneralProjector object waiting on it.
       ///
       /// \brief Creates a FromMultiProjector object from the data 
       /// waiting on the socket.  If this fails for some reason, an
       /// exception is thrown.
       static GeneralProjector createFromSocket( ClientSocket & socket);

       /// \param socket The socket which we would like to export the data
       /// for the current projector to.
       ///
       /// \brief Puts data out onto the socket which can then be used by a
       /// call to createFromSocket to re-construct the same object on the
       /// other end.
       virtual void exportToSocket( ClientSocket & socket )const;
       
    private:
       /// \param beginLine Line which we should begin reprojecting at with the 
       /// current ProjImageIn and Out.  (zero based.)
       ///
       /// \param endLine The line which we should end reprojecting at with the
       /// current ProjImageIn and Out.  (zero based.)
       ///
       /// \note Unless this object was constructed with a output reference, 
       /// to use this function you must have called setupOutput.
       ///
       /// \brief Performs the image reprojection.
       scanlines_t project( long unsigned int beginLine, 
                            long unsigned int endLine ); 
       
       /// \param mesh A reference to a pointer to a mesh which will point
       /// to a constructed mesh upon a successful completion of the funciton.
       ///
       /// \param outerbounds The outerbounds in degrees, w.r.t. the output
       /// projection, over which the mesh which will be constructed.
       ///
       /// \param forward True if constructing a forward mesh.  False if 
       /// constructing a reverse mesh.
       ///
       /// \brief Constructs a mesh out of the parameters forwarded to the 
       /// function using the m_fromProjection -> m_toProjection mapping.
       PmeshLib::ProjectionMesh * setupPmesh( PmeshLib::ProjectionMesh *& mesh,
                                              DRect outerbounds,
                                              bool forward  );
      
       /// \param mesh The mesh used to construct the output image.
       ///
       /// \brief Constructs an output image and calculates the outermost 
       /// bounds based on the mesh forwarded to the function.  The output
       /// image is saved in m_imgOut.
       void constructOutImage( const PmeshLib::ProjectionMesh & mesh );

       ProjImageInInterface & m_imgIn;  
       ProjImageOutInterface * m_imgOut; 
       static ProjIOLib::ProjectionReader m_projReader; 
       static ProjIOLib::ProjectionWriter m_projWriter;
       static ProjImageFactory m_imgFactory; ///< Creates ProjImages.
       ProjImageParams * m_params;    ///< The input parameters.
       
       unsigned int m_pmeshDivisions; ///< Used for the number of divisions 
                                      ///< vertically  and horizontally made 
                                      ///< for the image when 
                                      ///< calculating a mesh
                             
       MathLib::InterpolatorType m_pmeshInterp; ///< An enum type which can be 
                                                ///< found in 
                                                ///< InterpolatorTypes.h 
                                                ///< in the Mathlib.  It 
                                                ///< specifies the type of 
                                                ///< interpolation that is 
                                                ///< used with the mesh. 
                                                
       /// used for boundary reprojection.
       const PmeshLib::ProjectionMesh * m_forwardMesh;  

       /// used for reprojecting the body of the image.
       const PmeshLib::ProjectionMesh * m_reverseMesh;
       
       /// what projection we're going from.
       const ProjLib::Projection * m_fromProj;

       /// what projection we're going to.
       const ProjLib::Projection * m_toProj;

};

/******************************************************************************/

inline const ProjImageInInterface * 
GeneralProjector::getProjImageIn()const
{
    return &m_imgIn;
}

/******************************************************************************/

inline ProjImageOutInterface * 
GeneralProjector::getProjImageOut()
{
    return m_imgOut;
}

/******************************************************************************/

} // namespace

#endif // _USGSMOSIX_FROMMULTIGEOPROJECTOR_H_

