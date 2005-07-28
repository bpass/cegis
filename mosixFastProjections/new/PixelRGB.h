#ifndef _USGSMOSIX_PIXELRGB_H_
#define _USGSMOSIX_PIXELRGB_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file PixelRGB.h
 * 
 * \brief PixelInterface outlines a ``generic'' type which can 
 * be used for storing pixel information.  AbstractPixel of the 
 * ImageLib involved way too much casting for the purposes of
 * the MOSIX code.  RGB Defines a pixel with three channels, 
 * a red channel, a green channel, and a blue channel, also 
 * known as samples.
 * 
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "PixelInterface.h"

namespace USGSMosix 
{

template<class T>
class PixelRGB  : public PixelInterface<T>
{
public:
    PixelRGB(const T* redPointer ) : m_red(redPointer)  
    {
        if ( m_red == NULL ) 
            throw GeneralException("Pixel initialized with NULL value.");
    } 
   
    virtual ~PixelRGB() {}
    
    virtual void getRGB(T& r, T& g, T& b)const;
    
    virtual void getGrey(T& grey)const;

    //virtual void setRGB(const T& r, const T& g, const T& b);
    
    //virtual void setGrey(const T& grey);

private:
    inline const T* m_green()const { return (m_red + sizeof(m_red)); } 
    inline const T* m_blue()const { return (m_red + 2 * sizeof(m_red)); }  
    const T* m_red;
};

/******************************************************************************/
   
template<typename T>
inline void PixelRGB<T>::getRGB(T& r, T& g, T& b)const
{
    r = *m_red;
    g = *m_green();
    b = *m_blue();

    return;
}

/******************************************************************************/

template<typename T>
inline void PixelRGB<T>::getGrey(T& grey)const
{
    grey = ( 21 * (*m_red) + 32 * (*m_blue()) + 11 * (*m_green()) )  / 64;

    return;
}

/******************************************************************************/

/*template<typename T>
inline void PixelRGB<T>::setRGB(const T& r, const T& g, 
                                const T& b)
{
    *m_red = r; 
    *m_green() = g;
    *m_blue() = b;

    return;
}*/

/******************************************************************************/

/*
template<typename T>
inline void PixelRGB<T>::setGrey(const T& grey)
{
    *m_red = grey;
    *m_green() = grey;
    *m_blue() = grey;
    
    return;
}*/

/******************************************************************************/

} // namespace

#endif // _USGSMOSIX_PIXELRGB_H_
