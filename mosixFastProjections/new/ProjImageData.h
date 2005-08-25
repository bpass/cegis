#ifndef __USGSMOSIX_PROJIMAGEDATA_H_
#define __USGSMOSIX_PROJIMAGEDATA_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/25 21:07:29 $
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
        
        /// \brief Returns the left bound in degrees of the current image.
        virtual double getLeftBound()                        const;
        /// \brief Returns the right bound in degrees of the current image.
        virtual double getRightBound()                       const;
        /// \brief Returns the top bound in degress of the current image.
        virtual double getTopBound()                         const;
        /// \brief Returns the top bound in degress of the current image.
        virtual double getBottomBound()                      const;
        /// \brief Returns the outerbounds in degrees of the current image.
        virtual DRect  getOuterBounds()                      const;
        /// \brief Returns the height in pixels of the current image.
        virtual long int getHeight()                         const;
        /// \brief Returns the width in pixels of the current image. 
        virtual long int getWidth()                          const; 
        /// \brief Returns the projection of the current image.
        virtual const ProjLib::Projection * getProjection()  const;
        /// \brief Returns the units/pixel measurement of the image.
        virtual ProjImageScale getPixelScale()               const;
        /// \brief Returns the image's photometric (e.g., RGB, greyscale)
        virtual int getPhotometric()                         const;
        /// \brief Returns the image's bits per sample (or channel.)
        virtual int getBPS()                                 const;
        /// \brief Returns the samples per pixel. 
        virtual int getSPP()                                 const;
        /// \brief Returns the filename for the current image file.
        std::string getFilename()                            const;
        
        /// \brief Returns the pixel scale for the current image.
        virtual void setPixelScale( const ProjImageScale & s );
        
        /// \brief Sets bounds for the current image file (in degrees.) 
        void setBounds( const DRect& rect );
        /// \brief Sets the left bound for the current image file (in deg.)
        void setLeftBound( const double& l ); 
        /// \brief Sets the right bound for the current image file (in deg.)
        void setRightBound( const double& r );
        /// \brief Sets the top bound for the current image file (in deg.)
        void setTopBound( const double& t );
        /// \brief Sets the bottom bound for the current image file (in deg.)
        void setBottomBound( const double& b );
        /// \brief Sets the width of the current file (in pixels).
        void setWidth( const long int& w);
        /// \brief Sets the height of the current file (in pixels).
        void setHeight(const long int& h);
        /// \brief Sets the filename for the current image file.
        void setFilename( std::string & s );

        /// \param toProjection The destination projection to be 
        /// projected to from this image's current projection.  In other
        /// words, the mesh will be mapping from the current projimage's
        /// projection to the destination.
        /// 
        /// \param divisions The number of vertical/horizontal divisions
        /// made in the mesh.  Defaults to a globally defined constant.
        ///
        /// \param interp.  The type of interpolation to be used in 
        /// mesh pixel getting.  Defaults to a globally defined constant.
        /// 
        /// \brief Sets up a forward mesh using this ProjImage's projection
        /// and a second Projection.  
        virtual const PmeshLib::ProjectionMesh & setupMesh(
                const ProjLib::Projection & toProjection,
                unsigned int divisions = kgMeshDivisions , 
                MathLib::InterpolatorType interp = kgInterpolator )const;

        /// \param fromProjection The source projection.  In other words,
        /// the mesh will be a mapping from fromProjection to the 
        /// current projection.
        /// 
        /// \param divisions The number of vertical/horizontal divisions
        /// made in the mesh.  Defaults to a globally defined constant.
        ///
        /// \param interp.  The type of interpolation to be used in 
        /// mesh pixel getting.  Defaults to a globally defined constant.
        /// 
        /// \brief Sets up a reverse mesh using this ProjImage's projection
        /// and a second Projection.  
        virtual const PmeshLib::ProjectionMesh & setupReverseMesh(
                const ProjLib::Projection & fromProjection,
                const DRect & boundaries, 
                unsigned int divisions = kgMeshDivisions , 
                MathLib::InterpolatorType interp = kgInterpolator )const;
        
    protected:
        /// \param filename A image filename with an extension.
        /// \brief Returns the file extension of the filename passed 
        /// to it.
        std::string getFileExtension( const std::string& filename );

        /// the scale of the current image.
        ProjImageScale m_scale;
        /// a pointer to the current image.
        USGSImageLib::ImageFile * m_file;
        /// the projection which m_file is in.
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
