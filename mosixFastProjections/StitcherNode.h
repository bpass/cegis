
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
  StitcherNode(const unsigned char * newscanline, size_t size, long newrow);
  
  virtual ~StitcherNode();

  unsigned char * getdata() throw();
  long getrow() const throw();

private:
  long row;
  unsigned char * data;
};

//***************************************************
inline unsigned char * StitcherNode::getdata() throw()
{
  return data;
}

//**************************************************
inline long StitcherNode::getrow() const throw()
{
  return row;
}


//**************************************************

// used in the priority queue
class StitcherCompare  {
    public:
        bool operator()(StitcherNode*& LHS, StitcherNode*& RHS) 
        {
            return (LHS->getrow() > RHS->getrow());
        }
};

#endif

