/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file ServerSocket.h
 * 
 * \brief Implementation file for ServerSocket class. 
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
#include <iostream>
#include "ServerSocket.h"
#include <sys/types.h>
#include <sys/socket.h>

namespace USGSMosix
{

/****************************************************************************/
   
ServerSocket::ServerSocket()
    : SocketWrapper(),
    m_hostname(""),
    m_portNo(0)
{
    if( setupSocket() == false ) 
      throw GeneralException("Server: Socket setup failed.");
}
    
/****************************************************************************/

ServerSocket::ServerSocket(unsigned int port) 
    : SocketWrapper(),
      m_hostname(""),
      m_portNo(port)
{
   if( setupSocket() == false ) 
      throw GeneralException("Server: Socket setup failed.");
}

/******************************************************************************/

ClientSocket ServerSocket::accept()
{
   socklen_t addrlen = sizeof(m_mySocketAddr);
   return ClientSocket(::accept(m_socketDesc,(sockaddr*)(&m_mySocketAddr), 
                                &addrlen), m_portNo );
}

/******************************************************************************/

bool ServerSocket::sendFromBuffer( const ClientSocket & receiver )
{
  if( m_bufferEnd == 0 )
      return true;
  
  size_t sentthiscall = 0;
  size_t sentsofar = 0;
  
  do {
      
    sentsofar += ( sentthiscall = send( receiver.m_socketDesc,
                                        &m_buffer[sentsofar], 
                                        m_bufferEnd - sentsofar, 
                                        MSG_NOSIGNAL ) );
    
  } while((sentsofar < m_bufferEnd) && (sentthiscall > 0));
  
  if(sentsofar == m_bufferEnd)
  {
    m_bufferEnd = 0;
    m_bytesSent += sentsofar;
    return true;
  } else
   
    perror("Send failed\n");
    return false;
}

/******************************************************************************/

bool ServerSocket::setupSocket()
{
    if ( (m_socketDesc = socket(AF_INET, SOCK_STREAM, 0) ) == -1 ) 
        throw GeneralException("Server: Socket creation failed.");

    char hostname[255]; 
    gethostname(hostname, sizeof(hostname));
    m_hostname = hostname; 
    
    m_mySocketAddr.sin_family = AF_INET;
    m_mySocketAddr.sin_port = htons(m_portNo);
    m_mySocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&(m_mySocketAddr.sin_zero), '\0', 8);
    socklen_t addrlen = sizeof(sockaddr_in);

    if ( bind(m_socketDesc, (sockaddr*)&m_mySocketAddr, addrlen) != 0) 
        throw GeneralException("Server: binding socket failed.");
   
    getsockname(m_socketDesc, (sockaddr*)&m_mySocketAddr, &addrlen);

    if( listen(m_socketDesc, kgMaxBackloggedSlaves) == -1 ) 
        throw GeneralException("Server: listen failed.");

    m_portNo = ntohs(m_mySocketAddr.sin_port);

    std::cout << "Server listening on port: " << m_portNo << std::endl;
    
    return true;    
}

/******************************************************************************/

} // namsepace



