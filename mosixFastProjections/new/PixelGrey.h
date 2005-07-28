#ifndef _USGSMOSIX_PIXELGREY_H_
#define _USGSMOSIX_PIXELGREY_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file PixelGrey.h
 * 
 * \brief PixelInterface outlines a ``generic'' type which can 
 * be used for storing pixel information.  AbstractPixel of the 
 * ImageLib involved way too much casting for the purposes of
 * the MOSIX code.  PixelGrey defines a pixel with one channels 
 * (or sample) which measures brightness.  
 * 
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

namespace USGSMosix 
{

template<class T>
class PixelGrey  : public PixelInterface<T> 
{
public:

    PixelGrey( const T * greyPointer ) : m_grey(greyPointer)  
    {
        if ( m_grey == NULL ) 
            throw GeneralException("Pixel initialized with NULL value.");
    } 
   
    virtual ~PixelGrey() {} 
    
    virtual void getRGB(T& r, T& g, T& b)const;
    
    virtual void getGrey(T& grey)const;

    // virtual void setRGB(const T& r, const T& g, const T& b);
    // virtual void setGrey(const T& grey);

private:
   const T * m_grey;
};

/******************************************************************************/
    
template<typename T>
inline void PixelGrey<T>::getRGB(T& r, T& g, T& b)const
{
    r = *m_grey;
    g = *m_grey;
    b = *m_grey;
    return;
}

/******************************************************************************/

template<typename T>
inline void PixelGrey<T>::getGrey(T& grey)const
{
    grey = *m_grey; 
    return;
}

/******************************************************************************/

/*
template<typename T>
inline void PixelGrey<T>::setRGB(const T& r, const T& g, 
                                 const T& b)
{
    *m_grey = (21 * r + 32 * g + 11 * b ) / 64; 
    return;
}*/

/******************************************************************************/

/*
template<typename T>
inline void PixelGrey<T>::setGrey(const T& grey)
{
    *m_grey = grey;
    return;
}*/

/******************************************************************************/

} // namespace

#endif // _USGSMOSIX_PIXELGREY_H_
