// $Id: coordinate.cpp,v 1.1 2005/08/27 00:20:31 rbuehler Exp $


#include "coordinate.h"

Coordinate::Coordinate()
: x(0.0), y(0.0), units(UNDEF)
{
}

Coordinate::Coordinate( double xx, double yy, ProjUnit uunits )
: x(xx), y(yy), units(uunits)
{
}

Coordinate::Coordinate( const Coordinate &c )
: x(c.x), y(c.y), units(c.units)
{
}

void Coordinate::copy( const Coordinate &c )
{
   x = c.x;
   y = c.y;
   units = c.units;
}
