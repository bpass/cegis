#ifndef _USGSMOSIX_PROJIMAGEFACTORY_H_
#define _USGSMOSIX_PROJIMAGEFACTORY_H_

/*!
 * 
 * \author Mark Schisler 
 *
 * \file ProjImageFactory.h
 *
 * \date $date$
 *
 * \version 0.1
 *
 * \brief This is the ProjImageFactory.  It is designed to encapulate the 
 * details of creating a ProjImage, so that if the process changes later,  
 * life will still proceed smoothly.
 *
 */

#include <ProjectionIO/ProjectionWriter.h>
#include <ProjectionIO/ProjectionReader.h>
#include <list>
#include "ProjImageInInterface.h"
#include "ProjImageInList.h"
#include "ProjImageParams.h"

namespace USGSMosix 
{

using ProjIOLib::ProjectionWriter;
using ProjIOLib::ProjectionReader;

class ProjImageFactory
{
    public:
        ProjImageFactory(ProjectionReader& r, ProjectionWriter& w) 
            : m_writer(w), m_reader(r) {} 
        virtual ~ProjImageFactory();
        
        ProjImageInInterface* 
        makeProjImageIn( const ProjImageParams & param );
        
        ProjImageInInterface* 
        makeProjImageInList( std::list<ProjImageParams*>& params,
                             ProjImageParams & outputParams );
        
    private:
        std::list<ProjImageInInterface *> m_images;
        ProjectionWriter & m_writer;
        ProjectionReader & m_reader;
};

} // namespace 


#endif // _USGSMOSIX_PROJIMAGEFACTORY_H_
