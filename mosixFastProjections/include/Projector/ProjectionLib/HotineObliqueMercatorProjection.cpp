// $Id: HotineObliqueMercatorProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <strstream>
#include <new>
#include "HotineObliqueMercatorProjection.h"

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif

// ***************************************************************************
HotineObliqueMercatorProjection::HotineObliqueMercatorProjection( 
		                 double scaleFactor, double long1, double lat1,
                     double long2, double lat2, double sMajor,
                     double sMinor, double originLat, double fe,
                     double fn, DATUM d, UNIT u, DATUM geoDatum,
                     UNIT geoUnit )
  : MercatorProjection(sMajor,sMinor,0.0,originLat,fe,fn,d,u,geoDatum,geoUnit)
{
  setProjectionCenterScaleFactor( scaleFactor );
  setLongitude1( long1 );
  setLatitude1( lat1 );
  setLongitude2( long2 );
  setLatitude2( lat2 );
  
  // Denote HotineA
  d_projParams[12] = 0;
}


// ***************************************************************************
HotineObliqueMercatorProjection::HotineObliqueMercatorProjection( 
                                 double scaleFactor, double aziAng,
                                 double sMajor, double sMinor, double aziPt,
                                 double originLat, double fe, double fn,
                                 DATUM d, UNIT u, DATUM geoDatum, UNIT geoUnit)
  : MercatorProjection(sMajor,sMinor,0.0,originLat,fe,fn,d,u,geoDatum,geoUnit)
{
  setProjectionCenterScaleFactor( scaleFactor );
  setAzimuthAngle( aziAng );
  setAzimuthPoint( aziPt );
  
  // Denote HotineB
  d_projParams[12] = 1;
}


// ***************************************************************************
HotineObliqueMercatorProjection::HotineObliqueMercatorProjection( const HotineObliqueMercatorProjection& p )
  : MercatorProjection(p)
{
  if ( this != &p )
  {
    setProjectionCenterScaleFactor( p.getProjectionCenterScaleFactor() );
    setAzimuthAngle( p.getAzimuthAngle() );
    setAzimuthPoint( p.getAzimuthPoint() );
    setLongitude1( p.getLongitude1() );
    setLongitude2( p.getLongitude2() );
    setLatitude1( p.getLatitude1() );
    setLatitude2( p.getLatitude2() );
    d_projParams[12] = p.d_projParams[12];
  }
}


// ***************************************************************************
bool HotineObliqueMercatorProjection::operator==( const Projection& p ) 
  const throw()
{
  const HotineObliqueMercatorProjection* pRHS = NULL;

  try
  {
    pRHS = dynamic_cast<const HotineObliqueMercatorProjection*>(&p);
    if ( NULL != pRHS )
    {
      if ( d_projParams[2]  == pRHS->getProjectionCenterScaleFactor() &&
           d_projParams[3]  == pRHS->getAzimuthAngle()                &&
           d_projParams[4]  == pRHS->getAzimuthPoint()                &&
           d_projParams[8]  == pRHS->getLongitude1()                  &&
           d_projParams[10] == pRHS->getLongitude2()                  &&
           d_projParams[9]  == pRHS->getLatitude1()                   &&
           d_projParams[11] == pRHS->getLatitude2()                   &&
           d_projParams[12] == pRHS->d_projParams[12] )
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


// ***************************************************************************
std::string HotineObliqueMercatorProjection::toString() const throw()
{
  std::string    ret;
  std::strstream tempstream;

  try
  {
    // Hotine A
    if ( 0 == d_projParams[12] )
    {
      tempstream << std::string("Hotine Oblique Mercator A Projection") 
                 << std::endl << Projection::toString()
                 << std::string("ORIGIN LATITUDE: ")
                 << packedDMSToString( d_projParams[5], true ) << std::endl
                 << std::string("FALSE EASTING: ") << d_projParams[6]
                 << std::endl << std::string("FALSE NORTHING: ")
                 << d_projParams[7] << std::endl
                 << std::string("SEMI-MAJOR AXIS: ") << d_projParams[0]
                 << std::endl << std::string("SEMI-MINOR AXIS: ")
                 << d_projParams[1] << std::endl
                 << std::string("CENTER PROJ SCALE FACTOR: ")
                 << d_projParams[2] << std::endl
                 << std::string("LONGITUDE 1: ")
                 << packedDMSToString(d_projParams[8], false ) << std::endl
                 << std::string("LATITUDE 1: ")
                 << packedDMSToString(d_projParams[9], true ) << std::endl
                 << std::string("LONGITUDE 2: ")
                 << packedDMSToString(d_projParams[10], false ) << std::endl
                 << std::string("LATITUDE 2: ")
                 << packedDMSToString(d_projParams[11], true ) << std::ends;
    }
    // Hotine B
    else
    {
      tempstream << std::string("Hotine Oblique Mercator B Projection")
                 << std::endl << Projection::toString()
                 << std::string("ORIGIN LATITUDE: ")
                 << packedDMSToString(d_projParams[5], true ) << std::endl
                 << std::string("FALSE EASTING: ") << d_projParams[6]
                 << std::endl << std::string("FALSE NORTHING: ") 
                 << d_projParams[7] << std::endl
                 << std::string("SEMI-MAJOR AXIS: ") << d_projParams[9]
                 << std::endl
                 << std::string("SEMI-MINOR AXIS: ") << d_projParams[1]
                 << std::endl
                 << std::string("CENTER PROJ SCALE FACTOR: ")
                 << d_projParams[2] << std::endl
                 << std::string("AZIMUTH ANGLE: ")
                 << packedDMSToString(d_projParams[3], false ) << std::endl
                 << std::string("AZIMUTH POINT: ")
                 << packedDMSToString(d_projParams[4], false ) << std::ends;
    }
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
Projection* HotineObliqueMercatorProjection::clone() 
  const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) HotineObliqueMercatorProjection(*this)))
    throw std::bad_alloc();

  return foo;
}

#ifdef _WIN32
#pragma warning( default : 4291 ) // Enable warning messages for new(nothrow)
#endif
