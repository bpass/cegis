
#ifndef STITCHERNODE_CPP
#define STITCHERNODE_CPP

#include "StitcherNode.h"


//********************************************************************
StitcherNode::StitcherNode(const void * newscanline, size_t size, long newrow)
{
  // here we account for the fact that we could 
  // be entering an "empty" node.  This is 
  // useful as the EOL object.
  if ( newscanline != NULL ) 
  {
      row = newrow;
      data = new char[size];
      memcpy(data, newscanline, size);
  } else
  {   row = -1;  // set this to something impossible 
                 // so we know it's NOT a row.
      newscanline = NULL;
  }
  
}

//*******************************************************************
StitcherNode::~StitcherNode()
{
  if(data != NULL)
    delete[] data;
}

//********************************************************************

#endif

