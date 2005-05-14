#ifndef EQUIRECTANGULAR_H
#define EQUIRECTANGULAR_H

#include "projection.h"


class Equirectangular : public Projection
{
  // Associations
  // Attributes
  protected:
    double m_majorAxis;
    double m_centerLongitude;
    double m_centerLatitude;

  // Operations
  public:
   Equirectangular();
   Equirectangular(double gctpParameters[15], int units, long datum, long spheroid);
   void forward( double lon, double lat, double* x = NULL, double* y = NULL );
   void inverse( double x, double y, double* lon = NULL, double* lat = NULL );
   
   void setCenterLat(double lat);
   void setCenterLon(double lon);
  public:
   void forward_init (  );
   void inverse_init (  );
};

#endif
