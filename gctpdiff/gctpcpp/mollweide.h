// $Id: mollweide.h,v 1.1 2005/08/27 00:20:31 rbuehler Exp $


#ifndef MOLLWEIDE_H
#define MOLLWEIDE_H

#include "projection.h"

//!This is the object used for the Mollweide projection.
class Mollweide: public Projection 
{
public:
	Mollweide();
	
	Mollweide(double gctpParams[],  ProjUnit units, ProjDatum dat);
		
protected:

	//! See documentation for Projection
	void _init();

	//! See documentation for Projection
	void _forward(double lon, double lat);

	//! See documentation for Projection
	void _inverse(double x, double y);

};

#endif