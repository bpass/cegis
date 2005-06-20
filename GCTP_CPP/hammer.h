
#ifndef HAMMER_H
#define HAMMER_H

#include "azimuthal.h"

class Hammer: public Azimuthal
{
public:
	Hammer();
	Hammer(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:

	void init();

	void _inverse(double x, double y);
	void _forward(double lon, double lat);
};

#endif