#ifndef LAMBERT_AZ_H
#define LAMBERT_AZ_H

#include "azimuthal.h"

class LambertAzimuthal : public Azimuthal
{
public:
	LambertAzimuthal();
	LambertAzimuthal(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:

	double m_sinCenterLat;
	double m_cosCenterLat;

	void _init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

};

#endif
