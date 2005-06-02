#ifndef CONIC_H
#define CONIC_H
#include "projection.h"

//! Base class for all conic projections.
class Conic : public Projection {
public:

	//! See documentation for Projection
	Conic();

	//! See documentation for Projection
	Conic(double gctpParams[], int units = 0, long datum = 0, long spheroid = 0);
	
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

	//! Get the c constanst.
	double c() {return m_c;}

	//! Get the eccentricity.
	double e() {return m_e;}

	//! Get the eccentricity squared.
	double es() {return m_es;}

	//! Get the height above the elipsoid
	double rh() {return m_rh;}

	//! Get the ratio between the meridians
	double ns0() {return m_ns0;}

	//! Get the center longitude for the projection.
	double centerLon() {return m_centerLon;}

	//! Get the center latitude for the projection.
	double centerLat() {return m_centerLat;}

	//! Get the latitude of the first standard parallel.
	double stdParallel1() {return m_stdParallelLat1;}

	//! Get the latitude of the second standard parallel.
	double stdParallel2() {return m_stdParallelLat2;}

protected:

	//! See documentation for Projection.
	virtual void inverse_init() = 0;

	//! See documentation for Projection
	virtual void forward_init() = 0;

	virtual void loadFromParams();

	//! Constant C.
	double m_c;

	//! eccentricity.
	double m_e;

	//! eccentricity squared.
	double m_es;

	//! height above elipsoid.
	double m_rh;

	//! Ratio between meridians.
	double m_ns0;

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