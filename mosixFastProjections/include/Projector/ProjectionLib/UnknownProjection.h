// $Id: UnknownProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _UNKNOWNPROJECTION_H_
#define _UNKNOWNPROJECTION_H_

#include "Projection.h"

namespace ProjLib
{

class UnknownProjection : public Projection  
{
public:
  UnknownProjection( double left, double bottom, double right, double top );
  UnknownProjection( const UnknownProjection& p );

  // Accessors
  PROJSYS getProjectionSystem() const throw();
  void getExtents( double& left, double& bottom, double& right, 
                   double& top ) const throw();

  // Modifiers
  void setExtents( double left, double bottom, double right, double top )
    throw();

  // Projection functions
  bool projectToGeo( double x, double y, double& latitude, double& longitude )
    const throw();
  bool projectFromGeo(double latitude, double longitude, double& x, double& y)
    const throw();

  // Operator overloads
  bool operator==( const Projection& p ) const throw();

  // toString override
  virtual std::string toString() const throw();

  // clone override
  Projection* clone() const throw(std::bad_alloc);

protected:
  // Data members
  double d_left, d_bottom, d_right, d_top;
};

// ***************************************************************************
inline PROJSYS UnknownProjection::getProjectionSystem() const throw()
{
  return UNKNOWN_PROJSYS;
}

// ***************************************************************************
inline void UnknownProjection::getExtents( double& left, double& bottom,
                                           double& right, double& top ) 
     const throw()
{
  left = d_left;
  bottom = d_bottom;
  right = d_right;
  top = d_top;
}

// ***************************************************************************
inline void UnknownProjection::setExtents( double left, double bottom,
                                           double right, double top ) throw()
{
  d_left = left;
  d_bottom = bottom;
  d_right = right;
  d_top = top;
}

} // namespace ProjLib

#endif

