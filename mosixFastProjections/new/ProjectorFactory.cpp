
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
 * \brief Implementation file for ProjectorFactory class. 
 *
 */

#include "GeneralProjector.h"
#include "SlaveProjector.h"
#include "ProjectorFactory.h"
#include "GeneralException.h"

namespace USGSMosix {

/******************************************************************************/

ProjectorFactory::~ProjectorFactory()
{
    for( std::list<ProjectorInterface *>::iterator i = m_projectors.begin(); 
         i != m_projectors.end(); ++i )
        delete *i; 
}
    
/******************************************************************************/

ProjectorInterface * ProjectorFactory::makeProjector( ClientSocket & client ) 
{
    PROJECTORTYPE ty;
    ProjectorInterface * iface = NULL;
   
    // peek at which type of projector it is and use that to construct
    // the projector type.
    client.peek(&ty, sizeof(ty));
    switch( ty )
    {
        case GEOPROJ:
            iface = new(std::nothrow) GeneralProjector(
                    GeneralProjector::createFromSocket(client));
            break;
        case SLAVEPROJ:
            iface = new(std::nothrow) SlaveProjector(
                    SlaveProjector::createFromSocket(client));
            break;
        default:
            throw GeneralException("Unknown type of Projector.");
            break;
    }

    if ( !iface )
        throw GeneralException("Factory creation of Projector failed.");
    else
        m_projectors.push_back(iface);
    
    return iface;
    
}

/******************************************************************************/

} // namespace 
