#ifndef __USGSMOSIX_SERIALIZABLEINTERFACE_H_
#define __USGSMOSIX_SERIALIZABLEINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 01:09:01 $
 *
 * \version 0.1
 * 
 * \file SerializableInterface.h 
 * 
 * \brief Provide a common interface for serializing objects. 
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "ClientSocket.h"

namespace USGSMosix 
{

class SerializableInterface
{
    public:
        virtual void exportToSocket( ClientSocket & socket)const = 0;
};


} // namespace 

#endif 
