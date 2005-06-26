
#ifndef GOODE_H
#define GOODE_H
#include "pseudocylindrical.h"

class GoodeH : public Pseudocylindrical
{
public:

	GoodeH();
	GoodeH(double gctpParams[], ProjUnit units, ProjDatum dat);

private:

	double m_centerLons[12];
	double m_falseEastings[12];

	void _init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);
};

#endif