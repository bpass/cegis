#ifndef SINUSOIDAL_H
#define SINUSOIDAL_H

#include "projection.h"

class Sinusoidal : public Projection
{
  // Associations
  // Attributes
  protected:
    double centerLongitude;
    double earthRadius;
    double falseEasting;
    double falseNorthing;
  // Operations
  
  public:
   void Sinusoidal( double gctpParameters[15] );
   long forward( double lon, double lat, double* x = null, double* y = null );
   long inverse( double x, double y, double* lon = null, double* lat = null );

  protected:
   long forward_init();
   long inverse_init();
};

#endif
