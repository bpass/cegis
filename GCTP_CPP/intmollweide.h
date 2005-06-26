
#ifndef INTMOLLWEIDE_H
#define INTMOLLWEIDE_H

#include "pseudocylindrical.h"

class IntMollweide : public Pseudocylindrical
{
public:
	IntMollweide();
	IntMollweide(double gctpParams[], ProjUnit units, ProjDatum dat);


protected:
	
	double m_centerLons[6];

	double m_falseEastings[6];

	//! See documentation for Projection
	void _init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

};

#endif
