
#include "albersConEqArea.h"
#include "projexception.h"
#include <math.h>
#include <stdio.h>

AlbersConEqArea::AlbersConEqArea()
: Projection(), m_c(0.0), m_e3(0.0), m_es(0.0),
 m_rh(0.0), m_ns0(0.0), m_lon_center(0.0), m_lat_center(0.0),
m_stdParallelLat1(0.0), m_stdParallelLat2(0.0)
{
	setName("Albers Conical Equal-Area");
	setNumber(ALBERS);
}

AlbersConEqArea::AlbersConEqArea(double gctpParameters[15], int units, int datum, int spheroid)
: Projection(gctpParameters, units, datum, spheroid), m_c(0.0), m_e3(0.0), m_es(0.0),
 m_rh(0.0), m_ns0(0.0), m_lon_center(0.0), m_lat_center(0.0), m_stdParallelLat1(0.0), m_stdParallelLat2(0.0) 
{
	setName("Albers Conical Equal-Area");
	setNumber(ALBERS);
	setCenterLon(m_gctpParams[4]);
	setCenterLat(m_gctpParams[5]);
	setStdParallel1(m_gctpParams[2]);
	setStdParallel2(m_gctpParams[3]);
	return;
}

long AlbersConEqArea::inverse_init() {
	double sin_po,cos_po;		/* sine and cos values			*/
	double con;					/* temporary variable			*/
	double temp;				/* temporary variable			*/
	double ms1;   		        /* small m 1                            */
	double ms2;            		/* small m 2                            */
	double qs0;            		/* small q 0                            */
	double qs1;            		/* small q 1                            */
	double qs2;            		/* small q 2                            */

	long tempErr = 0;

	if (fabs(m_stdParallelLat1 + m_stdParallelLat2) < EPSLN)
	{
	printf("Equal latitudes for Standard Parallels on opposite sides of equator in alber-invinit");
	return(31);
   }
	temp = m_rMinor / m_rMajor;
	m_es = 1.0 - (temp*temp);
	m_e3 = sqrt(m_es);

	Util::gctp_sincos(m_stdParallelLat1, &sin_po, &cos_po);
	con = sin_po;

	ms1 = Util::msfnz(m_e3,sin_po, cos_po);
	qs1 = Util::qsfnz(m_e3,sin_po);

	Util::gctp_sincos(m_stdParallelLat2,&sin_po,&cos_po);

	ms2 = Util::msfnz(m_e3,sin_po, cos_po);
	qs2 = Util::qsfnz(m_e3,sin_po);

	Util::gctp_sincos(m_lat_center,&sin_po,&cos_po);

	qs0 = Util::qsfnz(m_e3,sin_po);

	if (fabs(m_stdParallelLat1 - m_stdParallelLat2) > EPSLN)
		m_ns0 = (ms1 * ms1 - ms2 *ms2)/ (qs2 - qs1);
	else
		m_ns0 = con;
	m_c = ms1 * ms1 + m_ns0 * qs1;
	m_rh = m_rMajor * sqrt(m_c - m_ns0 * qs0)/m_ns0;

	/* Report parameters to the user
	-----------------------------*/
	printf("ALBERS CONICAL EQUAL-AREA\n");
	printf("Sphere SMajor: %f   SMinor: %f\n", m_rMajor, m_rMinor);
	printf("Standard Parallel -- Lat1: %f    Lat2:   %f\n", m_stdParallelLat1, m_stdParallelLat2);
	printf("Central Meridian Longitude: %f\n", m_lon_center);
	printf("Origin Latitude: %f\n", m_lat_center);
	printf( "False Easting = %f\n", m_falseEasting );
	printf( "False Northing = %f\n", m_falseNorthing );
	m_invInitNeeded = false;
	return(OK);
}

long AlbersConEqArea::forward_init() {
	double sin_po,cos_po;		/* sin and cos values			*/
	double con;					/* temporary variable			*/
	double temp;				/* eccentricity squared and temp var	*/
	double ms1;					/* small m 1				*/
	double ms2;					/* small m 2				*/
	double qs0;					/* small q 0				*/
	double qs1;					/* small q 1				*/
	double qs2;					/* small q 2				*/
	long tempErr = 0;

	if (fabs(m_stdParallelLat1 + m_stdParallelLat2) < EPSLN)
	{
	  printf("Equal latitudes for St. Parallels on opposite sides of equator in albers forward_init\n");
	  return(31);
   }

	temp = m_rMinor / m_rMajor;
	m_es = 1.0 - (temp*temp);
	m_e3 = sqrt(m_es);

	Util::gctp_sincos(m_stdParallelLat1, &sin_po, &cos_po);
	con = sin_po;

	ms1 = Util::msfnz(m_e3,sin_po,cos_po);
	qs1 = Util::qsfnz(m_e3,sin_po);

	Util::gctp_sincos(m_stdParallelLat2,&sin_po,&cos_po);

	ms2 = Util::msfnz(m_e3,sin_po,cos_po);
	qs2 = Util::qsfnz(m_e3,sin_po);

	Util::gctp_sincos(m_lat_center, &sin_po,&cos_po);

	qs0 = Util::qsfnz(m_e3,sin_po);

	if (fabs(m_stdParallelLat1 - m_stdParallelLat2) > EPSLN)
		m_ns0 = (ms1 * ms1 - ms2 *ms2)/ (qs2 - qs1);
	else
		m_ns0 = con;
	m_c = ms1 * ms1 + m_ns0 * qs1;
	m_rh = m_rMajor * sqrt(m_c - m_ns0 * qs0)/m_ns0;

	/* Report parameters to the user
	-----------------------------*/
	printf("ALBERS CONICAL EQUAL-AREA\n");
	printf("Sphere SMajor: %f   SMinor: %f\n", m_rMajor, m_rMinor);
	printf("Standard Parallel -- Lat1: %f    Lat2:   %f\n", m_stdParallelLat1,m_stdParallelLat2);
	printf("Central Meridian Longitude: %f\n", m_lon_center);
	printf("Origin Latitude: %f\n", m_lat_center);
	printf( "False Easting = %f\n", m_falseEasting );
	printf( "False Northing = %f\n", m_falseNorthing );
	m_forInitNeeded = false;
	return(OK);
}

long AlbersConEqArea::inverse(double x, double y, double* lon, double* lat) {
	double rh1;			/* height above ellipsoid	*/
	double qs;			/* function q			*/
	double con;			/* temporary sign value		*/
	double theta;			/* angle			*/
	long   flag;			/* error flag;			*/

	if(m_invInitNeeded)
		inverse_init();
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
	
	if (m_e3 >= 1e-10)
	{
		con = 1 - .5 * (1.0 - m_es) * log((1.0 - m_e3) / (1.0 + m_e3))/m_e3;
		if (fabs(fabs(con) - fabs(qs)) > .0000000001 )
		{
			m_latitude = Util::phi1z(m_e3,qs,&flag);
			if (flag != 0)
				return(flag);
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
		m_latitude = Util::phi1z(m_e3,qs,&flag);
		if (flag != 0)
			return(flag);
    }

	m_longitude = Util::adjust_lon(theta/m_ns0 + m_lon_center);

	if(lat)
		*lat = m_latitude;
	if(lon)
		*lon = m_longitude;

	return(OK);
}

long AlbersConEqArea::forward(double lon, double lat, double* x, double* y) {
	double sin_phi,cos_phi;		/* sine and cos values		*/
	double qs;			/* small q			*/
	double theta;			/* angle			*/ 
	double rh1;			/* height above ellipsoid	*/

	if(m_forInitNeeded)
		forward_init();

	//convert lat/lon from dec degrees to radians
	Util::convertCoords(DEGREE, RADIAN, lat, lon);

	Util::gctp_sincos(lat,&sin_phi,&cos_phi);
	qs = Util::qsfnz(m_e3,sin_phi);
	rh1 = m_rMajor * sqrt(m_c - m_ns0 * qs)/m_ns0;
	theta = m_ns0 * Util::adjust_lon(lon - m_lon_center); 
	m_x_coord = rh1 * sin(theta) + m_falseEasting;
	m_y_coord = m_rh - rh1 * cos(theta) + m_falseNorthing;

	if(x)
		*x = m_x_coord;
	if(y)
		*y = m_y_coord;

return(OK);
}

void AlbersConEqArea::setCenterLat(double lat) {
	long tempErr = 0;
	double tempNum = Util::paksz(lat, &tempErr) * 3600 * S2R;
	if(tempErr !=0)
		throw(ProjException(tempErr, "AlbersConEqArea::setCenterLat()"));

	m_lat_center = tempNum;
	m_forInitNeeded = true;
	m_invInitNeeded = true;

}

void AlbersConEqArea::setCenterLon(double lon) {
	long tempErr = 0;
	double tempNum = Util::paksz(lon, &tempErr) * 3600 * S2R;
	if(tempErr !=0)
		throw(ProjException(tempErr, "AlbersConEqArea::setCenterLon()"));

	m_lon_center = tempNum;
	m_forInitNeeded = true;
	m_invInitNeeded = true;

}

void AlbersConEqArea::setStdParallel1(double lat1) {
	long tempErr = 0;
	double tempNum = Util::paksz(lat1, &tempErr) * 3600 * S2R;
	if(tempErr !=0)
		throw(ProjException(tempErr, "AlbersConEqArea::setStdParallel1()"));

	m_stdParallelLat1 = tempNum;
	m_forInitNeeded = true;
	m_invInitNeeded = true;

}

void AlbersConEqArea::setStdParallel2(double lat2) {
	long tempErr = 0;
	double tempNum = Util::paksz(lat2, &tempErr) * 3600 * S2R;
	if(tempErr !=0)
		throw(ProjException(tempErr, "AlbersConEqArea::setStdParallel2()"));

	m_stdParallelLat2 = tempNum;
	m_forInitNeeded = true;
	m_invInitNeeded = true;

}






