
#ifndef GEN_VERT_H
#define GEN_VERT_H

#include "projection.h"

class GenVertNSP : public Projection
{
public:

	GenVertNSP();
	GenVertNSP(double gctpParams[], ProjUnit units, ProjDatum dat);

	void setCenterLon(double lon);
	void setCenterLat(double lat);
	void setHeight(double height) {m_height = height; setInit();}

protected:

	double m_centerLon;
	double m_centerLat;
	double m_p;
	double m_height;
	double m_sinCenterLat;
	double m_cosCenterLat;

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

	void init();

	void loadFromParams();
};

#endif

