#include <stdio.h>

#include "equirectangular.h"

Equirectangular::Equirectangular( double gctpParameters[15] ) : Projection( gctpParameters )
{

  return;
}

long Equirectangular::forward( double lon, double lat, double* x, double* y )
{
  double deltaLon;		/* delta longitude value			*/

  longitude = lon;
  latitude = lat;

  /* Forward equations */
  deltaLon = adjust_lon( longitude - centerLongitude );
  x_coord = falseEasting + earthRadius * deltaLon * cos( centerLatitude );
  y_coord = falseNorthing + earthRadius * latitude;

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

long Equirectangular::inverse ( double x, double y, double* lon, double* lat )
{
//  double deltaLon;	        /* delta longitude value			*/

  x_coord = x;
  y_coord = y;

  /* Inverse equations */
  x_coord -= falseEasting;
  y_coord -= falseNorthing;

  latitude = y_coord / earthRadius;

  if( fabs( latitude ) > HALF_PI )
  {
     fprintf( stderr, "Input data error in equi-inv\n" );
     return(174);
  }

  longitude = adjust_lon( centerLongitude + x_coord / ( earthRadius * cos( centerLongitude )));
  
  
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

long Equirectangular::forward_init()
{
  /* Place parameters in static storage for common use */
  earthRadius = gctpParams[0];
  centerLongitude = gctpParams[4];
  centerLatitude = gctpParams[5];
  falseNorthing = gctpParams[6];
  falseEasting = gctpParams[7];

  /* Report parameters to the user */
  printf( "EQUIRECTANGULAR\n" );
  printf( "Radius = %f\n", earthRadius );
  printf( "Center Meridian = %f\n", centerLongitude );
  printf( "Latitude of True Scale = %f\n", centerLatitude );
  printf( "False Easting = %f\n", falseEasting );
  printf( "False Northing = %f\n", falseNorthing );

  return 0;
}

long Equirectangular::inverse_init()
{
  /* Place parameters in static storage for common use */
  earthRadius = gctpParams[0];
  centerLongitude = gctpParams[4];
  centerLatitude = gctpParams[5];
  falseNorthing = gctpParams[6];
  falseEasting = gctpParams[7];

  /* Report parameters to the user */
  printf( "EQUIRECTANGULAR\n" );
  printf( "Radius = %f\n", earthRadius );
  printf( "Center Meridian = %f\n", centerLongitude );
  printf( "Latitude of True Scale = %f\n", centerLatitude );
  printf( "False Easting = %f\n", falseEasting );
  printf( "False Northing = %f\n", falseNorthing );

  return 0;
}


