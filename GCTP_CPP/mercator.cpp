
#include "mercator.h"
#include "projexception.h"

Mercator::Mercator(): Projection(), m_centerLat(0.0), m_centerLon(0.0), m_e(0.0),
m_es(0.0), m_m1(0.0)
{
	setName("Mercator");
	setNumber(MERCAT);
}

Mercator::Mercator(double gctpParams[], int units, long datum, long spheroid):
	Projection(gctpParams, units, datum, spheroid), m_centerLat(0.0), m_centerLon(0.0), m_e(0.0),
m_es(0.0), m_m1(0.0)
{
		setName("Mercator");
		setNumber(MERCAT);
		setCenterLon(m_gctpParams[4]);
		setCenterLat(m_gctpParams[5]);
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
//	double sin_phii;	/* sin value					*/
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

void Mercator::setCenterLat(double lat) {
	long err = 0;
	double temp = 0;	
	temp = Util::paksz(lat,&err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Mercator::setCenterLat()"));

	m_centerLat = temp;
	setInit();
}

void Mercator::setCenterLon(double lon) {
	long err = 0;
	double temp = 0;	
	temp = Util::paksz(lon,&err) * 3600 * S2R;
	if(err != 0)
		throw(ProjException(err, "Mercator::setCenterLon()"));

	m_centerLon = temp;
	setInit();
}











