
#ifndef POLARS_H
#define POLARS_H

#include "projection.h"

class PolarStereo : public Projection
{
public:

	PolarStereo();
	PolarStereo(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:
	
	double m_es;
	double m_e;
	double m_e4;
	double m_fac;
	double m_ind;
	double m_mcs;
	double m_tcs;

	//! See documentation for Projection
	void _init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

};

#endif