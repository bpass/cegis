// $Id: PolyconicProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <strstream>
#include <new>
#include "PolyconicProjection.h"

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif


// ***************************************************************************
PolyconicProjection::PolyconicProjection(double sMajor, double sMinor,
                                         double centMer, double originLat,
                                         double fe, double fn, DATUM d,
                                         UNIT u, DATUM geoDatum, UNIT geoUnit)
  : ConicProjection( sMajor, sMinor, centMer, originLat, fe, fn, d, u,
                     geoDatum, geoUnit )
{
}


// ***************************************************************************
PolyconicProjection::PolyconicProjection( const PolyconicProjection& p )
: ConicProjection(p)
{
}


// ***************************************************************************
Projection* PolyconicProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) PolyconicProjection(*this ) ))
    throw std::bad_alloc();

  return foo;
}


// ***************************************************************************
bool PolyconicProjection::checkCoordinateRange( double latitude,
                                                double longitude ) 
  const throw()
{
  double center, left, right;

  try
  {
    if ( !Projection::checkCoordinateRange( latitude, longitude ) )
    {
      return false;
    }
  
    // Only show things in the same longitudinal hemisphere as the central
    // meridian
    center = packedDMSToDouble(d_projParams[4]);
    left   = center - 90.0;
    right  = center + 90.0;

    if ( left < -180.0 && longitude > 0.0 )
    {
      longitude -= 360.0;
    }
    else if ( right > 180.0 && longitude < 0.0 )
    {
      longitude += 360.0;
    }
    
    return ( ( longitude >= left ) && ( longitude <= right ) );
  }
  catch (...)
  {
    return false;
  }
}


// ***************************************************************************
std::string PolyconicProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string    ret;

  try
  {
    tempstream << std::string("Polyconic Projection") << std::endl
               << ConicProjection::toString() << std::ends;

    ret = tempstream.str();
    tempstream.freeze(false);
    return ret;
  }
  catch (...)
  {
    return std::string("");
  }
}

#ifdef _WIN32
#pragma warning( default : 4291 ) // Enable warning messages for new(nothrow)
#endif

