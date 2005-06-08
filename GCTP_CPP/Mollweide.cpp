#include "mollweide.h"
#include "projexception.h"

Mollweide::Mollweide(): Pseudocylindrical() 
{
	setName("Mollweide");
	setNumber(MOLL);
}

Mollweide::Mollweide(double gctpParams[], ProjUnit units, ProjDatum dat):
Pseudocylindrical(gctpParams, units, dat) 
{
	setName("Mollweide");
	setNumber(MOLL);
}

void Mollweide::forward_init() 
{
	clearError();

	m_forInitNeeded = false;
}

void Mollweide::inverse_init() 
{
	clearError();

	m_invInitNeeded = false;
}

void Mollweide::forward(double lon, double lat, double* x, double* y) 
{

  double delta_lon;	/* Delta longitude (Given longitude - center */
  double theta;
  double delta_theta;
  double con;
  long i;

  clearError();

  if(m_forInitNeeded)
	  forward_init();

  Util::convertCoords(DEGREE, RADIAN, lon, lat);
  
  /* Forward equations
  -----------------*/
  delta_lon = Util::adjust_lon(lon - m_centerLon);
  theta = lat;
  con = PI * sin(lat);

 /* Iterate using the Newton-Raphson method to find theta
  -----------------------------------------------------*/
  for (i=0;;i++)
	{
	delta_theta = -(theta + sin(theta) - con)/ (1.0 + cos(theta));
	theta += delta_theta;
	if (fabs(delta_theta) < EPSLN) break;
	if (i >= 50) {
		setError(241);
		return;
	}
	  
	}
	theta /= 2.0;

	/*	If the latitude is 90 deg, force the x coordinate to be "0 + false easting"
	this is done here because of precision problems with "cos(theta)"
	 --------------------------------------------------------------------------*/
	if (PI/2 - fabs(lat) < EPSLN)
		delta_lon =0;
	
	m_x_coord = 0.900316316158 * m_radius * delta_lon * cos(theta) + m_falseEasting;
	m_y_coord = 1.4142135623731 * m_radius * sin(theta) + m_falseNorthing;

	Util::convertCoords(METER, m_unitCode, m_x_coord, m_y_coord);

	if(x)
		*x = m_x_coord;
	if(y)
		*y = m_y_coord;
}

void Mollweide::inverse(double x, double y, double* lon, double* lat) 
{
  double theta;
  double arg;

  clearError();

  if(m_invInitNeeded)
	  inverse_init();

  Util::convertCoords(m_unitCode, METER, x, y);

  /* Inverse equations
   -----------------*/
  x -= m_falseEasting; 
  y -= m_falseNorthing;
  arg = y /  (1.4142135623731 * m_radius);

  /* Because of division by zero problems, 'arg' can not be 1.0.  Therefore
     a number very close to one is used instead.
     -------------------------------------------------------------------*/
  if(fabs(arg) > 0.999999999999) 
	  arg=0.999999999999;
  theta = asin(arg);
  m_longitude = Util::adjust_lon(m_centerLon + (x / (0.900316316158 * m_radius * cos(theta))));
  if(m_longitude < (-PI)) 
	  m_longitude = -PI;
  if(m_longitude > PI) 
	  m_longitude = PI;
  arg = (2.0 * theta + sin(2.0 * theta)) / PI;
  if(fabs(arg) > 1.0)arg=1.0;
  m_latitude = asin(arg);

  Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);

  if(lat)
	  *lat = m_latitude;
  if(lon)
	  *lon = m_longitude;

}

