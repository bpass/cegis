
#ifndef WAGNERVII_H
#define WAGNERVII_H

#include "azimuthal.h"

class WagnerVII : public Azimuthal
{
public:
	WagnerVII();
	WagnerVII(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:

	void _init();

	void _inverse(double x, double y);
	void _forward(double lon, double lat);
};

#endif
