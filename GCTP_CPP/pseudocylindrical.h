
#ifndef PSEUDOCYL_H
#define PSEUDOCYL_H

#include "projection.h"
class Pseudocylindrical : public Projection 
{
public:
	Pseudocylindrical();

	Pseudocylindrical(double gctpParams[], ProjUnit units, ProjDatum dat);

	void setCenterLon(double lon);

	void setCenterLat(double lat);

	double centerLat() {return m_centerLat;}

	double centerLon() {return m_centerLon;}

protected:

	double m_centerLat;
	
	double m_centerLon;

	//! See documentation for Projection
	virtual void init() = 0;

	virtual void loadFromParams();
};

#endif
