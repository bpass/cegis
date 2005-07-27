/*!
 * 
 * \author Mark Schisler 
 *
 * \file ProjImageFactory.cpp
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

#include <utility>
#include "ProjImageIn.h"
#include "ProjImageFactory.h"

namespace USGSMosix 
{

ProjImageFactory::~ProjImageFactory()
{
    for( std::list<ProjImageInInterface *>::iterator i = m_images.begin(); 
         i != m_images.end();
         ++i )
    {
        if ( *i != NULL ) delete *i;
    }
}

    
ProjImageInInterface*  
ProjImageFactory::makeProjImageIn( const ProjImageParams & param )
{
    ProjImageInInterface * temp = new ProjImageIn(param, m_reader); 
    m_images.push_front(temp);
    return temp;
}

ProjImageInInterface* 
ProjImageFactory::makeProjImageInList( std::list<ProjImageParams*> & inParams,
                                       ProjImageParams & outParams )
{
    ProjImageInInterface *temp(NULL);
    std::pair<long int, long int> hwPair = outParams.getHeightWidthPair();
    ProjImageInList *list = new ProjImageInList(hwPair.first, hwPair.second); 
    // TODO : m_images.push_back(list);
    
    std::cout << inParams.size() << std::endl;
    
    for( std::list<ProjImageParams*>::iterator i = inParams.begin();
         i != inParams.end();
         ++i  )
    {
        std::cout << "making new image" << std::endl;
        temp = new ProjImageIn( *(*i), m_reader);
        m_images.push_front(temp);
        assert (list->appendHead(temp)); 
    }
    
        std::cout << "done making new image" << std::endl;
        std::cout << "size: " << m_images.size() << std::endl;
    return list;
}

} // namespace 
