// $Id: HotineObliqueMercatorProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _HOTINEOBLIQUEMERCATORPROJECTION_H_
#define _HOTINEOBLIQUEMERCATORPROJECTION_H_

#include "MercatorProjection.h"

namespace ProjLib
{

class HotineObliqueMercatorProjection : public MercatorProjection  
{
public:
  // Hotine A Constructor
  HotineObliqueMercatorProjection(double scaleFactor,double long1, double lat1,
                                  double long2, double lat2, double sMajor,
                                  double sMinor, double originLat, double fe,
                                  double fn, DATUM d, UNIT u,
                                  DATUM geoDatum = DEFAULT_DATUM,
                                  UNIT geoUnit = ARC_DEGREES );

  // Hotine B Constructor
  HotineObliqueMercatorProjection( double scaleFactor, double aziAng, 
                                   double sMajor, double sMinor,
                                   double aziPt, double originLat,
                                   double fe, double fn,
                                   DATUM d, UNIT u,
                                   DATUM geoDatum = DEFAULT_DATUM,
                                   UNIT geoUnit = ARC_DEGREES );
  HotineObliqueMercatorProjection( const HotineObliqueMercatorProjection& p );

  // Accessors
  PROJSYS getProjectionSystem()           const throw();
  double getProjectionCenterScaleFactor() const throw();
  double getAzimuthAngle()                const throw();
  double getAzimuthPoint()                const throw();
  double getLongitude1()                  const throw();
  double getLongitude2()                  const throw();
  double getLatitude1()                   const throw();
  double getLatitude2()                   const throw();

  // Modifiers
  void setProjectionCenterScaleFactor( double scaleFactor ) throw();
  /* Set the scale factor at the center of the projection */

  void setAzimuthAngle( double aziAng ) throw();
  /* Set the azimuth angle east of north of center line */

  void setAzimuthPoint( double aziPt ) throw();
  /* Set the longitude of point on central meridian where azimuth occurs */
  
  void setLongitude1( double long1 ) throw();
  /* Sets the longitude of first point on center line */
  
  void setLongitude2( double long2 ) throw();
  /* Sets the longitude of second point on center line */

  void setLatitude1( double lat1 ) throw();
  /* Sets the latitude of first point on center line */

  void setLatitude2( double lat2 ) throw();
  /* Sets the latitude of second point on center line */

  // Operator overloads
  bool operator==( const Projection& p ) const throw();

  // toString override
  virtual std::string toString() const throw();

  // Cloning
  virtual Projection* clone() const throw(std::bad_alloc);
};
 
// ***************************************************************************
inline PROJSYS HotineObliqueMercatorProjection::getProjectionSystem() 
  const throw()
{
  return HOM;
}

// ***************************************************************************
inline 
double HotineObliqueMercatorProjection::getProjectionCenterScaleFactor() 
     const throw()
{
  return d_projParams[2];
}

// ***************************************************************************
inline
void HotineObliqueMercatorProjection::setProjectionCenterScaleFactor( double scaleFactor ) throw()
{
  d_projParams[2] = scaleFactor;
}

// ***************************************************************************
inline double HotineObliqueMercatorProjection::getAzimuthAngle() const throw()
{
  return d_projParams[3];
}
    
// ***************************************************************************
inline void HotineObliqueMercatorProjection::setAzimuthAngle( double aziAng )
     throw()
{
  d_projParams[3] = aziAng;
}
    
// ***************************************************************************
inline double HotineObliqueMercatorProjection::getAzimuthPoint() const throw()
{
  return getCentralMeridian();
}
    
// ***************************************************************************
inline void HotineObliqueMercatorProjection::setAzimuthPoint( double aziPt )
     throw()
{
  setCentralMeridian( aziPt );
}
    
// ***************************************************************************
inline double HotineObliqueMercatorProjection::getLongitude1() const throw()
{
  return d_projParams[8];
}
    
// ***************************************************************************
inline void HotineObliqueMercatorProjection::setLongitude1( double long1 )
     throw()
{
  d_projParams[8] = long1;
}
    
// ***************************************************************************
inline double HotineObliqueMercatorProjection::getLongitude2() const throw()
{
  return d_projParams[10];
}
    
// ***************************************************************************
inline void HotineObliqueMercatorProjection::setLongitude2( double long2 )
     throw()
{
  d_projParams[10] = long2;
}
    
// ***************************************************************************
inline double HotineObliqueMercatorProjection::getLatitude1() const throw()
{
  return d_projParams[9];
}
    
// ***************************************************************************
inline void HotineObliqueMercatorProjection::setLatitude1( double lat1 ) 
     throw()
{
  d_projParams[9] = lat1;
}
    
// ***************************************************************************
inline double HotineObliqueMercatorProjection::getLatitude2() const throw()
{
  return d_projParams[11];
}

// ***************************************************************************
inline void HotineObliqueMercatorProjection::setLatitude2( double lat2 ) 
     throw()
{
  d_projParams[11] = lat2;
}

} // namespace ProjLib

#endif

