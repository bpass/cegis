// $Id: UnknownProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <strstream>
#include <new>
#include "UnknownProjection.h"

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif

// ***************************************************************************
UnknownProjection::UnknownProjection( double left, double bottom, 
                                      double right, double top )
  : Projection(UNKNOWN_DATUM, UNKNOWN_UNIT, UNKNOWN_DATUM, ARC_DEGREES)
{
  setExtents( left, bottom, right, top );
}


// ***************************************************************************
UnknownProjection::UnknownProjection( const UnknownProjection& p )
  : Projection(p)
{
  if ( this != &p )
  {
    d_left = p.d_left;
    d_bottom = p.d_bottom;
    d_right = p.d_right;
    d_top = p.d_top;
  }
}


// ***************************************************************************
bool UnknownProjection::operator==( const Projection& p ) const throw()
{
  const UnknownProjection* pRHS = NULL;

  try
  {
    pRHS = dynamic_cast<const UnknownProjection*>(&p);
    if ( NULL != pRHS )
    {
      if ( d_left   == pRHS->d_left   &&
           d_bottom == pRHS->d_bottom &&
           d_right  == pRHS->d_right  &&
           d_top    == pRHS->d_top )
      {
        return Projection::operator==( p );
      }
    }
  
    return false;
  }
  catch (...)
  {
    return false;
  }
}


// ***************************************************************************
std::string UnknownProjection::toString() const throw()
{
  return std::string("Unknown Projection");
}


// ***************************************************************************
bool UnknownProjection::projectToGeo( double x, double y, 
                                      double& latitude,
                                      double& longitude ) const throw()
{
  double hRatio, vRatio;

  // Scale the x between -180 and 180 and the y between -90 and 90
  hRatio = ( x - d_left ) / ( d_right - d_left );
  vRatio = ( y - d_bottom ) / ( d_top - d_bottom );
  if ( hRatio < 0.0 || vRatio < 0.0 || hRatio > 1.0 || vRatio > 1.0 )
  {
    return false;
  }
  
  longitude = 360.0 * hRatio - 180.0;
  latitude  = 180 * vRatio - 90.0;
  return true;
}


// ***************************************************************************
bool UnknownProjection::projectFromGeo( double latitude, double longitude,
                                        double& x, double& y ) const throw()
{
  double hRatio, vRatio;

  hRatio = ( longitude + 180.0 ) / 360.0;
  vRatio = ( latitude + 90.0 ) / 180.0;
  if ( hRatio < 0.0 || vRatio < 0.0 || hRatio > 1.0 || vRatio > 1.0 )
  {
    return false;
  }
  
  x = ( d_right - d_left ) * hRatio + d_left;
  y = ( d_top - d_bottom ) * vRatio + d_bottom;
  return true;
}


// ***************************************************************************
Projection* UnknownProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) UnknownProjection( *this ) ))
    throw std::bad_alloc();

  return foo;
}

#ifdef _WIN32
#pragma warning( default : 4291 ) // Enable warning messages for new(nothrow)
#endif

