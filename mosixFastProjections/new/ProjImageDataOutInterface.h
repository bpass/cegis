#ifndef __USGSMOSIX_PROJIMAGEDATAOUTINTERFACE_H_
#define __USGSMOSIX_PROJIMAGEDATAOUTINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 20:56:34 $
 *
 * \version 0.1
 * 
 * \file ProjOutImageOut.h 
 * 
 * \brief The ProjImageDataOutInterface serves as an abstract base  
 * for objects which need to output re-projected image data.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

namespace USGSMosix 
{
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
