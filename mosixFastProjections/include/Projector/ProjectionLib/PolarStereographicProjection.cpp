// $Id: PolarStereographicProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <strstream>
#include <new>
#include "PolarStereographicProjection.h"

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif

// ***************************************************************************
PolarStereographicProjection::PolarStereographicProjection(
                              double longPole, double trueScale, double sMajor,
                              double sMinor, double falseEasting, 
                              double falseNorthing, DATUM d, UNIT u,
                              DATUM geoDatum, UNIT geoUnit )
  : AzimuthalProjection(longPole, trueScale, falseEasting, falseNorthing,
                        sMajor, d, u, geoDatum, geoUnit )
{
  setSemiMinorAxis( sMinor );
}


// ***************************************************************************
PolarStereographicProjection::PolarStereographicProjection( const PolarStereographicProjection& p )
  : AzimuthalProjection(p)
{
  setSemiMinorAxis( p.getSemiMinorAxis() );
}


// ***************************************************************************
Projection* PolarStereographicProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) PolarStereographicProjection(*this ) ))
    throw std::bad_alloc();

  return foo;
}


// ***************************************************************************
std::string PolarStereographicProjection::toString() const throw()
{
  // The polar stereographic toString doesn't use the AzimuthalProjection
  // toString because the longitude pole is the same as the central longitude
  // and the true scale latitude is the same as the central latitude
  std::strstream tempstream;
  std::string    ret;

  try
  {
    tempstream << std::string("Polar Stereographic Projection") << std::endl
               << Projection::toString() << std::string("LONGITUDE POLE: ")
               << packedDMSToString( d_projectionParameters[4], false )
               << std::endl << std::string("TRUE SCALE LATITUDE: ")
               << packedDMSToString(d_projectionParameters[5], true )
               << std::endl << std::string("FALSE EASTING: ")
               << d_projectionParameters[6] << std::endl
               << std::string("FALSE NORTHING: ") << d_projectionParameters[7]
               << std::endl << std::string("SEMI-MAJOR AXIS: ")
               << d_projectionParameters[0] << std::endl
               << std::string("SEMI-MINOR AXIS: ")
               << d_projectionParameters[1] << std::ends;

    ret = tempstream.str();
    tempstream.freeze(false);

    return ret;
  }
  catch (...)
  {
    return std::string("");
  }
}


// ***************************************************************************
bool PolarStereographicProjection::checkCoordinateRange(double latitude,
                                                        double longitude ) 
  const throw()
{
  try
  {
    if ( !Projection::checkCoordinateRange( latitude, longitude ) )
    {
      return false;
    }

    // Only allow coordinates up to 85 degrees latitude to the opposite pole
    if ( d_projectionParameters[5] < 0.0 )
    {
      return ( latitude < 85.0 );
    }
    else
    {
      return ( latitude > -85.0 );
    }
  }
  catch (...)
  {
    return false;
  }
}

#ifdef _WIN32
#pragma warning( default : 4291 ) // Enable warning messages for new(nothrow)
#endif
