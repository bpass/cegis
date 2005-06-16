#ifndef MOLLWEIDE_H
#define MOLLWEIDE_H

#include "pseudocylindrical.h"

class Mollweide: public Pseudocylindrical {
public:
	Mollweide();
	
	Mollweide(double gctpParams[],  ProjUnit units, ProjDatum dat);
		
protected:

	//! See documentation for Projection
	void init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

};

#endif