// $Id: AlaskaConformalProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <new>
#include <strstream>
#include "AlaskaConformalProjection.h"
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
AlaskaConformalProjection::AlaskaConformalProjection( double sMajor,
                                                      double sMinor, 
                                                      double fe, double fn, 
                                                      DATUM d, UNIT u, 
                                                      DATUM geoDatum,
                                                      UNIT geoUnit )
  : TransverseMercatorProjection(0.0,sMajor,sMinor,0.0,0.0,fe,fn,d,u,geoDatum,
				 geoUnit)
{
}


// ***************************************************************************
AlaskaConformalProjection::AlaskaConformalProjection( const AlaskaConformalProjection& p )
  : TransverseMercatorProjection(p)
{
}


// ***************************************************************************
std::string AlaskaConformalProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string ret;

  try
  {
    tempstream << std::string("Alaska Conformal Projection") << std::endl
               << Projection::toString() << std::endl 
               << std::string("FALSE EASTING: ") << d_projParams[6] 
               << std::endl << std::string("FALSE NORTHING: ") 
               << d_projParams[7] << std::endl
               << std::string("SEMI-MAJOR AXIS: ") << d_projParams[0] 
               << std::endl << std::string("SEMI-MINOR AXIS: ")
               << d_projParams[1] << std::ends;
    ret = tempstream.str();
    tempstream.freeze(false);
    return ret;
  }
  catch (...)
  {
    return std::string("");
  }
}


// Projection functions
// ***************************************************************************
bool AlaskaConformalProjection::projectToGeo( double x, double y, 
					      double& latitude, 
					      double& longitude ) 
  const throw()
{
  long             iflg;  // error flag
  double           factor;
  InverseTransFunc inv_trans[MAXPROJ + 1];

  try
  {
    // Initialize the inverse transformation
    inv_init(ALASKA, 0, const_cast<double*>(d_projParams), 
             DatumConvertor::getDatumEllipsoid( d_datum ), 
             NULL, NULL, &iflg, inv_trans );

    // Convert the x and y to meters
    untfz(d_unit, METERS, &factor );
    x *= factor;
    y *= factor;    
    
    // do the inverse transformation
    alconinv( x, y, &longitude, &latitude );

    // Convert the input coordinates from radians to the correct units
    untfz( RADIANS, d_geoUnit, &factor );
    longitude *= factor;
    latitude  *= factor;

    // Convert the datums
    return convertDatum( latitude, longitude, d_datum, d_geoDatum);
  }
  catch (...)
  {
    return false;
  }
}


// ***************************************************************************
bool AlaskaConformalProjection::projectFromGeo( double latitude, 
                                                double longitude,
                                                double& x, double& y ) const
  throw()
{ 
  long             iflg;
  ForwardTransFunc for_trans[MAXPROJ + 1];
  double           factor;

  try
  {
    // Convert the datums
    if ( !convertDatum( latitude, longitude, d_geoDatum, d_datum) )
    {
      return false;
    }


    
    // Initialize the UTM forward transformation
    for_init(ALASKA, 0, const_cast<double*>(d_projParams), 
             DatumConvertor::getDatumEllipsoid(d_datum), 
             NULL, NULL, &iflg, for_trans );
    
    // Convert the input coordinates to radians
    untfz(d_geoUnit, RADIANS, &factor );
    longitude *= factor;
    latitude  *= factor;

    // do the forward transformation
    alconfor( longitude, latitude, &x, &y );

    // Convert the x and y from meters to the correct units
    untfz( METERS, d_unit, &factor );
    x *= factor;
    y *= factor;
    
    return true;
  }
  catch (...)
  {
    return false;
  }
}


// Cloning
// ***************************************************************************
Projection* AlaskaConformalProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) AlaskaConformalProjection(*this)))
    throw std::bad_alloc();
  
  return foo;
}

#ifdef _WIN32
#pragma warning( default : 4291 ) // Disable warning messages for new(nothrow)
#endif
