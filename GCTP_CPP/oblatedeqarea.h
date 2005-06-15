
#ifndef OBLATED_EQ_H
#define OBLATED_EQ_H

#include "projection.h"

class OblatedEqArea : public Projection
{
public:

	OblatedEqArea();
	OblatedEqArea(double gctpParams[], ProjUnit units, ProjDatum dat);

	void forward(double lon, double lat, double* x = NULL, double* y = NULL);

	void inverse(double x, double y, double* lon = NULL, double* lat = NULL);

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

	void loadFromParams();

	void init();
};

#endif