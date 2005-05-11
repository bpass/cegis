#ifndef ALBER_CON_EQ_AREA_H
#define ALBER_CON_EQ_AREA_H
#include "projection.h"

class AlbersConEqArea: public Projection {
public:
	AlbersConEqArea();
	AlbersConEqArea(double gctpParams[15], int units, int datum, int spheroid);

	
	//transformation functions
	long forward(double lon, double lat, double* x = NULL, double*y = NULL);
	long inverse(double x, double y, double* lat = NULL, double* lon = NULL);
	long forward_init();
	long inverse_init();

	//get / set functions
	void setCenterLon(double lon);
	void setCenterLat(double lat);
	void setStdParallel1(double lat1);
	void setStdParallel2(double lat2);

protected:
  double m_c;              /* constant c                           */
  double m_e3;             /* eccentricity                         */
  double m_es;			   /* eccentricity squared			       */
  double m_rh;             /* heigth above elipsoid                */
  double m_ns0;            /* ratio between meridians              */
  double m_lon_center;     /* center longitude                     */
  double m_lat_center;
  double m_stdParallelLat1; /* Latitude of standard parallel 1     */
  double m_stdParallelLat2; /* Latitude of standard parallel 2     */
};

#endif