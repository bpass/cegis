// $Id: VanDerGrintenProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _VanDerGrintenPROJECTION_H_
#define _VanDerGrintenPROJECTION_H_

#include "PseudocylindricalProjection.h"

namespace ProjLib
{

class VanDerGrintenProjection : public PseudocylindricalProjection  
{
public:
  VanDerGrintenProjection( double originLat, double sphereRadius,
                           double centralMeridian,
                           double falseEasting, double falseNorthing,
                           DATUM d, UNIT u,
                           DATUM geoDatum = DEFAULT_DATUM,
                           UNIT geoUnit = ARC_DEGREES );
  VanDerGrintenProjection( const VanDerGrintenProjection& p );

  // Accessors
  PROJSYS getProjectionSystem() const throw();
  double  getOriginLatitude()   const throw();
  
  // Modifiers
  void setOriginLatitude( double originLat ) throw();
  /* Sets the origin latitude.  The latitude must be in packed DMS
     (DDDMMMSSS.SSS) format. */
  
  // Operator overloads
  bool operator==( const Projection& rhs ) const throw();

  // Cloning
  Projection* clone() const throw(std::bad_alloc);

  // String override
  std::string toString() const throw();
};

// ***************************************************************************
inline PROJSYS VanDerGrintenProjection::getProjectionSystem() const throw()
{
  return VGRINT;
}

// ***************************************************************************
inline double VanDerGrintenProjection::getOriginLatitude() const throw()
{
  return d_projParams[5];
}

// ***************************************************************************
inline void VanDerGrintenProjection::setOriginLatitude( double originLat )
     throw()
{
  d_projParams[5] = originLat;
}

} // namespace ProjLib
#endif
