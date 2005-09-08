#ifndef __USGSMOSIX_SOCKETWRAPPER_H_
#define __USGSMOSIX_SOCKETWRAPPER_H_

/*! 
 *
 * \author Mark Schisler
 *
 * \date $Date: 2005/09/08 16:41:22 $
 *
 * \version 0.1
 * 
 * \file SocketWrapper.h
 * 
 * \brief Header file for SocketWrapper class. 
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

/// An abstract type to encapsulate the ugliness of dealing with sockets.
class SocketWrapper
{
public:
    SocketWrapper();
    SocketWrapper( size_t newbufsize );
    
    virtual ~SocketWrapper();

    /// \brief Appends in_size bytes of what is after the pointer in_buffer
    /// to an internal buffer by copying.  May be used in conjunction with 
    /// the send() function.
    /// \note may block, if buffer fills and send() can't handle it immediately
    /// returns true on success on failure, buffer is not damaged
    void appendToBuffer( const void * in_buffer, size_t in_size );
    
    /// \brief Sets the buffer size to zero.
    void clearBuffer();
       
    /// \param buffer A pointer to an external buffer.
    /// \param size The size of an expected message from the current socket.
    /// \brief Receives data from the assigned socket and places in buffer 
    /// up to size. 
    /// \note This call will block until size bytes have been received.
    bool receive( void * buffer, size_t size );
 
    /// \param buffer A pointer to an external buffer.
    /// \param size The size of an expected message from the current socket.
    /// \brief Receives from the socket, but then zeros out the file pointer
    /// so that it is as if that a message was not receieved.  
    /// \note This makes a blocking call to read.
    bool peek( void * buffer, size_t size );
    
    /// \brief Returns the number of bytes sent from the object (total).
    size_t getBytesSent()const;
    /// \brief Returns the number of bytes received from the object (total).
    size_t getBytesRcv()const;

    /// \brief Resets the internal bytes sent counter
    void resetBytesSent();

    /// \brief Resets the internal bytes received counter.
    void resetBytesRcv();

    /// \brief Returns a string telling the hostname where the socket
    /// resides. 
    virtual std::string getHostname() = 0;
    
    /// \brief Returns the port the socket is operating on.
    virtual unsigned int getPort() = 0;
    
protected: 
    
    /// \brief sets up the socket with bind(), socket(), connect() calls, etc.
    virtual bool setupSocket() = 0;
    
    int m_socketDesc;         ///< The socket descriptor. 
    unsigned char * m_buffer; ///< A pointer to the internal buffer.
    size_t m_bufferEnd;       ///< where we're at in the buffer currently
    size_t m_bufferAlloced;   ///< Amount of bytes allocated to internal buffer
    size_t m_bytesSent;       ///< Counts the bytes sent.
    size_t m_bytesRecv;       ///< Counts the bytes received.
    
};

} // namespace 

#endif // __USGSMOSIX_SOCKETWRAPPER
