#ifndef SINUSOIDAL_H
#define SINUSOIDAL_H

#include "pseudocylindrical.h"


class Sinusoidal : public Pseudocylindrical
{
public:
   Sinusoidal();
   
   Sinusoidal(double gctpParameters[], ProjUnit units, Datum dat);
   
   void forward( double lon, double lat, double* x = NULL, double* y = NULL );
   
   void inverse( double x, double y, double* lon = NULL, double* lat = NULL );


protected:
   void forward_init();
   
   void inverse_init();
  

};

#endif
