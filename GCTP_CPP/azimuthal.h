#ifndef AZIMUTH_H
#define AZIMUTH_H

#include "projection.h"

class Azimuthal : public Projection
{
public:
	Azimuthal();
	Azimuthal(double gctpParams[], ProjUnit units, ProjDatum dat);

	virtual void forward(double lon, double lat, double* x = NULL, double* y = NULL) = 0;
	virtual void inverse(double x, double y, double* lon = NULL, double* lat = NULL) = 0;

	void setCenterLon(double lon);
	void setCenterLat(double lat);

	double centerLon() {return m_centerLon;}

	double centerLat() {return m_centerLat;}

protected:

	double m_centerLon;
	double m_centerLat;

	virtual void loadFromParams();

	virtual void init() = 0;
};

#endif