#ifndef EQUIRECTANGULAR_H
#define EQUIRECTANGULAR_H


class Equirectangular {
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
   void Equirectangular( double gctpParameters[15] );
   long forward( double lon, double lat, double* x = null, double* y = null );
   long inverse( double x, double y, double* lon = null, double* lat = null );

  protected:
   long forward_init (  );
   long inverse_init (  );
};

#endif
