// $Id: StatePlaneProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <strstream>
#include <new>
#include "StatePlaneProjection.h"
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
// Static member initialization
std::string StatePlaneProjection::d_strNAD27ParamFilename;
std::string StatePlaneProjection::d_strNAD83ParamFilename;

// ***************************************************************************
StatePlaneProjection::StatePlaneProjection( long zone,
                                            DATUM d, UNIT u, 
                                            DATUM geoDatum, UNIT geoUnit )
  : Projection(d,u,geoDatum,geoUnit), ZonedProjection(zone)
{
}


// ***************************************************************************
StatePlaneProjection::StatePlaneProjection( const StatePlaneProjection& p )
  : Projection(p), ZonedProjection(p)
{
}


// ***************************************************************************
StatePlaneProjection::~StatePlaneProjection()
{
}


// ***************************************************************************
bool StatePlaneProjection::operator==( const Projection& p ) const throw()
{
  const StatePlaneProjection* pRHS = NULL;

  try
  {
    // Check to see if the projection passed in is UTM
    if ( SPCS == p.getProjectionSystem() )
    {
      pRHS = dynamic_cast<const StatePlaneProjection*>(&p);
      if ( NULL != pRHS )
      {
        if ( d_zone == pRHS->getZone() )
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
void StatePlaneProjection::setZone( long zone ) throw()
{
  // Make sure the zone number is valid before setting it
  if ( ( zone < 101 || zone > 5400 ) )
  {
    // Invalid zone number, use zero instead
    zone = 0;
  }
  
  ZonedProjection::setZone( zone );
}


// ***************************************************************************
bool StatePlaneProjection::projectToGeo( double x, double y, 
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
    // Initialize the state plane inverse transformation
    InverseTransFunc inv_trans[MAXPROJ + 1];
    inv_init( getProjectionSystem(), d_zone, inparm, 
              DatumConvertor::getDatumEllipsoid(d_datum), 
              (char*)d_strNAD27ParamFilename.c_str(),
              (char*)d_strNAD83ParamFilename.c_str(),
              &iflg, inv_trans );

    // Convert the x and y to meters
    untfz(d_unit, METERS, &factor );
    x *= factor;
    y *= factor;    
    
    // do the inverse transformation
    stplninv( x, y, &longitude, &latitude );

    // Convert the input coordinates from radians to the correct units
    untfz( RADIANS, d_geoUnit, &factor );
    longitude *= factor;
    latitude *= factor;
  
    // Convert the datums
    return convertDatum( latitude, longitude, d_datum, d_geoDatum);
  }
  catch(...)
  {
    return false;
  }
}


// ***************************************************************************
bool StatePlaneProjection::projectFromGeo( double latitude, double longitude,
                                           double& x, double& y ) 
  const throw()
{
  double outparm[15];	// output parameter array
  long   iflg;		// error flag
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
 
    // Initialize the state plane forward transformation
    ForwardTransFunc for_trans[MAXPROJ + 1];
    for_init(getProjectionSystem(), d_zone, outparm, 
             DatumConvertor::getDatumEllipsoid(d_datum),
             (char*)d_strNAD27ParamFilename.c_str(),
             (char*)d_strNAD83ParamFilename.c_str(),
             &iflg, for_trans );
    
    // Convert the input coordinates to radians
    untfz( d_geoUnit, RADIANS, &factor );
    longitude *= factor;
    latitude *= factor;
  
    // do the forward transformation
    stplnfor( longitude, latitude, &x, &y );
  
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


// ***************************************************************************
Projection* StatePlaneProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) StatePlaneProjection(*this)))
    throw std::bad_alloc();

  return foo;
}


// ***************************************************************************
std::string StatePlaneProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string    ret;

  try
  {
    tempstream << std::string("State Plane Projection") << std::endl
               << Projection::toString() << std::string("STATE PLANE ZONE: ")
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
bool StatePlaneProjection::checkCoordinateRange( double latitude,
                                                 double longitude ) 
  const throw()
{
  try
  {
    if ( !Projection::checkCoordinateRange( latitude, longitude ) )
    {
      return false;
    }

    return ( latitude <= 89.0 && latitude >= -89.0 );
  }
  catch (...)
  {
    return false;
  }
}


#ifdef _WIN32
#pragma warning( default : 4291 ) // Enable warning messages for new(nothrow)
#endif




