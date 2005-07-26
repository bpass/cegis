#ifndef _USGSMOSIX_PROJIMAGEINLIST_H_
#define _USGSMOSIX_PROJIMAGEINLIST_H_

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

#include "ProjImageInInterface.h"
#include "GeneralException.h"
#include <ProjectionLib/GeographicProjection.h>
#include <list>
#include <utility>

namespace USGSMosix 
{

class ProjImageInList : public ProjImageInInterface 
{
    public:
        
        ProjImageInList(unsigned long height, unsigned long width);
        
        ProjImageInList(const ProjImageScale & scale); 
        
        virtual ~ProjImageInList();

        // INTERFACE FUNCTIONS

        /// get the virtual bounds of the image 
        virtual long int getHeight()const;
        virtual long int getWidth()const;
        
        /// \pre None.
        /// \post Returns the pixel to latitude/longitude  
        /// ratio for all images in the list.
        virtual ProjImageScale getPixelScale()const; 
        virtual void setPixelScale(const ProjImageScale &  p);
        
        /// \pre ProjImageInList must contain at least one image.
        /// \post ProjImageInList returns the outermost bounds for 
        /// the list of images in its possession.  If no images
        /// are in its possession, then the function throws 
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
       
        inline const unsigned char * 
        getPixel( const double & x, const double & y)const;

        virtual const unsigned char *  
        getPixel( const unsigned int & x, const unsigned int & y )const; 
        
        virtual int getPhotometric() const;
        virtual int getBPS() const;
        virtual int getSPP() const; 
        virtual DRect getNewBounds(const PmeshLib::ProjectionMesh & mesh)const;
        virtual DRect getGeographicBounds()const;
        
        virtual const PmeshLib::ProjectionMesh & setupMesh(
            const ProjLib::Projection & fromProjection,
            unsigned int divisions = kgMeshDivisions,  
            MathLib::InterpolatorType interp = kgInterpolator)const;

        virtual const PmeshLib::ProjectionMesh & setupReverseMesh(
            const ProjLib::Projection & fromProjection,
            const DRect & boundaries, 
            unsigned int divisions = kgMeshDivisions, 
            MathLib::InterpolatorType interp = kgInterpolator)const;

        /// LIST FUNCTIONALITY 
        
        // insert and remove functions
        bool appendHead(ProjImageInInterface * img);
        bool appendTail(ProjImageInInterface * img);
        void removeTail(); 
        void removeHead();
        void clear();

        // iterator control
        ProjImageInInterface* first();
        ProjImageInInterface* next();
        ProjImageInInterface* prev();
        ProjImageInInterface* last();
        
    private:
        static const ProjLib::GeographicProjection m_geoProjection; 
        std::list<std::pair<ProjImageInInterface*,DRect> >::iterator m_iterator;
        std::list<std::pair<ProjImageInInterface*,DRect> > m_imgList;
        long int m_height;           
        long int m_width;            
        ProjImageScale m_pixelRatio; 
        mutable DRect m_outermost; 
        mutable bool m_modified; 
};

/****************************************************************************/

inline bool ProjImageInList::appendHead(ProjImageInInterface * img)
{

    if ( img != NULL ) 
    {
        std::cout << "appending to head" << std::endl;
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
    m_iterator = m_imgList.begin();
    
    for( size_t i = 0;  i < m_imgList.size(); ++i ) 
    {
        if (m_iterator != m_imgList.end())
            delete m_iterator->first;
    }
    
    m_imgList.clear();

    m_iterator = m_imgList.begin();
    
    return;
}

/****************************************************************************/

inline ProjImageInInterface* ProjImageInList::first()
{
    return (m_iterator = m_imgList.begin())->first;
}

/****************************************************************************/

inline ProjImageInInterface* ProjImageInList::next()
{
    return (++m_iterator != m_imgList.end())?(m_iterator->first):(NULL);
}

/****************************************************************************/

inline ProjImageInInterface* ProjImageInList::prev()
{
    return (--m_iterator != m_imgList.end())?(m_iterator->first):(NULL);
}

/****************************************************************************/

inline ProjImageInInterface* ProjImageInList::last()
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

inline const unsigned char * 
ProjImageInList::getPixel( const double & latit, const double & longit)const
{
    std::list<std::pair<ProjImageInInterface*, DRect> >::const_iterator kit;
    //std::cout << std::endl << "latitude: " << latit << std::endl;
    //std::cout << "longitude" << longit << std::endl;
    
    for ( kit = m_imgList.begin(); kit != m_imgList.end(); ++kit ) 
    {
        // if the latitude and longitude is contained within the 
        // bounding box
        if ( latit  <= kit->second.top && 
             latit  >= kit->second.bottom && 
             longit <= kit->second.right && 
             longit >= kit->second.left )
        {
            // std::cout << "not NULL!!!!!!!!!!!!!!!!" << std::endl;
            return kit->first->getPixel(latit,longit); 
        } 
    }
    
    // else return an empty pixel.
    return NULL; 
}

/****************************************************************************/

inline int ProjImageInList::getPhotometric() const 
{ 
   // TODO: always make appear as a RGB.
   if ( m_imgList.size() > 0 )
       return m_imgList.begin()->first->getPhotometric();
   else 
       throw GeneralException("Error, list is empty.");
} 

/****************************************************************************/

/// \note The assumption here is everything has the same BPS in the list.  
/// This could NOT be the case, in which case this function will, 
/// inevitably, blow up.  
inline int ProjImageInList::getBPS() const 
{ 
   if ( m_imgList.size() > 0 )
       return m_imgList.begin()->first->getBPS();
   else
       throw GeneralException("Error, list is empty.");
}  

/****************************************************************************/

inline int ProjImageInList::getSPP() const 
{ 
   // TODO: account for different samples per pixels.
   if ( m_imgList.size() > 0 )
       return m_imgList.begin()->first->getSPP();
   else
       throw GeneralException("Error, list is empty.");
}  

/****************************************************************************/

inline const unsigned char * 
ProjImageInList::getPixel(const unsigned int & x, const unsigned int & y)const
{
   (void)x;
   (void)y;
   return NULL; 
}

/****************************************************************************/

} // namespace 

#endif // _USGSMOSIX_PROJIMAGEINLIST_H_
