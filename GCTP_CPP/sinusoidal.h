#ifndef SINUSOIDAL_H
#define SINUSOIDAL_H

#include "projection.h"
#include "cproj.h"

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
   Sinusoidal( double gctpParameters[15] );
   long forward( double lon, double lat, double* x = NULL, double* y = NULL );
   long inverse( double x, double y, double* lon = NULL, double* lat = NULL );

  protected:
   long forward_init();
   long inverse_init();
};

#endif
