#ifndef _USGSMOSIX_PIXELRGB_H_
#define _USGSMOSIX_PIXELRGB_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file PixelRGB.h
 * 
 * \brief Header and implementation file for PixelRGB. 
 * 
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "PixelInterface.h"

namespace USGSMosix 
{

/// PixelRGB defines a pixel with three channels, 
/// a red channel, a green channel, and a blue channel, also 
/// known as samples.  Note: allows for const and non-const 
/// functionality for the forwarded pointer.
/// Templated on the data type of the channel or samples. */
template<class T>
class PixelRGB  : public PixelInterface<T>
{
public:
    PixelRGB(T* redPointer ) : m_red(redPointer), m_kred(m_red)
    {
        if ( m_kred == NULL ) 
            throw GeneralException("Pixel initialized with NULL value.");
    }
    
    PixelRGB(const T* redPointer ) : m_red(NULL), m_kred(redPointer) 
    {
        if ( m_kred == NULL ) 
            throw GeneralException("Pixel initialized with NULL value.");
    } 
   
    virtual ~PixelRGB() {}
 
    /// \param r The Red value.
    /// \param g The Green value.
    /// \param b The Blue value.
    /// \brief Gets the Red, green and blue values for the current pixel.
    virtual void getRGB(T& r, T& g, T& b)const;
   
    /// \param grey The grey value.
    /// \brief Returns the grey value for the current pixel.
    virtual void getGrey(T& grey)const;

    /// \param r The Red value.
    /// \param g The Green value.
    /// \param b The Blue value.
    /// \brief Sets the R,G,B values for the current pixel.
    virtual void setRGB(const T& r, const T& g, const T& b);
  
    /// \param grey The grey value.
    /// \brief Sets the grey value for the current pixel.
    virtual void setGrey(const T& grey);

private:
    /// \brief returns a const pointer to the green value.
    inline const T* m_kgreen()const { return (m_kred + sizeof(m_kred)); } 
    /// \brief returns a const pointer to the blue value.
    inline const T* m_kblue()const { return (m_kred + 2 * sizeof(m_kred)); }  
    /// \brief returns a pointer to the green value.
    inline T* m_green()const { return (m_red + sizeof(m_kred)); } 
    /// \brief returns a pointer to the blue value.
    inline T* m_blue()const { return (m_red + 2 * sizeof(m_kred)); }  
    
    T* m_red;
    const T* m_kred;
};

/******************************************************************************/
   
template<typename T>
inline void PixelRGB<T>::getRGB(T& r, T& g, T& b)const
{
    r = *m_kred;
    g = *m_kgreen();
    b = *m_kblue();

    return;
}

/******************************************************************************/

template<typename T>
inline void PixelRGB<T>::getGrey(T& grey)const
{
    grey = ( 21 * (*m_kred) + 32 * (*m_kblue()) + 11 * (*m_kgreen()) )  / 64;

    return;
}

/******************************************************************************/

template<typename T>
inline void PixelRGB<T>::setRGB(const T& r, const T& g, 
                                const T& b)
{
    if ( m_red != NULL  )
    {
        *m_red = r; 
        *m_green() = g;
        *m_blue() = b;
    } else throw GeneralException("Error, initialized with const pointer.");
    
    return;
}

/******************************************************************************/

template<typename T>
inline void PixelRGB<T>::setGrey(const T& grey)
{
    if ( m_red != NULL ) 
    {
        *m_red = grey;
        *m_green() = grey;
        *m_blue() = grey;
    } else throw GeneralException("Error, initialized with const pointer.");
    
    return;
}

/******************************************************************************/

} // namespace

#endif // _USGSMOSIX_PIXELRGB_H_
