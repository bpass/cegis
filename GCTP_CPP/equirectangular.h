#ifndef EQUIRECTANGULAR_H
#define EQUIRECTANGULAR_H

#include "projection.h"

//! Equirectangular projection object.
class Equirectangular : public Projection
{
  protected:
    double m_majorAxis;
    double m_centerLongitude;
    double m_centerLatitude;

  // Operations
  public:
   Equirectangular();
   Equirectangular(double gctpParameters[15], int units = 0, long datum = 0, long spheroid = 0);
   void forward( double lon, double lat, double* x = NULL, double* y = NULL );
   void inverse( double x, double y, double* lon = NULL, double* lat = NULL );
   
   void setCenterLat(double lat);
   void setCenterLon(double lon);
  public:
   void forward_init (  );
   void inverse_init (  );
};

#endif
