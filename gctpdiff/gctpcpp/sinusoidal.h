// $Id: sinusoidal.h,v 1.1 2005/08/27 00:20:31 rbuehler Exp $


#ifndef SINUSOIDAL_H
#define SINUSOIDAL_H

#include "projection.h"

//! This is the object used for the Sinusoidal projection.
class Sinusoidal : public Projection
{
public:
   Sinusoidal();
   
   Sinusoidal(double gctpParameters[], ProjUnit units, ProjDatum dat);
   
protected:

	//! See documentation for Projection
	void _init();

	//! See documentation for Projection
	void _forward(double lon, double lat);

	//! See documentation for Projection
	void _inverse(double x, double y);

};

#endif
