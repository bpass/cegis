#include <stdio.h>

#include "equirectangular.h"

Equirectangular::Equirectangular() : Cylindrical()
{
	setName("Equirectangular");
	setNumber(EQRECT);
}

Equirectangular::Equirectangular( double gctpParameters[15], ProjUnit units, ProjDatum dat) 
: Cylindrical(gctpParameters, units, dat)
{
	setName("Equirectangular");
	setNumber(EQRECT);
}

void Equirectangular::forward( double lon, double lat, double* x, double* y )
{
  double deltaLon;		/* delta longitude value			*/
	
  clearError();

  if(m_initNeeded)
		init();

  //convert lat/lon from dec degrees to radians
  Util::convertCoords(DEGREE, RADIAN, lat, lon);

  /* Forward equations */
  deltaLon = Util::adjust_lon( lon - m_centerLon );
  m_x_coord = m_falseEasting + m_radius * deltaLon * cos( m_centerLat );
  m_y_coord = m_falseNorthing + m_radius * lat;

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

void Equirectangular::inverse ( double x, double y, double* lon, double* lat )
{
  
  clearError();

  if(m_initNeeded)
	init();

  //convert lat/lon from dec degrees to radians
  Util::convertCoords(m_unitCode, METER, x, y);

  /* Inverse equations */
  x -= m_falseEasting;
  y -= m_falseNorthing;

  m_latitude = y / m_radius;

  if( fabs( m_latitude ) > HALF_PI ) {
	setError(174);
	return;
  }

  m_longitude = Util::adjust_lon( m_centerLon + x / ( m_radius * cos( m_centerLon )));

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

void Equirectangular::init()
{
  clearError();

  m_initNeeded = false;
}






