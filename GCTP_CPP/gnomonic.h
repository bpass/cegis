
#ifndef GNOMONIC_H
#define GNOMONIC_H
#include "azimuthal.h"

class Gnomonic : public Azimuthal
{
public:

	Gnomonic();
	Gnomonic(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:

	double m_sinCenterLat;
	double m_cosCenterLat;

	void _init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);
};

#endif