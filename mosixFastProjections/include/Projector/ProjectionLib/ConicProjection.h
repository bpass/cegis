// $Id: ConicProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _CONICPROJECTION_H_
#define _CONICPROJECTION_H_

#include "Projection.h"

namespace ProjLib
{

class ConicProjection : public Projection  
{
public:
  ConicProjection( double sMajor, double sMinor, double centMer,
                   double originLat,
                   double fe, double fn, DATUM d, UNIT u,
                   DATUM geoDatum = DEFAULT_DATUM,
                   UNIT geoUnit = ARC_DEGREES );
  ConicProjection( const ConicProjection& p );
  
  // Accessors
  double getSemiMajorAxis()   const throw();
  double getSemiMinorAxis()   const throw();
  double getCentralMeridian() const throw();
  double getOriginLatitude()  const throw();
  double getFalseEasting()    const throw();
  double getFalseNorthing()   const throw();

  // Modifiers
  virtual void setSemiMajorAxis( double sMajor ) throw();
  /* Sets the semi-major axis of the ellipsoid.  If zero, Clarke 1866 in meters
     is assumed. */

  virtual void setSemiMinorAxis( double sMinor ) throw();
  /* ( sMinor < 0 ) -> eccentricity squared of the ellipsoid,
     ( sMinor = 0 ) -> spherical form is assumed
     ( sMinor > 0 ) -> semi-minor axis of the ellipsoid. */

  virtual void setCentralMeridian( double centMer ) throw();
  /* Sets the longitude of the central meridian 
     The angle should be entered in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  virtual void setOriginLatitude( double originLat ) throw();
  /* Sets the latitude of the projection origin 
     The angle should be entered in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  virtual void setFalseEasting( double fe ) throw();
  /* Sets the false easting in the same units as the semi-major axis */
  
  virtual void setFalseNorthing( double fn ) throw();
  /* Sets the false northing in the same units as the semi-major axis */

  // Projection functions
  bool projectToGeo( double x, double y, 
                     double& latitude, double& longitude ) const throw();
  bool projectFromGeo( double latitude, double longitude,
                       double& x, double& y ) const throw();

  // Operator overloads
  bool operator==( const Projection& p ) const throw();
  
  // toString override
  virtual std::string toString() const throw();

protected:
  // Data members
  double d_projParams[15];
};

// ***************************************************************************
inline double ConicProjection::getSemiMajorAxis() const throw()
{
  return d_projParams[0];
}

// ***************************************************************************
inline double ConicProjection::getSemiMinorAxis() const throw()
{
  return d_projParams[1];
}

// ***************************************************************************
inline double ConicProjection::getCentralMeridian() const throw()
{
  return d_projParams[4];
}

// ***************************************************************************
inline double ConicProjection::getOriginLatitude() const throw()
{
  return d_projParams[5];
}
    
// ***************************************************************************
inline double ConicProjection::getFalseEasting() const throw()
{
  return d_projParams[6];
}
    
// ***************************************************************************
inline double ConicProjection::getFalseNorthing() const throw() 
{
  return d_projParams[7];
}

// ***************************************************************************
inline void ConicProjection::setSemiMajorAxis( double sMajor ) throw()
{
  d_projParams[0] = sMajor;
}
    
// ***************************************************************************
inline void ConicProjection::setSemiMinorAxis( double sMinor ) throw()
{
  d_projParams[1] = sMinor;
}
    
// ***************************************************************************
inline void ConicProjection::setCentralMeridian( double centMer ) throw() 
{
  d_projParams[4] = centMer;
}
 
// ***************************************************************************
inline void ConicProjection::setOriginLatitude( double originLat ) throw()
{
  d_projParams[5] = originLat;
}

// ***************************************************************************
inline void ConicProjection::setFalseEasting( double fe ) throw()
{
  d_projParams[6] = fe;
}

// ***************************************************************************
inline void ConicProjection::setFalseNorthing( double fn ) throw()
{
  d_projParams[7] = fn;
}

} // namespace ProjLib

#endif


