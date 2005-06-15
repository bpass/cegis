
#include "albersConEqArea.h"
#include <math.h>
#include <stdio.h>

AlbersConEqArea::AlbersConEqArea()
: Conic(), m_c(0.0), m_e(0.0), m_es(0.0), m_rh(0.0), m_ns0(0.0)
{
	setName("Albers Conical Equal-Area");
	setNumber(ALBERS);

}

AlbersConEqArea::AlbersConEqArea(double gctpParameters[15], ProjUnit units, ProjDatum dat)
: Conic(gctpParameters, units, dat), m_c(0.0), m_e(0.0), m_es(0.0), m_rh(0.0), m_ns0(0.0)
{
	setName("Albers Conical Equal-Area");
	setNumber(ALBERS);
}

<<<<<<< albersConEqArea.cpp
void AlbersConEqArea::init() {
=======


void AlbersConEqArea::inverse_init() {
	double sin_po,cos_po;		/* sine and cos values			*/
	double con;					/* temporary variable			*/
	double temp;				/* temporary variable			*/
	double ms1;   		        /* small m 1                            */
	double ms2;            		/* small m 2                            */
	double qs0;            		/* small q 0                            */
	double qs1;            		/* small q 1                            */
	double qs2;            		/* small q 2                            */
	
	clearError();

	if (fabs(m_stdParallelLat1 + m_stdParallelLat2) < EPSLN) {
		setError(31);
		return;
	}

	temp = m_rMinor / m_rMajor;
	m_es = 1.0 - (temp*temp);
	m_e = sqrt(m_es);

	Util::gctp_sincos(m_stdParallelLat1, &sin_po, &cos_po);
	con = sin_po;

	ms1 = Util::msfnz(m_e,sin_po, cos_po);
	qs1 = Util::qsfnz(m_e,sin_po);

	Util::gctp_sincos(m_stdParallelLat2,&sin_po,&cos_po);

	ms2 = Util::msfnz(m_e,sin_po, cos_po);
	qs2 = Util::qsfnz(m_e,sin_po);

	Util::gctp_sincos(m_centerLat,&sin_po,&cos_po);

	qs0 = Util::qsfnz(m_e,sin_po);

	if (fabs(m_stdParallelLat1 - m_stdParallelLat2) > EPSLN)
		m_ns0 = (ms1 * ms1 - ms2 *ms2)/ (qs2 - qs1);

	else
		m_ns0 = con;

	m_c = ms1 * ms1 + m_ns0 * qs1;
	m_rh = m_rMajor * sqrt(m_c - m_ns0 * qs0)/m_ns0;

	m_invInitNeeded = false;
}

void AlbersConEqArea::forward_init() {
>>>>>>> 1.12
	double sin_po,cos_po;		/* sin and cos values			*/
	double con;					/* temporary variable			*/
	double temp;				/* eccentricity squared and temp var	*/
	double ms1;					/* small m 1				*/
	double ms2;					/* small m 2				*/
	double qs0;					/* small q 0				*/
	double qs1;					/* small q 1				*/
	double qs2;					/* small q 2				*/

	clearError();

	if (fabs(m_stdParallelLat1 + m_stdParallelLat2) < EPSLN) {
		setError(31);
		return;
	}
    
	temp = m_rMinor / m_rMajor;
	m_es = 1.0 - (temp*temp);
	m_e = sqrt(m_es);

	Util::gctp_sincos(m_stdParallelLat1, &sin_po, &cos_po);
	con = sin_po;

	ms1 = Util::msfnz(m_e,sin_po,cos_po);
	qs1 = Util::qsfnz(m_e,sin_po);

	Util::gctp_sincos(m_stdParallelLat2,&sin_po,&cos_po);

	ms2 = Util::msfnz(m_e,sin_po,cos_po);
	qs2 = Util::qsfnz(m_e,sin_po);

	Util::gctp_sincos(m_centerLat, &sin_po,&cos_po);

	qs0 = Util::qsfnz(m_e,sin_po);

	if (fabs(m_stdParallelLat1 - m_stdParallelLat2) > EPSLN)
		m_ns0 = (ms1 * ms1 - ms2 *ms2)/ (qs2 - qs1);

	else
		m_ns0 = con;

	m_c = ms1 * ms1 + m_ns0 * qs1;
	m_rh = m_rMajor * sqrt(m_c - m_ns0 * qs0)/m_ns0;

	m_initNeeded = false;
}

void AlbersConEqArea::inverse(double x, double y, double* lon, double* lat) {
	double rh1;			/* height above ellipsoid	*/
	double qs;			/* function q			*/
	double con;			/* temporary sign value		*/
	double theta;			/* angle			*/
	long   flag;			/* error flag;			*/

	clearError();

	if(m_initNeeded)
		init();
	
	//convert coordinates to meters
	Util::convertCoords(m_unitCode, METER, x, y);

	flag = 0;
	x -= m_falseEasting;
	y = m_rh - y + m_falseNorthing;;
	
	if (m_ns0 >= 0)
	{
		rh1 = sqrt(x * x + y * y);
		con = 1.0;
    }
	
	else
	{
		rh1 = -sqrt(x * x + y * y);
		con = -1.0;
	}

	theta = 0.0;
	if (rh1 != 0.0)
		theta = atan2(con * x, con * y);
	
	con = rh1 * m_ns0 / m_rMajor;
	qs = (m_c - con * con) / m_ns0;
	
	if (m_e >= 1e-10)
	{
		con = 1 - .5 * (1.0 - m_es) * log((1.0 - m_e) / (1.0 + m_e))/m_e;
		if (fabs(fabs(con) - fabs(qs)) > .0000000001 )
		{
			m_latitude = Util::phi1z(m_e,qs,&flag);
			if (flag != 0) {
				setError(flag);
				return;
			}
		}

		else
	
		{
		 if (qs >= 0)
			m_latitude = .5 * PI;
		 else
			m_latitude = -.5 * PI;
		}
	}
	
	else
	{
		m_latitude = Util::phi1z(m_e,qs,&flag);
		if (flag != 0) {
			setError(flag);
			return;
		}
    }

	m_longitude = Util::adjust_lon(theta/m_ns0 + m_centerLon);

	Util::convertCoords(RADIAN, DEGREE, m_longitude, m_latitude);

	if(lat)
		*lat = m_latitude;
	if(lon)
		*lon = m_longitude;

}

void AlbersConEqArea::forward(double lon, double lat, double* x, double* y) {

	double sin_phi,cos_phi;		/* sine and cos values		*/
	double qs;			/* small q			*/
	double theta;			/* angle			*/ 
	double rh1;			/* height above ellipsoid	*/

	clearError();

	if(m_initNeeded)
		init();

	//convert lat/lon from dec degrees to radians
	Util::convertCoords(DEGREE, RADIAN, lat, lon);

	Util::gctp_sincos(lat,&sin_phi,&cos_phi);
	qs = Util::qsfnz(m_e,sin_phi);
	rh1 = m_rMajor * sqrt(m_c - m_ns0 * qs)/m_ns0;
	theta = m_ns0 * Util::adjust_lon(lon - m_centerLon); 
	m_x_coord = rh1 * sin(theta) + m_falseEasting;
	m_y_coord = m_rh - rh1 * cos(theta) + m_falseNorthing;

	Util::convertCoords(METER, m_unitCode, m_x_coord, m_y_coord);

	if(x)
		*x = m_x_coord;
	if(y)
		*y = m_y_coord;
	

}







