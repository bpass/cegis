
#include "genvertnsp.h"

GenVertNSP::GenVertNSP() : Projection(), m_centerLon(0.0), m_centerLat(0.0),
m_p(0.0), m_sinCenterLat(0.0), m_cosCenterLat(0.0)
{
	setNumber(GVNSP);
	setName("General Vertical Near-Side Perspective");
}


GenVertNSP::GenVertNSP(double gctpParams[], ProjUnit units, ProjDatum dat) :
Projection(gctpParams, units, dat), m_centerLon(0.0), m_centerLat(0.0),
m_p(0.0), m_sinCenterLat(0.0), m_cosCenterLat(0.0)
{
	setNumber(GVNSP);
	setName("General Vertical Near-Side Perspective");
	loadFromParams();
}

void GenVertNSP::loadFromParams()
{
	Projection::loadFromParams();
	setCenterLon(m_gctpParams[4]);
	setCenterLat(m_gctpParams[5]);
	setHeight(m_gctpParams[2]);
}

void GenVertNSP::setCenterLat(double lat) 
{
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lat, &err) * 3600 * S2R;
	if(err != 0) {
		setError(err);
		return;
	}

	m_centerLat = temp;
	setInit();
}

void GenVertNSP::setCenterLon(double lon) 
{
	long err = 0;
	double temp = 0;

	temp = Util::paksz(lon, &err) * 3600 * S2R;
	if(err != 0) {
		setError(err);
		return;
	}

	m_centerLon = temp;
	setInit();
}

void GenVertNSP::init() 
{
	/* Place parameters in static storage for common use
	-------------------------------------------------*/
	m_p = 1.0 + m_height / m_radius;
	Util::gctp_sincos(m_centerLat, &m_sinCenterLat, &m_cosCenterLat);
}

void GenVertNSP::_forward(double lon, double lat)
{
	double dlon;
	double sinphi,cosphi;
	double coslon;
	double g;
	double ksp;

	/* Forward equations
	-----------------*/
	dlon = Util::adjust_lon(lon - m_centerLon);
	Util::gctp_sincos(lat,&sinphi,&cosphi);
	coslon = cos(dlon);
	g = m_sinCenterLat * sinphi + m_cosCenterLat * cosphi * coslon;
	if (g < (1.0/ m_p))
	{
		setError(153);
		return;
	}

	ksp = (m_p - 1.0)/(m_p - g);
	m_x_coord = m_falseEasting + m_radius * ksp * cosphi * sin(dlon);
	m_y_coord = m_falseNorthing + m_radius * ksp * (m_cosCenterLat * sinphi - m_sinCenterLat * cosphi * coslon);
}

void GenVertNSP::_inverse(double x, double y)
{
	double rh;
	double r;
	double con;
	double com;
	double z,sinz,cosz;


	/* Inverse equations
	-----------------*/
	x -= m_falseEasting;
	y -= m_falseNorthing;
	rh = sqrt(x * x + y * y);
	r  = rh / m_radius;
	con = m_p - 1.0;
	com = m_p + 1.0;
	if (r > sqrt(con/com))
	{
		setError(155);
		return;
	}
	sinz = (m_p - sqrt(1.0 - (r * r * com) / con)) / (con / r + r/con);
	z = Util::asinz(sinz);
	Util::gctp_sincos(z,&sinz,&cosz);
	m_longitude = m_centerLon;
	if (fabs(rh) <= EPSLN)
	{
		m_latitude = m_centerLat;
		return;
	}
	m_latitude = Util::asinz(cosz * m_sinCenterLat + ( y * sinz * m_cosCenterLat)/rh);
	con = fabs(m_centerLat) - HALF_PI;
	if (fabs(con) <= EPSLN)
	{
		if (m_centerLat >= 0.0)
		{
			m_longitude = Util::adjust_lon(m_centerLon + atan2(x, -y));
			return;
		}
		else
		{
			m_longitude = Util::adjust_lon(m_centerLon - atan2(-x, y));
			return;
		}
	}
	con = cosz - m_sinCenterLat * sin(m_latitude);
	if ((fabs(con) < EPSLN) && (fabs(x) < EPSLN))
		return;
	m_longitude = Util::adjust_lon(m_centerLon + atan2((x * sinz * m_cosCenterLat), (con * rh)));
}
