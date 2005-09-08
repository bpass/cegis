#ifndef __USGSMOSIX_PROJIMAGEDATA_H_
#define __USGSMOSIX_PROJIMAGEDATA_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file ProjImageData.h 
 * 
 * \brief Header file for ProjImageData class. 
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

/// The ProjImageData object exploits the commonality between ProjImageIn and 
/// ProjImageOut and avoids repeated code by being in the inheritence tree
/// of both, implementing their common get/set and setup mesh funcitons.
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
        /// \brief Returns a pointer to the image file.
        USGSImageLib::ImageFile* getImageFile()              const; 
        
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
        /// \brief Sets the image photometric: RGB, black and white, etc.
        void setPhotometric(int photo);
        /// \brief Sets the Bits per sample
        void setBPS(int bps);
        /// \brief Sets the samples per pixel
        void setSPP(int spp);
        /// \brief Sets the image file to the forwarded pointer and syncs
        /// the data members indicating samples per pixel, bits per sample,
        /// filename, height, width, etc., to be those that are contained in
        /// the ImageFile.
        void setImageFile( USGSImageLib::ImageFile* file );
        
        /// \param toProjection The destination projection to be 
        /// projected to from this image's current projection.  In other
        /// words, the mesh will be mapping from the current projimage's
        /// projection to the destination.
        /// 
        /// \param divisions The number of vertical/horizontal divisions
        /// made in the mesh.  Defaults to a globally defined constant.
        ///
        /// \param interp The type of interpolation to be used in 
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
        /// \param boundaries The boundaries for the source image in 
        /// degrees.
        ///
        /// \param interp  The type of interpolation to be used in 
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
                /// the projection which m_file is in.
        const ProjLib::Projection * m_proj;
    private:
        /// a pointer to the current image.
        USGSImageLib::ImageFile * m_file;
        /// boundaries to scale, not lat, long or pixel.
        DRect m_boundaries; 
        int m_photo, m_bps, m_spp;
        long int m_width, m_height;
        std::string m_filename;
        mutable std::list<PmeshLib::ProjectionMesh *> m_meshes;
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

/****************************************************************************/

inline int ProjImageData::getPhotometric() const
{
    return m_photo;
}
    
/******************************************************************************/

inline int ProjImageData::getBPS() const
{
    return m_bps;
}

/******************************************************************************/

inline int ProjImageData::getSPP() const
{
    return m_spp;
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
    return m_width;
}

/******************************************************************************/

inline long int ProjImageData::getHeight() const 
{
    return m_height;
}

/******************************************************************************/

inline std::string ProjImageData::getFilename() const 
{
    return m_filename;
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
    m_width = w;
    if ( m_file ) m_file->setWidth(w);
    return;
}

/******************************************************************************/

inline void ProjImageData::setHeight(const long int & h)
{
    m_height = h;
    if ( m_file ) m_file->setHeight(h);
    return; 
}

/******************************************************************************/

inline void ProjImageData::setFilename( std::string & s) 
{
    m_filename = s;
    if ( m_file ) m_file->setFileName(s); 
    return;
}

/******************************************************************************/

inline void ProjImageData::setPhotometric(int photo)
{
    m_photo = photo;
    if ( m_file ) m_file->setPhotometric(photo);
}

/******************************************************************************/

inline void ProjImageData::setBPS(int bps)
{
    m_bps = bps;
    if ( m_file ) m_file->setBitsPerSample(bps);
}

/******************************************************************************/

inline void ProjImageData::setSPP(int spp)
{
    m_spp = spp;
    if ( m_file ) m_file->setSamplesPerPixel(spp);
}

/******************************************************************************/

inline void ProjImageData::setPixelScale( const ProjImageScale & s ) 
{
    m_scale = s;
    return;
}

/******************************************************************************/

inline USGSImageLib::ImageFile * ProjImageData::getImageFile()const
{
    return m_file;
}

/******************************************************************************/

inline void ProjImageData::setImageFile( USGSImageLib::ImageFile* file )
{
    m_file = file;
    
    if ( m_file != NULL )
    {
        m_file->getSamplesPerPixel(m_spp);
        m_file->getBitsPerSample(m_bps);
        m_file->getPhotometric(m_photo);
        m_file->getFileName(m_filename);
        m_file->getHeight(m_height);
        m_file->getWidth(m_width);
    }
    return;
}

/******************************************************************************/

} // namespace USGSMosix

#endif // _PROJIMAGEDATA_H_
