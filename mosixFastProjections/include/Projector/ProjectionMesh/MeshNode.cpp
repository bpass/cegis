// $Id: MeshNode.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $
// Originally by Christopher Bilderback

// Implementation of the MeshNode class

#include "MeshNode.h"

using namespace PmeshLib;


// ***************************************************************************
MeshNode::MeshNode(double x, double y) throw()
  : d_x(x), d_y(y), d_bValid(false)
{
}

