// $Id: PseudocylindricalProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _PSEUDOCYLINDRICALPROJECTION_H_
#define _PSEUDOCYLINDRICALPROJECTION_H_

#include "Projection.h"

namespace ProjLib
{

class PseudocylindricalProjection : public Projection  
{
public:
  PseudocylindricalProjection( double sphereRadius, double centMer,
                               double falseEasting, double falseNorthing,
                               DATUM d, UNIT u,
                               DATUM geoDatum = DEFAULT_DATUM,
                               UNIT geoUnit = ARC_DEGREES );
  PseudocylindricalProjection( const PseudocylindricalProjection& p );

  // Accessors
  double getCentralMeridian() const throw();
  /* Returns the longitude of the central meridian of the projection.
     The angle will be returned in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */
  
  double getFalseEasting() const throw();
  /* Returns the false easting */

  double getFalseNorthing() const throw();
  /* Returns the false northing */

  double getSphereRadius() const throw();
  /* Returns the radius of the reference sphere */

  // Modifers
  virtual void setCentralMeridian( double centralMeridian ) throw();
  /* Sets longitude of central meridian of the projection.
     The angle should be entered in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  virtual void setFalseEasting( double falseEasting ) throw();
  /* Sets false easting */

  virtual void setFalseNorthing( double falseNorthing ) throw();
  /* Sets false northing */

  virtual void setSphereRadius( double sphereRadius ) throw();
  /* Sets the radius of the reference sphere.  If zero, 6370997 meters is
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
  double d_projParams[15];
};

// ***************************************************************************
inline double PseudocylindricalProjection::getCentralMeridian() const throw()
{
  return d_projParams[4];
}

// ***************************************************************************
inline double PseudocylindricalProjection::getFalseEasting() const throw()
{
  return d_projParams[6];
}

// ***************************************************************************
inline double PseudocylindricalProjection::getFalseNorthing() const throw()
{
  return d_projParams[7];
}

// ***************************************************************************
inline double PseudocylindricalProjection::getSphereRadius() const throw()
{
  return d_projParams[0];
}

// ***************************************************************************
inline 
void PseudocylindricalProjection::setCentralMeridian( double centralMeridian )
     throw()
{
  d_projParams[4] = centralMeridian;
}

// ***************************************************************************
inline void PseudocylindricalProjection::setFalseEasting( double falseEasting )
     throw()
{
  d_projParams[6] = falseEasting;
}

// ***************************************************************************
inline void PseudocylindricalProjection::setFalseNorthing(double falseNorthing)
     throw()
{
  d_projParams[7] = falseNorthing;
}

// ***************************************************************************
inline void PseudocylindricalProjection::setSphereRadius( double sphereRadius )
     throw()
{
  d_projParams[0] = sphereRadius;
}

} // namespace Projlib

#endif



