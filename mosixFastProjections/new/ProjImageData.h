#ifndef _USGSMOSIX_PROJIMAGEDATA_H_
#define _USGSMOSIX_PROJIMAGEDATA_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file ProjImageData.h 
 * 
 * \brief A ProjImage object is meant to be a representation 
 * of a image for an image projection and all of its 
 * implicit characteristics.  The ProjImageData object 
 * exploits the commonality between ProjImageIn and ProjImageOut
 * and avoids repeated code by being in the inheritence tree
 * of both, implementing their common get/set and setup mesh 
 * funcitons.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "Globals.h"
#include <cassert>
#include <ImageLib/ImageFile.h>
#include "ProjImageInInterface.h"
#include "ProjImageOutInterface.h"
#include <MathLib/Point.h>
#include <DRect.h>
#include <string>


namespace USGSMosix
{

class ProjImageData : public virtual ProjImageDataInterface
{
    public:
        ProjImageData( DRect bounds, 
                       USGSImageLib::ImageFile * file, 
                       const ProjLib::Projection * proj );
        virtual ~ProjImageData(); 
        
        // virtual get methods
        virtual double getLeftBound()                        const;
        virtual double getRightBound()                       const;
        virtual double getTopBound()                         const;
        virtual double getBottomBound()                      const;
        virtual DRect  getOuterBounds()                      const;
        virtual long int getHeight()                         const;
        virtual long int getWidth()                          const; 
        virtual const ProjLib::Projection * getProjection()  const;
        virtual ProjImageScale getPixelScale()               const;
        virtual int getPhotometric()                         const;
        virtual int getBPS()                                 const;
        virtual int getSPP()                                 const;
       
        // additional get methods
        std::string getFilename()                            const;
        
        // virtual set methods
        virtual void setPixelScale( const ProjImageScale & s );
        
        // additional set functions
        void setBounds( const DRect& rect );
        void setLeftBound( const double& l ); 
        void setRightBound( const double& r );
        void setTopBound( const double& t );
        void setBottomBound( const double& b );
        void setWidth( const long int& w);
        void setHeight(const long int& h);
        void setFilename( std::string & s );

        virtual const PmeshLib::ProjectionMesh & setupMesh(
                const ProjLib::Projection & secondProjection,
                unsigned int divisions = kgMeshDivisions , 
                MathLib::InterpolatorType interp = kgInterpolator )const;

        virtual const PmeshLib::ProjectionMesh & setupReverseMesh(
                const ProjLib::Projection & secondProjection,
                const DRect & boundaries, 
                unsigned int divisions = kgMeshDivisions , 
                MathLib::InterpolatorType interp = kgInterpolator )const;
        
    protected:
        std::string getFileExtension( const std::string& filename );
        ProjImageScale m_scale;
        USGSImageLib::ImageFile * m_file;
        const ProjLib::Projection * m_proj;
    private:
        DRect m_boundaries;  ///< boundaries to scale, not lat, long or pixel.
};

/******************************************************************************/

inline const ProjLib::Projection * ProjImageData::getProjection()const
{ 
    return m_proj; 
}

/******************************************************************************/

inline ProjImageScale ProjImageData::getPixelScale() const
{
    return m_scale;
}

/******************************************************************************/

inline int ProjImageData::getPhotometric() const
{
    assert ( m_file );
    int i; m_file->getPhotometric(i);
    return i;
}
    
/******************************************************************************/

inline int ProjImageData::getBPS() const
{
    assert ( m_file );
    int i; m_file->getBitsPerSample(i);
    return i;
}

/******************************************************************************/

inline int ProjImageData::getSPP() const
{
    assert ( m_file );
    int i; m_file->getSamplesPerPixel(i);
    return i;
}

/******************************************************************************/

inline DRect ProjImageData::getOuterBounds() const
{
    return m_boundaries;
}

/******************************************************************************/

inline double ProjImageData::getLeftBound()const
{ 
    return m_boundaries.left; 
} 

/******************************************************************************/

inline double ProjImageData::getRightBound()const
{ 
    return m_boundaries.right; 
}

/******************************************************************************/

inline double ProjImageData::getTopBound()const
{ 
    return m_boundaries.top; 
} 

/******************************************************************************/

inline double ProjImageData::getBottomBound()const
{ 
    return m_boundaries.bottom; 
}  

/******************************************************************************/

inline long int ProjImageData::getWidth() const 
{
    assert ( m_file );
    long int w; m_file->getWidth(w);
    return w;
}

/******************************************************************************/

inline long int ProjImageData::getHeight() const 
{
    assert( m_file );
    long int h; m_file->getHeight(h);
    return h;
}

/******************************************************************************/

inline std::string ProjImageData::getFilename() const 
{
    assert( m_file );
    std::string s; m_file->getFileName(s); 
    return s;
}

/******************************************************************************/

inline void ProjImageData::setLeftBound(const double& l)
{ 
    m_boundaries.left = l; 
    return;
} 

/******************************************************************************/

inline void ProjImageData::setRightBound(const double& r)
{ 
    m_boundaries.right = r; 
    return;
} 

/******************************************************************************/

inline void ProjImageData::setTopBound(const double& t) 
{ 
    m_boundaries.top = t; 
    return;
} 

/******************************************************************************/

inline void ProjImageData::setBottomBound(const double& b) 
{ 
    m_boundaries.bottom = b; 
    return;
} 

/******************************************************************************/

inline void ProjImageData::setBounds(const DRect& rect)
{
    m_boundaries = rect;
    return;
}

/******************************************************************************/

inline void ProjImageData::setWidth(const long int & w)
{
    assert( m_file );
    m_file->setWidth(w);
    return;
}

/******************************************************************************/

inline void ProjImageData::setHeight(const long int & h)
{
    assert( m_file );
    m_file->setHeight(h);
    return; 
}

/******************************************************************************/

inline void ProjImageData::setFilename( std::string & s) 
{
    assert( m_file );
    m_file->setFileName(s); 
    return;
}

/******************************************************************************/

inline void ProjImageData::setPixelScale( const ProjImageScale & s ) 
{
    m_scale = s;
    return;
}

/******************************************************************************/

} // namespace USGSMosix

#endif // _PROJIMAGEDATA_H_
