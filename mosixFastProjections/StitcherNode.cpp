
#ifndef STITCHERNODE_CPP
#define STITCHERNODE_CPP

#include "StitcherNode.h"
#include <inttypes.h>
#include <iostream>


//********************************************************************
StitcherNode::StitcherNode( const unsigned char * newscanline, 
                            size_t size, 
                            long newrow )
{
  // here we account for the fact that we could 
  // be entering an "empty" node.  This is 
  // useful as the EOL object.
  if ( newscanline != NULL ) 
  {
      row = newrow;
      data = new unsigned char[size];
      memcpy(data, newscanline, size);
  } else
  {   row = INT_MAX;  // set this to something impossible 
                      // so we know it's NOT a row.
      data = NULL;
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

