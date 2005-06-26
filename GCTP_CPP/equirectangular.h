#ifndef EQUIRECTANGULAR_H
#define EQUIRECTANGULAR_H

#include "cylindrical.h"

//! Equirectangular projection object.
class Equirectangular : public Cylindrical
{

  // Operations
  public:
   Equirectangular();
   
   Equirectangular(double gctpParameters[15], ProjUnit units, ProjDatum dat);
      
  protected:

	//! See documentation for Projection
	void _init();

	void _forward(double lon, double lat);
	void _inverse(double x, double y);

   
};

#endif
