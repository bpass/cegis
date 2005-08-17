
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

#include "FromMultiGeoProjector.h"
#include "SlaveProjector.h"
#include "ProjectorFactory.h"
#include "GeneralException.h"

namespace USGSMosix {

/******************************************************************************/

ProjectorFactory::~ProjectorFactory()
{
    for( std::list<ProjectorInterface *>::iterator i; 
         i != m_projectors.end();
         ++i )
        delete *i;
}
    
/******************************************************************************/

ProjectorInterface * ProjectorFactory::makeProjector( ClientSocket & client ) 
{
    PROJECTORTYPE ty;
    ProjectorInterface * iface = NULL;
    
    client.peek(&ty, sizeof(ty));
    switch( ty )
    {
        case GEOPROJ:
            iface = new(std::nothrow) FromMultiGeoProjector(
                    FromMultiGeoProjector::createFromSocket(client));
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
