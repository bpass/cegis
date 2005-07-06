
/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file ProjImageInList.h 
 * 
 * \brief The ProjImageInList is a storage container for Projected
 * Images.  
 * 
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include <cmath>
#include "ProjImageInList.h"

namespace USGSMosix
{
/****************************************************************************/

const ProjLib::GeographicProjection ProjImageInList::m_geoProjection;

/****************************************************************************/

ProjImageInList::ProjImageInList() :
    ProjImageInInterface(),
    m_height(0), m_width(0), m_pixelRatio()
{
    m_pixelRatio.x = 0;
    m_pixelRatio.y = 0;
}

/****************************************************************************/

ProjImageInList::~ProjImageInList()
{ 
    clear();
}

/****************************************************************************/

DRect ProjImageInList::getOuterBounds()const 
{
    std::list<std::pair<ProjImageInInterface*,DRect> >::const_iterator 
        kiterator;
    
    if ( m_modified == true ) 
    {
        m_modified = false;
        
        if ( (kiterator = m_imgList.begin()) != m_imgList.end() )
            m_outermost = (kiterator->first)->getOuterBounds();
        else
            throw 
            GeneralException("ProjImageInList::getm_outermostBounds() error");
        
        for( ++kiterator; kiterator != m_imgList.end(); ++kiterator )
        {
            // get the top-most bound 
            m_outermost.top    = Math<double>::max(m_outermost.top, 
                                 (kiterator->second.top));              
            
            // get the bottom-most bound
            m_outermost.bottom = Math<double>::max(m_outermost.bottom, 
                                 (kiterator->second.bottom));
            
            // get the left-most bound 
            m_outermost.left   = Math<double>::max(m_outermost.left, 
                                 (kiterator->second.left));

            // get the right-most bound 
            m_outermost.right  = Math<double>::max(m_outermost.right, 
                                 (kiterator->second.right));
        }
    }
    
    return m_outermost;
}

/****************************************************************************/

double ProjImageInList::getLeftBound()const 
{
   getOuterBounds(); 
   return m_outermost.left;
}

/****************************************************************************/
    
double ProjImageInList::getRightBound()const 
{
    getOuterBounds();
    return m_outermost.right;
}
    
/****************************************************************************/

double ProjImageInList::getTopBound()const
{
    getOuterBounds();
    return m_outermost.top;
}
    
/****************************************************************************/

double ProjImageInList::getBottomBound()const 
{
    getOuterBounds();
    return m_outermost.bottom;
}

/****************************************************************************/

const ProjLib::Projection * ProjImageInList::getProjection()const 
{
    return &m_geoProjection;
}

/****************************************************************************/

void ProjImageInList::setPixelScale( const ProjImageScale &  p)
{
    // TODO......
    (void)p;
    return;
}

/****************************************************************************/

} // namespace
