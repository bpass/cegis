// $Id: GeographicProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <strstream>
#include <new>
#include "GeographicProjection.h"
#include "DatumConvertor.h"

// GCTPC includes
extern "C"
{
#include "gctpc/proj.h"
}

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif

// ***************************************************************************
GeographicProjection::GeographicProjection( DATUM d, UNIT u, DATUM geoDatum,
                                            UNIT geoUnit )
  : Projection(d,u,geoDatum,geoUnit)
{

}


// ***************************************************************************
GeographicProjection::GeographicProjection( const GeographicProjection& p )
  : Projection(p)
{
}


// ***************************************************************************
GeographicProjection::~GeographicProjection()
{
}


// ***************************************************************************
bool GeographicProjection::projectToGeo( double x, double y,
                                         double& latitude,
                                         double& longitude ) const throw()
{
  double factor;
  
  try
  {
    // Test for simple case where units and datums are equal
    if ( ( d_datum == d_geoDatum ) && ( d_unit == d_geoUnit ) )
    {
      latitude  = y;
      longitude = x;
    }
    else
    {
      // Conver the units
      untfz( d_unit, d_geoUnit, &factor );
      latitude  = y * factor;
      longitude = x * factor;
      
      // Convert the datums
      return convertDatum( latitude, longitude, d_datum, d_geoDatum );
    }

    return true;
  }
  catch (...)
  {
    return false;
  }
}


// ***************************************************************************
bool GeographicProjection::projectFromGeo( double latitude, double longitude,
                                           double& x, double& y ) const throw()
{
  double factor;

  try
  {
    // Test for simple case where units and datums are equal
    if ( ( d_datum == d_geoDatum ) && ( d_unit == d_geoUnit ) )
    {
      x = longitude;
      y = latitude;
    }
    else
    {
      // Convert the datums
      if ( !convertDatum( latitude, longitude, d_geoDatum, d_datum ) )
      {
        return false;
      }
      
      // Convert the units
      untfz( d_geoUnit, d_unit, &factor );
      y = latitude * factor;
      x = longitude * factor;
    }
    return true;
  }
  catch (...)
  {
    return false;
  }
}


// ***************************************************************************
Projection* GeographicProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) GeographicProjection(*this)))
    throw std::bad_alloc();
  else
    return foo;
}


// ***************************************************************************
std::string GeographicProjection::toString() const throw()
{
  std::string    retstr;
  std::strstream tempstream;

  try
  {
    tempstream << "Geographic Projection" << std::endl << Projection::toString()
      << std::ends;

    retstr = tempstream.str();
    tempstream.freeze(false);

    return retstr;
  }
  catch (...)
  {
    return std::string("");
  }
}


#ifdef _WIN32
#pragma warning( default : 4291 ) // Enable warning messages for new(nothrow)
#endif
