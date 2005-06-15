
#ifndef POLARS_H
#define POLARS_H

#include "azimuthal.h"

class PolarStereo : public Azimuthal
{
public:

	PolarStereo();
	PolarStereo(double gctpParams[], ProjUnit units, ProjDatum dat);

	void forward(double lon, double lat, double* x = NULL, double* y = NULL);

	void inverse(double x, double y, double* lon = NULL, double* lat = NULL);

protected:
	
	double m_es;
	double m_e;
	double m_e4;
	double m_fac;
	double m_ind;
	double m_mcs;
	double m_tcs;

	//! See documentation for Projection
	void init();

};

#endif