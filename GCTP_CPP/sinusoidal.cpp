#include "sinusoidal.h"
#include "projexception.h"

#include <stdio.h>
#include <math.h>


Sinusoidal::Sinusoidal(): Projection(), m_centerLongitude(0.0)
{
	setName("Sinusoidal");
	setNumber(SNSOID);
}

Sinusoidal::Sinusoidal( double gctpParameters[15], int units, long datum, long spheroid) 
: Projection( gctpParameters, units, datum, spheroid )
{
  setName("Sinusoidal");
  setNumber(SNSOID);
  setCenterLon(m_gctpParams[4]);
  return;
}

void Sinusoidal::forward ( double lon, double lat, double* x, double* y )
{
  double deltaLon;	/* Delta longitude (Given longitude - center */
  
  if(m_forInitNeeded)
	  forward_init();

  //convert lat/lon from dec degrees to radians
  Util::convertCoords(DEGREE, RADIAN, lat, lon);

  /* Forward equations */
  deltaLon = Util::adjust_lon(lon - m_centerLongitude);

  m_x_coord = m_radius * deltaLon * cos( lat ) + m_falseEasting;
  m_y_coord = m_radius * lat + m_falseNorthing;

  Util::convertCoords(METER, m_unitCode, m_x_coord, m_y_coord);

  if( x != NULL )
  {
     *x = m_x_coord;
  }

  if( y != NULL )
  {
     *y = m_y_coord;
  }
}

void Sinusoidal::inverse ( double x, double y, double* lon, double* lat )
{
  double temp;		/* Re-used temporary variable */
  
  if(m_invInitNeeded)
	  inverse_init();
  //convert lat/lon from dec degrees to radians
  Util::convertCoords(m_unitCode, METER, x, y);

  x -= m_falseEasting;
  y -= m_falseNorthing;

  m_latitude = y / m_radius;

  if( fabs( m_latitude ) > HALF_PI)
	  throw(ProjException(164, "Sinusoidal::inverse()"));


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

  Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);

  if( lon != NULL )
  {
     *lon = m_longitude;
  }

  if( lat != NULL )
  {
     *lat = m_latitude;
  }

}

void Sinusoidal::forward_init (  )
{
  printf( "SINUSOIDAL\n" );
  printf( "Radius = %f\n", m_radius );
  printf( "Center Longitude = %f\n", m_centerLongitude );
  printf( "False Easting = %f\n", m_falseEasting );
  printf( "False Northing = %f\n", m_falseNorthing );
  m_forInitNeeded = false;
}

void Sinusoidal::inverse_init (  )
{

  printf( "SINUSOIDAL\n" );
  printf( "Radius = %f\n", m_radius );
  printf( "Center Longitude = %f\n", m_centerLongitude );
  printf( "False Easting = %f\n", m_falseEasting );
  printf( "False Northing = %f\n", m_falseNorthing );
  m_invInitNeeded = false;
}

void Sinusoidal::setCenterLon(double centerLon) {
	long err = 0;
	m_centerLongitude = Util::paksz(centerLon, &err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Sinusoidal::setCenterLon()"));
	setInit();
}


