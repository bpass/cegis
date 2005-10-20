#ifndef __USGSMOSIX_PROJIMAGEDATAOUTINTERFACE_H_
#define __USGSMOSIX_PROJIMAGEDATAOUTINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/10/20 23:21:15 $
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

        /// \param scanline The scanline raw data to be placed into the
        /// ProjImage. 
        /// \param lineNo Number indicating which line number's raw data
        /// is being placed.
        virtual void putScanline( scanline_t scanline, 
                                  const unsigned int& lineNo )   = 0;

        /// \param scanlines The scanline's raw data to be placed into the
        /// ProjImage.  A contiguous block of scanlines in memory.
        /// \param beginLine A number indicating the begining scanline at 
        /// which the raw data begins --zero based.
        /// \param endLine A number indicating the ending scanline at 
        /// which the raw data begins --zero based.
        virtual void putScanlines( scanlines_t scanlines, 
                                   const unsigned int& beginLine,
                                   const unsigned int& endLine )  = 0; 
    };

} // namespace 

#endif // __USGSMOSIX_PROJIMAGEDATAOUTINTERFACE_H_
