#ifndef _USGSMOSIX_PROJIMAGEOUTINTERFACE_H_
#define _USGSMOSIX_PROJIMAGEOUTINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file ProjImageOutInterface.h
 * 
 * \brief Header file for the ProjImageOutInterface class 
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "ProjImageDataOutInterface.h"
#include "ProjImageOutPiece.h" 

namespace USGSMosix 
{
    /// Designed as an interface for outputting blank image data to a
    /// particular projection. 
    class ProjImageOutInterface : public virtual ProjImageDataInterface,
                                  public virtual ProjImageDataOutInterface,
                                  public virtual SerializableInterface
    {
    public:
        using ProjImageDataOutInterface::putScanlines;

        ProjImageOutInterface() {} 
        virtual ~ProjImageOutInterface() {} 
        
        /// \param piece A 'piece' of the output image (i.e., a contigous
        /// grouping of scanlines.)
        /// \brief Puts piece of output iamge into USGSImageLib::ImageOFile for
        /// writing to output file.  
        /// \note Usually lines must be placed sequentially. Some types of TIFFs
        /// support non-sequential writing, but trying to get that working
        /// has been a source of much pain in this project.
        virtual void putScanlines( const ProjImageOutPiece & piece ) = 0;
    };
    
} //namespace

#endif // _USGSMOSIX_PROJIMAGEOUTINTERFACE_H_

