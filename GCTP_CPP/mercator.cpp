
#include "mercator.h"
#include "projexception.h"

Mercator::Mercator(): Cylindrical(), m_e(0.0),
m_es(0.0), m_m1(0.0)
{
	setName("Mercator");
	setNumber(MERCAT);
}

Mercator::Mercator(double gctpParams[], int units, long datum, long spheroid):
Cylindrical(gctpParams, units, datum, spheroid), m_e(0.0),
m_es(0.0), m_m1(0.0)
{
		setName("Mercator");
		setNumber(MERCAT);
}

void Mercator::inverse_init() {
	double temp;			/* temporary variable		*/

	temp = m_rMinor / m_rMajor;
	m_es = 1.0 - SQUARE(temp);
	m_e = sqrt(m_es);
	m_m1 = cos(m_centerLat)/(sqrt(1.0 - m_es * sin(m_centerLat) * sin(m_centerLat)));
	m_invInitNeeded = false;

}
void Mercator::inverse(double x, double y, double* lon, double* lat) 
{
	double ts;		/* small t value				*/
	long flag;		/* error flag 					*/
	
	if(m_invInitNeeded)
		inverse_init();
	
	Util::convertCoords(m_unitCode, METER, x, y);

	/* Inverse equations
	-----------------*/
	flag = 0;
	x -= m_falseEasting;
	y -= m_falseNorthing;
	ts = exp(-y/(m_rMajor * m_m1));
	m_latitude = Util::phi2z(m_e,ts,&flag);
	
	if (flag != 0)
		throw(ProjException(flag, "Mercator::inverse()"));

	m_longitude = Util::adjust_lon(m_centerLon + x/(m_rMajor * m_m1));

	Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);

	if(lon)
		*lon = m_longitude;

	if(lat)
		*lat = m_latitude;
}

void Mercator::forward_init() {
	double temp;			/* temporary variable		*/

	temp = m_rMinor / m_rMajor;
	m_es = 1.0 - SQUARE(temp);
	m_e = sqrt(m_es);
	m_m1 = cos(m_centerLat)/(sqrt(1.0 - m_es * sin(m_centerLat) * sin(m_centerLat)));
	m_forInitNeeded = false;
}

void Mercator::forward(double lon, double lat, double* x, double* y) {
	double ts;		/* small t value				*/
	double sinphi;		/* sin value					*/

	if(m_forInitNeeded)
		forward_init();

	Util::convertCoords(DEGREE, RADIAN, lon, lat);

	/* Forward equations
	 -----------------*/
	if (fabs(fabs(lat) - HALF_PI)  <= EPSLN)
		throw(ProjException(52, "Mercator::forward()"));
    
	else
	 {
		sinphi = sin(lat);
		ts = Util::tsfnz(m_e,lat,sinphi);
		m_x_coord = m_falseEasting + m_rMajor * m_m1 * Util::adjust_lon(lon - m_centerLon);
		m_y_coord = m_falseNorthing - m_rMajor * m_m1 * log(ts);
	 }

	 Util::convertCoords(METER, m_unitCode, m_x_coord, m_y_coord);

	 if(x)
		 *x = m_x_coord;
	 if(y)
		 *y = m_y_coord;
}













