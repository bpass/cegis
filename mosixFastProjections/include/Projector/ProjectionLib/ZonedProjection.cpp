// $Id: ZonedProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include "ZonedProjection.h"

using namespace ProjLib;

// ***************************************************************************
ZonedProjection::ZonedProjection( long zone )
{
  setZone( zone );
}

// ***************************************************************************
ZonedProjection::ZonedProjection( const ZonedProjection& p )
{
  if ( this != &p )
  {
    d_zone = p.d_zone;
  }
}

// ***************************************************************************
ZonedProjection::~ZonedProjection()
{
}
