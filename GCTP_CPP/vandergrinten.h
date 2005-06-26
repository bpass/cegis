#ifndef VAND_H
#define VAND_H

#include "projection.h"

class VanDerGrinten : public Projection
{
public:
	VanDerGrinten();
	VanDerGrinten(double gctpParams[], ProjUnit units, ProjDatum dat);

	void setCenterLon(double lon);

	double centerLon() {return m_centerLon;}

protected:

	double m_centerLon;

	void _init();

	void _loadFromParams();

	void _inverse(double x, double y);
	void _forward(double lon, double lat);
};
#endif
