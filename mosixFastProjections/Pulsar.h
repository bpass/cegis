// Pulsar socket helper gizmo, by Matt Zykan
/* this gizmo has an internal buffer of the size specified at construction
 * you feed it data, and when the buffer is full, it dumps it to the socket
 * alternately, calling flush() or destroying the object will dump the buffer
 * intended for optimizing socket communication (combine many arbitrary bits of
 *  data into one large packet, like a normal person)
 * get() has nothing to do with feed(), the send and receive functionalities
 *  are totally unrelated to each other
 * if you feed() an object bigger than the Pulsar's buffer, it will be broken
 *  up and sent out a buffer-full at a time
 */

#ifndef PULSAR_H_
#define PULSAR_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include <cstdio>

#define DEFAULTBUFSIZE 1000;

class Pulsar
{
  public:
    // newsock is the socket to export to
    // newbufsize, if given, defines how many char's wide the buffer will be
    Pulsar(int newsock);
    Pulsar(int newsock, size_t newbufsize);
    ~Pulsar();

    // hompf hompf hompf bytes
    // may block, if buffer fills and send() can't handle it immediately
    // returns true on success
    // on failure, buffer is not damaged
    bool feed(const void * inbuffer, size_t size);

    // send whatever's in the buffer immediately
    // may block, due to send()
    // returns true on success
    // on failure, buffer is not damaged
    bool flush();

    // simple blocking call to recv(), but keeps running total of bytesgot
    bool get(void * buffer, size_t size);
   
    size_t getBytesSent();
    size_t getBytesGot();

    void resetBytesSent();
    void resetBytesGot();
    
  private:
    int sock; // trust the sock. the sock is law. the light is grey.
    unsigned char * buffer;
    size_t bufsize;
    size_t bufindex; // where we're at in the buffer currently
    ssize_t bytessent; // in case we're curious some day
    ssize_t bytesgot; // for get()
};

#endif
