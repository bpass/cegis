#ifndef TRANS_MERC_H
#define TRANS_MERC_H
#include "cylindrical.h"

class TransverseMercator : public Cylindrical 
{
public:
	TransverseMercator();
	TransverseMercator(double gctpParams[], ProjUnit units, ProjDatum datum);

	void forward(double lon, double lat, double* x = NULL, double* y = NULL);

	void inverse(double x, double y, double* lon = NULL, double* lat = NULL);

	void setScaleFactor(double scale) {m_scaleFactor = scale; setInit();}

	double scaleFactor() {return m_scaleFactor;}

protected:

	//! See documentation for Projection
	void init();

	void loadFromParams();

	double m_scaleFactor;

	double m_e0;
	double m_e1;
	double m_e2;
	double m_e3;
	double m_e;
	double m_es;
	double m_esp;
	double m_ml0;
	double m_ind;
};

#endif

	