// $Id: LambertAzimuthalProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _LAMBERTAZIMUTHALPROJECTION_H_
#define _LAMBERTAZIMUTHALPROJECTION_H_

#include "AzimuthalProjection.h"

namespace ProjLib
{

class LambertAzimuthalProjection : public AzimuthalProjection  
{
public:
  LambertAzimuthalProjection( double centralLongitude, double centralLatitude,
                              double falseEasting, double falseNorthing,
                              double sphereRadius, DATUM d, UNIT u,
                              DATUM geoDatum = DEFAULT_DATUM,
                              UNIT geoUnit = ARC_DEGREES );
  LambertAzimuthalProjection( const LambertAzimuthalProjection& p );

  // Accessors
  PROJSYS getProjectionSystem() const throw();
  
  // Cloning
  Projection* clone() const throw(std::bad_alloc);

  // String override
  std::string toString() const throw();
};

// ***************************************************************************
inline PROJSYS LambertAzimuthalProjection::getProjectionSystem() const throw()
{
  return LAMAZ;
}

} // namespace ProjLib

#endif

