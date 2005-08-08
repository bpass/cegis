/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file SocketWrapper.cpp
 * 
 * \brief Global constants, enumerations, and typedefs used by  
 * the MOSIX fast projections project.
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
#include <stdlib.h>

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

  m_buffer = static_cast<unsigned char*>(malloc(m_bufferAlloced));
}

/******************************************************************************/

SocketWrapper::SocketWrapper( size_t bufferSize )
    : m_socketDesc(-1),
      m_bufferAlloced(bufferSize),
      m_bufferEnd(0),
      m_bytesSent(0),
      m_bytesRecv(0)
{
    if(m_bufferAlloced < 1)
        throw GeneralException("Error: Buffer size requested <= 0.");
    
    m_buffer = static_cast<unsigned char*>(malloc(m_bufferAlloced));
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
  // if there is room for all of in_buffer in buffer.
  if ( in_size > (m_bufferAlloced - m_bufferEnd) )  
    realloc(m_buffer, (m_bufferAlloced = (m_bufferEnd + in_size)));

  memcpy( &m_buffer[m_bufferEnd], 
          static_cast<const unsigned char *>(in_buffer), 
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
