// $Id: MillerCylindricalProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <strstream>
#include <new>
#include "MillerCylindricalProjection.h"

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif

// ***************************************************************************
MillerCylindricalProjection::MillerCylindricalProjection( 
                             double sphereRadius, double centralMeridian,
                             double falseEasting, double falseNorthing,
                             DATUM d, UNIT u, DATUM geoDatum, UNIT geoUnit )
  : PseudocylindricalProjection(sphereRadius, centralMeridian, falseEasting,
                                falseNorthing, d, u, geoDatum, geoUnit )
{
}


// ***************************************************************************
MillerCylindricalProjection::MillerCylindricalProjection( const MillerCylindricalProjection& p )
  : PseudocylindricalProjection(p)
{
}


// ***************************************************************************
Projection* MillerCylindricalProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) MillerCylindricalProjection(*this)))
    throw std::bad_alloc();
  
  return foo;
}


// ***************************************************************************
std::string MillerCylindricalProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string    ret;

  try
  {
    tempstream << std::string("Miller Cylindrical Projection") << std::endl
               << PseudocylindricalProjection::toString() << std::ends;

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
