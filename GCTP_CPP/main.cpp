#include <stdio.h>

#include "sinusoidal.h"
#include "equirectangular.h"
#include "albersConEqArea.h"
#include ".\gctpc\source\proj.h"
int main( int argc, char **argv )
{
  double equirectParams[15] = { 6370997.000000, 0.000000, 0.000000, 0.000000, 0.000000, 
				   			          0.000000,	0.000000, 0.000000, 0.000000, 0.000000, 
								      0.000000,	0.000000, 0.000000, 0.000000, 0.000000  };

  double sinusoidalParams[15] = { 6370997.000000, 0.000000, 0.000000, 0.000000, 0.000000, 
				    		            0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 
								        0.000000, 0.000000, 0.000000, 0.000000, 0.000000  };

  double alberParams[15] = { 6370997.000000, 0, 40000000.000000, 50000000.000000, 0.000000, 
				    		            0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 
								        0.000000, 0.000000, 0.000000, 0.000000, 0.000000  };

  double lat = 110.4;
  double lon = -55.234;
  double x = 0;
  double y = 0;
  
  Sinusoidal sinSd(sinusoidalParams, DEGREE, 0, 0);
  Equirectangular eq(equirectParams, DEGREE, 0, 0);
  AlbersConEqArea albers(alberParams, DEGREE, 0, 0);

  sinSd.forward_init();
  eq.forward_init();
  albers.forward_init();

  printf("GCTP-CPP Output\n\n");
  sinSd.forward(lon, lat, &x, &y);
  printf("Sinusoidal lon: %f lat: %f x: %f y %f\n", lon, lat, x, y);
  eq.forward(lon, lat, &x, &y);
  printf("Equirectangular lon: %f lat: %f x: %f y %f\n", lon, lat, x, y);
  albers.forward(lon, lat, &x, &y);
  printf("Albers lon: %f lat: %f x: %f y %f\n", lon, lat, x, y);


  return 0;
}
