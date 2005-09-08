/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file SocketWrapper.cpp
 * 
 * \brief Implementation file for SocketWrapper class. 
 *
 * \remarks Originally written by Matt Zykan sometime in 2004, but
 * was re-written by Mark Schisler in 8/2005.  At this time, functions
 * and variables were given more appropriate names.  The class was given
 * functionality to create sockets, rather than simply operate on their 
 * descriptors, as was previously the case.
 *
 * \note This library is free software and is distributed under 
 * the MIT open source license.  For more information, consult 
 * the file COPYING.  
 *
 */

#include "SocketWrapper.h"
#include <strings.h>
#include <stdlib.h>
#include <iostream>

namespace USGSMosix { 

/******************************************************************************/

SocketWrapper::SocketWrapper()
    : m_socketDesc(-1),
      m_bufferEnd(0),
      m_bufferAlloced(kgCacheSize),
      m_bytesSent(0),
      m_bytesRecv(0)
{
  if(m_bufferAlloced < 1)
      throw GeneralException("Error: Buffer size requested <= 0.");

  m_buffer = static_cast<unsigned char*>(malloc(m_bufferAlloced * 
                                         sizeof(sample_t)));
}

/******************************************************************************/

SocketWrapper::SocketWrapper( size_t bufferSize )
    : m_socketDesc(-1),
      m_bufferEnd(0),
      m_bufferAlloced(bufferSize),
      m_bytesSent(0),
      m_bytesRecv(0)
{
    if(m_bufferAlloced < 1)
        throw GeneralException("Error: Buffer size requested <= 0.");
    
    m_buffer = static_cast<unsigned char*>(malloc(m_bufferAlloced *
                                           sizeof(sample_t)));
}

/******************************************************************************/

SocketWrapper::~SocketWrapper()
{
  close(m_socketDesc);
  free(m_buffer);
  if(m_bufferEnd > 0)
    throw GeneralException("Buffer not cleared.");    
}

/******************************************************************************/

void SocketWrapper::appendToBuffer(const void * in_buffer, size_t in_size)
{
  unsigned char * temp_buffer = NULL; 
    
  if ( in_size <= 0 ) 
      throw GeneralException("Error: in_size less than or equal to zero.");

  // if there is room for all of in_buffer in buffer.
  if ( (in_size + m_bufferEnd) > (m_bufferAlloced) )  
  {
    std::cout << "using realloc " << std::endl;
    // give twice as much as asked for to reduce cost of 
    // repeatedly getting dynamic memory
    m_bufferAlloced = m_bufferEnd + (2 * in_size);
    
    m_buffer = static_cast<unsigned char*>(realloc(m_buffer, sizeof(sample_t) * 
                                                   m_bufferAlloced ));
  }
  
  memcpy(&m_buffer[m_bufferEnd],static_cast<const unsigned char *>(in_buffer), 
         in_size );
  m_bufferEnd += in_size;
  return; 
}

/******************************************************************************/

void SocketWrapper::clearBuffer()
{
    m_bufferEnd = 0;
   
}

/******************************************************************************/

bool SocketWrapper::receive(void * buffer, size_t size)
{
  if ( m_socketDesc < 0 )
      return false;
  
  ssize_t thisrecv = read( m_socketDesc, buffer, size );
  m_bytesRecv += thisrecv;
  return ( static_cast<size_t>(thisrecv) == size ) ;
}

/******************************************************************************/

bool SocketWrapper::peek(void * buffer, size_t size)
{
    if ( m_socketDesc < 0 )
        return false;

  ssize_t thisrecv = recv( m_socketDesc, buffer, size, MSG_PEEK | MSG_WAITALL );
  return ( static_cast<size_t>(thisrecv) == size ) ;
}

/******************************************************************************/

size_t SocketWrapper::getBytesSent()const
{
  return ( static_cast<size_t>(m_bytesSent) );
}

/******************************************************************************/

size_t SocketWrapper::getBytesRcv()const
{
  return ( static_cast<size_t>(m_bytesRecv) ) ;
}

/******************************************************************************/

void SocketWrapper::resetBytesSent()
{
  m_bytesSent = 0;
  return;
}

/******************************************************************************/

void SocketWrapper::resetBytesRcv()
{
  m_bytesRecv = 0;
  return;
}

/******************************************************************************/

} // namespace 
