#ifndef MOLLWEIDE_H
#define MOLLWEIDE_H

#include "pseudocylindrical.h"

class Mollweide: public Pseudocylindrical {
public:
	Mollweide();
	
	Mollweide(double gctpParams[],  ProjUnit units, ProjDatum dat);
	
	void forward(double lon, double lat, double* x=NULL, double* y=NULL);
	
	void inverse(double x, double y, double* lon=NULL, double* lat=NULL);
	
protected:

	void forward_init();

	void inverse_init();
};

#endif