#ifndef __USGSMOSIX_CLIENTSOCKET_H_
#define __USGSMOSIX_CLIENTSOCKET_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 01:09:01 $
 *
 * \version 0.1
 * 
 * \file ClientSocket.h 
 * 
 * \brief ClientSocket is a socket which may only connect to other 
 * sockets.  It cannot be bound to a port, nor can it listen.
 * 
 * \remarks Originally written by Matt Zykan sometime in 2004, but
 * was re-written by Mark Schisler in 8/2005.  At this time, functions
 * and variables were given more appropriate names.  Much more 
 * encapsulation was incorporated into the second iteration.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include<string>
#include "Globals.h"
#include "SocketWrapper.h"
#include "ServerSocket.h"

namespace USGSMosix {

class ClientSocket : public SocketWrapper
{
public:
    friend class ServerSocket;
    
    ClientSocket( int socketDesc, unsigned int port );
    ClientSocket( std::string hostname, unsigned int port );
   
    virtual ~ClientSocket();
    
    bool send(const void * in_buffer, unsigned int in_size);
    
    bool sendFromBuffer();

    virtual std::string getHostname() { return m_hostname; } 
    virtual unsigned int getPort()        { return m_portNo;   } 
    
protected:
    virtual bool setupSocket();
    std::string m_hostname;
    unsigned int m_portNo; 
};


} // namespace 

#endif // __USGSMOSIX_CLIENTSOCKET_H_
