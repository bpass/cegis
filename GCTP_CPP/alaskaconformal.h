
#ifndef ALASKA_CON_H
#define ALASKA_CON_H

#include "projection.h"

//! This is the object used to perform the Alaska conformal projection.
class AlaskaConformal : public Projection
{
public:

	AlaskaConformal();
	
	AlaskaConformal(double gctpParams[], ProjUnit units, ProjDatum dat);
	
	//! Set the center longitude of the projection.
	/*! This function sets the center longitude for
		the projection.
		\param lon Center longitude in decimal degrees.
	*/
	void setCenterLon(double lon);

	//! Set the center latitude of the projection.
	/*! This function sets the center latitude for
		the projection.
		\param lon Center latitude in decimal degrees.
	*/
	void setCenterLat(double lat);

	//! Get the center longitude of the projection.
	double centerLon() {return m_centerLon;}

	//! Get the center latitude of the projection.
	double centerLat() {return m_centerLat;}

protected:
	
	//!See documentation for Projection.
	void _init();

	//!See documentation for Projection.
	void _inverse(double x, double y);

	//!See documentation for Projection.
	void _forward(double lon, double lat);
	
	double m_centerLon;
	double m_centerLat;
	double m_acoef[7];
	double m_bcoef[7];
	double m_sinCenterLat;
	double m_cosCenterLat;
	double m_e;
	int m_n;
};

#endif
