
#ifndef LAMBERT_CC_H
#define LAMBERT_CC_H
#include "conic.h"

class LambertCC : public Conic 
{
public:
	LambertCC();
	LambertCC(double gctpParams[], ProjUnit units, ProjDatum dat);

	void forward(double lon, double lat, double* x = NULL, double* y = NULL);
	void inverse(double x, double y, double* lat = NULL, double* lon = NULL);

protected:
	void inverse_init();
	void forward_init();
	
	double m_es;
	double m_e;
	double m_ns;
	double m_f0;
	double m_rh;
};
#endif
