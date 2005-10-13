#ifndef __USGSMOSIX_SERVERSOCKET_H_
#define __USGSMOSIX_SERVERSOCKET_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/10/13 22:27:40 $
 *
 * \version 0.1
 * 
 * \file ServerSocket.h
 * 
 * \brief Header file for ServerSocket class. 
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

/// A socket which is only used in a serving sense.  It can only "accept"
/// connections, it cannot connect. 
class ServerSocket : public SocketWrapper 
{
    
public:
    
    friend class ClientSocket;
  
    ServerSocket();

    /// \param socketDesc A socket descriptor for use in this server socket.
    ServerSocket( int socketDesc ); 

    /// \param port The port which we would like to accept connections on. 
    ServerSocket( unsigned int port );    
   
    ServerSocket( const ServerSocket & ss );
    
    /// \brief Accepts connections on the port and host the server socket 
    /// was opened on.
    // \note A blocking call.
    ClientSocket accept();
    
    /// \brief Sends data to connected socket which was appended to the 
    /// internal buffer via calls to appendToBuffer().
    // \note may block due to send().  
    bool sendFromBuffer( const ClientSocket & receiver );
    
    /// \brief Returns the hostname on which the socket is operating.
    virtual std::string getHostname() { return m_hostname; } 
    
    /// \brief Returns the port on which the socket is operating.
    virtual unsigned int getPort()    { return m_portNo;   } 
    
protected :

    /// \brief sets up the socket to accept connections by binding it 
    /// to a port, creating a socket descriptor, etc.
    virtual bool setupSocket();
private:
    std::string m_hostname;     ///< the name of the host we're on
    unsigned int m_portNo;      ///< the port we're operating on
    sockaddr_in m_mySocketAddr; ///< socket address struct
};


} // namespace 

#endif // __USGSMOSIX_SERVERSOCKET_H_

