#ifndef CONIC_H
#define CONIC_H
#include "projection.h"

//! Base class for all conic projections.
class Conic : public Projection {
public:

	//! See documentation for Projection
	Conic();

	//! See documentation for Projection
	Conic(double gctpParams[], ProjUnit units, ProjDatum dat);
	
	//! See documentation for Projection
	virtual void forward(double lon, double lat, double*x = NULL, double*y = NULL) = 0;
		
	//! See documentation for Projection
	virtual void inverse(double x, double y, double* lon = NULL, double* lat = NULL) = 0;

	//! Set the center longitude for this projection.
	void setCenterLon(double lon);

	//! Set the center latitude for this projection.
	void setCenterLat(double lat);

	//! Set the latitude of the first standard parallel.
	void setStdParallel1(double lat);

	//! Set the latitude for the second standard parallel.
	void setStdParallel2(double lat);

	//! Get the center longitude for the projection.
	double centerLon() {return m_centerLon;}

	//! Get the center latitude for the projection.
	double centerLat() {return m_centerLat;}

	//! Get the latitude of the first standard parallel.
	double stdParallel1() {return m_stdParallelLat1;}

	//! Get the latitude of the second standard parallel.
	double stdParallel2() {return m_stdParallelLat2;}

protected:

	//! See documentation for Projection
	virtual void init() = 0;

	virtual void loadFromParams();

	//! Center longitude of projection.
	double m_centerLon;

	//! Center latitude of projection.
	double m_centerLat;

	//! Latitude of first standard parallel.
	double m_stdParallelLat1;

	//! Latitude of second standard parallel.
	double m_stdParallelLat2;
};

#endif