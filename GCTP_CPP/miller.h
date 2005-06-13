
#ifndef MILLER_H
#define MILLER_H

#include "cylindrical.h"

class Miller : public Cylindrical
{
public:

	Miller();
	Miller(double gctpParams[], ProjUnit units, ProjDatum dat);

	void forward(double lon, double lat, double* x = NULL, double* y = NULL);
	void inverse(double x, double y, double* lon = NULL, double* lat = NULL);

protected:

	void inverse_init();
	void forward_init();
};

#endif