#ifndef MERCATOR_H
#define MERCATOR_H
#include "projection.h"

class Mercator: public Projection {
public:
	Mercator();
	Mercator(double gctpParams[],  int units = 0, long datum = 0, long spheroid = 0);

	void forward(double lon, double lat, double* x = NULL, double* y = NULL);
	void inverse(double x, double y, double* lon = NULL, double* lat = NULL);
	void setCenterLon(double lon);
	void setCenterLat(double lat);
	
protected:

	void inverse_init();
	void forward_init();

	double m_centerLon;
	double m_centerLat;
	double m_e;
	double m_es;
	double m_m1;

};

#endif
	