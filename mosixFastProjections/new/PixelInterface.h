#ifndef _USGSMOSIX_PIXELINTERFACE_H_
#define _USGSMOSIX_PIXELINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file PixelInterface.h
 * 
 * \brief Header file for PixelInterface class.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "PixelInterface.h"

namespace USGSMosix 
{

/// PixelInterface outlines a ``generic'' type which can be used for 
/// storing pixel information.  AbstractPixel of the ImageLib involved way too 
/// much casting for the purposes of the MOSIX code.  So here we introduce a 
/// parameterized type templated on the data type of the sample or channels.
template<class T>
class PixelInterface
{
public:
    PixelInterface() {} 
    virtual ~PixelInterface() {} 
   
    /// \param r The Red value.
    /// \param g The Green value.
    /// \param b The Blue value.
    /// \brief Gets the Red, green and blue values for the current pixel.
    virtual void getRGB(sample_t& r, sample_t& g, sample_t& b)const = 0;
    
    /// \param g The grey value.
    /// \brief Returns the grey value for the current pixel.
    virtual void getGrey(sample_t& g)const = 0;

    /// \param r The Red value.
    /// \param g The Green value.
    /// \param b The Blue value.
    /// \brief Sets the R,G,B values for the current pixel.
    virtual void setRGB(const sample_t& r, 
                        const sample_t& g, const sample_t&b) = 0;
    
    /// \param g The grey value.
    /// \brief Sets the grey value for the current pixel.
    virtual void setGrey(const sample_t& g) = 0;
};
    
} // USGSMosix

#endif // _USGSMOSIX_PIXELINTERFACE_

