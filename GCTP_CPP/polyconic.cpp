
#include "polyconic.h"

Polyconic::Polyconic() : 
Conic(), m_e0(0.0), m_e1(0.0), m_e2(0.0), m_e3(0.0), m_ml0(0.0)
{
	setNumber(POLYC);
	setName("Polyconic");
}

Polyconic::Polyconic(double gctpParams[], ProjUnit units, ProjDatum dat):
Conic(gctpParams, units, dat),  m_e0(0.0), m_e1(0.0), m_e2(0.0), 
m_e3(0.0), m_ml0(0.0)
{
	setNumber(POLYC);
	setName("Polyconic");
}

void Polyconic::forward_init() 
{
	double temp;	

	clearError();
	temp = m_rMinor / m_rMajor;
	m_es = 1.0 - SQUARE(temp);
	m_e = sqrt(m_es);
	m_e0 = Util::e0fn(m_es);
	m_e1 = Util::e1fn(m_es);
	m_e2 = Util::e2fn(m_es);
	m_e3 = Util::e3fn(m_es);
	m_ml0 = Util::mlfn(m_e0, m_e1, m_e2, m_e3, m_centerLat);
}

void Polyconic::inverse_init() 
{
	double temp;		

	clearError();
	temp = m_rMinor / m_rMajor;
	m_es = 1.0 - SQUARE(temp);
	m_e = sqrt(m_es);
	m_e0 = Util::e0fn(m_es);
	m_e1 = Util::e1fn(m_es);
	m_e2 = Util::e2fn(m_es);
	m_e3 = Util::e3fn(m_es);
	m_ml0 = Util::mlfn(m_e0, m_e1, m_e2, m_e3, m_centerLat);
}

void Polyconic::forward(double lon, double lat, double* x, double* y) 
{
	double sinphi, cosphi;	/* sin and cos value				*/
	double con, ml;		/* cone constant, small m			*/
	double ms;		/* small m					*/

	clearError();
	if(m_forInitNeeded)
		forward_init();

	Util::convertCoords(DEGREE, RADIAN, lon, lat);
	/* Forward equations
	-----------------*/
	con = Util::adjust_lon(lon - m_centerLon);
	if (fabs(lat) <= .0000001)
	{
		m_x_coord = m_falseEasting + m_rMajor * con;
		m_y_coord = m_falseNorthing - m_rMajor * m_ml0;
	}
	else
	{
		Util::gctp_sincos(lat,&sinphi,&cosphi);
		ml = Util::mlfn(m_e0, m_e1, m_e2, m_e3, lat);
		ms = Util::msfnz(m_e,sinphi,cosphi);
		con *= sinphi;
		m_x_coord = m_falseEasting + m_rMajor * ms * sin(con)/sinphi;
		m_y_coord = m_falseNorthing + m_rMajor * (ml - m_ml0 + ms * (1.0 - cos(con))/sinphi);
	}

	Util::convertCoords(METER, m_unitCode, m_x_coord, m_y_coord);

	if(x)
		*x = m_x_coord;

	if(y)
		*y = m_y_coord;
}

void Polyconic::inverse(double x, double y, double* lon, double* lat)
{
	double al;		/* temporary values				*/
	double b;		/* temporary values				*/
	double c;		/* temporary values				*/
	long iflg;		/* error flag					*/

	clearError();

	if(m_invInitNeeded)
		inverse_init();

	Util::convertCoords(m_unitCode, METER, x, y);

	/* Inverse equations
	-----------------*/
	x -= m_falseEasting;
	y -= m_falseNorthing;
	al = m_ml0 + y/m_rMajor;
	iflg = 0;
	if (fabs(al) <= .0000001)
	{
		*lon = x/m_rMajor + m_centerLon;
		*lat = 0.0;
	}
	else
	{
		b = al * al + (x/m_rMajor) * (x/m_rMajor);
		iflg = Util::phi4z(m_es,m_e0,m_e1,m_e2,m_e3,al,b,&c,&m_latitude);

		if (iflg != OK) {
			setError(iflg);
			return;
		}

		m_longitude = Util::adjust_lon((Util::asinz(x * c / m_rMajor) / sin(m_latitude)) + m_centerLon);
	}

	Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);

	if(lon)
		*lon = m_longitude;

	if(lat)
		*lat = m_latitude;
}
