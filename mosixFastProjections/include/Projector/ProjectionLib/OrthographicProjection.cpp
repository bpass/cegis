// $Id: OrthographicProjection.cpp,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#include <strstream>
#include <new>
#include "OrthographicProjection.h"

using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif

// ***************************************************************************
OrthographicProjection::OrthographicProjection( double centralLongitude,
                                                double centralLatitude,
                                                double falseEasting,
                                                double falseNorthing,
                                                double sphereRadius, DATUM d,
                                                UNIT u,
                                                DATUM geoDatum, UNIT geoUnit )
  : AzimuthalProjection(centralLongitude, centralLatitude, falseEasting,
                        falseNorthing, sphereRadius, d, u, geoDatum, geoUnit )
{
  // Set bad radius
  d_badDegreeRadius = 65.0;
}


// ***************************************************************************
OrthographicProjection::OrthographicProjection(const OrthographicProjection& p)
  : AzimuthalProjection(p)
{
}


// ***************************************************************************
Projection* OrthographicProjection::clone() const throw(std::bad_alloc)
{
  Projection* foo = NULL;

  if (!(foo = new(std::nothrow) OrthographicProjection(*this) ))
    throw std::bad_alloc();

  return foo;
}


// ***************************************************************************
std::string OrthographicProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string    ret;

  try
  {
    tempstream << std::string("Orthographic Projection") << std::endl
               << AzimuthalProjection::toString() << std::ends;

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

