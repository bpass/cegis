/*!
 * 
 * \author Mark Schisler 
 *
 * \file ProjImageFactory.cpp
 *
 * \date $Date: 2005/10/13 22:27:40 $
 *
 * \version 0.1
 *
 * \brief Implementation file for ProjImageFactory class. 
 *
 */

#include "Globals.h"
#include <utility>
#include "ProjImageIn.h"
#include "ProjImageOut.h"
#include "ProjImageFactory.h"

namespace USGSMosix 
{

/******************************************************************************/

ProjImageFactory::~ProjImageFactory()
{
    for( std::list<ProjImageDataInterface *>::iterator i = m_images.begin(); 
         i != m_images.end();
         ++i )
    {
        if ( *i != NULL ) delete *i;
    }
}

/******************************************************************************/
    
ProjImageInInterface*  
ProjImageFactory::makeProjImageIn( const ProjImageParams & param )
{
    ProjImageInInterface * temp = new ProjImageIn( param ); 
    m_images.push_front(temp);
    return temp;
}

/******************************************************************************/

ProjImageInInterface* 
ProjImageFactory::makeProjImageInList( std::list<ProjImageParams*> & inParams,
                                       ProjImageParams & outParams )
{
    ProjImageInInterface *temp(NULL);
    std::pair<long int, long int> hwPair = outParams.getHeightWidthPair();
    ProjImageInList *list = new ProjImageInList(hwPair.first, hwPair.second); 
    m_images.push_back(list);
    
    WRITE_DEBUG ( inParams.size() << std::endl );
    
    for( std::list<ProjImageParams*>::iterator i = inParams.begin();
         i != inParams.end();
         ++i  )
    {
        WRITE_DEBUG (  "making new image" << std::endl );
        
        temp = new ProjImageIn( *(*i) );
        assert (list->appendHead(temp)); 
    }
    
    WRITE_DEBUG ( "done making new image" << std::endl );
    WRITE_DEBUG ( "size: " << m_images.size() << std::endl );
    
    return list;
}

/******************************************************************************/

ProjImageInInterface * 
ProjImageFactory::makeProjImageIn( ClientSocket & socket )
{
    int noInputs = 0; 
    unsigned long int width(0), height(0);
    std::list<ProjImageParams*> params;
    ProjImageInList * imgList = NULL;
    ProjImageInInterface * tempInImg = NULL; 
    
    socket.peek(&noInputs, sizeof(noInputs));
    
    // An individual image
    if ( noInputs == -1 ) 
    {
        tempInImg  = new ProjImageIn(ProjImageIn::createFromSocket( socket ));
        m_images.push_front(tempInImg);
        
    // Create a list of images
    } else
    { 
        socket.receive(&noInputs, sizeof(noInputs));
        socket.receive(&height, sizeof(height));
        socket.receive(&width, sizeof(width));
       
        imgList = new ProjImageInList(height, width); 
        m_images.push_back(imgList);
        
        for( int i = 1; i <= noInputs; ++i ) 
        {
            tempInImg  = new ProjImageIn(ProjImageIn::createFromSocket(socket));
            assert(imgList->appendHead(tempInImg)); 
        }

        tempInImg = imgList;
     }
    
    return tempInImg;
}

/******************************************************************************/

ProjImageOutInterface * 
ProjImageFactory::makeProjImageOut( ClientSocket & socket )
{
    ProjImageOutInterface * tempImgOut = NULL;
    
    tempImgOut = new ProjImageOut(ProjImageOut::createFromSocket( socket ));
    m_images.push_front(tempImgOut);

    return tempImgOut;
 
}

/******************************************************************************/

} // namespace 
