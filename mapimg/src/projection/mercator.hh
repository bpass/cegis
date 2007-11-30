/* File: mercator.hh
   Programmer: David Mattli

   Copyright 2007 U.S. Geological Survey

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License version
   2 as published by the Free Software Foundation.
*/

#ifndef PROJECTION_MERCATOR_HH
#define PROJECTION_MERCATOR_HH

#define EPSLN	1.0e-10 // TODO: Move this somewhere.

#include <limits.h>
#include <math.h>

#include "util.hh"

/* CLASS NAME: mercator
 *
 */

namespace projection {

    template <typename C>
    class mercator
    {
    public:
	mercator(C _maj_axis, C _min_axis);
	mercator();
	~mercator();
    
	C forward_x (C lon, C lat);
	C forward_y (C lon, C lat);
	C inverse_lon (C x, C y);
	C inverse_lat (C x, C y);

	void set_feasting (double x);
	void set_fnorthing (double x);
	void set_center_lon (double x);
	void set_center_lat (double x);

	bool good();

    private:
	/* Projection Parameters */
	double eccen;
	double eccen_sq;
	double m1;  
	double feasting;
	double fnorthing;
	double maj_axis;
	double min_axis;
	double cen_lon;
	double cen_lat;
	int err_flag;
    
    };

    template <typename C>
    mercator<C>::mercator (C _maj_axis, C _min_axis)
    {
	maj_axis = _maj_axis;
	min_axis = _min_axis;
	eccen = sqrt(maj_axis*maj_axis - min_axis*min_axis)/maj_axis;
	eccen_sq = sqrt(1 - eccen);
	m1 = cos(cen_lat)/(sqrt(1.0 - eccen_sq * sin(cen_lat) * sin(cen_lat))); 
	err_flag = 0;
	feasting = 0;
	fnorthing = 0;
	cen_lon = 0;

	return;
    }
    
    template <typename C>
    mercator<C>::mercator()
    {
	maj_axis = 6378137;
	min_axis = 6378137;
	eccen = sqrt(maj_axis*maj_axis - min_axis*min_axis)/maj_axis;
	eccen_sq = eccen*eccen;
	m1 = cos(cen_lat)/(sqrt(1.0 - sqrt(1-eccen*eccen) * sin(cen_lat) * sin(cen_lat))); 
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
	    err_flag = 1; // ERROR!
	}
	else {
	    tmp = feasting + maj_axis * (lon - cen_lon);
	}
	
	return tmp;
    }

    template <typename C>
    C mercator<C>::forward_y (C lon, C lat)
    {
	C tmp = 0; // Return value
	double t; // Small t value
	double sinphi; // sin value

	if (fabs (fabs (lat) - M_PI_2) <= EPSLN) {
	    err_flag = 1; // ERROR!, value at poles = infinity
	}
	else {
	    sinphi = sin(lat);
	    t = tsfnz(eccen, lat, sinphi);
	    tmp = fnorthing - maj_axis * m1* log(t);
	}

	return tmp;
    }
  
    template <typename C>
    C mercator<C>::inverse_lat (C x, C y)
    {
	C tmp = y - fnorthing;
	double t = 0.0;

	t = exp(-tmp/maj_axis);
	return phi2z(eccen, t);
    }

    template <typename C>
    C mercator<C>::inverse_lon (C x, C y)
    {
	C tmp = x - feasting;

	return adjust_lon(cen_lon + tmp/maj_axis);
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
    void mercator<C>::set_center_lon (double x)
    {
	cen_lon = x;
	return;
    }

    template <typename C>
    void mercator<C>::set_center_lat (double x)
    {
	cen_lat = x;
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

}


#endif
