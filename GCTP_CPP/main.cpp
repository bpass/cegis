#include <stdio.h>

#include "sinusoidal.h"
#include "equirectangular.h"

int main( int argc, char **argv )
{
  double gctpParams[15] = { 0 };

  Sinusoidal sinusoid( gctpParams );
  Equirectangular equirectangular( gctpParams );
  
  printf( "hi\n" );

  return 0;
}
