// $Id: DatumConvertor.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include "DatumConvertor.h"
#include <math.h>

// Define WGS84 constants
namespace ProjLib
{
#define PI 3.1415926535
#define WGS84_RADIUS 6378137.0
#define WGS84_FLATTENING ( 1.0 / 298.257223563 )
}

using namespace ProjLib;

// Initialize datum conversion parameters array
DatumConvertor::DatumParameters DatumConvertor::d_datumParams[] =
{ 
  { ADINDAN, CLARKE_1880, -162, -12, -206 },
  { ARC1950, CLARKE_1880, -143, -90, -294 },
  { ARC1960, CLARKE_1880, -160,  -8, -300 },
  { AUSTRALIAN_GEODETIC_1966, AUSTRALIAN_NATIONAL, -133, -48, 148 },
  { AUSTRALIAN_GEODETIC_1984, AUSTRALIAN_NATIONAL, -134, -48, 149 },
  { CAMP_AREA_ASTRO, INTERNATIONAL_1967, -104, -129, 239 },
  { CAPE, CLARKE_1880, -136, -108, -292 },
  { EUROPEAN_DATUM_1950, INTERNATIONAL_1967, -87, -98, -121 },
  { EUROPEAN_DATUM_1979, INTERNATIONAL_1967, -86, -98, -119 },
  { GEODETIC_DATUM_1949, INTERNATIONAL_1967, 84, -22, 209 },
  { HONG_KONG_1963, INTERNATIONAL_1967, -156, -271, -189 },
  { HU_TZU_SHAN, INTERNATIONAL_1967, -634, -549, -201 },
  { INDIAN, EVEREST, 289, 734, 257 },
  { NAD27, CLARKE_1866, -8, 160, 176 },
  { NAD83, GRS_1980, 0, 0, 0 },
  { OLD_HAWAIIAN_MEAN, CLARKE_1866, 89, -279, -183 },
  { OMAN, CLARKE_1880, -346, -1, 224 },
  { ORDNANCE_SURVEY_1936, AIRY, 375, -111, 431 },
  { PUERTO_RICO, CLARKE_1866, 11, 72, -101 },
  { PULKOVO_1942, KRASSOVSKY, 27, -135, -89 },
  { PROVISIONAL_S_AMERICAN_1956, INTERNATIONAL_1967, -288, 175, -376 },
  { TOKYO, BESSEL, -128, 481, 664 },
  { WGS_72, WGS_72_ELLIPSOID, 0, 0, -4.5 },
  { WGS_84, WGS_84_ELLIPSOID, 0, 0, 0 } 
};

// Initialize ellipsoid parameters array
DatumConvertor::EllipsoidParameters DatumConvertor::d_ellipsoidParams[] =
{
  { CLARKE_1866, -69.4, -0.000037264639 },
  { CLARKE_1880, -112.145, -0.000054750714 },
  { BESSEL, 739.845, 0.000010037483 },
  { INTERNATIONAL_1967, -251.0, -0.000014192702 },
  { WGS_72_ELLIPSOID, 2.0, 0.00000003121057 },
  { EVEREST, 860.655, 0.000028361368 },
  { GRS_1980, 0.0, -0.000000000016 },
  { AIRY, 573.604, 0.000011960023 },
  { MODIFIED_EVEREST, 832.937, 0.000028361368 },
  { MODIFIED_AIRY, 796.811, 0.000011960023 },
  { WGS_84_ELLIPSOID, 0.0, 0.0 },
  { AUSTRALIAN_NATIONAL, -23.0, -0.000000081204 },
  { KRASSOVSKY, -108, 0.000000480795 },
  { HOUGH, -133, -0.000014192702 }
};



// Conversion functions
// ***************************************************************************
bool DatumConvertor::convertDatum( double& lat, double& lon, 
                                   DATUM from, DATUM to ) throw()
{
  if ( from == to )
  {
    return true;
  }

  // XXX - Should I be getting the actual height for this location from a table
  //       or something?  The height doesn't seem to effect the results much,
  //       at least within the accuracy limits of USGS data.
  double height = 0;

  // Convert the coordinates to radians
  lat *= PI / 180.0;
  lon *= PI / 180.0;

  // Convert the point to WGS84
  if (from != WGS_84)
  {
    if ( !convertToWGS84( lat, lon, height, from ) )
    {
      return false;
    }
  }

  // Convert the point from WGS84
  if (to != WGS_84)
  {
    if ( !convertFromWGS84( lat, lon, height, to ) )
    {
      return false;
    }
  }

  // Convert the coordinates back to degrees
  lat *= 180.0 / PI;
  lon *= 180.0 / PI;
  
  return true;
}


// ***************************************************************************
bool DatumConvertor::convertFromWGS84( double& lat, double& lon,
                                       double& height, DATUM to ) throw()
{
  double z = height;
  double deltaA, deltaF;
  double fromA, fromF, fromES;
  double bda;
  double sLat, cLat, sLon, cLon;
  double dx, dy, dz;
  double tmp, sqrtTmp;
  double rn, rm;
  double dLat, dLon, dHeight;
  DatumParameters*     pDatumParams = NULL;
  EllipsoidParameters* pEllipsoidParams = NULL;

  try
  {
    // Get the datum parameters
    pDatumParams = getDatumParameters( to );
    if ( NULL == pDatumParams )
    {
      return false;
    }
    
    // Get the ellipsoid parameters
    pEllipsoidParams = getEllipsoidParameters( pDatumParams->d_ellipsoid );
    if ( NULL == pEllipsoidParams )
    {
      return false;
    }
    
    // Initialize parameters
    deltaA = -pEllipsoidParams->d_deltaA;
    deltaF = -pEllipsoidParams->d_deltaF;
    fromA  = WGS84_RADIUS;
    fromF  = WGS84_FLATTENING;
    fromES = ( 2.0 - fromF ) * fromF;
    bda    = 1.0 - fromF;
    sLat   = sin( lat );
    cLat   = cos( lat );
    sLon   = sin( lon );
    cLon   = cos( lon );
    dx     = -pDatumParams->d_deltaX;
    dy     = -pDatumParams->d_deltaY;
    dz     = -pDatumParams->d_deltaZ;
    
    // Calculate the radius of curvature in prime veritcal
    tmp     = 1.0 - fromES * sLat * sLat;
    sqrtTmp = sqrt( tmp );
    rn      = fromA / sqrtTmp;
    
    // Calculate the radius of curvature in prime meridian
    rm = fromA * ( 1.0 - fromES ) / ( tmp * sqrtTmp );
    
    // Calculate the difference in latitude
    dLat  = ( -dx * sLat * cLon - dy * sLat * sLon ) + dz * cLat;
    dLat += ( deltaA * rn * fromES * sLat * cLat ) / fromA;
    dLat += deltaF * ( rm / bda + rn * bda ) * sLat * cLat;
    dLat /= rm + z;
    
    // Calculate the difference in longitude
    dLon = ( -dx * sLon + dy * cLon ) / ( cLat * ( rn + z ) );
    
    // Calculate the difference in height
    dHeight = dx * cLat * cLon + dy * cLat * sLon + dz 
              * sLat - ( deltaA * fromA ) / rn;
    dHeight += deltaF * bda * rn * sLat * sLat;
    
    lat += dLat;
    lon += dLon;
    height += dHeight;
    
    return true;
  }
  catch (...)
  {
    return false;
  }
}

bool DatumConvertor::convertToWGS84( double& lat, double& lon, double& height,
                                     DATUM from ) throw()
{
  // Use z as zero for now
  double z = height;
  double deltaA, deltaF;
  double fromA, fromF, fromES;
  double bda;
  double sLat, cLat, sLon, cLon;
  double dx, dy, dz;
  double tmp, sqrtTmp;
  double rn, rm;
  double dLat, dLon, dHeight;
  DatumParameters*      pDatumParams     = NULL;
  EllipsoidParameters* pEllipsoidParams = NULL;

  try
  {
    // Get the datum parameters
    pDatumParams = getDatumParameters( from );
    if ( NULL == pDatumParams )
    {
      return false;
    }

    // Get the ellipsoid parameters
    pEllipsoidParams = getEllipsoidParameters( pDatumParams->d_ellipsoid );
    if ( NULL == pEllipsoidParams )
    {
      return false;
    }

    // Initialize parameters
    deltaA = pEllipsoidParams->d_deltaA;
    deltaF = pEllipsoidParams->d_deltaF;
    fromA  = WGS84_RADIUS - deltaA;
    fromF  = WGS84_FLATTENING - deltaF;
    fromES = ( 2.0 - fromF ) * fromF;
    bda    = 1.0 - fromF;
    sLat   = sin( lat );
    cLat   = cos( lat );
    sLon   = sin( lon );
    cLon   = cos( lon );
    dx     = pDatumParams->d_deltaX;
    dy     = pDatumParams->d_deltaY;
    dz     = pDatumParams->d_deltaZ;
    
    // Calculate the radius of curvature in prime veritcal
    tmp     = 1.0 - fromES * sLat * sLat;
    sqrtTmp = sqrt( tmp );
    rn      = fromA / sqrtTmp;

    // Calculate the radius of curvature in prime meridian
    rm = fromA * ( 1.0 - fromES ) / ( tmp * sqrtTmp );

    // Calculate the difference in latitude
    dLat  = ( -dx * sLat * cLon - dy * sLat * sLon ) + dz * cLat;
    dLat += ( deltaA * rn * fromES * sLat * cLat ) / fromA;
    dLat += deltaF * ( rm / bda + rn * bda ) * sLat * cLat;
    dLat /= rm + z;
  
    // Calculate the difference in longitude
    dLon = ( -dx * sLon + dy * cLon ) / ( cLat * ( rn + z ) );

    // Calculate the difference in height
    dHeight = dx * cLat * cLon + dy * cLat * sLon + dz *
              sLat - ( deltaA * fromA ) / rn;
    dHeight += deltaF * bda * rn * sLat * sLat;
    
    lat    += dLat;
    lon    += dLon;
    height += dHeight;
    
    return true;
  }
  catch (...)
  {
    return false;
  }
}

/*
bool
DatumConvertor::convertToECEF( double& lat, double& lon, double& height,
DATUM from )
{
   // Get the conversion parameters
   DatumParameters* pParams = getConversionParameters( from );
    if ( NULL == pParams )
    {
        return false;
    }

    // Initialize parameters
    double deltaA = pParams->d_deltaA;
    double deltaF = pParams->d_deltaF;
    double fromA = WGS84_RADIUS - deltaA;
    double fromF = WGS84_FLATTENING - deltaF;
    double fromES = ( 2.0 - fromF ) * fromF;
    double polarRadius = fromA * ( 1.0 - fromF );
    double sLat = sin( lat );
    double cLat = cos( lat );
    double sLon = sin( lon );
    double cLon = cos( lon );
    double N = fromA / sqrt( 1 - fromES * sLat * sLat );

    lon = ( N + height ) * cLat * cLon + pParams->d_deltaX;
    lat = ( N + height ) * cLat * sLon + pParams->d_deltaY;
    height = ( N * ( 1.0 - fromES ) + height ) * sLat + pParams->d_deltaZ;

    return true;
}

bool
DatumConvertor::convertFromECEF( double& lat, double& lon, double& height, DATUM to )
{
    // Get the conversion parameters
    DatumParameters* pParams = getConversionParameters( to );
    if ( NULL == pParams )
    {
        return false;
    }

    lon -= pParams->d_deltaX;
    lat -= pParams->d_deltaY;
    height -= pParams->d_deltaZ;

    // Initialize parameters
    double deltaA = pParams->d_deltaA;
    double deltaF = pParams->d_deltaF;
    double toA = WGS84_RADIUS - deltaA;
    double toF = WGS84_FLATTENING - deltaF;
    double toES = ( 2.0 - toF ) * toF;
    double polarRadius = toA * ( 1.0 - toF );
    double p = sqrt( lat * lat + lon * lon );
    double theta = atan( height / p );
    double e2s = ( toA * toA - polarRadius * polarRadius ) / ( polarRadius * polarRadius );

    double Y = lat;
    lat = atan( ( height + e2s * polarRadius * sin( theta ) * sin( theta ) * sin( theta ) ) /
                ( p - toES * toA * cos( theta ) * cos( theta ) * cos( theta ) ) );
    lon = atan2( Y, lon );

    double sLat = sin( lat );
    double N = toA / sqrt( 1 - toES * sLat * sLat );
    height = p / cos( lat ) - N;

    return true;
}*/

DatumConvertor::DatumParameters* DatumConvertor::getDatumParameters( DATUM d )
  throw()
{
  int numEntries;
  
  try
  {
    numEntries = sizeof( d_datumParams ) / sizeof( DatumParameters );
    for ( int i = 0; i < numEntries; i++ )
    {
      if ( d_datumParams[i].d_datum == d )
      {
        return &d_datumParams[i];
      }
    }
  
    return NULL;
  }
  catch (...)
  {
    return NULL;
  }
}

DatumConvertor::EllipsoidParameters* 
DatumConvertor::getEllipsoidParameters( ELLIPSOID e ) throw()
{
  int numEntries;

  try
  {
    numEntries = sizeof( d_ellipsoidParams ) / sizeof( EllipsoidParameters );
    for ( int i = 0; i < numEntries; i++ )
    {
      if ( d_ellipsoidParams[i].d_ellipsoid == e )
      {
        return &d_ellipsoidParams[i];
      }
    }
    
    return NULL;
  }
  catch (...)
  {
    return NULL;
  }
}

ELLIPSOID DatumConvertor::getDatumEllipsoid( DATUM d ) throw()
{
  DatumParameters* pParams = NULL;
  
  try
  {
    pParams = getDatumParameters( d );

    if ( NULL == pParams )
    {
      return UNKNOWN_ELLIPSOID;
    }

    return pParams->d_ellipsoid;
  }
  catch (...)
  {
    return UNKNOWN_ELLIPSOID;
  }
}
