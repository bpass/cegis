// $Id: EquirectangularProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <new>
#include <strstream>
#include "EquirectangularProjection.h"

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif

// ***************************************************************************
EquirectangularProjection::EquirectangularProjection( double trueScaleLat,
                                                      double sphereRadius, 
                                                      double centralMeridian,
                                                      double falseEasting,
                                                      double falseNorthing,
                                                      DATUM d, UNIT u,
                                                      DATUM geoDatum,
                                                      UNIT geoUnit )
  : PseudocylindricalProjection(sphereRadius, centralMeridian, falseEasting,
                                falseNorthing, d, u, geoDatum, geoUnit )
{
  setTrueScaleLatitude( trueScaleLat );
}


// ***************************************************************************
EquirectangularProjection::EquirectangularProjection( const EquirectangularProjection& p )
  : PseudocylindricalProjection(p)
{
  setTrueScaleLatitude( p.getTrueScaleLatitude() );
}


// ***************************************************************************
bool EquirectangularProjection::operator==( const Projection& rhs ) 
  const throw()
{
  const EquirectangularProjection* pRHS = NULL;

  try
  {
    pRHS = dynamic_cast<const EquirectangularProjection*>(&rhs);
    if ( NULL != pRHS )
    {
      if ( d_projParams[5]  == pRHS->getTrueScaleLatitude() )
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
Projection* EquirectangularProjection::clone() const throw (std::bad_alloc)
{
  Projection* foo = NULL;
  
  if (!(foo = new(std::nothrow) EquirectangularProjection(*this)))
    throw std::bad_alloc();

  return foo;
}


// ***************************************************************************
std::string EquirectangularProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string    ret;

  try
  {
    tempstream << std::string("Equirectangular Projection") << std::endl
               << PseudocylindricalProjection::toString() << std::endl
               << std::string("TRUE SCALE LATITUDE: ")
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
