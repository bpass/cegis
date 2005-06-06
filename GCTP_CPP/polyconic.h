
#ifndef POLYCON_H
#define POLYCON_H
#include "conic.h"

class Polyconic: public Conic {
public:

	Polyconic();
	
	Polyconic(double gctpParams[], ProjUnit units, Datum dat);

	void forward(double lon, double lat, double* x = NULL, double* y = NULL);

	void inverse(double x, double y, double* lon = NULL, double* lat = NULL);

private:

	/* eccentricity constansts */
	double m_e0;
	double m_e1;
	double m_e2;
	double m_e3;
	
	double m_ml0; //small value m;

	void forward_init();

	void inverse_init();

};

#endif