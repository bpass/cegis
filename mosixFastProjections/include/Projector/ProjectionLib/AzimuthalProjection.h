// $Id: AzimuthalProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _AZIMUTHALPROJECTION_H_
#define _AZIMUTHALPROJECTION_H_

#include "Projection.h"

namespace ProjLib
{

class AzimuthalProjection : public Projection  
{
public:
  AzimuthalProjection( double centralLongitude, double centralLatitude,
                       double falseEasting, double falseNorthing,
                       double sphereRadius, DATUM d, UNIT u,
                       DATUM geoDatum = DEFAULT_DATUM,
                       UNIT geoUnit = ARC_DEGREES );
  AzimuthalProjection( const AzimuthalProjection& p );

  // Accessors
  double getCentralLatitude() const throw();
  /* Returns the latitude of the center of the projection
     The angle will be returned in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */
  
  double getCentralLongitude() const throw();
  /* Returns the longitude of the center of the projection
     The angle will be returned in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  double getFalseEasting() const throw();
  /* Returns the false easting */
  
  double getFalseNorthing() const throw();
  /* Returns the false northing */
  
  double getSphereRadius() const throw();
  /* Returns the radius of the reference sphere */

  // Modifers
  virtual void setCentralLatitude( double centralLatitude ) throw();
  /* Sets latitude of center of projection. 
     The angle should be entered in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  virtual void setCentralLongitude( double centralLongitude ) throw();
  /* Sets longitude of center of projection 
     The angle should be entered in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  virtual void setFalseEasting( double falseEasting ) throw();
  /* Sets false easting */
  
  virtual void setFalseNorthing( double falseNorthing ) throw();
  /* Sets false northing */
  
  virtual void setSphereRadius( double sphereRadius ) throw();
  /* Sets the radius of the reference sphere.  If zero, 6370997 meters are
     used. */

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
  // Overrides
  virtual bool checkCoordinateRange( double latitude, double longitude ) 
    const throw();

  double d_projectionParameters[15];
  double d_badDegreeRadius;
};

// ***************************************************************************
inline double AzimuthalProjection::getCentralLatitude() const throw()
{
  return d_projectionParameters[5];
}

// ***************************************************************************
inline double AzimuthalProjection::getCentralLongitude() const throw()
{
  return d_projectionParameters[4];
}

// ***************************************************************************
inline double AzimuthalProjection::getFalseEasting() const throw()
{
  return d_projectionParameters[6];
}

// ***************************************************************************
inline double AzimuthalProjection::getFalseNorthing() const throw()
{
  return d_projectionParameters[7];
}

// ***************************************************************************
inline double AzimuthalProjection::getSphereRadius() const throw()
{
  return d_projectionParameters[0];
}

// ***************************************************************************
inline void AzimuthalProjection::setCentralLatitude( double centralLatitude )
  throw()
{
  d_projectionParameters[5] = centralLatitude;
}

// ***************************************************************************
inline void AzimuthalProjection::setCentralLongitude( double centralLongitude )
  throw()
{
  d_projectionParameters[4] = centralLongitude;
}

// ***************************************************************************
inline void AzimuthalProjection::setFalseEasting( double falseEasting )
  throw()
{
  d_projectionParameters[6] = falseEasting;
}

// ***************************************************************************
inline void AzimuthalProjection::setFalseNorthing( double falseNorthing )
  throw()
{
  d_projectionParameters[7] = falseNorthing;
}

// ***************************************************************************
inline void AzimuthalProjection::setSphereRadius( double sphereRadius )
  throw()
{
  d_projectionParameters[0] = sphereRadius;
}

} // namespace ProjLib

#endif
