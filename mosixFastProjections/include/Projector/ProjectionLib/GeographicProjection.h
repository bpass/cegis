// $Id: GeographicProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _GEOGRAPHICPROJECTION_H_
#define _GEOGRAPHICPROJECTION_H_

#include "Projection.h"

namespace ProjLib
{

class GeographicProjection : public Projection  
{
 public:
  GeographicProjection( DATUM d = NAD27, UNIT u = ARC_DEGREES,
                        DATUM geoDaum = DEFAULT_DATUM,
                        UNIT geoUnit = ARC_DEGREES );
  GeographicProjection( const GeographicProjection& p );
  virtual ~GeographicProjection();
    
  // Accessors
  PROJSYS getProjectionSystem() const throw();

  // Projection functions 
  bool projectToGeo( double x, double y, 
                     double& latitude, double& longitude ) const throw();
  bool projectFromGeo( double latitude, double longitude,
                       double& x, double& y ) const throw();
  
  // String override
  std::string toString() const throw();
  
  // Cloning
  Projection* clone() const throw(std::bad_alloc);
};


// ***************************************************************************
inline PROJSYS GeographicProjection::getProjectionSystem() const throw()
{
  return GEO;
}

} // namespace ProjLib
#endif
