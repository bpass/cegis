// $Id: StatePlaneProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _STATEPLANEPROJECTION_H_
#define _STATEPLANEPROJECTION_H_

#include "Projection.h"
#include "ZonedProjection.h"

namespace ProjLib
{

class StatePlaneProjection : public Projection, public ZonedProjection 
{
public:
  // Construction/Destruction
  StatePlaneProjection( long zone, DATUM d, UNIT u, 
                        DATUM geoDatum = DEFAULT_DATUM,
                        UNIT geoUnit = ARC_DEGREES );
  StatePlaneProjection( const StatePlaneProjection& p );
  virtual ~StatePlaneProjection();

  // Accessors
  PROJSYS getProjectionSystem() const throw();

  // Modifiers
  void setZone(long zone) throw();

  // Projection functions
  bool projectToGeo( double x, double y, 
                     double& latitude, double& longitude ) const throw();
  bool projectFromGeo( double latitude, double longitude,
                       double& x, double& y ) const throw();

  // String overload
  std::string toString() const throw();

  // Operator overloads
  bool operator==( const Projection& p ) const throw();

  // Cloning
  Projection* clone() const throw(std::bad_alloc);

  // Parameter file name modifiers
  static void setNAD27ParameterFilename( const std::string& strNAD27 ) throw();
  static void setNAD83ParameterFilename( const std::string& strNAD83 ) throw();

protected:
  // Overrides
  bool checkCoordinateRange( double latitude, double longitude ) const throw();

private:
  // Parameter file names
  static std::string d_strNAD27ParamFilename;
  static std::string d_strNAD83ParamFilename;
};

// ***************************************************************************
inline PROJSYS StatePlaneProjection::getProjectionSystem() const throw()
{
  return SPCS;
}

// ***************************************************************************
inline 
void StatePlaneProjection::setNAD27ParameterFilename(const std::string& strNAD27)
     throw()
{
  d_strNAD27ParamFilename = strNAD27;
}

// ***************************************************************************
inline
void StatePlaneProjection::setNAD83ParameterFilename(const std::string& strNAD83)
     throw()
{
  d_strNAD83ParamFilename = strNAD83;
}

} // namespace ProjLib
#endif
