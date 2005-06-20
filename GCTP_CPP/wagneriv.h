#ifndef WAGNERIV_H
#define WAGNERIV_H

#include "azimuthal.h"

class WagnerIV : public Azimuthal
{
public:

	WagnerIV();
	WagnerIV(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:

	void init();

	void _inverse(double x, double y);
	void _forward(double lon, double lat);

};

#endif


