// $Id: RobinsonProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <strstream>
#include <new>
#include "RobinsonProjection.h"

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif

// ***************************************************************************
RobinsonProjection::RobinsonProjection( double sphereRadius,
                                        double centralMeridian,
                                        double falseEasting,
                                        double falseNorthing,
                                        DATUM d, UNIT u, DATUM geoDatum,
                                        UNIT geoUnit )
  : PseudocylindricalProjection(sphereRadius, centralMeridian, falseEasting,
                                falseNorthing, d, u, geoDatum, geoUnit )
{
}


// ***************************************************************************
RobinsonProjection::RobinsonProjection( const RobinsonProjection& p )
  : PseudocylindricalProjection(p)
{
}


// ***************************************************************************
Projection* RobinsonProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) RobinsonProjection( *this ) ))
    throw std::bad_alloc();

  return foo;
}


// ***************************************************************************
std::string RobinsonProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string    ret;

  try
  {
    tempstream << std::string("Robinson Projection") << std::endl
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

