#ifndef __USGSMOSIX_PROJIMAGEDATAOUTINTERFACE_H_
#define __USGSMOSIX_PROJIMAGEDATAOUTINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file ProjImageDataOutInterface.h
 * 
 * \brief Header file for ProjImageDataOutInterface class. 
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "Globals.h"

namespace USGSMosix 
{
    /// The ProjImageDataOutInterface serves as an abstract base  
    /// for objects which need to output re-projected image data. 
    class ProjImageDataOutInterface 
    {
    public:
        ProjImageDataOutInterface() {}
        
        virtual ~ProjImageDataOutInterface() {}

        virtual void putScanline( scanline_t scanline, 
                                  const unsigned int& lineNo )   = 0;

        virtual void putScanlines( scanlines_t scanlines, 
                                   const unsigned int& beginLine,
                                   const unsigned int& endLine )  = 0; 
    };

} // namespace 

#endif // __USGSMOSIX_PROJIMAGEDATAOUTINTERFACE_H_
