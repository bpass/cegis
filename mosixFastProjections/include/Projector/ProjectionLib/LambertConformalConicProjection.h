// $Id: LambertConformalConicProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _LAMBERTCONFORMALCONICPROJECTION_H_
#define _LAMBERTCONFORMALCONICPROJECTION_H_

#include "ConicProjection.h"

namespace ProjLib
{

class LambertConformalConicProjection : public ConicProjection  
{
public:
  // Constructors/Destructors
  LambertConformalConicProjection( double stdpr1, double stdpr2, 
                                   double sMajor, double sMinor,
                                   double centMer, double originLat,
                                   double fe, double fn, DATUM d, UNIT u,
                                   DATUM geoDatum = DEFAULT_DATUM,
                                   UNIT geoUnit = ARC_DEGREES );
  LambertConformalConicProjection( const LambertConformalConicProjection& p );

  // Accessors

  PROJSYS getProjectionSystem() const throw();
  /* Returns the projection system (LAMCC) */

  double getFirstStandardParallel() const throw();
  /* Returns the latitude of the 1st standard parallel.
     The angle is returned in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  double getSecondStandardParallel() const throw();
  /* Returns the latitude of the 2nd standard parallel 
     The angle is returned in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */


  virtual void setFirstStandardParallel( double stdpr1 ) throw();
  /* Sets the latitude of the 1st standard parallel.  Subclasses may
     override this to provide additional error checking. 
     The angle should be entered in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  virtual void setSecondStandardParallel( double stdpr2 ) throw();
  /* Sets the latitude of the 2nd standard parallel.  Subclasses may
     override this to provide additional error checking. 
     The angle should be entered in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  // Operator overloads
  bool operator==( const Projection& p ) const throw();

  // String override
  std::string toString() const throw();

  // Cloning
  Projection* clone() const throw(std::bad_alloc);

protected:
  // Overrides
  bool checkCoordinateRange( double latitude, double longitude ) const throw();
};

// ***************************************************************************
inline PROJSYS LambertConformalConicProjection::getProjectionSystem() 
  const throw()
{
  return LAMCC;
}

// ***************************************************************************
inline double LambertConformalConicProjection::getFirstStandardParallel() 
  const throw()
{
  return d_projParams[2];
}

// ***************************************************************************
inline double LambertConformalConicProjection::getSecondStandardParallel() 
     const throw()
{
  return d_projParams[3];
}

// ***************************************************************************
inline
void LambertConformalConicProjection::setFirstStandardParallel( double stdpr1 )
     throw()
{
  d_projParams[2] = stdpr1;
}

// ***************************************************************************
inline void 
LambertConformalConicProjection::setSecondStandardParallel( double stdpr2 )
     throw()
{
  d_projParams[3] = stdpr2;
}

} // namespace ProjLib

#endif
