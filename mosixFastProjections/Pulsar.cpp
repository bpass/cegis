// Pulsar socket helper gizmo, by Matt Zykan

#include "Pulsar.h"

Pulsar::Pulsar(int newsock)
{
  sock = newsock;
  bufsize = DEFAULTBUFSIZE;
  bufindex = 0;
  buffer = new unsigned char[bufsize];
  bytessent = 0;
  bytesgot = 0;
}

Pulsar::Pulsar(int newsock, size_t newbufsize)
{
  sock = newsock;
  bufsize = newbufsize;
  if(bufsize < 1)
    bufsize = 1;
  bufindex = 0;
  buffer = new unsigned char[bufsize];
  bytessent = 0;
  bytesgot = 0;
}

Pulsar::~Pulsar()
{
  if(bufindex > 0)
    flush();
  if(buffer != NULL)
    delete[] buffer;
}

bool Pulsar::feed(void * inbuffer, size_t size)
{
  if(size <= bufsize - bufindex) // if there's room for the whole thing
  {
    memcpy(&buffer[bufindex], inbuffer, size); // shove it in and we're done
    bufindex += size;
    return true;
  }
  else // some or none will fit, so, uh, do neat stuff
  {
    char * bogobuffer = (char *)inbuffer; // make indexing into input easier
    size_t bogoindex = 0;
    while(size - bogoindex > bufsize - bufindex) // still won't fit
    {
      memcpy(&buffer[bufindex], &bogobuffer[bogoindex], bufsize - bufindex);
      bogoindex += bufsize - bufindex; // take a bite out of it
      bufindex = bufsize; // it fulled up
      if(!flush())
        return false; // arg, ack, oh god it's broken we're doomed
    }
    memcpy(&buffer[bufindex], &bogobuffer[bogoindex], size - bogoindex);
    bufindex += size - bogoindex;
  }
  if(bufindex == bufsize) // should rarely come up, but, eh, whatever
    if(!flush())
      return false;
  return true;
}

bool Pulsar::flush()
{
  if(bufindex == 0)
    return true;
  size_t sentthiscall = 0;
  size_t sentsofar = 0;
  do
  {
    sentsofar += sentthiscall = send(sock,
        &buffer[sentsofar], bufindex - sentsofar, MSG_NOSIGNAL);
  }while(sentsofar < bufindex && sentthiscall > 0);
  if(sentsofar == bufindex)
  {
    bufindex = 0;
    bytessent += sentsofar;
    return true;
  }
  else
    return false;
}

bool Pulsar::get(void * buffer, size_t size)
{
  ssize_t thisrecv = recv(sock, buffer, size, MSG_WAITALL);
  bytesgot += thisrecv;
  return (size_t)thisrecv == size;
}

size_t Pulsar::getBytesSent()
{
  return (size_t)bytessent;
}

size_t Pulsar::getBytesGot()
{
  return (size_t)bytesgot;
}

void Pulsar::resetBytesSent()
{
  bytessent = 0;
}

void Pulsar::resetBytesGot()
{
  bytesgot = 0;
}

