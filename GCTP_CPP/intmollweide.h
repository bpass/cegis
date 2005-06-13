
#ifndef INTMOLLWEIDE_H
#define INTMOLLWEIDE_H

#include "pseudocylindrical.h"

class IntMollweide : public Pseudocylindrical
{
public:
	IntMollweide();
	IntMollweide(double gctpParams[], ProjUnit units, ProjDatum dat);

	void forward(double lon, double lat, double* x = NULL, double* y = NULL);
	void inverse(double x, double y, double* lon = NULL, double* lat = NULL);

protected:
	
	double m_centerLons[6];

	double m_falseEastings[6];

	void forward_init();
	void inverse_init();
};

#endif
