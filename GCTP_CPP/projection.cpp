#include "projection.h"

double Projection::getX()
{
  return x_coord;
}

double Projection::getY( )
{
  return y_coord;
}

double Projection::getLat()
{
  return latitude;
}

double Projection::getLon()
{
  return longitude;
}

long Projection::getXY ( double* x, double* y )
{
  if( x != NULL )
  {
     *x = x_coord;
  }
  
  if( y != NULL )
  {
     *y = y_coord;
  }
  
  return 0;
}

long Projection::getLatLon ( double* lat, double* lon )
{
  if( lat != NULL )
  {
     *lat = latitude;
  }
  
  if( lon != NULL )
  {
     *lon = longitude;
  }
  
  return 0;
}

Projection::Projection ( double gctpParameters[15] )
{
  for( int index = 0; index < 15; index++ )
  {
     gctpParams[index] = gctpParameters[index];
  }
  
  return; 
}

