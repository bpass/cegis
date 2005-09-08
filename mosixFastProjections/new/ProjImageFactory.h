#ifndef __USGSMOSIX_PROJIMAGEFACTORY_H_
#define __USGSMOSIX_PROJIMAGEFACTORY_H_

/*!
 * 
 * \author Mark Schisler 
 *
 * \file ProjImageFactory.h
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 *
 * \brief Header file for ProjImageFactory class.
 *
 */

#include <ProjectionIO/ProjectionWriter.h>
#include <ProjectionIO/ProjectionReader.h>
#include <list>
#include "ProjImageInInterface.h"
#include "ProjImageInList.h"
#include "ProjImageParams.h"
#include "ClientSocket.h"

namespace USGSMosix 
{

using ProjIOLib::ProjectionWriter;
using ProjIOLib::ProjectionReader;

/// Designed to encapsulate the details of creating a ProjImage. 
class ProjImageFactory
{
    public:
        ProjImageFactory(ProjectionReader& r, ProjectionWriter& w) 
            : m_writer(w), m_reader(r) {} 

        virtual ~ProjImageFactory();

        /// \param param The parameters needed for the ProjImageIn creation.
        /// \brief Creates a ProjImageIn based on the forwarded parameters.
        ProjImageInInterface* 
        makeProjImageIn( const ProjImageParams & param );
        
        /// \param params The parameters needed for the ProjImageInList 
        /// creation.
        /// \param outputParams The output parameters for the list.  
        /// Primarily what is needed here is what the virtual height 
        /// and width should be of the image list.
        ProjImageInInterface* 
        makeProjImageInList( std::list<ProjImageParams*>& params,
                             ProjImageParams & outputParams );
        
        /// \param socket The socket which contains construction data 
        /// for a ProjImageIn or ProjImageInList.
        /// \brief Constructs a ProjImageIn/List based on the parameters
        /// which are waiting on the socket.
        ProjImageInInterface * 
        makeProjImageIn( ClientSocket & socket );

        /// \param socket The socket which contains construction data
        /// for a ProjImageIn or ProjImageInList.
        /// \brief Constructs a ProjImageOut based on the parameters
        /// which are waiting on the socket.
        ProjImageOutInterface * 
        makeProjImageOut( ClientSocket & socket );
        
    private:
        /// to keep track of and clean up dynamic memory.
        std::list<ProjImageDataInterface *> m_images;
        
        ProjectionWriter & m_writer;
        ProjectionReader & m_reader;
};

} // namespace 


#endif // _USGSMOSIX_PROJIMAGEFACTORY_H_
