#ifndef _USGSMOSIX_PIXELINTERFACE_H_
#define _USGSMOSIX_PIXELINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 01:09:01 $
 *
 * \version 0.1
 * 
 * \file PixelInterface.h
 * 
 * \brief PixelInterface outlines a ``generic'' type which can 
 * be used for storing pixel information.  AbstractPixel of the 
 * ImageLib involved way too much casting for the purposes of
 * the MOSIX code.  So here we introduce a parameterized type.
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
class PixelInterface
{
public:
    PixelInterface() {} 
    virtual ~PixelInterface() {} 
    
    virtual void getRGB(sample_t& r, sample_t& g, sample_t& b)const = 0;
    
    virtual void getGrey(sample_t& g)const = 0;

    // virtual void setRGB(const sample_t& r, 
    //                     const sample_t& g, const sample_t&b) = 0;
    
    // virtual void setGrey(const sample_t& g) = 0;
};
    
} // USGSMosix

#endif // _USGSMOSIX_PIXELINTERFACE_

