#ifndef EQUIRECTANGULAR_H
#define EQUIRECTANGULAR_H

#include "projection.h"
#include "cproj.h"

class Equirectangular : public Projection
{
  // Associations
  // Attributes
  protected:
    double majorAxis;
    double centerLongitude;
    double centerLatitude;
    double falseEasting;
    double falseNorthing;
    
    double earthRadius;
  // Operations
  public:
   Equirectangular( double gctpParameters[15] );
   long forward( double lon, double lat, double* x = NULL, double* y = NULL );
   long inverse( double x, double y, double* lon = NULL, double* lat = NULL );

  public:
   long forward_init (  );
   long inverse_init (  );
};

#endif
