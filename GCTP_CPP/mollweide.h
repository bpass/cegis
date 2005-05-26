#ifndef MOLLWEIDE_H
#define MOLLWEIDE_H
#include "projection.h"

class Mollweide: public Projection {
public:
	Mollweide();
	Mollweide(double gctpParams[],  int units = 0, long datum = 0, long spheroid = 0);
	void forward(double lon, double lat, double* x=NULL, double* y=NULL);
	void inverse(double x, double y, double* lon=NULL, double* lat=NULL);
	void setCenterLon(double lon);
protected:
	void forward_init();
	void inverse_init();
	double m_centerLon;
};

#endif