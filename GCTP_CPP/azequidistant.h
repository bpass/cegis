
#ifndef AZ_EQ_H
#define AZ_EQ_H

#include "azimuthal.h"

class AzEquidistant : public Azimuthal
{
public:

	AzEquidistant();
	AzEquidistant(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:

	double m_sinCenterLat;
	
	double m_cosCenterLat;

	void init();
	
	void _forward(double lon, double lat);
	void _inverse(double x, double y);

};

#endif