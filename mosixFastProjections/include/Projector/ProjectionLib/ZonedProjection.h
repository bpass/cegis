// $Id: ZonedProjection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _ZONEDPROJECTION_H_
#define _ZONEDPROJECTION_H_

namespace ProjLib
{

class ZonedProjection  
{
public:
  ZonedProjection( long zone );
  ZonedProjection( const ZonedProjection& p );
  virtual ~ZonedProjection();

  // Accessors
  long getZone() const throw();

  // Modifiers
  virtual void setZone( long zone ) throw();
  /* Sets the zone for the projection.  This function can be
     overriden in derived classes to provide additional error
     checking */
  
protected:
  // Data members
  long d_zone;
};

// ***************************************************************************
inline long ZonedProjection::getZone() const throw()
{
  return d_zone;
}

// ***************************************************************************
inline void ZonedProjection::setZone( long zone ) throw()
{
  d_zone = zone;
}

} // namespace ProjLib

#endif
