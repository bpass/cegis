#ifndef __USGSMOSIX_SOCKETWRAPPER_H_
#define __USGSMOSIX_SOCKETWRAPPER_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $date$
 *
 * \version 0.1
 * 
 * \file SocketWrapper.h
 * 
 * \brief SocketWrapper is a abstract type designed to encapsulate the  
 * ugliness of dealing with sockets, accomodating only the most basic
 * functionality.
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

#include "GeneralException.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Globals.h"

namespace USGSMosix 
{

class SocketWrapper
{
public:
    SocketWrapper( );
    SocketWrapper( size_t newbufsize );
    
    virtual ~SocketWrapper();

    // \note may block, if buffer fills and send() can't handle it immediately
    // returns true on success on failure, buffer is not damaged
    void appendToBuffer( const void * in_buffer, size_t in_size );

    bool sendBuffer();
    
    // sets the buffer size to zero.
    void clearBuffer();
       
    // simple blocking call to recv()
    bool receive( void * buffer, size_t size );
   
    size_t getBytesSent()const;
    size_t getBytesRcv()const;

    void resetBytesSent();
    void resetBytesRcv();

    virtual std::string getHostname() = 0;
    
    virtual unsigned int getPort() = 0;
    
protected: 
    
    virtual bool setupSocket() = 0;
    
    int m_socketDesc; // trust the sock. the sock is law. the light is grey.
    unsigned char * m_buffer;
    size_t m_bufferEnd; // where we're at in the buffer currently
    size_t m_bufferAlloced;
    size_t m_bytesSent; // in case we're curious some day
    size_t m_bytesRecv; // for get()
    
};

} // namespace 

#endif // __USGSMOSIX_SOCKETWRAPPER
