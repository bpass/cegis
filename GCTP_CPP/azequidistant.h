
#ifndef AZ_EQ_H
#define AZ_EQ_H

#include "azimuthal.h"

class AzEquidistant : public Azimuthal
{
public:

	AzEquidistant();
	AzEquidistant(double gctpParams[], ProjUnit units, ProjDatum dat);

	//! See documentation for Projection.
	void forward(double lon, double lat, double* x = NULL, double*y = NULL);

	//! See documentation for Projection.
	void inverse(double x, double y, double* lat = NULL, double* lon = NULL);

protected:

	double m_sinCenterLat;
	
	double m_cosCenterLat;

	void init();
};

#endif