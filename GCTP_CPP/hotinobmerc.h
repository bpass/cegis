#ifndef HOTIN_OB_MERC_H
#define HOTIN_OB_MERC_H

#include "cylindrical.h"

class HotineObMerc : public Cylindrical
{
public:

	HotineObMerc();
	HotineObMerc(double gctpParams[], ProjUnit units, ProjDatum dat);

	void setScaleFactor(double fac) {m_scaleFactor = fac; setInit();}
	void setAzimuth(double angle);
	void setLon1(double lon);
	void setLon2(double lon);
	void setLat1(double lat);
	void setLat2(double lat);
	void setMode(int mode) {m_mode = mode; setInit();}

	double scaleFactor() {return m_scaleFactor;}
	double azimuth() {return m_azimuth;}
	double lon1() {return m_lon1;}
	double lon2() {return m_lon2;}
	double lat1() {return m_lat1;}
	double lat2() {return m_lat2;}
	int mode() {return m_mode;}

protected:

	double m_azimuth;
	double m_scaleFactor;
	double m_e;
	double m_es;
	double m_sinCenterLat;
	double m_cosCenterLat;
	double m_bl;
	double m_al;
	double m_ts;
	double m_d;
	double m_el;
	double m_u;
	double m_singam;
	double m_cosgam;
	double m_sinaz;
	double m_cosaz;
	double m_lat1;
	double m_lat2;
	double m_lon1;
	double m_lon2;
	int m_mode;

	void _init();
	void _loadFromParams();

	void _inverse(double x, double y);
	void _forward(double lon, double lat);
};

#endif
