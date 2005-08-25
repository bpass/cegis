#ifndef __USGSMOSIX_PROJECTORINTERFACE_H_
#define __USGSMOSIX_PROJECTORINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/25 21:07:29 $
 *
 * \version 0.1
 * 
 * \file ProjectorInterface.h 
 * 
 * \brief This Interface is designed to lessen the tight-coupledness
 * of the frequent inheritence from the Projector class encountered
 * in the MOSIX Fast Projections project.  
 *
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include<ProjectorException.h>
#include "Globals.h"
#include "SerializableInterface.h"
#include "ProjImageInInterface.h"
#include "ProjImageOutInterface.h"

namespace USGSMosix {

class ProjectorInterface : public SerializableInterface {
    public:

        ProjectorInterface() {}
        
        virtual ~ProjectorInterface() {} 
        
        /// \brief Returns the projection we are projecting to.
        virtual const ProjLib::Projection * getOutputProjection()const = 0;
        
        /// \brief Sets up the ProjImageOut and the interpolating mesh used  
        /// for the image re-projection.
        virtual bool setupOutput() = 0;
        
        /// \brief The line which we should begin reprojecting at with the 
        /// current ProjImageIn and Out.  (zero based.)
        ///
        /// \brief The line which we should end reprojecting at with the
        /// current ProjImageIn and Out.  (zero based.)
        ///
        /// \note Unless this object was constructed with a output reference, 
        /// to use this function you must have called setupOutput.
        ///
        /// \brief Performs the image reprojection.
        virtual scanlines_t project( long unsigned int beginLine, 
                                     long unsigned int endLine ) = 0;

        /// \note Unless this object was constructed with a output reference, 
        /// to use this function you must have called setupOutput.
        ///
        /// \brief Performs the image reprojection.
        virtual void project() = 0;  
        
        /// \brief Returns a const pointer to the input ProjImage.
        virtual const ProjImageInInterface * getProjImageIn()const = 0;
        
        /// \brief Returns a const pointer to the output ProjImage.
        virtual const ProjImageOutInterface * getProjImageOut()const = 0;
};

} // namespace

#endif // _USGSMOSIX_PROJECTORINTERFACE_H_

