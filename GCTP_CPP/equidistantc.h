
#ifndef EQUID_C_H
#define EQUID_C_H
#include "conic.h"
class EquidistantC : public Conic
{
public:
	EquidistantC();

	EquidistantC(double gctpParams[], ProjUnit units, ProjDatum dat);

	void setMode(int mode) {m_mode = mode; setInit();}

	int mode() {return m_mode;}
protected:
	
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

	int m_mode;
	
	//! See documentation for Projection
	void init();
	
	void loadFromParams();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

};

#endif

