// $Id: UTMProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <strstream>
#include <new>
#include "UTMProjection.h"
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
UTMProjection::UTMProjection( long zone,
                              DATUM d, UNIT u, 
                              DATUM geoDatum, UNIT geoUnit )
  : Projection(d,u,geoDatum,geoUnit), ZonedProjection(zone), 
    d_latInZone(0.0), d_lonInZone(0.0)
{
}


// ***************************************************************************
UTMProjection::UTMProjection( double latInZone, double lonInZone,
                              DATUM d, UNIT u, 
                              DATUM geoDatum, UNIT geoUnit )
  : Projection(d,u,geoDatum,geoUnit), ZonedProjection(0), 
    d_latInZone(latInZone), d_lonInZone(lonInZone)
{
}


// ***************************************************************************
UTMProjection::UTMProjection( const UTMProjection& p )
  : Projection(p), ZonedProjection(p)
{
  if ( this != &p )
  {
    d_latInZone = p.d_latInZone;
    d_lonInZone = p.d_lonInZone;
  }
}


// ***************************************************************************
UTMProjection::~UTMProjection()
{
}


// ***************************************************************************
bool UTMProjection::operator==( const Projection& p ) const throw()
{
  const UTMProjection* pRHS = NULL;

  try
  {
    // Check to see if the projection passed in is UTM
    if ( UTM == p.getProjectionSystem() )
    {
      pRHS = dynamic_cast<const UTMProjection*>(&p);
      if ( NULL != pRHS )
      {
        if (d_zone == pRHS->getZone() )
        {
          return Projection::operator==(p);
        }
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
void UTMProjection::setZone( long zone ) throw()
{
  // Make sure the zone number is valid before setting it
  if ( ( zone < -60 || zone > 60 ) )
  {
    // Invalid zone number, use zero instead
    zone = 0;
  }

  ZonedProjection::setZone( zone );
}


// ***************************************************************************
bool UTMProjection::projectToGeo( double x, double y, 
                                  double& latitude, double& longitude ) 
  const throw()
{
  double inparm[15];  // input parameter array
  long   iflg;		// error flag
  double factor;

  // Initialize the projection parameter arrays to all zeroes
  for ( int i = 0; i < 15; i++ )
  {
    inparm[i] = 0.0;
  }

  try
  {
    // If a zone wasn't specified, use the lat and lon passed in
    if ( 0 == d_zone)
    {
      inparm[0] = d_lonInZone;
      inparm[1] = d_latInZone;
    }

    // Initialize the UTM inverse transformation
    InverseTransFunc inv_trans[MAXPROJ + 1];
   
    inv_init( getProjectionSystem(), d_zone, inparm, 
              DatumConvertor::getDatumEllipsoid(d_datum), 
              NULL, NULL, &iflg, inv_trans );
    
    // Convert the x and y to meters
    untfz( d_unit, METERS, &factor );
    x *= factor;
    y *= factor;    
    
    // do the inverse transformation
    utminv( x, y, &longitude, &latitude );

    // Convert the input coordinates from radians to the correct units
    untfz( RADIANS, d_geoUnit, &factor );
    longitude *= factor;
    latitude  *= factor;

    // Convert the datums
    return convertDatum( latitude, longitude, d_datum, d_geoDatum );
  }
  catch(...)
  {
    return false;
  }
}


// ***************************************************************************
bool UTMProjection::projectFromGeo( double latitude, double longitude,
                                    double& x, double& y ) const throw()
{
  double outparm[15];	// output parameter array
  long   iflg;
  double factor;
    
  
  try
  {
    // Convert the datums
    if ( !convertDatum( latitude, longitude, d_geoDatum, d_datum) )
    {
      return false;
    }
 
    // Initialize the projection parameter arrays to all zeroes
    for ( int i = 0; i < 15; i++ )
    {
      outparm[i] = 0.0;
    }
    
    // If a zone wasn't specified, use the lat and lon passed in
    if ( 0 == d_zone)
    {
      outparm[0] = d_lonInZone;
      outparm[1] = d_latInZone;
    }

    // Initialize the UTM forward transformation
    ForwardTransFunc for_trans[MAXPROJ + 1];
    for_init( getProjectionSystem(), d_zone, outparm, 
              DatumConvertor::getDatumEllipsoid(d_datum), 
              NULL, NULL, &iflg, for_trans );
    
    // Convert the input coordinates to radians
    untfz( d_geoUnit, RADIANS, &factor );
    longitude *= factor;
    latitude  *= factor;

    // do the forward transformation
    utmfor( longitude, latitude, &x, &y );

    // Convert the x and y from meters to the correct units
    untfz( METERS, d_unit, &factor );
    x *= factor;
    y *= factor;
    
    return true;
  }
  catch(...)
  {
    return false;
  }
}


// ***************************************************************************
Projection* UTMProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) UTMProjection( *this ) ))
    throw std::bad_alloc();

  return foo;
}				       


// ***************************************************************************
std::string UTMProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string    ret;

  try
  {
    tempstream << std::string("UTM Projection") << std::endl
               << Projection::toString() << std::string("UTM ZONE: ")
               << d_zone << std::ends;
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
bool UTMProjection::checkCoordinateRange( double latitude,
					  double longitude ) const throw()
{
  try
  {
    if ( !Projection::checkCoordinateRange( latitude, longitude ) )
    {
      return false;
    }

    // UTM is only valid between 84N and 80S
    return ( latitude <= 84.0 && latitude >= -80.0 );
  }
  catch (...)
  {
    return false;
  }
}

#ifdef _WIN32
#pragma warning( default : 4291 ) // Disable warning messages for new(nothrow)
#endif  
