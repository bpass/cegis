#ifndef EQUIRECTANGULAR_H
#define EQUIRECTANGULAR_H

#include "cylindrical.h"

//! Equirectangular projection object.
class Equirectangular : public Cylindrical
{

  // Operations
  public:
   Equirectangular();
   
   Equirectangular(double gctpParameters[15], int units = 0, long datum = 0, long spheroid = 0);
   
   void forward( double lon, double lat, double* x = NULL, double* y = NULL );
   
   void inverse( double x, double y, double* lon = NULL, double* lat = NULL );
   
  protected:
   void forward_init ();

   void inverse_init ();
   
};

#endif
