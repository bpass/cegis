#include "sinusoidal.h"

#include <math.h>

void Sinusoidal::Sinusoidal( double gctpParameters[15] ) : Projection( gctpParameters )
{

  return;
}

long Sinusoidal::forward ( double lon, double lat, double* x = null, double* y = null )
{
  double deltaLon;	/* Delta longitude (Given longitude - center */

  longitude = lon;
  latitude = lat;

  /* Forward equations */
  delta_lon = adjust_lon(longitude - centerLongitude);

  x_coord = earthRadius * deltaLon * cos( latitude ) + falseEasting;
  y_coord = earthRadius * latitude + falseNorthing;

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

long Sinusoidal::inverse ( double x, double y, double* lon = null, double* lat = null )
{
  double temp;		/* Re-used temporary variable */

  /* Inverse equations */

  x_coord = x;
  y_coord = y;

  x_coord -= falseEasting;
  y_coord -= falseNorthing;

  latitude = y_coord / earthRadius;

  if( fabs( latitude ) > HALF_PI)
  {
     fprintf( stderr, "Input data error in sinusoidal-inverse\n" );
     return(164);
  }

  temp = fabs( latitude ) - HALF_PI;
  if( fabs( temp ) > EPSLN )
  {
     temp = centerLongitude + x_coord / (earthRadius * cos( latitude ));
     lonitude = adjust_lon( temp );
  }
  else
  {
     longitude = centerLongitude;
  }

  if( lon != NULL )
  {
     *lon = longitude;
  }

  if( lat != NULL )
  {
     *lat = latitude;
  }

  return 0;
}

long Sinusoidal::forward_init (  )
{
  earthRadius = gctpParams[0];
  centerLongitude = gctpParams[4];
  falseEasting = gctpParams[6];
  falseEorthing = gctpParams[7];

  printf( "SINUSOIDAL\n" );
  printf( "Radius = %f\n", earthRadius );
  printf( "Center Longitude = %f\n", centerLongitude );
  printf( "False Easting = %f\n", falseEasting );
  printf( "False Northing = %f\n", falseNorthing );

  return 0;
}

long Sinusoidal::inverse_init (  )
{
  earthRadius = gctpParams[0];
  centerLongitude = gctpParams[4];
  falseEasting = gctpParams[6];
  falseEorthing = gctpParams[7];

  printf( "SINUSOIDAL\n" );
  printf( "Radius = %f\n", earthRadius );
  printf( "Center Longitude = %f\n", centerLongitude );
  printf( "False Easting = %f\n", falseEasting );
  printf( "False Northing = %f\n", falseNorthing );

  return 0;
}


