// $Id: AlbersConicProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <new>
#include <strstream>
#include "AlbersConicProjection.h"

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif


// ***************************************************************************
AlbersConicProjection::AlbersConicProjection( double stdpr1,  double stdpr2,
                                              double sMajor,  double sMinor,
                                              double centMer, double originLat,
                                              double fe, double fn, DATUM d,
                                              UNIT u, DATUM geoDatum, 
                                              UNIT geoUnit )
  : ConicProjection( sMajor, sMinor, centMer, originLat, fe, fn, d, u,
                     geoDatum, geoUnit )
{
  setFirstStandardParallel(stdpr1);
  setSecondStandardParallel(stdpr2);
}


// ***************************************************************************
AlbersConicProjection::AlbersConicProjection( const AlbersConicProjection& p )
: ConicProjection(p)
{
  if ( this != &p )
  {
    setFirstStandardParallel(p.getFirstStandardParallel());
    setSecondStandardParallel(p.getSecondStandardParallel());
  }
}


// ***************************************************************************
bool AlbersConicProjection::operator==( const Projection& p ) const throw()
{
  const AlbersConicProjection* pRHS = NULL;

  try
  {
    // Check to see if the projection passed in is Albers Conic
    if ( ALBERS == p.getProjectionSystem() )
    {
      pRHS = dynamic_cast<const AlbersConicProjection*>(&p);
      if ( NULL != pRHS )
      {
        if ( d_projParams[2] == pRHS->getFirstStandardParallel() &&
             d_projParams[3] == pRHS->getSecondStandardParallel() )
        {
          return ConicProjection::operator==( *pRHS );
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


// Cloning
// ***************************************************************************
Projection* AlbersConicProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) AlbersConicProjection(*this)))
    throw std::bad_alloc();

  return foo;
}


// String override
// ***************************************************************************
std::string AlbersConicProjection::toString() const throw()
{
  std::string    ret;
  std::strstream tempstream;

  try
  {
    tempstream << std::string("Albers Conic Projection") << std::endl
               << ConicProjection::toString() << std::endl
               << std::string("FIRST STANDARD PARALLEL: ")
               << packedDMSToString(d_projParams[2], true) << std::endl
               << std::string("SECOND STANDARD PARALLEL: ")
               << packedDMSToString(d_projParams[3], true) << std::ends;
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
bool AlbersConicProjection::checkCoordinateRange( double latitude,
                                                  double longitude ) 
  const throw()
{
  double deltaN, deltaS;
  double maxLat, minLat;

  try
  {
    if ( !Projection::checkCoordinateRange( latitude, longitude ) )
    {
      return false;
    }

    deltaN = ( 90.0 - packedDMSToDouble(d_projParams[3]) ) / 20.0;
    deltaS = ( packedDMSToDouble(d_projParams[2]) + 90.0 ) / 20.0;
    maxLat = 90.0 - deltaN * deltaN;
    minLat = -90 + deltaS * deltaS;
    return ( latitude <= maxLat && latitude >= minLat );
  }
  catch (...)
  {
    return false;
  }
}

#ifdef _WIN32
#pragma warning( default : 4291 ) // Enable warning messages for new(nothrow)
#endif

