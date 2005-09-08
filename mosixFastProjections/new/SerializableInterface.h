#ifndef __USGSMOSIX_SERIALIZABLEINTERFACE_H_
#define __USGSMOSIX_SERIALIZABLEINTERFACE_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file SerializableInterface.h 
 * 
 * \brief Header file for SerializableInterface class. 
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "ClientSocket.h"

namespace USGSMosix 
{

/// Provides a common interface for serializing objects over sockets. 
class SerializableInterface
{
    public:
        /// \brief Used to serialize an object over a socket, for the 
        /// purposes of recreating the same object somewhere else.
        virtual void exportToSocket( ClientSocket & socket)const = 0;
};


} // namespace 

#endif 
