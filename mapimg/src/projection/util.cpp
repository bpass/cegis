
/* File: util.cpp
   Programmer: David Mattli

   Copyright 2007 U.S. Geological Survey

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License version
   2 as published by the Free Software Foundation.
*/

#include "util.h"

namespace trans {
  
    int sign (double x )
    {
	if (x < 0.0)
	    return(-1);
	else
	    return(1);
    }

    double phi2z (double eccen, double ts)
    {
	double half_eccen, phi, esin_phi, dphi, sinphi;

	half_eccen = .5 * eccen;
	phi = M_PI_2 - (2*atan(ts));
	for(int i=0; i<16; ++i) {
	    sinphi = sin(phi);
	    esin_phi = eccen * sinphi;
	    dphi = M_PI_2 - (atan(ts * (pow(((1.0 - esin_phi)/(1.0 + esin_phi)),
					    half_eccen))) * 2) - phi;
	    phi += dphi;
	    if (fabs(dphi) <= 1e-7)
		return phi;
	}

	return 0; // Error...
    }

  double tsfnz(double eccen, double phi, double sinphi) 
  {
    double con;

    con = eccen * sinphi;
    con = pow(((1.0 - con) / (1.0 + con)), eccen * .5);

    return (tan(M_PI_4 + phi*.5)*con);
  }

    double adjust_lon (double x)
    {
#define DBLLONG 4.61168601e18
    
	for(unsigned int i=0; i<4; ++i) {
	    if (fabs(x)<=M_PI)
		break;
	    
	    else if (((long) fabs(x / M_PI)) < 2)
		x = x-(sign(x) * M_TWOPI);
	    
	    else if (((long) fabs(x / M_TWOPI)) < LONG_MAX)
		x = x-(((long)(x / M_TWOPI))*M_TWOPI);
		
      
	    else if (((long) fabs(x / (LONG_MAX * M_TWOPI))) < LONG_MAX)
		x = x-(((long)(x / (LONG_MAX * M_TWOPI))) 
		       * (M_TWOPI * LONG_MAX));
      
      
	    else if (((long) fabs(x / (DBLLONG * M_TWOPI))) < LONG_MAX)
		x = x-(((long)(x / (DBLLONG * M_TWOPI))) 
		       * (M_TWOPI * DBLLONG));
      
	    else
		x = x-(sign(x) * M_TWOPI);
	}

	return x;
    }

}

