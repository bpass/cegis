
#ifndef LAMBERT_CC_H
#define LAMBERT_CC_H
#include "conic.h"

class LambertCC : public Conic 
{
public:
	LambertCC();
	LambertCC(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:
	//! See documentation for Projection
	void init();
	
	void _forward(double lon, double lat);
	void _inverse(double x, double y);

	double m_es;
	double m_e;
	double m_ns;
	double m_f0;
	double m_rh;
};
#endif
