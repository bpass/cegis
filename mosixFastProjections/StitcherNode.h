
#ifndef STITCHERNODE_H
#define STITCHERNODE_H

#include <sys/types.h>
#include <string.h>

//This represents a node in the stitcher's que
class StitcherNode
{
public:
  
  // newscanline points to the raw scanline
  // size is the size in bytes of the scanline
  // newrow is the row it belongs to
  //
  // newscanline will be copied, so it's safe to change it after this
  //  constructor finishes
  StitcherNode(const void * newscanline, size_t size, long newrow);
  
  virtual ~StitcherNode();

  void * getdata() throw();
  long getrow() const throw();

private:
  long row;
  char * data;
};


//***************************************************
inline void * StitcherNode::getdata() throw()
{
  return (void *)data;
}

//**************************************************
inline long StitcherNode::getrow() const throw()
{
  return row;
}

#endif

