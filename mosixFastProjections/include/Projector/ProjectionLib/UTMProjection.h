// $Id: UTMProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _UTMPROJECTION_H_
#define _UTMPROJECTION_H_

#include "Projection.h"
#include "ZonedProjection.h"

namespace ProjLib
{

class UTMProjection : public Projection, public ZonedProjection 
{
public:
  // Construction/Destruction
  UTMProjection( long zone, DATUM d, UNIT u, DATUM geoDatum = DEFAULT_DATUM, 
                 UNIT geoUnit = ARC_DEGREES );
  UTMProjection( double latInZone, double lonInZone, DATUM d, UNIT u,
                 DATUM geoDatum = DEFAULT_DATUM, UNIT geoUnit = ARC_DEGREES );
  UTMProjection( const UTMProjection& p );
  virtual ~UTMProjection();

  // Accessors
  PROJSYS getProjectionSystem() const throw(); 

  // Modifiers
  void setZone( long zone ) throw();

  // Projection functions
  bool projectToGeo( double x, double y, double& latitude, double& longitude)
    const throw();
  bool projectFromGeo(double latitude, double longitude, double& x, double& y)
    const throw();

  // String overload
  std::string toString() const throw();

  // Operator overloads
  bool operator==( const Projection& p ) const throw();

  // Cloning
  Projection* clone() const throw(std::bad_alloc);

protected:
  // Overrides
  bool checkCoordinateRange( double latitude, double longitude ) const throw();

  // Data members
  double d_latInZone;
  double d_lonInZone;
};

// ***************************************************************************
inline PROJSYS UTMProjection::getProjectionSystem() const throw()
{
  return UTM;
}

} // namespace ProjLib

#endif
