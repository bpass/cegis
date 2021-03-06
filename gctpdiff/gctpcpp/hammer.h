// $Id: hammer.h,v 1.1 2005/08/27 00:20:31 rbuehler Exp $


#ifndef HAMMER_H
#define HAMMER_H

#include "projection.h"

//!This is the object used for the Hammer projection.
class Hammer: public Projection
{
public:

	Hammer();
	Hammer(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:
	
	//!See documentation for Projection.
	void _init();

	//!See documentation for Projection.
	void _inverse(double x, double y);

	//!See documentation for Projection.
	void _forward(double lon, double lat);
};

#endif