#include <stdio.h>

#include "sinusoidal.h"
#include "equirectangular.h"

int main( int argc, char **argv )
{
  double equirectParams[15] = { 6370997.000000, 0.000000, 0.000000, 0.000000, 0.000000, 
				   			          0.000000,	0.000000, 0.000000, 0.000000, 0.000000, 
								      0.000000,	0.000000, 0.000000, 0.000000, 0.000000  };


  double sinusoidalParams[15] = { 6370997.000000, 0.000000, 0.000000, 0.000000, 0.000000, 
				    		            0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 
								        0.000000, 0.000000, 0.000000, 0.000000, 0.000000  };

  Sinusoidal sinusoid( sinusoidalParams );
  sinusoid.name = "Sinusoidal\0";
  sinusoid.number = 16;
  sinusoid.unitCode = 2;
  sinusoid.datum = 19;
  sinusoid.spheroid = 0;   ///what number is thisss???


  Equirectangular equirectangular( equirectParams );
  equirectangular.name = "Equirectangular\0";
  equirectangular.number = 17;
  equirectangular.unitCode = 2;
  equirectangular.datum = 0;       //is this right???
  equirectangular.spheroid = 0;  // what number is this


  equirectangular.inverse_init();
  sinusoid.forward_init();

  double x = 0, 
		 y = 0,
		 lat = 0,
		 lon = 0;

  for( int y_index = 0; y_index < 180; y_index++ )
  {
	  y = 9951941.230782 - ( y_index * 111194.909679 );

	  for( int x_index = 0; x_index < 360; x_index++ )
	  {
			x = -19959479.916403 + ( x_index * 111194.909679 );

			equirectangular.inverse( x, y, &lon, &lat );

			sinusoid.forward( lon, lat, &x, &y );


			y = ( ( 10063136.140461 - y ) / 111194.909679) + 0.5;
			x = ( ( x - -19959479.916403 ) / 111194.909679) + 0.5;

			if( y_index%10 == 0 )
			{
				printf( "Input ( %f, %f ) => Output ( %f, %f )\n", (double)x_index, (double)y_index, x, y );
				fflush( stdout );
			}
	  }
  }

  return 0;
}
