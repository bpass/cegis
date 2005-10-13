#ifndef __USGSMOSIX_PROJIMAGEINLIST_H_
#define __USGSMOSIX_PROJIMAGEINLIST_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/10/13 22:27:40 $
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

#include "ProjImageInInterface.h"
#include "GeneralException.h"
#include <ProjectionLib/GeographicProjection.h>
#include <ImageLib/ImageCommon.h>
#include <list>
#include <utility>

namespace USGSMosix 
{

/// A storage container for projected images which allows for groups
/// of image(s) to be re-projected as a single object. 
class ProjImageInList : public ProjImageInInterface 
{
    public:
        ProjImageInList(unsigned long height, unsigned long width);
        
        ProjImageInList(const ProjImageScale & scale); 
        
        virtual ~ProjImageInList();

        /******************** image functonality ***************************/
        
        /// \brief exports each image parameter file to the socket
        /// so that the list can be reconstructed on the other end.
        virtual void exportToSocket( ClientSocket & socket )const;
        
        /// \brief Returns the virtual height in pixels.
        virtual long int getHeight()const;

        /// \brief Returns the virtual width in pixels. 
        virtual long int getWidth()const;
        
        /// \pre None.
        /// \post Returns the pixel to latitude/longitude  
        /// ratio for all images in the list.
        virtual ProjImageScale getPixelScale()const; 
        
        /// \brief sets the pixel scale of the image and reverse 
        /// calculates the width and height of the image.
        virtual void setPixelScale(const ProjImageScale &  p);
        
        /// \pre ProjImageInList must contain at least one image.
        /// \post ProjImageInList returns the outermost bounds for 
        /// the list of images in its possession in geographic coordinates.  
        /// If no images are in its possession, then the function throws 
        /// a GeneralException indicating an error has occured.
        virtual DRect getOuterBounds()const; 
        virtual double getLeftBound()const;
        virtual double getRightBound()const;
        virtual double getTopBound()const;
        virtual double getBottomBound()const;
        
        /// \pre None.
        /// \post getProjection returns the projection which is
        /// contained in every projimage in the list. If it is not
        /// the same, then a general exception is thrown.  If the
        /// list is empty, an exception is also thrown.
        virtual const ProjLib::Projection * getProjection()const;

        /// \param latitude The latitude of the pixel you are requesting.
        /// \param longitude The longitude of the pixel you are requesting.
        ///
        /// \brief Returns a pixel based off of latitude and longitude 
        /// coordinates in the image. 
        virtual const PixelInterface<sample_t> *
        getPixel( const double & latitude, const double & longitude )const;
        
        /// \brief Returns the common photometric (e.g., RGB, greyscale)
        virtual int getPhotometric() const;
        /// \brief Returns the common bits per sample (or channel.)
        virtual int getBPS() const;
        /// \brief Returns the samples per pixel.  Assumed to be 8 now.
        virtual int getSPP() const;
        /// \brief Returns the new bounds for the current image based off 
        /// of a forward projecting mesh which is forwarded to the function.
        /// \brief  
        virtual DRect getNewBounds(const PmeshLib::ProjectionMesh & mesh)const;
        /// \brief Returns a rectangle representing the latitude and longitude
        /// bounds for the image.
        virtual DRect getGeographicBounds()const;
        
        /// \param toProjection The destination projection to be 
        /// projected to from this image's current projection.  In other
        /// words, the mesh will be mapping from the current projimage's
        /// projection to the destination.
        /// 
        /// \param divisions The number of vertical/horizontal divisions
        /// made in the mesh.  Defaults to a globally defined constant.
        ///
        /// \param interp  The type of interpolation to be used in 
        /// mesh pixel getting.  Defaults to a globally defined constant.
        /// 
        /// \brief Sets up a forward mesh using this ProjImage's projection
        /// and a second Projection.  
        virtual const PmeshLib::ProjectionMesh & setupMesh(
            const ProjLib::Projection & toProjection,
            unsigned int divisions = kgMeshDivisions,  
            MathLib::InterpolatorType interp = kgInterpolator)const;

        /// \param fromProjection The source projection.  In other words,
        /// the mesh will be a mapping from fromProjection to the 
        /// current projection.
        /// 
        /// \param divisions The number of vertical/horizontal divisions
        /// made in the mesh.  Defaults to a globally defined constant.
        /// \param boundaries The boundaries for the source image in 
        /// degrees.
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
            unsigned int divisions = kgMeshDivisions, 
            MathLib::InterpolatorType interp = kgInterpolator)const;

        /******************** list functonality ***************************/
        
        /// \brief appends an image to the begining of the list.  Has 
        /// no effect on the iterator. 
        bool appendHead(ProjImageInInterface * img);

        /// \brief appends an image to the tail of the list.  Has 
        /// no effect on the iterator.
        bool appendTail(ProjImageInInterface * img);

        /// \brief removes the tail image from the list. 
        void removeTail(); 
        /// \brief removes the head image from the list.
        void removeHead();
        
        /// \brief Clears the image list of its contents. 
        void clear();
       
        /// \brief Returns the number of images in the image list.
        unsigned int size()const;
        
        /// \brief Returns a pointer to the first item in the list and
        /// resets the iterator to the first item in the list.  If the 
        /// list is empty, NULL is returned.  
        ProjImageInInterface* first()const;
        
        /// \brief Increments the iterator by a position.  
        /// Returns a pointer to the next item.  If there is no next item, 
        /// NULL is returned. 
        ProjImageInInterface* next()const;
        
        /// \brief Decrements the iterator by a position.  Returns a pointer 
        /// to the previous item in the list. 
        ProjImageInInterface* prev()const;
        
        /// \brief Returns a pointer to the last item in the list and
        /// resets the iterator to the last item in the list.  If the 
        /// list is empty, NULL is returned.  
        ProjImageInInterface* last()const;
        
    private:

        /// used to calculate geographic bounds and such of the images
        /// in the list.
        static const ProjLib::GeographicProjection m_geoProjection; 
        
        /// iterator over the list.
        mutable 
        std::list<std::pair<ProjImageInInterface*,DRect> >::const_iterator 
        m_iterator;
        
        /// the image list.
        std::list<std::pair<ProjImageInInterface*,DRect> > m_imgList;

        /// virtual height in pixels of image list.
        long int m_height;           

        /// virtual width in pixels of image list.
        long int m_width;            
        
        /// the unit/pixel scale.
        ProjImageScale m_pixelRatio; 

        /// the outermost bounds of the image, saved because the calculation
        /// is expensive.
        mutable DRect m_outermost; 

        /// has anything about the list been modified since the last time
        /// we calculated the outermost boundaries?
        mutable bool m_modified; 
};

/****************************************************************************/

inline bool ProjImageInList::appendHead(ProjImageInInterface * img)
{
    if ( img != NULL ) 
    {
        WRITE_DEBUG ( "appending to head" << std::endl; );
        m_modified = true;
        DRect rect = img->getGeographicBounds();
        m_imgList.push_front(std::pair<ProjImageInInterface*, DRect>(img,rect));
        return true;
    } 
    return false;
}

/****************************************************************************/

inline bool ProjImageInList::appendTail(ProjImageInInterface * img)
{
    if ( img != NULL )
    {
        m_modified = true;
        DRect rect = img->getGeographicBounds(); 
        m_imgList.push_back(std::pair<ProjImageInInterface*, DRect>(img,rect));
        return true;
    }
    return false;
}

/****************************************************************************/

inline void ProjImageInList::removeTail()
{
    m_modified = true;
    m_imgList.pop_back();
    
    return;
}

/****************************************************************************/
 
inline void ProjImageInList::removeHead()
{
    m_modified = true;
    m_imgList.pop_front();
    
    return;
}

/****************************************************************************/

inline void ProjImageInList::clear()
{
    m_modified = true;
    
    for( m_iterator = m_imgList.begin();  
         m_iterator != m_imgList.end(); 
         ++m_iterator ) 
    {
        delete m_iterator->first;
    }
    
    m_imgList.clear();

    m_iterator = m_imgList.begin();
    
    return;
}

/****************************************************************************/

inline ProjImageInInterface* ProjImageInList::first()const
{
    return (m_iterator = m_imgList.begin())->first;
}

/****************************************************************************/

inline ProjImageInInterface* ProjImageInList::next()const
{
    return (++m_iterator != m_imgList.end())?(m_iterator->first):(NULL);
}

/****************************************************************************/

inline ProjImageInInterface* ProjImageInList::prev()const
{
    return (--m_iterator != m_imgList.end())?(m_iterator->first):(NULL);
}

/****************************************************************************/

inline ProjImageInInterface* ProjImageInList::last()const
{
    return (m_iterator = m_imgList.end())->first;
}

/****************************************************************************/

inline long int ProjImageInList::getHeight()const
{
    return m_height;
}

/****************************************************************************/

inline long int ProjImageInList::getWidth()const 
{
    return m_width;
}

/****************************************************************************/

inline ProjImageScale ProjImageInList::getPixelScale()const
{
    return m_pixelRatio;
}

/****************************************************************************/

inline const PixelInterface<sample_t> *  
ProjImageInList::getPixel( const double & latit, const double & longit)const
{
    std::list<std::pair<ProjImageInInterface*, DRect> >::const_iterator kit;
    
    for ( kit = m_imgList.begin(); kit != m_imgList.end(); ++kit ) 
    {
        // if the latitude and longitude is contained within the 
        // bounding box
        if ( latit  <= kit->second.top && 
             latit  >= kit->second.bottom && 
             longit <= kit->second.right && 
             longit >= kit->second.left )
        {
            return kit->first->getPixel(latit,longit); 
        } 
    }
    
    // else return an empty pixel.
    return NULL; 
}

/****************************************************************************/

inline int ProjImageInList::getPhotometric() const 
{ 
   if ( m_imgList.size() > 0 )
   {
        std::list<std::pair<ProjImageInInterface*, DRect> >::const_iterator kit;
        for ( kit = m_imgList.begin(); kit != m_imgList.end(); ++kit ) 
        {
            if ( kit->first->getPhotometric() == PHOTO_RGB ) 
                return PHOTO_RGB;
        }
        return PHOTO_GREY;
   }
   else 
       throw GeneralException("Error, list is empty.");
} 

/****************************************************************************/

/// \note This could blow up.  It hasn't been tested with different Bits per
// sample yet.
inline int ProjImageInList::getBPS() const 
{ 
   int maxBPS = -1;
   if ( m_imgList.size() > 0 )
   {
        std::list<std::pair<ProjImageInInterface*, DRect> >::const_iterator kit;
        for ( kit = m_imgList.begin(); kit != m_imgList.end(); ++kit ) 
        {
            maxBPS = Math<int>::max(kit->first->getBPS(), maxBPS);
        }
        
        return maxBPS;
        
   } else
       throw GeneralException("Error, list is empty.");
}  

/****************************************************************************/

inline int ProjImageInList::getSPP() const 
{
   int maxSPP = -1;
    
   if ( m_imgList.size() > 0 )
   {
       std::list<std::pair<ProjImageInInterface*, DRect> >::const_iterator kit;
       for ( kit = m_imgList.begin(); kit != m_imgList.end(); ++kit ) 
       {
           maxSPP = Math<int>::max(kit->first->getSPP(), maxSPP);
       }
       
       return maxSPP; 
       
   } else
       throw GeneralException("Error, list is empty.");
}  

/****************************************************************************/

inline unsigned int ProjImageInList::size()const
{
    return static_cast<unsigned int>(m_imgList.size());    
}

/****************************************************************************/

} // namespace 

#endif // _USGSMOSIX_PROJIMAGEINLIST_H_
