
#include "miller.h"

Miller::Miller() : Cylindrical()
{
	setNumber(MILLER);
	setName("Miller Cylindrical");
}

Miller::Miller(double gctpParams[], ProjUnit units, ProjDatum dat):
Cylindrical(gctpParams, units, dat)
{
	setNumber(MILLER);
	setName("Miller Cylindrical");
}

void Miller::forward_init() 
{
	clearError();
	m_forInitNeeded = false;
}

void Miller::inverse_init()
{
	clearError();
	m_invInitNeeded = false;
}

void Miller::forward(double lon, double lat, double* x, double* y)
{
	double dlon;

	if(m_forInitNeeded)
		forward_init();
	
	clearError();

	Util::convertCoords(DEGREE, RADIAN, lon, lat);
	/* Forward equations
	-----------------*/
	dlon = Util::adjust_lon(lon - m_centerLon);
	m_x_coord = m_falseEasting + m_radius * dlon;
	m_y_coord = m_falseNorthing + m_radius * log(tan((PI / 4.0) + (lat / 2.5))) * 1.25;

	Util::convertCoords(METER, m_unitCode, m_x_coord, m_y_coord);

	if(x)
		*x = m_x_coord;
	if(y)
		*y = m_y_coord;
}

void Miller::inverse(double x, double y, double* lon, double* lat)
{

	Util::convertCoords(m_unitCode, METER, x, y);

	if(m_invInitNeeded)
		inverse_init();

	clearError();

	/* Inverse  equations
	------------------*/
	x -= m_falseEasting;
	y -= m_falseNorthing;

	m_longitude = Util::adjust_lon(m_centerLon + x / m_radius);
	m_latitude = 2.5 * (atan(exp(y / m_radius / 1.25)) - PI / 4.0);

	Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);

	if(lon)
		*lon = m_longitude;

	if(lat)
		*lat = m_latitude;
}

