#ifndef SINUSOIDAL_H
#define SINUSOIDAL_H

#include "pseudocylindrical.h"


class Sinusoidal : public Pseudocylindrical
{
public:
   Sinusoidal();
   
   Sinusoidal(double gctpParameters[], ProjUnit units, ProjDatum dat);
   
protected:

	//! See documentation for Projection
	void _init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

  

};

#endif
