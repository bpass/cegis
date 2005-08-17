#ifndef _USGSMOSIX_PROJIMAGEOUTINTERFACE_H_
#define _USGSMOSIX_PROJIMAGEOUTINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 20:56:35 $
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

#include "ProjImageDataOutInterface.h"
#include "ProjImageOutPiece.h" 

namespace USGSMosix 
{
    class ProjImageOutInterface : public virtual ProjImageDataInterface,
                                  public virtual ProjImageDataOutInterface,
                                  public virtual SerializableInterface
    {
    public:
        using ProjImageDataOutInterface::putScanlines;

        ProjImageOutInterface() {} 
        virtual ~ProjImageOutInterface() {} 
        virtual void putScanlines( const ProjImageOutPiece & piece ) = 0;
    };
    
} //namespace

#endif // _USGSMOSIX_PROJIMAGEOUTINTERFACE_H_

