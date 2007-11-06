/* Programmer: David Mattli */

#ifndef TRANS_MERCATOR_HH
#define TRANS_MERCATOR_HH

#include <limits.h>
#include <math.h>

#include "transformation.hh"

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
    
    coord<C> forward (coord<C> geo_coord);
    coord<C> inverse (coord<C> rec_coord);

    void set_feasting(double x);
    void set_fnorthing(double x);
    
  protected:
    double eccen;
    double eccen_sq;
    double feasting;
    double fnorthing;
    double maj_axis;
    double min_axis;
    double cen_lon;

  private:
    int err_flag;
    int sign(double x);
    double phi2z(double eccen, double ts);
    double adjust_lon(double x);
    
  };

  template <typename C>
  mercator<C>::mercator(C _maj_axis, C _min_axis)
  {
    maj_axis = _maj_axis;
    min_axis = _min_axis;
    eccen = sqrt(maj_axis*maj_axis - min_axis*min_axis)/maj_axis;
    eccen_sq = eccen*eccen;
    err_flag = 0;
    feasting = 0;
    fnorthing = 0;
    cen_lon = 0;

    return;
  }
  
  template <typename C>
  mercator<C>::~mercator()
  {
    return;
  }
  
  template <typename C>
  coord<C> mercator<C>::forward (coord<C> geo_coord)
  {
    coord<C> tmp;
    tmp.x = 0;
    tmp.y = 0; 

    return tmp;
  }
  
  template <typename C>
  coord<C> mercator<C>::inverse (coord<C> rec_coord)
  {
    double t;
    C lat(0), lon(0);
    coord<C> tmp;

    rec_coord.x -= feasting;
    rec_coord.y -= fnorthing;
      
    t = exp(-rec_coord.y/maj_axis);
    lat = phi2z(eccen, t);
    lon = adjust_lon(cen_lon + rec_coord.x/maj_axis);
    tmp.x = lat;
      
      
    return tmp;
  }
  
  template <typename C>
  void mercator<C>::set_feasting(double x)
  {
    feasting = x;
    return;
  }

  template <typename C>
  void mercator<C>::set_fnorthing(double x)
  {
    fnorthing = x;
    return;
  }

  template <typename C>
  double mercator<C>::phi2z(double eccen, double ts)
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
  int mercator<C>::sign(double x )
  {
    if (x < 0.0)
      return(-1);
    else
      return(1);
  }
  
  template <typename C>
  double mercator<C>::adjust_lon(double x)
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
	  x = x-(((long)(x / (LONG_MAX * M_TWOPI))) * (M_TWOPI * LONG_MAX));
      
      
	else if (((long) fabs(x / (DBLLONG * M_TWOPI))) < LONG_MAX)
	  x = x-(((long)(x / (DBLLONG * M_TWOPI))) * (M_TWOPI * DBLLONG));
      
	else
	  x = x-(sign(x) * M_TWOPI);
    }

    return x;
  }

}


#endif
