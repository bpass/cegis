
#ifndef ROBINSON_H
#define ROBINSON_H

#include "pseudocylindrical.h"

class Robinson : public Pseudocylindrical
{
public:

	Robinson();
	Robinson(double gctpParams[], ProjUnit units, ProjDatum dat);

protected:
	
	double m_pr[21];
	double m_xlr[21];

	void init();

	void _inverse(double x, double y);
	void _forward(double lon, double lat);

};

#endif
