/* File: mercator.hh
   Programmer: David Mattli

   Copyright 2007 U.S. Geological Survey

   This program is free software; you can redistribute it and/or
   modify if under the terms of the GNU General Public License version
   2 as published by the Free Software Foundation.
*/

#ifndef TRANS_MERCATOR_HH
#define TRANS_MERCATOR_HH

#define EPSLN	1.0e-10

#include <limits.h>
#include <math.h>

/* CLASS NAME: mercator
 *
 */

namespace trans {

    template <typename C>
    class mercator
    {
    public:
	mercator(C _maj_axis, C _min_axis);
	~mercator();
    
	C forward_x (C lon, C lat);
	C forward_y (C lon, C lat);
	C inverse_lon (C x, C y);
	C inverse_lat (C x, C y);

	void set_feasting (double x);
	void set_fnorthing (double x);

	bool good();
	bool bad();

    private:
	/* Projection Parameters */
	double eccen;
	double eccen_sq;
	double m1;  // TODO: What is this?
	double feasting;
	double fnorthing;
	double maj_axis;
	double min_axis;
	double cen_lon;
	double cen_lat;

	int err_flag;
	int sign(double x);
	double phi2z(double eccen, double ts);
	double adjust_lon(double x);
    
    };

    template <typename C>
    mercator<C>::mercator (C _maj_axis, C _min_axis)
    {
	maj_axis = _maj_axis;
	min_axis = _min_axis;
	eccen = sqrt(maj_axis*maj_axis - min_axis*min_axis)/maj_axis;
	eccen_sq = eccen*eccen;
	m1 = cos(cen_lat)/(sqrt(1.0 - m_es * sin(cen_lat) * sin(cen_lat))); 
	err_flag = 0;
	feasting = 0;
	fnorthing = 0;
	cen_lon = 0;

	return;
    }
  
    template <typename C>
    mercator<C>::~mercator ()
    {
	return;
    }
  
    template <typename C>
    C mercator<C>::forward_x (C lon, C lat)
    {
	C tmp = 0; 

	if (fabs (fabs (lat) - M_PI_2) <= EPSLN) {
	    // ERROR!
	}
	else {
	    tmp = feasting + (maj_axis * m1 * adjust_lon(lon - cen_lon));
	}
	
	return tmp;
    }

    template <typename C>
    C mercator<C>::forward_y (C lon, C lat)
    {
	C tmp = 0; 
	double t; // Small t value
	double sinphi; // sin value

	if (fabs (fabs (lat) - M_PI_2) <= EPSLN) {
	    // ERROR!
	}
	else {
	    sinphi = sin(lat);
	    t = tsfnz(m_e, lat, sinphi);
	    tmp = fnorthing - (maj_axis * m1 * log(t));
	}


	return tmp;
    }
  
    template <typename C>
    C mercator<C>::inverse_lon (C x, C y)
    {
	C tmp = x - feasting;
	double t = 0.0;

	t = exp(-y/maj_axis);
	return phi2z(eccen, t);
    }

    template <typename C>
    C mercator<C>::inverse_lat (C x, C y)
    {
	C tmp = y - fnorthing;

	return adjust_lon(cen_lon + x/maj_axis);
    }

    template <typename C>
    void mercator<C>::set_feasting (double x)
    {
	feasting = x;
	return;
    }

    template <typename C>
    void mercator<C>::set_fnorthing (double x)
    {
	fnorthing = x;
	return;
    }
    
    template <typename C>
    bool mercator<C>::good()
    {
	if (err_flag == 0) {
	    return true;
	}
	else {
	    return false;
	}
    }

    template <typename C>
    bool mercator<C>::bad()
    {
	return !good();
    }

    

    template <typename C>
    double mercator<C>::phi2z (double eccen, double ts)
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
  
    template <typename C>
    int mercator<C>::sign (double x )
    {
	if (x < 0.0)
	    return(-1);
	else
	    return(1);
    }
  
    template <typename C>
    double mercator<C>::adjust_lon (double x)
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


#endif
