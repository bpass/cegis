#ifndef CYLINDRICAL_H
#define CYLINDRICAL_H

#include "projection.h"

class Cylindrical : public Projection {
public:
	Cylindrical();

	Cylindrical(double gctpParams[], ProjUnit units, ProjDatum dat);

	void setCenterLon(double lon);

	void setCenterLat(double lat);

	double centerLat() {return m_centerLat;}

	double centerLon() {return m_centerLon;}

protected:

	double m_centerLon;

	double m_centerLat;

	virtual void _loadFromParams();

	virtual void init() = 0;
};

#endif