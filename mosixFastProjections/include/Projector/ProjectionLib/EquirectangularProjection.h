// $Id: EquirectangularProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _EquirectangularPROJECTION_H_
#define _EquirectangularPROJECTION_H_

#include "PseudocylindricalProjection.h"

namespace ProjLib
{

class EquirectangularProjection : public PseudocylindricalProjection  
{
public:
  EquirectangularProjection( double trueScaleLat, double sphereRadius,
                             double centralMeridian,
                             double falseEasting, double falseNorthing,
                             DATUM d, UNIT u,
                             DATUM geoDatum = DEFAULT_DATUM,
                             UNIT geoUnit = ARC_DEGREES );
  EquirectangularProjection( const EquirectangularProjection& p );
  
  // Accessors
  PROJSYS getProjectionSystem() const throw();
  double getTrueScaleLatitude() const throw();

  // Modifiers
  void setTrueScaleLatitude( double trueScaleLat ) throw();
  /* Sets the true scale latitude.  The latitude must be in packed DMS
     (DDDMMMSSS.SSS) format. */

  // Operator overloads
  bool operator==( const Projection& rhs ) const throw();

  // Cloning
  Projection* clone() const throw(std::bad_alloc);
  
  // String override
  std::string toString() const throw();
};

// ***************************************************************************
inline PROJSYS EquirectangularProjection::getProjectionSystem() const throw()
{
  return EQRECT;
}

// ***************************************************************************
inline double EquirectangularProjection::getTrueScaleLatitude() const throw()
{
  return d_projParams[5];
}

// ***************************************************************************
inline 
void EquirectangularProjection::setTrueScaleLatitude( double trueScaleLat )
     throw()
{
  d_projParams[5] = trueScaleLat;
}

} // namespace ProjLib

#endif

