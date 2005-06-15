#ifndef MERCATOR_H
#define MERCATOR_H
#include "cylindrical.h"

class Mercator: public Cylindrical {
public:
	Mercator();

	Mercator(double gctpParams[],  ProjUnit units, ProjDatum dat);

	void forward(double lon, double lat, double* x = NULL, double* y = NULL);

	void inverse(double x, double y, double* lon = NULL, double* lat = NULL);
	
	double e() {return m_e;}

	double eSquared() {return m_es;}

	double m() {return m_m1;}

protected:

	double m_e;

	double m_es;

	double m_m1;

	//! See documentation for Projection
	void init();

};

#endif
	