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
    ProjImageInList *list = new ProjImageInList; 
    m_images.push_back(temp);
    
    for( std::list<ProjImageParams*>::iterator i = params.begin();
         i != params.end();
         ++i  )
    {
        temp = new ProjImageIn( *(*i), m_reader);
        m_images.push_front(temp);
        list->appendHead(temp); 
    }
    
    return list;
}

} // namespace 
