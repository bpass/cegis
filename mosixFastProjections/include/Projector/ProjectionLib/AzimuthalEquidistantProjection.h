// $Id: AzimuthalEquidistantProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _AZIMUTHALEQUIDISTANTPROJECTION_H_
#define _AZIMUTHALEQUIDISTANTPROJECTION_H_

#include "AzimuthalProjection.h"

namespace ProjLib
{

class AzimuthalEquidistantProjection : public AzimuthalProjection  
{
public:
  AzimuthalEquidistantProjection( double centralLongitude, 
                                  double centralLatitude,
                                  double falseEasting, double falseNorthing,
                                  double sphereRadius, DATUM d, UNIT u,
                                  DATUM geoDatum = DEFAULT_DATUM,
                                  UNIT geoUnit = ARC_DEGREES );
  AzimuthalEquidistantProjection( const AzimuthalEquidistantProjection& p );

  // Accessors
  PROJSYS getProjectionSystem() const throw();

  // Cloning
  Projection* clone() const throw(std::bad_alloc);
  
  // String override
  std::string toString() const throw();
};


// ***************************************************************************
inline PROJSYS AzimuthalEquidistantProjection::getProjectionSystem() 
  const throw()
{
  return AZMEQD;
}

} // namespace ProjLib

#endif
