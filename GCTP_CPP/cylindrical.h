#ifndef CYLINDRICAL_H
#define CYLINDRICAL_H

#include "projection.h"

class Cylindrical : public Projection {
public:
	Cylindrical();

	Cylindrical(double gctpParams[], int units = 0, long speroid = 0, long datum = 0);

	virtual void forward(double lon, double lat, double* x, double* y) = 0;

	virtual void inverse(double x, double y, double* lon, double* lat) = 0;

	void setCenterLon(double lon);

	void setCenterLat(double lat);

	double centerLat() {return m_centerLat;}

	double centerLon() {return m_centerLon;}

protected:

	double m_centerLon;

	double m_centerLat;

	virtual void loadFromParams();

	virtual void inverse_init() = 0;

	virtual void forward_init() = 0;
};

#endif