
#ifndef POLYCON_H
#define POLYCON_H
#include "conic.h"

class Polyconic: public Conic {
public:

	Polyconic();
	
	Polyconic(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:

	/* eccentricity constansts */
	double m_e;
	double m_es;
	double m_e0;
	double m_e1;
	double m_e2;
	double m_e3;
	double m_ml0; //small value m;

	//! See documentation for Projection
	void init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

};

#endif