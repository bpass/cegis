
#ifndef UTM_H
#define UTM_H

#include "projection.h"

class UTM : public Projection
{
public:

	UTM();
	UTM(double gctpParams[], ProjUnit units, ProjDatum dat, int zone = 1);

	void setZone(int zone) {m_zone = zone;}

	void setScaleFactor(double factor) {m_scaleFactor = factor;}

	void setCenterLon(double lon);

	void setCenterLat(double lat);

	int zone() {return m_zone;}

	double scaleFactor() {return m_scaleFactor;}

	double centerLon() {return m_centerLon;}

	double centerLat() {return m_centerLat;}

protected:

	double m_scaleFactor;
	double m_centerLon;
	double m_centerLat;
	double m_e0;
	double m_e1;
	double m_e2;
	double m_e3;
	double m_e;
	double m_es;
	double m_esp;
	double m_ml0;
	double m_ind;
	int m_zone;

	void loadFromParams();

	void init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);
};

#endif
