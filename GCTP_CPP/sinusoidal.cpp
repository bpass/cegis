#include "sinusoidal.h"

#include <stdio.h>
#include <math.h>


Sinusoidal::Sinusoidal(): Pseudocylindrical()
{
	setName("Sinusoidal");
	setNumber(SNSOID);
}

Sinusoidal::Sinusoidal( double gctpParameters[15], ProjUnit units, ProjDatum dat) 
: Pseudocylindrical( gctpParameters, units, dat )
{
  setName("Sinusoidal");
  setNumber(SNSOID);
}

void Sinusoidal::forward ( double lon, double lat, double* x, double* y )
{
  double deltaLon;	/* Delta longitude (Given longitude - center */
  
  clearError();

  if(m_initNeeded)
	init();

  //convert lat/lon from dec degrees to radians
  Util::convertCoords(DEGREE, RADIAN, lat, lon);

  /* Forward equations */
  deltaLon = Util::adjust_lon(lon - m_centerLon);

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
  
  clearError();
  
  if(m_initNeeded)
	init();
  //convert lat/lon from dec degrees to radians
  Util::convertCoords(m_unitCode, METER, x, y);

  x -= m_falseEasting;
  y -= m_falseNorthing;

  m_latitude = y / m_radius;

  if( fabs( m_latitude ) > HALF_PI) {
		setError(164);
		return;
  }


  temp = fabs( m_latitude ) - HALF_PI;
  if( fabs( temp ) > EPSLN )
  {
     temp = m_centerLon + x / (m_radius * cos( m_latitude ));
	 m_longitude = Util::adjust_lon( temp );
  }
  else
  {
     m_longitude = m_centerLon;
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

void Sinusoidal::init(  )
{
  clearError();
  m_initNeeded = false;
}




