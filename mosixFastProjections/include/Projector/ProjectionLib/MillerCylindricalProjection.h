// $Id: MillerCylindricalProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _MILLERCYLINDRICALPROJECTION_H_
#define _MILLERCYLINDRICALPROJECTION_H_

#include "PseudocylindricalProjection.h"

namespace ProjLib
{

class MillerCylindricalProjection : public PseudocylindricalProjection  
{
 public:
  MillerCylindricalProjection( double sphereRadius, double centralMeridian,
                               double falseEasting, double falseNorthing,
                               DATUM d, UNIT u,
                               DATUM geoDatum = DEFAULT_DATUM,
                               UNIT geoUnit = ARC_DEGREES );
  MillerCylindricalProjection( const MillerCylindricalProjection& p );
  
  // Accessors
  PROJSYS getProjectionSystem() const throw();

  // Cloning
  Projection* clone() const throw(std::bad_alloc);

  // String override
  std::string toString() const throw();
};


// ***************************************************************************
inline PROJSYS MillerCylindricalProjection::getProjectionSystem() const throw()
{
  return MILLER;
}

} // namespace ProjLib

#endif
