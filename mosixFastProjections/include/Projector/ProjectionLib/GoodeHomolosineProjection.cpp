//impelemtation for the Goode's Homolosine Projection Class

#ifndef _GOODEHOMOLOSINEPROJECTION_CPP_
#define _GOODEHOMOLOSINEPROJECTION_CPP_



#include <strstream>
#include <new>
#include "GoodeHomolosineProjection.h"


using namespace ProjLib;

#ifdef _WIN32
#pragma warning( disable : 4291 ) // Disable warning messages for new(nothrow)
#endif


//*********************************************************************************
GoodeHomolosineProjection::GoodeHomolosineProjection(double sphereRadius, DATUM d, UNIT u,
			                  DATUM geoDatum, UNIT geoUnit):
PseudocylindricalProjection(sphereRadius, 0.0, 0.0, 0.0, d, u, geoDatum, geoUnit)
{
}

//**************************************************************************************
GoodeHomolosineProjection::GoodeHomolosineProjection(const GoodeHomolosineProjection & rhs)
: PseudocylindricalProjection(rhs)
{
}

//******************************************************************************************
Projection * GoodeHomolosineProjection::clone() const throw(std::bad_alloc)
{
	Projection * temp = NULL;

	if (!(temp = new (std::nothrow) GoodeHomolosineProjection(*this)))
		throw std::bad_alloc();

	return temp;
}

//******************************************************************************************
std::string GoodeHomolosineProjection::toString() const throw()
{
  std::strstream tempstream;
  std::string    ret;

  try
  {
    tempstream << std::string("Goode's Homolosine Projection") << std::endl
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

#endif