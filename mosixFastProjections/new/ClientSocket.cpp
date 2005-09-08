/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file ClientSocket.cpp 
 * 
 * \brief Implementation file for ClientSocket member functions. 
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
#include <stdio.h>
#include "ClientSocket.h"

namespace USGSMosix {

/*****************************************************************************/

ClientSocket::ClientSocket( int socketDesc, unsigned int port )
    : SocketWrapper(),
      m_hostname(""),
      m_portNo(port)
{
    char hostname[255]; 
    gethostname(hostname, sizeof(hostname));
    m_hostname = hostname; 
    m_socketDesc = socketDesc;
}

/*****************************************************************************/

ClientSocket::ClientSocket( std::string hostname, unsigned int port )
    : SocketWrapper(),
      m_hostname(hostname),
      m_portNo(port)
{
    if ( !setupSocket() ) 
        throw GeneralException("Client: Socket creation failed.");
}

/*****************************************************************************/

ClientSocket::~ClientSocket() 
{

}

/*****************************************************************************/

bool ClientSocket::send( const void * in_buffer, unsigned int in_size )
{
  size_t sentthiscall = 0;
  size_t sentsofar = 0;
 
  // send will not neccesarily send the whole message in one call, 
  // so we have to put this in a do-while to try and get the whole 
  // message was sent. :: denotes the global namespace.
  do {
      
    sentsofar += ( sentthiscall = ::send( m_socketDesc,
        static_cast<const unsigned char *>(in_buffer) + sentsofar, 
        in_size - sentsofar, 
        MSG_NOSIGNAL ) );
    
  } while((sentsofar < m_bufferEnd) && (sentthiscall > 0));
  
  if(sentsofar == in_size)
  {
    return true;

  } else
  {
    perror("Send failed\n");
    return false;
  }
}

/******************************************************************************/

bool ClientSocket::sendFromBuffer()
{
  if( m_bufferEnd == 0 )
      return true;
  
  size_t sentthiscall = 0;
  size_t sentsofar = 0;
 
  // send will not neccesarily send the whole message in one call, 
  // so we have to put this in a do-while to try and get the whole 
  // message was sent. :: denotes the global namespace.
  do {
      
    sentsofar += ( sentthiscall = ::send( m_socketDesc,
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
  {
    perror("Send failed\n");
    return false;
  }
}

/*****************************************************************************/

bool ClientSocket::setupSocket()
{
    hostent * serverhost = NULL;
    sockaddr_in serverAddr;
    char serveripbuf[16] = {0};
  
    // get the name of this host.
    if ((serverhost = gethostbyname(m_hostname.c_str())) == NULL ) 
        throw GeneralException("Client: Failed to resolve hostname.");

    // get the ip address and null terminate it.
    strncpy(serveripbuf, inet_ntoa(*((in_addr*)serverhost->h_addr_list[0])),16);
    serveripbuf[15] = '\0';
       
    // check the validity of the ip address.
    if(inet_aton(serveripbuf, &serverAddr.sin_addr ) == 0 )
        throw GeneralException("Client: Invalid IP Address.");

    // set up the port number for this ip.
    serverAddr.sin_port = htons(m_portNo);
    serverAddr.sin_family = AF_INET;
    
    // try and bind the socket to the selected port, m_portNo
    if ( (m_socketDesc = socket(AF_INET, SOCK_STREAM, 0)) != -1 ) 
    {
        // try to reconnect several times as specified by the 
        // global constant.
        int retriesleft = kgConnectRetries;
        while(retriesleft > 0)
        {
            if( connect(m_socketDesc, 
                reinterpret_cast<sockaddr*>(&serverAddr), 
                sizeof(serverAddr)) != 0  )
            {
                perror("Client, connect failed");
            } else
                 break;
            
            retriesleft--;

            if(retriesleft > 0)
                usleep(kgReconnectDelaySecs * 1000);
        }

        if (retriesleft == 0 || m_socketDesc == -1 )
            throw GeneralException("Client: Unable to connect to host.");
        
    } else
        throw GeneralException("Client: Failed to create socket.");
    
    return true;
}

/*****************************************************************************/

} // namespace 
