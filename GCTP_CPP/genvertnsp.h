
#ifndef GEN_VERT_H
#define GEN_VERT_H

#include "azimuthal.h"

class GenVertNSP : public Azimuthal
{
public:

	GenVertNSP();
	GenVertNSP(double gctpParams[], ProjUnit units, ProjDatum dat);

	void setHeight(double height) {m_height = height; setInit();}

	double height() {return m_height;}

protected:

	double m_centerLon;
	double m_centerLat;
	double m_p;
	double m_height;
	double m_sinCenterLat;
	double m_cosCenterLat;

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

	void _init();

	void _loadFromParams();
};

#endif

