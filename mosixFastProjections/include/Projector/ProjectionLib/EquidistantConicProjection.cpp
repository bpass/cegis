// $Id: EquidistantConicProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <new>
#include <strstream>
#include "EquidistantConicProjection.h"

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif

// ***************************************************************************
EquidistantConicProjection::EquidistantConicProjection( double stdpar, 
                                                        double sMajor,
                                                        double sMinor, 
                                                        double centMer,
                                                        double originLat,
                                                        double fe, double fn, 
                                                        DATUM d, UNIT u,
                                                        DATUM geoDatum,
                                                        UNIT geoUnit )
  : ConicProjection( sMajor, sMinor, centMer, originLat, fe, fn, d, u, 
                     geoDatum, geoUnit )
{
  setStandardParallel( stdpar );
  d_projParams[8] = 0;
}


// ***************************************************************************
EquidistantConicProjection::EquidistantConicProjection( double stdpr1,
                                                        double stdpr2, 
                                                        double sMajor,
                                                        double sMinor, 
                                                        double centMer,
                                                        double originLat,
                                                        double fe, double fn, 
                                                        DATUM d, UNIT u,
                                                        DATUM geoDatum,
                                                        UNIT geoUnit )
  : ConicProjection( sMajor, sMinor, centMer, originLat, fe, fn, d, u,
                     geoDatum, geoUnit )
{
  setFirstStandardParallel( stdpr1 );
  setSecondStandardParallel( stdpr2 );
  d_projParams[8] = 1;
}


// ***************************************************************************
EquidistantConicProjection::EquidistantConicProjection( const EquidistantConicProjection& p )
  : ConicProjection(p)
{
  if ( this != &p )
  {
    setFirstStandardParallel( p.getFirstStandardParallel() );
    setSecondStandardParallel( p.getSecondStandardParallel() );
    setStandardParallel( p.getStandardParallel() );
    d_projParams[8] = p.d_projParams[8];
  }
}


// ***************************************************************************
bool EquidistantConicProjection::operator==( const Projection& p ) 
  const throw()
{
  const EquidistantConicProjection* pRHS = NULL; 
  try
  {
    // Check to see if the projection passed in is Equidistant Conic
    if ( EQUIDC == p.getProjectionSystem() )
    {
      pRHS = dynamic_cast<const EquidistantConicProjection*>(&p);
      if ( NULL != pRHS )
      {
        if ( d_projParams[2]  == pRHS->getFirstStandardParallel()  &&
             d_projParams[3]  == pRHS->getSecondStandardParallel() &&
             d_projParams[2]  == pRHS->getStandardParallel()       &&
             d_projParams[8]  == pRHS->d_projParams[8] )
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


// ***************************************************************************
Projection* EquidistantConicProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;
  
  if (!(foo = new(std::nothrow) EquidistantConicProjection(*this)))
    throw std::bad_alloc();

  return foo;
}


// ***************************************************************************
std::string EquidistantConicProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string    ret;

  try
  {
    tempstream << std::string("Equidistant Conic Projection") << std::endl;
  
    if ( 0 == d_projParams[8] )
    {
      tempstream << std::string("STANDARD PARALLEL: ")
                 << packedDMSToString(d_projParams[2], true );
    }
    else
    {
      tempstream << std::string("FIRST STANDARD PARALLEL: ")
                 << packedDMSToString(d_projParams[2], true )
                 << std::endl << std::string("SECOND STANDARD PARALLEL: ")
                 << packedDMSToString(d_projParams[3], true );
    }
    tempstream << std::ends;
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
bool EquidistantConicProjection::checkCoordinateRange( double latitude,
                                                       double longitude ) 
  const throw()
{
  if ( !Projection::checkCoordinateRange( latitude, longitude ) )
  {
    return false;
  }
  return true;
  /*
    double deltaN = ( 90.0 - packedDMSToDouble( getSecondStandardParallel() ) ) / 20.0;
    double deltaS = ( packedDMSToDouble( getFirstStandardParallel() ) + 90.0 ) / 20.0;
    double maxLat = 90.0 - deltaN * deltaN;
    double minLat = -90 + deltaS * deltaS;  
    return ( latitude <= maxLat && latitude >= minLat );
    */
}


#ifdef _WIN32
#pragma warning( default : 4291 ) // Enable warning messages for new(nothrow)
#endif
