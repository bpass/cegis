// $Id: VanDerGrintenProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <strstream>
#include <new>
#include "VanDerGrintenProjection.h"

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif

VanDerGrintenProjection::VanDerGrintenProjection( 
                         double originLat, double sphereRadius, 
                         double centralMeridian, double falseEasting,
                         double falseNorthing, DATUM d, UNIT u,
                         DATUM geoDatum, UNIT geoUnit )
  : PseudocylindricalProjection(sphereRadius, centralMeridian, falseEasting,
                                falseNorthing, d, u, geoDatum, geoUnit )
{
  setOriginLatitude( originLat );
}

// ***************************************************************************
VanDerGrintenProjection::VanDerGrintenProjection( const VanDerGrintenProjection& p )
  : PseudocylindricalProjection(p)
{
  setOriginLatitude( p.getOriginLatitude() );
}


// ***************************************************************************
bool VanDerGrintenProjection::operator==( const Projection& rhs ) const throw()
{
  const VanDerGrintenProjection* pRHS = NULL;

  try
  {
    pRHS = dynamic_cast<const VanDerGrintenProjection*>(&rhs);
    if ( NULL != pRHS )
    {
      if ( d_projParams[5] == pRHS->getOriginLatitude() )
      {
        return PseudocylindricalProjection::operator==(rhs);
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
Projection* VanDerGrintenProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) VanDerGrintenProjection( *this ) ))
    throw std::bad_alloc();

  return foo;
}


// ***************************************************************************
std::string VanDerGrintenProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string    ret;

  try
  {
    tempstream << std::string("Van der Grinten Projection") << std::endl
               << PseudocylindricalProjection::toString()   << std::endl
               << std::string("ORIGIN LATITUDE: ")
               << packedDMSToString( d_projParams[5], true ) << std::ends;
    ret = tempstream.str();
    tempstream.freeze(false);
    return ret;
  }
  catch (...)
  {
    return std::string("");
  }
}

#ifdef _WIN32
#pragma warning( default : 4291 ) // Enable warning messages for new(nothrow)
#endif

