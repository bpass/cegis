
#ifndef MILLER_H
#define MILLER_H

#include "cylindrical.h"

class Miller : public Cylindrical
{
public:

	Miller();
	Miller(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:

	//! See documentation for Projection
	void init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

};

#endif