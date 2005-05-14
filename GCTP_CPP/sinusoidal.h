#ifndef SINUSOIDAL_H
#define SINUSOIDAL_H

#include "projection.h"


class Sinusoidal : public Projection
{
  // Associations
  // Attributes
  protected:
    double m_centerLongitude;
    void forward_init();
    void inverse_init();

  // Operations
  
  public:
   Sinusoidal();
   Sinusoidal(double gctpParameters[], int units = 0, int datum = 0, int spheroid = 0);
   void forward( double lon, double lat, double* x = NULL, double* y = NULL );
   void inverse( double x, double y, double* lon = NULL, double* lat = NULL );

   void setCenterLon(double centerLon); 
};

#endif
