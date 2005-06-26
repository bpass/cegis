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
	
	//! Set the center longitude for this projection.
	/*! This function sets the center longitude of the 
		projection.
		\param lon Center longitude of the projection in decimal degrees
	*/
	void setCenterLon(double lon);

	//! Set the center latitude for this projection.
	/*! This function sets the center latitude of the 
		projection.
		\param lon Center latitude of the projection in decimal degrees
	*/
	void setCenterLat(double lat);

	//! Set the latitude of the first standard parallel.
	/*! This function sets the latitude of the first 
		standard parallel for the projection.
		\param lat Latitude of first standard parallel in decimal degrees.
	*/
	void setStdParallel1(double lat);

	//! Set the latitude for the second standard parallel.
	/*! This function sets the latitude of the second 
		standard parallel for the projection.
		\param lat Latitude of second standard parallel in decimal degrees.
	*/
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

	//! See documentation for Projection.
	virtual void _loadFromParams();

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