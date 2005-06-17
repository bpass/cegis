
#ifndef ORTHO_H
#define ORTHO_H

#include "azimuthal.h"

class Orthographic : public Azimuthal
{
public:
	Orthographic();
	Orthographic(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:

	double m_sinCenterLat;
	double m_cosCenterLat;

	void init();
	void _inverse(double x, double y);
	void _forward(double lon, double lat);

};

#endif
