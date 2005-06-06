
#ifndef EQUID_C_H
#define EQUID_C_H
#include "conic.h"
class EquidistantCA : public Conic
{
public:
	EquidistantCA();

	EquidistantCA(double gctpParams[], ProjUnit units, ProjDatum dat);

	void forward(double lon, double lat, double* x = NULL, double* y = NULL);

	void inverse(double x, double y, double* lon = NULL, double* lat = NULL);

private:
	
	double m_e;
	
	double m_es;
	
	double m_esp;
	
	double m_e0;
	
	double m_e1;
	
	double m_e2;
	
	double m_e3;
	
	double m_ml0;
	
	double m_ns;
	
	double m_g;
	
	double m_rh;

	void forward_init();

	void inverse_init();
};

#endif

