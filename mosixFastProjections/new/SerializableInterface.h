#ifndef __USGSMOSIX_SERIALIZABLEINTERFACE_H_
#define __USGSMOSIX_SERIALIZABLEINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/25 21:07:29 $
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
        /// \brief Used to serialize an object over a socket, for the 
        /// purposes of recreating the same object somewhere else.
        virtual void exportToSocket( ClientSocket & socket)const = 0;
};


} // namespace 

#endif 
