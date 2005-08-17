#ifndef __USGSMOSIX_PROJECTORFACTORY_H_
#define __USGSMOSIX_PROJECTORFACTORY_H_

/*!
 * 
 * \author Mark Schisler 
 *
 * \file ProjectorFactory.h
 *
 * \date $Date: 2005/08/17 01:09:01 $
 *
 * \version 0.1
 *
 * \brief This is the Projector.  It is designed to encapulate the 
 * details of creating a Projimage Reprojector, so that if the process 
 * changes later, life will still proceed smoothly.
 *
 */

#include "ProjectorInterface.h"
#include "ClientSocket.h"

namespace USGSMosix {

    class ProjectorFactory {

    public:
        ProjectorFactory() {} 
        virtual ~ProjectorFactory();
        
        ProjectorInterface * makeProjector( ClientSocket & client );
    private:
        std::list<ProjectorInterface *> m_projectors;
            
    };

} // namespace 

#endif // __USGSMOSIX_PROJECTORFACTORY_H_

