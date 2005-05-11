#ifndef SINUSOIDAL_H
#define SINUSOIDAL_H

#include "projection.h"


class Sinusoidal : public Projection
{
  // Associations
  // Attributes
  protected:
    double m_centerLongitude;
    double m_earthRadius;

  // Operations
  
  public:
   Sinusoidal();
   Sinusoidal( double gctpParameters[15], int units, int datum, int spheroid );
   long forward( double lon, double lat, double* x = NULL, double* y = NULL );
   long inverse( double x, double y, double* lon = NULL, double* lat = NULL );
   long forward_init();
   long inverse_init();

   void setCenterLon(double centerLon); 
   void setEarthRadius(double earthRadius) {m_earthRadius = earthRadius;}
};

#endif
