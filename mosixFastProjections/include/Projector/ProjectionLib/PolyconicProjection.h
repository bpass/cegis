// $Id: PolyconicProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _POLYCONICPROJECTION_H_
#define _POLYCONICPROJECTION_H_

#include "ConicProjection.h"

namespace ProjLib
{

class PolyconicProjection : public ConicProjection  
{
public:
  PolyconicProjection( double sMajor, double sMinor, double centMer, 
                       double originLat, double fe, double fn,
                       DATUM d, UNIT u,
                       DATUM geoDatum = DEFAULT_DATUM,
                       UNIT geoUnit = ARC_DEGREES );
  PolyconicProjection( const PolyconicProjection& p );

  // Accessors
  PROJSYS getProjectionSystem() const throw();

  // Cloning
  Projection* clone() const throw(std::bad_alloc);

  // String override
  std::string toString() const throw();

protected:
  // Overrides
  virtual bool checkCoordinateRange( double latitude, double longitude ) 
    const throw();
};

// ***************************************************************************
inline PROJSYS PolyconicProjection::getProjectionSystem() const throw()
{
  return POLYC;
}

} // namespace ProjLib

#endif
