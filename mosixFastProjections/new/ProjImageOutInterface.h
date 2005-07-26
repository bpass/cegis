#ifndef _USGSMOSIX_PROJIMAGEOUTINTERFACE_H_
#define _USGSMOSIX_PROJIMAGEOUTINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file ProjImageInterface.h 
 * 
 * \brief The ProjImage object is meant to be a representation 
 * of a image for an image projection and all of its 
 * implicit characteristics.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "DRect.h"

namespace USGSMosix 
{
    class ProjImageOutInterface : public virtual ProjImageDataInterface 
    {
        public:
            
        ProjImageOutInterface() {} 
        virtual ~ProjImageOutInterface() {} 

        // virtual set 
        virtual void putScanline( scanline_t scanline, 
                                  const unsigned int& lineNo )   = 0;
        virtual void putScanlines( scanlines_t scanlines, 
                                   const unsigned int& height )  = 0; 
    };
    
} //namespace

#endif // _USGSMOSIX_PROJIMAGEOUTINTERFACE_H_

