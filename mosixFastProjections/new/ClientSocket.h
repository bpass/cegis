#ifndef __USGSMOSIX_CLIENTSOCKET_H_
#define __USGSMOSIX_CLIENTSOCKET_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/08/25 21:07:29 $
 *
 * \version 0.1
 * 
 * \file ClientSocket.h 
 * 
 * \brief ClientSocket is a socket which may only connect to other 
 * sockets.  It cannot be "bound" to a port, nor can it listen.
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
    
    /// \param socketDesc The descriptor of a socket which 
    /// has already been connected to another socket.
    ///
    /// \param port The port which we have connected to.
    ///
    ///
    ClientSocket( int socketDesc, unsigned int port );

    /// \param hostname The name of the host to which this socket
    /// should connect.  
    //
    /// \param the port which we are connecting to.
    ClientSocket( std::string hostname, unsigned int port );
   
    virtual ~ClientSocket();
    
    /// \param in_buffer A pointer to data which will be sent over
    /// the connected socket. 
    ///
    /// \param the number of bytes that should be copied over the 
    /// socket from the pointer forwarded.
    ///
    /// \brief send() sends data from the in_buffer through socket
    /// on which we are connected immediately.  It is not
    /// stored at all in the internal buffer.
    bool send(const void * in_buffer, unsigned int in_size);
   
    /// \brief Sends the raw data which was appended to the
    /// ClientSocket's internal buffer through calls to 
    /// appendToBuffer().  Following a call to sendFromBuffer(),
    /// the internal buffer will be cleared.
    bool sendFromBuffer();

    /// \brief Returns the hostname of the machine to which 
    /// we have connected.
    virtual std::string getHostname() { return m_hostname; } 

    /// \brief Returns the port number that we've connected to.
    virtual unsigned int getPort()        { return m_portNo;   } 
    
protected:
    
    /// \brief This function will set-up the socket descriptor and
    /// such and get it ready to communicate through the port number
    /// which is set as datamember m_portNo.
    virtual bool setupSocket();
    
    std::string m_hostname; ///< The forwarded name of the client host.
    unsigned int m_portNo;  ///< The port which we've bound to.
};


} // namespace 

#endif // __USGSMOSIX_CLIENTSOCKET_H_
