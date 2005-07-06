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
        ProjImageInList();
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
       
        virtual const unsigned char *  
        getPixel( const unsigned int & x, const unsigned int & y )const; 
        
        virtual int getPhotometric() const { return 0; } // TODO....
        virtual int getBPS() const { return 0; }   // TODO....
        virtual int getSPP() const { return 0; }   // TODO....
        virtual DRect getNewBounds(const PmeshLib::ProjectionMesh & mesh)const;
        virtual const PmeshLib::ProjectionMesh & setupMesh(
            const ProjLib::Projection & secondProjection,
            unsigned int divisions, 
            MathLib::InterpolatorType interp );

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
        long int m_height;           ///< TODO: calculate this or input it
        long int m_width;            ///< TODO: calculate this or input it
        ProjImageScale m_pixelRatio; ///< TODO: calculate this or input it.
        mutable DRect m_outermost; 
        mutable bool m_modified; 
};

/****************************************************************************/

inline bool ProjImageInList::appendHead(ProjImageInInterface * img)
{

    if ( img != NULL ) 
    {
        m_modified = true;
        DRect rect = img->getNewBounds(img->setupMesh(m_geoProjection)); 
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
        DRect rect = img->getNewBounds(img->setupMesh(m_geoProjection)); 
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
ProjImageInList::getPixel(const unsigned int & x, const unsigned int & y)const
{
   /// TODO.......
   (void)x;
   (void)y;
   return NULL; 
}

/****************************************************************************/
inline
DRect ProjImageInList::getNewBounds(const PmeshLib::ProjectionMesh & mesh)const
{
    // TODO ............
    (void)mesh;
    return *(new DRect);
}

/****************************************************************************/
inline
const PmeshLib::ProjectionMesh & ProjImageInList::setupMesh(
    const ProjLib::Projection & secondProjection,
    unsigned int divisions, 
    MathLib::InterpolatorType interp )
{
    // TODO.............
    (void)secondProjection;
    (void)divisions;
    (void)interp;
    return *(new PmeshLib::ProjectionMesh);
}

/****************************************************************************/

} // namespace 

#endif // _USGSMOSIX_PROJIMAGEINLIST_H_
