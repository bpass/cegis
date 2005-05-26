#include <stdio.h>

#include "equirectangular.h"
#include "projexception.h"

Equirectangular::Equirectangular( double gctpParameters[15], int units, long datum, long spheroid ) 
: Projection(gctpParameters, units, datum, spheroid), m_centerLongitude(0.0), m_centerLatitude(0.0)
{
	setName("Equirectangular");
	setNumber(EQRECT);
	setCenterLon(m_gctpParams[4]);
	setCenterLat(m_gctpParams[5]);

}

void Equirectangular::forward( double lon, double lat, double* x, double* y )
{
  double deltaLon;		/* delta longitude value			*/
	
  if(m_forInitNeeded)
	  forward_init();
  //convert lat/lon from dec degrees to radians
  Util::convertCoords(DEGREE, RADIAN, lat, lon);

  /* Forward equations */
  deltaLon = Util::adjust_lon( lon - m_centerLongitude );
  m_x_coord = m_falseEasting + m_radius * deltaLon * cos( m_centerLatitude );
  m_y_coord = m_falseNorthing + m_radius * lat;

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
  if(m_invInitNeeded)
	  inverse_init();
  //convert lat/lon from dec degrees to radians
  Util::convertCoords(m_unitCode, METER, x, y);


  /* Inverse equations */
  x -= m_falseEasting;
  y -= m_falseNorthing;

  m_latitude = y / m_radius;

  if( fabs( m_latitude ) > HALF_PI )
	  throw(ProjException(174, "Equirectangular::inverse()"));

  m_longitude = Util::adjust_lon( m_centerLongitude + x / ( m_radius * cos( m_centerLongitude )));
  
  
  if( lon != NULL )
  {
     *lon = m_longitude;
  }

  if( lat != NULL )
  {
     *lat = m_latitude;
  }
	 
  Util::convertCoords(RADIAN, DEGREE, *lon, *lat);

}

void Equirectangular::forward_init()
{
  /* Report parameters to the user */
  printf( "EQUIRECTANGULAR\n" );
  printf( "Radius = %f\n", m_radius );
  printf( "Center Meridian = %f\n", m_centerLongitude );
  printf( "Latitude of True Scale = %f\n", m_centerLatitude );
  printf( "False Easting = %f\n", m_falseEasting );
  printf( "False Northing = %f\n", m_falseNorthing );
  m_forInitNeeded = false;
}

void Equirectangular::inverse_init()
{

  /* Report parameters to the user */
  printf( "EQUIRECTANGULAR\n" );
  printf( "Radius = %f\n", m_radius );
  printf( "Center Meridian = %f\n", m_centerLongitude );
  printf( "Latitude of True Scale = %f\n", m_centerLatitude );
  printf( "False Easting = %f\n", m_falseEasting );
  printf( "False Northing = %f\n", m_falseNorthing );
  m_invInitNeeded = false;
}

void Equirectangular::setCenterLat(double lat) {
	long tempErr = 0;
	double tempNum = Util::paksz(lat, &tempErr) * 3600 * S2R;
	if(tempErr !=0)
		throw(ProjException(tempErr, "Equirectangular::setCenterLat()"));

	m_centerLatitude = tempNum;
setInit();
}

void Equirectangular::setCenterLon(double lon) {
	long tempErr = 0;
	double tempNum = Util::paksz(lon, &tempErr) * 3600 * S2R;
	if(tempErr !=0)
		throw(ProjException(tempErr, "Equirectangular::setCenterLat()"));

	m_centerLongitude = tempNum;
	setInit();

}


