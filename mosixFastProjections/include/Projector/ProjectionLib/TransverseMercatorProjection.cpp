// $Id: TransverseMercatorProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <iostream>
#include <strstream>
#include <new>
#include "TransverseMercatorProjection.h"
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
TransverseMercatorProjection::TransverseMercatorProjection( 
            double scaleFactor, double sMajor, double sMinor,
			      double centMer, double originLat, double fe, 
			      double fn, DATUM d, UNIT u, DATUM geoDatum,
			      UNIT geoUnit )
  : MercatorProjection(sMajor,sMinor,centMer,originLat,fe,fn,d,u,geoDatum,
                       geoUnit)
{
  setCentralMeridianScaleFactor( scaleFactor );
}


// ***************************************************************************
TransverseMercatorProjection::TransverseMercatorProjection( const TransverseMercatorProjection& p )
  : MercatorProjection(p)
{
  if ( this != &p )
  {
    setCentralMeridianScaleFactor( p.getCentralMeridianScaleFactor() );
  }
}


// Comparison functions
// ***************************************************************************
bool TransverseMercatorProjection::operator==( const Projection& p ) 
  const throw()
{
  const TransverseMercatorProjection* pRHS = NULL;

  try
  {
    pRHS = dynamic_cast<const TransverseMercatorProjection*>(&p);
    if ( NULL != pRHS )
    {
      if ( d_projParams[2] == pRHS->getCentralMeridianScaleFactor() )
      {
        return MercatorProjection::operator==( p );
      }
    }
    
    return false;
  }
  catch (...)
  {
    return false;
  }
}


// toString override
// ***************************************************************************
std::string TransverseMercatorProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string    str;

  try
  {
    tempstream << std::string("Transverse Mercator Projection") << std::endl
               << Projection::toString() << std::string("CENTRAL MERIDIAN: ")
               << packedDMSToString(d_projParams[4], false) << std::endl
               << std::string("ORIGIN LATITUDE: ")
               << packedDMSToString(d_projParams[5], true) << std::endl
               << std::string("FALSE EASTING: ") << d_projParams[6] 
               << std::endl << std::string("FALSE NORTHING: ") 
               << d_projParams[7] << std::endl 
               << std::string("SEMI-MAJOR AXIS: ") << d_projParams[0]
               << std::endl << std::string("SEMI-MINOR AXIS: ")
               << d_projParams[1] << std::endl
               << std::string("CENT. MER. SCALE FACTOR: ") << d_projParams[2]
               << std::ends;
    
    str = tempstream.str();
    tempstream.freeze(false); // so it'll clean up
   
    return str;
  }
  catch (...)
  {
    return std::string("");
  }
}


// Projection functions
// ***************************************************************************
bool TransverseMercatorProjection::projectToGeo( double x, double y, 
                                                 double& latitude,
                                                 double& longitude ) 
  const throw()
{
  long             iflg; // error flag
  double           factor;
  InverseTransFunc inv_trans[MAXPROJ + 1];
    
  try
  {
    // Initialize the inverse transformation

    inv_init( getProjectionSystem(), 0, const_cast<double*>(d_projParams), 
              DatumConvertor::getDatumEllipsoid(d_datum), NULL, NULL, 
              &iflg, inv_trans );
    
    // Convert the x and y to meters
    untfz( d_unit, METERS, &factor );
    x *= factor;
    y *= factor;    
    
    // do the inverse transformation
    tminv( x, y, &longitude, &latitude );
    
    // Convert the input coordinates from radians to the correct units
    untfz( RADIANS, d_geoUnit, &factor );
    longitude *= factor;
    latitude *= factor;
    
    std::cout << " converting datum" << std::endl;
    // Convert the datums
    return convertDatum( latitude, longitude, d_datum, d_geoDatum);
  }
  catch (...)
  {
    std::cout << "caught exception" << std::endl;
    return false;
  }
}

bool TransverseMercatorProjection::projectFromGeo( double latitude,
                                                   double longitude,
                                                   double& x, double& y ) 
  const throw()
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
    for_init( TM, 0, const_cast<double*>(d_projParams), 
              DatumConvertor::getDatumEllipsoid(d_datum), 
              NULL, NULL, &iflg, for_trans );
    
    // Convert the input coordinates to radians

    untfz( d_geoUnit, RADIANS, &factor );
    longitude *= factor;
    latitude *= factor;

    // do the forward transformation
    tmfor( longitude, latitude, &x, &y );
    
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
Projection* TransverseMercatorProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) TransverseMercatorProjection(*this) ))
    throw std::bad_alloc();
  
  return foo;
}

#ifdef _WIN32
#pragma warning( default : 4291 ) // Enable warning messages for new(nothrow)
#endif

