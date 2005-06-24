
#ifndef OBLATED_EQ_H
#define OBLATED_EQ_H

#include "projection.h"

class OblatedEqArea : public Projection
{
public:

	OblatedEqArea();
	OblatedEqArea(double gctpParams[], ProjUnit units, ProjDatum dat);

	void setShapeM(double m) {m_m = m; setInit();}

	void setShapeN(double n) {m_n = n; setInit();}

	void setAngle(double theta);

	void setCenterLon(double lon);

	void setCenterLat(double lat);

	double shapeM() {return m_m;}

	double shapeN() {return m_n;}

	double angle() {return m_theta;}

	double centerLon() {return m_centerLon;}

	double ceterLat() {return m_centerLat;}

protected:

	double m_theta;
	double m_m;
	double m_n;
	double m_sinLatO;
	double m_cosLatO;
	double m_centerLon;
	double m_centerLat;

	void _loadFromParams();

	void init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

};

#endif