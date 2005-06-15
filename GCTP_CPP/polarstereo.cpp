
#include "polarstereo.h"

PolarStereo::PolarStereo() : Azimuthal(),
m_es(0.0), m_e(0.0), m_e4(0.0), m_fac(0.0), m_ind(0.0),
m_mcs(0.0), m_tcs(0.0)
{
	setNumber(PS);
	setName("Polar Stereo");
}

PolarStereo::PolarStereo(double gctpParams[], ProjUnit units, ProjDatum dat):
Azimuthal(gctpParams, units, dat),
m_es(0.0), m_e(0.0), m_e4(0.0), m_fac(0.0), m_ind(0.0),
m_mcs(0.0), m_tcs(0.0)
{
	setNumber(PS);
	setName("Polar Stereo");
}

void PolarStereo::init()
{
	double temp;				/* temporary variable		*/
	double con1;				/* temporary angle		*/
	double sinphi;				/* sin value			*/
	double cosphi;				/* cos value			*/

	clearError();

	temp = m_rMinor / m_rMajor;
	m_es = 1.0 - SQUARE(temp);
	m_e = sqrt(m_es);
	m_e4 = Util::e4fn(m_e);

	if (m_centerLat < 0)
		m_fac = -1.0;
	else
		m_fac = 1.0;
	
	m_ind = 0;
	if (fabs(fabs(m_centerLat) - HALF_PI) > EPSLN)
	{
		m_ind = 1;
		con1 = m_fac * m_centerLat; 
		Util::gctp_sincos(con1,&sinphi,&cosphi);
		m_mcs = Util::msfnz(m_e,sinphi,cosphi);
		m_tcs = Util::tsfnz(m_e,con1,sinphi);
	}

	m_initNeeded = false;
}

void PolarStereo::forward(double lon, double lat, double* x, double* y)
{
	double con1;			/* adjusted longitude		*/
	double con2;			/* adjusted latitude		*/
	double rh;			/* height above ellipsoid	*/
	double sinphi;			/* sin value			*/
	double ts;			/* value of small t		*/

	clearError();
	
	if(m_initNeeded)
		init();

	Util::convertCoords(DEGREE, RADIAN, lon, lat);

	con1 = m_fac * Util::adjust_lon(lon - m_centerLon);
	con2 = m_fac * lat;
	sinphi = sin(con2);
	ts = Util::tsfnz(m_e,con2,sinphi);
	if (m_ind != 0)
		rh = m_rMajor * m_mcs * ts / m_tcs;
	else
		rh = 2.0 * m_rMajor * ts / m_e4;

	m_x_coord = m_fac * rh * sin(con1) + m_falseEasting;
	m_y_coord = -m_fac * rh * cos(con1) + m_falseEasting;

	Util::convertCoords(METER, m_unitCode, m_x_coord, m_y_coord);

	if(x)
		*x = m_x_coord;
	if(y)
		*y = m_y_coord;
}

void PolarStereo::inverse(double x, double y, double* lon, double* lat)
{
	double rh;			/* height above ellipsiod	*/
	double ts;			/* small value t		*/
	double temp;			/* temporary variable		*/
	long   flag;			/* error flag			*/

	clearError();
	if(m_initNeeded)
		init();

	Util::convertCoords(m_unitCode, METER, x, y);

	flag = 0;
	x = (x - m_falseEasting) * m_fac;
	y = (y - m_falseNorthing) * m_fac;
	rh = sqrt(x * x + y * y);
	if (m_ind != 0)
		ts = rh * m_tcs/(m_rMajor * m_mcs);
	else
		ts = rh * m_e4 / (m_rMajor * 2.0);

	m_latitude = m_fac * Util::phi2z(m_e,ts,&flag);

	if (flag != 0) {
		setError(flag);
		return;
	}
	
	if (rh == 0)
		m_longitude = m_fac * m_centerLon;
	else
	{
		temp = atan2(x, -y);
		m_longitude = Util::adjust_lon(m_fac *temp + m_centerLon);
	}

	Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);

	if(lon)
		*lon = m_longitude;

	if(lat)
		*lat = m_latitude;
}
