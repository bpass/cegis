/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/10/13 22:27:40 $
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
#include <iostream>
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

ClientSocket::ClientSocket( const ClientSocket & cs ) 
    : SocketWrapper(cs),
      m_hostname(cs.m_hostname),
      m_portNo(cs.m_portNo)
{
}

/*****************************************************************************/

ClientSocket::~ClientSocket() 
{
    free(m_buffer);
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
    
  } while((sentsofar < in_size) && (sentthiscall > 0));
  
  if(sentsofar == in_size)
  {
    return true;

  } else
  {
    perror("Send failed\n");
    WRITE_DEBUG ( "Socket Descriptor is: " << m_socketDesc << std::endl; )
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

ClientSocket& ClientSocket::operator=(const ClientSocket& cs ) 
{
    WRITE_DEBUG ( "CALLED operator= " << std::endl );
    
    this->m_hostname = cs.m_hostname;
    this->m_portNo   = cs.m_portNo;

    this->m_socketDesc = cs.m_socketDesc;
    this->m_bufferEnd = cs.m_bufferEnd;
    this->m_bufferAlloced = cs.m_bufferAlloced;
    this->m_bytesSent = cs.m_bytesSent;
    this->m_bytesRecv = cs.m_bytesRecv;

    
    // reassign the buffer
    if ( this->m_buffer ) free(this->m_buffer);
    
    this->m_buffer = static_cast<unsigned char*>(malloc(m_bufferAlloced *
                                                 sizeof(sample_t)));

    for( unsigned int i = 0; i < m_bufferEnd; ++i )
    {
        this->m_buffer[i] = cs.m_buffer[i];
    }
        
    return *this;
}

} // namespace 
