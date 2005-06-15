#ifndef ALBER_CON_EQ_AREA_H
#define ALBER_CON_EQ_AREA_H
#include "conic.h"

//! Albers Conical Equal Area projection object.
class AlbersConEqArea: public Conic {
public:
	//! Default constructor
	AlbersConEqArea();

	//! Initialize all fields neccessary to perform our forward and inverse transformations.
	AlbersConEqArea(double gctpParameters[], ProjUnit units, ProjDatum dat);

	//! See documentation for Projection.
	void forward(double lon, double lat, double* x = NULL, double*y = NULL);

	//! See documentation for Projection.
	void inverse(double x, double y, double* lat = NULL, double* lon = NULL);

protected:
  
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

  //! See documentation for Projection
  void init();
};

#endif