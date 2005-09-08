
/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file ProjImageInList.h 
 * 
 * \brief Implementation file for the ProjImageInList class.
 * 
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include <DRect.h>
#include <cmath>
#include "ProjImageInList.h"

namespace USGSMosix
{
 
// static members    
/****************************************************************************/

const ProjLib::GeographicProjection ProjImageInList::m_geoProjection;

// function definitions
/****************************************************************************/

ProjImageInList::ProjImageInList(unsigned long height, unsigned long width) :
    ProjImageInInterface(),
    m_height(height), m_width(width), m_modified(false)
{
    m_pixelRatio.x = Math<double>::abs((getRightBound() - getLeftBound())) 
                   / m_width;

    m_pixelRatio.y = Math<double>::abs((getTopBound() - getBottomBound()))
                   / m_height;
}

/****************************************************************************/
        
ProjImageInList::ProjImageInList(const ProjImageScale & scale) :
    ProjImageInInterface()
{
    setPixelScale(scale);
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
   
    // if the list contents have been modified since the last time
    // we looked at the outerbounds.
    if ( m_modified == true ) 
    {
        // reset the flag
        m_modified = false;
        
        if ( (kiterator = m_imgList.begin()) != m_imgList.end() )
        {
            m_outermost = (kiterator->second);
            std::cout << "first rect:" << m_outermost << std::endl;
        } else
            throw 
            GeneralException("ProjImageInList::getOutermostBounds() error");
        
        for( ++kiterator; kiterator != m_imgList.end(); ++kiterator )
        {
            std::cout << "second rect:" << kiterator->second << std::endl; 
            
            // get the top-most bound 
            m_outermost.top    = Math<double>::max(m_outermost.top, 
                                 (kiterator->second.top));              
            
            // get the bottom-most bound
            m_outermost.bottom = Math<double>::min(m_outermost.bottom, 
                                 (kiterator->second.bottom));
            
            // get the left-most bound 
            m_outermost.left   = Math<double>::min(m_outermost.left, 
                                 (kiterator->second.left));

            // get the right-most bound 
            m_outermost.right  = Math<double>::max(m_outermost.right, 
                                 (kiterator->second.right));
        }
        std::cout << "List bounds:  " << m_outermost << std::endl;
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
    m_pixelRatio = p;
    
    m_width = Math<long int>::ceil (
              Math<double>::abs( getRightBound() - getLeftBound() ) 
              / m_pixelRatio.x ) ;

    m_height = Math<long int>::ceil ( 
               Math<double>::abs( getTopBound() - getBottomBound() ) 
               / m_pixelRatio.y ) ; 
    return;
}

/****************************************************************************/

DRect ProjImageInList::getNewBounds(const PmeshLib::ProjectionMesh & mesh)const
{
    const long int inHeight = this->getHeight();
    const long int inWidth =  this->getWidth();
    DRect sourceBounds = this->getOuterBounds();
    std::list<double> xPts, yPts;
    double xSrcScale(0), ySrcScale(0);
    long int xPixelCount(0), yPixelCount(0);
    DRect outputBounds;
    
    for( xPixelCount = 0; xPixelCount < inWidth; ++xPixelCount )
    {
        // reproject the top boundary
        xSrcScale = sourceBounds.left + m_pixelRatio.x * xPixelCount;
        ySrcScale = sourceBounds.top;
        mesh.projectPoint(xSrcScale,ySrcScale); // modifies parameters

        xPts.push_back(xSrcScale);
        yPts.push_back(ySrcScale);

        // reproject the bottom boundary
        xSrcScale = sourceBounds.left + m_pixelRatio.x * xPixelCount;
        ySrcScale = sourceBounds.bottom;
        mesh.projectPoint(xSrcScale,ySrcScale); // modifies parameters
        
        xPts.push_back(xSrcScale);
        yPts.push_back(ySrcScale); 
    }
    
    for( yPixelCount = 0; yPixelCount < inHeight; ++yPixelCount ) 
    {
        // reproject the left line
        ySrcScale = sourceBounds.top - m_pixelRatio.y * yPixelCount;
        xSrcScale = sourceBounds.left;
        mesh.projectPoint(xSrcScale,ySrcScale); // modifies parameters

        xPts.push_back(xSrcScale);
        yPts.push_back(ySrcScale); 

        // reproject the right line
        ySrcScale = sourceBounds.top - m_pixelRatio.y * yPixelCount;
        xSrcScale = sourceBounds.right;
        mesh.projectPoint(xSrcScale,ySrcScale); // modifies parameters

        xPts.push_back(xSrcScale);
        yPts.push_back(ySrcScale); 
    }
   
    // calculate the new image's bounds in terms of the 
    // old map's measurement system.
    outputBounds.left   = Math<double>::getMin(xPts);
    outputBounds.right  = Math<double>::getMax(xPts);
    outputBounds.bottom = Math<double>::getMin(yPts);
    outputBounds.top    = Math<double>::getMax(yPts);

    return outputBounds;
}

inline DRect ProjImageInList::getGeographicBounds()const
{
    return getOuterBounds();
}

/****************************************************************************/

const PmeshLib::ProjectionMesh & ProjImageInList::setupMesh(
    const ProjLib::Projection & toProjection,
    unsigned int divisions, 
    MathLib::InterpolatorType interp )const
{
    PmeshLib::ProjectionMesh * mesh;
    mesh = new(std::nothrow) PmeshLib::ProjectionMesh;

    try 
    { 
        if ( mesh == NULL ) 
            throw GeneralException("NULL in setting up mesh."); 

        mesh->setSourceMeshBounds( getLeftBound(),
                                   getBottomBound(),
                                   getRightBound(),
                                   getTopBound() );

        mesh->setMeshSize( divisions, divisions );
        mesh->setInterpolator( interp );
        mesh->calculateMesh( m_geoProjection, toProjection );

    } catch ( GeneralException & ge ) 
    {
        std::cout << ge.toString() << std::endl;
        throw;
    }

    return *(mesh);

}

/****************************************************************************/

const PmeshLib::ProjectionMesh & ProjImageInList::setupReverseMesh(
    const ProjLib::Projection & fromProjection,
    const DRect & boundaries,
    unsigned int divisions, 
    MathLib::InterpolatorType interp )const
{
    PmeshLib::ProjectionMesh * mesh;
    mesh = new(std::nothrow) PmeshLib::ProjectionMesh;

    try { 

        if ( mesh == NULL ) 
            throw GeneralException("NULL in setting up mesh.");

        mesh->setSourceMeshBounds( boundaries.left, 
                                   boundaries.bottom,
                                   boundaries.right,
                                   boundaries.top );

        mesh->setMeshSize( divisions, divisions );
        mesh->setInterpolator( interp );
        mesh->calculateMesh( fromProjection, m_geoProjection );

    } catch ( GeneralException & ge ) 
    {
        std::cout << ge.toString() << std::endl; 
        throw;
    }   
    
    
    return *(mesh);
}

/****************************************************************************/

void ProjImageInList::exportToSocket( ClientSocket & socket )const 
{
    std::list<std::pair<ProjImageInInterface*,DRect> >::const_iterator 
        kiterator = m_imgList.begin();
    unsigned int _size = size();

    socket.send(&_size, sizeof(_size));
    socket.send(&m_height, sizeof(m_height));
    socket.send(&m_width, sizeof(m_width));
    
    for( ; kiterator != m_imgList.end(); ++kiterator )
    {
        kiterator->first->exportToSocket(socket); 
    }
}

/****************************************************************************/

} // namespace
