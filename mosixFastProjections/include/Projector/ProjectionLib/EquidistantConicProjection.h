// $Id: EquidistantConicProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _EQUIDISTANTCONICPROJECTION_H_
#define _EQUIDISTANTCONICPROJECTION_H_

#include "ConicProjection.h"

namespace ProjLib
{

class EquidistantConicProjection : public ConicProjection  
{
public:
  // Equidistant Conic A Constructor
  EquidistantConicProjection( double stdpar, double sMajor, double sMinor, 
                              double centMer, double originLat,
                              double fe, double fn,
                              DATUM d, UNIT u,
                              DATUM geoDatum = DEFAULT_DATUM,
                              UNIT geoUnit = ARC_DEGREES );
  
  // Equidistant Conic B Constructor
  EquidistantConicProjection( double stdpr1, double stdpr2, 
                              double sMajor, double sMinor,
                              double centMer, double originLat,
                              double fe, double fn, 
                              DATUM d, UNIT u,
                              DATUM geoDatum = DEFAULT_DATUM,
                              UNIT geoUnit = ARC_DEGREES );
  EquidistantConicProjection( const EquidistantConicProjection& p );


  // Accessors
  PROJSYS getProjectionSystem() const throw();
  /* Returns the projection system (Equidistant) */

  double getFirstStandardParallel() const throw();
  /* Returns the latitude of the 1st standard parallel (type B projection).
     The angle is returned in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  double getSecondStandardParallel() const throw();
  /* Returns the latitude of the 2nd standard parallel (type B projection).
     The angle is returned in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  double getStandardParallel() const throw();
  /* Returns the latitude of the standard parallel (type A projection).
     The angle is returned in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  // Modifiers

  void setFirstStandardParallel( double stdpr1 ) throw();
  /* Sets the latitude of the 1st standard parallel (type B projection).
     The angle should be entered in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  void setSecondStandardParallel( double stdpr2 ) throw();
  /* Sets the latitude of the 2nd standard parallel (type B projection).
     The angle should be entered in packed degrees/minutes/seconds
     format (DDDMMMSSS.SS) */

  virtual void setStandardParallel( double stdpar ) throw();
  /* Sets the latitude of the standard parallel (type A projection).
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
inline PROJSYS EquidistantConicProjection::getProjectionSystem() const throw()
{
  return EQUIDC;
}

// ***************************************************************************
inline double EquidistantConicProjection::getFirstStandardParallel() 
     const throw()
{
  return d_projParams[2];
}

// ***************************************************************************
inline double EquidistantConicProjection::getSecondStandardParallel() 
     const throw()
{
  return d_projParams[3];
}

// ***************************************************************************
inline double EquidistantConicProjection::getStandardParallel() const throw()
{
  return d_projParams[2];
}

// ***************************************************************************
inline
void EquidistantConicProjection::setFirstStandardParallel( double stdpr1 )
     throw()
{
  d_projParams[2] = stdpr1;
}

// ***************************************************************************
inline 
void EquidistantConicProjection::setSecondStandardParallel( double stdpr2 )
     throw()
{
  d_projParams[3] = stdpr2;
}

// ***************************************************************************
inline void EquidistantConicProjection::setStandardParallel( double stdpar )
     throw()
{
  d_projParams[2] = stdpar;
}

} // namespace ProjLib

#endif
