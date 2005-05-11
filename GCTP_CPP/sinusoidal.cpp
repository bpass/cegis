#include "sinusoidal.h"
#include "projexception.h"

#include <stdio.h>
#include <math.h>



Sinusoidal::Sinusoidal( double gctpParameters[15], int units, int datum, int spheroid) 
: Projection( gctpParameters, units, datum, spheroid )
{
  setName("Sinusoidal");
  setNumber(SNSOID);
  setCenterLon(m_gctpParams[4]);
  return;
}

long Sinusoidal::forward ( double lon, double lat, double* x, double* y )
{
  double deltaLon;	/* Delta longitude (Given longitude - center */


  /* Forward equations */
  deltaLon = Util::adjust_lon(lon - m_centerLongitude);

  m_x_coord = m_radius * deltaLon * cos( lat ) + m_falseEasting;
  m_y_coord = m_radius * lat + m_falseNorthing;

  if( x != NULL )
  {
     *x = m_x_coord;
  }

  if( y != NULL )
  {
     *y = m_y_coord;
  }

  return 0;
}

long Sinusoidal::inverse ( double x, double y, double* lon, double* lat )
{
  double temp;		/* Re-used temporary variable */

  /* Inverse equations */

  prepCoords(x, y);
  
  x -= m_falseEasting;
  y -= m_falseNorthing;

  m_latitude = y / m_radius;

  if( fabs( m_latitude ) > HALF_PI)
  {
     fprintf( stderr, "Input data error in sinusoidal-inverse\n" );
     return(164);
  }

  temp = fabs( m_latitude ) - HALF_PI;
  if( fabs( temp ) > EPSLN )
  {
     temp = m_centerLongitude + x / (m_radius * cos( m_latitude ));
	 m_longitude = Util::adjust_lon( temp );
  }
  else
  {
     m_longitude = m_centerLongitude;
  }

  if( lon != NULL )
  {
     *lon = m_longitude;
  }

  if( lat != NULL )
  {
     *lat = m_latitude;
  }

  return 0;
}

long Sinusoidal::forward_init (  )
{
  
  printf( "SINUSOIDAL\n" );
  printf( "Radius = %f\n", m_radius );
  printf( "Center Longitude = %f\n", m_centerLongitude );
  printf( "False Easting = %f\n", m_falseEasting );
  printf( "False Northing = %f\n", m_falseNorthing );

  return 0;
}

long Sinusoidal::inverse_init (  )
{
	long tempErr = 0;

  m_radius = m_gctpParams[0];
  //from inv_init.c
  m_centerLongitude = Util::paksz(m_gctpParams[4], &tempErr) * 3600 * S2R;
  if(tempErr != 0)
	  return(tempErr);

  m_falseEasting = m_gctpParams[6];
  m_falseNorthing = m_gctpParams[7];

  printf( "SINUSOIDAL\n" );
  printf( "Radius = %f\n", m_radius );
  printf( "Center Longitude = %f\n", m_centerLongitude );
  printf( "False Easting = %f\n", m_falseEasting );
  printf( "False Northing = %f\n", m_falseNorthing );
  
  return 0;
}

void Sinusoidal::setCenterLon(double centerLon) {
	long err = 0;
	m_centerLongitude = Util::paksz(centerLon, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Sinusoidal::setCenterLon()"));
}


