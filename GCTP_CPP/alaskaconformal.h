
#ifndef ALASKA_CON_H
#define ALASKA_CON_H

#include "projection.h"

class AlaskaConformal : public Projection
{
public:

	AlaskaConformal();
	AlaskaConformal(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:

	void init();

	void _inverse(double x, double y);
	void _forward(double lon, double lat);

	double m_centerLon;
	double m_centerLat;
	double m_acoef[7];
	double m_bcoef[7];
	double m_sinCenterLat;
	double m_cosCenterLat;
	double m_e;
	int m_n;
};

#endif
