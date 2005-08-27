// $Id: equirectangular.h,v 1.1 2005/08/27 00:20:31 rbuehler Exp $


#ifndef EQUIRECTANGULAR_H
#define EQUIRECTANGULAR_H

#include "projection.h"

//! This is the object used for the equirectangular projection.
class Equirectangular : public Projection
{

  public:
   Equirectangular();
   
   Equirectangular(double gctpParameters[15], ProjUnit units, ProjDatum dat);
      
  protected:

	//! See documentation for Projection
	void _init();

	//! See documentation for Projection
	void _forward(double lon, double lat);
	
	//! See documentation for Projection	
	void _inverse(double x, double y);

};

#endif
