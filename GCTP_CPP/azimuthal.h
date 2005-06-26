#ifndef AZIMUTH_H
#define AZIMUTH_H

#include "projection.h"

class Azimuthal : public Projection
{
public:
	Azimuthal();
	Azimuthal(double gctpParams[], ProjUnit units, ProjDatum dat);


	void setCenterLon(double lon);
	void setCenterLat(double lat);

	double centerLon() {return m_centerLon;}

	double centerLat() {return m_centerLat;}

protected:

	double m_centerLon;
	double m_centerLat;

	virtual void _loadFromParams();

};

#endif