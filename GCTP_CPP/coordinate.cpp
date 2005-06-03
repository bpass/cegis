#include "coordinate.h"

Coordinate::Coordinate()
: x(0.0), y(0.0), units(0)
{
}

Coordinate::Coordinate( double sx, double yy, int uunits )
: x(sx), y(yy), units(uunits)
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
