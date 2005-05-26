#ifndef ALBER_CON_EQ_AREA_H
#define ALBER_CON_EQ_AREA_H
#include "conic.h"

//! Albers Conical Equal Area projection object.
class AlbersConEqArea: public Conic {
public:
	//! Default constructor
	AlbersConEqArea();

	//! Initialize all fields neccessary to perform our forward and inverse transformations.
	AlbersConEqArea(double gctpParameters[], int units = 0, long datum = 0, long spheroid = 0);

	//! See documentation for Projection.
	void forward(double lon, double lat, double* x = NULL, double*y = NULL);

	//! See documentation for Projection.
	void inverse(double x, double y, double* lat = NULL, double* lon = NULL);

protected:

  //! See documentation for Projection.
  void forward_init();

  //! See documentation for Projection.
  void inverse_init();

};

#endif