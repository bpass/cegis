// $Id: Projection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <iomanip>
#include <strstream>
#include "Projection.h"
#include "DatumConvertor.h"
#include "GeographicProjection.h"

extern "C"
{
#include "gctpc/proj.h"
}

using namespace ProjLib;

// ***************************************************************************
Projection::Projection( DATUM d, UNIT u, DATUM geoDatum, UNIT geoUnit )
  : d_datum(d), d_geoDatum(geoDatum), d_unit(u), d_geoUnit(geoUnit)
{
  // If the geographic datum is default just use the projection's datum
  if ( DEFAULT_DATUM == d_geoDatum )
  {
    d_geoDatum = d_datum;
  }
}


// ***************************************************************************
Projection::Projection( const Projection& p )
{
  if ( this != &p )
  {
    d_datum = p.d_datum;
    d_unit  = p.d_unit;
    d_geoDatum = p.d_geoDatum;
    d_geoUnit = p.d_geoUnit;
  }
}


// ***************************************************************************
Projection::~Projection()
{
}

// Operator overlays
// ***************************************************************************
bool Projection::operator==( const Projection& p ) const throw()
{
  try
  {
    if ( d_datum == p.getDatum() && 
         d_unit  == p.getUnit()  &&
         getProjectionSystem() == p.getProjectionSystem() )
      return true;
    else
      return false;
  }
  catch (...)
  {
    return false;
  }
}


// ***************************************************************************
bool Projection::operator!=( const Projection& p ) const throw()
{
  return ( !operator==( p ) );
}


// ***************************************************************************
std::string Projection::toString() const throw()
{
  std::strstream tempstream;
  std::string    ret;
  
  try
  {
    tempstream << std::string("DATUM: ") << datumToString(d_datum)
               << std::endl << std::string("UNIT: ") << unitToString(d_unit)
               << std::endl << std::ends;
    ret = tempstream.str();
    tempstream.freeze(false); // so it'll clean up after itself

    return ret;
  }
  catch(...)
  {
    return std::string("");
  }
}


// ***************************************************************************
double Projection::packedDMSToDouble( double packedDMS ) throw()
{
  double sign = 1.0;
  short  deg, min;
  double result;

  if ( packedDMS < 0.0 )
  {
    sign = -1.0;
    packedDMS = -packedDMS;
  }

  deg        = static_cast<short int>(packedDMS / 1000000);
  packedDMS -= deg * 1000000;
  min        = static_cast<short int>(packedDMS / 1000);
  packedDMS -= min * 1000;

  result  = static_cast<double>(deg);
  result += min / 60.0;
  result += packedDMS / 3600.0;
  result *= sign;

  return result;
}


// ***************************************************************************
std::string Projection::packedDMSToString( double packedDMS, bool bLat )
  throw()
{
  char   nsFlag = 'N';
  char   ewFlag = 'E';
  short  deg;
  short  min;
  double sec;
  char   flag;

  std::strstream tempstream;
  std::string    strTemp;

  if ( packedDMS < 0 )
  {
    nsFlag = 'S';
    ewFlag = 'W';
    packedDMS *= -1;
  }

  deg        = static_cast<short int>(packedDMS / 1000000);
  packedDMS -= deg * 1000000;
  min        = static_cast<short int>(packedDMS / 1000);
  packedDMS -= min * 1000;
  sec        = packedDMS;
  flag       = bLat ? nsFlag : ewFlag;
  
  tempstream.setf(std::ios::fixed);
  tempstream << deg << "° " << std::setw(2) << min << "\' " << std::setw(5) 
             << std::setprecision(2) << sec << "\" " << flag << std::ends;
  strTemp = tempstream.str();
  tempstream.freeze(false);
  
  return strTemp;
}


// ***************************************************************************
std::string Projection::unitToString( UNIT u ) throw()
{
  switch ( u )
  {
  case RADIANS:
    return std::string("radians");
  case US_FEET:
    return std::string("feet");
  case METERS:
    return std::string("meters");
  case ARC_SECONDS:
    return std::string("arc seconds");
  case ARC_DEGREES:
    return std::string("arc degrees");
  case INTERNATIONAL_FEET:
    return std::string("international feet");
  case STATE_ZONE_TABLE:
    return std::string("state zone table units");
  default:
    return std::string("unknown units");
  }
}


// ***************************************************************************
std::string Projection::datumToString( DATUM d ) throw()
{
  switch ( d )
  {
  case ADINDAN:
    return std::string("ADINDAN");
  case ARC1950:
    return std::string("ARC1950");
  case ARC1960:
    return std::string("ARC1960");
  case AUSTRALIAN_GEODETIC_1966:
    return std::string("AUSTRALIAN GEODETIC 1966");
  case AUSTRALIAN_GEODETIC_1984:
    return std::string("AUSTRALIAN GEODETIC 1984");
  case CAMP_AREA_ASTRO:
    return std::string("CAMP AREA ASTRO");
  case CAPE:
    return std::string("CAPE");
  case EUROPEAN_DATUM_1950:
    return std::string("EUROPEAN DATUM 1950");
  case EUROPEAN_DATUM_1979:
    return std::string("EUROPEAN DATUM 1979");
  case GEODETIC_DATUM_1949:
    return std::string("GEODETIC DATUM 1949");
  case HONG_KONG_1963:
    return std::string("HONG KONG 1963");
  case HU_TZU_SHAN:
    return std::string("HU TZU SHAN");
  case INDIAN:
    return std::string("INDIAN");
  case NAD27:
    return std::string("NAD27");
  case NAD83:
    return std::string("NAD83");
  case OLD_HAWAIIAN_MEAN:
    return std::string("OLD HAWAIIAN MEAN");
  case OMAN:
    return std::string("OMAN");
  case ORDNANCE_SURVEY_1936:
    return std::string("ORDNANCE SURVEY 1936");
  case PULKOVO_1942:
    return std::string("PULKOVO 1942");
  case PROVISIONAL_S_AMERICAN_1956:
    return std::string("PROVISIONAL SOUTH AMERICAN 1956");
  case TOKYO:
    return std::string("TOKYO");
  case WGS_72:
    return std::string("WGS72");
  case WGS_84:
    return std::string("WGS84");
  case NO_DATUM:
    return std::string("NO DATUM");
  default:
    return std::string("Unknown Datum");
  }
}


// Datum conversion
// **************************************************************************
bool Projection::convertDatum( double& latitude, double& longitude, 
                               DATUM from, DATUM to ) 
  const throw()
{
  // Convert the coordinates to degrees if they aren't there already
  double factor;
  bool   bResult = true;

  try
  {
    untfz( d_geoUnit, ARC_DEGREES, &factor );

    latitude  *= factor;
    longitude *= factor;

    // Make sure the coordinates are in the valid range for this projection
    if ( bResult = checkCoordinateRange( latitude, longitude ) )
    {
      // Convert the datum of the point if necessary
      if ( from != to )
      {
        bResult = DatumConvertor::convertDatum( latitude, longitude, from, to );
      }
      
      // Check the coordinate range again to make sure the datum stuff didn't
      // screw it up
      if ( bResult )
      {
        bResult = checkCoordinateRange( latitude, longitude );
      }
    }
    
    // Convert the point back to its original units
    latitude  /= factor;
    longitude /= factor;
    
    return bResult;
  }
  catch (...)
  {
    return false;
  }
}

