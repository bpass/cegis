// $Id: Projection.h,v 1.1 2005/03/11 23:59:10 mschisler Exp $
// Last modified by $Author: mschisler $ on $Date: 2005/03/11 23:59:10 $

#ifndef _PROJECTION_H_
#define _PROJECTION_H_

#include <new>
#include <string>
#include "ProjectionTypes.h"
#include "ProjectionException.h"

namespace ProjLib
{

// Some defines to make gctpc happier
//typedef long (*ForwardTransFunc)(double, double, double*, double*);
//typedef long (*InverseTransFunc)(double, double, double*, double*);
typedef long (*ForwardTransFunc)();
typedef long (*InverseTransFunc)();

class Projection  
{
 public:
  // Constructors/Destructors
  Projection( DATUM d, UNIT u, DATUM geoDatum = DEFAULT_DATUM,
              UNIT geoUnit = ARC_DEGREES );
  Projection( const Projection& p );
  virtual ~Projection();

  // Accessors
  DATUM getDatum() const throw();
  /* Returns the datum for the projection coordinates in the projection
     functions */

  DATUM getGeoDatum() const throw();
  /* Returns the datum for the lat-long coordinates in the projection
     functions */
  
  UNIT getUnit() const throw();
  /* Returns the unit for the projection coordinates given in the projection
     functions */
  
  UNIT getGeoUnit() const throw();
  /* Returns the unit for lat-long coordinates in the projection functions */

  virtual PROJSYS getProjectionSystem() const throw() = 0;
  /* Returns the PROJSYS code.  This must be overriden in derived classes */

  virtual std::string toString() const throw();
  /* Returns a string representation of the projection */

  // Modifiers
  void setDatum( DATUM d ) throw();
  /* Sets the datum for the projection */
  
  void setGeoDatum( DATUM d ) throw();
  /* Sets the datum which the lat-long coordinates in the projection functions
     are given in */
  
  void setUnit( UNIT u ) throw();
  /* Sets the unit for coordinates in the projection */
  
  void setGeoUnit( UNIT u ) throw();
  /* Sets the unit for the lat-long coordinates in the projection functions
     are given in */

  // Projection functions
  virtual bool projectToGeo( double x, double y, 
                             double& latitude, double& longitude ) 
    const throw() = 0;
  /* Projects the point (x,y) which is in the projection to lat-long.  This
     function must be overriden in derived classes. If the coordinates cannot
     be projected, false should be returned. */
  
  virtual bool projectFromGeo( double latitude, double longitude,
                               double& x, double& y ) 
    const throw() = 0;
  /* Projects the latitude and longitude given to the projection.  This
     function must be overriden in derived classes.  If the coordinates cannot
     be projected, false should be returned. */

  // Operator overloads
  virtual bool operator==( const Projection& p ) const throw();
  bool         operator!=( const Projection& p ) const throw();

  // Cloning functions
  virtual Projection* clone() const throw(std::bad_alloc) =0;
  /* Creates a copy of the projection and returns a pointer to it.  It is the
     callers responsibility to destroy the pointer when done with it.  This
     function must be overriden in derived classes */
    
  // Helpful functions for getting the string representations of units and
  // datums
  static std::string datumToString( DATUM d )                         throw();
  static std::string unitToString( UNIT u )                           throw();
  static double      packedDMSToDouble( double packedDMS )            throw();
  static std::string packedDMSToString( double packedDMS, bool bLat ) throw();

protected:
  bool convertDatum( double& latitude, double& longitude, DATUM from, 
                     DATUM to ) const throw();
  virtual bool checkCoordinateRange( double latitude, double longitude ) const
    throw();
  /* Return true if the given coordinate (in arc degrees) is within the range
     allowed for this projection, false if it's not */

  DATUM d_datum;
  DATUM d_geoDatum;
  UNIT  d_unit;
  UNIT  d_geoUnit;
};

// ***************************************************************************
inline DATUM Projection::getDatum() const throw()
{
  return d_datum;
}

// ***************************************************************************
inline DATUM Projection::getGeoDatum() const throw()
{
  return d_geoDatum;
}

// ***************************************************************************
inline UNIT Projection::getUnit() const throw()
{
  return d_unit;
}

// ***************************************************************************
inline UNIT Projection::getGeoUnit() const throw()
{
  return d_geoUnit;
}

// ***************************************************************************
inline void Projection::setDatum( DATUM d ) throw()
{
  d_datum = d;
}

// ***************************************************************************
inline void Projection::setGeoDatum( DATUM d ) throw()
{
  d_geoDatum = d;
}

// ***************************************************************************
inline void Projection::setUnit( UNIT u ) throw()
{
  d_unit = u;
}

// ***************************************************************************
inline void Projection::setGeoUnit( UNIT u ) throw()
{
  d_geoUnit = u;
}

// ***************************************************************************
inline bool Projection::checkCoordinateRange( double latitude,
                                              double longitude ) const throw()
{
  return ( ( latitude <= 90.0 && latitude >= -90.0 ) &&
	   ( longitude <= 180.0 && longitude >= -180.0 ) );
}

}

#endif

