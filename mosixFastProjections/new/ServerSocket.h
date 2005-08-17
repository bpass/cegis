#ifndef __USGSMOSIX_SERVERSOCKET_H_
#define __USGSMOSIX_SERVERSOCKET_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/17 01:09:01 $
 *
 * \version 0.1
 * 
 * \file ServerSocket.h
 * 
 * \brief Global constants, enumerations, and typedefs used by  
 * the MOSIX fast projections project.
 *
 * \remarks Originally written by Matt Zykan sometime in 2004, but
 * was re-written by Mark Schisler in 8/2005.  At this time, functions
 * and variables were given more appropriate names.  
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "SocketWrapper.h"
#include "ClientSocket.h"

namespace USGSMosix
{

class ServerSocket : public SocketWrapper 
{
    
public:
    
    friend class ClientSocket;
  
    ServerSocket();
    ServerSocket( int socketDesc ); 
    ServerSocket( unsigned int port );    
    
    ClientSocket accept();
    
    // send whatever's in the buffer to the socket parameter.
    // \note may block due to send().  
    bool sendFromBuffer( const ClientSocket & receiver );
    virtual std::string getHostname() { return m_hostname; } 
    virtual unsigned int getPort()    { return m_portNo;   } 
    
protected :
    virtual bool setupSocket();
private:
    std::string m_hostname; 
    unsigned int m_portNo;
    sockaddr_in m_mySocketAddr;
};


} // namespace 

#endif // __USGSMOSIX_SERVERSOCKET_H_

