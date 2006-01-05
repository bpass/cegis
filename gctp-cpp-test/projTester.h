
#ifndef PROJTESTER_H
#define PROJTESTER_H

#include "point.h"

class ProjTester 
{

public:

	virtual ~ProjTester() {}
	virtual Point doForward(const Point& geoP) = 0;
	virtual Point doInverse(const Point& projP) = 0;

};

#endif
