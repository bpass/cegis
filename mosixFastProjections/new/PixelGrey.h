#ifndef _USGSMOSIX_PIXELGREY_H_
#define _USGSMOSIX_PIXELGREY_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file PixelGrey.h
 * 
 * \brief Implementation and header file for PixelGrey class.
 * 
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

namespace USGSMosix 
{

/// PixelGrey defines a pixel with one channel which measures brightness.  
/// Allows for const and non-const functionality, depending on how initialized.
/// Templated over the data type used for the sample or channel data.  
template<class T>
class PixelGrey  : public PixelInterface<T> 
{
public:

    PixelGrey( T* greyPointer) : m_kgrey(greyPointer), m_grey(greyPointer) 
    {
        if ( m_grey == NULL ) 
            throw GeneralException("Pixel initialized with NULL value.");
    }
    
    PixelGrey( const T * greyPointer ) : m_kgrey(greyPointer), 
                                         m_grey(NULL)  
    {
        if ( m_kgrey == NULL ) 
            throw GeneralException("Pixel initialized with NULL value.");
    } 
   
    virtual ~PixelGrey() {} 
    
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
   const T * m_kgrey;
   T* m_grey;
};

/******************************************************************************/
    
template<typename T>
inline void PixelGrey<T>::getRGB(T& r, T& g, T& b)const
{
    r = *m_kgrey;
    g = *m_kgrey;
    b = *m_kgrey;
    return;
}

/******************************************************************************/

template<typename T>
inline void PixelGrey<T>::getGrey(T& grey)const
{
    grey = *m_kgrey; 
    return;
}

/******************************************************************************/


template<typename T>
inline void PixelGrey<T>::setRGB(const T& r, const T& g, 
                                 const T& b)
{
    if ( m_grey != NULL ) 
    {
        *m_grey = (21 * r + 32 * g + 11 * b ) / 64; 
    } else throw GeneralException("Error, initialized with const pointer.");
    return;
}

/******************************************************************************/


template<typename T>
inline void PixelGrey<T>::setGrey(const T& grey)
{
    if ( m_grey != NULL )
    {
        *m_grey = grey;
    } else throw GeneralException("Error, initialized with const pointer.");
    return;
}

/******************************************************************************/

} // namespace

#endif // _USGSMOSIX_PIXELGREY_H_
