
#ifndef PSEUDOCYL_H
#define PSEUDOCYL_H

#include "projection.h"
class Pseudocylindrical : public Projection 
{
public:
	Pseudocylindrical();

	Pseudocylindrical(double gctpParams[], int units, long datum = 0, long spheroid = 0);

	virtual void forward(double lon, double lat, double* x = NULL, double* y = NULL) = 0;

	virtual void inverse(double x, double y, double* lon = NULL, double* lat = NULL) = 0;

	void setCenterLon(double lon);

	void setCenterLat(double lat);

	double centerLat() {return m_centerLat;}

	double centerLon() {return m_centerLon;}

protected:

	double m_centerLat;
	
	double m_centerLon;

	virtual void inverse_init() = 0;

	virtual void forward_init() = 0;

	virtual void loadFromParams();
};

#endif
