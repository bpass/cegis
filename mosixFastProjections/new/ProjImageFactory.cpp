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
ProjImageFactory::makeProjImageInList( std::list<ProjImageParams*> & params )
{
    ProjImageInInterface *temp(NULL);
    // ProjImageInList *list = new ProjImageInList(7631, 6211); 
    ProjImageInList *list = new ProjImageInList(763, 621); 
    //ProjImageInList *list = new ProjImageInList(1200, 2400); 
    // TODO : m_images.push_back(list);
    
    std::cout << params.size() << std::endl;
    
    for( std::list<ProjImageParams*>::iterator i = params.begin();
         i != params.end();
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
