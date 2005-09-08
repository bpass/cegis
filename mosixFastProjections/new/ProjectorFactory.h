#ifndef __USGSMOSIX_PROJECTORFACTORY_H_
#define __USGSMOSIX_PROJECTORFACTORY_H_

/*!
 * 
 * \author Mark Schisler 
 *
 * \file ProjectorFactory.h
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 *
 * \brief Header file for ProjectorFactory class. 
 *
 */

#include "ProjectorInterface.h"
#include "ClientSocket.h"

namespace USGSMosix {

    /// A factory which makes projectors, designed to encapulate the 
    /// details of creating a ProjImage re-projector. 
    class ProjectorFactory {

    public:
        ProjectorFactory() {} 
        virtual ~ProjectorFactory();

        /// \param client A socket containing information regarding
        /// the construction of an unknown type of projector.
        ///
        /// \brief Makes a projector based on the type of information
        /// waiting in the socket.
        ProjectorInterface * makeProjector( ClientSocket & client );
        
    private:
        /// for memory management
        std::list<ProjectorInterface *> m_projectors;
            
    };

} // namespace 

#endif // __USGSMOSIX_PROJECTORFACTORY_H_

